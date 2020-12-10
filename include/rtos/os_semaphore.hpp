#pragma once

#include <rtos/os_kernel.hpp>

#include <chrono>

namespace os::rtos {

class semaphore : private non_copyable<semaphore> {
public:
    semaphore(int32_t count = 0) : semaphore(count, 0xffff) {}

    semaphore(int32_t count, uint16_t max_count);

    ~semaphore();

    os_status_t release();

    void acquire();

    bool try_acquire(int32_t timeout = 0);

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time);

    bool try_acquire_for(kernel::clock::duration_u32 rel_time);

    template <class Clock, class Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time);

    bool try_acquire_until(kernel::clock::time_point abs_time);


private:
    SemaphoreHandle_t m_handle;
};

} // namespace os::rtos


namespace os {

template <std::ptrdiff_t least_max_value = static_cast<std::ptrdiff_t>(os::wait_forever)>
class counting_semaphore : private os::non_copyable<counting_semaphore<least_max_value>> {
    using native_semaphore_type = ::os::rtos::semaphore;

public:
    static constexpr std::ptrdiff_t max() noexcept { return least_max_value; }

    counting_semaphore(std::ptrdiff_t count = 0) : _semaphore(count) {}

    ~counting_semaphore() { _semaphore.~semaphore(); };


    void release(std::ptrdiff_t _update = 1) {
        while(_update--) {
            _semaphore.release();
        }
    }

    void acquire() { _semaphore.acquire(); }

    bool try_acquire() { return _semaphore.try_acquire(); }

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time) {
        return _semaphore.try_acquire_for(rel_time);
    }

    template <class Clock, class Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        return _semaphore.try_acquire_until(abs_time);
    }

private:
    native_semaphore_type _semaphore;
};

using binary_semaphore = counting_semaphore<1>;


} // namespace stdex

#ifndef _GLIBCXX_HAS_GTHREADS
namespace std {

template<ptrdiff_t least_max_value>
using counting_semaphore = ::os::counting_semaphore<least_max_value>;
using binary_semaphore = counting_semaphore<1>;

}
#endif
