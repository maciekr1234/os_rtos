#pragma once

#include <rtos/os_types.hpp>
#include <rtos/os_kernel.hpp>

namespace os::rtos {

// class mutex : private non_copyable<mutex> {
// public:
//     mutex();

//     mutex(const char* name);

//     void lock();

//     bool try_lock();

//     bool try_lock_for(uint32_t millisec);

//     bool try_lock_for(kernel::clock::duration_u32 rel_time);


//     bool try_lock_until(uint64_t millisec);

//     bool try_lock_until(kernel::clock::time_point abs_time);

//     void unlock();

//     osThreadId_t get_owner();

//     ~mutex();

// private:
//     void constructor(const char* name = nullptr);

//     osMutexId_t m_id;
//     os_mutex_attr_t m_attr;
//     uint32_t    m_count;
// };

////////////////////////////////////////////////////////////////////////////////

enum class mutex_type {none = 0, standard, recursive, timed };
namespace detail {


class base_timed_mutex {
protected:
    typedef void* native_handle_type;

    base_timed_mutex(const char* name, bool recursive);
    ~base_timed_mutex() noexcept(false);

    void lock();
    void unlock();
    bool try_lock();

    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period>& rel_time) {
        return try_lock_for_usec(rel_time);
    }

    template <class Clock, class Duration>
    bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        return try_lock_for(abs_time - Clock::now());
    }

    native_handle_type native_handle() noexcept { return m_id; }

    base_timed_mutex(const base_timed_mutex&) = delete;
    base_timed_mutex& operator=(const base_timed_mutex&) = delete;

private:
    bool try_lock_for_usec(std::chrono::microseconds usec);

    
private:
    native_handle_type m_id; ///< mutex identifier
};


} // namespace detail

// STL like implementation
class mutex : private detail::base_timed_mutex {
public:
    typedef detail::base_timed_mutex::native_handle_type native_handle_type;

    mutex() : detail::base_timed_mutex("mutex", false) {}
    ~mutex() = default;

    void lock() { detail::base_timed_mutex::lock(); }
    void unlock() { detail::base_timed_mutex::unlock(); }
    bool try_lock() { return detail::base_timed_mutex::try_lock(); }

    native_handle_type native_handle() noexcept { return detail::base_timed_mutex::native_handle(); }

    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;
};

class recursive_mutex : private detail::base_timed_mutex {
public:
    typedef detail::base_timed_mutex::native_handle_type native_handle_type;

    recursive_mutex() : detail::base_timed_mutex("recursive_mutex", true) {}
    ~recursive_mutex() = default;

    void lock() { detail::base_timed_mutex::lock(); }
    void unlock() { detail::base_timed_mutex::unlock(); }
    bool try_lock() { return detail::base_timed_mutex::try_lock(); }

    native_handle_type native_handle() noexcept { return detail::base_timed_mutex::native_handle(); }

    recursive_mutex(const recursive_mutex&) = delete;
    recursive_mutex& operator=(const recursive_mutex&) = delete;
};

class timed_mutex : private detail::base_timed_mutex {
public:
    typedef detail::base_timed_mutex::native_handle_type native_handle_type;

    timed_mutex() : detail::base_timed_mutex("timed_mutex", false) {}
    ~timed_mutex() = default;

    void lock() { detail::base_timed_mutex::lock(); }
    void unlock() { detail::base_timed_mutex::unlock(); }
    bool try_lock() { return detail::base_timed_mutex::try_lock(); }

    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period>& rel_time) {
        return detail::base_timed_mutex::try_lock_for(rel_time);
    }

    template <class Clock, class Duration>
    bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        return detail::base_timed_mutex::try_lock_until(abs_time);
    }

    native_handle_type native_handle() noexcept { return detail::base_timed_mutex::native_handle(); }

    timed_mutex(const timed_mutex&) = delete;
    timed_mutex& operator=(const timed_mutex&) = delete;
};


class recursive_timed_mutex : private detail::base_timed_mutex {
public:
    typedef detail::base_timed_mutex::native_handle_type native_handle_type;

    recursive_timed_mutex() : detail::base_timed_mutex("recursive_timed_mutex", true) {}
    ~recursive_timed_mutex() = default;

    void lock() { detail::base_timed_mutex::lock(); }
    void unlock() { detail::base_timed_mutex::unlock(); }
    bool try_lock() { return detail::base_timed_mutex::try_lock(); }

    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period>& rel_time) {
        return detail::base_timed_mutex::try_lock_for(rel_time);
    }

    template <class Clock, class Duration>
    bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        return detail::base_timed_mutex::try_lock_until(abs_time);
    }

    native_handle_type native_handle() noexcept { return detail::base_timed_mutex::native_handle(); }

    recursive_timed_mutex(const recursive_timed_mutex&) = delete;
    recursive_timed_mutex& operator=(const recursive_timed_mutex&) = delete;
};


} // namespace os::rtos
