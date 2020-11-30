// #include <rtos/os_cv.hpp>
// #include <rtos/os_semaphore.hpp>


// namespace os::rtos {
// void condition_variable::notify_one() noexcept {
//     std::lock_guard<rtos::mutex> lg(m_mutex);
//     if (!m_wait.empty()) {
//         m_wait.front()->release();
//         m_wait.pop_front();
//     }
// }

// void condition_variable::notify_all() noexcept {
//     std::lock_guard<rtos::mutex> lg(m_mutex);
//     for (auto psema : m_wait)
//         psema->release();

//     m_wait.clear();
// }

// void condition_variable::wait(std::unique_lock<rtos::mutex>& lock) { wait_for(lock, std::chrono::microseconds::max()); }

// rtos::cv_status condition_variable::wait_for_usec(std::unique_lock<rtos::mutex>& lock, std::chrono::microseconds usec) {
//     if (!lock.owns_lock())
//         std::terminate();

//     rtos::binary_semaphore                       sema(0);
//     std::list<rtos::binary_semaphore*>::iterator it;
//     {
//         std::lock_guard<os::rtos::mutex> lg(m_mutex);
//         it = m_wait.insert(m_wait.end(), &sema);
//     }

//     lock.unlock();
//     bool st = sema.try_acquire_for(usec);
//     lock.lock();

//     if (!st) {
//         std::lock_guard<os::rtos::mutex> lg(m_mutex);
//         m_wait.erase(it);
//         return rtos::cv_status::timeout;
//     }

//     return rtos::cv_status::no_timeout;
// }


// } // namespace os::rtos
