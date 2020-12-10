#pragma once

#include <rtos/os_types.hpp>
#include <rtos/os_time.hpp>


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
    using state = os::kernel_state;
    using clock = kernel_clock;

    static constexpr os::version version = {.major = tskKERNEL_VERSION_MAJOR,
                                                .minor = tskKERNEL_VERSION_MINOR,
                                                .patch = tskKERNEL_VERSION_BUILD};

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


    static uint32_t lock();
    static uint32_t unlock();
    static uint32_t restore_lock(uint32_t lock);

    static uint64_t get_tick_count();

    static bool is_irq();


private:
    kernel() = delete;
    friend class thread;

    static TickType_t get_native_tick_count();

private:
    static inline state    sKernelState = state::inactive;
    static inline uint32_t s_critical_section_flag;

    // static inline thread_map_t thread_map;
};


} // namespace rtos

///


/** @} */

using rtos::kernel;


} // namespace os

/** @} */
// using os_kernel_state = ::os::rtos::kernel::state;
