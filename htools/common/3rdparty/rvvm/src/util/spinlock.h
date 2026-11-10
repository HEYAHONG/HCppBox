
#ifndef LEGACY_SPINLOCK_H
#define LEGACY_SPINLOCK_H

/*
 * TODO: Use <util/locking.h>
 */

#include <util/locking.h>

typedef rvvm_lock_t spinlock_t;

#define SPINLOCK_INIT                    RVVM_LOCK_INIT
#define spin_init(lock)                  rvvm_lock_init(lock)

#define spin_try_lock(lock)              rvvm_try_lock(lock)
#define spin_lock(lock)                  rvvm_lock(lock)
#define spin_lock_slow(lock)             rvvm_lock_slow(lock)
#define spin_unlock(lock)                rvvm_unlock(lock)

#define spin_lock_busy_loop(lock)        rvvm_spin_lock(lock)
#define spin_unlock_busy_loop(lock)      rvvm_spin_unlock(lock)

#define spin_try_read_lock(lock)         rvvm_try_read_lock(lock)
#define spin_read_lock(lock)             rvvm_read_lock(lock)
#define spin_read_lock_slow(lock)        rvvm_read_lock_slow(lock)
#define spin_read_unlock(lock)           rvvm_read_unlock(lock)

#define scoped_spin_lock(lock)           rvvm_scoped_lock(lock)
#define scoped_spin_try_lock(lock)       rvvm_scoped_try_lock(lock)
#define scoped_spin_lock_slow(lock)      rvvm_scoped_lock_slow(lock)

#define scoped_spin_read_lock(lock)      rvvm_scoped_read_lock(lock)
#define scoped_spin_try_read_lock(lock)  rvvm_scoped_try_read_lock(lock)
#define scoped_spin_read_lock_slow(lock) rvvm_scoped_read_lock_slow(lock)

#endif
