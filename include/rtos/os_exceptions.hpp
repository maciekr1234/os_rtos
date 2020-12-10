#pragma once
#include <exception>
#include <stdexcept>
#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#endif
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cerrno>

#include <rtos/os_types.hpp>
#include <os/source_location.h>

namespace os::rtos {
namespace detail {


template <class Stream>
inline void report_exception(Stream& os, const char* name, const char* info) {
    os << "\n** uncaught exception: " << name << " " << info << '\n';
}
} // namespace detail

template <class Generator, class Stream>
inline int catch_exceptions(Generator function_object, Stream& out, Stream& err) {
    int  result           = 0;
    bool exception_thrown = true;

    result           = function_object();
    exception_thrown = false;
    // OS_CATCH(...) { report_exception(out, "unknown exception", ""); }

    if (exception_thrown)
        result = EXIT_FAILURE;

    if (result != 0 && result != EXIT_SUCCESS) {
        out << '\n' << "**** returning with error code " << result << '\n';
        if (err != out)
            err << "**********  errors detected; see stdout for details  ***********" << '\n';
    }
    return result;
}

inline unsigned int uncaught_exceptions() noexcept {
#if defined(_CXXABI_H) && (_CXXABI_H == 1)
    unsigned int count;
    std::memcpy(&count, reinterpret_cast<const unsigned char*>(::abi::__cxa_get_globals()) + sizeof(void*),
                sizeof(count)); // __cxa_eh_globals::uncaughtExceptions, x32 offset - 0x4, x64 - 0x8
    return count;
#else
    return static_cast<unsigned int>(std::uncaught_exceptions());
#endif
}

} // namespace os::rtos
namespace os {
using rtos::catch_exceptions;
using rtos::uncaught_exceptions;

struct throw_exception {
    template <class Stream>
    static void report_exception(Stream& os, const char* name, const char* info) {
        os << "\n** uncaught exception: " << name << " " << info << '\n';
    }

    void operator()(const char* ex) {}
    void operator()(const std::string& ex) { report_exception(stdout, "", ex.c_str()); }
    void operator()(const std::bad_exception& ex) { report_exception(stdout, "std::bad_exception:", ex.what()); }
    void operator()(const std::domain_error& ex) { report_exception(stdout, "std::domain_error:", ex.what()); }
    void operator()(const std::invalid_argument& ex) { report_exception(stdout, "std::invalid_argument:", ex.what()); }
    void operator()(const std::length_error& ex) { report_exception(stdout, "std::length_error:", ex.what()); }
    void operator()(const std::out_of_range& ex) { report_exception(stdout, "std::out_of_range:", ex.what()); }
    void operator()(const std::range_error& ex) { report_exception(stdout, "std::range_error:", ex.what()); }
    void operator()(const std::overflow_error& ex) { report_exception(stdout, "std::overflow_error:", ex.what()); }
    void operator()(const std::underflow_error& ex) { report_exception(stdout, "std::underflow_error:", ex.what()); }
    void operator()(const std::logic_error& ex) { report_exception(stdout, "std::logic_error:", ex.what()); }
    void operator()(const std::runtime_error& ex) { report_exception(stdout, "std::runtime_error:", ex.what()); }
    void operator()(const std::exception& ex) { report_exception(stdout, "std::exception:", ex.what()); }
    void operator()(...) { report_exception(stdout, "unknown exception", ""); }
};


} // namespace os
