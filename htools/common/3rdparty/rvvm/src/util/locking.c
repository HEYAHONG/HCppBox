/*
locking.c - Fast hybrid reader/writer lock
Copyright (C) 2021  LekKit <github.com/LekKit>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#if !defined(USE_THREAD_EMU)

#include <util/locking.h>
#include <util/threading.h>

#include "rvtimer.h"
#include "stacktrace.h"
#include "utils.h"

PUSH_OPTIMIZATION_SIZE

// Maximum allowed bounded locking time, reports a deadlock upon expiration
#define DEADLOCK_NS      10000000000ULL

#define LOCK_QUIESCENT   0x00000000UL // No one holds the lock
#define LOCK_HAS_WRITER  0x00000001UL // Writer holds the lock
#define LOCK_HAS_READERS 0x7FFFFFFEUL // Reader(s) hold the lock
#define LOCK_HAS_WAITERS 0x80000000UL // Has waiters

static slow_path void lock_debug_report(rvvm_lock_t* lock, bool fatal)
{
    UNUSED(lock);
#if defined(USE_LOCK_DEBUG)
    rvvm_warn("The lock was last exclusively held at %s", lock->location);
#endif
#if defined(RVVM_VERSION)
    rvvm_warn("Version: RVVM " RVVM_VERSION);
#endif
    if (fatal) {
        rvvm_fatal("Locking issue detected!");
    } else {
        stacktrace_print();
    }
}

static inline bool lock_has_writer(uint32_t flag)
{
    return !!(flag & LOCK_HAS_WRITER);
}

static inline bool lock_has_readers(uint32_t flag)
{
    return !!(flag & LOCK_HAS_READERS);
}

static inline bool lock_has_waiters(uint32_t flag)
{
    return !!(flag & LOCK_HAS_WAITERS);
}

static inline bool lock_possibly_available(uint32_t flag, bool writer)
{
    if (writer) {
        // No other writers, readers, or waiters
        return !flag;
    } else {
        // No writers or waiters
        return !(flag & (LOCK_HAS_WRITER | LOCK_HAS_WAITERS));
    }
}

static inline bool lock_try_claim(rvvm_lock_t* lock, const char* location, bool writer, uint32_t waits)
{
    if (writer) {
        uint32_t exp = LOCK_QUIESCENT;
        uint32_t val = LOCK_HAS_WRITER | waits;
        if (likely(atomic_cas_uint32_try(&lock->flag, exp, val, false, ATOMIC_ACQUIRE))) {
            LOCK_MARK_LOCATION(lock, location);
            return true;
        }
        return false;
    } else {
        uint32_t prev = atomic_load_uint32_relax(&lock->flag);
        uint32_t new;
        // TODO: Chain-wake readers
        if (lock_has_waiters(prev)) {
            rvvm_futex_wake(&lock->flag, -1);
        }
        do {
            if (unlikely(!lock_possibly_available(prev, false))) {
                return false;
            }
            new = (prev + 2) | waits;
        } while (!atomic_cas_uint32_ex(&lock->flag, &prev, new, true, ATOMIC_ACQUIRE, ATOMIC_RELAXED));
        return true;
    }
}

static bool lock_try_wait_user(rvvm_lock_t* lock, const char* location, bool writer)
{
    // Spin on a lock in userspace for a few times before using a heavyweight kernel futex
    for (size_t i = 0; i < 40; ++i) {
        uint32_t flag = atomic_load_uint32_relax(&lock->flag);
        if (lock_possibly_available(flag, writer)) {
            if (lock_try_claim(lock, location, writer, 0)) {
                // Succesfully claimed the lock
                return true;
            } else {
                // Contention is going on, fallback to kernel wait
                return false;
            }
        }
        rvvm_cpu_relax();
    }
    return false;
}

static void rvvm_lock_wait_raw(rvvm_lock_t* lock, const char* location, uint32_t flags, bool writer)
{
    rvtimer_t deadlock_timer = {0};
    bool      reset_timer    = true;

    if (lock_try_wait_user(lock, location, writer)) {
        return;
    }

    while (true) {
        uint32_t flag = atomic_load_uint32_relax(&lock->flag);
        if (lock_possibly_available(flag, writer)) {
            if (lock_try_claim(lock, location, writer, LOCK_HAS_WAITERS)) {
                // Succesfully claimed the lock, mark it for wakeup
                return;
            }

            // Contention is going on, retry
            reset_timer = true;
            rvvm_sched_yield();
        } else if (flags & LOCK_WAIT_BUSY_LOOP) {
            rvvm_sched_yield();
        } else {
            if (reset_timer) {
                reset_timer = false;
                rvtimer_init(&deadlock_timer, 1000000000ULL);
            }

            // Indicate that we're waiting on this lock
            if (!lock_has_waiters(flag)) {
                if (atomic_cas_uint32(&lock->flag, flag, flag | LOCK_HAS_WAITERS)) {
                    flag |= LOCK_HAS_WAITERS;
                }
            }

            // Wait on a futex if we succesfully marked ourselves as a waiter
            if (lock_has_waiters(flag)) {
                if (rvvm_futex_wait(&lock->flag, flag, DEADLOCK_NS)) {
                    // Reset deadlock timer upon noticing any forward progress
                    reset_timer = true;
                }
            }

            // Check for deadlock
            if (rvtimer_get(&deadlock_timer) >= DEADLOCK_NS) {
                reset_timer = true;
                if (!location) {
                    location = "[unknown]";
                }
                if (flags & LOCK_WAIT_FOREVER) {
                    rvvm_debug("Still holding %slock at %s", writer ? "" : "reader ", location);
                } else {
                    rvvm_warn("Possible %sdeadlock at %s", writer ? "" : "reader ", location);
                    lock_debug_report(lock, false);
                }
            }
        }
    }
}

slow_path void rvvm_lock_wait(rvvm_lock_t* lock, const char* location, uint32_t flags)
{
    rvvm_lock_wait_raw(lock, location, flags, true);
}

slow_path void rvvm_read_lock_wait(rvvm_lock_t* lock, const char* location, uint32_t flags)
{
    rvvm_lock_wait_raw(lock, location, flags, false);
}

slow_path void rvvm_lock_wake(rvvm_lock_t* lock, uint32_t prev)
{
    if (lock_has_readers(prev)) {
        rvvm_warn("Mismatched unlock of a reader lock");
        lock_debug_report(lock, true);
    } else if (!lock_has_writer(prev)) {
        rvvm_warn("Unlock of a non-locked writer lock");
        lock_debug_report(lock, true);
    } else if (lock_has_waiters(prev)) {
        // Wake a reader/writer
        rvvm_futex_wake(&lock->flag, 1);
    }
}

slow_path void rvvm_read_lock_wake(rvvm_lock_t* lock, uint32_t prev)
{
    if (lock_has_writer(prev)) {
        rvvm_warn("Mismatched unlock of a writer lock");
        lock_debug_report(lock, true);
    } else if (!lock_has_readers(prev)) {
        rvvm_warn("Unlock of a non-locked reader lock");
        lock_debug_report(lock, true);
    } else if (lock_has_waiters(prev)) {
        // Wake a writer
        atomic_and_uint32(&lock->flag, ~0x80000000U);
        rvvm_futex_wake(&lock->flag, 1);
    }
}

POP_OPTIMIZATION_SIZE

#endif
