/**
 * \file
 *
 */
// stdlib includes.

/** C/C++ Standard Library Headers */
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <os/compiler.h>

// framework includes:
#include <Arduino.h>
// #include "board.h"


// os includes:
// #include <STM32FreeRTOS.h>
#include <FreeRTOS.h>
#include <os_rtos.hpp>

// #include <rtos/memory/malloc.h>

#ifdef __cplusplus
extern "C" {
#endif
// void* malloc(size_t size);
// void* realloc(void* ptr, size_t size);
// void* calloc(size_t ptr, size_t size);
// void  free(void* ptr);
// void* __wrap_malloc(size_t size);
// void* __wrap_realloc(void* ptr, size_t size);
// void* __wrap_calloc(size_t num, size_t size);
// void  __wrap_free(void* ptr);
// void* __wrap__malloc_r(void*, size_t size);
// void* __wrap__realloc_r(void*, void* ptr, size_t size);
// void* __wrap__calloc_r(void*, size_t num, size_t size);
// void  __wrap__free_r(void*, void* ptr);
// void  _exit(int);
// int   _kill(int, int);
// int   _getpid();
// int   _stat(const char*, struct stat*);
// int   _fstat(int, struct stat*);
// int   _isatty(int);
// int   _open(const char*, int, int);
// int   _close(int);
// int   _link(const char*, const char*);
// int   _unlink(const char*);
// int   _lseek(int, int, int);
// int   _read(int, char*, int);
// int   _write(int file, char* ptr, int len);
// void  __libc_fini_array(void);



extern void initVariant();

void initVariant() {
#ifdef HAVE_SERIALUSB
    SerialUSB.begin();
#endif

}

#if (configUSE_MALLOC_FAILED_HOOK == 1)
/** vApplicationMallocFailedHook()
Blink two short pulses if malloc fails.

will only be called if
configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
function that will get called if a call to pvPortMalloc() fails.
pvPortMalloc() is called internally by the kernel whenever a task, queue,
timer or semaphore is created.  It is also called by various parts of the
demo application.  If heap_1.c or heap_2.c are used, then the size of the
heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
to query the size of free heap space that remains (although it does not
provide information on how the remaining heap might be fragmented). */
void __attribute__((weak)) vApplicationMallocFailedHook() {}
#endif /* configUSE_MALLOC_FAILED_HOOK == 1 */

#if (configUSE_IDLE_HOOK == 1)
/** vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
task.  It is essential that code added to this hook function never attempts
to block in any way (for example, call xQueueReceive() with a block time
specified, or call vTaskDelay()).  If the application makes use of the
vTaskDelete() API function (as this demo application does) then it is also
important that vApplicationIdleHook() is permitted to return to its calling
function, because it is the responsibility of the idle task to clean up
memory allocated by the kernel to any task that has since been deleted. */
void __attribute__((weak)) vApplicationIdleHook(void) {
    // void loop();
    // loop();
}
#endif /* configUSE_IDLE_HOOK == 1 */

#if (configCHECK_FOR_STACK_OVERFLOW >= 1)
/**  Blink three short pulses if stack overflow is detected.
Run time stack overflow checking is performed if
configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
function is called if a stack overflow is detected.
\param[in] pxTask Task handle
\param[in] pcTaskName Task name
*/
void __attribute__((weak)) vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName) {
    (void)pcTaskName;
    (void)pxTask;
}
#endif /* configCHECK_FOR_STACK_OVERFLOW >= 1 */


#if (configUSE_TICK_HOOK == 1)
void __attribute__((weak)) vApplicationTickHook() {}
#endif /* configUSE_TICK_HOOK == 1 */

void          vMainConfigureTimerForRunTimeStats(void) {}
unsigned long ulMainGetRunTimeCounterValue() { return 0UL; }

#ifdef __cplusplus
}
#endif
