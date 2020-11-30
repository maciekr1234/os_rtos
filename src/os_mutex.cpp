
// #include <rtos/os_kernel.hpp>
// #include <rtos/os_mutex.hpp>


// namespace os::rtos {
// namespace detail {
// base_timed_mutex::base_timed_mutex(const char* name, bool recursive) : m_id(0) {
//     if (recursive) {
//         m_id = xSemaphoreCreateRecursiveMutex();
//     } else {
//         m_id = xSemaphoreCreateMutex();
//     }
//     // if (m_id == 0)
//     // throw std::system_error(osError, os_category(), "osMutexNew");
// }

// base_timed_mutex::~base_timed_mutex() noexcept(false) {
//     vSemaphoreDelete(m_id);

//     // osStatus sta = osMutexDelete((osMutexId)m_id);
//     // if (sta != osOK)
//     // throw std::system_error(sta, os_category(), internal::str_error("osMutexDelete", (osMutexId)m_id));
// }

// void base_timed_mutex::lock() {
//     os_status_t status = os_mutex_acquire((SemaphoreHandle_t)m_id, kernel::wait_for_u32_forever.count());


//     // osStatus sta = osMutexWait((osMutexId)m_id, osWaitForever);
//     // if (sta != osOK)
//     // throw std::system_error(sta, os_category(), internal::str_error("osMutexWait ", (osMutexId)m_id));
// }

// void base_timed_mutex::unlock() {
//     os_status_t sta = os_mutex_release((SemaphoreHandle_t)m_id);
//     // if (sta != osOK)
//     // throw std::system_error(sta, os_category(), internal::str_error("osMutexRelease", (osMutexId)m_id));
// }

// bool base_timed_mutex::try_lock() {
//     os_status_t sta = os_mutex_acquire((SemaphoreHandle_t)m_id, 0);
//     // if (sta != osOK && sta != osErrorTimeoutResource)
//     // throw std::system_error(sta, os_category(), internal::str_error("osMutexWait ", (osMutexId)m_id));

//     return (sta != osErrorTimeout);
// }


// bool base_timed_mutex::try_lock_for_usec(std::chrono::microseconds usec) {
//     // if (usec < std::chrono::microseconds::zero())
//     // throw std::system_error(osErrorParameter, os_category(), "base_timed_mutex: negative timer");


//     auto now = kernel::clock::now();

//     uint32_t timeout = kernel::convert_duration<kernel::clock::duration_u32>(usec.count(), configTICK_RATE_HZ).count();

//     if (timeout > std::numeric_limits<uint32_t>::max()) {
//         timeout = kernel::wait_for_u32_forever.count();
//     } else if (usec - now > kernel::wait_for_u32_max) {
//         timeout = kernel::wait_for_u32_max.count();
//     } else {
//         timeout = kernel::convert_duration<kernel::clock::duration_u32>(usec - now, configTICK_RATE_HZ).count();
//     }

//     auto sta = os_mutex_acquire((SemaphoreHandle_t)m_id, timeout);
//     // if (sta != osOK && sta != osErrorTimeoutResource)

//     // throw std::system_error(sta, os_category(), internal::str_error("osMutexWait ", (osMutexId)m_id));

//     return (sta != osErrorTimeout);
// }
// } // namespace detail


// } // namespace os::rtos
