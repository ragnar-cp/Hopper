#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "idt.h"

// Initialize PIT to a specific frequency (in Hz)
void init_timer(uint32_t frequency);

#endif // TIMER_H
