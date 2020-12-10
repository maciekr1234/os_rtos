#include <rtos/detail/utility.hpp>

#include <sstream>

namespace os {

/**
 * \brief utility to convert os::status code to debug message.
 *
 * \param _status os::status value.
 * \return string message
 */
const char* str_status(const status& _status) {
    switch (_status) {
        case os::status::ok:
            return "Operation completed successfully";
        case os::status::error:
            return "Unspecified RTOS error: run-time error but no other error message fits";
        case os::status::error_timeout:
            return "Operation not completed within the timeout period";
        case os::status::error_resource:
            return "Resource not available";
        case os::status::error_parameter:
            return "Parameter error";
        case os::status::error_no_memory:
            return "System is out of memory: it was impossible to allocate or reserve memory for the operation";
        case os::status::error_isr:
            return "Not allowed in ISR context: the function cannot be called from interrupt service routines";
        default:
            return "Unknown error";
    }
}
const char* str_state(const kernel_state& _state) {
    switch (_state) {
        case kernel_state::inactive:
            return "kernel_state::inactive";
            break;
        case kernel_state::ready:
            return "kernel_state::ready";
            break;
        case kernel_state::running:
            return "kernel_state::running";
            break;
        case kernel_state::locked:
            return "kernel_state::locked";
            break;
        case kernel_state::suspended:
            return "kernel_state::suspended";
            break;
        case kernel_state::error:
        default:
            return "kernel_state::error";
            break;
    }
}
const char* str_state(const thread_state& _state) {
    switch (_state) {
        case thread_state::inactive:
            return "thread_state::inactive";
            break;
        case thread_state::ready:
            return "thread_state::ready";
            break;
        case thread_state::running:
            return "thread_state::running";
            break;
        case thread_state::blocked:
            return "thread_state::locked";
            break;
        case thread_state::terminated:
            return "thread_state::suspended";
            break;
        case thread_state::error:
        default:
            return "thread_state::error";
            break;
    }
}


} // namespace os
