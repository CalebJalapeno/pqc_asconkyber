#include "kyber_hw.h"
#include "ntt.h"
#include "benchlog.h"
#include "test/cpucycles.h"

void kyber_ntt_hw(int16_t *poly) {
  uint64_t s = cpucycles();
  ntt(poly);
  bench_add_ntt(cpucycles() - s);
}

void kyber_invntt_hw(int16_t *poly) {
  uint64_t s = cpucycles();
  invntt(poly);
  bench_add_ntt(cpucycles() - s);
}