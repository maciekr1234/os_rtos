#pragma once

#include <functional>
#include <mutex>
#include <cxxabi.h>

using __cxxabiv1::__guard;


extern "C" int  __cxa_guard_acquire(__guard* raw_guard_object);
extern "C" void __cxa_guard_release(__guard* raw_guard_object);
extern "C" void __cxa_guard_abort(__guard* raw_guard_object);


namespace os {

template <typename Mutex, typename... Args>
[[nodiscard]] std::unique_lock<Mutex> make_unique_lock(Mutex& mutex, Args&&... args) {
    return std::unique_lock<Mutex>{mutex, std::forward<Args>(args)...};
}

template <typename Mutex>
[[nodiscard]] std::lock_guard<Mutex> make_lock_guard(Mutex& mutex) {
    return std::lock_guard<Mutex>{mutex};
}


struct once_flag {
    constexpr once_flag() noexcept : m_guard() {}
    ~once_flag() = default;

    once_flag(const once_flag&) = delete;
    once_flag& operator=(const once_flag&) = delete;

private:
    template <class Callable, class... Args>
    friend void call_once(once_flag& flag, Callable&& f, Args&&... args);

    __guard m_guard;
};


template <class Callable, class... Args>
void call_once(once_flag& flag, Callable&& f, Args&&... args) {
    if (__cxa_guard_acquire(&flag.m_guard)) {
        std::invoke(std::forward<Callable>(f), std::forward<Args>(args)...);
        __cxa_guard_release(&flag.m_guard);
    }
}


}
