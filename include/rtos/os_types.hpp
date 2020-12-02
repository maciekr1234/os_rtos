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


} // namespace os

#ifndef OS_NO_GLOBAL_TYPES

using os_status_t = os::status;


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
