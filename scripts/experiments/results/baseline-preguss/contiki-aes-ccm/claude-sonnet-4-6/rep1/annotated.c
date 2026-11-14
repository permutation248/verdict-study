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
void aes_128_set_padded_key(uint8_t *key, uint8_t key_len);
struct aes_128_driver const aes_128_driver;
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
static uint8_t galois_mul2(uint8_t value);
void aes_128_set_key(uint8_t const *key);
void aes_128_encrypt(uint8_t *state);
struct aes_128_driver const aes_128_driver =
  {.set_key = & aes_128_set_key, .encrypt = & aes_128_encrypt};
struct ccm_star_driver const ccm_star_driver;
static void set_iv(uint8_t *iv, uint8_t flags, uint8_t const *nonce,
                   uint8_t counter);
static void ctr_step(uint8_t const *nonce, uint8_t pos,
                     uint8_t *m_and_result, uint8_t m_len, uint8_t counter);
static void mic(uint8_t const *nonce, uint8_t const *m, uint8_t m_len,
                uint8_t const *a, uint8_t a_len, uint8_t *result,
                uint8_t mic_len);
static void ctr(uint8_t const *nonce, uint8_t *m, uint8_t m_len);
static void set_key(uint8_t const *key);
static void aead(uint8_t const *nonce, uint8_t *m, uint8_t m_len,
                 uint8_t const *a, uint8_t a_len, uint8_t *result,
                 uint8_t mic_len, int forward);
struct ccm_star_driver const ccm_star_driver =
  {.set_key = & set_key, .aead = & aead};

/*@
assigns \nothing;
*/
static uint8_t galois_mul2(uint8_t value)
{
  uint8_t __retres;
  /*@ assert
        rte_4: signed_overflow: -2147483648 ≤ (int)((int)value >> 7) * 0x1b;
  */
  /*@ assert
        rte_5: signed_overflow: (int)((int)value >> 7) * 0x1b ≤ 2147483647;
  */
  uint8_t xor_val = (uint8_t)(((int)value >> 7) * 0x1b);
  /*@ assert rte_6: shift: 0 ≤ (int)value; */
  /*@ assert rte_7: signed_overflow: (int)value << 1 ≤ 2147483647; */
  __retres = (uint8_t)(((int)value << 1) ^ (int)xor_val);
  return __retres;
}

/*@
requires \valid_read(key + (0 .. 15));
*/
void aes_128_set_key(uint8_t const *key)
{
  uint8_t i;
  uint8_t j;
  uint8_t rcon;
  rcon = (uint8_t)0x01;
  memcpy((void *)(round_keys[0]),(void const *)key,(size_t)16);
  i = (uint8_t)1;
  /*@
  loop invariant 1 <= i <= 11;
  loop variant 11 - i;
  */
  while ((int)i <= 10) {
    /*@ assert rte_120: index_bound: i < 11; */
    /*@ assert rte_121: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_122: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_123: index_bound: (int)((int)i - 1) < 11; */
    /*@ assert rte_124: index_bound: round_keys[(int)((int)i - 1)][13] < 256; */
    /*@ assert rte_125: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_126: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_127: index_bound: (int)((int)i - 1) < 11; */
    round_keys[i][0] = (uint8_t)(((int)sbox[round_keys[(int)i - 1][13]] ^ (int)round_keys[
                                  (int)i - 1][0]) ^ (int)rcon);
    /*@ assert rte_128: index_bound: i < 11; */
    /*@ assert rte_129: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_130: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_131: index_bound: (int)((int)i - 1) < 11; */
    /*@ assert rte_132: index_bound: round_keys[(int)((int)i - 1)][14] < 256; */
    /*@ assert rte_133: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_134: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_135: index_bound: (int)((int)i - 1) < 11; */
    round_keys[i][1] = (uint8_t)((int)sbox[round_keys[(int)i - 1][14]] ^ (int)round_keys[
                                 (int)i - 1][1]);
    /*@ assert rte_136: index_bound: i < 11; */
    /*@ assert rte_137: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_138: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_139: index_bound: (int)((int)i - 1) < 11; */
    /*@ assert rte_140: index_bound: round_keys[(int)((int)i - 1)][15] < 256; */
    /*@ assert rte_141: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_142: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_143: index_bound: (int)((int)i - 1) < 11; */
    round_keys[i][2] = (uint8_t)((int)sbox[round_keys[(int)i - 1][15]] ^ (int)round_keys[
                                 (int)i - 1][2]);
    /*@ assert rte_144: index_bound: i < 11; */
    /*@ assert rte_145: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_146: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_147: index_bound: (int)((int)i - 1) < 11; */
    /*@ assert rte_148: index_bound: round_keys[(int)((int)i - 1)][12] < 256; */
    /*@ assert rte_149: signed_overflow: -2147483648 ≤ (int)i - 1; */
    /*@ assert rte_150: index_bound: 0 ≤ (int)((int)i - 1); */
    /*@ assert rte_151: index_bound: (int)((int)i - 1) < 11; */
    round_keys[i][3] = (uint8_t)((int)sbox[round_keys[(int)i - 1][12]] ^ (int)round_keys[
                                 (int)i - 1][3]);
    j = (uint8_t)4;
    /*@
    loop invariant 4 <= j <= 16;
    loop invariant 1 <= i <= 10;
    loop assigns j, round_keys[i][4 .. 15];
    loop variant 16 - j;
    */
    while ((int)j < 16) {
      /*@ assert rte_152: index_bound: i < 11; */
      /*@ assert rte_153: index_bound: j < 16; */
      /*@ assert rte_154: signed_overflow: -2147483648 ≤ (int)i - 1; */
      /*@ assert rte_155: index_bound: 0 ≤ (int)((int)i - 1); */
      /*@ assert rte_156: index_bound: (int)((int)i - 1) < 11; */
      /*@ assert rte_157: index_bound: j < 16; */
      /*@ assert rte_158: signed_overflow: -2147483648 ≤ (int)j - 4; */
      /*@ assert rte_159: index_bound: i < 11; */
      /*@ assert rte_160: index_bound: 0 ≤ (int)((int)j - 4); */
      /*@ assert rte_161: index_bound: (int)((int)j - 4) < 16; */
      round_keys[i][j] = (uint8_t)((int)round_keys[(int)i - 1][j] ^ (int)round_keys[i][
                                   (int)j - 4]);
      /*@ assert rte_162: signed_overflow: (int)j + 1 ≤ 2147483647; */
      j = (uint8_t)((int)j + 1);
    }
    rcon = galois_mul2(rcon);
    /*@ assert rte_163: signed_overflow: (int)i + 1 ≤ 2147483647; */
    i = (uint8_t)((int)i + 1);
  }
  return;
}

/*@
requires \valid(state + (0..15));
*/
void aes_128_encrypt(uint8_t *state)
{
  uint8_t buf1;
  uint8_t buf2;
  uint8_t buf3;
  uint8_t buf4;
  uint8_t round;
  uint8_t i;
  i = (uint8_t)0;
  /*@
  loop invariant 0 <= i <= 16;
  loop invariant \valid(state + (0..15));
  loop assigns i, state[0..15];
  loop variant 16 - i;
  */
  while ((int)i < 16) {
    /*@ assert rte_8: mem_access: \valid(state + (int)i); */
    /*@ assert rte_9: mem_access: \valid_read(state + (int)i); */
    /*@ assert rte_10: index_bound: i < 16; */
    *(state + (int)i) = (uint8_t)((int)*(state + (int)i) ^ (int)round_keys[0][i]);
    /*@ assert rte_11: signed_overflow: (int)i + 1 ≤ 2147483647; */
    i = (uint8_t)((int)i + 1);
  }
  round = (uint8_t)1;
  /*@
  loop invariant \valid(state + (0..15));
  */
  while ((int)round <= 10) {
    i = (uint8_t)0;
    /*@
    loop invariant 0 <= i <= 16;
    loop invariant \valid(state + (0..15));
    loop assigns i, state[0..15];
    loop variant 16 - i;
    */
    while ((int)i < 16) {
      /*@ assert rte_12: mem_access: \valid(state + (int)i); */
      /*@ assert rte_13: mem_access: \valid_read(state + (int)i); */
      /*@ assert rte_14: index_bound: *(state + (int)i) < 256; */
      *(state + (int)i) = sbox[*(state + (int)i)];
      /*@ assert rte_15: signed_overflow: (int)i + 1 ≤ 2147483647; */
      i = (uint8_t)((int)i + 1);
    }
    /*@ assert rte_16: mem_access: \valid_read(state + 1); */
    buf1 = *(state + 1);
    /*@ assert rte_17: mem_access: \valid(state + 1); */
    /*@ assert rte_18: mem_access: \valid_read(state + 5); */
    *(state + 1) = *(state + 5);
    /*@ assert rte_19: mem_access: \valid(state + 5); */
    /*@ assert rte_20: mem_access: \valid_read(state + 9); */
    *(state + 5) = *(state + 9);
    /*@ assert rte_21: mem_access: \valid(state + 9); */
    /*@ assert rte_22: mem_access: \valid_read(state + 13); */
    *(state + 9) = *(state + 13);
    /*@ assert rte_23: mem_access: \valid(state + 13); */
    *(state + 13) = buf1;
    /*@ assert rte_24: mem_access: \valid_read(state + 2); */
    buf1 = *(state + 2);
    /*@ assert rte_25: mem_access: \valid_read(state + 6); */
    buf2 = *(state + 6);
    /*@ assert rte_26: mem_access: \valid(state + 2); */
    /*@ assert rte_27: mem_access: \valid_read(state + 10); */
    *(state + 2) = *(state + 10);
    /*@ assert rte_28: mem_access: \valid(state + 6); */
    /*@ assert rte_29: mem_access: \valid_read(state + 14); */
    *(state + 6) = *(state + 14);
    /*@ assert rte_30: mem_access: \valid(state + 10); */
    *(state + 10) = buf1;
    /*@ assert rte_31: mem_access: \valid(state + 14); */
    *(state + 14) = buf2;
    /*@ assert rte_32: mem_access: \valid_read(state + 15); */
    buf1 = *(state + 15);
    /*@ assert rte_33: mem_access: \valid(state + 15); */
    /*@ assert rte_34: mem_access: \valid_read(state + 11); */
    *(state + 15) = *(state + 11);
    /*@ assert rte_35: mem_access: \valid(state + 11); */
    /*@ assert rte_36: mem_access: \valid_read(state + 7); */
    *(state + 11) = *(state + 7);
    /*@ assert rte_37: mem_access: \valid(state + 7); */
    /*@ assert rte_38: mem_access: \valid_read(state + 3); */
    *(state + 7) = *(state + 3);
    /*@ assert rte_39: mem_access: \valid(state + 3); */
    *(state + 3) = buf1;
    if ((int)round < 10) {
      i = (uint8_t)0;
      /*@
      loop invariant 0 <= i <= 4;
      loop invariant \valid(state + (0..15));
      loop variant 4 - i;
      */
      while ((int)i < 4) {
        /*@ assert rte_40: shift: 0 ≤ (int)i; */
        /*@ assert rte_41: signed_overflow: (int)i << 2 ≤ 2147483647; */
        buf4 = (uint8_t)((int)i << 2);
        /*@ assert rte_42: mem_access: \valid_read(state + (int)buf4); */
        /*@ assert rte_43: signed_overflow: (int)buf4 + 1 ≤ 2147483647; */
        /*@ assert
        rte_44: mem_access: \valid_read(state + (int)((int)buf4 + 1));
        */
        /*@ assert rte_45: signed_overflow: (int)buf4 + 2 ≤ 2147483647; */
        /*@ assert
        rte_46: mem_access: \valid_read(state + (int)((int)buf4 + 2));
        */
        /*@ assert rte_47: signed_overflow: (int)buf4 + 3 ≤ 2147483647; */
        /*@ assert
        rte_48: mem_access: \valid_read(state + (int)((int)buf4 + 3));
        */
        buf1 = (uint8_t)((((int)*(state + (int)buf4) ^ (int)*(state + (
                                                              (int)buf4 + 1))) ^ (int)*(
                          state + ((int)buf4 + 2))) ^ (int)*(state + (
                                                             (int)buf4 + 3)));
        /*@ assert rte_49: mem_access: \valid_read(state + (int)buf4); */
        buf2 = *(state + (int)buf4);
        /*@ assert rte_50: mem_access: \valid_read(state + (int)buf4); */
        /*@ assert rte_51: signed_overflow: (int)buf4 + 1 ≤ 2147483647; */
        /*@ assert
        rte_52: mem_access: \valid_read(state + (int)((int)buf4 + 1));
        */
        buf3 = (uint8_t)((int)*(state + (int)buf4) ^ (int)*(state + (
                                                            (int)buf4 + 1)));
        buf3 = galois_mul2(buf3);
        /*@ assert rte_53: mem_access: \valid(state + (int)buf4); */
        /*@ assert rte_54: mem_access: \valid_read(state + (int)buf4); */
        *(state + (int)buf4) = (uint8_t)(((int)*(state + (int)buf4) ^ (int)buf3) ^ (int)buf1);
        /*@ assert rte_55: signed_overflow: (int)buf4 + 1 ≤ 2147483647; */
        /*@ assert
        rte_56: mem_access: \valid_read(state + (int)((int)buf4 + 1));
        */
        /*@ assert rte_57: signed_overflow: (int)buf4 + 2 ≤ 2147483647; */
        /*@ assert
        rte_58: mem_access: \valid_read(state + (int)((int)buf4 + 2));
        */
        buf3 = (uint8_t)((int)*(state + ((int)buf4 + 1)) ^ (int)*(state + (
                                                                  (int)buf4 + 2)));
        buf3 = galois_mul2(buf3);
        /*@ assert rte_59: mem_access: \valid(state + (int)((int)buf4 + 1)); */
        /*@ assert rte_60: signed_overflow: (int)buf4 + 1 ≤ 2147483647; */
        /*@ assert rte_61: signed_overflow: (int)buf4 + 1 ≤ 2147483647; */
        /*@ assert
        rte_62: mem_access: \valid_read(state + (int)((int)buf4 + 1));
        */
        *(state + ((int)buf4 + 1)) = (uint8_t)(((int)*(state + ((int)buf4 + 1)) ^ (int)buf3) ^ (int)buf1);
        /*@ assert rte_63: signed_overflow: (int)buf4 + 2 ≤ 2147483647; */
        /*@ assert
        rte_64: mem_access: \valid_read(state + (int)((int)buf4 + 2));
        */
        /*@ assert rte_65: signed_overflow: (int)buf4 + 3 ≤ 2147483647; */
        /*@ assert
        rte_66: mem_access: \valid_read(state + (int)((int)buf4 + 3));
        */
        buf3 = (uint8_t)((int)*(state + ((int)buf4 + 2)) ^ (int)*(state + (
                                                                  (int)buf4 + 3)));
        buf3 = galois_mul2(buf3);
        /*@ assert rte_67: mem_access: \valid(state + (int)((int)buf4 + 2)); */
        /*@ assert rte_68: signed_overflow: (int)buf4 + 2 ≤ 2147483647; */
        /*@ assert rte_69: signed_overflow: (int)buf4 + 2 ≤ 2147483647; */
        /*@ assert
        rte_70: mem_access: \valid_read(state + (int)((int)buf4 + 2));
        */
        *(state + ((int)buf4 + 2)) = (uint8_t)(((int)*(state + ((int)buf4 + 2)) ^ (int)buf3) ^ (int)buf1);
        /*@ assert rte_71: signed_overflow: (int)buf4 + 3 ≤ 2147483647; */
        /*@ assert
        rte_72: mem_access: \valid_read(state + (int)((int)buf4 + 3));
        */
        buf3 = (uint8_t)((int)*(state + ((int)buf4 + 3)) ^ (int)buf2);
        buf3 = galois_mul2(buf3);
        /*@ assert rte_73: mem_access: \valid(state + (int)((int)buf4 + 3)); */
        /*@ assert rte_74: signed_overflow: (int)buf4 + 3 ≤ 2147483647; */
        /*@ assert rte_75: signed_overflow: (int)buf4 + 3 ≤ 2147483647; */
        /*@ assert
        rte_76: mem_access: \valid_read(state + (int)((int)buf4 + 3));
        */
        *(state + ((int)buf4 + 3)) = (uint8_t)(((int)*(state + ((int)buf4 + 3)) ^ (int)buf3) ^ (int)buf1);
        /*@ assert rte_77: signed_overflow: (int)i + 1 ≤ 2147483647; */
        i = (uint8_t)((int)i + 1);
      }
    }
    i = (uint8_t)0;
    /*@
    loop invariant 0 <= i <= 16;
    loop invariant \valid(state + (0..15));
    loop assigns i, state[0..15];
    loop variant 16 - i;
    */
    while ((int)i < 16) {
      /*@ assert rte_78: mem_access: \valid(state + (int)i); */
      /*@ assert rte_79: mem_access: \valid_read(state + (int)i); */
      /*@ assert rte_80: index_bound: round < 11; */
      /*@ assert rte_81: index_bound: i < 16; */
      *(state + (int)i) = (uint8_t)((int)*(state + (int)i) ^ (int)round_keys[round][i]);
      /*@ assert rte_82: signed_overflow: (int)i + 1 ≤ 2147483647; */
      i = (uint8_t)((int)i + 1);
    }
    /*@ assert rte_83: signed_overflow: (int)round + 1 ≤ 2147483647; */
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

/*@
requires \valid(iv + (0..15));
requires \valid_read(nonce + (0..12));
ensures \valid(iv + (0..15));
ensures *(iv + 0) == flags;
ensures *(iv + 15) == counter;
ensures *(iv + 14) == 0;
*/
static void set_iv(uint8_t *iv, uint8_t flags, uint8_t const *nonce,
                   uint8_t counter)
{
  /*@ assert rte_1: mem_access: \valid(iv + 0); */
  *(iv + 0) = flags;
  memcpy((void *)(iv + 1),(void const *)nonce,(size_t)13);
  /*@ assert rte_2: mem_access: \valid(iv + 14); */
  *(iv + 14) = (uint8_t)0;
  /*@ assert rte_3: mem_access: \valid(iv + 15); */
  *(iv + 15) = counter;
  return;
}

/*@
requires \valid_read(nonce + (0..12));
requires (int)pos + (int)m_len <= 255;
requires m_len <= 16;
requires m_len == 0 || \valid(m_and_result + (pos .. (int)pos + (int)m_len - 1));
ensures \valid(m_and_result + (pos .. (int)pos + (int)m_len - 1)) || m_len == 0;
ensures m_len == 0 || \valid(m_and_result + (pos .. (int)pos + (int)m_len - 1));
*/
static void ctr_step(uint8_t const *nonce, uint8_t pos,
                     uint8_t *m_and_result, uint8_t m_len, uint8_t counter)
{
  uint8_t a[16];
  uint8_t i;
  set_iv(a,(uint8_t)1,nonce,counter);
  aes_128_encrypt(a);
  i = (uint8_t)0;
  /*@
  loop invariant 0 <= i <= 16;
  loop invariant 0 <= (int)i <= (int)m_len || (int)i <= 16;
  loop invariant (int)pos + (int)i <= 255;
  loop invariant m_len == 0 || \valid(m_and_result + (pos .. (int)pos + (int)m_len - 1));
  loop invariant \valid(m_and_result + ((int)pos + (int)i - 1)) || i == 0;
  loop assigns i, m_and_result[pos .. (int)pos + (int)m_len - 1];
  loop variant 16 - i;
  */
  while (1) {
    /*@ assert rte_84: signed_overflow: -2147483648 ≤ (int)pos + (int)i; */
    /*@ assert rte_85: signed_overflow: (int)pos + (int)i ≤ 2147483647; */
    if ((int)pos + (int)i < (int)m_len) {
      if (! ((int)i < 16)) break;
    }
    else break;
    /*@ assert
        rte_86: mem_access: \valid(m_and_result + (int)((int)pos + (int)i));
    */
    /*@ assert rte_87: signed_overflow: -2147483648 ≤ (int)pos + (int)i; */
    /*@ assert rte_88: signed_overflow: (int)pos + (int)i ≤ 2147483647; */
    /*@ assert rte_89: signed_overflow: -2147483648 ≤ (int)pos + (int)i; */
    /*@ assert rte_90: signed_overflow: (int)pos + (int)i ≤ 2147483647; */
    /*@ assert
        rte_91: mem_access: \valid_read(m_and_result + (int)((int)pos + (int)i));
    */
    /*@ assert rte_92: index_bound: i < 16; */
    *(m_and_result + ((int)pos + (int)i)) = (uint8_t)((int)*(m_and_result + (
                                                             (int)pos + (int)i)) ^ (int)a[i]);
    /*@ assert rte_93: signed_overflow: (int)i + 1 ≤ 2147483647; */
    i = (uint8_t)((int)i + 1);
  }
  return;
}

/*@
requires \valid_read(nonce + (0..12));
requires a_len == 0 || \valid_read(a + (0..(a_len-1)));
requires m_len == 0 || \valid_read(m + (0..(m_len-1)));
requires \valid(result + (0..(mic_len-1)));
requires mic_len <= 16;
requires mic_len >= 2;
*/
static void mic(uint8_t const *nonce, uint8_t const *m, uint8_t m_len,
                uint8_t const *a, uint8_t a_len, uint8_t *result,
                uint8_t mic_len)
{
  uint8_t x[16];
  uint8_t pos;
  uint8_t i;
  unsigned int tmp;
  if (a_len) tmp = 1u << 6; else tmp = (unsigned int)0;
  set_iv(x,
         (uint8_t)((tmp | ((((unsigned int)mic_len - 2u) >> 1) << 3)) | 1u),
         nonce,m_len);
  aes_128_encrypt(x);
  if (a_len) {
    x[1] = (uint8_t)((int)x[1] ^ (int)a_len);
    i = (uint8_t)2;
    /*@
    loop invariant 2 <= i <= 16;
    loop invariant (int)i - 2 <= (int)a_len;
    loop invariant \valid(x + (0..15));
    loop assigns i, x[0..15];
    loop variant 16 - i;
    */
    while (1) {
      /*@ assert rte_96: signed_overflow: -2147483648 ≤ (int)i - 2; */
      if ((int)i - 2 < (int)a_len) {
        if (! ((int)i < 16)) break;
      }
      else break;
      /*@ assert rte_97: index_bound: i < 16; */
      /*@ assert rte_98: index_bound: i < 16; */
      /*@ assert rte_99: signed_overflow: -2147483648 ≤ (int)i - 2; */
      /*@ assert rte_100: mem_access: \valid_read(a + (int)((int)i - 2)); */
      x[i] = (uint8_t)((int)x[i] ^ (int)*(a + ((int)i - 2)));
      /*@ assert rte_101: signed_overflow: (int)i + 1 ≤ 2147483647; */
      i = (uint8_t)((int)i + 1);
    }
    aes_128_encrypt(x);
    pos = (uint8_t)14;
    /*@
    loop invariant (int)pos <= (int)a_len + 16;
    loop invariant \valid(x + (0..15));
    */
    while ((int)pos < (int)a_len) {
      i = (uint8_t)0;
      /*@
      loop invariant 0 <= i <= 16;
      loop invariant (int)pos + (int)i <= (int)a_len || (int)pos + (int)i >= (int)a_len;
      loop invariant (int)pos < (int)a_len;
      loop assigns i, x[0..15];
      loop variant 16 - i;
      */
      while (1) {
        /*@ assert rte_102: signed_overflow: -2147483648 ≤ (int)pos + (int)i;
        */
        /*@ assert rte_103: signed_overflow: (int)pos + (int)i ≤ 2147483647; */
        if ((int)pos + (int)i < (int)a_len) {
          if (! ((int)i < 16)) break;
        }
        else break;
        /*@ assert rte_104: index_bound: i < 16; */
        /*@ assert rte_105: index_bound: i < 16; */
        /*@ assert rte_106: signed_overflow: -2147483648 ≤ (int)pos + (int)i;
        */
        /*@ assert rte_107: signed_overflow: (int)pos + (int)i ≤ 2147483647; */
        /*@ assert
        rte_108: mem_access: \valid_read(a + (int)((int)pos + (int)i));
        */
        x[i] = (uint8_t)((int)x[i] ^ (int)*(a + ((int)pos + (int)i)));
        /*@ assert rte_109: signed_overflow: (int)i + 1 ≤ 2147483647; */
        i = (uint8_t)((int)i + 1);
      }
      /*@ assert rte_110: signed_overflow: (int)pos + 16 ≤ 2147483647; */
      pos = (uint8_t)((int)pos + 16);
      aes_128_encrypt(x);
    }
  }
  if (m_len) {
    pos = (uint8_t)0;
    /*@
    loop invariant 0 <= (int)pos;
    loop invariant \valid(x + (0..15));
    */
    while ((int)pos < (int)m_len) {
      i = (uint8_t)0;
      /*@
      loop invariant 0 <= i <= 16;
      loop invariant (int)pos + (int)i <= (int)m_len || (int)pos + (int)i >= (int)m_len;
      loop invariant (int)pos < (int)m_len;
      loop assigns i, x[0..15];
      loop variant 16 - i;
      */
      while (1) {
        /*@ assert rte_111: signed_overflow: -2147483648 ≤ (int)pos + (int)i;
        */
        /*@ assert rte_112: signed_overflow: (int)pos + (int)i ≤ 2147483647; */
        if ((int)pos + (int)i < (int)m_len) {
          if (! ((int)i < 16)) break;
        }
        else break;
        /*@ assert rte_113: index_bound: i < 16; */
        /*@ assert rte_114: index_bound: i < 16; */
        /*@ assert rte_115: signed_overflow: -2147483648 ≤ (int)pos + (int)i;
        */
        /*@ assert rte_116: signed_overflow: (int)pos + (int)i ≤ 2147483647; */
        /*@ assert
        rte_117: mem_access: \valid_read(m + (int)((int)pos + (int)i));
        */
        x[i] = (uint8_t)((int)x[i] ^ (int)*(m + ((int)pos + (int)i)));
        /*@ assert rte_118: signed_overflow: (int)i + 1 ≤ 2147483647; */
        i = (uint8_t)((int)i + 1);
      }
      /*@ assert rte_119: signed_overflow: (int)pos + 16 ≤ 2147483647; */
      pos = (uint8_t)((int)pos + 16);
      aes_128_encrypt(x);
    }
  }
  ctr_step(nonce,(uint8_t)0,x,(uint8_t)16,(uint8_t)0);
  memcpy((void *)result,(void const *)(x),(size_t)mic_len);
  return;
}

/*@
requires \valid_read(nonce + (0..12));
requires \valid(m + (0 .. (int)m_len - 1));
requires (int)m_len + 16 <= 255;
*/
static void ctr(uint8_t const *nonce, uint8_t *m, uint8_t m_len)
{
  uint8_t pos;
  uint8_t counter;
  pos = (uint8_t)0;
  counter = (uint8_t)1;
  /*@
  loop invariant \valid_read(nonce + (0..12));
  loop invariant \valid(m + (0 .. (int)m_len - 1));
  loop invariant (int)pos % 16 == 0;
  loop invariant (int)pos + (int)m_len <= 255;
  loop variant (int)m_len - (int)pos;
  */
  while ((int)pos < (int)m_len) {
    uint8_t tmp;
    tmp = counter;
    /*@ assert rte_94: signed_overflow: (int)counter + 1 ≤ 2147483647; */
    counter = (uint8_t)((int)counter + 1);
    ctr_step(nonce,pos,m,m_len,tmp);
    /*@ assert rte_95: signed_overflow: (int)pos + 16 ≤ 2147483647; */
    pos = (uint8_t)((int)pos + 16);
  }
  return;
}

/*@
requires \valid_read(key + (0 .. 15));
assigns \nothing;
*/
static void set_key(uint8_t const *key)
{
  aes_128_set_key(key);
  return;
}

/*@
requires \valid_read(nonce + (0..12));
requires m_len == 0 || \valid(m + (0 .. (int)m_len - 1));
requires (int)m_len + 16 <= 255;
requires a_len == 0 || \valid_read(a + (0 .. (int)a_len - 1));
requires \valid(result + (0 .. (int)mic_len - 1));
requires mic_len <= 16;
requires mic_len >= 2;
*/
static void aead(uint8_t const *nonce, uint8_t *m, uint8_t m_len,
                 uint8_t const *a, uint8_t a_len, uint8_t *result,
                 uint8_t mic_len, int forward)
{
  if (! forward) ctr(nonce,m,m_len);
  mic(nonce,(uint8_t const *)m,m_len,a,a_len,result,mic_len);
  if (forward) ctr(nonce,m,m_len);
  return;
}