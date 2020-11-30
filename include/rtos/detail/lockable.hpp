#pragma once

#include <rtos/os_types.hpp>
#include <rtos/os_kernel.hpp>


namespace os::rtos {


class lockable : private non_copyable<lockable>, non_moveable<lockable> {
public:
    enum class type {
        semaphore,          ///< Signaling flag type lock appropriate for thread synchronization and usage in ISRs.
        counting_semaphore, ///< Counting version of a regular semaphore appropriate for tracking available resource
                            ///< usage.  Appropriate for usage in ISRs.
        mutex, ///< Mutual exclusion primitive appropriate for protecting data. NOTE: Not appropriate for use in ISRs.
        recursive_mutex, ///< Recursive style mutex appropriate for ensuring a single thread can repeatedly 'acquire' a
                         ///< given resource. NOTE: Not usable in ISRs.
    };

public:
    lockable(const type type, const size_t maxCount = 1, const size_t initialCount = 1);
    ~lockable() noexcept;


    /**
     * \brief The calling thread will attempt to acquire the lock within the specified timeout.
     * This is the same as attempting to decrement the count for a counting semaphore, which will only
     * succeed if the count is greater than zero. By default, the timeout is specified to be the maximum possible in the
     * system.
     */
    os_status_t lock(const kernel::clock::duration_u32& timeout = kernel::wait_for_u32_max) noexcept;

    /**
     * \brief The calling thread will release the lock if it currently holds it.
     * This is the same as increasing the count for a counting semaphore.
     * If the lock is currently free or at its maximum count, this function will have no effect.
     */
    void unlock() noexcept;

    bool operator==(const lockable& other) const noexcept { return (m_handle == other.m_handle); }
    bool operator==(const lockable& other) const noexcept { return !(*this == other); }


protected:
    using handle = void*;

    type   m_type;
    handle m_handle;
};


} // namespace os::rtos
