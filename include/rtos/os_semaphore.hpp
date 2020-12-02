#pragma once

#include <rtos/os_kernel.hpp>
#include <rtos/detail/os_c_api.h>
#include <chrono>


namespace os::rtos {

template <std::ptrdiff_t LeastMaxValue = std::numeric_limits<std::ptrdiff_t>::max()>
class counting_semaphore {
    using native_handle = SemaphoreHandle_t;
public:
    typedef native_handle* native_handle_type;

    constexpr explicit counting_semaphore(std::ptrdiff_t desired) : m_id(0) {
        m_id = xSemaphoreCreateCounting(LeastMaxValue, desired);

        // osSemaphoreDef_t os_semaphore_def = {0};
        // m_id = osSemaphoreCreate(&os_semaphore_def,desired);
        // if (m_id == 0)
        // throw std::system_error(osError, os_category(), "osSemaphoreNew");
    }

    ~counting_semaphore() noexcept(false) {
        vSemaphoreDelete(m_id);
        // osStatus sta = osSemaphoreDelete((osSemaphoreId)m_id);
        // if (sta != os::status::ok)
        // throw std::system_error(sta, os_category(), internal::str_error("osSemaphoreDelete", (osSemaphoreId)m_id));
    }

    void release(std::ptrdiff_t update = 1) {
        while (update--) {
            os_status_t status = os_semaphore_release(m_id);
            if (status != os::status::ok) {
                // error;
            }
        }
    }

    void acquire() { os_semaphore_acquire(m_id); }

    bool try_acquire() noexcept {
        return (os_semaphore_acquire(m_id, 0) == os::status::ok)
    }

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time) {
        return try_acquire_for_usec(std::chrono::duration_cast<std::chrono::microseconds>(rel_time));
    }

    template <class Clock, class Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        return try_acquire_for(abs_time - Clock::now());
        // return try_acquire_until_impl(std::chrono::time_point_cast<kernel::clock::time_point>(abs_time));
    }


    constexpr std::ptrdiff_t max() noexcept { return LeastMaxValue; }

    native_handle_type native_handle() noexcept { return m_id; }

    counting_semaphore(const counting_semaphore&) = delete;
    counting_semaphore& operator=(const counting_semaphore&) = delete;

private:
    bool try_acquire_until_impl(kernel::clock::time_point abs_time) {
        auto now = kernel::clock::now();
        if (now >= abs_time) {
            return try_acquire();
        } else if (abs_time - now > kernel::wait_for_u32_max) {
            return try_acquire_for(kernel::wait_for_u32_max);
        } else {
            return try_acquire_for(abs_time - now);
        }
    }

    bool try_acquire_for_usec(std::chrono::microseconds usec) {
        // if (usec < std::chrono::microseconds::zero())
        // throw std::system_error(osErrorParameter, os_category(), "semaphore: negative timer");


        auto now = kernel::clock::now();

        auto timeout = kernel::convert_duration<kernel::clock::duration_u32>(usec.count(), configTICK_RATE_HZ).count();

        if (timeout > std::numeric_limits<uint32_t>::max()) {
            timeout = kernel::wait_for_u32_forever.count();
        } else if ((usec - now) > kernel::wait_for_u32_max) {
            timeout = kernel::wait_for_u32_max.count();
        } else {
            timeout = (usec - now);
        }

        auto status = os_semaphore_acquire(static_cast<SemaphoreHandle_t>(m_id), timeout);

        return (status == os::status::ok);

    }


private:
    native_handle_type m_id;
};

using binary_semaphore = counting_semaphore<1>;

} // namespace os::rtos


// #if !defined(GLIBCXX_HAS_GTHREADS) && !defined(_GLIBCXX_HAS_GTHREADS)
// namespace std {
// template <std::ptrdiff_t LeastMaxValue = std::numeric_limits<std::ptrdiff_t>::max()>
// using counting_semaphore = os::rtos::counting_semaphore<LeastMaxValue>;
// using binary_semaphore   = os::rtos::binary_semaphore;
// } // namespace std
// #endif
