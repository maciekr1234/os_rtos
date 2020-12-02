#include <rtos/os_types.hpp>

#include <sstream>
namespace os {
/**
 * \brief utility to convert os::status code to debug message.
 *
 * \param _status os::status value.
 * \return string message
 */
std::string str_status(const status& _status) {
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


} // namespace os
