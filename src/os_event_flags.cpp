#include <rtos/os_event_flags.hpp>


using std::chrono::duration;

namespace os::rtos {

event_flags::event_flags() { m_handle = xEventGroupCreate(); }

event_flags::~event_flags() { vEventGroupDelete(m_handle); }


uint32_t event_flags::set(uint32_t flags) {
    if (IS_IRQ()) {
        // return xEventGroupSetBitsFromISR(m_handle, flags);
    }
    return xEventGroupSetBits(m_handle, flags);
}

uint32_t event_flags::clear(uint32_t flags) {
    if (IS_IRQ()) {
        return xEventGroupClearBitsFromISR(m_handle, flags);
    }
    return xEventGroupClearBits(m_handle, flags);
}

uint32_t event_flags::get() const {
    if (IS_IRQ()) {
        return xEventGroupGetBitsFromISR(m_handle);
    }
    return xEventGroupGetBits(m_handle);
}
//


uint32_t event_flags::wait_all(uint32_t flags, uint32_t millisec, bool clear) {
    return wait_all_for(flags, duration<uint32_t, std::milli>(millisec), clear);
}

uint32_t event_flags::wait_all_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear) {
    return wait_for(flags, os::flags_wait_all, rel_time, clear);
}

uint32_t event_flags::wait_all_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear) {
    return wait_until(flags, os::flags_wait_all, abs_time, clear);
}

uint32_t event_flags::wait_any(uint32_t flags, uint32_t millisec, bool clear) {
    return wait_any_for(flags, duration<uint32_t, std::milli>(millisec), clear);
}

uint32_t event_flags::wait_any_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear) {
    return wait_for(flags, os::flags_wait_any, rel_time, clear);
}

uint32_t event_flags::wait_any_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear) {
    return wait_until(flags, os::flags_wait_any, abs_time, clear);
}


// Private: ===================================================================/


uint32_t event_flags::wait_for(uint32_t flags, uint32_t opt, kernel::clock::duration_u32 rel_time, bool clear) {
    BaseType_t wait_all;
    BaseType_t exit_clr;
    uint32_t   rflags;

    if (clear == false) {
        opt |= os::flags_no_clear;
    }
    if (IS_IRQ()) {
        rflags = (uint32_t)os::status::error_isr;
    } else {
        wait_all = (opt & os::flags_wait_all) ? (pdTRUE) : (pdFAIL);
        exit_clr = (opt & os::flags_no_clear) ? (pdFAIL) : (pdTRUE);

        rflags = xEventGroupWaitBits(m_handle, flags, exit_clr, wait_all, rel_time.count());

        if (opt & os::flags_wait_all) {
            if (flags != rflags) {
                if (rel_time.count() > 0U) {
                    rflags = (uint32_t)os::status::error_timeout;
                } else {
                    rflags = (uint32_t)os::status::error_resource;
                }
            }
        } else {
            if ((flags & rflags) == 0U) {
                if (rel_time.count() > 0U) {
                    rflags = (uint32_t)os::status::error_timeout;
                } else {
                    rflags = (uint32_t)os::status::error_resource;
                }
            }
        }
    }
    return (rflags);
}

uint32_t event_flags::wait_until(uint32_t flags, uint32_t opt, kernel::clock::time_point abs_time, bool clear) {
    kernel::clock::time_point now = kernel::clock::now();

    kernel::clock::duration_u32 rel_time;
    if (now >= abs_time) {
        rel_time = rel_time.zero();
    } else if (abs_time - now > wait_for_u32_max) {
        rel_time = wait_for_u32_max;
    } else {
        rel_time = abs_time - now;
    }
    return wait_for(flags, opt, rel_time, clear);
}

} // namespace os::rtos
