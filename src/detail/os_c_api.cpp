#include <rtos/detail/os_c_api.h>
#include <rtos/os_types.hpp>
extern "C" {
//
os_status_t os_delay(uint32_t ticks) {
    os_status_t status = os::status::ok;

    if (IS_IRQ()) {
        status = os::status::error_isr;
    } else {
        if (ticks != 0U) {
            vTaskDelay(ticks);
        }
    }
    return (status);
}

os_status_t os_delay_until(uint32_t ticks) {
    TickType_t  tcnt, delay;
    os_status_t status = os::status::ok;

    if (IS_IRQ()) {
        status = os::status::error_isr;
    } else {
        tcnt = xTaskGetTickCount();

        /* Determine remaining number of ticks to delay */
        delay = (TickType_t)ticks - tcnt;

        /* Check if target tick has not expired */
        if ((delay != 0U) && (0 == (delay >> (8 * sizeof(TickType_t) - 1)))) {
            vTaskDelayUntil(&tcnt, delay);
        } else {
            status = os::status::error_parameter;
        }
    }
    return (status);
}

//

os_status_t os_semaphore_acquire(SemaphoreHandle_t semaphore_id, uint32_t timeout) {
    os_status_t status = os::status::ok;
    BaseType_t  yield;

    if (IS_IRQ()) {
        if (timeout != 0U) {
            status = os::status::error_parameter;
        } else {
            yield = pdFALSE;
            if (xSemaphoreTakeFromISR(semaphore_id, &yield) != pdPASS) {
                status = os::status::error_resource;
            } else {
                portYIELD_FROM_ISR(yield);
            }
        }
    } else {
        if (xSemaphoreTake(semaphore_id, (TickType_t)timeout) != pdPASS) {
            if (timeout != 0U) {
                status = os::status::error_timeout;
            } else {
                status = os::status::error_resource;
            }
        }
    }
    return status;
}

os_status_t os_semaphore_release(SemaphoreHandle_t semaphore_id) {
    os_status_t status = os::status::ok;
    BaseType_t  yield  = pdFALSE;

    if (IS_IRQ()) {
        if (xSemaphoreGiveFromISR(semaphore_id, &yield) != pdTRUE) {
            return os::status::error;
        }
        portYIELD_FROM_ISR(yield);
    } else {
        if (xSemaphoreGive(semaphore_id) != pdTRUE) {
            status = os::status::error;
        }
    }
    return status;
}


os_status_t os_mutex_acquire(SemaphoreHandle_t mutex_id, uint32_t timeout) {
    os_status_t status = os::status::ok;


    if (IS_IRQ()) {
        status = os::status::error_isr;
    } else {
        if (xSemaphoreTake(mutex_id, timeout) != pdPASS) {
            if (timeout != 0) {
                status = os::status::error_timeout;
            } else {
                status = os::status::error_resource;
            }
        }
    }
    return status;
}


os_status_t os_mutex_release(SemaphoreHandle_t mutex_id) {
    os_status_t status = os::status::ok;

    if (IS_IRQ()) {
        status = os::status::error_isr;
    } else {
        if (xSemaphoreGive(mutex_id) != pdPASS) {
            status = os::status::error_resource;
        }
    }
    return status;
}

os_status_t os_mutex_acquire_recursive(SemaphoreHandle_t mutex_id, uint32_t timeout) {
    os_status_t status = os::status::ok;

    if (IS_IRQ()) {
        status = os::status::error_isr;
    } else {
        if (xSemaphoreTakeRecursive(mutex_id, timeout) != pdPASS) {
            if (timeout != 0) {
                status = os::status::error_timeout;
            } else {
                status = os::status::error_resource;
            }
        }
    }
    return status;
}


os_status_t os_mutex_release_recursive(SemaphoreHandle_t mutex_id) {
    os_status_t status = os::status::ok;

    if (IS_IRQ()) {
        status = os::status::error_isr;
    } else {
        if (xSemaphoreGiveRecursive(mutex_id) != pdPASS) {
            status = os::status::error_resource;
        }
    }

    return status;
}


/* threads -------------------------------------------------------------------*/

uint32_t os_thread_flags_set(TaskHandle_t thread_id, uint32_t flags) {
    uint32_t   rflags;
    BaseType_t yield;

    if (thread_id == nullptr) {
        rflags = (uint32_t)os::status::error_parameter;
    } else {
        rflags = (uint32_t)os::status::error;
        if (IS_IRQ()) {
            yield = pdFALSE;

            (void)xTaskNotifyFromISR(thread_id, flags, eSetBits, &yield);
            (void)xTaskNotifyAndQueryFromISR(thread_id, 0, eNoAction, &rflags, NULL);

            portYIELD_FROM_ISR(yield);
        } else {
            (void)xTaskNotify(thread_id, flags, eSetBits);
            (void)xTaskNotifyAndQuery(thread_id, 0, eNoAction, &rflags);
        }
    }
    /* Return flags after setting */
    return (rflags);
}

uint32_t os_thread_flags_clear(uint32_t flags) {
    TaskHandle_t hTask;
    uint32_t     rflags, cflags;

    if (IS_IRQ()) {
        rflags = (uint32_t)os::status::error_isr;
    } else {
        hTask = xTaskGetCurrentTaskHandle();

        if (xTaskNotifyAndQuery(hTask, 0, eNoAction, &cflags) == pdPASS) {
            rflags = cflags;
            cflags &= ~flags;

            if (xTaskNotify(hTask, cflags, eSetValueWithOverwrite) != pdPASS) {
                rflags = (uint32_t)os::status::error;
            }
        } else {
            rflags = (uint32_t)os::status::error;
        }
    }
    /* Return flags before clearing */
    return (rflags);
}


uint32_t os_thread_flags_get(void) {
    TaskHandle_t hTask;
    uint32_t     rflags;

    if (IS_IRQ()) {
        rflags = (uint32_t)os::status::error_isr;
    } else {
        hTask = xTaskGetCurrentTaskHandle();

        if (xTaskNotifyAndQuery(hTask, 0, eNoAction, &rflags) != pdPASS) {
            rflags = (uint32_t)os::status::error;
        }
    }
    return (rflags);
}


uint32_t os_thread_flags_wait(uint32_t flags, uint32_t options, uint32_t timeout) {
    uint32_t   rflags, nval;
    uint32_t   clear;
    TickType_t t0, td, tout;
    BaseType_t rval;

    if (IS_IRQ()) {
        rflags = (uint32_t)os::status::error_isr;
    } else {
        if ((options & os::flags_no_clear) == os::flags_no_clear) {
            clear = 0U;
        } else {
            clear = flags;
        }

        rflags = 0U;
        tout   = timeout;

        t0 = xTaskGetTickCount();
        do {
            rval = xTaskNotifyWait(0, clear, &nval, tout);

            if (rval == pdPASS) {
                rflags &= flags;
                rflags |= nval;

                if ((options & os::flags_wait_all) == os::flags_wait_all) {
                    if ((flags & rflags) == flags) {
                        break;
                    } else {
                        if (timeout == 0U) {
                            rflags = (uint32_t)os::status::error_resource;
                            break;
                        }
                    }
                } else {
                    if ((flags & rflags) != 0) {
                        break;
                    } else {
                        if (timeout == 0U) {
                            rflags = (uint32_t)os::status::error_resource;
                            break;
                        }
                    }
                }

                /* Update timeout */
                td = xTaskGetTickCount() - t0;

                if (td > tout) {
                    tout = 0;
                } else {
                    tout -= td;
                }
            } else {
                if (timeout == 0) {
                    rflags = (uint32_t)os::status::error;
                } else {
                    rflags = (uint32_t)os::status::error_timeout;
                }
            }
        } while (rval != pdFAIL);
    }

    /* Return flags before clearing */
    return (rflags);
}
}
