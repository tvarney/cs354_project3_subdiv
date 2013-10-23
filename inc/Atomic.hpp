
#ifndef CS354_GENERIC_ATOMIC_HPP
#define CS354_GENERIC_ATOMIC_HPP

/* __GNUC__ captures gcc, icc and clang/llvm. All of them define the same
 * atomic builtins.
 * These macros should not be used to examine the contents of the value being
 * incremented or decremented, as the function they resolve to may not return
 * the same thing between systems.
 */
#ifdef __GNUC__
# define AtomicIncrement(ptr) __sync_fetch_and_add(ptr, 1)
# define AtomicDecrement(ptr) __sync_fetch_and_sub(ptr, 1)
#elif defined(_WIN32)
# define AtomicIncrement(ptr) InterlockedIncrement(ptr)
# define AtomicDecrement(ptr) InterlockedDecrement(ptr)
#elif defined(__APPLE__)
# include <libkern/OSAtomic.h>
# define AtomicIncrement(ptr) \
    OSAtomicIncrement32Barrier((volatile int32_t *)ptr)
# define AtomicDecrement(ptr) \
    OSAtomicDecrement32Barrier((volatile int32_t *)ptr);
#else
# error "Can't define atomic operations"
#endif

#endif
