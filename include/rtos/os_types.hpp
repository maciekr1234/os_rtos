#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include <os/compiler.h>
#include <os/non_copyable.h>
#include <rtos/detail/utility.hpp>
#include <string.h>

/**
 * \defgroup os_api Operating System Api
 * @{
 */


/** \defgroup os_types OS Types
 * @{
 */


namespace os {


enum class status : int32_t {
    ok              = 0,  /// Operation completed successfully.
    error           = -1, /// Unspecified RTOS error: run-time error but no other error message fits.
    error_timeout   = -2, /// Operation not completed within the timeout period.
    error_resource  = -3, /// Resource not available.
    error_parameter = -4, /// Parameter error.
    error_no_memory = -5, /// System is out of memory: it was impossible to allocate or reserve memory for the operation
    error_isr       = -6, /// Not allowed in ISR context: the function cannot be called from interrupt service routines.
};


enum class priority : int8_t {
    idle         = 0,
    low          = 1,
    below_normal = 2,
    normal       = 3,
    above_normal = 4,
    high         = 5,
    realtime     = 6,
    error        = -1,
};

enum class kernel_state : int8_t {
    inactive  = 0,
    ready     = 1,
    running   = 2,
    locked    = 3,
    suspended = 4,
    error     = -1,
};

enum class thread_state : int8_t {
    inactive   = 0,
    ready      = 1,
    running    = 2,
    blocked    = 3,
    terminated = 4,
    error      = -1,
};

/**
 * \brief magic "wait forever" value.
 * Same as osWaitForever in CMSIS-RTOS which is 0xFFFFFFFF
 *
 */
inline constexpr uint32_t wait_forever{0xFFFFFFFF};

inline constexpr uint32_t flags_wait_any = 0x00000000U; ///< Wait for any flag (default).
inline constexpr uint32_t flags_wait_all = 0x00000001U; ///< Wait for all flags.os::flags_wait_all
inline constexpr uint32_t flags_no_clear = 0x00000002U; ///< Do not clear flags which have been specified to wait for.

inline constexpr uint32_t flags_error          = 0x80000000U; ///< Error indicator.
inline constexpr uint32_t flags_error_unknown   = 0xFFFFFFFFU; ///< osError (-1).
inline constexpr uint32_t flags_error_timeout   = 0xFFFFFFFEU; ///< osErrorTimeout (-2).
inline constexpr uint32_t flags_error_resource  = 0xFFFFFFFDU; ///< osErrorResource (-3).
inline constexpr uint32_t flags_error_parameter = 0xFFFFFFFCU; ///< osErrorParameter (-4).
inline constexpr uint32_t flags_error_isr       = 0xFFFFFFFAU; ///< osErrorISR (-6).


} // namespace os


using os_status_t = os::status;

/** @} */
/** @} */
/** @} */
