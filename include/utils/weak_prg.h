#ifndef WEAK_PRG_H
#define WEAK_PRG_H

#include <stdint.h>
#define SEED_SIZE 42


volatile uint8_t seed[SEED_SIZE];

void set_rng_type_timer0();
void set_rng_type_tiltball();

void rng(void);
void false_rng(void);

int uECC_rng(uint8_t*, unsigned);

#endif