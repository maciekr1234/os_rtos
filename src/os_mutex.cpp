
#include <rtos/os_kernel.hpp>
#include <rtos/os_mutex.hpp>

#include <rtos/detail/os_c_api.h>
using namespace std::chrono_literals;
// using std::milli;
using std::chrono::duration;

namespace os::rtos {
namespace detail {

template <mutex_type T>
mutex_base<T>::mutex_base(const char* name) : m_id(0) {
    if (T == mutex_type::recursive) {
        m_id = xSemaphoreCreateRecursiveMutex();
    } else {
        m_id = xSemaphoreCreateMutex();
    }
}

template <mutex_type T>
mutex_base<T>::~mutex_base() {
    vSemaphoreDelete(m_id);
}


template <mutex_type T>
void mutex_base<T>::lock() {
    if (T == mutex_type::recursive) {
        os_mutex_acquire_recursive(m_id, kernel::wait_for_u32_forever.count());
    } else {
        os_mutex_acquire(m_id, kernel::wait_for_u32_forever.count());
    }
}

template <mutex_type T>
bool mutex_base<T>::try_lock() {
    return try_lock_for(0s);
}

template <mutex_type T>
bool mutex_base<T>::try_lock_for(kernel::clock::duration_u32 rel_time) {
    if (T == mutex_type::recursive) {
        return (os_mutex_acquire_recursive(m_id, rel_time.count()) == os::status::ok);
    } else {
        return (os_mutex_acquire(m_id, rel_time.count()) == os::status::ok);
    }
}

template <mutex_type T>
bool mutex_base<T>::try_lock_until(kernel::clock::time_point abs_time) {
    auto now = kernel::clock::now();

    if (now >= abs_time) {
        return try_lock();
    } else if (abs_time - now > kernel::wait_for_u32_max) {
        return try_lock_for(kernel::wait_for_u32_max);
    } else {
        return try_lock_for(abs_time - now);
    }
}

template <mutex_type T>
void mutex_base<T>::unlock() {
    if (T == mutex_type::recursive) {
        os_mutex_release_recursive(m_id);
    } else {
        os_mutex_release(m_id);
    }
}

template <mutex_type T>
TaskHandle_t mutex_base<T>::get_owner() {
    if (kernel::is_irq()) {
        return (TaskHandle_t)xSemaphoreGetMutexHolderFromISR(m_id);
    } else {
        return (TaskHandle_t)xSemaphoreGetMutexHolder(m_id);
    }
}


} // namespace detail


} // namespace os::rtos
