#pragma once

#include <rtos/os_kernel.hpp>
#include <rtos/os_semaphore.hpp>
#include <rtos/os_mutex.hpp>


#include <string>

namespace os::rtos {


class thread : private non_copyable<thread> {
public:
    enum class priority : int8_t {
        idle         = 0,
        low          = 1,
        below_normal = 2,
        normal       = 3,
        above_normal = 4,
        high         = 5,
        realtime     = 6,
        isr          = 7,
        error        = -1,
    };

    enum class state : int8_t {
        inactive   = 0,
        ready      = 1,
        running    = 2,
        blocked    = 3,
        terminated = 4,
        error      = -1,
    };

    typedef TaskHandle_t id;
public:
    thread(const std::string& name,
           thread::priority   prio       = priority::normal,
           size_t             stack_size = configMINIMAL_STACK_SIZE)
        : thread(name.c_str(), prio, stack_size) {}

    thread(const char*      name       = nullptr,
           thread::priority prio       = priority::normal,
           size_t           stack_size = configMINIMAL_STACK_SIZE);

    virtual ~thread();

    virtual void init() = 0;
    virtual void run()  = 0;

    status start();

    status terminate();

    status set_priority(priority prio);

    priority get_priority() const;

    state get_state() const;

    size_t stack_size() const;

    size_t free_stack() const;

    size_t used_stack() const;

    size_t max_stack() const;

    const char* get_name() const { return m_name; }

    thread::id get_id() const;

private:
    static void handler(void* _thread) {
        thread* handle = static_cast<thread*>(_thread);
        handle->init();
        handle->run();
        handle->m_mutex.lock();
        handle->m_id   = nullptr;
        handle->m_finished = true;
        // rtos will release the mutex automatically
    }

private:
    thread::id m_id;


    mutable mutex m_mutex;
    const char*   m_name;
    priority      m_priority;
    size_t        m_stack_size;

    bool m_finished;
};


} // namespace os::rtos

using os_priority     = ::os::rtos::thread::priority;
using os_thread_state = ::os::rtos::thread::state;
using os_thread_id = ::os::rtos::thread::id;
