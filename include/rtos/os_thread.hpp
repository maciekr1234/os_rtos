#pragma once

#include <rtos/os_types.hpp>


namespace os::rtos {


// class thread : private non_copyable<thread> {
// public:
//     thread(const char*  name,
//            osPriority_t priority  = osPriorityNormal,
//            uint32_t     stacksize = configMINIMAL_STACK_SIZE,
//            uint32_t     attr      = osThreadDetached);


//     thread(const os_thread_attr_t* attr);

//     virtual ~thread();


// public:
//     virtual void run() = 0;

// private:
//     static void handler(void* _thread) {
//         thread* handle = (thread*)_thread;

//         if (handle)
//             handle->run();
//     }


//     void constructor(const char*    name       = nullptr,
//                      osPriority_t   priority   = osPriorityNormal,
//                      uint32_t       stack_size = configMINIMAL_STACK_SIZE,
//                      uint32_t attr = osThreadDetached);

// private:
//     // osThreadFunc_t   m_task;

//     osThreadId_t     m_id;
//     os_thread_attr_t m_attr;
// };


} // namespace os::rtos
