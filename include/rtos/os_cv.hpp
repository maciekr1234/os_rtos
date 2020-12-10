#pragma once

#include <rtos/os_mutex.hpp>
#include <rtos/os_semaphore.hpp>

#include <list>
#include <memory>

namespace os::rtos {

// cv_status
enum class cv_status { no_timeout, timeout };

// STL like implementation
class condition_variable {
public:
    typedef void* native_handle_type;

    typedef kernel::clock clock_t;

    condition_variable() = default;

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


private:
    cv_status wait_for_usec(std::unique_lock<rtos::mutex>& lock, std::chrono::microseconds usec);

private:
    rtos::mutex                  _mutex;
    std::list<binary_semaphore*> _wait;
};


// void notify_all_at_thread_exit(condition_variable& cond, std::unique_lock<rtos::mutex> lk);

struct __lock_external {
    template <class Lock>
    void operator()(Lock* _m) {
        _m->lock();
    }
};

class condition_variable_any : private non_copyable<condition_variable_any> {
public:
    condition_variable_any() : _mutex(std::make_shared<rtos::mutex>()) {}
    ~condition_variable_any();

    void notify_one() noexcept {
        { std::lock_guard<rtos::mutex> _lk(*_mutex); }
        _cv.notify_one();
    }
    void notify_all() noexcept {
        { std::lock_guard<rtos::mutex> _lk(*_mutex); }
        _cv.notify_all();
    }

    template <class Lock>
    void wait(Lock& lock) {
        std::shared_ptr<rtos::mutex>  _mut = _mutex;
        std::unique_lock<rtos::mutex> _lk(*_mut);

        lock.unlock();
        std::unique_ptr<Lock, __lock_external>         __lxx(&lock);
        std::lock_guard<std::unique_lock<rtos::mutex>> _lx(_lk, std::adopt_lock);
        _cv.wait(_lk);
    }

    template <class Lock, class Predicate>
    void wait(Lock& lock, Predicate pred) {
        while (!pred) {
            wait(lock);
        }
    }

    template <class Lock, class Clock, class Duration>
    cv_status wait_until(Lock& lock, const std::chrono::time_point<Clock, Duration>& abs_time) {
        std::shared_ptr<rtos::mutex>  _mut = _mutex;
        std::unique_lock<rtos::mutex> _lk(*_mut);

        lock.unlock();
        std::unique_ptr<Lock, __lock_external>         __lxx(&lock);
        std::lock_guard<std::unique_lock<rtos::mutex>> _lx(_lk, std::adopt_lock);
        return _cv.wait_until(_lk, abs_time);
    }

    template <class Lock, class Clock, class Duration, class Predicate>
    bool wait_until(Lock& lock, const std::chrono::time_point<Clock, Duration>& abs_time, Predicate pred) {
        while( !pred ){
            if(wait_until(lock, abs_time) == cv_status::timeout) {
                return pred();
            }
        }
        return true;
    }

    template <class Lock, class Rep, class Period>
    cv_status wait_for(Lock& lock, const std::chrono::duration<Rep, Period>& rel_time) {
        return wait_until(lock, kernel::clock::now() + rel_time);
    }

    template <class Lock, class Rep, class Period, class Predicate>
    bool wait_for(Lock& lock, const std::chrono::duration<Rep, Period>& rel_time, Predicate pred) {
        return wait_until(lock, kernel::clock::now() + rel_time, std::move(pred));
    }

private:
    condition_variable           _cv;
    std::shared_ptr<rtos::mutex> _mutex;
};

} // namespace os::rtos

#if !defined(GLIBCXX_HAS_GTHREADS) && !defined(_GLIBCXX_HAS_GTHREADS)
namespace std {
    using cv_status = ::os::rtos::cv_status;
    using condition_variable = ::os::rtos::condition_variable;
    using condition_variable_any = ::os::rtos::condition_variable_any;

}
#endif
