#include "errno.h"
#include "stdlib.h"

typedef long time_t;
struct timespec {
   time_t tv_sec ;
   long tv_nsec ;
};
struct timerel {
   struct timespec ts ;
};
struct timeabs {
   struct timespec ts ;
};
struct timemono {
   struct timespec ts ;
};
typedef unsigned long long uint64_t;
_Bool time_greater_(struct timespec a, struct timespec b);
_Bool time_after(struct timeabs a, struct timeabs b);
_Bool time_greater(struct timerel a, struct timerel b);
_Bool time_less_(struct timespec a, struct timespec b);
_Bool time_before(struct timeabs a, struct timeabs b);
_Bool time_less(struct timerel a, struct timerel b);
_Bool timeabs_eq(struct timeabs a, struct timeabs b);
_Bool timemono_eq(struct timemono a, struct timemono b);
_Bool timerel_eq(struct timerel a, struct timerel b);
struct timespec time_sub_(struct timespec recent, struct timespec old);
struct timerel time_sub(struct timerel a, struct timerel b);
struct timerel time_between(struct timeabs recent, struct timeabs old);
struct timerel timemono_between(struct timemono recent, struct timemono old);
struct timeabs timeabs_sub(struct timeabs abs_0, struct timerel rel);
struct timespec time_add_(struct timespec a, struct timespec b);
struct timeabs timeabs_add(struct timeabs a, struct timerel b);
struct timemono timemono_add(struct timemono a, struct timerel b);
struct timerel timerel_add(struct timerel a, struct timerel b);
struct timerel time_divide(struct timerel t, unsigned long div_0);
struct timerel time_multiply(struct timerel t, unsigned long mult);
uint64_t time_to_sec(struct timerel t);
uint64_t time_to_msec(struct timerel t);
uint64_t time_to_usec(struct timerel t);
uint64_t time_to_nsec(struct timerel t);
struct timerel time_from_sec(uint64_t sec);
struct timerel time_from_msec(uint64_t msec);
struct timerel time_from_usec(uint64_t usec);
struct timerel time_from_nsec(uint64_t nsec);

_Bool time_greater_(struct timespec a, struct timespec b)
{
  _Bool __retres;
  if (a.tv_sec > b.tv_sec) {
    __retres = (_Bool)1;
    goto return_label;
  }
  else 
    if (a.tv_sec < b.tv_sec) {
      __retres = (_Bool)0;
      goto return_label;
    }
  __retres = (_Bool)(a.tv_nsec > b.tv_nsec);
  return_label: return __retres;
}

_Bool time_after(struct timeabs a, struct timeabs b)
{
  _Bool tmp;
  tmp = time_greater_(a.ts,b.ts);
  return tmp;
}

_Bool time_greater(struct timerel a, struct timerel b)
{
  _Bool tmp;
  tmp = time_greater_(a.ts,b.ts);
  return tmp;
}

_Bool time_less_(struct timespec a, struct timespec b)
{
  _Bool __retres;
  if (a.tv_sec < b.tv_sec) {
    __retres = (_Bool)1;
    goto return_label;
  }
  else 
    if (a.tv_sec > b.tv_sec) {
      __retres = (_Bool)0;
      goto return_label;
    }
  __retres = (_Bool)(a.tv_nsec < b.tv_nsec);
  return_label: return __retres;
}

_Bool time_before(struct timeabs a, struct timeabs b)
{
  _Bool tmp;
  tmp = time_less_(a.ts,b.ts);
  return tmp;
}

_Bool time_less(struct timerel a, struct timerel b)
{
  _Bool tmp;
  tmp = time_less_(a.ts,b.ts);
  return tmp;
}

_Bool timeabs_eq(struct timeabs a, struct timeabs b)
{
  _Bool __retres;
  int tmp;
  if (a.ts.tv_sec == b.ts.tv_sec) 
    if (a.ts.tv_nsec == b.ts.tv_nsec) tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

_Bool timemono_eq(struct timemono a, struct timemono b)
{
  _Bool __retres;
  int tmp;
  if (a.ts.tv_sec == b.ts.tv_sec) 
    if (a.ts.tv_nsec == b.ts.tv_nsec) tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

_Bool timerel_eq(struct timerel a, struct timerel b)
{
  _Bool __retres;
  int tmp;
  if (a.ts.tv_sec == b.ts.tv_sec) 
    if (a.ts.tv_nsec == b.ts.tv_nsec) tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
requires recent.tv_sec >= 0;
requires old.tv_sec >= 0;
requires 0 <= recent.tv_nsec < 1000000000;
requires 0 <= old.tv_nsec < 1000000000;
assigns \nothing;
ensures \result.tv_sec >= 0;
ensures 0 <= \result.tv_nsec < 1000000000;
*/
struct timespec time_sub_(struct timespec recent, struct timespec old)
{
  struct timespec x;
  struct timespec y;
  struct timespec diff;
  if (recent.tv_sec < old.tv_sec) goto _LOR;
  else 
    if (recent.tv_sec == old.tv_sec) {
      if (recent.tv_nsec < old.tv_nsec) {
        _LOR: {
                x = old;
                y = recent;
              }
      }
      else goto _LAND;
    }
    else {
      _LAND: {
               x = recent;
               y = old;
             }
    }
  x = x;
  y = y;
  /*@ assert
        rte_1: signed_overflow: -9223372036854775808 ≤ x.tv_sec - y.tv_sec;
  */
  /*@ assert
        rte_2: signed_overflow: x.tv_sec - y.tv_sec ≤ 9223372036854775807;
  */
  diff.tv_sec = x.tv_sec - y.tv_sec;
  if (y.tv_nsec > x.tv_nsec) {
    /*@ assert
        rte_3: signed_overflow: -9223372036854775808 ≤ diff.tv_sec - 1;
    */
    (diff.tv_sec) --;
    /*@ assert
        rte_4: signed_overflow: 1000000000 + x.tv_nsec ≤ 9223372036854775807;
    */
    /*@ assert
        rte_5: signed_overflow:
          -9223372036854775808 ≤ (long)(1000000000 + x.tv_nsec) - y.tv_nsec;
    */
    /*@ assert
        rte_6: signed_overflow:
          (long)(1000000000 + x.tv_nsec) - y.tv_nsec ≤ 9223372036854775807;
    */
    diff.tv_nsec = ((long)1000000000 + x.tv_nsec) - y.tv_nsec;
  }
  else 
    /*@ assert
        rte_7: signed_overflow: -9223372036854775808 ≤ x.tv_nsec - y.tv_nsec;
    */
    /*@ assert
        rte_8: signed_overflow: x.tv_nsec - y.tv_nsec ≤ 9223372036854775807;
    */
    diff.tv_nsec = x.tv_nsec - y.tv_nsec;
  return diff;
}

/*@
requires a.ts.tv_sec >= 0;
requires b.ts.tv_sec >= 0;
requires 0 <= a.ts.tv_nsec < 1000000000;
requires 0 <= b.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timerel time_sub(struct timerel a, struct timerel b)
{
  struct timespec x;
  struct timespec y;
  struct timerel t;
  if (a.ts.tv_sec < b.ts.tv_sec) goto _LOR;
  else 
    if (a.ts.tv_sec == b.ts.tv_sec) {
      if (a.ts.tv_nsec < b.ts.tv_nsec) {
        _LOR: {
                x = b.ts;
                y = a.ts;
              }
      }
      else goto _LAND;
    }
    else {
      _LAND: {
               x = a.ts;
               y = b.ts;
             }
    }
  t.ts = time_sub_(x,y);
  return t;
}

/*@
requires recent.ts.tv_sec >= 0;
requires old.ts.tv_sec >= 0;
requires 0 <= recent.ts.tv_nsec < 1000000000;
requires 0 <= old.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timerel time_between(struct timeabs recent, struct timeabs old)
{
  struct timerel t;
  t.ts = time_sub_(recent.ts,old.ts);
  return t;
}

/*@
requires recent.ts.tv_sec >= 0;
requires old.ts.tv_sec >= 0;
requires 0 <= recent.ts.tv_nsec < 1000000000;
requires 0 <= old.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timerel timemono_between(struct timemono recent, struct timemono old)
{
  struct timerel t;
  t.ts = time_sub_(recent.ts,old.ts);
  return t;
}

/*@
requires abs_0.ts.tv_sec >= 0;
requires rel.ts.tv_sec >= 0;
requires 0 <= abs_0.ts.tv_nsec < 1000000000;
requires 0 <= rel.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timeabs timeabs_sub(struct timeabs abs_0, struct timerel rel)
{
  struct timeabs t;
  t.ts = time_sub_(abs_0.ts,rel.ts);
  return t;
}

/*@
requires a.tv_sec >= 0;
requires b.tv_sec >= 0;
requires a.tv_sec + b.tv_sec <= 9223372036854775806;
requires 0 <= a.tv_nsec < 1000000000;
requires 0 <= b.tv_nsec < 1000000000;
assigns \nothing;
ensures \result.tv_sec >= 0;
ensures \result.tv_sec <= a.tv_sec + b.tv_sec + 1;
ensures 0 <= \result.tv_nsec < 1000000000;
*/
struct timespec time_add_(struct timespec a, struct timespec b)
{
  struct timespec sum;
  /*@ assert
        rte_23: signed_overflow: -9223372036854775808 ≤ a.tv_sec + b.tv_sec;
  */
  /*@ assert
        rte_24: signed_overflow: a.tv_sec + b.tv_sec ≤ 9223372036854775807;
  */
  sum.tv_sec = a.tv_sec + b.tv_sec;
  /*@ assert
        rte_25: signed_overflow: -9223372036854775808 ≤ a.tv_nsec + b.tv_nsec;
  */
  /*@ assert
        rte_26: signed_overflow: a.tv_nsec + b.tv_nsec ≤ 9223372036854775807;
  */
  sum.tv_nsec = a.tv_nsec + b.tv_nsec;
  if (sum.tv_nsec >= (long)1000000000) {
    /*@ assert rte_27: signed_overflow: sum.tv_sec + 1 ≤ 9223372036854775807;
    */
    (sum.tv_sec) ++;
    /*@ assert
        rte_28: signed_overflow:
          -9223372036854775808 ≤ sum.tv_nsec - 1000000000;
    */
    sum.tv_nsec -= (long)1000000000;
  }
  return sum;
}

/*@
requires a.ts.tv_sec >= 0;
requires b.ts.tv_sec >= 0;
requires a.ts.tv_sec + b.ts.tv_sec <= 9223372036854775806;
requires 0 <= a.ts.tv_nsec < 1000000000;
requires 0 <= b.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timeabs timeabs_add(struct timeabs a, struct timerel b)
{
  struct timeabs t;
  t.ts = time_add_(a.ts,b.ts);
  return t;
}

/*@
requires a.ts.tv_sec >= 0;
requires b.ts.tv_sec >= 0;
requires a.ts.tv_sec + b.ts.tv_sec <= 9223372036854775806;
requires 0 <= a.ts.tv_nsec < 1000000000;
requires 0 <= b.ts.tv_nsec < 1000000000;
ensures \result.ts.tv_sec >= 0;
ensures \result.ts.tv_sec <= a.ts.tv_sec + b.ts.tv_sec + 1;
ensures 0 <= \result.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timemono timemono_add(struct timemono a, struct timerel b)
{
  struct timemono t;
  t.ts = time_add_(a.ts,b.ts);
  return t;
}

/*@
requires a.ts.tv_sec >= 0;
requires b.ts.tv_sec >= 0;
requires a.ts.tv_sec + b.ts.tv_sec <= 9223372036854775806;
requires 0 <= a.ts.tv_nsec < 1000000000;
requires 0 <= b.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timerel timerel_add(struct timerel a, struct timerel b)
{
  struct timerel t;
  t.ts = time_add_(a.ts,b.ts);
  return t;
}

/*@
requires div_0 > 0;
requires t.ts.tv_sec >= 0;
requires 0 <= t.ts.tv_nsec < 1000000000;
assigns \nothing;
*/
struct timerel time_divide(struct timerel t, unsigned long div_0)
{
  struct timerel res;
  uint64_t rem;
  uint64_t ns;
  /*@ assert rte_9: division_by_zero: div_0 ≢ 0; */
  res.ts.tv_sec = (time_t)((unsigned long)t.ts.tv_sec / div_0);
  /*@ assert rte_10: division_by_zero: div_0 ≢ 0; */
  rem = (uint64_t)((unsigned long)t.ts.tv_sec % div_0);
  if (rem & ~ (((uint64_t)1 << 30) - (uint64_t)1)) {
    /*@ assert
        rte_11: is_nan_or_infinite:
          \is_finite(\mul_double((double)rem, (double)1000000000.0));
    */
    /*@ assert
        rte_12: is_nan_or_infinite:
          \is_finite(\add_double(\mul_double((double)rem,
                                            (double)1000000000.0),
                                (double)t.ts.tv_nsec));
    */
    double nsec = (double)rem * 1000000000.0 + (double)t.ts.tv_nsec;
    /*@ assert
        rte_13: is_nan_or_infinite: \is_finite(\div_double(nsec, (double)div_0));
    */
    /*@ assert rte_14: float_to_int: nsec / (double)div_0 < 9223372036854775808;
    */
    /*@ assert
        rte_15: float_to_int: -9223372036854775809 < nsec / (double)div_0;
    */
    res.ts.tv_nsec = (long)(nsec / (double)div_0);
  }
  else {
    ns = rem * (uint64_t)1000000000 + (uint64_t)t.ts.tv_nsec;
    /*@ assert rte_16: division_by_zero: (uint64_t)div_0 ≢ 0; */
    res.ts.tv_nsec = (long)(ns / (uint64_t)div_0);
  }
  return res;
}

/*@
requires 0 <= t.ts.tv_nsec < 1000000000;
requires t.ts.tv_sec >= 0;
requires mult <= 0xFFFFFFFFFFFFFFFF;
requires (mult & ~((1UL << 30) - 1UL)) != 0 ==> mult <= 0x7FFFFFFFFFFFF;
assigns \nothing;
*/
struct timerel time_multiply(struct timerel t, unsigned long mult)
{
  struct timerel res;
  if (mult & ~ ((1UL << 30) - (unsigned long)1)) {
    /*@ assert
        rte_17: is_nan_or_infinite:
          \is_finite(\mul_double((double)t.ts.tv_nsec, (double)mult));
    */
    double nsec = (double)t.ts.tv_nsec * (double)mult;
    /*@ assert
        rte_18: is_nan_or_infinite:
          \is_finite(\div_double(nsec, (double)1000000000.0));
    */
    res.ts.tv_sec = (time_t)(nsec / 1000000000.0);
    /*@ assert
        rte_19: is_nan_or_infinite:
          \is_finite(\mul_double((double)res.ts.tv_sec, (double)1000000000.0));
    */
    /*@ assert
        rte_20: is_nan_or_infinite:
          \is_finite(\sub_double(nsec,
                                \mul_double((double)res.ts.tv_sec,
                                           (double)1000000000.0)));
    */
    /*@ assert
        rte_21: float_to_int:
          nsec - \mul_double((double)res.ts.tv_sec, (double)1000000000.0) <
          9223372036854775808;
    */
    /*@ assert
        rte_22: float_to_int:
          -9223372036854775809 <
          nsec - \mul_double((double)res.ts.tv_sec, (double)1000000000.0);
    */
    res.ts.tv_nsec = (long)(nsec - (double)res.ts.tv_sec * 1000000000.0);
  }
  else {
    uint64_t nsec_0 = (uint64_t)((unsigned long)t.ts.tv_nsec * mult);
    res.ts.tv_nsec = (long)(nsec_0 % (unsigned long long)1000000000);
    res.ts.tv_sec = (time_t)(nsec_0 / (uint64_t)1000000000);
  }
  res.ts.tv_sec = (time_t)((unsigned long)res.ts.tv_sec + (unsigned long)t.ts.tv_sec * mult);
  return res;
}

uint64_t time_to_sec(struct timerel t)
{
  uint64_t __retres;
  __retres = (uint64_t)t.ts.tv_sec;
  return __retres;
}

uint64_t time_to_msec(struct timerel t)
{
  uint64_t __retres;
  uint64_t sec_part = (uint64_t)t.ts.tv_sec * (uint64_t)1000;
  uint64_t nsec_part = (uint64_t)(t.ts.tv_nsec / (long)1000000);
  __retres = sec_part + nsec_part;
  return __retres;
}

uint64_t time_to_usec(struct timerel t)
{
  uint64_t __retres;
  uint64_t sec_part = (uint64_t)t.ts.tv_sec * (uint64_t)1000000;
  uint64_t nsec_part = (uint64_t)(t.ts.tv_nsec / (long)1000);
  __retres = sec_part + nsec_part;
  return __retres;
}

uint64_t time_to_nsec(struct timerel t)
{
  uint64_t __retres;
  uint64_t sec_part = (uint64_t)t.ts.tv_sec * (uint64_t)1000000000;
  uint64_t nsec_part = (uint64_t)t.ts.tv_nsec;
  __retres = sec_part + nsec_part;
  return __retres;
}

struct timerel time_from_sec(uint64_t sec)
{
  struct timerel t;
  t.ts.tv_nsec = (long)0;
  t.ts.tv_sec = (time_t)sec;
  return t;
}

struct timerel time_from_msec(uint64_t msec)
{
  struct timerel t;
  t.ts.tv_nsec = (long)((msec % (unsigned long long)1000) * (unsigned long long)1000000);
  t.ts.tv_sec = (time_t)(msec / (uint64_t)1000);
  return t;
}

struct timerel time_from_usec(uint64_t usec)
{
  struct timerel t;
  t.ts.tv_nsec = (long)((usec % (unsigned long long)1000000) * (unsigned long long)1000);
  t.ts.tv_sec = (time_t)(usec / (uint64_t)1000000);
  return t;
}

struct timerel time_from_nsec(uint64_t nsec)
{
  struct timerel t;
  t.ts.tv_nsec = (long)(nsec % (unsigned long long)1000000000);
  t.ts.tv_sec = (time_t)(nsec / (uint64_t)1000000000);
  return t;
}