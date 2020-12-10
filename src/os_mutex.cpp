
#include <rtos/os_kernel.hpp>
#include <rtos/os_mutex.hpp>

#include <rtos/detail/os_c_api.h>

using namespace std::chrono_literals;
using std::chrono::duration;

namespace os::rtos {


SemaphoreHandle_t mutex::constructor(bool recursive) {

    if (recursive) {
        _mutex = xSemaphoreCreateRecursiveMutex();
    } else {
        _mutex = xSemaphoreCreateMutex();
    }
    return _mutex;
}

mutex::~mutex() {
    if (!kernel::is_irq()) {
        vSemaphoreDelete(_mutex);
    }
}

void mutex::lock() { try_lock_for(wait_for_u32_forever); }

bool mutex::try_lock() { return try_lock_for(0s); }

bool mutex::try_lock_for(kernel::clock::duration_u32 rel_time) {
    configASSERT(!kernel::is_irq());

    BaseType_t ret;

    if (_is_recursive) {
        ret = xSemaphoreTakeRecursive(_mutex, static_cast<TickType_t>(rel_time.count()));
    } else {
        ret = xSemaphoreTake(_mutex, static_cast<TickType_t>(rel_time.count()));
    }
    if (ret == pdPASS) {
        _count++;

        return (true);
    }
    return (false);
}

bool mutex::try_lock_until(kernel::clock::time_point abs_time) {
    auto now = kernel::clock::now();

    if (now >= abs_time) {
        return try_lock();
    } else if (abs_time - now > wait_for_u32_max) {
        return try_lock_for(wait_for_u32_max);
    } else {
        return try_lock_for(abs_time - now);
    }
}

void mutex::unlock() {
    configASSERT(!kernel::is_irq());

    _count--;

    if (_is_recursive) {
        configASSERT(xSemaphoreGiveRecursive(_mutex));
    } else {
        configASSERT(xSemaphoreGive(_mutex));
    }
}


} // namespace os::rtos
