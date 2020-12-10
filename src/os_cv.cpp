
#include <rtos/os_cv.hpp>
#include <condition_variable>

namespace os::rtos {

void condition_variable::notify_one() noexcept {
    std::lock_guard<rtos::mutex> lg(_mutex);
    if (!_wait.empty()) {
        _wait.front()->release();
        _wait.pop_front();
    }
}

void condition_variable::notify_all() noexcept {
    std::lock_guard<rtos::mutex> lg(_mutex);
    for (auto psema : _wait)
        psema->release();

    _wait.clear();
}

void condition_variable::wait(std::unique_lock<rtos::mutex>& lock) { wait_for(lock, std::chrono::microseconds::max()); }

rtos::cv_status condition_variable::wait_for_usec(std::unique_lock<rtos::mutex>& lock, std::chrono::microseconds usec) {
    if (!lock.owns_lock())
        std::terminate();


    binary_semaphore sema(0);

    std::list<binary_semaphore*>::iterator it;
    {
        std::lock_guard<rtos::mutex> lg(_mutex);
        it = _wait.insert(_wait.end(), &sema);
    }

    lock.unlock();
    bool st = sema.try_acquire_for(usec);
    lock.lock();

    if (!st) {
        std::lock_guard<rtos::mutex> lg(_mutex);
        _wait.erase(it);
        return rtos::cv_status::timeout;
    }

    return rtos::cv_status::no_timeout;
}


} // namespace os::rtos
