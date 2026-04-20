CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wpointer-arith -O3 -fomit-frame-pointer -z noexecstack
NISTFLAGS += -Wno-unused-result -O3 -fomit-frame-pointer
RM = /bin/rm

SOURCES = kem.c indcpa.c polyvec.c poly.c ntt.c cbd.c reduce.c verify.c
HEADERS = params.h kem.h indcpa.h polyvec.h poly.h ntt.h cbd.h reduce.h verify.h symmetric.h

# ---- ASCON (ascon-c cloned into ref/ascon/) ----
ASCON_DIR := ascon/ascon-c
ASCON_CXOF_DIR := $(ASCON_DIR)/crypto_cxof/asconcxof128/ref
ASCON_CFLAGS := -I$(ASCON_DIR) -I$(ASCON_DIR)/tests -I$(ASCON_CXOF_DIR)
ASCON_SOURCES := $(wildcard $(ASCON_CXOF_DIR)/*.c)

# Kyber sources when using Ascon backend
SOURCES_ASCON = $(SOURCES) symmetric-ascon.c $(ASCON_SOURCES)
HEADERS_ASCON = $(HEADERS)

.PHONY: all test speed shared nistkat clean

all: test speed shared nistkat

test: \
  test/test_kyber512 \
  test/test_kyber768 \
  test/test_kyber1024 \
  test/test_vectors512 \
  test/test_vectors768 \
  test/test_vectors1024 \

speed: \
  test/test_speed512 \
  test/test_speed768 \
  test/test_speed1024 \

shared: \
  lib/libpqcrystals_kyber512_ref.so \
  lib/libpqcrystals_kyber768_ref.so \
  lib/libpqcrystals_kyber1024_ref.so \

nistkat: \
  nistkat/PQCgenKAT_kem512 \
  nistkat/PQCgenKAT_kem768 \
  nistkat/PQCgenKAT_kem1024 \


# ---- Shared libraries ----

lib/libpqcrystals_kyber512_ref.so: $(SOURCES_ASCON) $(HEADERS_ASCON)
	mkdir -p lib
	$(CC) -shared -fPIC $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=2 $(SOURCES_ASCON) -o $@

lib/libpqcrystals_kyber768_ref.so: $(SOURCES_ASCON) $(HEADERS_ASCON)
	mkdir -p lib
	$(CC) -shared -fPIC $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=3 $(SOURCES_ASCON) -o $@

lib/libpqcrystals_kyber1024_ref.so: $(SOURCES_ASCON) $(HEADERS_ASCON)
	mkdir -p lib
	$(CC) -shared -fPIC $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=4 $(SOURCES_ASCON) -o $@


# ---- Tests ----

test/test_kyber512: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=2 $(SOURCES_ASCON) randombytes.c test/test_kyber.c -o $@

test/test_kyber768: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=3 $(SOURCES_ASCON) randombytes.c test/test_kyber.c -o $@

test/test_kyber1024: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=4 $(SOURCES_ASCON) randombytes.c test/test_kyber.c -o $@

test/test_vectors512: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_vectors.c fips202.c fips202.h
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=2 $(SOURCES_ASCON) fips202.c test/test_vectors.c -o $@

test/test_vectors768: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_vectors.c fips202.c fips202.h
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=3 $(SOURCES_ASCON) fips202.c test/test_vectors.c -o $@

test/test_vectors1024: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_vectors.c fips202.c fips202.h
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=4 $(SOURCES_ASCON) fips202.c test/test_vectors.c -o $@


# ---- Speed ----

test/test_speed512: $(SOURCES_ASCON) $(HEADERS_ASCON) test/cpucycles.h test/cpucycles.c test/speed_print.h test/speed_print.c test/test_speed.c randombytes.c
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=2 $(SOURCES_ASCON) randombytes.c test/cpucycles.c test/speed_print.c test/test_speed.c -o $@

test/test_speed768: $(SOURCES_ASCON) $(HEADERS_ASCON) test/cpucycles.h test/cpucycles.c test/speed_print.h test/speed_print.c test/test_speed.c randombytes.c
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=3 $(SOURCES_ASCON) randombytes.c test/cpucycles.c test/speed_print.c test/test_speed.c -o $@

test/test_speed1024: $(SOURCES_ASCON) $(HEADERS_ASCON) test/cpucycles.h test/cpucycles.c test/speed_print.h test/speed_print.c test/test_speed.c randombytes.c
	$(CC) $(CFLAGS) $(ASCON_CFLAGS) -DKYBER_K=4 $(SOURCES_ASCON) randombytes.c test/cpucycles.c test/speed_print.c test/test_speed.c -o $@


# ---- NIST KAT ----

nistkat/PQCgenKAT_kem512: $(SOURCES_ASCON) $(HEADERS_ASCON) nistkat/PQCgenKAT_kem.c nistkat/rng.c nistkat/rng.h
	$(CC) $(NISTFLAGS) $(ASCON_CFLAGS) -DKYBER_K=2 -o $@ $(SOURCES_ASCON) nistkat/rng.c nistkat/PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

nistkat/PQCgenKAT_kem768: $(SOURCES_ASCON) $(HEADERS_ASCON) nistkat/PQCgenKAT_kem.c nistkat/rng.c nistkat/rng.h
	$(CC) $(NISTFLAGS) $(ASCON_CFLAGS) -DKYBER_K=3 -o $@ $(SOURCES_ASCON) nistkat/rng.c nistkat/PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

nistkat/PQCgenKAT_kem1024: $(SOURCES_ASCON) $(HEADERS_ASCON) nistkat/PQCgenKAT_kem.c nistkat/rng.c nistkat/rng.h
	$(CC) $(NISTFLAGS) $(ASCON_CFLAGS) -DKYBER_K=4 -o $@ $(SOURCES_ASCON) nistkat/rng.c nistkat/PQCgenKAT_kem.c $(LDFLAGS) -lcrypto


# ---- NTT TIMING ----

test/test_ntt_timing512: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_ntt_timing.c randombytes.c test/cpucycles.c
	$(CC) $(CFLAGS) -I. $(ASCON_CFLAGS) -DKYBER_K=2 $(SOURCES_ASCON) randombytes.c test/cpucycles.c test/test_ntt_timing.c -o $@

test/test_ntt_timing768: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_ntt_timing.c randombytes.c test/cpucycles.c
	$(CC) $(CFLAGS) -I. $(ASCON_CFLAGS) -DKYBER_K=3 $(SOURCES_ASCON) randombytes.c test/cpucycles.c test/test_ntt_timing.c -o $@

test/test_ntt_timing1024: $(SOURCES_ASCON) $(HEADERS_ASCON) test/test_ntt_timing.c randombytes.c test/cpucycles.c
	$(CC) $(CFLAGS) -I. $(ASCON_CFLAGS) -DKYBER_K=4 $(SOURCES_ASCON) randombytes.c test/cpucycles.c test/test_ntt_timing.c -o $@

clean:
	-$(RM) -f *.gcno *.gcda *.lcov *.o *.so
	-$(RM) -f test/test_kyber512 test/test_kyber768 test/test_kyber1024
	-$(RM) -f test/test_vectors512 test/test_vectors768 test/test_vectors1024
	-$(RM) -f test/test_speed512 test/test_speed768 test/test_speed1024
	-$(RM) -f nistkat/PQCgenKAT_kem512 nistkat/PQCgenKAT_kem768 nistkat/PQCgenKAT_kem1024
	-$(RM) -f nistkat/*.req nistkat/*.rsp
	-$(RM) -rf lib/