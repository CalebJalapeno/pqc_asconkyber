#include "benchlog.h"
#include <stdio.h>

static uint64_t g_ntt = 0;
static uint64_t g_polymul = 0;
static uint64_t g_encaps = 0;
static uint64_t g_decaps = 0;

void bench_reset(void) {
  g_ntt = g_polymul = g_encaps = g_decaps = 0;
}

void bench_add_ntt(uint64_t c) { g_ntt += c; }
void bench_add_poly_mul(uint64_t c) { g_polymul += c; }
void bench_add_encaps(uint64_t c) { g_encaps += c; }
void bench_add_decaps(uint64_t c) { g_decaps += c; }

uint64_t bench_get_ntt(void) { return g_ntt; }
uint64_t bench_get_polymul(void) { return g_polymul; }
uint64_t bench_get_encaps(void) { return g_encaps; }
uint64_t bench_get_decaps(void) { return g_decaps; }

void bench_print(const char *tag) {
  printf("[BENCH] %s ntt_cycles=%llu polymul_cycles=%llu encaps_cycles=%llu decaps_cycles=%llu\n",
         tag,
         (unsigned long long)g_ntt,
         (unsigned long long)g_polymul,
         (unsigned long long)g_encaps,
         (unsigned long long)g_decaps);
}