#pragma once

// clang-format off
#if __has_include(<source_location>)
    #include <source_location>
    namespace os {
        using source_location = std::source_location;
    } // namespace os
#elif __has_include(<experimental/source_location>)
    #include <experimental/source_location>

    namespace os {
        using source_location = std::experimental::source_location;
    } // namespace os
#else

    #ifndef _PLATFORM_CURRENT_FUNCTION
        #if defined(__has_builtin) && __has_builtin(__builtin_FUNCTION)
            #define _PLATFORM_CURRENT_FUNCTION() __builtin_FUNCTION()
        #elif defined(__GNUC__) || defined(__clang__)
            #define _PLATFORM_CURRENT_FUNCTION() __PRETTY_FUNCTION__
        #elif defined(__FUNCSIG__)
            #define _PLATFORM_CURRENT_FUNCTION() __FUNCSIG__
        #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
            #define _PLATFORM_CURRENT_FUNCTION() __func__
        #elif defined(__cplusplus) && (__cplusplus >= 201103)
            #define _PLATFORM_CURRENT_FUNCTION() __func__
        #else
            #define _PLATFORM_CURRENT_FUNCTION() "(unknown)"
        #endif
    #endif // _PLATFORM_CURRENT_FUNCTION
    #ifndef _PLATFORM_CURRENT_FILE
        #if defined(__has_builtin) && __has_builtin(__builtin_FILE)
            #define _PLATFORM_CURRENT_FILE() __builtin_FILE()
        #elif defined(__FILE__)
            #define _PLATFORM_CURRENT_FILE() __FILE__
        #else
            #define _PLATFORM_CURRENT_FILE() "(unknown)"
        #endif
    #endif // _PLATFORM_CURRENT_FILE
    #ifndef _PLATFORM_CURRENT_LINE
        #if defined(__has_builtin) && __has_builtin(__builtin_LINE)
            #define _PLATFORM_CURRENT_LINE() __builtin_LINE()
        #elif defined(__LINE__)
            #define _PLATFORM_CURRENT_LINE() __LINE__
        #else
            #define _PLATFORM_CURRENT_LINE() 0
        #endif
    #endif // _PLATFORM_CURRENT_LINE
    namespace os {
    struct source_location {
        using string_type  = char const*;
        using numeric_type = uint_least32_t;

        constexpr source_location() noexcept
            : m_file("(unknown)"), m_function("(unknown)"), m_line(0), m_column(0) {}

        constexpr source_location(string_type file, string_type function, numeric_type line,
                                numeric_type column = 0) noexcept
            : m_file(file), m_function(function), m_line(line), m_column(column) {}

        static constexpr source_location current(string_type  file     = _PLATFORM_CURRENT_FILE(),
                                                string_type  function = _PLATFORM_CURRENT_FUNCTION(),
                                                numeric_type line     = _PLATFORM_CURRENT_LINE(),
                                                numeric_type column   = 0

                                                ) noexcept {
            return source_location(file, function, line);
        }

        constexpr string_type  file_name() const noexcept { return m_file; }
        constexpr string_type  function_name() const noexcept { return m_function; }
        constexpr numeric_type line() const noexcept { return m_line; }
        constexpr numeric_type column() const noexcept { return m_column; }

    private:
        string_type  m_file;
        string_type  m_function;
        numeric_type m_line;
        numeric_type m_column;
    };
    } // namespace os
#endif
// clang-format on

// #ifndef PLATFORM_CURRENT_LOCATION
    // #define PLATFORM_CURRENT_LOCATION os::source_location::current()
    // #define PLATFORM_CURRENT_FILE     os::source_location::current().file_name()
    // #define PLATFORM_CURRENT_LINE     os::source_location::current().line()
    // #define PLATFORM_CURRENT_FUNCTION os::source_location::current().function_name()
// #endif  // PLATFORM_CURRENT_LOCATION


}  // namespace os
