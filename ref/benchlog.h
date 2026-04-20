#ifndef BENCHLOG_H
#define BENCHLOG_H

#include <stdint.h>

void bench_reset(void);
void bench_add_ntt(uint64_t cycles);
void bench_add_poly_mul(uint64_t cycles);
void bench_add_encaps(uint64_t cycles);
void bench_add_decaps(uint64_t cycles);

uint64_t bench_get_ntt(void);
uint64_t bench_get_polymul(void);
uint64_t bench_get_encaps(void);
uint64_t bench_get_decaps(void);

/* Print and clear */
void bench_print(const char *tag);

#endif