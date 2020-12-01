
#include <os_rtos.hpp>
#include <Arduino.h>
using namespace os;

extern "C" {

void initVariant() {
#ifdef HAVE_SERIALUSB
    SerialUSB.begin();
#endif
    // rtos::kernel::initialize();
}
}// extern "C"
