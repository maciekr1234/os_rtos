#pragma once


#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <queue.h>
#include <semphr.h>
#include <event_groups.h>

#include <string.h>
#include <string>

#include <os/compiler.h>
#include <os/non_copyable.h>


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

std::string str_status(const status& _status);

inline namespace flags {
inline constexpr uint32_t osFlagsWaitAny = 0x00000000U; ///< Wait for any flag (default).
inline constexpr uint32_t osFlagsWaitAll = 0x00000001U; ///< Wait for all flags.osFlagsWaitAll
inline constexpr uint32_t osFlagsNoClear = 0x00000002U; ///< Do not clear flags which have been specified to wait for.

inline constexpr uint32_t osFlagsError          = 0x80000000U; ///< Error indicator.
inline constexpr uint32_t osFlagsErrorUnknown   = 0xFFFFFFFFU; ///< osError (-1).
inline constexpr uint32_t osFlagsErrorTimeout   = 0xFFFFFFFEU; ///< osErrorTimeout (-2).
inline constexpr uint32_t osFlagsErrorResource  = 0xFFFFFFFDU; ///< osErrorResource (-3).
inline constexpr uint32_t osFlagsErrorParameter = 0xFFFFFFFCU; ///< osErrorParameter (-4).
inline constexpr uint32_t osFlagsErrorISR       = 0xFFFFFFFAU; ///< osErrorISR (-6).

} // namespace flags


} // namespace os
#define OS_NO_CMSIS_COMPABILITY
#ifndef OS_NO_GLOBAL_TYPES
using os_status_t = os::status;

using namespace os::flags;


#endif

#ifndef OS_NO_CMSIS_COMPABILITY
#ifdef osWaitForever
#undef osWaitForever
#endif

#define osWaitForever 0xFFFFFFFF


#define osOK             os::status::ok
#define osError          os::status::error
#define osErrorTimeout   os::status::error_timeout
#define osErrorResource  os::status::error_resource
#define osErrorParameter os::status::error_parameter
#define osErrorNoMemory  os::status::error_no_memory
#define osErrorISR       os::status::error_isr
#endif
/** @} */
/** @} */
/** @} */
