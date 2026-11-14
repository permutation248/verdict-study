#include "stdint.h"
#include "string.h"
#include "strings.h"
#include "unistd.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
enum __anonenum_tag_class_1 {
    CLASS_UNIVERSAL = 0x00,
    CLASS_APPLICATION = 0x01,
    CLASS_CONTEXT_SPECIFIC = 0x02,
    CLASS_PRIVATE = 0x03
};
typedef enum __anonenum_tag_class_1 tag_class;
enum __anonenum_asn1_type_2 {
    ASN1_TYPE_INTEGER = 0x02,
    ASN1_TYPE_BIT_STRING = 0x03,
    ASN1_TYPE_OCTET_STRING = 0x04,
    ASN1_TYPE_NULL = 0x05,
    ASN1_TYPE_OID = 0x06,
    ASN1_TYPE_SEQUENCE = 0x10,
    ASN1_TYPE_SET = 0x11,
    ASN1_TYPE_PrintableString = 0x13,
    ASN1_TYPE_T61String = 0x14,
    ASN1_TYPE_IA5String = 0x16,
    ASN1_TYPE_UTCTime = 0x17,
    ASN1_TYPE_GeneralizedTime = 0x18
};
struct __anonstruct_cert_parsing_ctx_3 {
   int empty_subject ;
   int san_empty ;
   int san_critical ;
   int ca_true ;
   int bc_critical ;
   int has_ski ;
   int has_keyUsage ;
   int keyCertSign_set ;
   int cRLSign_set ;
   int pathLenConstraint_set ;
   int has_name_constraints ;
   int has_crldp ;
   int one_crldp_has_all_reasons ;
   int aki_has_keyIdentifier ;
   int self_signed ;
};
typedef struct __anonstruct_cert_parsing_ctx_3 cert_parsing_ctx;
static int bufs_differ(u8 const *b1, u8 const *b2, u16 n);
static int _extract_complex_tag(u8 const *buf, u16 len, u32 *tag_num,
                                u16 *eaten);
static int get_identifier(u8 const *buf, u16 len, tag_class *cls, u8 *prim,
                          u32 *tag_num, u16 *eaten);
static int get_length(u8 const *buf, u16 len, u16 *adv_len, u16 *eaten);
static int _parse_arc(u8 const *buf, u16 len, u32 *arc_val, u16 *eaten);
static int parse_id_len(u8 const *buf, u16 len, tag_class exp_class,
                        u32 exp_type, u16 *parsed, u16 *content_len);
static int parse_OID(u8 const *buf, u16 len, u16 *parsed);
static int parse_integer(u8 const *buf, u16 len, tag_class exp_class,
                         u32 exp_type, u16 *eaten);
static int check_utf8_string(u8 const *buf, u16 len);
static int check_visible_string(u8 const *buf, u16 len);
static int check_bmp_string(u8 const *buf, u16 len);
static int check_ia5_string(u8 const *buf, u16 len);
static int parse_ia5_string(u8 const *buf, u16 len, u16 lb, u16 ub);
static int parse_CPSuri(u8 const *buf, u16 len, u16 *eaten);
static int parse_DisplayText(u8 const *buf, u16 len, u16 *eaten);
static int parse_NoticeReference(u8 const *buf, u16 len, u16 *eaten);
static int parse_UserNotice(u8 const *buf, u16 len, u16 *eaten);
static int parse_policyQualifierInfo(u8 const *buf, u16 len, u16 *eaten);
static int parse_PolicyInformation(u8 const *buf, u16 len, u16 *eaten);
static int parse_ext_certPolicies(u8 const *buf, u16 len, int critical,
                                  cert_parsing_ctx *ctx);

/*@
requires n == 0 || \valid_read(b1 + (0 .. n - 1));
requires n == 0 || \valid_read(b2 + (0 .. n - 1));
requires \separated(b1 + (0 .. n - 1), b2 + (0 .. n - 1));
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
static int bufs_differ(u8 const *b1, u8 const *b2, u16 n)
{
  int ret = 0;
  u16 i = (u16)0;
  i = (u16)0;
  /*@
  loop invariant 0 <= i <= n;
  loop invariant ret == 0 || ret == 1;
  loop invariant \valid_read(b1 + (0 .. n - 1)) || n == 0;
  loop invariant \valid_read(b2 + (0 .. n - 1)) || n == 0;
  loop assigns i, ret;
  loop variant n - i;
  */
  while ((int)i < (int)n) {
    /*@ assert rte_59: mem_access: \valid_read(b1 + i); */
    /*@ assert rte_60: mem_access: \valid_read(b2 + i); */
    if ((int)*(b1 + i) != (int)*(b2 + i)) {
      ret = 1;
      break;
    }
    /*@ assert rte_61: signed_overflow: (int)i + 1 ≤ 2147483647; */
    i = (u16)((int)i + 1);
  }
  return ret;
}

/*@
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. (len <= 4 ? len : 4) - 1));
requires \valid(tag_num);
requires \valid(eaten);
requires \separated(tag_num, eaten);
assigns *tag_num, *eaten;
ensures \result == 0 || \result == -62 || \result == -93 || \result == -99;
ensures \result == 0 || \result < 0;
ensures (len == 0 || buf == \null) ==> \result == -62;
ensures \result == 0 ==> *tag_num >= 0x1f;
ensures \result == 0 ==> *eaten >= 1 && *eaten <= 4;
ensures (\result == 0) ==> (*eaten >= 1 && *eaten <= \old(len));
ensures \result != 0 ==> *tag_num == \old(*tag_num);
ensures \result != 0 ==> *eaten == \old(*eaten);
*/
static int _extract_complex_tag(u8 const *buf, u16 len, u32 *tag_num,
                                u16 *eaten)
{
  u16 rbytes;
  int ret;
  u32 t = (u32)0;
  if ((int)len == 0) goto _LOR;
  else 
    if (buf == (u8 const *)0) {
      _LOR: {
              ret = -62;
              goto out;
            }
    }
  if ((int)len > 4) len = (u16)4;
  rbytes = (u16)0;
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant len <= 4;
  loop invariant (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
  loop assigns rbytes, t;
  */
  while ((int)rbytes < (int)len) {
    {
      u32 tmp1;
      u32 tmp2;
      tmp1 = t << (u32)7;
      /*@ assert rte_1: mem_access: \valid_read(buf + rbytes); */
      tmp2 = (u32)*(buf + rbytes) & (u32)0x7f;
      t = tmp1 + tmp2;
      /*@ assert rte_2: mem_access: \valid_read(buf + rbytes); */
      if (((int)*(buf + rbytes) & 0x80) == 0) break;
    }
    /*@ assert rte_3: signed_overflow: (int)rbytes + 1 ≤ 2147483647; */
    rbytes = (u16)((int)rbytes + 1);
  }
  if ((int)rbytes == (int)len) {
    ret = -93;
    goto out;
  }
  if (t < (u32)0x1f) {
    ret = -99;
    goto out;
  }
  /*@ assert rte_4: mem_access: \valid(tag_num); */
  *tag_num = t;
  /*@ assert rte_5: mem_access: \valid(eaten); */
  /*@ assert rte_6: signed_overflow: (int)rbytes + 1 ≤ 2147483647; */
  *eaten = (u16)((int)rbytes + 1);
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires \separated(cls, prim, tag_num, eaten);
requires \valid(cls);
requires \valid(prim);
requires \valid(tag_num);
requires \valid(eaten);
assigns *cls, *prim, *tag_num, *eaten;
ensures \result == 0 || \result == -144 || \result == -155 || \result == -178 || \result < 0;
ensures (buf == \null) ==> (\result == -144);
ensures (buf != \null && len == 0) ==> (\result == -155);
ensures (\result == 0) ==> (*eaten >= 1 && *eaten <= len);
ensures (\result == 0) ==> (*cls >= 0 && *cls <= 3);
ensures (\result == 0) ==> (*prim == 0 || *prim == 1);
ensures (\result == 0) ==> (buf != \null && len >= 1);
*/
static int get_identifier(u8 const *buf, u16 len, tag_class *cls, u8 *prim,
                          u32 *tag_num, u16 *eaten)
{
  int ret;
  u32 t;
  u8 p;
  tag_class c;
  u16 rbytes = (u16)0;
  if (buf == (u8 const *)0) {
    ret = -144;
    goto out;
  }
  if ((int)len == 0) {
    ret = -155;
    goto out;
  }
  /*@ assert rte_7: mem_access: \valid_read(buf + 0); */
  c = (tag_class)(((int)*(buf + 0) >> 6) & 0x03);
  /*@ assert rte_8: mem_access: \valid_read(buf + 0); */
  p = (u8)(((int)*(buf + 0) >> 5) & 0x01);
  /*@ assert rte_9: mem_access: \valid_read(buf + 0); */
  t = (u32)((int)*(buf + 0) & 0x1f);
  rbytes = (u16)1;
  switch (c) {
    case CLASS_UNIVERSAL: case CLASS_APPLICATION:
    case CLASS_CONTEXT_SPECIFIC: case CLASS_PRIVATE:
    break;
    default: ret = -178;
    goto out;
    break;
  }
  if (t == (u32)0x1f) {
    u16 tag_len = (u16)0;
    /*@ assert rte_10: signed_overflow: -2147483648 ≤ (int)len - (int)rbytes;
    */
    /*@ assert rte_11: signed_overflow: (int)len - (int)rbytes ≤ 2147483647;
    */
    ret = _extract_complex_tag(buf + (int)rbytes,
                               (u16)((int)len - (int)rbytes),& t,& tag_len);
    if (ret) goto out;
    /*@ assert
        rte_12: signed_overflow: -2147483648 ≤ (int)rbytes + (int)tag_len;
    */
    /*@ assert
        rte_13: signed_overflow: (int)rbytes + (int)tag_len ≤ 2147483647;
    */
    rbytes = (u16)((int)rbytes + (int)tag_len);
  }
  /*@ assert rte_14: mem_access: \valid(cls); */
  *cls = c;
  /*@ assert rte_15: mem_access: \valid(prim); */
  *prim = p;
  /*@ assert rte_16: mem_access: \valid(tag_num); */
  *tag_num = t;
  /*@ assert rte_17: mem_access: \valid(eaten); */
  *eaten = rbytes;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires \valid(adv_len);
requires \valid(eaten);
requires \separated(adv_len, eaten);
requires (buf != \null && len > 0) ==> \separated(buf + (0 .. len - 1), adv_len, eaten);
assigns *adv_len, *eaten;
ensures \result == 0 || \result == -233 || \result == -239 || \result == -259 || \result == -278 || \result == -299 || \result == -308 || \result == -321 || \result == -334 || \result == -349 || \result == -358;
ensures (buf == \null) ==> \result == -233;
ensures (buf != \null && len == 0) ==> \result == -239;
ensures (\result == 0) ==> (*eaten >= 1 && *eaten <= 3);
ensures (\result == 0) ==> (*adv_len <= len - *eaten);
ensures (\result == 0 && *adv_len > 0) ==> \valid_read((buf + *eaten) + (0 .. *adv_len - 1));
ensures (\result == 0) ==> (*eaten <= len);
ensures (\result == 0) ==> (*adv_len + *eaten <= len);
*/
static int get_length(u8 const *buf, u16 len, u16 *adv_len, u16 *eaten)
{
  u16 l;
  u8 len_len;
  u8 b0;
  int ret;
  u16 rbytes = (u16)0;
  if (buf == (u8 const *)0) {
    ret = -233;
    goto out;
  }
  if ((int)len == 0) {
    ret = -239;
    goto out;
  }
  /*@ assert rte_18: mem_access: \valid_read(buf + 0); */
  b0 = *(buf + 0);
  if (((int)b0 & 0x80) == 0) {
    l = (u16)((int)b0 & 0x7f);
    /*@ assert rte_19: signed_overflow: (int)l + 1 ≤ 2147483647; */
    if ((int)l + 1 > (int)len) {
      ret = -259;
      goto out;
    }
    /*@ assert rte_20: mem_access: \valid(eaten); */
    *eaten = (u16)1;
    /*@ assert rte_21: mem_access: \valid(adv_len); */
    *adv_len = l;
    ret = 0;
    goto out;
  }
  if ((int)b0 == 0x80) {
    ret = -278;
    goto out;
  }
  len_len = (u8)((int)b0 & 0x7f);
  /*@ assert rte_22: signed_overflow: (int)rbytes + 1 ≤ 2147483647; */
  rbytes = (u16)((int)rbytes + 1);
  /*@ assert rte_23: signed_overflow: (int)len_len + 1 ≤ 2147483647; */
  if ((int)len_len + 1 > (int)len) {
    ret = -299;
    goto out;
  }
  switch ((int)len_len) {
    case 0: ret = -308;
    goto out;
    break;
    case 1:
    /*@ assert rte_24: mem_access: \valid_read(buf + 1); */
    l = (u16)*(buf + 1);
    if ((int)l <= 127) {
      ret = -321;
      goto out;
    }
    /*@ assert rte_25: signed_overflow: (int)rbytes + 1 ≤ 2147483647; */
    rbytes = (u16)((int)rbytes + 1);
    break;
    case 2:
    /*@ assert rte_26: mem_access: \valid_read(buf + 1); */
    /*@ assert
        rte_27: signed_overflow: -2147483648 ≤ (int)((u16)*(buf + 1)) * 256;
    */
    /*@ assert
        rte_28: signed_overflow: (int)((u16)*(buf + 1)) * 256 ≤ 2147483647;
    */
    /*@ assert rte_29: mem_access: \valid_read(buf + 2); */
    /*@ assert
        rte_30: signed_overflow:
          -2147483648 ≤
          (int)((int)((u16)*(buf + 1)) * 256) + (int)*(buf + 2);
    */
    /*@ assert
        rte_31: signed_overflow:
          (int)((int)((u16)*(buf + 1)) * 256) + (int)*(buf + 2) ≤
          2147483647;
    */
    l = (u16)((int)((u16)*(buf + 1)) * 256 + (int)*(buf + 2));
    if ((int)l <= 0xff) {
      ret = -334;
      goto out;
    }
    /*@ assert rte_32: signed_overflow: (int)rbytes + 2 ≤ 2147483647; */
    rbytes = (u16)((int)rbytes + 2);
    break;
    default: ret = -349;
    goto out;
    break;
  }
  /*@ assert rte_33: signed_overflow: -2147483648 ≤ (int)len - (int)rbytes; */
  /*@ assert rte_34: signed_overflow: (int)len - (int)rbytes ≤ 2147483647; */
  if ((int)len - (int)rbytes < (int)l) {
    ret = -358;
    goto out;
  }
  /*@ assert rte_35: mem_access: \valid(eaten); */
  *eaten = rbytes;
  /*@ assert rte_36: mem_access: \valid(adv_len); */
  *adv_len = l;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(arc_val);
requires \valid(eaten);
requires \separated(arc_val, eaten);
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
assigns *arc_val, *eaten;
ensures \result == 0 || \result == -382 || \result == -422;
ensures (\at(len, Pre) == 0 || buf == \null) ==> \result == -382;
ensures \result == 0 ==> (*eaten >= 1 && *eaten <= 4);
ensures \result == 0 ==> *eaten <= \at(len, Pre);
ensures \result != 0 ==> (*arc_val == \old(*arc_val) && *eaten == \old(*eaten));
*/
static int _parse_arc(u8 const *buf, u16 len, u32 *arc_val, u16 *eaten)
{
  u16 rbytes;
  int ret;
  u32 av = (u32)0;
  if ((int)len == 0) goto _LOR;
  else 
    if (buf == (u8 const *)0) {
      _LOR: {
              ret = -382;
              goto out;
            }
    }
  if ((int)len > 4) len = (u16)4;
  rbytes = (u16)0;
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant rbytes <= 4;
  loop assigns rbytes, av;
  loop variant len - rbytes;
  */
  while ((int)rbytes < (int)len) {
    {
      u32 tmp1;
      u32 tmp2;
      tmp1 = av << (u32)7;
      /*@ assert rte_45: mem_access: \valid_read(buf + rbytes); */
      tmp2 = (u32)*(buf + rbytes) & (u32)0x7f;
      av = tmp1 + tmp2;
      /*@ assert rte_46: mem_access: \valid_read(buf + rbytes); */
      if (((int)*(buf + rbytes) & 0x80) == 0) break;
    }
    /*@ assert rte_47: signed_overflow: (int)rbytes + 1 ≤ 2147483647; */
    rbytes = (u16)((int)rbytes + 1);
  }
  if ((int)rbytes >= (int)len) {
    ret = -422;
    goto out;
  }
  /*@ assert rte_48: mem_access: \valid(arc_val); */
  *arc_val = av;
  /*@ assert rte_49: mem_access: \valid(eaten); */
  /*@ assert rte_50: signed_overflow: (int)rbytes + 1 ≤ 2147483647; */
  *eaten = (u16)((int)rbytes + 1);
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires \valid(parsed);
requires \valid(content_len);
requires \separated(parsed, content_len);
assigns *parsed, *content_len;
ensures \result == 0 || \result == -464 || \result == -471 || \result == -489 || \result == -496 || \result == -521 || \result < 0;
ensures (\old(buf) == \null) ==> (\result == -464);
ensures (\result == 0) ==> (*parsed >= 2 && *parsed <= \old(len));
ensures (\result == 0) ==> (*content_len <= \old(len) - *parsed);
ensures (\result == 0) ==> (*parsed + *content_len <= \old(len));
ensures (\result == 0 && \old(buf) != \null && *content_len > 0) ==> \valid_read((\old(buf) + *parsed) + (0 .. *content_len - 1));
*/
static int parse_id_len(u8 const *buf, u16 len, tag_class exp_class,
                        u32 exp_type, u16 *parsed, u16 *content_len)
{
  u8 p;
  u16 grabbed;
  int ret;
  tag_class c = (tag_class)0;
  u32 t = (u32)0;
  u16 cur_parsed = (u16)0;
  u16 adv_len = (u16)0;
  if (buf == (u8 const *)0) {
    ret = -464;
    goto out;
  }
  if ((int)len > 65534) {
    ret = -471;
    goto out;
  }
  ret = get_identifier(buf,len,& c,& p,& t,& cur_parsed);
  if (ret) goto out;
  if (t != exp_type) {
    ret = -489;
    goto out;
  }
  if (c != exp_class) {
    ret = -496;
    goto out;
  }
  grabbed = cur_parsed;
  /*@ assert
        rte_37: signed_overflow: -2147483648 ≤ (int)len - (int)cur_parsed;
  */
  /*@ assert rte_38: signed_overflow: (int)len - (int)cur_parsed ≤ 2147483647;
  */
  len = (u16)((int)len - (int)cur_parsed);
  buf += (int)cur_parsed;
  ret = get_length(buf,len,& adv_len,& cur_parsed);
  if (ret) goto out;
  /*@ assert
        rte_39: signed_overflow: -2147483648 ≤ (int)grabbed + (int)cur_parsed;
  */
  /*@ assert
        rte_40: signed_overflow: (int)grabbed + (int)cur_parsed ≤ 2147483647;
  */
  grabbed = (u16)((int)grabbed + (int)cur_parsed);
  /*@ assert
        rte_41: signed_overflow: -2147483648 ≤ (int)len - (int)cur_parsed;
  */
  /*@ assert rte_42: signed_overflow: (int)len - (int)cur_parsed ≤ 2147483647;
  */
  len = (u16)((int)len - (int)cur_parsed);
  buf += (int)cur_parsed;
  if ((int)adv_len > (int)len) {
    ret = -521;
    goto out;
  }
  /*@ assert rte_43: mem_access: \valid(parsed); */
  *parsed = grabbed;
  /*@ assert rte_44: mem_access: \valid(content_len); */
  *content_len = adv_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(parsed);
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
assigns *parsed;
ensures \result == 0 || \result < 0;
ensures (buf == \null || \at(len, Pre) == 0) ==> \result == -566;
ensures \result == 0 ==> (*parsed >= 2 && *parsed <= \at(len, Pre));
ensures \result != 0 ==> *parsed == \old(*parsed);
*/
static int parse_OID(u8 const *buf, u16 len, u16 *parsed)
{
  u16 num;
  int ret;
  u16 data_len = (u16)0;
  u16 hdr_len = (u16)0;
  u16 remain = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -566;
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_OID,& hdr_len,
                     & data_len);
  if (ret) goto out;
  /*@ assert rte_51: signed_overflow: -2147483648 ≤ (int)len - (int)hdr_len;
  */
  /*@ assert rte_52: signed_overflow: (int)len - (int)hdr_len ≤ 2147483647; */
  len = (u16)((int)len - (int)hdr_len);
  buf += (int)hdr_len;
  if ((int)data_len < 1) {
    ret = -581;
    goto out;
  }
  remain = data_len;
  num = (u16)0;
  /*@
  loop invariant 0 <= remain <= data_len;
  loop invariant 0 <= num <= 21;
  loop invariant \valid_read(buf + (0 .. remain - 1)) || remain == 0;
  loop assigns num, buf, remain, ret;
  */
  while (remain) {
    u32 arc_val = (u32)0;
    u16 rbytes = (u16)0;
    if ((int)num > 20) {
      ret = -600;
      goto out;
    }
    ret = _parse_arc(buf,remain,& arc_val,& rbytes);
    if (ret) goto out;
    /*@ assert rte_53: signed_overflow: (int)num + 1 ≤ 2147483647; */
    num = (u16)((int)num + 1);
    buf += (int)rbytes;
    /*@ assert
        rte_54: signed_overflow: -2147483648 ≤ (int)remain - (int)rbytes;
    */
    /*@ assert
        rte_55: signed_overflow: (int)remain - (int)rbytes ≤ 2147483647;
    */
    remain = (u16)((int)remain - (int)rbytes);
  }
  if ((int)num < 1) {
    ret = -625;
    goto out;
  }
  /*@ assert rte_56: mem_access: \valid(parsed); */
  /*@ assert
        rte_57: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
  */
  /*@ assert
        rte_58: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
  */
  *parsed = (u16)((int)hdr_len + (int)data_len);
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(eaten);
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires \separated(eaten, buf + (0 .. len - 1));
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (\result == 0) ==> (*eaten >= 1 && *eaten <= len);
*/
static int parse_integer(u8 const *buf, u16 len, tag_class exp_class,
                         u32 exp_type, u16 *eaten)
{
  int ret;
  u16 hdr_len = (u16)0;
  u16 data_len = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -653;
              goto out;
            }
    }
  ret = parse_id_len(buf,len,exp_class,exp_type,& hdr_len,& data_len);
  if (ret) goto out;
  buf += (int)hdr_len;
  if ((int)data_len == 0) {
    ret = -671;
    goto out;
  }
  if ((int)data_len > 1) {
    /*@ assert rte_94: mem_access: \valid_read(buf + 0); */
    if ((int)*(buf + 0) == 0) 
      /*@ assert rte_95: mem_access: \valid_read(buf + 1); */
      if (((int)*(buf + 1) & 0x80) == 0) {
        ret = -686;
        goto out;
      }
    /*@ assert rte_96: mem_access: \valid_read(buf + 0); */
    if ((int)*(buf + 0) == 0xff) 
      /*@ assert rte_97: mem_access: \valid_read(buf + 1); */
      if ((int)*(buf + 1) & 0x80) {
        ret = -692;
        goto out;
      }
  }
  /*@ assert rte_98: mem_access: \valid(eaten); */
  /*@ assert
        rte_99: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
  */
  /*@ assert
        rte_100: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
  */
  *eaten = (u16)((int)hdr_len + (int)data_len);
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
assigns \nothing;
ensures \result == 0 || \result == -717 || \result == -744 || \result == -751 || \result == -768 || \result == -776 || \result == -787 || \result == -792 || \result == -809 || \result == -821 || \result == -826 || \result == -834 || \result == -844;
ensures (buf == \null || len == 0) ==> \result == -717;
*/
static int check_utf8_string(u8 const *buf, u16 len)
{
  int ret;
  u8 b0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -717;
              goto out;
            }
    }
  /*@
  loop invariant 0 <= len <= \at(len, Pre);
  loop invariant len > 0 ==> \valid_read(buf + (0 .. len - 1));
  loop assigns len, buf, b0;
  */
  while (len) {
    /*@ assert rte_68: mem_access: \valid_read(buf + 0); */
    b0 = *(buf + 0);
    if ((int)b0 <= 0x7f) {
      /*@ assert rte_69: signed_overflow: -2147483648 ≤ (int)len - 1; */
      len = (u16)((int)len - 1);
      buf ++;
      continue;
    }
    if ((int)b0 >= 0xc2) 
      if ((int)b0 <= 0xdf) {
        if ((int)len < 2) {
          ret = -744;
          goto out;
        }
        /*@ assert rte_70: mem_access: \valid_read(buf + 1); */
        if (((int)*(buf + 1) & 0xc0) != 0x80) {
          ret = -751;
          goto out;
        }
        /*@ assert rte_71: signed_overflow: -2147483648 ≤ (int)len - 2; */
        len = (u16)((int)len - 2);
        buf += 2;
        continue;
      }
    if ((int)b0 >= 0xe0) 
      if ((int)b0 <= 0xef) {
        if ((int)len < 3) {
          ret = -768;
          goto out;
        }
        /*@ assert rte_72: mem_access: \valid_read(buf + 1); */
        if (((int)*(buf + 1) & 0xc0) != 0x80) goto _LOR_0;
        else 
          /*@ assert rte_73: mem_access: \valid_read(buf + 2); */
          if (((int)*(buf + 2) & 0xc0) != 0x80) {
            _LOR_0: {
                      ret = -776;
                      goto out;
                    }
          }
        if ((int)b0 == 0xe0) {
          /*@ assert rte_74: mem_access: \valid_read(buf + 1); */
          if ((int)*(buf + 1) < 0xa0) goto _LOR_2;
          else 
            /*@ assert rte_75: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) > 0xbf) {
              _LOR_2: {
                        ret = -787;
                        goto out;
                      }
            }
            else goto _LAND;
        }
        else {
          _LAND: ;
          if ((int)b0 == 0xed) 
            /*@ assert rte_76: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) < 0x80) goto _LOR_1;
            else 
              /*@ assert rte_77: mem_access: \valid_read(buf + 1); */
              if ((int)*(buf + 1) > 0x9f) {
                _LOR_1: {
                          ret = -792;
                          goto out;
                        }
              }
        }
        /*@ assert rte_78: signed_overflow: -2147483648 ≤ (int)len - 3; */
        len = (u16)((int)len - 3);
        buf += 3;
        continue;
      }
    if ((int)b0 >= 0xf0) 
      if ((int)b0 <= 0xf4) {
        if ((int)len < 4) {
          ret = -809;
          goto out;
        }
        if ((int)b0 == 0xf0) {
          /*@ assert rte_79: mem_access: \valid_read(buf + 1); */
          if ((int)*(buf + 1) < 0x90) goto _LOR_4;
          else 
            /*@ assert rte_80: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) > 0xbf) {
              _LOR_4: {
                        ret = -821;
                        goto out;
                      }
            }
            else goto _LAND_0;
        }
        else {
          _LAND_0: ;
          if ((int)b0 == 0xf4) 
            /*@ assert rte_81: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) < 0x80) goto _LOR_3;
            else 
              /*@ assert rte_82: mem_access: \valid_read(buf + 1); */
              if ((int)*(buf + 1) > 0x8f) {
                _LOR_3: {
                          ret = -826;
                          goto out;
                        }
              }
        }
        /*@ assert rte_83: mem_access: \valid_read(buf + 1); */
        if (((int)*(buf + 1) & 0xc0) != 0x80) goto _LOR_5;
        else 
          /*@ assert rte_84: mem_access: \valid_read(buf + 2); */
          if (((int)*(buf + 2) & 0xc0) != 0x80) goto _LOR_5;
          else 
            /*@ assert rte_85: mem_access: \valid_read(buf + 3); */
            if (((int)*(buf + 3) & 0xc0) != 0x80) {
              _LOR_5: {
                        ret = -834;
                        goto out;
                      }
            }
        /*@ assert rte_86: signed_overflow: -2147483648 ≤ (int)len - 4; */
        len = (u16)((int)len - 4);
        buf += 4;
        continue;
      }
    ret = -844;
    goto out;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
assigns \nothing;
ensures \result == 0 || \result == -866 || \result == -921;
ensures (buf == \null || len == 0) ==> \result == -866;
ensures \result == 0 ==> (buf != \null && len > 0);
*/
static int check_visible_string(u8 const *buf, u16 len)
{
  int ret;
  u8 c;
  u16 rbytes = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -866;
              goto out;
            }
    }
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
  loop assigns rbytes, c;
  */
  while ((int)rbytes < (int)len) {
    /*@ assert rte_87: mem_access: \valid_read(buf + rbytes); */
    c = *(buf + rbytes);
    if ((int)c >= 'a') {
      if ((int)c <= 'z') goto _LOR_0; else goto _LAND_0;
    }
    else {
      _LAND_0: ;
      if ((int)c >= 'A') {
        if ((int)c <= 'Z') goto _LOR_0; else goto _LAND;
      }
      else {
        _LAND: ;
        if ((int)c >= '0') 
          if ((int)c <= '9') {
            _LOR_0:
            {
              /*@ assert
        rte_88: signed_overflow: (int)rbytes + 1 ≤ 2147483647;
              */
              rbytes = (u16)((int)rbytes + 1);
              continue;
            }
          }
      }
    }
    switch ((int)c) {
      case 39: case '=': case '(': case ')': case '+': case ',': case '-':
      case '.': case '/': case ':': case '?': case ' ': case '!': case '\"':
      case '#': case '$': case '%': case '&': case '*': case ';': case '<':
      case '>': case '[': case '\\': case ']': case '^': case '_': case '`':
      case '{': case '|': case '}': case '~':
      /*@ assert rte_89: signed_overflow: (int)rbytes + 1 ≤ 2147483647; */
      rbytes = (u16)((int)rbytes + 1);
      continue;
      default: break;
    }
    ret = -921;
    goto out;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
assigns \nothing;
ensures \result == -937;
*/
static int check_bmp_string(u8 const *buf, u16 len)
{
  int __retres;
  __retres = -937;
  return __retres;
}

/*@
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
assigns \nothing;
ensures \result == 0 || \result == -947 || \result == -955;
ensures (buf == \null || len == 0) ==> \result == -947;
ensures (\result == 0) ==> (buf != \null && len > 0);
*/
static int check_ia5_string(u8 const *buf, u16 len)
{
  int ret;
  u16 i;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -947;
              goto out;
            }
    }
  i = (u16)0;
  /*@
  loop invariant 0 <= i <= len;
  loop invariant \valid_read(buf + (0 .. len - 1));
  loop assigns i;
  */
  while ((int)i < (int)len) {
    /*@ assert rte_62: mem_access: \valid_read(buf + i); */
    if ((int)*(buf + i) > 0x7f) {
      ret = -955;
      goto out;
    }
    /*@ assert rte_63: signed_overflow: (int)i + 1 ≤ 2147483647; */
    i = (u16)((int)i + 1);
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (len >= 2 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
assigns \nothing;
ensures \result == 0 || \result == -985 || \result == -991 || \result == -998 || \result == -1005 || \result == -1012 || \result == -947 || \result == -955;
ensures (buf == \null || len == 0) ==> \result == -985;
ensures (buf != \null && len == 1) ==> \result == -991;
ensures (\result == 0) ==> (buf != \null && len >= 2);
*/
static int parse_ia5_string(u8 const *buf, u16 len, u16 lb, u16 ub)
{
  u8 str_type;
  int ret = -981;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -985;
              goto out;
            }
    }
  if ((int)len < 2) {
    ret = -991;
    goto out;
  }
  /*@ assert rte_64: mem_access: \valid_read(buf + 0); */
  str_type = *(buf + 0);
  if ((int)str_type != 22) {
    ret = -998;
    goto out;
  }
  /*@ assert rte_65: signed_overflow: -2147483648 ≤ (int)len - 2; */
  len = (u16)((int)len - 2);
  /*@ assert rte_66: mem_access: \valid_read(buf + 1); */
  if ((int)*(buf + 1) != (int)len) {
    ret = -1005;
    goto out;
  }
  buf += 2;
  if ((int)len < (int)lb) goto _LOR_0;
  else 
    if ((int)len > (int)ub) {
      _LOR_0: {
                ret = -1012;
                goto out;
              }
    }
  ret = check_ia5_string(buf,len);
  if (ret) goto out;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires (buf != \null && len > 0) ==> \valid(eaten);
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result == -1036;
ensures \result == 0 ==> *eaten == len;
*/
static int parse_CPSuri(u8 const *buf, u16 len, u16 *eaten)
{
  int ret;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -1036;
              goto out;
            }
    }
  ret = parse_ia5_string(buf,len,(u16)1,(u16)65534);
  if (ret) goto out;
  /*@ assert rte_67: mem_access: \valid(eaten); */
  *eaten = len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires \valid(eaten);
requires \separated(buf + (0 .. len - 1), eaten);
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result == -1071;
ensures (\result == 0) ==> (*eaten >= 1 && *eaten <= len);
*/
static int parse_DisplayText(u8 const *buf, u16 len, u16 *eaten)
{
  u8 str_type;
  u16 hdr_len = (u16)0;
  u16 data_len = (u16)0;
  int ret = -1;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -1071;
              goto out;
            }
    }
  /*@ assert rte_90: mem_access: \valid_read(buf + 0); */
  str_type = *(buf + 0);
  switch ((int)str_type) {
    case 12: case 22: case 26: case 30:
    ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)str_type,& hdr_len,
                       & data_len);
    if (ret) goto out;
    buf += (int)hdr_len;
    switch ((int)str_type) {
      case 12: ret = check_utf8_string(buf,data_len);
      if (ret) goto out;
      break;
      case 22: ret = check_ia5_string(buf,data_len);
      if (ret) goto out;
      break;
      case 26: ret = check_visible_string(buf,data_len);
      if (ret) goto out;
      break;
      case 30: ret = check_bmp_string(buf,data_len);
      if (ret) goto out;
      break;
      default: ret = -1122;
      goto out;
      break;
    }
    /*@ assert rte_91: mem_access: \valid(eaten); */
    /*@ assert
        rte_92: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
    */
    /*@ assert
        rte_93: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
    */
    *eaten = (u16)((int)hdr_len + (int)data_len);
    break;
    default: ret = -1132;
    goto out;
    break;
  }
  out: ;
  return ret;
}

/*@
requires \valid(eaten);
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires \separated(eaten, buf + (0 .. len - 1));
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures \result == 0 ==> (*eaten >= 1 && *eaten <= len);
ensures (\result != 0) ==> (*eaten == \old(*eaten));
*/
static int parse_NoticeReference(u8 const *buf, u16 len, u16 *eaten)
{
  u16 remain;
  int ret;
  u16 parsed = (u16)0;
  u16 saved_len = (u16)0;
  u16 hdr_len = (u16)0;
  u16 data_len = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -1162;
              goto out;
            }
    }
  remain = len;
  ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
                     & hdr_len,& data_len);
  if (ret) goto out;
  /*@ assert
        rte_101: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
  */
  /*@ assert
        rte_102: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
  */
  saved_len = (u16)((int)hdr_len + (int)data_len);
  remain = data_len;
  buf += (int)hdr_len;
  ret = parse_DisplayText(buf,remain,& parsed);
  if (ret) goto out;
  /*@ assert rte_103: signed_overflow: -2147483648 ≤ (int)remain - (int)parsed;
  */
  /*@ assert rte_104: signed_overflow: (int)remain - (int)parsed ≤ 2147483647;
  */
  remain = (u16)((int)remain - (int)parsed);
  buf += (int)parsed;
  ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
                     & hdr_len,& data_len);
  if (ret) goto out;
  /*@ assert
        rte_105: signed_overflow: -2147483648 ≤ (int)remain - (int)hdr_len;
  */
  /*@ assert rte_106: signed_overflow: (int)remain - (int)hdr_len ≤ 2147483647;
  */
  remain = (u16)((int)remain - (int)hdr_len);
  buf += (int)hdr_len;
  if ((int)remain != (int)data_len) {
    ret = -1210;
    goto out;
  }
  /*@
  loop invariant 0 <= remain <= data_len;
  loop invariant \valid_read(buf + (0 .. remain - 1)) || remain == 0;
  loop assigns remain, buf, ret, parsed;
  */
  while (remain) {
    ret = parse_integer(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_INTEGER,
                        & parsed);
    if (ret) goto out;
    /*@ assert
        rte_107: signed_overflow: -2147483648 ≤ (int)remain - (int)parsed;
    */
    /*@ assert
        rte_108: signed_overflow: (int)remain - (int)parsed ≤ 2147483647;
    */
    remain = (u16)((int)remain - (int)parsed);
    buf += (int)parsed;
  }
  /*@ assert rte_109: mem_access: \valid(eaten); */
  *eaten = saved_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires (buf != \null && len > 0) ==> \valid(eaten);
requires \separated(eaten, buf + (0 .. len - 1));
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (\result == 0) ==> (*eaten >= 1 && *eaten <= len);
ensures (buf == \null || len == 0) ==> \result == -1262;
*/
static int parse_UserNotice(u8 const *buf, u16 len, u16 *eaten)
{
  int ret;
  u16 hdr_len = (u16)0;
  u16 data_len = (u16)0;
  u16 remain = (u16)0;
  u16 parsed = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -1262;
              goto out;
            }
    }
  remain = len;
  ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
                     & hdr_len,& data_len);
  if (ret) goto out;
  /*@ assert
        rte_110: signed_overflow: -2147483648 ≤ (int)remain - (int)hdr_len;
  */
  /*@ assert rte_111: signed_overflow: (int)remain - (int)hdr_len ≤ 2147483647;
  */
  remain = (u16)((int)remain - (int)hdr_len);
  buf += (int)hdr_len;
  if (! data_len) {
    ret = -1282;
    goto out;
  }
  ret = parse_NoticeReference(buf,remain,& parsed);
  if (! ret) {
    /*@ assert
        rte_112: signed_overflow: -2147483648 ≤ (int)remain - (int)parsed;
    */
    /*@ assert
        rte_113: signed_overflow: (int)remain - (int)parsed ≤ 2147483647;
    */
    remain = (u16)((int)remain - (int)parsed);
    buf += (int)parsed;
  }
  ret = parse_DisplayText(buf,remain,& parsed);
  if (! ret) {
    /*@ assert
        rte_114: signed_overflow: -2147483648 ≤ (int)remain - (int)parsed;
    */
    /*@ assert
        rte_115: signed_overflow: (int)remain - (int)parsed ≤ 2147483647;
    */
    remain = (u16)((int)remain - (int)parsed);
    buf += (int)parsed;
  }
  if (remain) {
    ret = -1302;
    goto out;
  }
  /*@ assert rte_116: mem_access: \valid(eaten); */
  /*@ assert
        rte_117: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
  */
  /*@ assert
        rte_118: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
  */
  *eaten = (u16)((int)hdr_len + (int)data_len);
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires (buf != \null && len > 0) ==> \valid(eaten);
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result == -1340;
ensures \result == 0 ==> *eaten >= 1;
ensures \result == 0 ==> *eaten <= len;
*/
static int parse_policyQualifierInfo(u8 const *buf, u16 len, u16 *eaten)
{
  int ret;
  u16 hdr_len = (u16)0;
  u16 data_len = (u16)0;
  u16 oid_len = (u16)0;
  u16 remain = (u16)0;
  u8 id_qt_cps_oid[10] =
    {(u8)0x06,
     (u8)0x08,
     (u8)0x2b,
     (u8)0x06,
     (u8)0x01,
     (u8)0x05,
     (u8)0x05,
     (u8)0x07,
     (u8)0x02,
     (u8)0x01};
  u8 id_qt_unotice_oid[10] =
    {(u8)0x06,
     (u8)0x08,
     (u8)0x2b,
     (u8)0x06,
     (u8)0x01,
     (u8)0x05,
     (u8)0x05,
     (u8)0x07,
     (u8)0x02,
     (u8)0x02};
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -1340;
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
                     & hdr_len,& data_len);
  if (ret) goto out;
  remain = data_len;
  buf += (int)hdr_len;
  ret = parse_OID(buf,remain,& oid_len);
  if (ret) goto out;
  if ((unsigned long)oid_len == sizeof(id_qt_cps_oid)) {
    int tmp_0;
    tmp_0 = bufs_differ(buf,(u8 const *)(id_qt_cps_oid),oid_len);
    if (tmp_0) goto _LAND_0;
    else {
      u16 cpsuri_len = (u16)0;
      buf += (int)oid_len;
      /*@ assert
        rte_119: signed_overflow: -2147483648 ≤ (int)remain - (int)oid_len;
      */
      /*@ assert
        rte_120: signed_overflow: (int)remain - (int)oid_len ≤ 2147483647;
      */
      remain = (u16)((int)remain - (int)oid_len);
      ret = parse_CPSuri(buf,remain,& cpsuri_len);
      if (ret) goto out;
      /*@ assert
        rte_121: signed_overflow: -2147483648 ≤ (int)remain - (int)cpsuri_len;
      */
      /*@ assert
        rte_122: signed_overflow: (int)remain - (int)cpsuri_len ≤ 2147483647;
      */
      remain = (u16)((int)remain - (int)cpsuri_len);
      buf += (int)cpsuri_len;
    }
  }
  else {
    _LAND_0: ;
    if ((unsigned long)oid_len == sizeof(id_qt_unotice_oid)) {
      int tmp;
      tmp = bufs_differ(buf,(u8 const *)(id_qt_unotice_oid),oid_len);
      if (tmp) goto _LAND;
      else {
        u16 cpsunotice_len = (u16)0;
        buf += (int)oid_len;
        /*@ assert
        rte_123: signed_overflow: -2147483648 ≤ (int)remain - (int)oid_len;
        */
        /*@ assert
        rte_124: signed_overflow: (int)remain - (int)oid_len ≤ 2147483647;
        */
        remain = (u16)((int)remain - (int)oid_len);
        ret = parse_UserNotice(buf,remain,& cpsunotice_len);
        if (ret) goto out;
        /*@ assert
        rte_125: signed_overflow:
              -2147483648 ≤ (int)remain - (int)cpsunotice_len;
        */
        /*@ assert
        rte_126: signed_overflow:
              (int)remain - (int)cpsunotice_len ≤ 2147483647;
        */
        remain = (u16)((int)remain - (int)cpsunotice_len);
        buf += (int)cpsunotice_len;
      }
    }
    else {
      _LAND: {
               ret = -1399;
               goto out;
             }
    }
  }
  /*@ assert rte_127: mem_access: \valid(eaten); */
  /*@ assert
        rte_128: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
  */
  /*@ assert
        rte_129: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
  */
  *eaten = (u16)((int)hdr_len + (int)data_len);
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(eaten);
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires len <= 65534;
requires \separated(eaten, buf + (0 .. len - 1));
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (\result == 0) ==> (*eaten >= 1 && *eaten <= len);
ensures (buf == \null || len == 0) ==> \result == -1426;
*/
static int parse_PolicyInformation(u8 const *buf, u16 len, u16 *eaten)
{
  u16 saved_pi_len;
  u16 remain;
  int ret;
  u16 hdr_len = (u16)0;
  u16 data_len = (u16)0;
  u16 oid_len = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -1426;
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
                     & hdr_len,& data_len);
  if (ret) goto out;
  /*@ assert
        rte_130: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
  */
  /*@ assert
        rte_131: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
  */
  saved_pi_len = (u16)((int)hdr_len + (int)data_len);
  remain = data_len;
  buf += (int)hdr_len;
  ret = parse_OID(buf,remain,& oid_len);
  if (ret) goto out;
  /*@ assert
        rte_132: signed_overflow: -2147483648 ≤ (int)remain - (int)oid_len;
  */
  /*@ assert rte_133: signed_overflow: (int)remain - (int)oid_len ≤ 2147483647;
  */
  remain = (u16)((int)remain - (int)oid_len);
  buf += (int)oid_len;
  if (remain) {
    ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
                       & hdr_len,& data_len);
    if (ret) goto out;
    /*@ assert
        rte_134: signed_overflow: -2147483648 ≤ (int)remain - (int)hdr_len;
    */
    /*@ assert
        rte_135: signed_overflow: (int)remain - (int)hdr_len ≤ 2147483647;
    */
    remain = (u16)((int)remain - (int)hdr_len);
    buf += (int)hdr_len;
    if ((int)remain != (int)data_len) {
      ret = -1469;
      goto out;
    }
    /*@
    loop invariant 0 <= remain <= data_len;
    loop invariant \valid_read(buf + (0 .. remain - 1));
    loop assigns remain, buf, ret;
    */
    while (remain) {
      u16 pqi_len = (u16)0;
      ret = parse_policyQualifierInfo(buf,remain,& pqi_len);
      if (ret) goto out;
      /*@ assert
        rte_136: signed_overflow: -2147483648 ≤ (int)remain - (int)pqi_len;
      */
      /*@ assert
        rte_137: signed_overflow: (int)remain - (int)pqi_len ≤ 2147483647;
      */
      remain = (u16)((int)remain - (int)pqi_len);
      buf += (int)pqi_len;
    }
  }
  /*@ assert rte_138: mem_access: \valid(eaten); */
  *eaten = saved_pi_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(ctx);
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires len <= 65534;
assigns \nothing;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result == -1582;
*/
static int parse_ext_certPolicies(u8 const *buf, u16 len, int critical,
                                  cert_parsing_ctx *ctx)
{
  int ret;
  u16 remain = (u16)0;
  u16 data_len = (u16)0;
  u16 hdr_len = (u16)0;
  u16 eaten = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if ((int)len == 0) {
      _LOR: {
              ret = -1582;
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
                     & hdr_len,& data_len);
  if (ret) goto out;
  buf += (int)hdr_len;
  remain = data_len;
  /*@ assert
        rte_139: signed_overflow: -2147483648 ≤ (int)hdr_len + (int)data_len;
  */
  /*@ assert
        rte_140: signed_overflow: (int)hdr_len + (int)data_len ≤ 2147483647;
  */
  if ((int)len != (int)hdr_len + (int)data_len) {
    ret = -1614;
    goto out;
  }
  /*@
  loop invariant 0 <= remain <= \at(data_len, LoopEntry);
  loop invariant \valid_read(buf + (0 .. remain - 1));
  loop assigns remain, buf, eaten, ret;
  */
  while (remain) {
    ret = parse_PolicyInformation(buf,remain,& eaten);
    if (ret) goto out;
    /*@ assert
        rte_141: signed_overflow: -2147483648 ≤ (int)remain - (int)eaten;
    */
    /*@ assert rte_142: signed_overflow: (int)remain - (int)eaten ≤ 2147483647;
    */
    remain = (u16)((int)remain - (int)eaten);
    buf += (int)eaten;
  }
  ret = 0;
  out: ;
  return ret;
}