#include "params.h"
#include "symmetric.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "tests/crypto_cxof.h"  // provides crypto_xof()

/* 1-byte domain separation tags */
enum {
  DST_XOF   = 0x00,
  DST_PRF   = 0x01,
  DST_RKPRF = 0x02,
  DST_HASHH = 0x03,
  DST_HASHG = 0x04
};

void xof_absorb(xof_state *state,
                const uint8_t seed[KYBER_SYMBYTES],
                uint8_t x, uint8_t y)
{
  state->in[0] = DST_XOF;
  memcpy(state->in + 1, seed, KYBER_SYMBYTES);
  state->in[1 + KYBER_SYMBYTES + 0] = x;
  state->in[1 + KYBER_SYMBYTES + 1] = y;
  state->inlen = 1 + KYBER_SYMBYTES + 2;
  state->offset = 0;
}

void xof_squeezeblocks(uint8_t *out, size_t nblocks, xof_state *state)
{
  size_t want  = nblocks * XOF_BLOCKBYTES;
  size_t total = state->offset + want;

  uint8_t *tmp = (uint8_t *)malloc(total);
  if (!tmp) return;

  /* crypto_xof(out, outlen, in, inlen) */
  crypto_cxof(tmp, (unsigned long long)total,
            state->in, (unsigned long long)state->inlen,
            NULL, 0);

  memcpy(out, tmp + state->offset, want);
  state->offset += want;
  free(tmp);
}

void prf(uint8_t *out, size_t outlen,
         const uint8_t key[KYBER_SYMBYTES],
         uint8_t nonce)
{
  uint8_t in[1 + KYBER_SYMBYTES + 1];
  in[0] = DST_PRF;
  memcpy(in + 1, key, KYBER_SYMBYTES);
  in[1 + KYBER_SYMBYTES] = nonce;

  crypto_cxof(out, (unsigned long long)outlen,
            in, (unsigned long long)sizeof(in),
            NULL, 0);
}

void rkprf(uint8_t out[KYBER_SSBYTES],
           const uint8_t key[KYBER_SYMBYTES],
           const uint8_t *ct)
{
  uint8_t *in = (uint8_t *)malloc(1 + KYBER_SYMBYTES + KYBER_CIPHERTEXTBYTES);
  if (!in) return;

  in[0] = DST_RKPRF;
  memcpy(in + 1, key, KYBER_SYMBYTES);
  memcpy(in + 1 + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);

  crypto_cxof(out, (unsigned long long)KYBER_SSBYTES,
            in, (unsigned long long)(1 + KYBER_SYMBYTES + KYBER_CIPHERTEXTBYTES),
            NULL, 0);
  free(in);
}

void hash_h(uint8_t out[32], const uint8_t *in, size_t inlen)
{
  uint8_t *buf = (uint8_t *)malloc(1 + inlen);
  if (!buf) return;
  buf[0] = DST_HASHH;
  memcpy(buf + 1, in, inlen);

  crypto_cxof(out, 32,
            buf, (unsigned long long)(1 + inlen),
            NULL, 0);

  free(buf);
}

void hash_g(uint8_t out[64], const uint8_t *in, size_t inlen)
{
  uint8_t *buf = (uint8_t *)malloc(1 + inlen);
  if (!buf) return;
  buf[0] = DST_HASHG;
  memcpy(buf + 1, in, inlen);

  crypto_cxof(out, 64,
            buf, (unsigned long long)(1 + inlen),
            NULL, 0);
            
  free(buf);
}