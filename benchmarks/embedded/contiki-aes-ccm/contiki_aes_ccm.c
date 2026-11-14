#include "ctype.h"
#include "errno.h"
#include "inttypes.h"
#include "stddef.h"
#include "stdint.h"
#include "string.h"
#include "strings.h"
#include "sys/select.h"
struct aes_128_driver {
   void (*set_key)(uint8_t const *key) ;
   void (*encrypt)(uint8_t *plaintext_and_result) ;
};
struct ccm_star_driver {
   void (*set_key)(uint8_t const *key) ;
   void (*aead)(uint8_t const *nonce, uint8_t *m, uint8_t m_len,
                uint8_t const *a, uint8_t a_len, uint8_t *result,
                uint8_t mic_len, int forward) ;
};
extern int strcasecmp(char const *s1, char const *s2);

void aes_128_set_padded_key(uint8_t *key, uint8_t key_len);

struct aes_128_driver const aes_128_driver;

/*@ requires valid_string_s1: valid_read_string(s1);
    requires valid_string_s2: valid_read_string(s2);
    assigns \result;
    assigns \result
      \from (indirect: *(s1 + (0 ..))), (indirect: *(s2 + (0 ..)));
 */
extern int strcasecmp(char const *s1, char const *s2);

static uint8_t const sbox[256] =
  {(uint8_t)0x63,
   (uint8_t)0x7c,
   (uint8_t)0x77,
   (uint8_t)0x7b,
   (uint8_t)0xf2,
   (uint8_t)0x6b,
   (uint8_t)0x6f,
   (uint8_t)0xc5,
   (uint8_t)0x30,
   (uint8_t)0x01,
   (uint8_t)0x67,
   (uint8_t)0x2b,
   (uint8_t)0xfe,
   (uint8_t)0xd7,
   (uint8_t)0xab,
   (uint8_t)0x76,
   (uint8_t)0xca,
   (uint8_t)0x82,
   (uint8_t)0xc9,
   (uint8_t)0x7d,
   (uint8_t)0xfa,
   (uint8_t)0x59,
   (uint8_t)0x47,
   (uint8_t)0xf0,
   (uint8_t)0xad,
   (uint8_t)0xd4,
   (uint8_t)0xa2,
   (uint8_t)0xaf,
   (uint8_t)0x9c,
   (uint8_t)0xa4,
   (uint8_t)0x72,
   (uint8_t)0xc0,
   (uint8_t)0xb7,
   (uint8_t)0xfd,
   (uint8_t)0x93,
   (uint8_t)0x26,
   (uint8_t)0x36,
   (uint8_t)0x3f,
   (uint8_t)0xf7,
   (uint8_t)0xcc,
   (uint8_t)0x34,
   (uint8_t)0xa5,
   (uint8_t)0xe5,
   (uint8_t)0xf1,
   (uint8_t)0x71,
   (uint8_t)0xd8,
   (uint8_t)0x31,
   (uint8_t)0x15,
   (uint8_t)0x04,
   (uint8_t)0xc7,
   (uint8_t)0x23,
   (uint8_t)0xc3,
   (uint8_t)0x18,
   (uint8_t)0x96,
   (uint8_t)0x05,
   (uint8_t)0x9a,
   (uint8_t)0x07,
   (uint8_t)0x12,
   (uint8_t)0x80,
   (uint8_t)0xe2,
   (uint8_t)0xeb,
   (uint8_t)0x27,
   (uint8_t)0xb2,
   (uint8_t)0x75,
   (uint8_t)0x09,
   (uint8_t)0x83,
   (uint8_t)0x2c,
   (uint8_t)0x1a,
   (uint8_t)0x1b,
   (uint8_t)0x6e,
   (uint8_t)0x5a,
   (uint8_t)0xa0,
   (uint8_t)0x52,
   (uint8_t)0x3b,
   (uint8_t)0xd6,
   (uint8_t)0xb3,
   (uint8_t)0x29,
   (uint8_t)0xe3,
   (uint8_t)0x2f,
   (uint8_t)0x84,
   (uint8_t)0x53,
   (uint8_t)0xd1,
   (uint8_t)0x00,
   (uint8_t)0xed,
   (uint8_t)0x20,
   (uint8_t)0xfc,
   (uint8_t)0xb1,
   (uint8_t)0x5b,
   (uint8_t)0x6a,
   (uint8_t)0xcb,
   (uint8_t)0xbe,
   (uint8_t)0x39,
   (uint8_t)0x4a,
   (uint8_t)0x4c,
   (uint8_t)0x58,
   (uint8_t)0xcf,
   (uint8_t)0xd0,
   (uint8_t)0xef,
   (uint8_t)0xaa,
   (uint8_t)0xfb,
   (uint8_t)0x43,
   (uint8_t)0x4d,
   (uint8_t)0x33,
   (uint8_t)0x85,
   (uint8_t)0x45,
   (uint8_t)0xf9,
   (uint8_t)0x02,
   (uint8_t)0x7f,
   (uint8_t)0x50,
   (uint8_t)0x3c,
   (uint8_t)0x9f,
   (uint8_t)0xa8,
   (uint8_t)0x51,
   (uint8_t)0xa3,
   (uint8_t)0x40,
   (uint8_t)0x8f,
   (uint8_t)0x92,
   (uint8_t)0x9d,
   (uint8_t)0x38,
   (uint8_t)0xf5,
   (uint8_t)0xbc,
   (uint8_t)0xb6,
   (uint8_t)0xda,
   (uint8_t)0x21,
   (uint8_t)0x10,
   (uint8_t)0xff,
   (uint8_t)0xf3,
   (uint8_t)0xd2,
   (uint8_t)0xcd,
   (uint8_t)0x0c,
   (uint8_t)0x13,
   (uint8_t)0xec,
   (uint8_t)0x5f,
   (uint8_t)0x97,
   (uint8_t)0x44,
   (uint8_t)0x17,
   (uint8_t)0xc4,
   (uint8_t)0xa7,
   (uint8_t)0x7e,
   (uint8_t)0x3d,
   (uint8_t)0x64,
   (uint8_t)0x5d,
   (uint8_t)0x19,
   (uint8_t)0x73,
   (uint8_t)0x60,
   (uint8_t)0x81,
   (uint8_t)0x4f,
   (uint8_t)0xdc,
   (uint8_t)0x22,
   (uint8_t)0x2a,
   (uint8_t)0x90,
   (uint8_t)0x88,
   (uint8_t)0x46,
   (uint8_t)0xee,
   (uint8_t)0xb8,
   (uint8_t)0x14,
   (uint8_t)0xde,
   (uint8_t)0x5e,
   (uint8_t)0x0b,
   (uint8_t)0xdb,
   (uint8_t)0xe0,
   (uint8_t)0x32,
   (uint8_t)0x3a,
   (uint8_t)0x0a,
   (uint8_t)0x49,
   (uint8_t)0x06,
   (uint8_t)0x24,
   (uint8_t)0x5c,
   (uint8_t)0xc2,
   (uint8_t)0xd3,
   (uint8_t)0xac,
   (uint8_t)0x62,
   (uint8_t)0x91,
   (uint8_t)0x95,
   (uint8_t)0xe4,
   (uint8_t)0x79,
   (uint8_t)0xe7,
   (uint8_t)0xc8,
   (uint8_t)0x37,
   (uint8_t)0x6d,
   (uint8_t)0x8d,
   (uint8_t)0xd5,
   (uint8_t)0x4e,
   (uint8_t)0xa9,
   (uint8_t)0x6c,
   (uint8_t)0x56,
   (uint8_t)0xf4,
   (uint8_t)0xea,
   (uint8_t)0x65,
   (uint8_t)0x7a,
   (uint8_t)0xae,
   (uint8_t)0x08,
   (uint8_t)0xba,
   (uint8_t)0x78,
   (uint8_t)0x25,
   (uint8_t)0x2e,
   (uint8_t)0x1c,
   (uint8_t)0xa6,
   (uint8_t)0xb4,
   (uint8_t)0xc6,
   (uint8_t)0xe8,
   (uint8_t)0xdd,
   (uint8_t)0x74,
   (uint8_t)0x1f,
   (uint8_t)0x4b,
   (uint8_t)0xbd,
   (uint8_t)0x8b,
   (uint8_t)0x8a,
   (uint8_t)0x70,
   (uint8_t)0x3e,
   (uint8_t)0xb5,
   (uint8_t)0x66,
   (uint8_t)0x48,
   (uint8_t)0x03,
   (uint8_t)0xf6,
   (uint8_t)0x0e,
   (uint8_t)0x61,
   (uint8_t)0x35,
   (uint8_t)0x57,
   (uint8_t)0xb9,
   (uint8_t)0x86,
   (uint8_t)0xc1,
   (uint8_t)0x1d,
   (uint8_t)0x9e,
   (uint8_t)0xe1,
   (uint8_t)0xf8,
   (uint8_t)0x98,
   (uint8_t)0x11,
   (uint8_t)0x69,
   (uint8_t)0xd9,
   (uint8_t)0x8e,
   (uint8_t)0x94,
   (uint8_t)0x9b,
   (uint8_t)0x1e,
   (uint8_t)0x87,
   (uint8_t)0xe9,
   (uint8_t)0xce,
   (uint8_t)0x55,
   (uint8_t)0x28,
   (uint8_t)0xdf,
   (uint8_t)0x8c,
   (uint8_t)0xa1,
   (uint8_t)0x89,
   (uint8_t)0x0d,
   (uint8_t)0xbf,
   (uint8_t)0xe6,
   (uint8_t)0x42,
   (uint8_t)0x68,
   (uint8_t)0x41,
   (uint8_t)0x99,
   (uint8_t)0x2d,
   (uint8_t)0x0f,
   (uint8_t)0xb0,
   (uint8_t)0x54,
   (uint8_t)0xbb,
   (uint8_t)0x16};
static uint8_t round_keys[11][16];
static uint8_t galois_mul2(uint8_t value)
{
  uint8_t __retres;
  uint8_t xor_val = (uint8_t)(((int)value >> 7) * 0x1b);
  __retres = (uint8_t)(((int)value << 1) ^ (int)xor_val);
  return __retres;
}

void aes_128_set_key(uint8_t const *key)
{
  uint8_t i;
  uint8_t j;
  uint8_t rcon;
  rcon = (uint8_t)0x01;
  memcpy((void *)(round_keys[0]),(void const *)key,(size_t)16);
  i = (uint8_t)1;
  while ((int)i <= 10) {
    round_keys[i][0] = (uint8_t)(((int)sbox[round_keys[(int)i - 1][13]] ^ (int)round_keys[
                                  (int)i - 1][0]) ^ (int)rcon);
    round_keys[i][1] = (uint8_t)((int)sbox[round_keys[(int)i - 1][14]] ^ (int)round_keys[
                                 (int)i - 1][1]);
    round_keys[i][2] = (uint8_t)((int)sbox[round_keys[(int)i - 1][15]] ^ (int)round_keys[
                                 (int)i - 1][2]);
    round_keys[i][3] = (uint8_t)((int)sbox[round_keys[(int)i - 1][12]] ^ (int)round_keys[
                                 (int)i - 1][3]);
    j = (uint8_t)4;
    while ((int)j < 16) {
      round_keys[i][j] = (uint8_t)((int)round_keys[(int)i - 1][j] ^ (int)round_keys[i][
                                   (int)j - 4]);
      j = (uint8_t)((int)j + 1);
    }
    rcon = galois_mul2(rcon);
    i = (uint8_t)((int)i + 1);
  }
  return;
}

void aes_128_encrypt(uint8_t *state)
{
  uint8_t buf1;
  uint8_t buf2;
  uint8_t buf3;
  uint8_t buf4;
  uint8_t round;
  uint8_t i;
  i = (uint8_t)0;
  while ((int)i < 16) {
    *(state + i) = (uint8_t)((int)*(state + i) ^ (int)round_keys[0][i]);
    i = (uint8_t)((int)i + 1);
  }
  round = (uint8_t)1;
  while ((int)round <= 10) {
    i = (uint8_t)0;
    while ((int)i < 16) {
      *(state + i) = sbox[*(state + i)];
      i = (uint8_t)((int)i + 1);
    }
    buf1 = *(state + 1);
    *(state + 1) = *(state + 5);
    *(state + 5) = *(state + 9);
    *(state + 9) = *(state + 13);
    *(state + 13) = buf1;
    buf1 = *(state + 2);
    buf2 = *(state + 6);
    *(state + 2) = *(state + 10);
    *(state + 6) = *(state + 14);
    *(state + 10) = buf1;
    *(state + 14) = buf2;
    buf1 = *(state + 15);
    *(state + 15) = *(state + 11);
    *(state + 11) = *(state + 7);
    *(state + 7) = *(state + 3);
    *(state + 3) = buf1;
    if ((int)round < 10) {
      i = (uint8_t)0;
      while ((int)i < 4) {
        buf4 = (uint8_t)((int)i << 2);
        buf1 = (uint8_t)((((int)*(state + buf4) ^ (int)*(state + ((int)buf4 + 1))) ^ (int)*(
                          state + ((int)buf4 + 2))) ^ (int)*(state + (
                                                             (int)buf4 + 3)));
        buf2 = *(state + buf4);
        buf3 = (uint8_t)((int)*(state + buf4) ^ (int)*(state + ((int)buf4 + 1)));
        buf3 = galois_mul2(buf3);
        *(state + buf4) = (uint8_t)(((int)*(state + buf4) ^ (int)buf3) ^ (int)buf1);
        buf3 = (uint8_t)((int)*(state + ((int)buf4 + 1)) ^ (int)*(state + (
                                                                  (int)buf4 + 2)));
        buf3 = galois_mul2(buf3);
        *(state + ((int)buf4 + 1)) = (uint8_t)(((int)*(state + ((int)buf4 + 1)) ^ (int)buf3) ^ (int)buf1);
        buf3 = (uint8_t)((int)*(state + ((int)buf4 + 2)) ^ (int)*(state + (
                                                                  (int)buf4 + 3)));
        buf3 = galois_mul2(buf3);
        *(state + ((int)buf4 + 2)) = (uint8_t)(((int)*(state + ((int)buf4 + 2)) ^ (int)buf3) ^ (int)buf1);
        buf3 = (uint8_t)((int)*(state + ((int)buf4 + 3)) ^ (int)buf2);
        buf3 = galois_mul2(buf3);
        *(state + ((int)buf4 + 3)) = (uint8_t)(((int)*(state + ((int)buf4 + 3)) ^ (int)buf3) ^ (int)buf1);
        i = (uint8_t)((int)i + 1);
      }
    }
    i = (uint8_t)0;
    while ((int)i < 16) {
      *(state + i) = (uint8_t)((int)*(state + i) ^ (int)round_keys[round][i]);
      i = (uint8_t)((int)i + 1);
    }
    round = (uint8_t)((int)round + 1);
  }
  return;
}

void aes_128_set_padded_key(uint8_t *key, uint8_t key_len)
{
  uint8_t block[16];
  memset((void *)(block),0,(size_t)16);
  memcpy((void *)(block),(void const *)key,(size_t)key_len);
  aes_128_set_key((uint8_t const *)(block));
  return;
}

struct aes_128_driver const aes_128_driver =
  {.set_key = & aes_128_set_key, .encrypt = & aes_128_encrypt};
struct ccm_star_driver const ccm_star_driver;

static void set_iv(uint8_t *iv, uint8_t flags, uint8_t const *nonce,
                   uint8_t counter)
{
  *(iv + 0) = flags;
  memcpy((void *)(iv + 1),(void const *)nonce,(size_t)13);
  *(iv + 14) = (uint8_t)0;
  *(iv + 15) = counter;
  return;
}

static void ctr_step(uint8_t const *nonce, uint8_t pos,
                     uint8_t *m_and_result, uint8_t m_len, uint8_t counter)
{
  uint8_t a[16];
  uint8_t i;
  set_iv(a,(uint8_t)1,nonce,counter);
  aes_128_encrypt(a);
  i = (uint8_t)0;
  while (1) {
    if ((int)pos + (int)i < (int)m_len) {
      if (! ((int)i < 16)) break;
    }
    else break;
    *(m_and_result + ((int)pos + (int)i)) = (uint8_t)((int)*(m_and_result + (
                                                             (int)pos + (int)i)) ^ (int)a[i]);
    i = (uint8_t)((int)i + 1);
  }
  return;
}

static void mic(uint8_t const *nonce, uint8_t const *m, uint8_t m_len,
                uint8_t const *a, uint8_t a_len, uint8_t *result,
                uint8_t mic_len)
{
  uint8_t x[16];
  uint8_t pos;
  uint8_t i;
  unsigned int tmp;
  ;
  ;
  if (a_len) tmp = 1u << 6; else tmp = (unsigned int)0;
  ;
  set_iv(x,
         (uint8_t)((tmp | ((((unsigned int)mic_len - 2u) >> 1) << 3)) | 1u),
         nonce,m_len);
  aes_128_encrypt(x);
  if (a_len) {
    x[1] = (uint8_t)((int)x[1] ^ (int)a_len);
    i = (uint8_t)2;
    while (1) {
      if ((int)i - 2 < (int)a_len) {
        if (! ((int)i < 16)) break;
      }
      else break;
      x[i] = (uint8_t)((int)x[i] ^ (int)*(a + ((int)i - 2)));
      i = (uint8_t)((int)i + 1);
    }
    aes_128_encrypt(x);
    pos = (uint8_t)14;
    while ((int)pos < (int)a_len) {
      i = (uint8_t)0;
      while (1) {
        if ((int)pos + (int)i < (int)a_len) {
          if (! ((int)i < 16)) break;
        }
        else break;
        x[i] = (uint8_t)((int)x[i] ^ (int)*(a + ((int)pos + (int)i)));
        i = (uint8_t)((int)i + 1);
      }
      pos = (uint8_t)((int)pos + 16);
      aes_128_encrypt(x);
    }
  }
  if (m_len) {
    pos = (uint8_t)0;
    while ((int)pos < (int)m_len) {
      i = (uint8_t)0;
      while (1) {
        if ((int)pos + (int)i < (int)m_len) {
          if (! ((int)i < 16)) break;
        }
        else break;
        x[i] = (uint8_t)((int)x[i] ^ (int)*(m + ((int)pos + (int)i)));
        i = (uint8_t)((int)i + 1);
      }
      pos = (uint8_t)((int)pos + 16);
      aes_128_encrypt(x);
    }
  }
  ctr_step(nonce,(uint8_t)0,x,(uint8_t)16,(uint8_t)0);
  memcpy((void *)result,(void const *)(x),(size_t)mic_len);
  return;
}

static void ctr(uint8_t const *nonce, uint8_t *m, uint8_t m_len)
{
  uint8_t pos;
  uint8_t counter;
  pos = (uint8_t)0;
  counter = (uint8_t)1;
  while ((int)pos < (int)m_len) {
    uint8_t tmp;
    tmp = counter;
    counter = (uint8_t)((int)counter + 1);
    ;
    ;
    ;
    ;
    ;
    ctr_step(nonce,pos,m,m_len,tmp);
    pos = (uint8_t)((int)pos + 16);
  }
  return;
}

static void set_key(uint8_t const *key)
{
  aes_128_set_key(key);
  return;
}

static void aead(uint8_t const *nonce, uint8_t *m, uint8_t m_len,
                 uint8_t const *a, uint8_t a_len, uint8_t *result,
                 uint8_t mic_len, int forward)
{
  if (! forward) ctr(nonce,m,m_len);
  mic(nonce,(uint8_t const *)m,m_len,a,a_len,result,mic_len);
  if (forward) ctr(nonce,m,m_len);
  return;
}

struct ccm_star_driver const ccm_star_driver =
  {.set_key = & set_key, .aead = & aead};

