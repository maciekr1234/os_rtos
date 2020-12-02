/**
 * \brief
 *
 * \file os_thread.cpp
 * \date 26-11-2020
 */

#include <rtos/os_kernel.hpp>
#include <rtos/os_thread.hpp>

#include <rtos/detail/os_assert.hpp>
#include <rtos/detail/os_c_api.h>
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

uint32_t thread::flags_set(uint32_t flags) {
    flags = os_thread_flags_set(m_id, flags);
    OS_ASSERT(!(flags & osFlagsError));
    return flags;
}

thread::state thread::get_state() const {
    eTaskState    task_state = eDeleted;
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

thread::id thread::get_id() const { return m_id; }


uint32_t this_thread::flags_clear(uint32_t flags) {
    flags = os_thread_flags_clear(flags);
    OS_ASSERT(!(flags & osFlagsError));
    return flags;
}

uint32_t this_thread::flags_get() { return os_thread_flags_get(); }


static uint32_t flags_wait_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear, uint32_t options) {
    if (!clear) {
        options |= osFlagsNoClear;
    }
    flags = os_thread_flags_wait(flags, options, rel_time.count());

    if (flags & osFlagsError) {
        OS_ASSERT((flags == osFlagsErrorTimeout && rel_time != kernel::wait_for_u32_forever) ||
                  (flags == osFlagsErrorResource && rel_time == rel_time.zero()));
        flags = this_thread::flags_get();
    }


    return flags;
}

static uint32_t flags_wait(uint32_t flags, bool clear, uint32_t options) {
    return flags_wait_for(flags, kernel::wait_for_u32_forever, clear, options);
}

static uint32_t flags_wait_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear, uint32_t options) {
    kernel::clock::time_point now = kernel::clock::now();

    kernel::clock::duration_u32 rel_time;
    if (now >= abs_time) {
        rel_time = rel_time.zero();
    } else if (abs_time - now > kernel::wait_for_u32_max) {
        // Documentation permits early return for big offsets
        rel_time = kernel::wait_for_u32_max;
    } else {
        rel_time = abs_time - now;
    }
    return flags_wait_for(flags, rel_time, clear, options);
}

uint32_t this_thread::flags_wait_all(uint32_t flags, bool clear) { return flags_wait(flags, clear, osFlagsWaitAll); }


uint32_t this_thread::flags_wait_all_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear) {
    return flags_wait_for(flags, rel_time, clear, osFlagsWaitAll);
}


uint32_t this_thread::flags_wait_all_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear) {
    return flags_wait_until(flags, abs_time, clear, osFlagsWaitAll);
}

uint32_t this_thread::flags_wait_any(uint32_t flags, bool clear) { return flags_wait(flags, clear, osFlagsWaitAny); }


uint32_t this_thread::flags_wait_any_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear) {
    return flags_wait_for(flags, rel_time, clear, osFlagsWaitAny);
}


uint32_t this_thread::flags_wait_any_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear) {
    return flags_wait_until(flags, abs_time, clear, osFlagsWaitAny);
}

void this_thread::sleep_for(kernel::clock::duration_u32 rel_time) {
    os_status_t status = os_delay(rel_time.count());
    OS_ASSERT(status == os::status::ok);
}


void this_thread::sleep_until(kernel::clock::time_point abs_time) {
    kernel::clock::time_point now;

    while ((now = kernel::clock::now()) < abs_time) {
        if (abs_time - now > kernel::wait_for_u32_max) {
            os_status_t status = os_delay(kernel::wait_for_u32_max.count());
            OS_ASSERT(status == os::status::ok);
            continue;
        } else {
            os_status_t status = os_delay((abs_time - now).count());
            OS_ASSERT(status == os::status::ok);
            break;
        }
    }
}

void this_thread::yield() {
    if (!IS_IRQ()) {
        taskYIELD();
    }
}

thread::id this_thread::get_id() { return xTaskGetCurrentTaskHandle(); }

const char* this_thread::get_name() {
    thread::id id = xTaskGetCurrentTaskHandle();

    if (IS_IRQ() || (id == nullptr)) {
        return nullptr;
    } else {
        return pcTaskGetName(id);
    }
}

} // namespace os::rtos
