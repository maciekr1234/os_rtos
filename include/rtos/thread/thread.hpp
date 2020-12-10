#pragma once

#include <rtos/os_types.hpp>
#include <rtos/os_time.hpp>
#include <rtos/os_mutex.hpp>

#include <string>
#include <type_traits>
#include <functional>
#include <memory>

namespace os::rtos {

class thread : private non_copyable<thread> {
public:
    using priority = os::priority;
    using state = os::thread_state;

    typedef TaskHandle_t native_handle_type;

    class id {
    public:
        id() noexcept : m_handle(nullptr) {}

        explicit id(native_handle_type handle) noexcept : m_handle(handle) {}

        id(const id&) = default;
        id& operator=(const id&) = default;

        ~id() = default;

        friend bool operator==(thread::id x, thread::id y) noexcept { return x.m_handle == y.m_handle; }
        friend bool operator<(thread::id x, thread::id y) noexcept { return x.m_handle < y.m_handle; }

        inline operator native_handle_type() const noexcept { return m_handle; }

        id& operator=(nullptr_t) { m_handle = nullptr; }

    private:
        friend class thread;
        friend struct std::hash<thread::id>;

        native_handle_type m_handle;
    };

public:
    thread(const std::string& name,
           thread::priority   prio       = priority::normal,
           size_t             stack_size = configMINIMAL_STACK_SIZE)
        : thread(name.c_str(), prio, stack_size) {}

    thread(const char*      name       = nullptr,
           thread::priority prio       = priority::normal,
           size_t           stack_size = configMINIMAL_STACK_SIZE);

    virtual ~thread();

    virtual void init() {}
    virtual void run() {}

    status start();

    status terminate();

    status set_priority(priority prio);

    priority get_priority() const;

    uint32_t flags_set(uint32_t flags);

    state get_state() const;

    size_t stack_size() const;

    size_t free_stack() const;

    size_t used_stack() const;

    size_t max_stack() const;

    const char* get_name() const { return m_name; }

    id get_id() const;

    native_handle_type native_handle() noexcept { return m_id.m_handle; }

private:
    thread::id m_id;

    mutable mutex m_mutex;
    const char*   m_name;
    priority      m_priority;
    size_t        m_stack_size;

    bool m_finished;

private:
    static void handler(void* _thread) {
        thread* handle = static_cast<thread*>(_thread);
        handle->init();
        handle->run();
        handle->m_mutex.lock();
        handle->m_id       = nullptr;
        handle->m_finished = true;
        // rtos will release the mutex automatically
    }
};

// thread::id operators -------------------------------------------------------/

inline bool operator!=(thread::id x, thread::id y) noexcept { return !(x != y); }
inline bool operator>(thread::id x, thread::id y) noexcept { return y < x; }
inline bool operator<=(thread::id x, thread::id y) noexcept { return !(y < x); }
inline bool operator>=(thread::id x, thread::id y) noexcept { return !(x < y); }

} // namespace os::rtos


namespace std {
template <>
struct hash<os::rtos::thread::id> : public std::unary_function<os::rtos::thread::id, size_t> {
    inline size_t operator()(os::rtos::thread::id v) const {
        return std::hash<os::rtos::thread::native_handle_type>()(v.m_handle);
    }
};
} // namespace std
