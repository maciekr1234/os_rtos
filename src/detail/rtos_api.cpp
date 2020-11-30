// #include <rtos_api.hpp>


// os_status_t os_semaphore_acquire(SemaphoreHandle_t semaphore_id, uint32_t timeout) {
//     os_status_t status = os::status::ok;
//     BaseType_t  yield;

//     if (IS_IRQ()) {
//         if (timeout != 0U) {
//             status = os::status::error_parameter;
//         } else {
//             yield = pdFALSE;
//             if (xSemaphoreTakeFromISR(semaphore_id, &yield) != pdPASS) {
//                 status = os::status::error_resource;
//             } else {
//                 portYIELD_FROM_ISR(yield);
//             }
//         }
//     } else {
//         if (xSemaphoreTake(semaphore_id, (TickType_t)timeout) != pdPASS) {
//             if (timeout != 0U) {
//                 status = os::status::error_timeout;
//             } else {
//                 status = os::status::error_resource;
//             }
//         }
//     }
//     return status;
// }

// os_status_t os_semaphore_release(SemaphoreHandle_t semaphore_id) {
//     os_status_t status = os::status::ok;
//     BaseType_t  yield  = pdFALSE;

//     if (IS_IRQ()) {
//         if (xSemaphoreGiveFromISR(semaphore_id, &yield) != pdTRUE) {
//             return os::status::error;
//         }
//         portYIELD_FROM_ISR(yield);
//     } else {
//         if (xSemaphoreGive(semaphore_id) != pdTRUE) {
//             status = os::status::error;
//         }
//     }
//     return status;
// }


// os_status_t os_mutex_acquire(SemaphoreHandle_t mutex_id, uint32_t timeout, bool reqursive) {
//     os_status_t status = os::status::ok;


//     if (IS_IRQ()) {
//         status = os::status::error_isr;
//     } else {
//         if (reqursive) {
//             if (xSemaphoreTakeRecursive(mutex_id, timeout) != pdPASS) {
//                 if (timeout != 0) {
//                     status = os::status::error_timeout;
//                 } else {
//                     status = os::status::error_resource;
//                 }
//             }
//         } else {
//             if (xSemaphoreTake(mutex_id, timeout) != pdPASS) {
//                 if (timeout != 0) {
//                     status = os::status::error_timeout;
//                 } else {
//                     status = os::status::error_resource;
//                 }
//             }
//         }
//     }
//     return status;
// }
// os_status_t os_mutex_release(SemaphoreHandle_t mutex_id, bool reqursive) {
//     os_status_t status = os::status::ok;

//     if (IS_IRQ()) {
//         status = os::status::error_isr;
//     } else {
//         if (reqursive) {
//             if (xSemaphoreGiveRecursive(mutex_id) != pdPASS) {
//                 status = os::status::error_resource;
//             }
//         } else {
//             if (xSemaphoreGive(mutex_id) != pdPASS) {
//                 status = os::status::error_resource;
//             }
//         }
//     }
//     return status;
// }
