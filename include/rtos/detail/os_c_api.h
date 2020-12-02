#pragma once
#include <rtos/os_kernel.hpp>

#ifdef __cplusplus
extern "C" {
#endif

os_status_t os_semaphore_acquire(SemaphoreHandle_t, uint32_t);
os_status_t os_semaphore_release(SemaphoreHandle_t);
os_status_t os_mutex_acquire(SemaphoreHandle_t, uint32_t);
os_status_t os_mutex_acquire_recursive(SemaphoreHandle_t, uint32_t);
os_status_t os_mutex_release(SemaphoreHandle_t);
os_status_t os_mutex_release_recursive(SemaphoreHandle_t);




#ifdef __cplusplus
}
#endif
