#pragma once

#include <rtos/os_kernel.hpp>
// #include <rtos/os_semaphore.hpp>
#include <rtos/os_mutex.hpp>


#include <string>

namespace os::rtos {


class thread : private non_copyable<thread> {
public:
    enum class priority : int8_t {
        idle         = 0,
        low          = 1,
        below_normal = 2,
        normal       = 3,
        above_normal = 4,
        high         = 5,
        realtime     = 6,

        error = -1,
    };

    enum class state : int8_t {
        inactive   = 0,
        ready      = 1,
        running    = 2,
        blocked    = 3,
        terminated = 4,
        error      = -1,
    };

    typedef TaskHandle_t id;

public:
    thread(const std::string& name,
           thread::priority   prio       = priority::normal,
           size_t             stack_size = configMINIMAL_STACK_SIZE)
        : thread(name.c_str(), prio, stack_size) {}

    thread(const char*      name       = nullptr,
           thread::priority prio       = priority::normal,
           size_t           stack_size = configMINIMAL_STACK_SIZE);

    virtual ~thread();

    virtual void init() {}
    virtual void run() = 0;

    status start();

    status terminate();

    status set_priority(priority prio);

    priority get_priority() const;

    uint32_t flags_set(uint32_t flags);

    state get_state() const;

    size_t stack_size() const;

    size_t free_stack() const;

    size_t used_stack() const;

    size_t max_stack() const;

    const char* get_name() const { return m_name; }

    thread::id get_id() const;

private:
    thread::id m_id;

    mutable mutex m_mutex;
    const char*   m_name;
    priority      m_priority;
    size_t        m_stack_size;

    bool m_finished;

private:
    static void handler(void* _thread) {
        thread* handle = static_cast<thread*>(_thread);
        handle->init();
        handle->run();
        handle->m_mutex.lock();
        handle->m_id       = nullptr;
        handle->m_finished = true;
        // rtos will release the mutex automatically
    }
};

namespace this_thread {

/**
 * \brief Clears the specified Thread Flags of the currently running thread.
 * \param   flags  specifies the flags of the thread that should be cleared.
 * \return  thread flags before clearing.
 * \note You cannot call this function from ISR context.
 */
uint32_t flags_clear(uint32_t flags);

/**
 * \brief Returns the Thread Flags currently set for the currently running thread.
 * \return  current thread flags or 0 if not in a valid thread.
 * \note You cannot call this function from ISR context.
 */
uint32_t flags_get();

/**
 * \brief Wait for all of the specified Thread Flags to become signaled for the current thread.
 * \param   flags     specifies the flags to wait for
 * \param   clear     whether to clear the specified flags after waiting for them. (default: true)
 * \return  actual thread flags before clearing, which will satisfy the wait
 * \note You cannot call this function from ISR context.
 */
uint32_t flags_wait_all(uint32_t flags, bool clear = true);

/**
 * \brief Wait for any of the specified Thread Flags to become signaled for the current thread.
 * \param   flags     specifies the flags to wait for
 * \param   clear     whether to clear the specified flags after waiting for them. (default: true)
 * \return  actual thread flags before clearing, which will satisfy the wait
 * \note You cannot call this function from ISR context.
 */
uint32_t flags_wait_any(uint32_t flags, bool clear = true);


/**
 * \brief Wait for all of the specified Thread Flags to become signaled for the current thread.
 * \param   flags     specifies the flags to wait for
 * \param   rel_time  timeout value.
 * \param   clear     whether to clear the specified flags after waiting for them. (default: true)
 * \return  actual thread flags before clearing, which may not satisfy the wait
 * \note You cannot call this function from ISR context.

 */
uint32_t flags_wait_all_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear = true);

/**
 * \brief Wait for all of the specified Thread Flags to become signaled for the current thread.
 * \param   flags     specifies the flags to wait for
 * \param   abs_time  absolute timeout time, referenced to Kernel::Clock
 * \param   clear     whether to clear the specified flags after waiting for them. (default: true)
 * \return  actual thread flags before clearing, which may not satisfy the wait
 * \note You cannot call this function from ISR context.
 * \note the underlying RTOS may have a limit to the maximum wait time
 *      due to internal 32-bit computations, but this is guaranteed to work if the
 *      wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
 *      the wait will time out earlier than specified.
 */
uint32_t flags_wait_all_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear = true);

/**
 * \brief Wait for any of the specified Thread Flags to become signaled for the current thread.
 * \param   flags     specifies the flags to wait for
 * \param   rel_time  timeout value.
 * \param   clear     whether to clear the specified flags after waiting for them. (default: true)
 * \return  actual thread flags before clearing, which may not satisfy the wait
 */
uint32_t flags_wait_any_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear = true);

/**
 * \brief Wait for any of the specified Thread Flags to become signaled for the current thread.
 *
 * \param flags     specifies the flags to wait for
 * \param abs_time  absolute timeout time, referenced to Kernel::Clock
 * \param clear     whether to clear the specified flags after waiting for them. (default: true)
 * \return actual thread flags before clearing, which may not satisfy the wait
 *
 * \note  the underlying RTOS may have a limit to the maximum wait time
 *        due to internal 32-bit computations, but this is guaranteed to work if the
 *        wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
 *        the wait will time out earlier than specified.
 */
uint32_t flags_wait_any_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear = true);

void sleep_for(kernel::clock::duration_u32 rel_time);

void sleep_until(kernel::clock::time_point abs_time);

void yield();

thread::id get_id();

const char* get_name();

} // namespace this_thread


} // namespace os::rtos

using os_priority     = ::os::rtos::thread::priority;
using os_thread_state = ::os::rtos::thread::state;
using os_thread_id    = ::os::rtos::thread::id;
