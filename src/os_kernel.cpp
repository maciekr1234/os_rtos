#include <stdlib.h>


#include <rtos/os_kernel.hpp>
// #include <STM32FreeRTOS.h>


extern "C" uint32_t SystemCoreClock;             /**< System Clock Frequency (Core Clock) */
extern "C" void     SystemCoreClockUpdate(void); /**< Updates the variable SystemCoreClock */


int IS_IRQ(void) { return os::rtos::kernel::is_irq(); }

namespace os::rtos {

/*----------------------------------------------------------------------------
 * kernel
 *---------------------------------------------------------------------------*/

os_status_t kernel::init() {
    status ret;
    //
    if (is_irq()) {
        ret = status::error_isr;
    } else {
        if (get_state() == state::inactive) {
            set_state(state::ready);
            ret = status::ok;

        } else {
            ret = state::error;
        }
    }


    //

    return ret;
}

os_status_t kernel::start() {
    status ret;
    //
    if (is_irq()) {
        ret = status::error_isr;
    } else {
        if (get_state() == state::ready) {
            set_state(state::running);
            vTaskStartScheduler();
            ret = status::ok;
        } else {
            ret = status::error;
        }
    }
    return ret;
}

kernel::state kernel::get_state() {
    switch (xTaskGetSchedulerState()) {
        case taskSCHEDULER_RUNNING:
            s_state = state::running;
            break;
        case taskSCHEDULER_SUSPENDED:
            s_state = state::locked;
            break;
        case taskSCHEDULER_NOT_STARTED:
        default:
            return state::inactive;
            break;
    }
    return s_state;
}

lock_flag_t kernel::lock() {
    lock_flag_t lock;
    if (is_irq()) {
        lock = (lock_flag_t)status::error_isr;
    } else {
        switch (xTaskGetSchedulerState()) {
            case taskSCHEDULER_SUSPENDED:
                lock = 1;
                break;
            case taskSCHEDULER_RUNNING:
                vTaskSuspendAll();
                lock = 0;
                break;
            case taskSCHEDULER_NOT_STARTED:
            default:
                lock = (lock_flag_t)status::error;
                break;
        }
    }
    return lock;
}

lock_flag_t kernel::unlock() {
    lock_flag_t lock;
    if (is_irq()) {
        lock = (lock_flag_t)status::error_isr;
    } else {
        switch (xTaskGetSchedulerState()) {
            case taskSCHEDULER_SUSPENDED:
                lock = 1;
                if (xTaskResumeAll() != pdTRUE) {
                    if (xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED) {
                        lock = (lock_flag_t)status::error;
                    }
                }
                break;
            case taskSCHEDULER_RUNNING:
                lock = 0;
                break;
            case taskSCHEDULER_NOT_STARTED:
            default:
                lock = (lock_flag_t)status::error;
                break;
        }
    }
    return lock;
}

lock_flag_t kernel::restore_lock(lock_flag_t lock) {
    if (is_irq()) {
        lock = (lock_flag_t)status::error_isr;
    } else {
        switch (xTaskGetSchedulerState()) {
            case taskSCHEDULER_SUSPENDED:
            case taskSCHEDULER_RUNNING:
                if (lock == 1) {
                    vTaskSuspendAll();
                } else {
                    if (xTaskResumeAll() != pdTRUE) {
                        if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING) {
                            lock = (lock_flag_t)status::error;
                        }
                    }
                }
                break;
            case taskSCHEDULER_NOT_STARTED:
            default:
                lock = (lock_flag_t)status::error;
                break;
        }
    }
    return lock;
}

bool kernel::is_irq() { return (IS_IRQ_MODE() || (IS_IRQ_MASKED() && get_state() == state::running)); }

void kernel::set_state(kernel::state _state) {
    if (_state != s_state) {
        s_state = _state;
    }
}

/*----------------------------------------------------------------------------
 * tick conversion.
 *---------------------------------------------------------------------------*/


TickType_t kernel::get_native_tick_count() { return ((IS_IRQ()) ? xTaskGetTickCountFromISR() : xTaskGetTickCount()); }

uint64_t kernel::get_tick_count() {
    if (sizeof get_native_tick_count() == sizeof(uint64_t)) {
        return get_native_tick_count();
    } else { /* assume 32-bit */

        // Based on suggestion in CMSIS-RTOS 2.1.1 docs, but with reentrancy
        // protection for the tick memory. We use critical section rather than a
        // mutex, as hopefully this method can be callable from interrupt later -
        // only thing currently preventing it is that pre CMSIS RTOS 2.1.1, it's
        // not defined as safe.
        // We assume this is called multiple times per 32-bit wrap period (49 days).
        static uint32_t tick_h, tick_l;

        uint64_t ret;
        {
            auto     _cs    = critical_section();
            uint32_t tick32 = get_native_tick_count();
            if (tick32 < tick_l) {
                tick_h++;
            }
            tick_l = tick32;
            ret    = ((uint64_t)tick_h << 32) | tick_l;

            // os_critical_exit();
        }
        return ret;
    }
}


} // namespace os::rtos
