
#if 0
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
// #include <rtos/os_kernel.hpp>
#include <os/os_assert.h>

#include <rtos/memory/malloc.h>


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef FREERTOS_HEAP_REGION_CNT
#define FREERTOS_HEAP_REGION_CNT 2
#endif


extern "C" {

static const uint8_t    heap_region_max = FREERTOS_HEAP_REGION_CNT;
static volatile uint8_t heap_region_cnt = 0;
static HeapRegion_t     heap_regions[FREERTOS_HEAP_REGION_CNT + 1];

static volatile bool initialized_ = false;


static int cmp_heap(const void* a, const void* b) {
    const HeapRegion_t* ua = (HeapRegion_t*)a;
    const HeapRegion_t* ub = (HeapRegion_t*)b;

    return ((ua->pucStartAddress < ub->pucStartAddress) ? -1 : ((ua->pucStartAddress != ub->pucStartAddress)));
}

/**
 * malloc_addblock must be called before memory allocation calls are made.
 * In this FreeRTOS implementation, malloc() calls will block until memory
 * has been allocated
 */
void os_malloc_addblock(void* addr, size_t size) {
    OS_ASSERT(addr && (size > 0));
    OS_ASSERT((heap_region_cnt < heap_region_max) && "Too many heap regions!");

    // Increment the count early to claim a spot in case of multi-threads
    uint8_t cnt = heap_region_cnt++;

    if (cnt < heap_region_max) {
        // We have space - add it to the table
        heap_regions[cnt].pucStartAddress = (uint8_t*)addr;
        heap_regions[cnt].xSizeInBytes    = size;
    } else {
        // Decrement the count if we don't have space
        heap_region_cnt--;
    }
}

__attribute__((weak)) void os_malloc_init() {
    OS_ASSERT((heap_region_cnt > 0) && !initialized_);

    if (heap_region_cnt > 0 && !initialized_) {
        // Sort the heap regions so addresses are in the correct order
        qsort(heap_regions, heap_region_cnt, sizeof(HeapRegion_t), cmp_heap);

        // Pass the array into vPortDefineHeapRegions() to enable malloc()
        vPortDefineHeapRegions(heap_regions);

        // Signal to any waiting threads that we are done initializing
        initialized_ = true;
    }
}

void* __attribute__((weak))
__wrap_malloc(size_t size) {
    void* ptr = NULL;

    while (!initialized_) {
        // Thread blocks until application malloc has been correctly initialized
        vTaskDelay(1);
    }

    if (size > 0) {
        // We simply wrap the FreeRTOS call into a standard form
        ptr = pvPortMalloc(size);
    } // else NULL if there was an error

    return ptr;
}

void __attribute__((weak))
__wrap_free(void* ptr) {
    /// free should NEVER be called before malloc is init'd
    OS_ASSERT(initialized_);

    if (ptr) {
        // We simply wrap the FreeRTOS call into a standard form
        vPortFree(ptr);

    }
}

} // extern "C"
#endif
