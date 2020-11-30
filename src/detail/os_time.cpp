#include <rtos/os_kernel.hpp>
#include <rtos/detail/os_time.hpp>

namespace os::rtos {
kernel::clock::time_point kernel::clock::now() { return time_point(duration(kernel::get_tick_count())); }
} // namespace os::rtos

using os::rtos::detail::convert_duration;

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

int _gettimeofday(struct timeval*, void*);

int _gettimeofday(struct timeval* tp, void* tzvp) {
    if (tp != NULL) {
        uint64_t uu =
            convert_duration<std::chrono::microseconds>(os::rtos::kernel::get_tick_count(), configTICK_RATE_HZ).count();
        tp->tv_sec  = static_cast<time_t>(uu / 1000000);
        tp->tv_usec = static_cast<long>(uu % 100000);
    }

    if (tzvp != NULL) {
        struct timezone* tzp = static_cast<struct timezone*>(tzvp);
        tzp->tz_minuteswest  = 0;
        tzp->tz_dsttime      = 0;
    }

    return 0;
}
#ifdef __cplusplus
}
#endif
