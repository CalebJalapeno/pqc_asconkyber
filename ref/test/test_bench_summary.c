#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "params.h"
#include "kem.h"
#include "randombytes.h"

#include "benchlog.h"

static void sort_u64(uint64_t *a, size_t n) {
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      if (a[j] < a[i]) { uint64_t t = a[i]; a[i] = a[j]; a[j] = t; }
    }
  }
}

static uint64_t median_u64(uint64_t *a, size_t n) {
  sort_u64(a, n);
  return a[n/2];
}

int main(void) {
  enum { TRIALS = 200 };

  uint8_t pk[KYBER_PUBLICKEYBYTES];
  uint8_t sk[KYBER_SECRETKEYBYTES];
  uint8_t ct[KYBER_CIPHERTEXTBYTES];
  uint8_t ss1[KYBER_SSBYTES];
  uint8_t ss2[KYBER_SSBYTES];

  // One-time keypair
  crypto_kem_keypair(pk, sk);

  uint64_t ntt_enc[TRIALS], mul_enc[TRIALS], enc[TRIALS];
  uint64_t ntt_dec[TRIALS], mul_dec[TRIALS], dec[TRIALS];

  int failures = 0;

  for (int i = 0; i < TRIALS; i++) {
    // Encaps
    crypto_kem_enc(ct, ss1, pk);
    ntt_enc[i] = bench_get_ntt();
    mul_enc[i] = bench_get_polymul();
    enc[i]     = bench_get_encaps();

    // Decaps
    crypto_kem_dec(ss2, ct, sk);
    ntt_dec[i] = bench_get_ntt();
    mul_dec[i] = bench_get_polymul();
    dec[i]     = bench_get_decaps();

    if (memcmp(ss1, ss2, KYBER_SSBYTES) != 0) failures++;
  }

  printf("KYBER_K=%d TRIALS=%d failures=%d\n", KYBER_K, TRIALS, failures);

  printf("ENC median cycles: total=%llu  ntt=%llu  polymul=%llu\n",
         (unsigned long long)median_u64(enc, TRIALS),
         (unsigned long long)median_u64(ntt_enc, TRIALS),
         (unsigned long long)median_u64(mul_enc, TRIALS));

  printf("DEC median cycles: total=%llu  ntt=%llu  polymul=%llu\n",
         (unsigned long long)median_u64(dec, TRIALS),
         (unsigned long long)median_u64(ntt_dec, TRIALS),
         (unsigned long long)median_u64(mul_dec, TRIALS));

  return (failures == 0) ? 0 : 1;
}