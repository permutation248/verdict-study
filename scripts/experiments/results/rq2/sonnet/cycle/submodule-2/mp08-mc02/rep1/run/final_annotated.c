#include "__fc_builtin.h"
#include "stddef.h"
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
struct __anonstruct__name_oid_2 {
   u8 const *oid ;
   u8 oid_len ;
   int (*parse_rdn_val)(u8 const *buf, u32 len) ;
};
typedef struct __anonstruct__name_oid_2 _name_oid;
struct __anonstruct_cert_parsing_ctx_3 {
   int has_crldp ;
   int one_crldp_has_all_reasons ;
};
typedef struct __anonstruct_cert_parsing_ctx_3 cert_parsing_ctx;
int bufs_differ(u8 const *b1, u8 const *b2, u32 n);
static int _extract_complex_tag(u8 const *buf, u32 len, u32 *tag_num,
                                u32 *eaten);
static int get_identifier(u8 const *buf, u32 len, tag_class *cls, u8 *prim,
                          u32 *tag_num, u32 *eaten);
int get_length(u8 const *buf, u32 len, u32 *adv_len, u32 *eaten);
static int _parse_arc(u8 const *buf, u32 len, u32 *arc_val, u32 *eaten);
int parse_id_len(u8 const *buf, u32 len, tag_class exp_class, u32 exp_type,
                 u32 *parsed, u32 *content_len);
static u8 const null_encoded_val[2] = {(u8)0x05, (u8)0x00};
int parse_null(u8 const *buf, u32 len, u32 *parsed);
int parse_OID(u8 const *buf, u32 len, u32 *parsed);
static int check_utf8_string(u8 const *buf, u32 len);
static int check_printable_string(u8 const *buf, u32 len);
static int check_numeric_string(u8 const *buf, u32 len);
static int check_visible_string(u8 const *buf, u32 len);
static int check_teletex_string(u8 const *buf, u32 len);
static int check_universal_string(u8 const *buf, u32 len);
static int check_bmp_string(u8 const *buf, u32 len);
static int check_ia5_string(u8 const *buf, u32 len);
static int parse_directory_string(u8 const *buf, u32 len, u32 lb, u32 ub);
static int parse_printable_string(u8 const *buf, u32 len, u32 lb, u32 ub);
static int parse_numeric_string(u8 const *buf, u32 len, u32 lb, u32 ub);
int parse_ia5_string(u8 const *buf, u32 len, u32 lb, u32 ub);
static int parse_utf8_string(u8 const *buf, u32 len, u32 lb, u32 ub);
static int parse_rdn_val_cn(u8 const *buf, u32 len);
static int parse_rdn_val_x520name(u8 const *buf, u32 len);
static int parse_rdn_val_emailaddress(u8 const *buf, u32 len);
static int parse_rdn_val_serial(u8 const *buf, u32 len);
static int parse_rdn_val_country(u8 const *buf, u32 len);
static int parse_rdn_val_locality(u8 const *buf, u32 len);
static int parse_rdn_val_state(u8 const *buf, u32 len);
static int parse_rdn_val_org(u8 const *buf, u32 len);
static int parse_rdn_val_org_unit(u8 const *buf, u32 len);
static int parse_rdn_val_title(u8 const *buf, u32 len);
static int parse_rdn_val_dn_qual(u8 const *buf, u32 len);
__inline static int _is_digit(u8 c);
__inline static int _is_alpha(u8 c);
static int parse_rdn_val_dc(u8 const *buf, u32 len);
static int parse_rdn_val_pseudo(u8 const *buf, u32 len);
static int parse_rdn_val_ogrn(u8 const *buf, u32 len);
static int parse_rdn_val_snils(u8 const *buf, u32 len);
static int parse_rdn_val_ogrnip(u8 const *buf, u32 len);
static int parse_rdn_val_inn(u8 const *buf, u32 len);
static int parse_rdn_val_street_address(u8 const *buf, u32 len);
static int parse_rdn_val_bad_oid(u8 const *buf, u32 len);
static u8 const _dn_oid_cn[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x03};
static u8 const _dn_oid_surname[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x04};
static u8 const _dn_oid_serial[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x05};
static u8 const _dn_oid_country[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x06};
static u8 const _dn_oid_locality[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x07};
static u8 const _dn_oid_state[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x08};
static u8 const _dn_oid_org[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x0a};
static u8 const _dn_oid_org_unit[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x0b};
static u8 const _dn_oid_title[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x0c};
static u8 const _dn_oid_name[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x29};
static u8 const _dn_oid_emailaddress[11] =
  {(u8)0x06,
   (u8)0x09,
   (u8)0x2a,
   (u8)0x86,
   (u8)0x48,
   (u8)0x86,
   (u8)0xf7,
   (u8)0x0d,
   (u8)0x01,
   (u8)0x09,
   (u8)0x01};
static u8 const _dn_oid_given_name[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2a};
static u8 const _dn_oid_initials[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2b};
static u8 const _dn_oid_gen_qual[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2c};
static u8 const _dn_oid_dn_qual[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2e};
static u8 const _dn_oid_pseudo[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x41};
static u8 const _dn_oid_dc[12] =
  {(u8)0x06,
   (u8)0x0a,
   (u8)0x09,
   (u8)0x92,
   (u8)0x26,
   (u8)0x89,
   (u8)0x93,
   (u8)0xf2,
   (u8)0x2c,
   (u8)0x64,
   (u8)0x01,
   (u8)0x19};
static u8 const _dn_oid_ogrn[7] =
  {(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x01};
static u8 const _dn_oid_snils[7] =
  {(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x03};
static u8 const _dn_oid_ogrnip[7] =
  {(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x05};
static u8 const _dn_oid_inn[10] =
  {(u8)0x06,
   (u8)0x08,
   (u8)0x2a,
   (u8)0x85,
   (u8)0x03,
   (u8)0x03,
   (u8)0x81,
   (u8)0x03,
   (u8)0x01,
   (u8)0x01};
static u8 const _dn_oid_street_address[5] =
  {(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x09};
static _name_oid const generic_unsupported_rdn_oid =
  {.oid = (u8 const *)0,
   .oid_len = (u8)0,
   .parse_rdn_val = & parse_rdn_val_bad_oid};
static _name_oid const known_dn_oids[22] =
  {{.oid = _dn_oid_cn,
    .oid_len = (u8)sizeof(_dn_oid_cn),
    .parse_rdn_val = & parse_rdn_val_cn},
   {.oid = _dn_oid_surname,
    .oid_len = (u8)sizeof(_dn_oid_surname),
    .parse_rdn_val = & parse_rdn_val_x520name},
   {.oid = _dn_oid_serial,
    .oid_len = (u8)sizeof(_dn_oid_serial),
    .parse_rdn_val = & parse_rdn_val_serial},
   {.oid = _dn_oid_country,
    .oid_len = (u8)sizeof(_dn_oid_country),
    .parse_rdn_val = & parse_rdn_val_country},
   {.oid = _dn_oid_locality,
    .oid_len = (u8)sizeof(_dn_oid_locality),
    .parse_rdn_val = & parse_rdn_val_locality},
   {.oid = _dn_oid_state,
    .oid_len = (u8)sizeof(_dn_oid_state),
    .parse_rdn_val = & parse_rdn_val_state},
   {.oid = _dn_oid_org,
    .oid_len = (u8)sizeof(_dn_oid_org),
    .parse_rdn_val = & parse_rdn_val_org},
   {.oid = _dn_oid_org_unit,
    .oid_len = (u8)sizeof(_dn_oid_org_unit),
    .parse_rdn_val = & parse_rdn_val_org_unit},
   {.oid = _dn_oid_title,
    .oid_len = (u8)sizeof(_dn_oid_title),
    .parse_rdn_val = & parse_rdn_val_title},
   {.oid = _dn_oid_name,
    .oid_len = (u8)sizeof(_dn_oid_name),
    .parse_rdn_val = & parse_rdn_val_x520name},
   {.oid = _dn_oid_emailaddress,
    .oid_len = (u8)sizeof(_dn_oid_emailaddress),
    .parse_rdn_val = & parse_rdn_val_emailaddress},
   {.oid = _dn_oid_given_name,
    .oid_len = (u8)sizeof(_dn_oid_given_name),
    .parse_rdn_val = & parse_rdn_val_x520name},
   {.oid = _dn_oid_initials,
    .oid_len = (u8)sizeof(_dn_oid_initials),
    .parse_rdn_val = & parse_rdn_val_x520name},
   {.oid = _dn_oid_gen_qual,
    .oid_len = (u8)sizeof(_dn_oid_gen_qual),
    .parse_rdn_val = & parse_rdn_val_x520name},
   {.oid = _dn_oid_dn_qual,
    .oid_len = (u8)sizeof(_dn_oid_dn_qual),
    .parse_rdn_val = & parse_rdn_val_dn_qual},
   {.oid = _dn_oid_pseudo,
    .oid_len = (u8)sizeof(_dn_oid_pseudo),
    .parse_rdn_val = & parse_rdn_val_pseudo},
   {.oid = _dn_oid_dc,
    .oid_len = (u8)sizeof(_dn_oid_dc),
    .parse_rdn_val = & parse_rdn_val_dc},
   {.oid = _dn_oid_ogrn,
    .oid_len = (u8)sizeof(_dn_oid_ogrn),
    .parse_rdn_val = & parse_rdn_val_ogrn},
   {.oid = _dn_oid_snils,
    .oid_len = (u8)sizeof(_dn_oid_snils),
    .parse_rdn_val = & parse_rdn_val_snils},
   {.oid = _dn_oid_ogrnip,
    .oid_len = (u8)sizeof(_dn_oid_ogrnip),
    .parse_rdn_val = & parse_rdn_val_ogrnip},
   {.oid = _dn_oid_inn,
    .oid_len = (u8)sizeof(_dn_oid_inn),
    .parse_rdn_val = & parse_rdn_val_inn},
   {.oid = _dn_oid_street_address,
    .oid_len = (u8)sizeof(_dn_oid_street_address),
    .parse_rdn_val = & parse_rdn_val_street_address}};
static _name_oid const *find_dn_by_oid(u8 const *buf, u32 len);
static int parse_AttributeTypeAndValue(u8 const *buf, u32 len, u32 *eaten);
static int parse_RelativeDistinguishedName(u8 const *buf, u32 len, u32 *eaten);
int parse_x509_Name(u8 const *buf, u32 len, u32 *eaten, int *empty);
int parse_nine_bit_named_bit_list(u8 const *buf, u32 len, u16 *val);
int parse_crldp_reasons(u8 const *buf, u32 len, u32 exp_type, u32 *eaten);
int parse_GeneralName(u8 const *buf, u32 len, u32 *eaten, int *empty);
int parse_GeneralNames(u8 const *buf, u32 len, tag_class exp_class,
                       u32 exp_type, u32 *eaten);
int parse_DistributionPoint(u8 const *buf, u32 len,
                            int *crldp_has_all_reasons, u32 *eaten);
static int parse_ext_CRLDP(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
                           u32 len, int critical);
int main(void);

/*@
requires \valid_read(b1 + (0 .. n-1));
requires \valid_read(b2 + (0 .. n-1));
requires \separated(b1 + (0 .. n-1), b2 + (0 .. n-1));
assigns \nothing;
ensures \result == 0 || \result == 1;
ensures \result == 0 ==> \forall integer k; 0 <= k < n ==> b1[k] == b2[k];
ensures \result == 1 ==> \exists integer k; 0 <= k < n && b1[k] != b2[k];
*/
int bufs_differ(u8 const *b1, u8 const *b2, u32 n)
{
  int ret = 0;
  u32 i = (u32)0;
  i = (u32)0;
  /*@
  loop invariant 0 <= i <= n;
  loop invariant \valid_read(b1 + (0 .. n-1));
  loop invariant \valid_read(b2 + (0 .. n-1));
  loop invariant \forall integer k; 0 <= k < i ==> b1[k] == b2[k];
  loop invariant ret == 0;
  loop assigns i, ret;
  loop variant n - i;
  */
  while (i < n) {
    /*@ assert rte_35: mem_access: \valid_read(b1 + i); */
    /*@ assert rte_36: mem_access: \valid_read(b2 + i); */
    if ((int)*(b1 + i) != (int)*(b2 + i)) {
      ret = 1;
      break;
    }
    i ++;
  }
  return ret;
}

/*@
requires (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. (len <= 4 ? len - 1 : 3)));
requires \valid(tag_num);
requires (eaten != \null) ==> \valid(eaten);
requires \separated(tag_num, eaten);
assigns *tag_num, *eaten;
ensures \result == 0 || \result == -(3 * 100000 + 468) || \result == -(3 * 100000 + 506) || \result == -(3 * 100000 + 512);
ensures \result == 0 ==> (\valid(tag_num) && *tag_num >= 0x1f);
ensures \result == 0 ==> (\valid(eaten) && *eaten >= 1 && *eaten <= 5);
ensures (len == 0 || buf == \null || eaten == \null) ==> \result == -(3 * 100000 + 468);
*/
static int _extract_complex_tag(u8 const *buf, u32 len, u32 *tag_num,
                                u32 *eaten)
{
  u32 rbytes;
  int ret;
  u32 t = (u32)0;
  if (len == (u32)0) goto _LOR;
  else 
    if (buf == (u8 const *)0) goto _LOR;
    else 
      if (eaten == (u32 *)0) {
        _LOR: {
                ret = - (3 * 100000 + 468);
                goto out;
              }
      }
  if (len > (u32)4) len = (u32)4;
  rbytes = (u32)0;
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant len <= 4;
  loop invariant (len > 0 && buf != \null) ==> \valid_read(buf + (0 .. len - 1));
  loop assigns rbytes, t;
  loop variant len - rbytes;
  */
  while (rbytes < len) {
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
    rbytes ++;
  }
  if (rbytes == len) {
    ret = - (3 * 100000 + 506);
    goto out;
  }
  if (t < (u32)0x1f) {
    ret = - (3 * 100000 + 512);
    goto out;
  }
  /*@ assert rte_3: mem_access: \valid(tag_num); */
  *tag_num = t;
  /*@ assert rte_4: mem_access: \valid(eaten); */
  *eaten = rbytes + (u32)1;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires \valid(cls);
requires \valid(prim);
requires \valid(tag_num);
requires \valid(eaten);
requires \separated(cls, prim, tag_num, eaten);
assigns *cls, *prim, *tag_num, *eaten;
ensures \result == 0 || \result == -(3 * 100000 + 568) || \result == -(3 * 100000 + 579) || \result == -(3 * 100000 + 602) || \result == -(3 * 100000 + 468) || \result == -(3 * 100000 + 506) || \result == -(3 * 100000 + 512);
ensures buf == \null ==> \result == -(3 * 100000 + 568);
ensures (buf != \null && len == 0) ==> \result == -(3 * 100000 + 579);
ensures \result == 0 ==> (*eaten >= 1);
ensures \result == 0 ==> \valid(cls) && \valid(prim) && \valid(tag_num) && \valid(eaten);
*/
static int get_identifier(u8 const *buf, u32 len, tag_class *cls, u8 *prim,
                          u32 *tag_num, u32 *eaten)
{
  int ret;
  u32 t;
  u8 p;
  tag_class c;
  u32 rbytes = (u32)0;
  if (buf == (u8 const *)0) {
    ret = - (3 * 100000 + 568);
    goto out;
  }
  if (len == (u32)0) {
    ret = - (3 * 100000 + 579);
    goto out;
  }
  /*@ assert rte_5: mem_access: \valid_read(buf + 0); */
  c = (tag_class)(((int)*(buf + 0) >> 6) & 0x03);
  /*@ assert rte_6: mem_access: \valid_read(buf + 0); */
  p = (u8)(((int)*(buf + 0) >> 5) & 0x01);
  /*@ assert rte_7: mem_access: \valid_read(buf + 0); */
  t = (u32)((int)*(buf + 0) & 0x1f);
  rbytes = (u32)1;
  switch (c) {
    case CLASS_UNIVERSAL: case CLASS_APPLICATION:
    case CLASS_CONTEXT_SPECIFIC: case CLASS_PRIVATE:
    break;
    default: ret = - (3 * 100000 + 602);
    goto out;
    break;
  }
  if (t == (u32)0x1f) {
    u32 tag_len = (u32)0;
    ret = _extract_complex_tag(buf + rbytes,len - rbytes,& t,& tag_len);
    if (ret) goto out;
    rbytes += tag_len;
  }
  /*@ assert rte_8: mem_access: \valid(cls); */
  *cls = c;
  /*@ assert rte_9: mem_access: \valid(prim); */
  *prim = p;
  /*@ assert rte_10: mem_access: \valid(tag_num); */
  *tag_num = t;
  /*@ assert rte_11: mem_access: \valid(eaten); */
  *eaten = rbytes;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(adv_len);
requires \valid(eaten);
requires \separated(adv_len, eaten);
requires buf != \null ==> \valid_read(buf + (0..len-1));
requires buf != \null ==> \separated(buf + (0..len-1), adv_len);
requires buf != \null ==> \separated(buf + (0..len-1), eaten);
assigns *eaten, *adv_len;
ensures \result == 0 || \result < 0;
ensures \result == 0 ==> (*eaten >= 1 && *eaten <= 5);
ensures \result == 0 ==> *eaten <= len;
*/
int get_length(u8 const *buf, u32 len, u32 *adv_len, u32 *eaten)
{
  u32 l;
  u32 len_len;
  u32 b0;
  int ret;
  u32 rbytes = (u32)0;
  if (buf == (u8 const *)0) {
    ret = - (3 * 100000 + 674);
    goto out;
  }
  if (len == (u32)0) {
    ret = - (3 * 100000 + 680);
    goto out;
  }
  /*@ assert rte_12: mem_access: \valid_read(buf + 0); */
  b0 = (u32)*(buf + 0);
  if ((b0 & (unsigned int)0x80) == (unsigned int)0) {
    l = b0 & (unsigned int)0x7f;
    if (l + (u32)1 > len) {
      ret = - (3 * 100000 + 700);
      goto out;
    }
    /*@ assert rte_13: mem_access: \valid(eaten); */
    *eaten = (u32)1;
    /*@ assert rte_14: mem_access: \valid(adv_len); */
    *adv_len = l;
    ret = 0;
    goto out;
  }
  if (b0 == (u32)0x80) {
    ret = - (3 * 100000 + 719);
    goto out;
  }
  len_len = b0 & (unsigned int)0x7f;
  rbytes += (u32)1;
  if (len_len + (u32)1 > len) {
    ret = - (3 * 100000 + 740);
    goto out;
  }
  switch (len_len) {
    case (u32)0: ret = - (3 * 100000 + 749);
    goto out;
    break;
    case (u32)1:
    /*@ assert rte_15: mem_access: \valid_read(buf + 1); */
    l = (u32)*(buf + 1);
    if (l <= (u32)127) {
      ret = - (3 * 100000 + 762);
      goto out;
    }
    rbytes += (u32)1;
    break;
    case (u32)2:
    /*@ assert rte_16: mem_access: \valid_read(buf + 1); */
    /*@ assert rte_17: mem_access: \valid_read(buf + 2); */
    l = ((u32)*(buf + 1) << 8) + (u32)*(buf + 2);
    if (l <= (u32)0xff) {
      ret = - (3 * 100000 + 775);
      goto out;
    }
    rbytes += (u32)2;
    break;
    case (u32)3:
    /*@ assert rte_18: mem_access: \valid_read(buf + 1); */
    /*@ assert rte_19: mem_access: \valid_read(buf + 2); */
    /*@ assert rte_20: mem_access: \valid_read(buf + 3); */
    l = (((u32)*(buf + 1) << 16) + ((u32)*(buf + 2) << 8)) + (u32)*(buf + 3);
    if (l <= (u32)0xffff) {
      ret = - (3 * 100000 + 788);
      goto out;
    }
    rbytes += (u32)3;
    break;
    case (u32)4:
    /*@ assert rte_21: mem_access: \valid_read(buf + 1); */
    /*@ assert rte_22: mem_access: \valid_read(buf + 2); */
    /*@ assert rte_23: mem_access: \valid_read(buf + 3); */
    /*@ assert rte_24: mem_access: \valid_read(buf + 4); */
    l = ((((u32)*(buf + 1) << 24) + ((u32)*(buf + 2) << 16)) + ((u32)*(
                                                                buf + 3) << 8)) + (u32)*(
        buf + 4);
    if (l <= (u32)0xffffff) {
      ret = - (3 * 100000 + 801);
      goto out;
    }
    rbytes += (u32)4;
    break;
    default: ret = - (3 * 100000 + 816);
    goto out;
    break;
  }
  if (len - rbytes < l) {
    ret = - (3 * 100000 + 825);
    goto out;
  }
  /*@ assert rte_25: mem_access: \valid(eaten); */
  *eaten = rbytes;
  /*@ assert rte_26: mem_access: \valid(adv_len); */
  *adv_len = l;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(arc_val);
requires \valid(eaten);
requires \separated(arc_val, eaten);
requires (buf != \null && len > 0 && len <= 4) ==> \valid_read(buf + (0 .. len - 1));
requires (buf != \null && len > 4) ==> \valid_read(buf + (0 .. 3));
assigns *arc_val, *eaten;
ensures \result == 0 || \result == -301048 || \result == -301095;
ensures \result == 0 ==> (*eaten >= 1 && *eaten <= 4);
ensures \result == 0 ==> *eaten <= \old(len);
*/
static int _parse_arc(u8 const *buf, u32 len, u32 *arc_val, u32 *eaten)
{
  u32 rbytes;
  int ret;
  u32 av = (u32)0;
  if (len == (u32)0) goto _LOR;
  else 
    if (buf == (u8 const *)0) {
      _LOR: {
              ret = - (3 * 100000 + 1048);
              goto out;
            }
    }
  if (len > (u32)4) len = (u32)4;
  rbytes = (u32)0;
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant len > 0;
  loop invariant len <= 4;
  loop invariant \valid_read(buf + (0 .. len - 1));
  loop assigns rbytes, av;
  loop variant len - rbytes;
  */
  while (rbytes < len) {
    {
      u32 tmp1;
      u32 tmp2;
      tmp1 = av << (u32)7;
      /*@ assert rte_30: mem_access: \valid_read(buf + rbytes); */
      tmp2 = (u32)*(buf + rbytes) & (u32)0x7f;
      av = tmp1 + tmp2;
      /*@ assert rte_31: mem_access: \valid_read(buf + rbytes); */
      if (((int)*(buf + rbytes) & 0x80) == 0) break;
    }
    rbytes ++;
  }
  if (rbytes >= len) {
    ret = - (3 * 100000 + 1095);
    goto out;
  }
  /*@ assert rte_32: mem_access: \valid(arc_val); */
  *arc_val = av;
  /*@ assert rte_33: mem_access: \valid(eaten); */
  *eaten = rbytes + (u32)1;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires \valid(parsed);
requires \valid(content_len);
requires \separated(parsed, content_len);
assigns *parsed, *content_len;
ensures \result == 0 || \result < 0;
ensures buf == \null ==> \result == -(3 * 100000 + 883);
ensures \result == 0 ==> (*parsed >= 2 && *parsed <= len);
ensures \result == 0 ==> (*content_len <= len - *parsed);
ensures \result == 0 ==> \valid(parsed) && \valid(content_len);
*/
int parse_id_len(u8 const *buf, u32 len, tag_class exp_class, u32 exp_type,
                 u32 *parsed, u32 *content_len)
{
  u8 p;
  u32 grabbed;
  int ret;
  tag_class c = (tag_class)0;
  u32 t = (u32)0;
  u32 cur_parsed = (u32)0;
  u32 adv_len = (u32)0;
  if (buf == (u8 const *)0) {
    ret = - (3 * 100000 + 883);
    goto out;
  }
  ret = get_identifier(buf,len,& c,& p,& t,& cur_parsed);
  if (ret) goto out;
  if (t != exp_type) {
    ret = - (3 * 100000 + 901);
    goto out;
  }
  if (c != exp_class) {
    ret = - (3 * 100000 + 908);
    goto out;
  }
  grabbed = cur_parsed;
  len -= cur_parsed;
  buf += cur_parsed;
  ret = get_length(buf,len,& adv_len,& cur_parsed);
  if (ret) goto out;
  grabbed += cur_parsed;
  len -= cur_parsed;
  buf += cur_parsed;
  if (adv_len > len) {
    ret = - (3 * 100000 + 933);
    goto out;
  }
  /*@ assert rte_27: mem_access: \valid(parsed); */
  *parsed = grabbed;
  /*@ assert rte_28: mem_access: \valid(content_len); */
  *content_len = adv_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires buf != \null ==> \valid_read(buf + (0 .. len-1));
requires parsed != \null ==> \valid(parsed);
assigns *parsed;
ensures \result == 0 || \result == -(3*100000+1133) || \result == -(3*100000+1139) || \result == -(3*100000+1146);
ensures \result == 0 ==> \valid(parsed);
ensures \result == 0 ==> *parsed == 2;
ensures \result == 0 ==> len == 2;
*/
int parse_null(u8 const *buf, u32 len, u32 *parsed)
{
  int ret;
  if (len == (u32)0) goto _LOR;
  else 
    if (buf == (u8 const *)0) goto _LOR;
    else 
      if (parsed == (u32 *)0) {
        _LOR: {
                ret = - (3 * 100000 + 1133);
                goto out;
              }
      }
  if ((unsigned long)len != sizeof(null_encoded_val)) {
    ret = - (3 * 100000 + 1139);
    goto out;
  }
  ret = bufs_differ(buf,null_encoded_val,(u32)sizeof(null_encoded_val));
  if (ret) {
    ret = - (3 * 100000 + 1146);
    goto out;
  }
  ret = 0;
  /*@ assert rte_114: mem_access: \valid(parsed); */
  *parsed = (u32)sizeof(null_encoded_val);
  out: ;
  return ret;
}

/*@
requires \valid(parsed);
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
requires (buf != \null && len > 0) ==> \separated(parsed, buf + (0 .. len - 1));
assigns *parsed;
ensures \result == 0 || \result < 0;
ensures \result == 0 ==> (*parsed >= 1);
ensures \result == 0 ==> (*parsed <= len);
*/
int parse_OID(u8 const *buf, u32 len, u32 *parsed)
{
  u32 num;
  int ret;
  u32 data_len = (u32)0;
  u32 hdr_len = (u32)0;
  u32 remain = (u32)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 1185);
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)0x06,& hdr_len,& data_len);
  if (ret) goto out;
  len -= hdr_len;
  buf += hdr_len;
  if (data_len < (u32)1) {
    ret = - (3 * 100000 + 1200);
    goto out;
  }
  remain = data_len;
  num = (u32)0;
  /*@
  loop invariant 0 <= num <= 21;
  loop invariant remain > 0 ==> \valid_read(buf + (0 .. remain - 1));
  loop assigns num, buf, remain, ret;
  loop variant remain;
  */
  while (remain) {
    u32 arc_val = (u32)0;
    u32 rbytes = (u32)0;
    if (num > (u32)20) {
      ret = - (3 * 100000 + 1223);
      goto out;
    }
    ret = _parse_arc(buf,remain,& arc_val,& rbytes);
    if (ret) goto out;
    num += (u32)1;
    buf += rbytes;
    remain -= rbytes;
  }
  if (num < (u32)1) {
    ret = - (3 * 100000 + 1248);
    goto out;
  }
  /*@ assert rte_34: mem_access: \valid(parsed); */
  *parsed = hdr_len + data_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires buf != \null ==> \valid_read(buf + (0 .. len - 1));
assigns \nothing;
ensures \result == 0 || \result < 0;
ensures (\at(buf, Pre) == \null || \at(len, Pre) == 0) ==> \result < 0;
*/
static int check_utf8_string(u8 const *buf, u32 len)
{
  int ret;
  u8 b0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 1833);
              goto out;
            }
    }
  /*@
  loop invariant buf != \null;
  loop invariant \at(len, Pre) >= len;
  loop invariant buf == \at(buf, Pre) + (\at(len, Pre) - len);
  loop invariant \valid_read(buf + (0 .. len - 1));
  loop assigns len, buf, b0, ret;
  loop variant len;
  */
  while (len) {
    /*@ assert rte_42: mem_access: \valid_read(buf + 0); */
    b0 = *(buf + 0);
    if ((int)b0 <= 0x7f) {
      len -= (u32)1;
      buf ++;
      continue;
    }
    if ((int)b0 >= 0xc2) 
      if ((int)b0 <= 0xdf) {
        if (len < (u32)2) {
          ret = - (3 * 100000 + 1864);
          goto out;
        }
        /*@ assert rte_43: mem_access: \valid_read(buf + 1); */
        if (((int)*(buf + 1) & 0xc0) != 0x80) {
          ret = - (3 * 100000 + 1871);
          goto out;
        }
        len -= (u32)2;
        buf += 2;
        continue;
      }
    if ((int)b0 >= 0xe0) 
      if ((int)b0 <= 0xef) {
        if (len < (u32)3) {
          ret = - (3 * 100000 + 1888);
          goto out;
        }
        /*@ assert rte_44: mem_access: \valid_read(buf + 1); */
        if (((int)*(buf + 1) & 0xc0) != 0x80) goto _LOR_0;
        else 
          /*@ assert rte_45: mem_access: \valid_read(buf + 2); */
          if (((int)*(buf + 2) & 0xc0) != 0x80) {
            _LOR_0: {
                      ret = - (3 * 100000 + 1896);
                      goto out;
                    }
          }
        if ((int)b0 == 0xe0) {
          /*@ assert rte_46: mem_access: \valid_read(buf + 1); */
          if ((int)*(buf + 1) < 0xa0) goto _LOR_2;
          else 
            /*@ assert rte_47: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) > 0xbf) {
              _LOR_2: {
                        ret = - (3 * 100000 + 1907);
                        goto out;
                      }
            }
            else goto _LAND;
        }
        else {
          _LAND: ;
          if ((int)b0 == 0xed) 
            /*@ assert rte_48: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) < 0x80) goto _LOR_1;
            else 
              /*@ assert rte_49: mem_access: \valid_read(buf + 1); */
              if ((int)*(buf + 1) > 0x9f) {
                _LOR_1: {
                          ret = - (3 * 100000 + 1912);
                          goto out;
                        }
              }
        }
        len -= (u32)3;
        buf += 3;
        continue;
      }
    if ((int)b0 >= 0xf0) 
      if ((int)b0 <= 0xf4) {
        if (len < (u32)4) {
          ret = - (3 * 100000 + 1929);
          goto out;
        }
        if ((int)b0 == 0xf0) {
          /*@ assert rte_50: mem_access: \valid_read(buf + 1); */
          if ((int)*(buf + 1) < 0x90) goto _LOR_4;
          else 
            /*@ assert rte_51: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) > 0xbf) {
              _LOR_4: {
                        ret = - (3 * 100000 + 1941);
                        goto out;
                      }
            }
            else goto _LAND_0;
        }
        else {
          _LAND_0: ;
          if ((int)b0 == 0xf4) 
            /*@ assert rte_52: mem_access: \valid_read(buf + 1); */
            if ((int)*(buf + 1) < 0x80) goto _LOR_3;
            else 
              /*@ assert rte_53: mem_access: \valid_read(buf + 1); */
              if ((int)*(buf + 1) > 0x8f) {
                _LOR_3: {
                          ret = - (3 * 100000 + 1946);
                          goto out;
                        }
              }
        }
        /*@ assert rte_54: mem_access: \valid_read(buf + 1); */
        if (((int)*(buf + 1) & 0xc0) != 0x80) goto _LOR_5;
        else 
          /*@ assert rte_55: mem_access: \valid_read(buf + 2); */
          if (((int)*(buf + 2) & 0xc0) != 0x80) goto _LOR_5;
          else 
            /*@ assert rte_56: mem_access: \valid_read(buf + 3); */
            if (((int)*(buf + 3) & 0xc0) != 0x80) {
              _LOR_5: {
                        ret = - (3 * 100000 + 1954);
                        goto out;
                      }
            }
        len -= (u32)4;
        buf += 4;
        continue;
      }
    ret = - (3 * 100000 + 1964);
    goto out;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result == -302001 || \result == -302038;
ensures (buf == \null || len == 0) ==> \result == -302001;
ensures \result == 0 ==> (buf != \null && len > 0);
*/
static int check_printable_string(u8 const *buf, u32 len)
{
  int ret;
  u32 rbytes;
  u8 c;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 2001);
              goto out;
            }
    }
  rbytes = (u32)0;
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant \valid_read(buf + (0..len-1));
  loop assigns rbytes, c, ret;
  loop variant len - rbytes;
  */
  while (rbytes < len) {
    /*@ assert rte_41: mem_access: \valid_read(buf + rbytes); */
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
          if ((int)c <= '9') _LOR_0: goto __Cont;
      }
    }
    switch ((int)c) {
      case 39: case '=': case '(': case ')': case '+': case ',': case '-':
      case '.': case '/': case ':': case '?': case ' ':
      goto __Cont;
      default: break;
    }
    ret = - (3 * 100000 + 2038);
    goto out;
    __Cont: rbytes ++;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid_read(buf + (0 .. len-1)) || buf == \null || len == 0;
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3*100000+2069) || \result == -(3*100000+2083);
ensures (buf == \null || len == 0) ==> \result == -(3*100000+2069);
ensures \result == 0 ==> (buf != \null && len > 0);
*/
static int check_numeric_string(u8 const *buf, u32 len)
{
  int ret;
  u32 rbytes;
  u8 c;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 2069);
              goto out;
            }
    }
  rbytes = (u32)0;
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant \valid_read(buf + (0 .. len-1));
  loop invariant buf != \null;
  loop assigns rbytes, c;
  loop variant len - rbytes;
  */
  while (rbytes < len) {
    /*@ assert rte_58: mem_access: \valid_read(buf + rbytes); */
    c = *(buf + rbytes);
    if ((int)c < '0') goto _LOR_0;
    else 
      if ((int)c > '9') {
        _LOR_0: {
                  ret = - (3 * 100000 + 2083);
                  goto out;
                }
      }
    rbytes ++;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires buf != \null ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result == -302114 || \result == -302172;
ensures (buf == \null || len == 0) ==> \result == -302114;
*/
static int check_visible_string(u8 const *buf, u32 len)
{
  int ret;
  u8 c;
  u32 rbytes = (u32)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 2114);
              goto out;
            }
    }
  /*@
  loop invariant 0 <= rbytes <= len;
  loop invariant buf != \null;
  loop invariant \valid_read(buf + (0 .. len-1));
  loop assigns rbytes, c;
  loop variant len - rbytes;
  */
  while (rbytes < len) {
    /*@ assert rte_57: mem_access: \valid_read(buf + rbytes); */
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
            _LOR_0: {
                      rbytes += (u32)1;
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
      rbytes += (u32)1;
      continue;
      default: break;
    }
    ret = - (3 * 100000 + 2172);
    goto out;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires buf != \null ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result == -302114 || \result == -302172;
ensures (buf == \null || len == 0) ==> \result == -302114;
*/
static int check_teletex_string(u8 const *buf, u32 len)
{
  int tmp;
  tmp = check_visible_string(buf,len);
  return tmp;
}

/*@
assigns \nothing;
ensures \result == -302215;
*/
static int check_universal_string(u8 const *buf, u32 len)
{
  int __retres;
  __retres = - (3 * 100000 + 2215);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == -302232;
*/
static int check_bmp_string(u8 const *buf, u32 len)
{
  int __retres;
  __retres = - (3 * 100000 + 2232);
  return __retres;
}

/*@
requires (buf != \null && len > 0) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3 * 100000 + 2250) || \result == -(3 * 100000 + 2263);
ensures (\result == -(3 * 100000 + 2250)) ==> (buf == \null || len == 0);
ensures (\result == 0) ==> (buf != \null && len > 0);
*/
static int check_ia5_string(u8 const *buf, u32 len)
{
  int ret;
  u32 i;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 2250);
              goto out;
            }
    }
  i = (u32)0;
  /*@
  loop invariant 0 <= i <= len;
  loop invariant buf != \null;
  loop invariant len > 0;
  loop invariant \valid_read(buf + (0..len-1));
  loop assigns i, ret;
  loop variant len - i;
  */
  while (i < len) {
    /*@ assert rte_29: mem_access: \valid_read(buf + i); */
    if ((int)*(buf + i) > 0x7f) {
      ret = - (3 * 100000 + 2263);
      goto out;
    }
    i ++;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result < 0;
*/
static int parse_directory_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
  u8 str_type;
  int ret = - (3 * 100000 + 3735);
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 3739);
              goto out;
            }
    }
  if (len < (u32)2) {
    ret = - (3 * 100000 + 3745);
    goto out;
  }
  /*@ assert rte_59: mem_access: \valid_read(buf + 0); */
  str_type = *(buf + 0);
  len -= (u32)2;
  /*@ assert rte_60: mem_access: \valid_read(buf + 1); */
  if ((u32)*(buf + 1) != len) {
    ret = - (3 * 100000 + 3754);
    goto out;
  }
  buf += 2;
  if (len < lb) goto _LOR_0;
  else 
    if (len > ub) {
      _LOR_0: {
                ret = - (3 * 100000 + 3761);
                goto out;
              }
    }
  switch ((int)str_type) {
    case 19: ret = check_printable_string(buf,len);
    int tmp = ret;
    break;
    case 12: ret = check_utf8_string(buf,len);
    int tmp_3 = ret;
    break;
    case 20: ret = check_teletex_string(buf,len);
    int tmp_5 = ret;
    break;
    case 28: ret = check_universal_string(buf,len);
    int tmp_7 = ret;
    break;
    case 30: ret = check_bmp_string(buf,len);
    int tmp_9 = ret;
    break;
    case 22: ret = check_ia5_string(buf,len);
    int tmp_11 = ret;
    break;
    case 18: ret = check_numeric_string(buf,len);
    int tmp_13 = ret;
    break;
    default: ret = - (3 * 100000 + 3817);
    break;
  }
  if (ret) goto out;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result == -303849 || \result == -303853 || \result == -303859 || \result == -303866 || \result == -303873 || \result == -303880 || \result == -302001 || \result == -302038;
ensures (buf == \null || len == 0) ==> \result == -303853;
ensures \result == 0 ==> (buf != \null && len >= 2);
*/
static int parse_printable_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
  u8 str_type;
  int ret = - (3 * 100000 + 3849);
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 3853);
              goto out;
            }
    }
  if (len < (u32)2) {
    ret = - (3 * 100000 + 3859);
    goto out;
  }
  /*@ assert rte_61: mem_access: \valid_read(buf + 0); */
  str_type = *(buf + 0);
  if ((int)str_type != 19) {
    ret = - (3 * 100000 + 3866);
    goto out;
  }
  len -= (u32)2;
  /*@ assert rte_62: mem_access: \valid_read(buf + 1); */
  if ((u32)*(buf + 1) != len) {
    ret = - (3 * 100000 + 3873);
    goto out;
  }
  buf += 2;
  if (len < lb) goto _LOR_0;
  else 
    if (len > ub) {
      _LOR_0: {
                ret = - (3 * 100000 + 3880);
                goto out;
              }
    }
  ret = check_printable_string(buf,len);
  if (ret) goto out;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3*100000+3917) || \result == -(3*100000+3923) || \result == -(3*100000+3930) || \result == -(3*100000+3937) || \result == -(3*100000+3944) || \result == -(3*100000+2069) || \result == -(3*100000+2083);
ensures (buf == \null || len == 0) ==> \result == -(3*100000+3917);
ensures \result == 0 ==> (buf != \null && len >= 2);
*/
static int parse_numeric_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
  u8 str_type;
  int ret = - (3 * 100000 + 3913);
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 3917);
              goto out;
            }
    }
  if (len < (u32)2) {
    ret = - (3 * 100000 + 3923);
    goto out;
  }
  /*@ assert rte_70: mem_access: \valid_read(buf + 0); */
  str_type = *(buf + 0);
  if ((int)str_type != 18) {
    ret = - (3 * 100000 + 3930);
    goto out;
  }
  len -= (u32)2;
  /*@ assert rte_71: mem_access: \valid_read(buf + 1); */
  if ((u32)*(buf + 1) != len) {
    ret = - (3 * 100000 + 3937);
    goto out;
  }
  buf += 2;
  if (len < lb) goto _LOR_0;
  else 
    if (len > ub) {
      _LOR_0: {
                ret = - (3 * 100000 + 3944);
                goto out;
              }
    }
  ret = check_numeric_string(buf,len);
  if (ret) goto out;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3*100000+3981) || \result == -(3*100000+3987) || \result == -(3*100000+3994) || \result == -(3*100000+4001) || \result == -(3*100000+4008) || \result == -(3*100000+2250) || \result == -(3*100000+2263);
ensures (\result == -(3*100000+3981)) ==> (buf == \null || len == 0);
ensures \result == 0 ==> (buf != \null && len >= lb + 2);
*/
int parse_ia5_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
  u8 str_type;
  int ret = - (3 * 100000 + 3977);
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 3981);
              goto out;
            }
    }
  if (len < (u32)2) {
    ret = - (3 * 100000 + 3987);
    goto out;
  }
  /*@ assert rte_63: mem_access: \valid_read(buf + 0); */
  str_type = *(buf + 0);
  if ((int)str_type != 22) {
    ret = - (3 * 100000 + 3994);
    goto out;
  }
  len -= (u32)2;
  /*@ assert rte_64: mem_access: \valid_read(buf + 1); */
  if ((u32)*(buf + 1) != len) {
    ret = - (3 * 100000 + 4001);
    goto out;
  }
  buf += 2;
  if (len < lb) goto _LOR_0;
  else 
    if (len > ub) {
      _LOR_0: {
                ret = - (3 * 100000 + 4008);
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
requires buf != \null ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_utf8_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
  u8 str_type;
  int ret = - (3 * 100000 + 4038);
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 4042);
              goto out;
            }
    }
  if (len < (u32)2) {
    ret = - (3 * 100000 + 4048);
    goto out;
  }
  /*@ assert rte_65: mem_access: \valid_read(buf + 0); */
  str_type = *(buf + 0);
  if ((int)str_type != 22) {
    ret = - (3 * 100000 + 4055);
    goto out;
  }
  len -= (u32)2;
  /*@ assert rte_66: mem_access: \valid_read(buf + 1); */
  if ((u32)*(buf + 1) != len) {
    ret = - (3 * 100000 + 4062);
    goto out;
  }
  buf += 2;
  if (len < lb) goto _LOR_0;
  else 
    if (len > ub) {
      _LOR_0: {
                ret = - (3 * 100000 + 4069);
                goto out;
              }
    }
  ret = check_utf8_string(buf,len);
  if (ret) goto out;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result < 0;
*/
static int parse_rdn_val_cn(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)192);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result < 0;
*/
static int parse_rdn_val_x520name(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)32768);
  return tmp;
}

/*@
requires buf != \null ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_rdn_val_emailaddress(u8 const *buf, u32 len)
{
  int ret;
  ret = parse_ia5_string(buf,len,(u32)1,(u32)255);
  if (ret) ret = parse_utf8_string(buf,len,(u32)1,(u32)255);
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3*100000+3981) || \result == -(3*100000+3987) || \result == -(3*100000+3994) || \result == -(3*100000+4001) || \result == -(3*100000+4008) || \result == -(3*100000+2250) || \result == -(3*100000+2263);
ensures (buf == \null || len == 0) ==> \result != 0;
*/
static int parse_rdn_val_serial(u8 const *buf, u32 len)
{
  int ret;
  ret = parse_printable_string(buf,len,(u32)1,(u32)64);
  if (ret) ret = parse_ia5_string(buf,len,(u32)1,(u32)64);
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_rdn_val_country(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)2,(u32)2);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_rdn_val_locality(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_rdn_val_state(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_rdn_val_org(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)64);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_rdn_val_org_unit(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result <= 0;
*/
static int parse_rdn_val_title(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)64);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result == -303849 || \result == -303853 || \result == -303859 || \result == -303866 || \result == -303873 || \result == -303880 || \result == -302001 || \result == -302038;
ensures (buf == \null || len == 0) ==> \result == -303853;
ensures \result == 0 ==> (buf != \null && len >= 2);
*/
static int parse_rdn_val_dn_qual(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_printable_string(buf,len,(u32)1,(u32)0xffffffffUL);
  return tmp;
}

/*@
assigns \nothing;
ensures (\result == 1) <==> ((int)c >= '0' && (int)c <= '9');
ensures \result == 0 || \result == 1;
*/
__inline static int _is_digit(u8 c)
{
  int tmp;
  if ((int)c >= '0') 
    if ((int)c <= '9') tmp = 1; else tmp = 0;
  else tmp = 0;
  return tmp;
}

/*@
assigns \nothing;
ensures \result == 0 || \result == 1;
ensures ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ==> \result == 1;
ensures ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z')) ==> \result == 0;
*/
__inline static int _is_alpha(u8 c)
{
  int tmp;
  if ((int)c >= 'a') {
    if ((int)c <= 'z') tmp = 1; else goto _LAND;
  }
  else {
    _LAND: ;
    if ((int)c >= 'A') 
      if ((int)c <= 'Z') tmp = 1; else tmp = 0;
    else tmp = 0;
  }
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3*100000+4376) || \result == -(3*100000+4395) || \result == -(3*100000+4412) || \result == -(3*100000+4430) || \result == -(3*100000+3981) || \result == -(3*100000+3987) || \result == -(3*100000+3994) || \result == -(3*100000+4001) || \result == -(3*100000+4008) || \result == -(3*100000+2250) || \result == -(3*100000+2263);
*/
static int parse_rdn_val_dc(u8 const *buf, u32 len)
{
  int ret;
  u32 i;
  u8 c;
  int tmp_1;
  int tmp;
  int tmp_4;
  int tmp_2;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 4376);
              goto out;
            }
    }
  ret = parse_ia5_string(buf,len,(u32)1,(u32)63);
  if (ret) goto out;
  buf += 2;
  len -= (u32)2;
  /*@ assert rte_67: mem_access: \valid_read(buf + 0); */
  c = *(buf + 0);
  tmp = _is_alpha(c);
  if (tmp) tmp_1 = 1;
  else {
    int tmp_0;
    tmp_0 = _is_digit(c);
    if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
  }
  ret = tmp_1;
  if (! ret) {
    ret = - (3 * 100000 + 4395);
    goto out;
  }
  buf ++;
  len -= (u32)1;
  if (! len) {
    ret = 0;
    goto out;
  }
  /*@ assert rte_68: mem_access: \valid_read(buf + (u32)(len - 1)); */
  c = *(buf + (len - (u32)1));
  tmp_2 = _is_alpha(c);
  if (tmp_2) tmp_4 = 1;
  else {
    int tmp_3;
    tmp_3 = _is_digit(c);
    if (tmp_3) tmp_4 = 1; else tmp_4 = 0;
  }
  ret = tmp_4;
  if (! ret) {
    ret = - (3 * 100000 + 4412);
    goto out;
  }
  buf ++;
  len -= (u32)1;
  i = (u32)0;
  /*@
  loop invariant 0 <= i <= len;
  loop invariant \valid_read(buf + (0..len-1));
  loop assigns i, c, ret;
  loop variant len - i;
  */
  while (i < len) {
    {
      int tmp_7;
      int tmp_5;
      /*@ assert rte_69: mem_access: \valid_read(buf + i); */
      c = *(buf + i);
      tmp_5 = _is_digit(c);
      if (tmp_5) tmp_7 = 1;
      else {
        int tmp_6;
        tmp_6 = _is_alpha(c);
        if (tmp_6) tmp_7 = 1;
        else 
          if ((int)c == '-') tmp_7 = 1; else tmp_7 = 0;
      }
      ret = tmp_7;
      if (! ret) {
        ret = - (3 * 100000 + 4430);
        goto out;
      }
    }
    i ++;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result < 0;
*/
static int parse_rdn_val_pseudo(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3*100000+3917) || \result == -(3*100000+3923) || \result == -(3*100000+3930) || \result == -(3*100000+3937) || \result == -(3*100000+3944) || \result == -(3*100000+2069) || \result == -(3*100000+2083);
ensures (buf == \null || len == 0) ==> \result == -(3*100000+3917);
ensures \result == 0 ==> (buf != \null && len >= 2);
*/
static int parse_rdn_val_ogrn(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_numeric_string(buf,len,(u32)1,(u32)13);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result == -(3*100000+3917) || \result == -(3*100000+3923) || \result == -(3*100000+3930) || \result == -(3*100000+3937) || \result == -(3*100000+3944) || \result == -(3*100000+2069) || \result == -(3*100000+2083);
ensures (buf == \null || len == 0) ==> \result == -(3*100000+3917);
ensures \result == 0 ==> (buf != \null && len >= 2);
*/
static int parse_rdn_val_snils(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_numeric_string(buf,len,(u32)1,(u32)11);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0 .. len - 1));
assigns \nothing;
ensures \result == 0 || \result < 0;
ensures \result == 0 ==> (buf != \null && len >= 2);
*/
static int parse_rdn_val_ogrnip(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_numeric_string(buf,len,(u32)1,(u32)15);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0 .. len-1));
assigns \nothing;
ensures \result == 0 || \result < 0;
*/
static int parse_rdn_val_inn(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_numeric_string(buf,len,(u32)1,(u32)12);
  return tmp;
}

/*@
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0..len-1));
assigns \nothing;
ensures \result == 0 || \result < 0;
*/
static int parse_rdn_val_street_address(u8 const *buf, u32 len)
{
  int tmp;
  tmp = parse_directory_string(buf,len,(u32)1,(u32)64);
  return tmp;
}

/*@
assigns \nothing;
ensures \result == 0;
*/
static int parse_rdn_val_bad_oid(u8 const *buf, u32 len)
{
  int __retres;
  __retres = 0;
  return __retres;
}

/*@
requires \valid_read(known_dn_oids + (0 .. 21));
requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len-1));
requires \forall integer i; 0 <= i < 22 ==> \valid_read(known_dn_oids[i].oid + (0 .. known_dn_oids[i].oid_len - 1));
requires \forall integer i; 0 <= i < 22 ==> \separated(buf + (0 .. len-1), known_dn_oids[i].oid + (0 .. known_dn_oids[i].oid_len - 1));
assigns \nothing;
ensures \result == \null || (\exists integer i; 0 <= i < 22 && \result == &known_dn_oids[i]);
ensures \result != \null ==> \valid_read(\result);
*/
static _name_oid const *find_dn_by_oid(u8 const *buf, u32 len)
{
  u8 k;
  _name_oid const *found = (_name_oid const *)0;
  _name_oid const *cur = (_name_oid const *)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) _LOR: goto out;
  k = (u8)0;
  /*@
  loop invariant 0 <= k <= 22;
  loop invariant \valid_read(known_dn_oids + (0 .. 21));
  loop invariant (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len-1));
  loop invariant \forall integer i; 0 <= i < 22 ==> \valid_read(known_dn_oids[i].oid + (0 .. known_dn_oids[i].oid_len - 1));
  loop invariant \forall integer i; 0 <= i < 22 ==> \separated(buf + (0 .. len-1), known_dn_oids[i].oid + (0 .. known_dn_oids[i].oid_len - 1));
  loop invariant found == \null || (\exists integer i; 0 <= i < k && found == &known_dn_oids[i]);
  loop assigns k, cur, found;
  loop variant 22 - k;
  */
  while ((unsigned long)k < sizeof(known_dn_oids) / sizeof(known_dn_oids[0])) {
    {
      int ret;
      int tmp_0;
      int tmp;
      cur = & known_dn_oids[k];
      /*@ assert rte_37: mem_access: \valid_read(&cur->oid_len); */
      if ((u32)cur->oid_len != len) goto __Cont;
      /*@ assert rte_38: mem_access: \valid_read(&cur->oid); */
      /*@ assert rte_39: mem_access: \valid_read(&cur->oid_len); */
      tmp = bufs_differ(cur->oid,buf,(u32)cur->oid_len);
      if (tmp) tmp_0 = 0; else tmp_0 = 1;
      ret = tmp_0;
      if (ret) {
        found = cur;
        break;
      }
    }
    __Cont:
    /*@ assert rte_40: signed_overflow: (int)k + 1 ≤ 2147483647; */
    k = (u8)((int)k + 1);
  }
  out: ;
  return found;
}

/*@
requires \valid(eaten);
requires (buf != \null) ==> \valid_read(buf + (0 .. len-1));
requires \separated(eaten, buf + (0 .. len-1));
requires \valid_read(known_dn_oids + (0 .. 21));
requires \forall integer i; 0 <= i < 22 ==> \valid_read(known_dn_oids[i].oid + (0 .. known_dn_oids[i].oid_len - 1));
requires buf != \null ==> \forall integer i; 0 <= i < 22 ==> \separated(buf + (0 .. len-1), known_dn_oids[i].oid + (0 .. known_dn_oids[i].oid_len - 1));
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures \result == 0 ==> (*eaten >= 2 && *eaten <= len);
ensures \result == 0 ==> \valid(eaten);
*/
static int parse_AttributeTypeAndValue(u8 const *buf, u32 len, u32 *eaten)
{
  u32 parsed;
  int ret;
  u32 hdr_len = (u32)0;
  u32 data_len = (u32)0;
  u32 oid_len = (u32)0;
  _name_oid const *cur = (_name_oid const *)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 4747);
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)0x10,& hdr_len,& data_len);
  if (ret) goto out;
  parsed = hdr_len + data_len;
  buf += hdr_len;
  len -= hdr_len;
  ret = parse_OID(buf,data_len,& oid_len);
  if (ret) goto out;
  cur = find_dn_by_oid(buf,oid_len);
  if (cur == (_name_oid const *)0) cur = & generic_unsupported_rdn_oid;
  data_len -= oid_len;
  buf += oid_len;
  if (cur == known_dn_oids) ret = parse_rdn_val_cn(buf,data_len);
  else 
    if (cur == & known_dn_oids[1]) ret = parse_rdn_val_x520name(buf,data_len);
    else 
      if (cur == & known_dn_oids[2]) ret = parse_rdn_val_serial(buf,data_len);
      else 
        if (cur == & known_dn_oids[3]) ret = parse_rdn_val_country(buf,
                                                                   data_len);
        else 
          if (cur == & known_dn_oids[4]) ret = parse_rdn_val_locality
                                         (buf,data_len);
          else 
            if (cur == & known_dn_oids[5]) ret = parse_rdn_val_state(buf,
                                                                    data_len);
            else 
              if (cur == & known_dn_oids[6]) ret = parse_rdn_val_org(buf,
                                                                    data_len);
              else 
                if (cur == & known_dn_oids[7]) ret = parse_rdn_val_org_unit
                                               (buf,data_len);
                else 
                  if (cur == & known_dn_oids[8]) ret = parse_rdn_val_title
                                                 (buf,data_len);
                  else 
                    if (cur == & known_dn_oids[9]) ret = parse_rdn_val_x520name
                                                   (buf,data_len);
                    else 
                      if (cur == & known_dn_oids[10]) ret = parse_rdn_val_emailaddress
                                                      (buf,data_len);
                      else 
                        if (cur == & known_dn_oids[11]) ret = parse_rdn_val_x520name
                                                        (buf,data_len);
                        else 
                          if (cur == & known_dn_oids[12]) ret = parse_rdn_val_x520name
                                                          (buf,data_len);
                          else 
                            if (cur == & known_dn_oids[13]) ret = parse_rdn_val_x520name
                                                            (buf,data_len);
                            else 
                              if (cur == & known_dn_oids[14]) ret = parse_rdn_val_dn_qual
                                                              (buf,data_len);
                              else 
                                if (cur == & known_dn_oids[15]) ret = parse_rdn_val_pseudo
                                                                (buf,
                                                                 data_len);
                                else 
                                  if (cur == & known_dn_oids[16]) ret = parse_rdn_val_dc
                                                                  (buf,
                                                                   data_len);
                                  else 
                                    if (cur == & known_dn_oids[17]) ret = parse_rdn_val_ogrn
                                                                    (buf,
                                                                    data_len);
                                    else 
                                      if (cur == & known_dn_oids[18]) 
                                        ret = parse_rdn_val_snils(buf,
                                                                  data_len);
                                      else 
                                        if (cur == & known_dn_oids[19]) 
                                          ret = parse_rdn_val_ogrnip(buf,
                                                                    data_len);
                                        else 
                                          if (cur == & known_dn_oids[20]) 
                                            ret = parse_rdn_val_inn(buf,
                                                                    data_len);
                                          else 
                                            if (cur == & known_dn_oids[21]) 
                                              ret = parse_rdn_val_street_address
                                              (buf,data_len);
                                            else 
                                              if (cur == & generic_unsupported_rdn_oid) 
                                                ret = parse_rdn_val_bad_oid
                                                (buf,data_len);
  if (ret) goto out;
  /*@ assert rte_72: mem_access: \valid(eaten); */
  *eaten = parsed;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(eaten);
requires (buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires (buf != \null && len > 0) ==> \separated(eaten, buf + (0 .. len - 1));
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result < 0;
ensures \result == 0 ==> (*eaten >= 2 && *eaten <= len);
*/
static int parse_RelativeDistinguishedName(u8 const *buf, u32 len, u32 *eaten)
{
  u32 rdn_remain;
  u32 saved_rdn_len;
  int ret;
  u32 hdr_len = (u32)0;
  u32 data_len = (u32)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 4865);
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)0x11,& hdr_len,& data_len);
  if (ret) goto out;
  saved_rdn_len = hdr_len + data_len;
  buf += hdr_len;
  rdn_remain = data_len;
  /*@
  loop invariant rdn_remain <= data_len;
  loop assigns rdn_remain, buf, ret;
  loop variant rdn_remain;
  */
  while (rdn_remain) {
    u32 parsed = (u32)0;
    ret = parse_AttributeTypeAndValue(buf,rdn_remain,& parsed);
    if (ret) goto out;
    rdn_remain -= parsed;
    buf += parsed;
  }
  /*@ assert rte_73: mem_access: \valid(eaten); */
  *eaten = saved_rdn_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires \valid(eaten);
requires \valid(empty);
requires \separated(eaten, empty);
assigns *eaten, *empty;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result == -(3 * 100000 + 5000);
ensures \result == 0 ==> (*eaten >= 2 && *eaten <= len);
ensures \result == 0 ==> (*empty == 0 || *empty == 1);
*/
int parse_x509_Name(u8 const *buf, u32 len, u32 *eaten, int *empty)
{
  int ret;
  u32 name_hdr_len = (u32)0;
  u32 name_data_len = (u32)0;
  u32 remain = (u32)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 5000);
              goto out;
            }
    }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)0x10,& name_hdr_len,
                     & name_data_len);
  if (ret) goto out;
  buf += name_hdr_len;
  remain = name_data_len;
  /*@
  loop invariant 0 <= remain <= name_data_len;
  loop invariant remain > 0 ==> \valid_read(buf + (0 .. remain - 1));
  loop assigns remain, buf, ret;
  loop variant remain;
  */
  while (remain) {
    u32 parsed = (u32)0;
    ret = parse_RelativeDistinguishedName(buf,remain,& parsed);
    if (ret) goto out;
    buf += parsed;
    remain -= parsed;
  }
  /*@ assert rte_74: mem_access: \valid(eaten); */
  *eaten = name_hdr_len + name_data_len;
  /*@ assert rte_75: mem_access: \valid(empty); */
  *empty = ! name_data_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid_read(buf + (0 .. len - 1));
requires \valid(val);
requires \separated(buf + (0 .. len - 1), val);
assigns *val;
ensures \result == 0 || \result < 0;
ensures \result == 0 ==> \valid(val);
ensures (buf == \null || len == 0 || val == \null) ==> \result < 0;
ensures \result < 0 ==> (val == \old(val));
*/
int parse_nine_bit_named_bit_list(u8 const *buf, u32 len, u16 *val)
{
  u8 k;
  u8 non_signif;
  u16 tmp;
  int ret;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) goto _LOR;
    else 
      if (val == (u16 *)0) {
        _LOR: {
                ret = - (3 * 100000 + 2857);
                goto out;
              }
      }
  /*@ assert rte_83: mem_access: \valid_read(buf + 0); */
  if ((int)*(buf + 0) & 0xf8) {
    ret = - (3 * 100000 + 2864);
    goto out;
  }
  switch (len) {
    int tmp_2;
    case (u32)1: ;
    /*@ assert rte_84: mem_access: \valid_read(buf + 0); */
    if ((int)*(buf + 0) != 0) {
      ret = - (3 * 100000 + 2882);
      goto out;
    }
    else {
      ret = - (3 * 100000 + 2894);
      goto out;
    }
    break;
    case (u32)2: ;
    /*@ assert rte_85: mem_access: \valid_read(buf + 1); */
    if ((int)*(buf + 1) == 0) {
      ret = - (3 * 100000 + 2910);
      goto out;
    }
    non_signif = (u8)0;
    k = (u8)0;
    /*@
    loop invariant 0 <= (int)k <= 8;
    loop invariant \valid_read(buf + 1);
    loop assigns k, non_signif;
    loop variant 8 - (int)k;
    */
    while ((int)k < 8) {
      /*@ assert rte_86: mem_access: \valid_read(buf + 1); */
      /*@ assert rte_87: shift: 0 ≤ (int)k < 32; */
      if (((int)*(buf + 1) >> (int)k) & 0x1) {
        non_signif = k;
        break;
      }
      /*@ assert rte_88: signed_overflow: (int)k + 1 ≤ 2147483647; */
      k = (u8)((int)k + 1);
    }
    /*@ assert rte_89: mem_access: \valid_read(buf + 0); */
    if ((int)*(buf + 0) != (int)non_signif) {
      ret = - (3 * 100000 + 2946);
      goto out;
    }
    tmp = (u16)0;
    k = (u8)0;
    /*@
    loop invariant 0 <= (int)k <= 8;
    loop invariant \valid_read(buf + 1);
    loop assigns k, tmp;
    loop variant 8 - (int)k;
    */
    while ((int)k < 8) {
      {
        int tmp_0;
        u8 const mask[8] =
          {(u8)1, (u8)2, (u8)4, (u8)8, (u8)16, (u8)32, (u8)64, (u8)128};
        /*@ assert rte_90: mem_access: \valid_read(buf + 1); */
        /*@ assert rte_91: index_bound: k < 8; */
        if ((int)*(buf + 1) & (int)mask[k]) 
          /*@ assert rte_92: signed_overflow: 7 - (int)k ≤ 2147483647; */
          /*@ assert rte_93: index_bound: 0 ≤ (int)(7 - (int)k); */
          /*@ assert rte_94: index_bound: (int)(7 - (int)k) < 8; */
          tmp_0 = (int)mask[7 - (int)k];
        else tmp_0 = 0;
        tmp = (u16)((int)tmp | tmp_0);
      }
      /*@ assert rte_95: signed_overflow: (int)k + 1 ≤ 2147483647; */
      k = (u8)((int)k + 1);
    }
    /*@ assert rte_96: mem_access: \valid(val); */
    *val = tmp;
    break;
    case (u32)3: ;
    /*@ assert rte_97: mem_access: \valid_read(buf + 0); */
    if ((int)*(buf + 0) != 7) goto _LOR_0;
    else 
      /*@ assert rte_98: mem_access: \valid_read(buf + 2); */
      if ((int)*(buf + 2) != 0x80) {
        _LOR_0: {
                  ret = - (3 * 100000 + 2977);
                  goto out;
                }
      }
    tmp = (u16)0;
    k = (u8)0;
    /*@
    loop invariant 0 <= (int)k <= 8;
    loop invariant \valid_read(buf + 1);
    loop assigns k, tmp;
    loop variant 8 - (int)k;
    */
    while ((int)k < 8) {
      {
        int tmp_1;
        u8 const mask_0[8] =
          {(u8)1, (u8)2, (u8)4, (u8)8, (u8)16, (u8)32, (u8)64, (u8)128};
        /*@ assert rte_99: mem_access: \valid_read(buf + 1); */
        /*@ assert rte_100: index_bound: k < 8; */
        if ((int)*(buf + 1) & (int)mask_0[k]) 
          /*@ assert rte_101: signed_overflow: 7 - (int)k ≤ 2147483647; */
          /*@ assert rte_102: index_bound: 0 ≤ (int)(7 - (int)k); */
          /*@ assert rte_103: index_bound: (int)(7 - (int)k) < 8; */
          tmp_1 = (int)mask_0[7 - (int)k];
        else tmp_1 = 0;
        tmp = (u16)((int)tmp | tmp_1);
      }
      /*@ assert rte_104: signed_overflow: (int)k + 1 ≤ 2147483647; */
      k = (u8)((int)k + 1);
    }
    /*@ assert rte_105: mem_access: \valid_read(buf + 2); */
    if ((int)*(buf + 2) & 0x80) tmp_2 = 0x0100; else tmp_2 = 0x0000;
    tmp = (u16)((int)tmp | tmp_2);
    /*@ assert rte_106: mem_access: \valid(val); */
    *val = tmp;
    break;
    default: ret = - (3 * 100000 + 3003);
    goto out;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires (eaten != \null) ==> \valid(eaten);
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0 || eaten == \null) ==> \result < 0;
ensures \result == 0 ==> \valid(eaten);
ensures \result == 0 ==> (*eaten >= 2 && *eaten <= len);
*/
int parse_crldp_reasons(u8 const *buf, u32 len, u32 exp_type, u32 *eaten)
{
  int ret;
  u32 hdr_len = (u32)0;
  u32 data_len = (u32)0;
  u16 val = (u16)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) goto _LOR;
    else 
      if (eaten == (u32 *)0) {
        _LOR: {
                ret = - (3 * 100000 + 3051);
                goto out;
              }
      }
  ret = parse_id_len(buf,len,CLASS_CONTEXT_SPECIFIC,exp_type,& hdr_len,
                     & data_len);
  if (ret) goto out;
  buf += hdr_len;
  len -= hdr_len;
  ret = parse_nine_bit_named_bit_list(buf,data_len,& val);
  if (ret) goto out;
  /*@ assert rte_107: mem_access: \valid(eaten); */
  *eaten = hdr_len + data_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires \valid(eaten);
requires \valid(empty);
requires \separated(eaten, empty);
requires (buf != \null && len >= 2) ==> \valid_read(buf + (0 .. len - 1));
requires (buf != \null && len >= 2) ==> \separated(buf + (0 .. len - 1), eaten);
requires (buf != \null && len >= 2) ==> \separated(buf + (0 .. len - 1), empty);
assigns *eaten, *empty;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result < 0;
ensures len < 2 ==> \result < 0;
ensures \result == 0 ==> (*eaten >= 2 && *eaten <= len);
ensures \result == 0 ==> \valid(eaten);
ensures \result == 0 ==> \valid(empty);
*/
int parse_GeneralName(u8 const *buf, u32 len, u32 *eaten, int *empty)
{
  u8 name_type;
  int ret;
  u32 remain = (u32)0;
  u32 name_len = (u32)0;
  u32 name_hdr_len = (u32)0;
  u32 grabbed = (u32)0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 2321);
              goto out;
            }
    }
  if (len < (u32)2) {
    ret = - (3 * 100000 + 2327);
    goto out;
  }
  remain = len;
  /*@ assert rte_76: mem_access: \valid_read(buf + 0); */
  name_type = *(buf + 0);
  if (! ((int)name_type & 0x80)) {
    ret = - (3 * 100000 + 2340);
    goto out;
  }
  switch ((int)name_type) {
    case 0x81: case 0x82: case 0x86: buf ++;
    remain -= (u32)1;
    ret = get_length(buf,remain,& name_len,& grabbed);
    if (ret) goto out;
    buf += grabbed;
    remain -= grabbed;
    if (name_len > remain) {
      ret = - (3 * 100000 + 2361);
      goto out;
    }
    ret = check_ia5_string(buf,name_len);
    if (ret) goto out;
    switch ((int)name_type) {
      case 0x81: break;
      case 0x82: break;
      case 0x86: break;
      default: break;
    }
    remain -= name_len;
    buf += name_len;
    /*@ assert rte_77: mem_access: \valid(eaten); */
    *eaten = (name_len + grabbed) + (u32)1;
    /*@ assert rte_78: mem_access: \valid(empty); */
    *empty = ! name_len;
    break;
    case 0x87: buf ++;
    remain -= (u32)1;
    ret = get_length(buf,remain,& name_len,& grabbed);
    if (ret) goto out;
    buf += grabbed;
    remain -= grabbed;
    if (name_len > remain) {
      ret = - (3 * 100000 + 2418);
      goto out;
    }
    remain -= name_len;
    buf += name_len;
    /*@ assert rte_79: mem_access: \valid(eaten); */
    *eaten = (name_len + grabbed) + (u32)1;
    /*@ assert rte_80: mem_access: \valid(empty); */
    *empty = ! name_len;
    break;
    case 0xa0: ret = - (3 * 100000 + 2434);
    goto out;
    break;
    case 0xa3: ret = - (3 * 100000 + 2441);
    goto out;
    break;
    case 0xa4:
    ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)4,
                       & name_hdr_len,& name_len);
    if (ret) goto out;
    buf += name_hdr_len;
    remain = name_len;
    ret = parse_x509_Name(buf,remain,& grabbed,empty);
    if (ret) goto out;
    buf += grabbed;
    remain -= grabbed;
    if (remain) {
      ret = - (3 * 100000 + 2467);
      goto out;
    }
    /*@ assert rte_81: mem_access: \valid(eaten); */
    *eaten = name_hdr_len + name_len;
    break;
    case 0xa5: ret = - (3 * 100000 + 2478);
    goto out;
    break;
    case 0x88: ret = - (3 * 100000 + 2485);
    goto out;
    break;
    default: ret = - (3 * 100000 + 2491);
    goto out;
    break;
  }
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires \valid(eaten);
assigns *eaten;
ensures \result == 0 || \result < 0;
ensures (buf == \null || len == 0) ==> \result < 0;
ensures \result == 0 ==> (*eaten >= 2 && *eaten <= len);
*/
int parse_GeneralNames(u8 const *buf, u32 len, tag_class exp_class,
                       u32 exp_type, u32 *eaten)
{
  u32 remain;
  int ret;
  u32 parsed = (u32)0;
  u32 hdr_len = (u32)0;
  u32 data_len = (u32)0;
  int unused = 0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) {
      _LOR: {
              ret = - (3 * 100000 + 2525);
              goto out;
            }
    }
  ret = parse_id_len(buf,len,exp_class,exp_type,& hdr_len,& data_len);
  if (ret) goto out;
  buf += hdr_len;
  remain = data_len;
  /*@
  loop invariant 0 <= remain <= data_len;
  loop invariant remain > 0 ==> \valid_read(buf + (0 .. remain - 1));
  loop assigns remain, buf, parsed, unused, ret;
  loop variant remain;
  */
  while (remain) {
    ret = parse_GeneralName(buf,remain,& parsed,& unused);
    if (ret) goto out;
    remain -= parsed;
    buf += parsed;
  }
  /*@ assert rte_82: mem_access: \valid(eaten); */
  *eaten = hdr_len + data_len;
  ret = 0;
  out: ;
  return ret;
}

/*@
requires (buf != \null) ==> \valid_read(buf + (0 .. len - 1));
requires (eaten != \null) ==> \valid(eaten);
requires \valid(crldp_has_all_reasons);
requires \separated(crldp_has_all_reasons, eaten);
assigns *eaten, *crldp_has_all_reasons;
ensures \result == 0 || \result < 0;
ensures \result == 0 ==> (*eaten >= 2 && *eaten <= len);
ensures \result == 0 ==> (*crldp_has_all_reasons == 0 || *crldp_has_all_reasons == 1);
*/
int parse_DistributionPoint(u8 const *buf, u32 len,
                            int *crldp_has_all_reasons, u32 *eaten)
{
  int ret;
  u32 hdr_len = (u32)0;
  u32 data_len = (u32)0;
  u32 remain = (u32)0;
  u32 total_len = (u32)0;
  int dp_or_issuer_present = 0;
  u32 parsed = (u32)0;
  int has_all_reasons = 0;
  if (buf == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) goto _LOR;
    else 
      if (eaten == (u32 *)0) {
        _LOR: {
                ret = - (3 * 100000 + 3126);
                goto out;
              }
      }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)0x10,& hdr_len,& data_len);
  if (ret) {
    ret = - (3 * 100000 + 3136);
    goto out;
  }
  total_len = hdr_len + data_len;
  remain = data_len;
  buf += hdr_len;
  ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,& hdr_len,
                     & data_len);
  if (! ret) {
    u8 dpn_type;
    u32 dpn_remain = (u32)0;
    u32 dpn_eaten = (u32)0;
    buf += hdr_len;
    remain -= hdr_len;
    dpn_remain = data_len;
    if (data_len == (u32)0) {
      ret = - (3 * 100000 + 3160);
      goto out;
    }
    /*@ assert rte_108: mem_access: \valid_read(buf + 0); */
    dpn_type = *(buf + 0);
    switch ((int)dpn_type) {
      case 0xa0:
      ret = parse_GeneralNames(buf,dpn_remain,CLASS_CONTEXT_SPECIFIC,(u32)0,
                               & dpn_eaten);
      if (ret) goto out;
      dpn_remain -= dpn_eaten;
      buf += dpn_eaten;
      break;
      case 0xa1: ret = - (3 * 100000 + 3193);
      goto out;
      break;
      default: ret = - (3 * 100000 + 3198);
      goto out;
      break;
    }
    if (dpn_remain) {
      ret = - (3 * 100000 + 3204);
      goto out;
    }
    dp_or_issuer_present |= 1;
    remain -= data_len;
  }
  ret = parse_crldp_reasons(buf,remain,(u32)0x01,& parsed);
  if (! ret) {
    buf += parsed;
    remain -= parsed;
  }
  else has_all_reasons = 1;
  ret = parse_GeneralNames(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)2,& parsed);
  if (! ret) {
    dp_or_issuer_present |= 1;
    buf += parsed;
    remain -= parsed;
  }
  if (remain) {
    ret = - (3 * 100000 + 3240);
    goto out;
  }
  if (! dp_or_issuer_present) {
    ret = - (3 * 100000 + 3251);
    goto out;
  }
  /*@ assert rte_109: mem_access: \valid(eaten); */
  *eaten = total_len;
  /*@ assert rte_110: mem_access: \valid(crldp_has_all_reasons); */
  *crldp_has_all_reasons = has_all_reasons;
  ret = 0;
  out: ;
  return ret;
}

static int parse_ext_CRLDP(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
                           u32 len, int critical)
{
  u32 remain;
  int ret;
  u32 hdr_len = (u32)0;
  u32 data_len = (u32)0;
  u8 const *buf = cert + off;
  if (cert == (u8 const *)0) goto _LOR;
  else 
    if (len == (u32)0) goto _LOR;
    else 
      if (ctx == (cert_parsing_ctx *)0) {
        _LOR: {
                ret = - (2 * 100000 + 5703);
                goto out;
              }
      }
  ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)0x10,& hdr_len,& data_len);
  if (ret) goto out;
  buf += hdr_len;
  remain = data_len;
  if (len != hdr_len + data_len) {
    ret = - (2 * 100000 + 5721);
    goto out;
  }
  /*@ assert rte_111: mem_access: \valid(&ctx->has_crldp); */
  ctx->has_crldp = 1;
  /*@ assert rte_112: mem_access: \valid(&ctx->one_crldp_has_all_reasons); */
  ctx->one_crldp_has_all_reasons = 0;
  while (remain) {
    int crldp_has_all_reasons = 0;
    u32 eaten = (u32)0;
    ret = parse_DistributionPoint(buf,remain,& crldp_has_all_reasons,& eaten);
    if (ret) goto out;
    if (crldp_has_all_reasons) 
      /*@ assert rte_113: mem_access: \valid(&ctx->one_crldp_has_all_reasons); */
      ctx->one_crldp_has_all_reasons = 1;
    remain -= eaten;
    buf += eaten;
  }
  ret = 0;
  out: ;
  return ret;
}

int main(void)
{
  int critical;
  cert_parsing_ctx ctx;
  int tmp_0;
  u32 len =
    Frama_C_unsigned_int_interval((unsigned int)0,(unsigned int)65535);
  u8 buf[65535] = {(u8)0};
  Frama_C_make_unknown((char *)(buf),(size_t)65535);
  u8 *ptr = & buf[(u32)65535 - len];
  Frama_C_make_unknown((char *)(& critical),sizeof(int));
  Frama_C_make_unknown((char *)(& ctx),sizeof(cert_parsing_ctx));
  tmp_0 = parse_ext_CRLDP(& ctx,(u8 const *)ptr,(u32)0,len,critical);
  return tmp_0;
}