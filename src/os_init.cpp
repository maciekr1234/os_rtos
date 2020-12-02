/**
 * \file
 *
 */
// stdlib includes.


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// framework includes:
#include <Arduino.h>
#include "board.h"


// os includes:
#include <STM32FreeRTOS.h>
#include <rtos/os_kernel.hpp>


extern "C" {

static TaskHandle_t osMainTask;

/**
 * \brief called by framework premain constructor.
 *
 */
void init() {
    hw_config_init(); // Init hardware
    os_init();
    os_rtos_init();
}

__attribute__((weak)) void os_init(void) {

#ifdef HAVE_SERIALUSB
    // SerialUSB.begin();
#endif

}

__attribute__((weak)) void os_rtos_init(void) { os::kernel::init(); }
__attribute__((weak)) void os_rtos_start(void) { os::kernel::start(); }

__attribute__((weak)) void os_main(void* args) { (void)args; }


} // extern "C"
