#include <stdlib.h>

#include <rtos/os_kernel.hpp>
#include <rtos/os_time.hpp>

#include <rtos/detail/critical_section.hpp>


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
            sKernelState = state::ready;

            ret = status::ok;
        } else {
            ret = status::error;
        }
    }

    return ret;
}

os_status_t kernel::start() {
    status ret;
    //
    if (is_irq()) {
        ret = status::error_isr;
    } else {
        if (sKernelState == state::ready) {
            sKernelState = (state::running);
            vTaskStartScheduler();
            ret = status::ok;
        } else {
            ret = status::error;
        }
    }
    return ret;
}

kernel::state kernel::get_state() {
    state _state;

    switch (xTaskGetSchedulerState()) {
        case taskSCHEDULER_RUNNING:
            _state = state::running;
            break;
        case taskSCHEDULER_SUSPENDED:
            _state = state::locked;
            break;
        case taskSCHEDULER_NOT_STARTED:
        default:
            _state = state::inactive;
            break;
    }
    if(_state == state::inactive && sKernelState == state::ready) {
        return state::ready;
    }
    return _state;
}

uint32_t kernel::lock() {
    uint32_t lock;
    if (is_irq()) {
        lock = (uint32_t)status::error_isr;
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
                lock = (uint32_t)status::error;
                break;
        }
    }
    return lock;
}

uint32_t kernel::unlock() {
    uint32_t lock;
    if (is_irq()) {
        lock = (uint32_t)status::error_isr;
    } else {
        switch (xTaskGetSchedulerState()) {
            case taskSCHEDULER_SUSPENDED:
                lock = 1;
                if (xTaskResumeAll() != pdTRUE) {
                    if (xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED) {
                        lock = (uint32_t)status::error;
                    }
                }
                break;
            case taskSCHEDULER_RUNNING:
                lock = 0;
                break;
            case taskSCHEDULER_NOT_STARTED:
            default:
                lock = (uint32_t)status::error;
                break;
        }
    }
    return lock;
}

uint32_t kernel::restore_lock(uint32_t lock) {
    if (is_irq()) {
        lock = (uint32_t)status::error_isr;
    } else {
        switch (xTaskGetSchedulerState()) {
            case taskSCHEDULER_SUSPENDED:
            case taskSCHEDULER_RUNNING:
                if (lock == 1) {
                    vTaskSuspendAll();
                } else {
                    if (xTaskResumeAll() != pdTRUE) {
                        if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING) {
                            lock = (uint32_t)status::error;
                        }
                    }
                }
                break;
            case taskSCHEDULER_NOT_STARTED:
            default:
                lock = (uint32_t)status::error;
                break;
        }
    }
    return lock;
}

bool kernel::is_irq() { return (IS_IRQ_MODE() || (IS_IRQ_MASKED() && sKernelState == state::running)); }

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
        critical_section _cs();

        uint32_t tick32 = get_native_tick_count();
        if (tick32 < tick_l) {
            tick_h++;
        }
        tick_l = tick32;
        ret    = ((uint64_t)tick_h << 32) | tick_l;

        }

        return ret;
    }
}

/*----------------------------------------------------------------------------
 * kernel::clock::now()
 *---------------------------------------------------------------------------*/
kernel_clock::time_point kernel_clock::now() { //
    return time_point(duration(kernel::get_tick_count()));
}

} // namespace os::rtos
