#pragma once

#include <mutex>

#include <rtos/os_types.hpp>
#include <rtos/os_time.hpp>
#include <rtos/detail/os_once.hpp>

namespace os::rtos {

class mutex : private non_copyable<mutex> {
public:
    constexpr mutex(bool recursive = false) noexcept : _mutex(), _is_recursive(recursive), _count(0) { constructor(recursive); }

    ~mutex();

    void lock();

    bool try_lock();

    bool try_lock_for(kernel_clock::duration_u32 rel_time);

    bool try_lock_until(kernel_clock::time_point abs_time);

    void unlock();

private:
    SemaphoreHandle_t constructor(bool recursive = false);

private:
    SemaphoreHandle_t _mutex;

    bool _is_recursive;

    uint32_t _count;
};


} // namespace os::rtos


#if !defined(GLIBCXX_HAS_GTHREADS) && !defined(_GLIBCXX_HAS_GTHREADS)
namespace std {

class mutex : private os::non_copyable<mutex> {
public:
    typedef os::rtos::mutex* native_handle_type;

    constexpr mutex() noexcept
        : _mutex{false} {

          };

    ~mutex() = default;

    void lock() { _mutex.lock(); }

    bool try_lock() noexcept { return _mutex.try_lock(); }

    void unlock() noexcept { _mutex.unlock(); }


    native_handle_type native_handle() { return static_cast<native_handle_type>(&_mutex); }


private:
    os::rtos::mutex _mutex{false};
};

class recursive_mutex {
public:
    typedef os::rtos::mutex* native_handle_type;

    recursive_mutex() : _mutex(true) {}

    ~recursive_mutex() = default;

    void lock() { _mutex.lock(); }

    bool try_lock() noexcept { return _mutex.try_lock(); }

    void unlock() noexcept { _mutex.unlock(); }

    native_handle_type native_handle() { return static_cast<native_handle_type>(&_mutex); }

private:
    os::rtos::mutex _mutex;
};
} // namespace std
#endif
