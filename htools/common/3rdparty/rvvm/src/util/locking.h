/*
locking.h - Fast hybrid reader/writer lock
Copyright (C) 2021  LekKit <github.com/LekKit>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef RVVM_UTIL_LOCKING_H
#define RVVM_UTIL_LOCKING_H

#include <util/atomics.h>

#if !defined(USE_THREAD_EMU)

/*
 * Internal implementation details
 *
 * 0x00000000: Quiescent state (No one holds the lock)
 * 0x00000001: Writer holds the lock
 * 0x7FFFFFFE: Reader count
 * 0x80000000: There are waiters
 */
typedef struct {
    uint32_t flag;
#if defined(USE_LOCK_DEBUG)
    const char* location;
#endif
} rvvm_lock_t;

#if defined(USE_LOCK_DEBUG)

#define LOCK_DEBUG_LOCATION SOURCE_LINE
#define LOCK_MARK_LOCATION(lock, location)                                                                             \
    do {                                                                                                               \
        lock->location = location;                                                                                     \
    } while (0)

#else

#define LOCK_DEBUG_LOCATION NULL
#define LOCK_MARK_LOCATION(lock, location)                                                                             \
    do {                                                                                                               \
        UNUSED(lock && location);                                                                                      \
    } while (0)

#endif

#define LOCK_WAIT_FOREVER   0x01
#define LOCK_WAIT_BUSY_LOOP 0x02

slow_path void rvvm_lock_wait(rvvm_lock_t* lock, const char* location, uint32_t flags);
slow_path void rvvm_lock_wake(rvvm_lock_t* lock, uint32_t prev);

slow_path void rvvm_read_lock_wait(rvvm_lock_t* lock, const char* location, uint32_t flags);
slow_path void rvvm_read_lock_wake(rvvm_lock_t* lock, uint32_t prev);

static forceinline bool rvvm_try_lock_raw(rvvm_lock_t* lock, const char* location)
{
    if (likely(atomic_cas_uint32_try(&lock->flag, 0x00, 0x01, false, ATOMIC_ACQUIRE))) {
        LOCK_MARK_LOCATION(lock, location);
        return true;
    }
    return false;
}

static forceinline void rvvm_lock_raw(rvvm_lock_t* lock, const char* location, uint32_t flags)
{
    // Use weak CAS in fast path
    if (likely(atomic_cas_uint32_try(&lock->flag, 0x00, 0x01, true, ATOMIC_ACQUIRE))) {
        LOCK_MARK_LOCATION(lock, location);
    } else {
        rvvm_lock_wait(lock, location, flags);
    }
}

static forceinline void rvvm_unlock_raw(rvvm_lock_t* lock)
{
    uint32_t prev = atomic_swap_uint32_ex(&lock->flag, 0x00, ATOMIC_RELEASE);
    if (unlikely(prev != 0x01)) {
        // Waiters are present, or invalid usage detected (Not locked / Locked as a reader)
        rvvm_lock_wake(lock, prev);
    }
}

static forceinline bool rvvm_try_read_lock_raw(rvvm_lock_t* lock)
{
    uint32_t prev = atomic_load_uint32_relax(&lock->flag);
    do {
        if (unlikely(prev & 0x80000001UL)) {
            // Writer owns the lock, writer waiters are present or too much readers (sic!)
            return false;
        }
    } while (!atomic_cas_uint32_ex(&lock->flag, &prev, prev + 0x02, true, ATOMIC_ACQUIRE, ATOMIC_RELAXED));
    return true;
}

static forceinline void rvvm_read_lock_raw(rvvm_lock_t* lock, const char* location, uint32_t flags)
{
    if (unlikely(!rvvm_try_read_lock_raw(lock))) {
        rvvm_read_lock_wait(lock, location, flags);
    }
}

static forceinline void rvvm_read_unlock_raw(rvvm_lock_t* lock)
{
    uint32_t prev = atomic_sub_uint32_ex(&lock->flag, 0x02, ATOMIC_RELEASE);
    if (unlikely(((int32_t)prev) < 0x02)) {
        // Waiters are present, or invalid usage detected (Not locked / Locked as a writer)
        rvvm_read_lock_wake(lock, prev);
    }
}

/*
 * Lock initializers
 */

#define RVVM_LOCK_INIT ZERO_INIT

static inline void rvvm_lock_init(rvvm_lock_t* lock)
{
    lock->flag = 0;
#if defined(USE_LOCK_DEBUG)
    lock->location = NULL;
#endif
}

/*
 * Writer locking
 */

// Try to claim lock
#define rvvm_try_lock(lock)              rvvm_try_lock_raw(lock, LOCK_DEBUG_LOCATION)

// Lock around small, bounded critical section, may report deadlocks
#define rvvm_lock(lock)                  rvvm_lock_raw(lock, LOCK_DEBUG_LOCATION, 0)

// Lock around slow operation, wait indefinitely
#define rvvm_lock_slow(lock)             rvvm_lock_raw(lock, LOCK_DEBUG_LOCATION, LOCK_WAIT_FOREVER)

// Release lock
#define rvvm_unlock(lock)                rvvm_unlock_raw(lock)

/*
 * Busy-wait spinlock, for places where sleeping is not allowed
 */

// Lock around small, bounded critical section
#define rvvm_spin_lock(lock)             rvvm_lock_raw(lock, LOCK_DEBUG_LOCATION, LOCK_WAIT_BUSY_LOOP)

// Release lock
#define rvvm_spin_unlock(lock)           atomic_store_uint32_ex(&(lock)->flag, 0x00, ATOMIC_RELEASE)

/*
 * Reader locking
 */

// Try to claim reader lock
#define rvvm_try_read_lock(lock)         rvvm_try_read_lock_raw(lock)

// Lock around small, bounded reader section, may report deadlocks
#define rvvm_read_lock(lock)             rvvm_read_lock_raw(lock, LOCK_DEBUG_LOCATION, 0)

// Lock around slow reader operation, wait indefinitely
#define rvvm_read_lock_slow(lock)        rvvm_read_lock_raw(lock, LOCK_DEBUG_LOCATION, LOCK_WAIT_FOREVER)

// Release reader lock
#define rvvm_read_unlock(lock)           rvvm_read_unlock_raw(lock)

/*
 * Scoped locking helpers, may be exited via break
 *
 * rvvm_scoped_lock(&lock) {
 *     do_something_under_lock();
 *
 *     if (need_exit_under_lock()) {
 *         break;
 *     }
 *
 *     do_something_under_lock();
 * }
 *
 * rvvm_scoped_try_lock(&lock) {
 *     do_something_under_lock();
 * } else {
 *     do_something_if_locking_failed();
 * }
 */

#define rvvm_scoped_lock(lock)           SCOPED_HELPER (rvvm_lock(lock), rvvm_unlock(lock))
#define rvvm_scoped_try_lock(lock)       POST_COND (rvvm_try_lock(lock), rvvm_unlock(lock))
#define rvvm_scoped_lock_slow(lock)      SCOPED_HELPER (rvvm_lock_slow(lock), rvvm_unlock(lock))

#define rvvm_scoped_read_lock(lock)      SCOPED_HELPER (rvvm_read_lock(lock), rvvm_read_unlock(lock))
#define rvvm_scoped_try_read_lock(lock)  POST_COND (rvvm_try_read_lock(lock), rvvm_read_unlock(lock))
#define rvvm_scoped_read_lock_slow(lock) SCOPED_HELPER (rvvm_read_lock_slow(lock), rvvm_read_unlock(lock))

#define rvvm_scoped_spin_lock(lock)      SCOPED_HELPER (rvvm_spin_lock(lock), rvvm_spin_unlock(lock))

#else

/*
 * Make all locking a no-op without threads
 */

typedef struct {
} rvvm_lock_t;

#define RVVM_LOCK_INIT                   ZERO_INIT

#define rvvm_lock_init(lock)             UNUSED(lock)

#define rvvm_try_lock(lock)              (UNUSED(lock), 1)
#define rvvm_lock(lock)                  UNUSED(lock)
#define rvvm_lock_slow(lock)             UNUSED(lock)
#define rvvm_unlock(lock)                UNUSED(lock)

#define rvvm_spin_lock(lock)             UNUSED(lock)
#define rvvm_spin_unlock(lock)           UNUSED(lock)

#define rvvm_try_read_lock(lock)         UNUSED(lock)
#define rvvm_read_lock(lock)             UNUSED(lock)
#define rvvm_read_lock_slow(lock)        UNUSED(lock)
#define rvvm_read_unlock(lock)           UNUSED(lock)

#define rvvm_scoped_lock(lock)           SCOPED_HELPER ((void)(lock), (void)(lock))
#define rvvm_scoped_try_lock(lock)       rvvm_scoped_lock(lock)
#define rvvm_scoped_lock_slow(lock)      rvvm_scoped_lock(lock)

#define rvvm_scoped_read_lock(lock)      rvvm_scoped_lock(lock)
#define rvvm_scoped_try_read_lock(lock)  rvvm_scoped_lock(lock)
#define rvvm_scoped_read_lock_slow(lock) rvvm_scoped_lock(lock)

#define rvvm_scoped_spin_lock(lock)      rvvm_scoped_lock(lock)

#endif

#endif
