#pragma once

#include <rtos/os_types.hpp>
#include <rtos/os_kernel.hpp>


namespace os::rtos {


class critical_section : private non_copyable<critical_section>, non_moveable<critical_section> {
public:
    critical_section() noexcept : m_flag() {
        if (IS_IRQ()){
            m_flag = critical_enter_isr();
        }else{
            critical_enter();}
    }

    ~critical_section() noexcept {
        if (IS_IRQ()) {
            critical_exit_isr(m_flag);
        } else {
            critical_exit();
        }
    }

private:
    static void        critical_enter() { return taskENTER_CRITICAL(); }
    static void        critical_exit() { return taskEXIT_CRITICAL(); }
    static lock_flag_t critical_enter_isr() { return taskENTER_CRITICAL_FROM_ISR(); }
    static void        critical_exit_isr(lock_flag_t lock) { taskEXIT_CRITICAL_FROM_ISR(lock); }

private:
    lock_flag_t m_flag;
};


} // namespace os::rtos
