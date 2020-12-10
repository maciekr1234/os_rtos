#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>

#include "FreeRTOS.h"


namespace os::rtos {
namespace memory {

template <typename T>
class rtos_allocator {
public:
    typedef T              value_type;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T*             pointer;
    typedef const T*       const_pointer;
    typedef T&             reference;
    typedef const T&       const_reference;

    rtos_allocator() noexcept = default;

    rtos_allocator(const rtos_allocator& rhs) noexcept = default;

    template <typename U>
    rtos_allocator(const rtos_allocator<U>& rhs) noexcept = default;

    ~rtos_allocator() = default;

    template <typename U>
    struct rebind {
        typedef rtos_allocator<U> other;
    };

    pointer address(reference val) const noexcept { return &val; }

    const_pointer address(const_reference val) const noexcept { return &val; }

    size_type max_size() const noexcept { return std::numeric_limits<std::size_t>::max() / sizeof(T); }

    // c++17 style
    pointer allocate(size_type num) { return allocate(num, 0); }

    // c++11 style
    pointer allocate(size_type num, const void* hint = 0) {
        pointer p = nullptr;
        if (alignof(T) <= portBYTE_ALIGNMENT) {
            // default alignment
            p = pvPortMalloc(num * sizeof(T));

            if (p == nullptr) {
                throw std::bad_alloc();
            }
        } else {
            // wider alignment
            constexpr std::uintptr_t allignment      = alignof(T);
            constexpr std::uintptr_t allignment_mask = alignof(T) - 1U;

            pointer raw_p = pvPortMalloc(num * sizeof(T) + allignment);

            if (raw_p == nullptr) {
                throw std::bad_alloc();
            }

            // get an aligned poiner
            p = reinterpret_cast<pointer>((reinterpret_cast<std::uintptr_t>(raw_p) & (~allignment_mask)) + allignment);

            // stash the original pointer
            *(reinterpret_cast<void**>(p) - 1) = raw_p;
        }

        return p;
    }

    void construct(pointer p, const T& val) { ::new (p) T(val); }

    template <class U, class... Args>
    void construct(U* p, Args&&... args) {
        ::new (p) U(std::forward<Args>(args)...);
    }

    void destroy(pointer p) { p->~T(); }

    void deallocate(pointer p, size_type num) {
        if (alignof(T) <= portBYTE_ALIGNMENT) {
            // default alignment
            vPortFree(p);
        } else {
            // wider alignment, we stashed the real heap pointer here
            pointer start = static_cast<pointer>(*(reinterpret_cast<void**>(p) - 1));
            vPortFree(start);
        }
    }
};

template <class T1, class T2>
bool operator==(const rtos_allocator<T1>& lhs, const rtos_allocator<T2>& rhs) noexcept {
    return true;
}

template <class T1, class T2>
bool operator!=(const rtos_allocator<T1>& lhs, const rtos_allocator<T2>& rhs) noexcept {
    return false;
}

} // namespace memory
} // namespace os::rtos
