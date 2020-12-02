
#include <rtos/os_kernel.hpp>
#include <os/os_assert.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

extern "C" {
void _os_assert_failed(const char* expr, const char* file, int line) {
    vPortEnterCritical();
    fprintf(stderr, "%s:%d: Assertion '%s' failed!\n",file, line, expr);
    abort();
}

}
