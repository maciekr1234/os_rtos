#pragma once

#include <rtos/os_types.hpp>
#include <rtos/os_kernel.hpp>

namespace os::rtos {

enum class mutex_type {
    none            = 0,
    standard        = 1,
    recursive       = 2,
    timed           = 3,
    recursive_timed = 2,
};

namespace detail {


template <mutex_type T>
class mutex_base : private non_copyable<mutex_base<T>> {
    // typedef SemaphoreHandle_t native_handle_t;

    static constexpr mutex_type type = T;

public:
    typedef SemaphoreHandle_t native_handle_type;

    mutex_base() : mutex_base(nullptr) {}
    mutex_base(const char* name);
    virtual ~mutex_base();

    void lock();
    bool try_lock();

    bool try_lock_for(kernel::clock::duration_u32 rel_time);

    bool try_lock_until(kernel::clock::time_point abs_time);

    void unlock();

    // osThreadId_t get_owner();

    native_handle_type native_handle() noexcept { return m_id; }

private:
    native_handle_type m_id;
};

} // namespace detail

class mutex : private detail::mutex_base<mutex_type::standard> {
    using base_type = detail::mutex_base<mutex_type::standard>;

public:
    using base_type::native_handle_type;

    mutex() : base_type("mutex") {}
    ~mutex() = default;

    void lock() { base_type::lock(); }

    void unlock() { base_type::unlock(); }

    bool try_lock() { return base_type::try_lock(); }
};

class recursive_mutex : private detail::mutex_base<mutex_type::recursive> {
    using base_type = detail::mutex_base<mutex_type::recursive>;

public:
    using base_type::native_handle_type;

    recursive_mutex() : base_type("recursive_mutex") {}

    ~recursive_mutex() = default;

    void lock() { base_type::lock(); }

    void unlock() { base_type::unlock(); }

    bool try_lock() { return base_type::try_lock(); }
};

class timed_mutex : private detail::mutex_base<mutex_type::timed> {
    using base_type = detail::mutex_base<mutex_type::timed>;

public:
    using base_type::native_handle_type;

    timed_mutex() : base_type("timed_mutex") {}
    ~timed_mutex() = default;

    void lock() { base_type::lock(); }

    void unlock() { base_type::unlock(); }

    bool try_lock() { return base_type::try_lock(); }

    native_handle_type native_handle() noexcept { return base_type::native_handle(); }

    bool try_lock_for(kernel::clock::duration_u32 rel_time) { return base_type::try_lock_for(rel_time); }

    bool try_lock_until(kernel::clock::time_point abs_time) { return base_type::try_lock_until(abs_time); }
};

class recursive_timed_mutex : private detail::mutex_base<mutex_type::recursive_timed> {
    using base_type = detail::mutex_base<mutex_type::recursive_timed>;

public:
    using base_type::native_handle_type;

    recursive_timed_mutex() : base_type("recursive_timed_mutex") {}
    ~recursive_timed_mutex() = default;

    void lock() { base_type::lock(); }

    void unlock() { base_type::unlock(); }

    bool try_lock() { return base_type::try_lock(); }

    native_handle_type native_handle() noexcept { return base_type::native_handle(); }

    bool try_lock_for(kernel::clock::duration_u32 rel_time) { return base_type::try_lock_for(rel_time); }

    bool try_lock_until(kernel::clock::time_point abs_time) { return base_type::try_lock_until(abs_time); }
};
} // namespace os::rtos
