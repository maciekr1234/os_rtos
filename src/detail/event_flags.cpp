#include <rtos/detail/event_flags.hpp>

#include <rtos/os_kernel.hpp>


namespace os {
/**
 * Event flag constructor
 * @throw std::system_error if an error occurs
 */
event::event(mask_type mask) : m_id(0) {
    // m_id = osEventFlagsNew(NULL);
    m_id = xEventGroupCreate();
    // if (m_id == 0)
    // throw std::system_error(osError, os_category(), "osEventFlagsNew");

    if (mask != 0)
        set(mask);
}

event::event(event&& evt) noexcept : m_id(0) { swap(evt); }

/**
 * Event flag destructor
 */
event::~event() noexcept(false) {
    if (m_id) {
        vEventGroupDelete((EventGroupHandle_t)m_id)
        // osStatus_t sta = osEventFlagsDelete(m_id);

        // if (sta != osOK)
        // throw std::system_error(sta, os_category(), detail::str_error("osEventFlagsDelete", m_id));
    }
}

void event::swap(event& evt) noexcept { std::swap(m_id, evt.m_id); }

event& event::operator=(event&& evt) noexcept {
    swap(evt);
    return *this;
}

/**
 * Get current event flag pattern.
 * @throw std::system_error if an error occurs
 */
event::mask_type event::get() const {
    return (event::mask_type)xEventGroupGetBits((EventGroupHandle_t)m_id);

    // if (flags < 0)
    // throw std::system_error(flags, os_category(), detail::str_error("osEventFlagsGet", m_id));
}

/**
 * Sets an event flag.
 * @throw std::system_error if an error occurs
 */
event::mask_type event::set(mask_type mask) {
    return xEventGroupSetBits((EventGroupHandle_t)m_id, mask);
    // if (flags < 0)
    //     throw std::system_error(flags, flags_category(), detail::str_error("osEventFlagsSet", m_id));

}

/**
 * Clears an event flag.
 * @throw std::system_error if an error occurs
 */
event::mask_type event::clear(mask_type mask) {
    // int32_t flags = osEventFlagsClear((EventGroupHandle_t)m_id, mask);
    // if (flags < 0)
    //     throw std::system_error(flags, flags_category(), detail::str_error("osEventFlagsClear", m_id));

    // return flags;

    return xEventGroupClearBits((EventGroupHandle_t)m_id,mask);
    }

/**
 * Wait until an event flag is set
 * @param mask
 * @return the event flag value
 * @throw std::system_error if an error occurs
 */
event::mask_type event::wait(mask_type mask, wait_flag flg) {
    uint32_t option =
        (static_cast<unsigned int>(flg) & static_cast<unsigned int>(wait_flag::any)) ? osFlagsWaitAny : osFlagsWaitAll;
    if ((static_cast<unsigned int>(flg) & static_cast<unsigned int>(wait_flag::clear)) == 0)
        option |= osFlagsNoClear;

    int32_t flags = osEventFlagsWait((EventGroupHandle_t)m_id, mask, option, osWaitForever);
    // if (flags < 0)
        throw std::system_error(flags, flags_category(), detail::str_error("osEventFlagsWait", m_id));

    return flags;
}

/**
 * Wait until an event flag is set or a timeout occurs
 * @param mask
 * @return the event flag value
 * @throw std::system_error if an error occurs
 */
event::status event::wait_for_usec(mask_type                 mask,
                                   wait_flag                 flg,
                                   std::chrono::microseconds usec,
                                   mask_type&                flagValue) {
    // if (usec < std::chrono::microseconds::zero())
        // throw std::system_error(osErrorParameter, os_category(), "event: negative timer");

    uint32_t timeout =
        static_cast<uint32_t>((usec.count() * osKernelGetTickFreq() * std::chrono::microseconds::period::num) /
                              std::chrono::microseconds::period::den);
    if (timeout > std::numeric_limits<uint32_t>::max())
        timeout = osWaitForever;

    uint32_t option =
        (static_cast<unsigned int>(flg) & static_cast<unsigned int>(wait_flag::any)) ? osFlagsWaitAny : osFlagsWaitAll;
    if ((static_cast<unsigned int>(flg) & static_cast<unsigned int>(wait_flag::clear)) == 0)
        option |= osFlagsNoClear;

    flagValue = osEventFlagsWait(m_id, mask, option, timeout);
    if (flagValue < 0 && flagValue != osErrorTimeout)
        throw std::system_error(flagValue, flags_category(), detail::str_error("osEventFlagsWait", m_id));

    return (flagValue == osErrorTimeout ? status::timeout : status::no_timeout);
}
} // namespace os
