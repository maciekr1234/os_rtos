/**
 * \file os_critical_api.h
 */
#pragma once
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif


void os_critical_section_enter();

void os_critical_section_exit();

bool os_in_critical_section();

#ifdef __cplusplus
}
#endif
