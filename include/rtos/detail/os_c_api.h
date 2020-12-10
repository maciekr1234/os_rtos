#pragma once
#include <rtos/os_kernel.hpp>

#ifdef __cplusplus
extern "C" {
#endif


os_status_t os_delay(uint32_t ticks);
os_status_t os_delay_until(uint32_t ticks);

os_status_t os_semaphore_acquire(SemaphoreHandle_t, uint32_t);
os_status_t os_semaphore_release(SemaphoreHandle_t);

os_status_t os_mutex_acquire(SemaphoreHandle_t, uint32_t);
os_status_t os_mutex_acquire_recursive(SemaphoreHandle_t, uint32_t);

os_status_t os_mutex_release(SemaphoreHandle_t);
os_status_t os_mutex_release_recursive(SemaphoreHandle_t);

/* threads ------------------------------------------------------------------ */

uint32_t os_thread_flags_set(TaskHandle_t thread_id, uint32_t flags);
uint32_t os_thread_flags_clear(uint32_t flags);
uint32_t os_thread_flags_get(void);
uint32_t os_thread_flags_wait(uint32_t flags, uint32_t options, uint32_t timeout);


#ifdef __cplusplus
}
#endif
