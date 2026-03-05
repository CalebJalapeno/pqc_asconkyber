#include "params.h"
#include "symmetric.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Ascon-C CXOF interface header (from ascon-c repo)
#include "tests/crypto_cxof.h"

static const uint8_t CS_XOF[]   = "KYBER_XOF";
static const uint8_t CS_PRF[]   = "KYBER_PRF";
static const uint8_t CS_RKPRF[] = "KYBER_RKPRF";
static const uint8_t CS_HASHG[] = "KYBER_HASHG";
static const uint8_t CS_HASHH[] = "KYBER_HASHH";

void xof_absorb(xof_state *state,
                const uint8_t seed[KYBER_SYMBYTES],
                uint8_t x, uint8_t y)
{
  memcpy(state->in, seed, KYBER_SYMBYTES);
  state->in[KYBER_SYMBYTES + 0] = x;
  state->in[KYBER_SYMBYTES + 1] = y;
  state->inlen = KYBER_SYMBYTES + 2;
  state->offset = 0;
}

void xof_squeezeblocks(uint8_t *out, size_t nblocks, xof_state *state)
{
  size_t want  = nblocks * XOF_BLOCKBYTES;
  size_t total = state->offset + want;

  uint8_t *tmp = (uint8_t *)malloc(total);
  if (!tmp) return;

  crypto_cxof(tmp, (unsigned long long)total,
              state->in, (unsigned long long)state->inlen,
              CS_XOF, (unsigned long long)(sizeof(CS_XOF) - 1));

  memcpy(out, tmp + state->offset, want);
  state->offset += want;

  free(tmp);
}

void prf(uint8_t *out, size_t outlen,
         const uint8_t key[KYBER_SYMBYTES],
         uint8_t nonce)
{
  uint8_t in[KYBER_SYMBYTES + 1];
  memcpy(in, key, KYBER_SYMBYTES);
  in[KYBER_SYMBYTES] = nonce;

  crypto_cxof(out, (unsigned long long)outlen,
              in, (unsigned long long)sizeof(in),
              CS_PRF, (unsigned long long)(sizeof(CS_PRF) - 1));
}

void rkprf(uint8_t out[KYBER_SSBYTES],
           const uint8_t key[KYBER_SYMBYTES],
           const uint8_t *ct)
{
  uint8_t in[KYBER_SYMBYTES + KYBER_CIPHERTEXTBYTES];
  memcpy(in, key, KYBER_SYMBYTES);
  memcpy(in + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);

  crypto_cxof(out, (unsigned long long)KYBER_SSBYTES,
              in, (unsigned long long)sizeof(in),
              CS_RKPRF, (unsigned long long)(sizeof(CS_RKPRF) - 1));
}

void hash_h(uint8_t out[32], const uint8_t *in, size_t inlen)
{
  // Option 1 (simple): derive 32 bytes via CXOF with a distinct customization string
  crypto_cxof(out, 32,
              in, (unsigned long long)inlen,
              CS_HASHH, (unsigned long long)(sizeof(CS_HASHH) - 1));

  // Option 2: If you want “real” Ascon-Hash256 specifically, use crypto_hash() and link that scheme.
  // crypto_hash(out, in, (unsigned long long)inlen);
}

void hash_g(uint8_t out[64], const uint8_t *in, size_t inlen)
{
  crypto_cxof(out, 64,
              in, (unsigned long long)inlen,
              CS_HASHG, (unsigned long long)(sizeof(CS_HASHG) - 1));
}