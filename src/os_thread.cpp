/**
 * \brief
 *
 * \file os_thread.cpp
 * \date 26-11-2020
 */
#include <rtos/os_kernel.hpp>
#include <rtos/os_thread.hpp>

#include <utility>


namespace os::rtos {


thread::thread(const char* name, thread::priority prio, size_t stack_size)
    : m_id(0), m_mutex(), m_name(name), m_priority(prio), m_stack_size(stack_size), m_finished(false) {
    if (xTaskCreate(static_cast<TaskFunction_t>(&handler), m_name, stack_size, this, static_cast<UBaseType_t>(prio),
                    &m_id) != pdPASS) {
        m_id = nullptr;
    }
}

thread::~thread() { terminate(); }

os_status_t thread::terminate() {
    status result = status::ok;
    m_mutex.lock();

    auto local_handle = m_id;
    m_id              = nullptr;
    if (!m_finished) {
        m_finished = true;
        if (local_handle != 0) {
            vTaskDelete(local_handle);
        }
    }

    m_mutex.unlock();
    return result;
}

os_status_t thread::set_priority(thread::priority prio) {
    m_mutex.lock();
    vTaskPrioritySet(m_id, (UBaseType_t)prio);
    m_mutex.unlock();
    return os::status::ok;
}

thread::priority thread::get_priority() const {
    thread::priority result;

    m_mutex.lock();
    if (kernel::is_irq()) {
        result = (priority)uxTaskPriorityGetFromISR(m_id);
    } else {
        result = (priority)uxTaskPriorityGet(m_id);
    }

    m_mutex.unlock();

    return result;
}

thread::state thread::get_state() const {
    eTaskState    task_state;
    thread::state result;
    m_mutex.lock();
    if (m_id != nullptr) {
        task_state = eTaskGetState(m_id);
    }
    m_mutex.unlock();

    switch (task_state) {
        case eRunning:
            result = state::running;
            break;
        case eReady:
            result = state::ready;
            break;
        case eBlocked:
        case eSuspended:
            result = state::blocked;
            break;
        case eDeleted:
            result = state::terminated;
            break;
        case eInvalid:
        default:
            result = state::error;
            break;
    };

    return result;
}

os_thread_id thread::get_id() const { return m_id; }

} // namespace os::rtos
