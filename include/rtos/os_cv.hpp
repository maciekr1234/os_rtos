#pragma once

#include <list>
#include <mutex>

#include <rtos/os_mutex.hpp>
#include <rtos/os_semaphore.hpp>

namespace os::rtos {

// cv_status
enum class cv_status { no_timeout, timeout };

// STL like implementation
class condition_variable {
public:
    typedef void* native_handle_type;

    typedef kernel::clock clock_t;

    condition_variable()  = default;
    ~condition_variable() = default;

    void notify_one() noexcept;
    void notify_all() noexcept;
    void wait(std::unique_lock<rtos::mutex>& lock);

    template <class Predicate>
    void wait(std::unique_lock<rtos::mutex>& lock, Predicate pred) {
        while (!pred())
            wait(lock);
    }

    template <class Clock, class Duration>
    cv_status wait_until(std::unique_lock<rtos::mutex>&                  lock,
                         const std::chrono::time_point<Clock, Duration>& abs_time) {
        return wait_for(lock, abs_time - Clock::now());
    }

    template <class Clock, class Duration, class Predicate>
    bool wait_until(std::unique_lock<rtos::mutex>&                  lock,
                    const std::chrono::time_point<Clock, Duration>& abs_time,
                    Predicate                                       pred) {
        while (!pred()) {
            if (wait_until(lock, abs_time) == cv_status::timeout)
                return pred();
        }
        return true;
    }

    template <class Rep, class Period>
    cv_status wait_for(std::unique_lock<rtos::mutex>& lock, const std::chrono::duration<Rep, Period>& rel_time) {
        return wait_for_usec(lock, std::chrono::duration_cast<std::chrono::microseconds>(rel_time));
    }

    template <class Rep, class Period, class Predicate>
    bool wait_for(std::unique_lock<rtos::mutex>&            lock,
                  const std::chrono::duration<Rep, Period>& rel_time,
                  Predicate                                 pred) {
        clock_t::time_point abs_time = clock_t::now() + rel_time;

        while (!pred()) {
            if (wait_until(lock, abs_time) == cv_status::timeout)
                return pred();
        }
        return true;
    }

    native_handle_type native_handle() noexcept { return nullptr; }

    condition_variable(const condition_variable&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;

private:
    cv_status wait_for_usec(std::unique_lock<rtos::mutex>& lock, std::chrono::microseconds usec);

private:
    rtos::mutex                                      m_mutex;
    std::list<rtos::experimental::binary_semaphore*> m_wait;
};

} // namespace os::rtos
