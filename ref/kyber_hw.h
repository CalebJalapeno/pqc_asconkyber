#ifndef KYBER_HW_H
#define KYBER_HW_H

#include <stdint.h>

/* Hardware hook stubs (software fallback for now) */
void kyber_ntt_hw(int16_t *poly);
void kyber_invntt_hw(int16_t *poly);

#endif