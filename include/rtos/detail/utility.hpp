/**
 * \file utility.hpp
 * \date 10-12-2020
 */
#pragma once

#include <rtos/os_types.hpp>

namespace os {

const char* str_status(const status& _status);
const char* str_state(const kernel_state& _state);
const char* str_state(const thread_state& _state);


struct version {
    uint8_t  major;
    uint8_t  minor;
    uint16_t patch;

    inline constexpr uint32_t operator()() const { return ((major * 1000) + (minor * 100) + patch); }

    friend constexpr bool operator==(const version& a, const version& b) noexcept { return (a() == b()); }
    friend constexpr bool operator<(const version& a, const version& b) noexcept { return (a() < b()); }
};
template <class StreamT>
inline StreamT& operator<<(StreamT& os, const version& ver) {
    os << 'v' << ver.major << '.' << ver.minor << '.' << ver.patch;
    return os;
}

inline constexpr bool operator!=(const version& a, const version& b) noexcept { return !(a == b); }
inline constexpr bool operator>(const version& a, const version& b) noexcept { return (b < a); }
inline constexpr bool operator<=(const version& a, const version& b) noexcept { return !(b < a); }
inline constexpr bool operator>=(const version& a, const version& b) noexcept { return !(a < b); }



} // namespace os
