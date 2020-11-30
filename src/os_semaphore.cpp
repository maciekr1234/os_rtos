// #include <rtos/os_semaphore.hpp>
// #include <rtos/os_kernel.hpp>

// #include <string.h>

// using namespace std::chrono_literals;
// using std::milli;
// using std::chrono::duration;

// namespace os::rtos {

// semaphore::semaphore(int32_t count) { constructor(count, 0xffff); }

// semaphore::semaphore(int32_t count, uint16_t max_count) { constructor(count, max_count); }

// void semaphore::constructor(int32_t count, uint16_t max_count) {
//     if (count == 1) {
//         m_id = xSemaphoreCreateBinary();
//     } else {
//         m_id = xSemaphoreCreateCounting(max_count, count);
//     }
// }

// bool semaphore::try_acquire(int32_t timeout) {
//     osStatus_t stat = osOK;
//     BaseType_t yield;

//     if (IS_IRQ()) {
//         if (timeout != 0U) {
//             stat = osErrorParameter;
//         } else {
//             yield = pdFALSE;
//             if (xSemaphoreTakeFromISR(m_id, &yield) !=pdPASS) {
//                 stat = osErrorResource;
//             } else {
//                 portYIELD_FROM_ISR(yield);
//             }
//         }
//     } else {
//         if (xSemaphoreTake(m_id, (TickType_t)timeout) != pdPASS) {
//             if (timeout != 0U) {
//                 stat = osErrorTimeout;
//             } else {
//                 stat = osErrorResource;
//             }
//         }
//     }
//     return (stat == osOK);
// }

// void semaphore::acquire() {
//     try_acquire(osWaitForever);
//     // xSemaphoreTake(m_id, osWaitForever);
// }

// bool semaphore::try_acquire_for(uint32_t millisec) {
//      return try_acquire_for(duration<uint32_t, milli>(millisec)); }

// bool semaphore::try_acquire_for(kernel::clock::duration_u32 rel_time) {
//     // return (xSemaphoreTake(m_id, rel_time.count()) == pdTRUE);
//     return try_acquire(rel_time.count());
// }

// bool semaphore::try_acquire_until(uint64_t millisec) {
//     return try_acquire_until(kernel::clock::time_point(duration<uint64_t, milli>(millisec)));
// }

// bool semaphore::try_acquire_until(kernel::clock::time_point abs_time) {
//     auto now = kernel::clock::now();

//     if (now >= abs_time) {
//         return try_acquire();
//     } else if (abs_time - now > kernel::wait_for_u32_max) {
//         // API permits early return
//         return try_acquire_for(kernel::wait_for_u32_max);
//     } else {
//         return try_acquire_for(abs_time - now);
//     }
// }


// osStatus_t semaphore::release(void) {
//     osStatus_t stat = osOK;
//     BaseType_t yield;

//     if (IS_IRQ()) {
//         yield = pdFALSE;
//         if (xSemaphoreGiveFromISR(m_id, &yield) != pdTRUE) {
//             stat = osErrorResource;
//         } else {
//             portYIELD_FROM_ISR(yield);
//         }
//     } else {
//         if (xSemaphoreGive(m_id) != pdPASS) {
//             stat = osErrorResource;
//         }
//     }

//     return stat;
// }

// semaphore::~semaphore() {
//     if (!IS_IRQ())
//         vSemaphoreDelete(m_id);
// }

// } // namespace os::rtos
