#pragma once

#include <rtos/os_kernel.hpp>
#include <rtos/detail/os_c_api.h>
#include <chrono>
#include <memory>

namespace os::rtos {

class semaphore : private non_copyable<semaphore> {
public:
    semaphore(int32_t count = 0) : semaphore(count, 0xffff) {}

    semaphore(int32_t count, uint16_t max_count);

    ~semaphore();

    void acquire();

    bool try_acquire(int32_t timeout = 0);

    bool try_acquire_for(kernel::clock::duration_u32 rel_time);

    bool try_acquire_until(kernel::clock::time_point abs_time);

    os_status_t release();

private:
    SemaphoreHandle_t m_handle;
};


namespace experimental {

template <std::ptrdiff_t LeastMaxValue = std::numeric_limits<std::ptrdiff_t>::max()>
class counting_semaphore {
    typedef os::rtos::semaphore native_type;

public:
    typedef native_type* native_handle_type;

    constexpr explicit counting_semaphore(std::ptrdiff_t desired) : m_id(nullptr) {
        m_id = ::new native_type(desired, LeastMaxValue);
    }

    ~counting_semaphore() noexcept(false) {
        if (m_id != nullptr) {
            delete m_id;
            m_id = nullptr;
        }
    }

    void release(std::ptrdiff_t update = 1) {
        while (update--) {
            os_status_t status = native_handle()->release();
            if (status != os::status::ok) {
                // error;
            }
        }
    }

    void acquire() { native_handle()->acquire(); }

    bool try_acquire() noexcept { return native_handle()->try_acquire(); }


    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time) {
        return native_handle()->try_acquire_for(
            std::chrono::duration_cast<os::kernel::clock::duration_u32>(rel_time)
        );
    }

    template <class Clock, class Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        return try_acquire_for(abs_time - Clock::now());
    }


    constexpr std::ptrdiff_t max() noexcept { return LeastMaxValue; }

    native_handle_type native_handle() noexcept { return m_id; }

    counting_semaphore(const counting_semaphore&) = delete;
    counting_semaphore& operator=(const counting_semaphore&) = delete;


private:
    native_handle_type m_id;
};

using binary_semaphore = counting_semaphore<1>;

} // namespace nonstd
} // namespace os::rtos

// #if !defined(GLIBCXX_HAS_GTHREADS) && !defined(_GLIBCXX_HAS_GTHREADS)
// namespace std {
// template <std::ptrdiff_t LeastMaxValue = std::numeric_limits<std::ptrdiff_t>::max()>
// using counting_semaphore = os::rtos::counting_semaphore<LeastMaxValue>;
// using binary_semaphore   = os::rtos::binary_semaphore;
// } // namespace std
// #endif
