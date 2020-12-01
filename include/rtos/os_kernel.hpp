#pragma once

#include <rtos/os_types.hpp>
#include <rtos/detail/os_time.hpp>

#include <list>


namespace os {
/**
 * \addtogroup rtos_api
 * @{
 */

namespace rtos {

/** \defgroup kernel_api RTOS Kernel Api
 *  @{
 */
class kernel : private non_copyable<kernel>, private non_moveable<kernel> {
public:
    enum class state : int8_t {
        inactive  = 0,
        ready     = 1,
        running   = 2,
        locked    = 3,
        suspended = 4,
        error     = -1,
    };

    using clock = detail::kernel_clock;

    static constexpr clock::duration_u32 wait_for_u32_max{0xFFFFFFFF - 1};
    static constexpr clock::duration_u32 wait_for_u32_forever{0xFFFFFFFF};

public:
    /**
     * \brief initiali(sz)es kernel object.
     */
    static os_status_t init();

    /**
     * \brief Start rtos scheduler.
     *
     * \retval os::status::ok
     */
    static os_status_t start();


    /**
     * \brief Get current kernel state.
     * \retval kernel::state
     */
    static state get_state();


    static lock_flag_t lock();
    static lock_flag_t unlock();
    static lock_flag_t restore_lock(lock_flag_t lock);

    static uint64_t get_tick_count();

    static bool is_irq();

    kernel() = delete;

private:
    static TickType_t get_native_tick_count();

    static void set_state(state _state);

private:
    static inline state    s_state = state::inactive;
    static inline uint32_t s_critical_section_flag;
};

} // namespace rtos

///


/** @} */

using rtos::kernel;


} // namespace os

/** @} */


#ifndef OS_NO_GLOBAL_TYPES
using os_kernel_state = ::os::rtos::kernel::state;
#endif
