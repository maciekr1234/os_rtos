/**
 * \file
 *
 */
// stdlib includes.

// framework includes:
#include <Arduino.h>
#include "board.h"


// os includes:
#include <rtos/os_kernel.hpp>


extern "C" {
extern void __libc_init_array(void);


TaskHandle_t g_main_thread_handle;


/**
 * \brief called by framework premain constructor.
 *
 */
void init() {
    hw_config_init(); // Init hardware
    os_init();
    os_rtos_init();
}


void os_init(void) {
    //
}

void os_start(void) {
    //
}

void os_rtos_init(void) { os::kernel::init(); }

__attribute__((__noreturn__)) void os_rtos_start(void) {
    xTaskCreate(
        (TaskFunction_t)setup, "main", 4096, nullptr, (UBaseType_t)os::priority::normal, &g_main_thread_handle

    );

    os::kernel::start();
}

#if ( configUSE_IDLE_HOOK == 1 )
	/** vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
void  __attribute__((weak)) vApplicationIdleHook( void ) {
  void loop();
  loop();
}
#endif /* configUSE_IDLE_HOOK == 1 */

} // extern "C"
