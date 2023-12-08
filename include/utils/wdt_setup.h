#ifndef WDT_SETUP_H
#define WDT_SETUP_H

#include <stdint.h>

void WDT_init(void);

void WDT_timeout_setup(uint8_t prescaler);

void WDT_stop(void);

#endif