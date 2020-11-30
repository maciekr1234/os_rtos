/**
 * \brief
 *
 * \file os_thread.cpp
 * \date 26-11-2020
 */
#include <rtos/os_kernel.hpp>
#include <rtos/os_thread.hpp>

#include <utility>
#include <functional>

// static osThreadId_t os_thread_create(osThreadFunc_t func, void* argument, const os_thread_attr_t* attr) {
//     osThreadId_t handle;

//     if (xTaskCreate((TaskFunction_t)func, (const portCHAR*)attr->name,
//                     (attr->stack_size ? attr->stack_size : configMINIMAL_STACK_SIZE), argument,
//                     (UBaseType_t)attr->priority, &handle) != pdPASS) {
//         return NULL;
//     }
//     return handle;
// }


// namespace os::rtos {

// void thread::constructor(const char* name, osPriority_t priority, uint32_t stack_size, uint32_t attr) {
//     memset(&m_attr, 0, sizeof(m_attr));
//     m_attr.name       = name ? name : "unnamed_thread";
//     m_attr.attr_bits  = attr;
//     m_attr.stack_size = stack_size;
//     m_attr.priority   = priority;


//     if (xTaskCreate((TaskFunction_t)&handler,           /** thread::handler*/
//                     (const char*)m_attr.name,           /** name */
//                     (const uint16_t)m_attr.stack_size,  /** stack size */
//                     this,                               /** argument*/
//                     (UBaseType_t)m_attr.priority, &m_id /** thread id */
//                     ) != pdPASS) {                      /**/

//         // kernel::set_errno(ENOMEM);
//         abort();
//     }
// }


// thread::thread(const char* name, osPriority_t priority, uint32_t stacksize, uint32_t attr) {
//     constructor(name, priority, stacksize, attr);
// }

// // thread::thread(const os_thread_attr_t* attr) {
// //     m_attr = {.name      = attr->name ? attr->name : "default",
// //               .priority  = attr->priority ? attr->priority : osPriorityNormal,
// //               .attr_bits = attr->attr_bits ? attr->attr_bits : 0,
// //               .stacksize = (attr->stacksize && attr->stacksize >= configMINIMAL_STACK_SIZE) ? attr->stacksize
// //                                                                                             : configMINIMAL_STACK_SIZE};
// //     m_id   = os_thread_create(&handler, this, &m_attr);
// // }

// thread::~thread() { osThreadTerminate(m_id); }


// } // namespace os::rtos
