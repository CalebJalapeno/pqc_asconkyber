#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"

#include "test/cpucycles.h"

static uint64_t median(uint64_t *a, size_t n) {
  // simple sort (n small)
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      if (a[j] < a[i]) {
        uint64_t t = a[i]; a[i] = a[j]; a[j] = t;
      }
    }
  }
  return a[n/2];
}

static void bench_ntt(void) {
  enum { N = 2000 };
  uint64_t t[N];
  poly a;

  randombytes((uint8_t*)a.coeffs, sizeof(a.coeffs));

  // warm-up
  poly_ntt(&a);
  poly_invntt_tomont(&a);

  for (int i = 0; i < N; i++) {
    randombytes((uint8_t*)a.coeffs, sizeof(a.coeffs));
    uint64_t s = cpucycles();
    poly_ntt(&a);
    t[i] = cpucycles() - s;
  }
  printf("poly_ntt:              median %llu cycles (N=%d)\n",
         (unsigned long long)median(t, N), N);

  for (int i = 0; i < N; i++) {
    randombytes((uint8_t*)a.coeffs, sizeof(a.coeffs));
    poly_ntt(&a); // inv expects NTT domain in typical flows
    uint64_t s = cpucycles();
    poly_invntt_tomont(&a);
    t[i] = cpucycles() - s;
  }
  printf("poly_invntt_tomont:    median %llu cycles (N=%d)\n",
         (unsigned long long)median(t, N), N);
}

static void bench_pointwise_mul(void) {
  enum { N = 2000 };
  uint64_t t[N];

  poly a, b, r;

  for (int i = 0; i < N; i++) {
    randombytes((uint8_t*)a.coeffs, sizeof(a.coeffs));
    randombytes((uint8_t*)b.coeffs, sizeof(b.coeffs));
    poly_ntt(&a);
    poly_ntt(&b);

    uint64_t s = cpucycles();
    poly_basemul_montgomery(&r, &a, &b); // pointwise in NTT domain
    t[i] = cpucycles() - s;
  }

  printf("poly_basemul_montgomery median %llu cycles (N=%d)\n",
         (unsigned long long)median(t, N), N);
}

static void bench_polyvec_matmul_like(void) {
  // Measures the core Kyber inner product: sum_i (a_i * b_i) in NTT domain
  enum { N = 2000 };
  uint64_t t[N];

  polyvec a, b;
  poly r;

  for (int i = 0; i < N; i++) {
    randombytes((uint8_t*)a.vec, sizeof(a.vec));
    randombytes((uint8_t*)b.vec, sizeof(b.vec));

    // Put both vectors into NTT domain like Kyber does
    polyvec_ntt(&a);
    polyvec_ntt(&b);

    uint64_t s = cpucycles();
    polyvec_basemul_acc_montgomery(&r, &a, &b);
    t[i] = cpucycles() - s;
  }

  printf("polyvec_basemul_acc_montgomery median %llu cycles (N=%d)\n",
         (unsigned long long)median(t, N), N);
}

int main(void) {
  printf("KYBER_K=%d\n", KYBER_K);

  bench_ntt();
  bench_pointwise_mul();
  bench_polyvec_matmul_like();

  return 0;
}