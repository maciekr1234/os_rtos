/**
 * \file event_flags.hpp
 * \date 06-12-2020
 */
#pragma once

#include <rtos/os_kernel.hpp>


namespace os::rtos {

/**
 * \brief The event_flags class is used to control event flags or wait for event flags other threads control.
 *
 * \note  event_flags support 31 flags. The MSB flag is ignored. It is used to return an error code (\a
 * os::flags_error).
 */
class event_flags : private non_copyable<event_flags> {
public:
    event_flags();

    ~event_flags();

    /**
     * \brief  Set the specified event flags.
     *
     * \param flags the flags that will be set.
     * \return event flags after setting or error code if highest bit set
     */
    uint32_t set(uint32_t flags);

    /**
     * \brief Clear the specified event flags.
     *
     * \param flags the flags that will be cleared (default: 0x7fffffff -- all flags).
     * \return event flags before clearing or error code if highest bit set.
     */
    uint32_t clear(uint32_t flags = 0x7fffffff);

    /**
     * \brief Get currently set flags.
     *
     * \return current event flags.
     */
    uint32_t get() const;

    /**
     * \brief  Wait for all of the specified event flags to become signaled.
     *
     * \param flags     the flags to wait for (default: 0 -- no flags).
     * \param millisec  timeout value. (default: os::wait_forever)
     * \param clear     clear specified event flags after waiting for them (default: true).
     * \return event flags before clearing or error code if highest bit set.
     */
    uint32_t wait_all(uint32_t flags = 0, uint32_t millisec = os::wait_forever, bool clear = true);

    /**
     * \brief  Wait for all of the specified event flags to become signaled.
     *
     * \param flags     the flags to wait for.
     * \param rel_time  timeout value.
     * \param clear     clear specified event flags after waiting for them (default: true).
     * \return event flags before clearing or error code if highest bit set.
     */
    uint32_t wait_all_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear = true);

    /**
     * \brief  Wait for all of the specified event flags to become signaled.
     *
     * \param flags     the flags to wait for.
     * \param abs_time  timeout value.
     * \param clear     clear specified event flags after waiting for them (default: true).
     * \return event flags before clearing or error code if highest bit set.
     */
    uint32_t wait_all_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear = true);

    /**
     * \brief  Wait for any of the specified event flags to become signaled.
     *
     * \param flags     the flags to wait for (default: 0 -- no flags).
     * \param millisec  timeout value. (default: os::wait_forever)
     * \param clear     clear specified event flags after waiting for them (default: true).
     * \return event flags before clearing or error code if highest bit set.
     */
    uint32_t wait_any(uint32_t flags = 0, uint32_t millisec = os::wait_forever, bool clear = true);
    /**
     * \brief  Wait for any of the specified event flags to become signaled.
     *
     * \param flags     the flags to wait for.
     * \param rel_time  timeout value.
     * \param clear     clear specified event flags after waiting for them (default: true).
     * \return event flags before clearing or error code if highest bit set.
     */
    uint32_t wait_any_for(uint32_t flags, kernel::clock::duration_u32 rel_time, bool clear = true);
    /**
     * \brief  Wait for any of the specified event flags to become signaled.
     *
     * \param flags     the flags to wait for.
     * \param abs_time  timeout value.
     * \param clear     clear specified event flags after waiting for them (default: true).
     * \return event flags before clearing or error code if highest bit set.
     */
    uint32_t wait_any_until(uint32_t flags, kernel::clock::time_point abs_time, bool clear = true);


private:
    uint32_t wait_for(uint32_t flags, uint32_t opt, kernel::clock::duration_u32 rel_time, bool clear);
    uint32_t wait_until(uint32_t flags, uint32_t opt, kernel::clock::time_point abs_time, bool clear);

    EventGroupHandle_t m_handle;
};
} // namespace os::rtos
