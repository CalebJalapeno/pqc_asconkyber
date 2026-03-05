// symmetric.h (ASCON backend)

#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"

// Choose a block size for Kyber's "squeezeblocks" refill behavior.
// Keeping SHAKE128's rate (168) minimizes downstream churn/perf surprises.
#define XOF_BLOCKBYTES 168

typedef struct {
  uint8_t in[KYBER_SYMBYTES + 2]; // seed || x || y
  size_t  inlen;
  size_t  offset;                // bytes already squeezed
} xof_state;

// XOF for matrix generation (seed,x,y -> stream)
void xof_absorb(xof_state *state,
                const uint8_t seed[KYBER_SYMBYTES],
                uint8_t x, uint8_t y);

void xof_squeezeblocks(uint8_t *out, size_t nblocks, xof_state *state);

// PRF for noise (key,nonce -> stream)
void prf(uint8_t *out, size_t outlen,
         const uint8_t key[KYBER_SYMBYTES],
         uint8_t nonce);

// rkprf (key,ct -> shared secret bytes)
void rkprf(uint8_t out[KYBER_SSBYTES],
           const uint8_t key[KYBER_SYMBYTES],
           const uint8_t *ct);

// hashes
void hash_h(uint8_t out[32], const uint8_t *in, size_t inlen);
void hash_g(uint8_t out[64], const uint8_t *in, size_t inlen);

#endif