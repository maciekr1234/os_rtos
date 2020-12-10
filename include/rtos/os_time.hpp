#pragma once

#include <chrono>

namespace os::rtos {

struct kernel_clock {
    using duration     = std::chrono::milliseconds;
    using rep          = duration::rep;
    using period       = duration::period;
    using time_point   = std::chrono::time_point<kernel_clock>;
    using duration_u32 = std::chrono::duration<uint32_t, period>;

    static constexpr bool is_steady = true;

    static time_point now();

    static void lock() {}

    static void unlock() {}


    kernel_clock() = delete;
};

using duration_u32 = kernel_clock::duration_u32;


/**
 * \brief Maximum duration for kernel::clock::duration_u32-based APIs
 *
 */
inline constexpr kernel_clock::duration_u32 wait_for_u32_max{os::wait_forever - 1};
/**
 * \brief Magic "wait forever" constant for  kernel::clock::duration_u32-based APIs
 *
 */
inline constexpr kernel_clock::duration_u32 wait_for_u32_forever{os::wait_forever};

template <class D>
D convert_duration(uint64_t count, uint32_t freq) {
    return D((count * D::period::den) / (freq * D::period::num));
}



}
