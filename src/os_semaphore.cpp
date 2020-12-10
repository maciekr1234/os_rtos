#include <rtos/os_kernel.hpp>

#include <rtos/os_semaphore.hpp>


using namespace std::chrono_literals;
using std::chrono::duration;

namespace os::rtos {


semaphore::semaphore(int32_t count, uint16_t max_count) {
    if (count == 1) {
        m_handle = xSemaphoreCreateBinary();
    } else {
        m_handle = xSemaphoreCreateCounting(max_count, count);
    }
}

semaphore::~semaphore() {
    if (!IS_IRQ()) {
        vSemaphoreDelete(m_handle);
    }
}

void semaphore::acquire() { try_acquire(portMAX_DELAY); }


bool semaphore::try_acquire(int32_t timeout) {
    os_status_t status = os::status::ok;
    BaseType_t  yield;

    if (IS_IRQ()) {
        if (timeout != 0U) {
            status = os::status::error_parameter;
        } else {
            yield = pdFALSE;
            if (xSemaphoreTakeFromISR(m_handle, &yield) != pdPASS) {
                status = os::status::error_resource;
            } else {
                portYIELD_FROM_ISR(yield);
            }
        }
    } else {
        if (xSemaphoreTake(m_handle, (TickType_t)timeout) != pdPASS) {
            if (timeout != 0U) {
                status = os::status::error_timeout;
            } else {
                status = os::status::error_resource;
            }
        }
    }
    return (status == os::status::ok);
}

template <class Rep, class Period>
bool semaphore::try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time) {
    return try_acquire(convert_duration<kernel_clock::duration_u32>(rel_time.count(), configTICK_RATE_HZ).count());
}

bool semaphore::try_acquire_for(kernel::clock::duration_u32 rel_time) {
    //
    return try_acquire(rel_time.count());
}


template <class Clock, class Duration>
bool semaphore::try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
    auto now = Clock::now();
    if (now >= abs_time) {
        return try_acquire();
    } else if (abs_time - now > wait_for_u32_max) {
        return try_acquire_for(wait_for_u32_max);
    } else {
        return try_acquire_for(abs_time - now);
    }
}


bool semaphore::try_acquire_until(kernel::clock::time_point abs_time) {
    auto now = kernel::clock::now();

    if (now >= abs_time) {
        return try_acquire();
    } else if (abs_time - now > wait_for_u32_max) {
        return try_acquire_for(wait_for_u32_max);
    } else {
        return try_acquire_for(abs_time - now);
    }
}

os_status_t semaphore::release(void) {
    os_status_t status = os::status::ok;
    BaseType_t  yield  = pdFALSE;

    if (IS_IRQ()) {
        if (xSemaphoreGiveFromISR(m_handle, &yield) != pdTRUE) {
            return os::status::error;
        }
        portYIELD_FROM_ISR(yield);
    } else {
        if (xSemaphoreGive(m_handle) != pdTRUE) {
            status = os::status::error;
        }
    }
    return status;
}



} // namespace os::rtos
