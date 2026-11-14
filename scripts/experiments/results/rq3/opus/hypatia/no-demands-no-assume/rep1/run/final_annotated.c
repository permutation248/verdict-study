#include "errno.h"
#include "math.h"
#include "stdarg.h"
#include "stddef.h"
#include "stdio.h"

struct __anonstruct_2 {
   float x ;
   float y ;
};
union __anonunion_1 {
   float v[2] ;
   struct __anonstruct_2 __anonCompField1 ;
};
struct vector2 {
   union __anonunion_1 __anonCompField2 ;
};
struct __anonstruct_4 {
   float x ;
   float y ;
   float z ;
};
struct __anonstruct_5 {
   float yaw ;
   float pitch ;
   float roll ;
};
union __anonunion_3 {
   float v[3] ;
   struct __anonstruct_4 __anonCompField3 ;
   struct __anonstruct_5 __anonCompField4 ;
};
struct vector3 {
   union __anonunion_3 __anonCompField5 ;
};
struct __anonstruct_7 {
   float x ;
   float y ;
   float z ;
   float w ;
};
union __anonunion_6 {
   float v[4] ;
   struct __anonstruct_7 __anonCompField6 ;
};
struct vector4 {
   union __anonunion_6 __anonCompField7 ;
};
float HYP_MIN(float a, float b);
float HYP_MAX(float a, float b);
void HYP_SWAP(float *a, float *b);
float HYP_SQUARE(float number);
float HYP_ABS(float value);
float HYP_CLAMP(float value, float start, float limit);
static struct vector2 _vector2_zero =
  {.__anonCompField2 = {.v = {0.0f, 0.0f}}};
static struct vector2 _vector2_one =
  {.__anonCompField2 = {.v = {1.0f, 1.0f}}};
static struct vector2 _vector2_unit_x =
  {.__anonCompField2 = {.v = {1.0f, 0.0f}}};
static struct vector2 _vector2_unit_y =
  {.__anonCompField2 = {.v = {0.0f, 1.0f}}};
static struct vector2 _vector2_unit_x_negative =
  {.__anonCompField2 = {.v = {- 1.0f, 0.0f}}};
static struct vector2 _vector2_unit_y_negative =
  {.__anonCompField2 = {.v = {0.0f, - 1.0f}}};
short scalar_equals_epsilonf(float const f1, float const f2,
                             float const epsilon);
short scalar_equalsf(float const f1, float const f2);
struct vector2 const *vector2_get_reference_vector2(int id);
struct vector2 *vector2_set(struct vector2 *self, struct vector2 const *vT);
struct vector2 *vector2_setf2(struct vector2 *self, float xT, float yT);
struct vector2 *vector2_zero(struct vector2 *self);
int vector2_equals(struct vector2 const *self, struct vector2 const *vT);
struct vector2 *vector2_negate(struct vector2 *self);
struct vector2 *vector2_add(struct vector2 *self, struct vector2 const *vT);
struct vector2 *vector2_addf(struct vector2 *self, float fT);
struct vector2 *vector2_subtract(struct vector2 *self,
                                 struct vector2 const *vT);
struct vector2 *vector2_subtractf(struct vector2 *self, float fT);
struct vector2 *vector2_multiply(struct vector2 *self,
                                 struct vector2 const *vT);
struct vector2 *vector2_multiplyf(struct vector2 *self, float fT);
struct vector2 *vector2_divide(struct vector2 *self, struct vector2 const *vT);
struct vector2 *vector2_dividef(struct vector2 *self, float fT);
float vector2_dot_product(struct vector2 const *self,
                          struct vector2 const *vT);
struct vector2 *vector2_cross_product(struct vector2 *vR,
                                      struct vector2 const *vT1,
                                      struct vector2 const *vT2);
static struct vector3 _vector3_zero =
  {.__anonCompField5 = {.v = {0.0f, 0.0f, 0.0f}}};
static struct vector3 _vector3_one =
  {.__anonCompField5 = {.v = {1.0f, 1.0f, 1.0f}}};
static struct vector3 _vector3_unit_x =
  {.__anonCompField5 = {.v = {1.0f, 0.0f, 0.0f}}};
static struct vector3 _vector3_unit_y =
  {.__anonCompField5 = {.v = {0.0f, 1.0f, 0.0f}}};
static struct vector3 _vector3_unit_z =
  {.__anonCompField5 = {.v = {0.0f, 0.0f, 1.0f}}};
static struct vector3 _vector3_unit_x_negative =
  {.__anonCompField5 = {.v = {- 1.0f, 0.0f, 0.0f}}};
static struct vector3 _vector3_unit_y_negative =
  {.__anonCompField5 = {.v = {0.0f, - 1.0f, 0.0f}}};
static struct vector3 _vector3_unit_z_negative =
  {.__anonCompField5 = {.v = {0.0f, 0.0f, - 1.0f}}};
struct vector3 const *vector3_get_reference_vector3(int id);
struct vector3 *vector3_setf3(struct vector3 *self, float xT, float yT,
                              float zT);
struct vector3 *vector3_set(struct vector3 *self, struct vector3 const *vT);
struct vector3 *vector3_zero(struct vector3 *self);
int vector3_equals(struct vector3 const *self, struct vector3 const *vT);
struct vector3 *vector3_negate(struct vector3 *self);
struct vector3 *vector3_add(struct vector3 *self, struct vector3 const *vT);
struct vector3 *vector3_addf(struct vector3 *self, float f);
struct vector3 *vector3_subtract(struct vector3 *self,
                                 struct vector3 const *vT);
struct vector3 *vector3_subtractf(struct vector3 *self, float f);
struct vector3 *vector3_multiply(struct vector3 *self,
                                 struct vector3 const *vT);
struct vector3 *vector3_multiplyf(struct vector3 *self, float f);
struct vector3 *vector3_divide(struct vector3 *self, struct vector3 const *vT);
float vector3_dot_product(struct vector3 const *self,
                          struct vector3 const *vT);
struct vector3 *vector3_cross_product(struct vector3 *vR,
                                      struct vector3 const *vT1,
                                      struct vector3 const *vT2);
static struct vector4 _vector4_zero =
  {.__anonCompField7 = {.v = {0.0f, 0.0f, 0.0f, 0.0f}}};
static struct vector4 _vector4_one =
  {.__anonCompField7 = {.v = {1.0f, 1.0f, 1.0f, 1.0f}}};
static struct vector4 _vector4_unit_x =
  {.__anonCompField7 = {.v = {1.0f, 0.0f, 0.0f, 0.0f}}};
static struct vector4 _vector4_unit_y =
  {.__anonCompField7 = {.v = {0.0f, 1.0f, 0.0f, 0.0f}}};
static struct vector4 _vector4_unit_z =
  {.__anonCompField7 = {.v = {0.0f, 0.0f, 1.0f, 0.0f}}};
static struct vector4 _vector4_unit_x_negative =
  {.__anonCompField7 = {.v = {- 1.0f, 0.0f, 0.0f, 0.0f}}};
static struct vector4 _vector4_unit_y_negative =
  {.__anonCompField7 = {.v = {0.0f, - 1.0f, 0.0f, 0.0f}}};
static struct vector4 _vector4_unit_z_negative =
  {.__anonCompField7 = {.v = {0.0f, 0.0f, - 1.0f, 0.0f}}};
struct vector4 const *vector4_get_reference_vector4(int id);
struct vector4 *vector4_setf4(struct vector4 *self, float xT, float yT,
                              float zT, float wT);
struct vector4 *vector4_set(struct vector4 *self, struct vector4 const *vT);
struct vector4 *vector4_zero(struct vector4 *self);
int vector4_equals(struct vector4 const *self, struct vector4 const *vT);
struct vector4 *vector4_negate(struct vector4 *self);
struct vector4 *vector4_add(struct vector4 *self, struct vector4 const *vT);
struct vector4 *vector4_addf(struct vector4 *self, float f);
struct vector4 *vector4_subtract(struct vector4 *self,
                                 struct vector4 const *vT);
struct vector4 *vector4_subtractf(struct vector4 *self, float f);
struct vector4 *vector4_multiply(struct vector4 *self,
                                 struct vector4 const *vT);
struct vector4 *vector4_multiplyf(struct vector4 *self, float f);
struct vector4 *vector4_divide(struct vector4 *self, struct vector4 const *vT);
float vector4_dot_product(struct vector4 const *self,
                          struct vector4 const *vT);
struct vector4 *vector4_cross_product(struct vector4 *vR,
                                      struct vector4 const *vT1,
                                      struct vector4 const *vT2);
int main(void);

/*@
assigns \nothing;
ensures \result <= a;
ensures \result <= b;
ensures \result == a || \result == b;
*/
float HYP_MIN(float a, float b)
{
  float tmp;
  if (a < b) tmp = a; else tmp = b;
  return tmp;
}

/*@
assigns \nothing;
ensures \result >= a && \result >= b;
ensures \result == a || \result == b;
*/
float HYP_MAX(float a, float b)
{
  float tmp;
  if (a > b) tmp = a; else tmp = b;
  return tmp;
}

/*@
requires \valid(a);
requires \valid(b);
requires \separated(a, b);
assigns *a, *b;
ensures *a == \old(*b);
ensures *b == \old(*a);
*/
void HYP_SWAP(float *a, float *b)
{
  /*@ assert rte_1: mem_access: \valid_read(a); */
  float f = *a;
  /*@ assert rte_2: mem_access: \valid(a); */
  /*@ assert rte_3: mem_access: \valid_read(b); */
  *a = *b;
  /*@ assert rte_4: mem_access: \valid(b); */
  *b = f;
  return;
}

/*@
requires \is_finite(number);
requires \is_finite(\mul_float(number, number));
assigns \nothing;
ensures \is_finite(\result);
ensures \result == \mul_float(number, number);
*/
float HYP_SQUARE(float number)
{
  float __retres;
  /*@ assert rte_5: is_nan_or_infinite: \is_finite(\mul_float(number, number));
  */
  __retres = number * number;
  return __retres;
}

/*@
requires \is_finite(value);
assigns \nothing;
ensures \is_finite(\result);
ensures \result >= 0.0f;
ensures (value >= 0.0f) ==> (\result == value);
ensures (value < 0.0f) ==> (\result == -value);
*/
float HYP_ABS(float value)
{
  float tmp;
  if (value < 0.0f) 
    /*@ assert rte_6: is_nan_or_infinite: \is_finite(value); */
    tmp = - value;
  else tmp = value;
  return tmp;
}

/*@
requires start <= limit;
assigns \nothing;
ensures \result >= start;
ensures \result <= limit;
ensures (value < start) ==> (\result == start);
ensures (value > limit) ==> (\result == limit);
ensures (start <= value <= limit) ==> (\result == value);
*/
float HYP_CLAMP(float value, float start, float limit)
{
  float tmp_0;
  if (value < start) tmp_0 = start;
  else {
    float tmp;
    if (value > limit) tmp = limit; else tmp = value;
    tmp_0 = tmp;
  }
  return tmp_0;
}

/*@
requires \is_finite(f1);
requires \is_finite(f2);
requires \is_finite(epsilon);
requires \is_finite(\sub_float(f1, f2));
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
short scalar_equals_epsilonf(float const f1, float const f2,
                             float const epsilon)
{
  short __retres;
  float tmp;
  /*@ assert rte_7: is_nan_or_infinite: \is_finite(\sub_float(f1, f2)); */
  tmp = HYP_ABS(f1 - f2);
  ;
  if ((tmp < epsilon) == 0) {
    __retres = (short)0;
    goto return_label;
  }
  __retres = (short)1;
  return_label: return __retres;
}

/*@
requires \is_finite(f1);
requires \is_finite(f2);
requires \is_finite(\sub_float(f1, f2));
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
short scalar_equalsf(float const f1, float const f2)
{
  short tmp;
  tmp = scalar_equals_epsilonf(f1,f2,1E-5f);
  return tmp;
}

/*@
assigns \nothing;
ensures \result != \null;
ensures \valid_read(\result);
ensures \result == &_vector2_zero || \result == &_vector2_one || \result == &_vector2_unit_x || \result == &_vector2_unit_y || \result == &_vector2_unit_x_negative || \result == &_vector2_unit_y_negative;
ensures id == 0 ==> \result == &_vector2_zero;
ensures id == 5 ==> \result == &_vector2_one;
ensures id == 1 ==> \result == &_vector2_unit_x;
ensures id == 2 ==> \result == &_vector2_unit_y;
ensures id == 3 ==> \result == &_vector2_unit_x_negative;
ensures id == 4 ==> \result == &_vector2_unit_y_negative;
ensures (id != 0 && id != 1 && id != 2 && id != 3 && id != 4 && id != 5) ==> \result == &_vector2_zero;
*/
struct vector2 const *vector2_get_reference_vector2(int id)
{
  struct vector2 const *__retres;
  switch (id) {
    case 0: __retres = (struct vector2 const *)(& _vector2_zero);
    goto return_label;
    case 5: __retres = (struct vector2 const *)(& _vector2_one);
    goto return_label;
    case 1: __retres = (struct vector2 const *)(& _vector2_unit_x);
    goto return_label;
    case 2: __retres = (struct vector2 const *)(& _vector2_unit_y);
    goto return_label;
    case 3: __retres = (struct vector2 const *)(& _vector2_unit_x_negative);
    goto return_label;
    case 4: __retres = (struct vector2 const *)(& _vector2_unit_y_negative);
    goto return_label;
    default: __retres = (struct vector2 const *)(& _vector2_zero);
    goto return_label;
  }
  return_label: return __retres;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
assigns self->__anonCompField2.__anonCompField1.x, self->__anonCompField2.__anonCompField1.y;
ensures \result == self;
ensures self->__anonCompField2.__anonCompField1.x == vT->__anonCompField2.__anonCompField1.x;
ensures self->__anonCompField2.__anonCompField1.y == vT->__anonCompField2.__anonCompField1.y;
*/
struct vector2 *vector2_set(struct vector2 *self, struct vector2 const *vT)
{
  /*@ assert
        rte_10: mem_access: \valid(&self->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_11: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.x);
  */
  self->__anonCompField2.__anonCompField1.x = vT->__anonCompField2.__anonCompField1.x;
  /*@ assert
        rte_12: mem_access: \valid(&self->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_13: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.y);
  */
  self->__anonCompField2.__anonCompField1.y = vT->__anonCompField2.__anonCompField1.y;
  return self;
}

/*@
requires \valid(self);
assigns self->__anonCompField2.__anonCompField1.x, self->__anonCompField2.__anonCompField1.y;
ensures \result == self;
ensures self->__anonCompField2.__anonCompField1.x == xT;
ensures self->__anonCompField2.__anonCompField1.y == yT;
*/
struct vector2 *vector2_setf2(struct vector2 *self, float xT, float yT)
{
  /*@ assert
        rte_8: mem_access: \valid(&self->__anonCompField2.__anonCompField1.x);
  */
  self->__anonCompField2.__anonCompField1.x = xT;
  /*@ assert
        rte_9: mem_access: \valid(&self->__anonCompField2.__anonCompField1.y);
  */
  self->__anonCompField2.__anonCompField1.y = yT;
  return self;
}

/*@
requires \valid(self);
assigns self->__anonCompField2.__anonCompField1.x, self->__anonCompField2.__anonCompField1.y;
ensures \result == self;
ensures self->__anonCompField2.__anonCompField1.x == 0.0f;
ensures self->__anonCompField2.__anonCompField1.y == 0.0f;
*/
struct vector2 *vector2_zero(struct vector2 *self)
{
  struct vector2 *tmp;
  tmp = vector2_setf2(self,0.0f,0.0f);
  return tmp;
}

/*@
requires \valid_read(self);
requires \valid_read(vT);
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
int vector2_equals(struct vector2 const *self, struct vector2 const *vT)
{
  int tmp_1;
  short tmp;
  /*@ assert
        rte_14: mem_access:
        \valid_read(&self->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_15: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.x);
  */
  tmp = scalar_equalsf(self->__anonCompField2.__anonCompField1.x,
                       vT->__anonCompField2.__anonCompField1.x);
  if (tmp) {
    short tmp_0;
    /*@ assert
        rte_16: mem_access:
          \valid_read(&self->__anonCompField2.__anonCompField1.y);
    */
    /*@ assert
        rte_17: mem_access:
          \valid_read(&vT->__anonCompField2.__anonCompField1.y);
    */
    tmp_0 = scalar_equalsf(self->__anonCompField2.__anonCompField1.y,
                           vT->__anonCompField2.__anonCompField1.y);
    if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
  }
  else tmp_1 = 0;
  return tmp_1;
}

/*@
requires \valid(self);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures self->__anonCompField2.v[0] == -\old(self->__anonCompField2.v[0]);
ensures self->__anonCompField2.v[1] == -\old(self->__anonCompField2.v[1]);
ensures \is_finite(self->__anonCompField2.v[0]);
ensures \is_finite(self->__anonCompField2.v[1]);
*/
struct vector2 *vector2_negate(struct vector2 *self)
{
  /*@ assert rte_18: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_19: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert
        rte_20: is_nan_or_infinite: \is_finite(self->__anonCompField2.v[0]);
  */
  self->__anonCompField2.v[0] = - self->__anonCompField2.v[0];
  /*@ assert rte_21: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_22: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert
        rte_23: is_nan_or_infinite: \is_finite(self->__anonCompField2.v[1]);
  */
  self->__anonCompField2.v[1] = - self->__anonCompField2.v[1];
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
requires \is_finite(vT->__anonCompField2.v[0]);
requires \is_finite(vT->__anonCompField2.v[1]);
requires \is_finite(\add_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0]));
requires \is_finite(\add_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1]));
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures \is_finite(self->__anonCompField2.v[0]);
ensures \is_finite(self->__anonCompField2.v[1]);
ensures self->__anonCompField2.v[0] == \add_float(\old(self->__anonCompField2.v[0]), \old(vT->__anonCompField2.v[0]));
ensures self->__anonCompField2.v[1] == \add_float(\old(self->__anonCompField2.v[1]), \old(vT->__anonCompField2.v[1]));
*/
struct vector2 *vector2_add(struct vector2 *self, struct vector2 const *vT)
{
  /*@ assert rte_24: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_25: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_26: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert
        rte_27: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField2.v[0],
                             vT->__anonCompField2.v[0]));
  */
  self->__anonCompField2.v[0] += vT->__anonCompField2.v[0];
  /*@ assert rte_28: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_29: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_30: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert
        rte_31: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField2.v[1],
                             vT->__anonCompField2.v[1]));
  */
  self->__anonCompField2.v[1] += vT->__anonCompField2.v[1];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
requires \is_finite(fT);
requires \is_finite(\add_float(self->__anonCompField2.v[0], fT));
requires \is_finite(\add_float(self->__anonCompField2.v[1], fT));
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures \is_finite(self->__anonCompField2.v[0]);
ensures \is_finite(self->__anonCompField2.v[1]);
ensures self->__anonCompField2.v[0] == \add_float(\old(self->__anonCompField2.v[0]), fT);
ensures self->__anonCompField2.v[1] == \add_float(\old(self->__anonCompField2.v[1]), fT);
*/
struct vector2 *vector2_addf(struct vector2 *self, float fT)
{
  /*@ assert rte_32: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_33: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert
        rte_34: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField2.v[0], fT));
  */
  self->__anonCompField2.v[0] += fT;
  /*@ assert rte_35: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_36: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert
        rte_37: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField2.v[1], fT));
  */
  self->__anonCompField2.v[1] += fT;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
requires \is_finite(vT->__anonCompField2.v[0]);
requires \is_finite(vT->__anonCompField2.v[1]);
requires \is_finite(\sub_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0]));
requires \is_finite(\sub_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1]));
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures \is_finite(self->__anonCompField2.v[0]);
ensures \is_finite(self->__anonCompField2.v[1]);
ensures self->__anonCompField2.v[0] == \sub_float(\old(self->__anonCompField2.v[0]), \old(vT->__anonCompField2.v[0]));
ensures self->__anonCompField2.v[1] == \sub_float(\old(self->__anonCompField2.v[1]), \old(vT->__anonCompField2.v[1]));
*/
struct vector2 *vector2_subtract(struct vector2 *self,
                                 struct vector2 const *vT)
{
  /*@ assert rte_38: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_39: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_40: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert
        rte_41: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField2.v[0],
                             vT->__anonCompField2.v[0]));
  */
  self->__anonCompField2.v[0] -= vT->__anonCompField2.v[0];
  /*@ assert rte_42: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_43: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_44: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert
        rte_45: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField2.v[1],
                             vT->__anonCompField2.v[1]));
  */
  self->__anonCompField2.v[1] -= vT->__anonCompField2.v[1];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(fT);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures \is_finite(self->__anonCompField2.v[0]);
ensures \is_finite(self->__anonCompField2.v[1]);
*/
struct vector2 *vector2_subtractf(struct vector2 *self, float fT)
{
  /*@ assert rte_46: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_47: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert
        rte_48: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField2.v[0], fT));
  */
  self->__anonCompField2.v[0] -= fT;
  /*@ assert rte_49: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_50: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert
        rte_51: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField2.v[1], fT));
  */
  self->__anonCompField2.v[1] -= fT;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
requires \is_finite(vT->__anonCompField2.v[0]);
requires \is_finite(vT->__anonCompField2.v[1]);
requires \is_finite(\mul_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0]));
requires \is_finite(\mul_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1]));
assigns self->__anonCompField2.v[0..1];
ensures \result == self;
ensures \is_finite(\result->__anonCompField2.v[0]);
ensures \is_finite(\result->__anonCompField2.v[1]);
*/
struct vector2 *vector2_multiply(struct vector2 *self,
                                 struct vector2 const *vT)
{
  /*@ assert rte_52: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_53: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_54: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert
        rte_55: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField2.v[0],
                             vT->__anonCompField2.v[0]));
  */
  self->__anonCompField2.v[0] *= vT->__anonCompField2.v[0];
  /*@ assert rte_56: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_57: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_58: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert
        rte_59: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField2.v[1],
                             vT->__anonCompField2.v[1]));
  */
  self->__anonCompField2.v[1] *= vT->__anonCompField2.v[1];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
requires \is_finite(fT);
requires \is_finite(\mul_float(self->__anonCompField2.v[0], fT));
requires \is_finite(\mul_float(self->__anonCompField2.v[1], fT));
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures \is_finite(self->__anonCompField2.v[0]);
ensures \is_finite(self->__anonCompField2.v[1]);
*/
struct vector2 *vector2_multiplyf(struct vector2 *self, float fT)
{
  /*@ assert rte_60: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_61: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert
        rte_62: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField2.v[0], fT));
  */
  self->__anonCompField2.v[0] *= fT;
  /*@ assert rte_63: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_64: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert
        rte_65: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField2.v[1], fT));
  */
  self->__anonCompField2.v[1] *= fT;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
requires \is_finite(vT->__anonCompField2.v[0]);
requires \is_finite(vT->__anonCompField2.v[1]);
requires vT->__anonCompField2.v[0] != 0.0f;
requires vT->__anonCompField2.v[1] != 0.0f;
requires \is_finite(\div_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0]));
requires \is_finite(\div_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1]));
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures \valid(\result);
ensures \is_finite(\result->__anonCompField2.v[0]);
ensures \is_finite(\result->__anonCompField2.v[1]);
*/
struct vector2 *vector2_divide(struct vector2 *self, struct vector2 const *vT)
{
  /*@ assert rte_66: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_67: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_68: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert
        rte_69: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField2.v[0],
                             vT->__anonCompField2.v[0]));
  */
  self->__anonCompField2.v[0] /= vT->__anonCompField2.v[0];
  /*@ assert rte_70: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_71: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_72: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert
        rte_73: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField2.v[1],
                             vT->__anonCompField2.v[1]));
  */
  self->__anonCompField2.v[1] /= vT->__anonCompField2.v[1];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(self->__anonCompField2.v[0]);
requires \is_finite(self->__anonCompField2.v[1]);
requires \is_finite(fT);
requires fT != 0.0f;
assigns self->__anonCompField2.v[0], self->__anonCompField2.v[1];
ensures \result == self;
ensures \is_finite(self->__anonCompField2.v[0]);
ensures \is_finite(self->__anonCompField2.v[1]);
*/
struct vector2 *vector2_dividef(struct vector2 *self, float fT)
{
  /*@ assert rte_74: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_75: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert
        rte_76: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField2.v[0], fT));
  */
  self->__anonCompField2.v[0] /= fT;
  /*@ assert rte_77: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_78: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert
        rte_79: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField2.v[1], fT));
  */
  self->__anonCompField2.v[1] /= fT;
  return self;
}

/*@
requires \valid_read(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField2.__anonCompField1.x);
requires \is_finite(self->__anonCompField2.__anonCompField1.y);
requires \is_finite(vT->__anonCompField2.__anonCompField1.x);
requires \is_finite(vT->__anonCompField2.__anonCompField1.y);
requires \is_finite(\mul_float(self->__anonCompField2.__anonCompField1.x, vT->__anonCompField2.__anonCompField1.x));
requires \is_finite(\mul_float(self->__anonCompField2.__anonCompField1.y, vT->__anonCompField2.__anonCompField1.y));
requires \is_finite(\add_float(\mul_float(self->__anonCompField2.__anonCompField1.x, vT->__anonCompField2.__anonCompField1.x), \mul_float(self->__anonCompField2.__anonCompField1.y, vT->__anonCompField2.__anonCompField1.y)));
assigns \nothing;
ensures \is_finite(\result);
ensures \result == \add_float(\mul_float(self->__anonCompField2.__anonCompField1.x, vT->__anonCompField2.__anonCompField1.x), \mul_float(self->__anonCompField2.__anonCompField1.y, vT->__anonCompField2.__anonCompField1.y));
*/
float vector2_dot_product(struct vector2 const *self,
                          struct vector2 const *vT)
{
  float __retres;
  /*@ assert
        rte_80: mem_access:
        \valid_read(&self->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_81: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_82: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField2.__anonCompField1.x,
                             vT->__anonCompField2.__anonCompField1.x));
  */
  /*@ assert
        rte_83: mem_access:
        \valid_read(&self->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_84: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_85: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField2.__anonCompField1.y,
                             vT->__anonCompField2.__anonCompField1.y));
  */
  /*@ assert
        rte_86: is_nan_or_infinite:
        \is_finite(\add_float(\mul_float(self->__anonCompField2.__anonCompField1.x,
                                        vT->__anonCompField2.__anonCompField1.x),
                             \mul_float(self->__anonCompField2.__anonCompField1.y,
                                       vT->__anonCompField2.__anonCompField1.y)));
  */
  __retres = self->__anonCompField2.__anonCompField1.x * vT->__anonCompField2.__anonCompField1.x + 
             self->__anonCompField2.__anonCompField1.y * vT->__anonCompField2.__anonCompField1.y;
  return __retres;
}

/*@
requires \valid(vR);
requires \valid_read(vT1);
requires \valid_read(vT2);
requires \is_finite(vT1->__anonCompField2.__anonCompField1.x);
requires \is_finite(vT1->__anonCompField2.__anonCompField1.y);
requires \is_finite(vT2->__anonCompField2.__anonCompField1.x);
requires \is_finite(vT2->__anonCompField2.__anonCompField1.y);
requires \separated(vR, vT1);
requires \separated(vR, vT2);
assigns *vR;
ensures \result == vR;
*/
struct vector2 *vector2_cross_product(struct vector2 *vR,
                                      struct vector2 const *vT1,
                                      struct vector2 const *vT2)
{
  /*@ assert
        rte_87: mem_access: \valid(&vR->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_88: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_89: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_90: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.x,
                             vT2->__anonCompField2.__anonCompField1.y));
  */
  /*@ assert
        rte_91: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_92: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_93: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.y,
                             vT2->__anonCompField2.__anonCompField1.x));
  */
  /*@ assert
        rte_94: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField2.__anonCompField1.x,
                                        vT2->__anonCompField2.__anonCompField1.y),
                             \mul_float(vT1->__anonCompField2.__anonCompField1.y,
                                       vT2->__anonCompField2.__anonCompField1.x)));
  */
  vR->__anonCompField2.__anonCompField1.x = vT1->__anonCompField2.__anonCompField1.x * vT2->__anonCompField2.__anonCompField1.y - 
                                            vT1->__anonCompField2.__anonCompField1.y * vT2->__anonCompField2.__anonCompField1.x;
  /*@ assert
        rte_95: mem_access: \valid(&vR->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_96: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_97: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_98: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.y,
                             vT2->__anonCompField2.__anonCompField1.x));
  */
  /*@ assert
        rte_99: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.x);
  */
  /*@ assert
        rte_100: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.y);
  */
  /*@ assert
        rte_101: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.x,
                             vT2->__anonCompField2.__anonCompField1.y));
  */
  /*@ assert
        rte_102: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField2.__anonCompField1.y,
                                        vT2->__anonCompField2.__anonCompField1.x),
                             \mul_float(vT1->__anonCompField2.__anonCompField1.x,
                                       vT2->__anonCompField2.__anonCompField1.y)));
  */
  vR->__anonCompField2.__anonCompField1.y = vT1->__anonCompField2.__anonCompField1.y * vT2->__anonCompField2.__anonCompField1.x - 
                                            vT1->__anonCompField2.__anonCompField1.x * vT2->__anonCompField2.__anonCompField1.y;
  return vR;
}

/*@
assigns \nothing;
ensures \result != \null;
ensures \valid_read(\result);
ensures \result == &_vector3_zero || \result == &_vector3_one || \result == &_vector3_unit_x || \result == &_vector3_unit_y || \result == &_vector3_unit_z || \result == &_vector3_unit_x_negative || \result == &_vector3_unit_y_negative || \result == &_vector3_unit_z_negative;
ensures (id == 0) ==> (\result == &_vector3_zero);
ensures (id == 7) ==> (\result == &_vector3_one);
ensures (id == 1) ==> (\result == &_vector3_unit_x);
ensures (id == 2) ==> (\result == &_vector3_unit_y);
ensures (id == 3) ==> (\result == &_vector3_unit_z);
ensures (id == 4) ==> (\result == &_vector3_unit_x_negative);
ensures (id == 5) ==> (\result == &_vector3_unit_y_negative);
ensures (id == 6) ==> (\result == &_vector3_unit_z_negative);
ensures (id != 0 && id != 1 && id != 2 && id != 3 && id != 4 && id != 5 && id != 6 && id != 7) ==> (\result == &_vector3_zero);
*/
struct vector3 const *vector3_get_reference_vector3(int id)
{
  struct vector3 const *__retres;
  switch (id) {
    case 0: __retres = (struct vector3 const *)(& _vector3_zero);
    goto return_label;
    case 7: __retres = (struct vector3 const *)(& _vector3_one);
    goto return_label;
    case 1: __retres = (struct vector3 const *)(& _vector3_unit_x);
    goto return_label;
    case 2: __retres = (struct vector3 const *)(& _vector3_unit_y);
    goto return_label;
    case 3: __retres = (struct vector3 const *)(& _vector3_unit_z);
    goto return_label;
    case 4: __retres = (struct vector3 const *)(& _vector3_unit_x_negative);
    goto return_label;
    case 5: __retres = (struct vector3 const *)(& _vector3_unit_y_negative);
    goto return_label;
    case 6: __retres = (struct vector3 const *)(& _vector3_unit_z_negative);
    goto return_label;
    default: __retres = (struct vector3 const *)(& _vector3_zero);
    goto return_label;
  }
  return_label: return __retres;
}

/*@
requires \valid(self);
assigns self->__anonCompField5.__anonCompField3.x, self->__anonCompField5.__anonCompField3.y, self->__anonCompField5.__anonCompField3.z;
ensures \result == self;
ensures self->__anonCompField5.__anonCompField3.x == xT;
ensures self->__anonCompField5.__anonCompField3.y == yT;
ensures self->__anonCompField5.__anonCompField3.z == zT;
*/
struct vector3 *vector3_setf3(struct vector3 *self, float xT, float yT,
                              float zT)
{
  /*@ assert
        rte_103: mem_access: \valid(&self->__anonCompField5.__anonCompField3.x);
  */
  self->__anonCompField5.__anonCompField3.x = xT;
  /*@ assert
        rte_104: mem_access: \valid(&self->__anonCompField5.__anonCompField3.y);
  */
  self->__anonCompField5.__anonCompField3.y = yT;
  /*@ assert
        rte_105: mem_access: \valid(&self->__anonCompField5.__anonCompField3.z);
  */
  self->__anonCompField5.__anonCompField3.z = zT;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
assigns *self;
ensures \result == self;
ensures self->__anonCompField5.__anonCompField3.x == vT->__anonCompField5.__anonCompField3.x;
ensures self->__anonCompField5.__anonCompField3.y == vT->__anonCompField5.__anonCompField3.y;
ensures self->__anonCompField5.__anonCompField3.z == vT->__anonCompField5.__anonCompField3.z;
*/
struct vector3 *vector3_set(struct vector3 *self, struct vector3 const *vT)
{
  /*@ assert
        rte_106: mem_access: \valid(&self->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_107: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.x);
  */
  self->__anonCompField5.__anonCompField3.x = vT->__anonCompField5.__anonCompField3.x;
  /*@ assert
        rte_108: mem_access: \valid(&self->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_109: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.y);
  */
  self->__anonCompField5.__anonCompField3.y = vT->__anonCompField5.__anonCompField3.y;
  /*@ assert
        rte_110: mem_access: \valid(&self->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_111: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.z);
  */
  self->__anonCompField5.__anonCompField3.z = vT->__anonCompField5.__anonCompField3.z;
  return self;
}

/*@
requires \valid(self);
assigns self->__anonCompField5.__anonCompField3.x, self->__anonCompField5.__anonCompField3.y, self->__anonCompField5.__anonCompField3.z;
ensures \result == self;
ensures self->__anonCompField5.__anonCompField3.x == 0.0f;
ensures self->__anonCompField5.__anonCompField3.y == 0.0f;
ensures self->__anonCompField5.__anonCompField3.z == 0.0f;
*/
struct vector3 *vector3_zero(struct vector3 *self)
{
  struct vector3 *tmp;
  tmp = vector3_setf3(self,0.0f,0.0f,0.0f);
  return tmp;
}

/*@
requires \valid_read(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField5.__anonCompField3.x);
requires \is_finite(self->__anonCompField5.__anonCompField3.y);
requires \is_finite(self->__anonCompField5.__anonCompField3.z);
requires \is_finite(vT->__anonCompField5.__anonCompField3.x);
requires \is_finite(vT->__anonCompField5.__anonCompField3.y);
requires \is_finite(vT->__anonCompField5.__anonCompField3.z);
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
int vector3_equals(struct vector3 const *self, struct vector3 const *vT)
{
  int tmp_2;
  float tmp;
  /*@ assert
        rte_112: mem_access:
        \valid_read(&self->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_113: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_114: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField5.__anonCompField3.x,
                             vT->__anonCompField5.__anonCompField3.x));
  */
  tmp = HYP_ABS(self->__anonCompField5.__anonCompField3.x - vT->__anonCompField5.__anonCompField3.x);
  if (tmp < 1E-5f) {
    float tmp_0;
    /*@ assert
        rte_115: mem_access:
          \valid_read(&self->__anonCompField5.__anonCompField3.y);
    */
    /*@ assert
        rte_116: mem_access:
          \valid_read(&vT->__anonCompField5.__anonCompField3.y);
    */
    /*@ assert
        rte_117: is_nan_or_infinite:
          \is_finite(\sub_float(self->__anonCompField5.__anonCompField3.y,
                               vT->__anonCompField5.__anonCompField3.y));
    */
    tmp_0 = HYP_ABS(self->__anonCompField5.__anonCompField3.y - vT->__anonCompField5.__anonCompField3.y);
    if (tmp_0 < 1E-5f) {
      float tmp_1;
      /*@ assert
        rte_118: mem_access:
            \valid_read(&self->__anonCompField5.__anonCompField3.z);
      */
      /*@ assert
        rte_119: mem_access:
            \valid_read(&vT->__anonCompField5.__anonCompField3.z);
      */
      /*@ assert
        rte_120: is_nan_or_infinite:
            \is_finite(\sub_float(self->__anonCompField5.__anonCompField3.z,
                                 vT->__anonCompField5.__anonCompField3.z));
      */
      tmp_1 = HYP_ABS(self->__anonCompField5.__anonCompField3.z - vT->__anonCompField5.__anonCompField3.z);
      if (tmp_1 < 1E-5f) tmp_2 = 1; else tmp_2 = 0;
    }
    else tmp_2 = 0;
  }
  else tmp_2 = 0;
  return tmp_2;
}

/*@
requires \valid(self);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
assigns self->__anonCompField5.v[0], self->__anonCompField5.v[1], self->__anonCompField5.v[2];
ensures \result == self;
ensures self->__anonCompField5.v[0] == -\old(self->__anonCompField5.v[0]);
ensures self->__anonCompField5.v[1] == -\old(self->__anonCompField5.v[1]);
ensures self->__anonCompField5.v[2] == -\old(self->__anonCompField5.v[2]);
ensures \is_finite(self->__anonCompField5.v[0]);
ensures \is_finite(self->__anonCompField5.v[1]);
ensures \is_finite(self->__anonCompField5.v[2]);
*/
struct vector3 *vector3_negate(struct vector3 *self)
{
  /*@ assert rte_121: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_122: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert
        rte_123: is_nan_or_infinite: \is_finite(self->__anonCompField5.v[0]);
  */
  self->__anonCompField5.v[0] = - self->__anonCompField5.v[0];
  /*@ assert rte_124: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_125: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert
        rte_126: is_nan_or_infinite: \is_finite(self->__anonCompField5.v[1]);
  */
  self->__anonCompField5.v[1] = - self->__anonCompField5.v[1];
  /*@ assert rte_127: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_128: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert
        rte_129: is_nan_or_infinite: \is_finite(self->__anonCompField5.v[2]);
  */
  self->__anonCompField5.v[2] = - self->__anonCompField5.v[2];
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
requires \is_finite(vT->__anonCompField5.v[0]);
requires \is_finite(vT->__anonCompField5.v[1]);
requires \is_finite(vT->__anonCompField5.v[2]);
requires \is_finite(\add_float(self->__anonCompField5.v[0], vT->__anonCompField5.v[0]));
requires \is_finite(\add_float(self->__anonCompField5.v[1], vT->__anonCompField5.v[1]));
requires \is_finite(\add_float(self->__anonCompField5.v[2], vT->__anonCompField5.v[2]));
assigns self->__anonCompField5.v[0..2];
ensures \result == self;
ensures \is_finite(self->__anonCompField5.v[0]);
ensures \is_finite(self->__anonCompField5.v[1]);
ensures \is_finite(self->__anonCompField5.v[2]);
*/
struct vector3 *vector3_add(struct vector3 *self, struct vector3 const *vT)
{
  /*@ assert rte_130: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_131: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_132: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert
        rte_133: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField5.v[0],
                             vT->__anonCompField5.v[0]));
  */
  self->__anonCompField5.v[0] += vT->__anonCompField5.v[0];
  /*@ assert rte_134: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_135: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_136: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert
        rte_137: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField5.v[1],
                             vT->__anonCompField5.v[1]));
  */
  self->__anonCompField5.v[1] += vT->__anonCompField5.v[1];
  /*@ assert rte_138: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_139: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_140: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert
        rte_141: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField5.v[2],
                             vT->__anonCompField5.v[2]));
  */
  self->__anonCompField5.v[2] += vT->__anonCompField5.v[2];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(f);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
requires \is_finite(\add_float(self->__anonCompField5.v[0], f));
requires \is_finite(\add_float(self->__anonCompField5.v[1], f));
requires \is_finite(\add_float(self->__anonCompField5.v[2], f));
assigns self->__anonCompField5.v[0], self->__anonCompField5.v[1], self->__anonCompField5.v[2];
ensures \result == self;
ensures \is_finite(self->__anonCompField5.v[0]);
ensures \is_finite(self->__anonCompField5.v[1]);
ensures \is_finite(self->__anonCompField5.v[2]);
ensures self->__anonCompField5.v[0] == \add_float(\old(self->__anonCompField5.v[0]), f);
ensures self->__anonCompField5.v[1] == \add_float(\old(self->__anonCompField5.v[1]), f);
ensures self->__anonCompField5.v[2] == \add_float(\old(self->__anonCompField5.v[2]), f);
*/
struct vector3 *vector3_addf(struct vector3 *self, float f)
{
  /*@ assert rte_142: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_143: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert
        rte_144: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField5.v[0], f));
  */
  self->__anonCompField5.v[0] += f;
  /*@ assert rte_145: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_146: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert
        rte_147: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField5.v[1], f));
  */
  self->__anonCompField5.v[1] += f;
  /*@ assert rte_148: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_149: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert
        rte_150: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField5.v[2], f));
  */
  self->__anonCompField5.v[2] += f;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
requires \is_finite(vT->__anonCompField5.v[0]);
requires \is_finite(vT->__anonCompField5.v[1]);
requires \is_finite(vT->__anonCompField5.v[2]);
requires \is_finite(\sub_float(self->__anonCompField5.v[0], vT->__anonCompField5.v[0]));
requires \is_finite(\sub_float(self->__anonCompField5.v[1], vT->__anonCompField5.v[1]));
requires \is_finite(\sub_float(self->__anonCompField5.v[2], vT->__anonCompField5.v[2]));
assigns self->__anonCompField5.v[0], self->__anonCompField5.v[1], self->__anonCompField5.v[2];
ensures \result == self;
ensures \valid(\result);
ensures \is_finite(\result->__anonCompField5.v[0]);
ensures \is_finite(\result->__anonCompField5.v[1]);
ensures \is_finite(\result->__anonCompField5.v[2]);
*/
struct vector3 *vector3_subtract(struct vector3 *self,
                                 struct vector3 const *vT)
{
  /*@ assert rte_151: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_152: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_153: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert
        rte_154: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField5.v[0],
                             vT->__anonCompField5.v[0]));
  */
  self->__anonCompField5.v[0] -= vT->__anonCompField5.v[0];
  /*@ assert rte_155: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_156: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_157: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert
        rte_158: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField5.v[1],
                             vT->__anonCompField5.v[1]));
  */
  self->__anonCompField5.v[1] -= vT->__anonCompField5.v[1];
  /*@ assert rte_159: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_160: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_161: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert
        rte_162: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField5.v[2],
                             vT->__anonCompField5.v[2]));
  */
  self->__anonCompField5.v[2] -= vT->__anonCompField5.v[2];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(f);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
requires \is_finite(\sub_float(self->__anonCompField5.v[0], f));
requires \is_finite(\sub_float(self->__anonCompField5.v[1], f));
requires \is_finite(\sub_float(self->__anonCompField5.v[2], f));
assigns self->__anonCompField5.v[0], self->__anonCompField5.v[1], self->__anonCompField5.v[2];
ensures \result == self;
ensures \is_finite(self->__anonCompField5.v[0]);
ensures \is_finite(self->__anonCompField5.v[1]);
ensures \is_finite(self->__anonCompField5.v[2]);
*/
struct vector3 *vector3_subtractf(struct vector3 *self, float f)
{
  /*@ assert rte_163: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_164: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert
        rte_165: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField5.v[0], f));
  */
  self->__anonCompField5.v[0] -= f;
  /*@ assert rte_166: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_167: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert
        rte_168: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField5.v[1], f));
  */
  self->__anonCompField5.v[1] -= f;
  /*@ assert rte_169: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_170: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert
        rte_171: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField5.v[2], f));
  */
  self->__anonCompField5.v[2] -= f;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
requires \is_finite(vT->__anonCompField5.v[0]);
requires \is_finite(vT->__anonCompField5.v[1]);
requires \is_finite(vT->__anonCompField5.v[2]);
requires \is_finite(\mul_float(self->__anonCompField5.v[0], vT->__anonCompField5.v[0]));
requires \is_finite(\mul_float(self->__anonCompField5.v[1], vT->__anonCompField5.v[1]));
requires \is_finite(\mul_float(self->__anonCompField5.v[2], vT->__anonCompField5.v[2]));
assigns self->__anonCompField5.v[0..2];
ensures \result == self;
ensures \is_finite(self->__anonCompField5.v[0]);
ensures \is_finite(self->__anonCompField5.v[1]);
ensures \is_finite(self->__anonCompField5.v[2]);
ensures self->__anonCompField5.v[0] == \mul_float(\at(self->__anonCompField5.v[0], Pre), \at(vT->__anonCompField5.v[0], Pre));
ensures self->__anonCompField5.v[1] == \mul_float(\at(self->__anonCompField5.v[1], Pre), \at(vT->__anonCompField5.v[1], Pre));
ensures self->__anonCompField5.v[2] == \mul_float(\at(self->__anonCompField5.v[2], Pre), \at(vT->__anonCompField5.v[2], Pre));
*/
struct vector3 *vector3_multiply(struct vector3 *self,
                                 struct vector3 const *vT)
{
  /*@ assert rte_172: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_173: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_174: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert
        rte_175: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.v[0],
                             vT->__anonCompField5.v[0]));
  */
  self->__anonCompField5.v[0] *= vT->__anonCompField5.v[0];
  /*@ assert rte_176: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_177: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_178: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert
        rte_179: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.v[1],
                             vT->__anonCompField5.v[1]));
  */
  self->__anonCompField5.v[1] *= vT->__anonCompField5.v[1];
  /*@ assert rte_180: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_181: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_182: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert
        rte_183: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.v[2],
                             vT->__anonCompField5.v[2]));
  */
  self->__anonCompField5.v[2] *= vT->__anonCompField5.v[2];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(f);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
requires \is_finite(\mul_float(self->__anonCompField5.v[0], f));
requires \is_finite(\mul_float(self->__anonCompField5.v[1], f));
requires \is_finite(\mul_float(self->__anonCompField5.v[2], f));
assigns self->__anonCompField5.v[0], self->__anonCompField5.v[1], self->__anonCompField5.v[2];
ensures \result == self;
*/
struct vector3 *vector3_multiplyf(struct vector3 *self, float f)
{
  /*@ assert rte_184: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_185: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert
        rte_186: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.v[0], f));
  */
  self->__anonCompField5.v[0] *= f;
  /*@ assert rte_187: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_188: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert
        rte_189: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.v[1], f));
  */
  self->__anonCompField5.v[1] *= f;
  /*@ assert rte_190: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_191: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert
        rte_192: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.v[2], f));
  */
  self->__anonCompField5.v[2] *= f;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField5.v[0]);
requires \is_finite(self->__anonCompField5.v[1]);
requires \is_finite(self->__anonCompField5.v[2]);
requires \is_finite(vT->__anonCompField5.v[0]);
requires \is_finite(vT->__anonCompField5.v[1]);
requires \is_finite(vT->__anonCompField5.v[2]);
requires vT->__anonCompField5.v[0] != 0.0f;
requires vT->__anonCompField5.v[1] != 0.0f;
requires vT->__anonCompField5.v[2] != 0.0f;
requires \is_finite((float)(self->__anonCompField5.v[0] / vT->__anonCompField5.v[0]));
requires \is_finite((float)(self->__anonCompField5.v[1] / vT->__anonCompField5.v[1]));
requires \is_finite((float)(self->__anonCompField5.v[2] / vT->__anonCompField5.v[2]));
assigns self->__anonCompField5.v[0], self->__anonCompField5.v[1], self->__anonCompField5.v[2];
ensures \result == self;
ensures \is_finite(self->__anonCompField5.v[0]);
ensures \is_finite(self->__anonCompField5.v[1]);
ensures \is_finite(self->__anonCompField5.v[2]);
*/
struct vector3 *vector3_divide(struct vector3 *self, struct vector3 const *vT)
{
  /*@ assert rte_193: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_194: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_195: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert
        rte_196: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField5.v[0],
                             vT->__anonCompField5.v[0]));
  */
  self->__anonCompField5.v[0] /= vT->__anonCompField5.v[0];
  /*@ assert rte_197: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_198: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_199: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert
        rte_200: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField5.v[1],
                             vT->__anonCompField5.v[1]));
  */
  self->__anonCompField5.v[1] /= vT->__anonCompField5.v[1];
  /*@ assert rte_201: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_202: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_203: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert
        rte_204: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField5.v[2],
                             vT->__anonCompField5.v[2]));
  */
  self->__anonCompField5.v[2] /= vT->__anonCompField5.v[2];
  return self;
}

/*@
requires \valid_read(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField5.__anonCompField3.x);
requires \is_finite(self->__anonCompField5.__anonCompField3.y);
requires \is_finite(self->__anonCompField5.__anonCompField3.z);
requires \is_finite(vT->__anonCompField5.__anonCompField3.x);
requires \is_finite(vT->__anonCompField5.__anonCompField3.y);
requires \is_finite(vT->__anonCompField5.__anonCompField3.z);
assigns \nothing;
ensures \is_finite(\result);
*/
float vector3_dot_product(struct vector3 const *self,
                          struct vector3 const *vT)
{
  float __retres;
  /*@ assert
        rte_205: mem_access:
        \valid_read(&self->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_206: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_207: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.__anonCompField3.x,
                             vT->__anonCompField5.__anonCompField3.x));
  */
  /*@ assert
        rte_208: mem_access:
        \valid_read(&self->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_209: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_210: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.__anonCompField3.y,
                             vT->__anonCompField5.__anonCompField3.y));
  */
  /*@ assert
        rte_211: is_nan_or_infinite:
        \is_finite(\add_float(\mul_float(self->__anonCompField5.__anonCompField3.x,
                                        vT->__anonCompField5.__anonCompField3.x),
                             \mul_float(self->__anonCompField5.__anonCompField3.y,
                                       vT->__anonCompField5.__anonCompField3.y)));
  */
  /*@ assert
        rte_212: mem_access:
        \valid_read(&self->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_213: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_214: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField5.__anonCompField3.z,
                             vT->__anonCompField5.__anonCompField3.z));
  */
  /*@ assert
        rte_215: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\mul_float(self->__anonCompField5.__anonCompField3.x,
                                                   vT->__anonCompField5.__anonCompField3.x),
                                        \mul_float(self->__anonCompField5.__anonCompField3.y,
                                                  vT->__anonCompField5.__anonCompField3.y)),
                             \mul_float(self->__anonCompField5.__anonCompField3.z,
                                       vT->__anonCompField5.__anonCompField3.z)));
  */
  __retres = (self->__anonCompField5.__anonCompField3.x * vT->__anonCompField5.__anonCompField3.x + 
              self->__anonCompField5.__anonCompField3.y * vT->__anonCompField5.__anonCompField3.y) + 
             self->__anonCompField5.__anonCompField3.z * vT->__anonCompField5.__anonCompField3.z;
  return __retres;
}

/*@
requires \valid(vR);
requires \valid_read(vT1);
requires \valid_read(vT2);
requires \is_finite(vT1->__anonCompField5.__anonCompField3.x);
requires \is_finite(vT1->__anonCompField5.__anonCompField3.y);
requires \is_finite(vT1->__anonCompField5.__anonCompField3.z);
requires \is_finite(vT2->__anonCompField5.__anonCompField3.x);
requires \is_finite(vT2->__anonCompField5.__anonCompField3.y);
requires \is_finite(vT2->__anonCompField5.__anonCompField3.z);
assigns *vR;
ensures \result == vR;
*/
struct vector3 *vector3_cross_product(struct vector3 *vR,
                                      struct vector3 const *vT1,
                                      struct vector3 const *vT2)
{
  /*@ assert
        rte_216: mem_access: \valid(&vR->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_217: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_218: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_219: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.y,
                             vT2->__anonCompField5.__anonCompField3.z));
  */
  /*@ assert
        rte_220: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_221: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_222: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.z,
                             vT2->__anonCompField5.__anonCompField3.y));
  */
  /*@ assert
        rte_223: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField5.__anonCompField3.y,
                                        vT2->__anonCompField5.__anonCompField3.z),
                             \mul_float(vT1->__anonCompField5.__anonCompField3.z,
                                       vT2->__anonCompField5.__anonCompField3.y)));
  */
  vR->__anonCompField5.__anonCompField3.x = vT1->__anonCompField5.__anonCompField3.y * vT2->__anonCompField5.__anonCompField3.z - 
                                            vT1->__anonCompField5.__anonCompField3.z * vT2->__anonCompField5.__anonCompField3.y;
  /*@ assert
        rte_224: mem_access: \valid(&vR->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_225: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_226: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_227: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.z,
                             vT2->__anonCompField5.__anonCompField3.x));
  */
  /*@ assert
        rte_228: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_229: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_230: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.x,
                             vT2->__anonCompField5.__anonCompField3.z));
  */
  /*@ assert
        rte_231: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField5.__anonCompField3.z,
                                        vT2->__anonCompField5.__anonCompField3.x),
                             \mul_float(vT1->__anonCompField5.__anonCompField3.x,
                                       vT2->__anonCompField5.__anonCompField3.z)));
  */
  vR->__anonCompField5.__anonCompField3.y = vT1->__anonCompField5.__anonCompField3.z * vT2->__anonCompField5.__anonCompField3.x - 
                                            vT1->__anonCompField5.__anonCompField3.x * vT2->__anonCompField5.__anonCompField3.z;
  /*@ assert
        rte_232: mem_access: \valid(&vR->__anonCompField5.__anonCompField3.z);
  */
  /*@ assert
        rte_233: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_234: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_235: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.x,
                             vT2->__anonCompField5.__anonCompField3.y));
  */
  /*@ assert
        rte_236: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.y);
  */
  /*@ assert
        rte_237: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.x);
  */
  /*@ assert
        rte_238: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.y,
                             vT2->__anonCompField5.__anonCompField3.x));
  */
  /*@ assert
        rte_239: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField5.__anonCompField3.x,
                                        vT2->__anonCompField5.__anonCompField3.y),
                             \mul_float(vT1->__anonCompField5.__anonCompField3.y,
                                       vT2->__anonCompField5.__anonCompField3.x)));
  */
  vR->__anonCompField5.__anonCompField3.z = vT1->__anonCompField5.__anonCompField3.x * vT2->__anonCompField5.__anonCompField3.y - 
                                            vT1->__anonCompField5.__anonCompField3.y * vT2->__anonCompField5.__anonCompField3.x;
  return vR;
}

/*@
assigns \nothing;
ensures \result != \null;
ensures \valid_read(\result);
*/
struct vector4 const *vector4_get_reference_vector4(int id)
{
  struct vector4 const *__retres;
  switch (id) {
    case 0: __retres = (struct vector4 const *)(& _vector4_zero);
    goto return_label;
    case 7: __retres = (struct vector4 const *)(& _vector4_one);
    goto return_label;
    case 1: __retres = (struct vector4 const *)(& _vector4_unit_x);
    goto return_label;
    case 2: __retres = (struct vector4 const *)(& _vector4_unit_y);
    goto return_label;
    case 3: __retres = (struct vector4 const *)(& _vector4_unit_z);
    goto return_label;
    case 4: __retres = (struct vector4 const *)(& _vector4_unit_x_negative);
    goto return_label;
    case 5: __retres = (struct vector4 const *)(& _vector4_unit_y_negative);
    goto return_label;
    case 6: __retres = (struct vector4 const *)(& _vector4_unit_z_negative);
    goto return_label;
    default: __retres = (struct vector4 const *)(& _vector4_zero);
    goto return_label;
  }
  return_label: return __retres;
}

/*@
requires \valid(self);
assigns self->__anonCompField7.__anonCompField6.x, self->__anonCompField7.__anonCompField6.y, self->__anonCompField7.__anonCompField6.z, self->__anonCompField7.__anonCompField6.w;
ensures \result == self;
ensures self->__anonCompField7.__anonCompField6.x == xT;
ensures self->__anonCompField7.__anonCompField6.y == yT;
ensures self->__anonCompField7.__anonCompField6.z == zT;
ensures self->__anonCompField7.__anonCompField6.w == wT;
*/
struct vector4 *vector4_setf4(struct vector4 *self, float xT, float yT,
                              float zT, float wT)
{
  /*@ assert
        rte_240: mem_access: \valid(&self->__anonCompField7.__anonCompField6.x);
  */
  self->__anonCompField7.__anonCompField6.x = xT;
  /*@ assert
        rte_241: mem_access: \valid(&self->__anonCompField7.__anonCompField6.y);
  */
  self->__anonCompField7.__anonCompField6.y = yT;
  /*@ assert
        rte_242: mem_access: \valid(&self->__anonCompField7.__anonCompField6.z);
  */
  self->__anonCompField7.__anonCompField6.z = zT;
  /*@ assert
        rte_243: mem_access: \valid(&self->__anonCompField7.__anonCompField6.w);
  */
  self->__anonCompField7.__anonCompField6.w = wT;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
assigns *self;
ensures \result == self;
ensures self->__anonCompField7.__anonCompField6.x == vT->__anonCompField7.__anonCompField6.x;
ensures self->__anonCompField7.__anonCompField6.y == vT->__anonCompField7.__anonCompField6.y;
ensures self->__anonCompField7.__anonCompField6.z == vT->__anonCompField7.__anonCompField6.z;
ensures self->__anonCompField7.__anonCompField6.w == vT->__anonCompField7.__anonCompField6.w;
*/
struct vector4 *vector4_set(struct vector4 *self, struct vector4 const *vT)
{
  /*@ assert
        rte_244: mem_access: \valid(&self->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_245: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.x);
  */
  self->__anonCompField7.__anonCompField6.x = vT->__anonCompField7.__anonCompField6.x;
  /*@ assert
        rte_246: mem_access: \valid(&self->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_247: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.y);
  */
  self->__anonCompField7.__anonCompField6.y = vT->__anonCompField7.__anonCompField6.y;
  /*@ assert
        rte_248: mem_access: \valid(&self->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_249: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.z);
  */
  self->__anonCompField7.__anonCompField6.z = vT->__anonCompField7.__anonCompField6.z;
  /*@ assert
        rte_250: mem_access: \valid(&self->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_251: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.w);
  */
  self->__anonCompField7.__anonCompField6.w = vT->__anonCompField7.__anonCompField6.w;
  return self;
}

/*@
requires \valid(self);
assigns self->__anonCompField7.__anonCompField6.x, self->__anonCompField7.__anonCompField6.y, self->__anonCompField7.__anonCompField6.z, self->__anonCompField7.__anonCompField6.w;
ensures \result == self;
*/
struct vector4 *vector4_zero(struct vector4 *self)
{
  struct vector4 *tmp;
  tmp = vector4_setf4(self,0.0f,0.0f,0.0f,0.0f);
  return tmp;
}

/*@
requires \valid_read(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField7.__anonCompField6.x);
requires \is_finite(self->__anonCompField7.__anonCompField6.y);
requires \is_finite(self->__anonCompField7.__anonCompField6.z);
requires \is_finite(self->__anonCompField7.__anonCompField6.w);
requires \is_finite(vT->__anonCompField7.__anonCompField6.x);
requires \is_finite(vT->__anonCompField7.__anonCompField6.y);
requires \is_finite(vT->__anonCompField7.__anonCompField6.z);
requires \is_finite(vT->__anonCompField7.__anonCompField6.w);
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
int vector4_equals(struct vector4 const *self, struct vector4 const *vT)
{
  int tmp_3;
  float tmp;
  /*@ assert
        rte_252: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_253: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_254: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.x,
                             vT->__anonCompField7.__anonCompField6.x));
  */
  tmp = HYP_ABS(self->__anonCompField7.__anonCompField6.x - vT->__anonCompField7.__anonCompField6.x);
  if (tmp < 1E-5f) {
    float tmp_0;
    /*@ assert
        rte_255: mem_access:
          \valid_read(&self->__anonCompField7.__anonCompField6.y);
    */
    /*@ assert
        rte_256: mem_access:
          \valid_read(&vT->__anonCompField7.__anonCompField6.y);
    */
    /*@ assert
        rte_257: is_nan_or_infinite:
          \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.y,
                               vT->__anonCompField7.__anonCompField6.y));
    */
    tmp_0 = HYP_ABS(self->__anonCompField7.__anonCompField6.y - vT->__anonCompField7.__anonCompField6.y);
    if (tmp_0 < 1E-5f) {
      float tmp_1;
      /*@ assert
        rte_258: mem_access:
            \valid_read(&self->__anonCompField7.__anonCompField6.z);
      */
      /*@ assert
        rte_259: mem_access:
            \valid_read(&vT->__anonCompField7.__anonCompField6.z);
      */
      /*@ assert
        rte_260: is_nan_or_infinite:
            \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.z,
                                 vT->__anonCompField7.__anonCompField6.z));
      */
      tmp_1 = HYP_ABS(self->__anonCompField7.__anonCompField6.z - vT->__anonCompField7.__anonCompField6.z);
      if (tmp_1 < 1E-5f) {
        float tmp_2;
        /*@ assert
        rte_261: mem_access:
              \valid_read(&self->__anonCompField7.__anonCompField6.w);
        */
        /*@ assert
        rte_262: mem_access:
              \valid_read(&vT->__anonCompField7.__anonCompField6.w);
        */
        /*@ assert
        rte_263: is_nan_or_infinite:
              \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.w,
                                   vT->__anonCompField7.__anonCompField6.w));
        */
        tmp_2 = HYP_ABS(self->__anonCompField7.__anonCompField6.w - vT->__anonCompField7.__anonCompField6.w);
        if (tmp_2 < 1E-5f) tmp_3 = 1; else tmp_3 = 0;
      }
      else tmp_3 = 0;
    }
    else tmp_3 = 0;
  }
  else tmp_3 = 0;
  return tmp_3;
}

/*@
requires \valid(self);
requires \is_finite(self->__anonCompField7.v[0]);
requires \is_finite(self->__anonCompField7.v[1]);
requires \is_finite(self->__anonCompField7.v[2]);
requires \is_finite(self->__anonCompField7.v[3]);
assigns self->__anonCompField7.v[0..3];
ensures \result == self;
ensures self->__anonCompField7.v[0] == -\old(self->__anonCompField7.v[0]);
ensures self->__anonCompField7.v[1] == -\old(self->__anonCompField7.v[1]);
ensures self->__anonCompField7.v[2] == -\old(self->__anonCompField7.v[2]);
ensures self->__anonCompField7.v[3] == -\old(self->__anonCompField7.v[3]);
ensures \valid(\result);
*/
struct vector4 *vector4_negate(struct vector4 *self)
{
  /*@ assert rte_264: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_265: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert
        rte_266: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[0]);
  */
  self->__anonCompField7.v[0] = - self->__anonCompField7.v[0];
  /*@ assert rte_267: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_268: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert
        rte_269: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[1]);
  */
  self->__anonCompField7.v[1] = - self->__anonCompField7.v[1];
  /*@ assert rte_270: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_271: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert
        rte_272: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[2]);
  */
  self->__anonCompField7.v[2] = - self->__anonCompField7.v[2];
  /*@ assert rte_273: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_274: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert
        rte_275: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[3]);
  */
  self->__anonCompField7.v[3] = - self->__anonCompField7.v[3];
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField7.v[0]);
requires \is_finite(self->__anonCompField7.v[1]);
requires \is_finite(self->__anonCompField7.v[2]);
requires \is_finite(self->__anonCompField7.v[3]);
requires \is_finite(vT->__anonCompField7.v[0]);
requires \is_finite(vT->__anonCompField7.v[1]);
requires \is_finite(vT->__anonCompField7.v[2]);
requires \is_finite(vT->__anonCompField7.v[3]);
requires \is_finite(\add_float(self->__anonCompField7.v[0], vT->__anonCompField7.v[0]));
requires \is_finite(\add_float(self->__anonCompField7.v[1], vT->__anonCompField7.v[1]));
requires \is_finite(\add_float(self->__anonCompField7.v[2], vT->__anonCompField7.v[2]));
requires \is_finite(\add_float(self->__anonCompField7.v[3], vT->__anonCompField7.v[3]));
assigns self->__anonCompField7.v[0..3];
ensures \result == self;
ensures \is_finite(self->__anonCompField7.v[0]);
ensures \is_finite(self->__anonCompField7.v[1]);
ensures \is_finite(self->__anonCompField7.v[2]);
ensures \is_finite(self->__anonCompField7.v[3]);
*/
struct vector4 *vector4_add(struct vector4 *self, struct vector4 const *vT)
{
  /*@ assert rte_276: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_277: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_278: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert
        rte_279: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[0],
                             vT->__anonCompField7.v[0]));
  */
  self->__anonCompField7.v[0] += vT->__anonCompField7.v[0];
  /*@ assert rte_280: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_281: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_282: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert
        rte_283: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[1],
                             vT->__anonCompField7.v[1]));
  */
  self->__anonCompField7.v[1] += vT->__anonCompField7.v[1];
  /*@ assert rte_284: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_285: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_286: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert
        rte_287: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[2],
                             vT->__anonCompField7.v[2]));
  */
  self->__anonCompField7.v[2] += vT->__anonCompField7.v[2];
  /*@ assert rte_288: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_289: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_290: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert
        rte_291: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[3],
                             vT->__anonCompField7.v[3]));
  */
  self->__anonCompField7.v[3] += vT->__anonCompField7.v[3];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(f);
requires \is_finite(self->__anonCompField7.v[0]);
requires \is_finite(self->__anonCompField7.v[1]);
requires \is_finite(self->__anonCompField7.v[2]);
requires \is_finite(self->__anonCompField7.v[3]);
requires \is_finite(\add_float(self->__anonCompField7.v[0], f));
requires \is_finite(\add_float(self->__anonCompField7.v[1], f));
requires \is_finite(\add_float(self->__anonCompField7.v[2], f));
requires \is_finite(\add_float(self->__anonCompField7.v[3], f));
assigns self->__anonCompField7.v[0..3];
ensures \result == self;
ensures \is_finite(self->__anonCompField7.v[0]);
ensures \is_finite(self->__anonCompField7.v[1]);
ensures \is_finite(self->__anonCompField7.v[2]);
ensures \is_finite(self->__anonCompField7.v[3]);
*/
struct vector4 *vector4_addf(struct vector4 *self, float f)
{
  /*@ assert rte_292: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_293: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert
        rte_294: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[0], f));
  */
  self->__anonCompField7.v[0] += f;
  /*@ assert rte_295: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_296: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert
        rte_297: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[1], f));
  */
  self->__anonCompField7.v[1] += f;
  /*@ assert rte_298: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_299: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert
        rte_300: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[2], f));
  */
  self->__anonCompField7.v[2] += f;
  /*@ assert rte_301: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_302: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert
        rte_303: is_nan_or_infinite:
        \is_finite(\add_float(self->__anonCompField7.v[3], f));
  */
  self->__anonCompField7.v[3] += f;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField7.v[0]);
requires \is_finite(self->__anonCompField7.v[1]);
requires \is_finite(self->__anonCompField7.v[2]);
requires \is_finite(self->__anonCompField7.v[3]);
requires \is_finite(vT->__anonCompField7.v[0]);
requires \is_finite(vT->__anonCompField7.v[1]);
requires \is_finite(vT->__anonCompField7.v[2]);
requires \is_finite(vT->__anonCompField7.v[3]);
requires \is_finite(\sub_float(self->__anonCompField7.v[0], vT->__anonCompField7.v[0]));
requires \is_finite(\sub_float(self->__anonCompField7.v[1], vT->__anonCompField7.v[1]));
requires \is_finite(\sub_float(self->__anonCompField7.v[2], vT->__anonCompField7.v[2]));
requires \is_finite(\sub_float(self->__anonCompField7.v[3], vT->__anonCompField7.v[3]));
assigns self->__anonCompField7.v[0..3];
ensures \result == self;
ensures \is_finite(self->__anonCompField7.v[0]);
ensures \is_finite(self->__anonCompField7.v[1]);
ensures \is_finite(self->__anonCompField7.v[2]);
ensures \is_finite(self->__anonCompField7.v[3]);
*/
struct vector4 *vector4_subtract(struct vector4 *self,
                                 struct vector4 const *vT)
{
  /*@ assert rte_304: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_305: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_306: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert
        rte_307: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.v[0],
                             vT->__anonCompField7.v[0]));
  */
  self->__anonCompField7.v[0] -= vT->__anonCompField7.v[0];
  /*@ assert rte_308: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_309: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_310: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert
        rte_311: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.v[1],
                             vT->__anonCompField7.v[1]));
  */
  self->__anonCompField7.v[1] -= vT->__anonCompField7.v[1];
  /*@ assert rte_312: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_313: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_314: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert
        rte_315: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.v[2],
                             vT->__anonCompField7.v[2]));
  */
  self->__anonCompField7.v[2] -= vT->__anonCompField7.v[2];
  /*@ assert rte_316: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_317: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_318: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert
        rte_319: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.v[3],
                             vT->__anonCompField7.v[3]));
  */
  self->__anonCompField7.v[3] -= vT->__anonCompField7.v[3];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(f);
requires \is_finite(self->__anonCompField7.__anonCompField6.x);
requires \is_finite(self->__anonCompField7.__anonCompField6.y);
requires \is_finite(self->__anonCompField7.__anonCompField6.z);
requires \is_finite(self->__anonCompField7.__anonCompField6.w);
requires \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.x, f));
requires \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.y, f));
requires \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.z, f));
requires \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.w, f));
assigns self->__anonCompField7.__anonCompField6.x, self->__anonCompField7.__anonCompField6.y, self->__anonCompField7.__anonCompField6.z, self->__anonCompField7.__anonCompField6.w;
ensures \result == self;
ensures \is_finite(self->__anonCompField7.__anonCompField6.x);
ensures \is_finite(self->__anonCompField7.__anonCompField6.y);
ensures \is_finite(self->__anonCompField7.__anonCompField6.z);
ensures \is_finite(self->__anonCompField7.__anonCompField6.w);
*/
struct vector4 *vector4_subtractf(struct vector4 *self, float f)
{
  /*@ assert
        rte_320: mem_access: \valid(&self->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_321: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_322: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.x, f));
  */
  self->__anonCompField7.__anonCompField6.x -= f;
  /*@ assert
        rte_323: mem_access: \valid(&self->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_324: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_325: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.y, f));
  */
  self->__anonCompField7.__anonCompField6.y -= f;
  /*@ assert
        rte_326: mem_access: \valid(&self->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_327: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_328: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.z, f));
  */
  self->__anonCompField7.__anonCompField6.z -= f;
  /*@ assert
        rte_329: mem_access: \valid(&self->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_330: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_331: is_nan_or_infinite:
        \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.w, f));
  */
  self->__anonCompField7.__anonCompField6.w -= f;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField7.v[0]);
requires \is_finite(self->__anonCompField7.v[1]);
requires \is_finite(self->__anonCompField7.v[2]);
requires \is_finite(self->__anonCompField7.v[3]);
requires \is_finite(vT->__anonCompField7.v[0]);
requires \is_finite(vT->__anonCompField7.v[1]);
requires \is_finite(vT->__anonCompField7.v[2]);
requires \is_finite(vT->__anonCompField7.v[3]);
requires \is_finite(\mul_float(self->__anonCompField7.v[0], vT->__anonCompField7.v[0]));
requires \is_finite(\mul_float(self->__anonCompField7.v[1], vT->__anonCompField7.v[1]));
requires \is_finite(\mul_float(self->__anonCompField7.v[2], vT->__anonCompField7.v[2]));
requires \is_finite(\mul_float(self->__anonCompField7.v[3], vT->__anonCompField7.v[3]));
assigns self->__anonCompField7.v[0..3];
ensures \result == self;
ensures \is_finite(self->__anonCompField7.v[0]);
ensures \is_finite(self->__anonCompField7.v[1]);
ensures \is_finite(self->__anonCompField7.v[2]);
ensures \is_finite(self->__anonCompField7.v[3]);
*/
struct vector4 *vector4_multiply(struct vector4 *self,
                                 struct vector4 const *vT)
{
  /*@ assert rte_332: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_333: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_334: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert
        rte_335: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[0],
                             vT->__anonCompField7.v[0]));
  */
  self->__anonCompField7.v[0] *= vT->__anonCompField7.v[0];
  /*@ assert rte_336: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_337: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_338: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert
        rte_339: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[1],
                             vT->__anonCompField7.v[1]));
  */
  self->__anonCompField7.v[1] *= vT->__anonCompField7.v[1];
  /*@ assert rte_340: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_341: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_342: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert
        rte_343: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[2],
                             vT->__anonCompField7.v[2]));
  */
  self->__anonCompField7.v[2] *= vT->__anonCompField7.v[2];
  /*@ assert rte_344: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_345: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_346: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert
        rte_347: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[3],
                             vT->__anonCompField7.v[3]));
  */
  self->__anonCompField7.v[3] *= vT->__anonCompField7.v[3];
  return self;
}

/*@
requires \valid(self);
requires \is_finite(f);
requires \is_finite(self->__anonCompField7.v[0]);
requires \is_finite(self->__anonCompField7.v[1]);
requires \is_finite(self->__anonCompField7.v[2]);
requires \is_finite(self->__anonCompField7.v[3]);
requires \is_finite(\mul_float(self->__anonCompField7.v[0], f));
requires \is_finite(\mul_float(self->__anonCompField7.v[1], f));
requires \is_finite(\mul_float(self->__anonCompField7.v[2], f));
requires \is_finite(\mul_float(self->__anonCompField7.v[3], f));
assigns self->__anonCompField7.v[0], self->__anonCompField7.v[1], self->__anonCompField7.v[2], self->__anonCompField7.v[3];
ensures \result == self;
ensures \is_finite(self->__anonCompField7.v[0]);
ensures \is_finite(self->__anonCompField7.v[1]);
ensures \is_finite(self->__anonCompField7.v[2]);
ensures \is_finite(self->__anonCompField7.v[3]);
*/
struct vector4 *vector4_multiplyf(struct vector4 *self, float f)
{
  /*@ assert rte_348: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_349: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert
        rte_350: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[0], f));
  */
  self->__anonCompField7.v[0] *= f;
  /*@ assert rte_351: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_352: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert
        rte_353: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[1], f));
  */
  self->__anonCompField7.v[1] *= f;
  /*@ assert rte_354: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_355: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert
        rte_356: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[2], f));
  */
  self->__anonCompField7.v[2] *= f;
  /*@ assert rte_357: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_358: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert
        rte_359: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.v[3], f));
  */
  self->__anonCompField7.v[3] *= f;
  return self;
}

/*@
requires \valid(self);
requires \valid_read(vT);
requires \separated(self, vT);
requires \is_finite(self->__anonCompField7.v[0]);
requires \is_finite(self->__anonCompField7.v[1]);
requires \is_finite(self->__anonCompField7.v[2]);
requires \is_finite(self->__anonCompField7.v[3]);
requires \is_finite(vT->__anonCompField7.v[0]);
requires \is_finite(vT->__anonCompField7.v[1]);
requires \is_finite(vT->__anonCompField7.v[2]);
requires \is_finite(vT->__anonCompField7.v[3]);
requires vT->__anonCompField7.v[0] != 0.0f;
requires vT->__anonCompField7.v[1] != 0.0f;
requires vT->__anonCompField7.v[2] != 0.0f;
requires vT->__anonCompField7.v[3] != 0.0f;
requires \is_finite((float)(self->__anonCompField7.v[0] / vT->__anonCompField7.v[0]));
requires \is_finite((float)(self->__anonCompField7.v[1] / vT->__anonCompField7.v[1]));
requires \is_finite((float)(self->__anonCompField7.v[2] / vT->__anonCompField7.v[2]));
requires \is_finite((float)(self->__anonCompField7.v[3] / vT->__anonCompField7.v[3]));
assigns self->__anonCompField7.v[0..3];
ensures \result == self;
ensures \is_finite(self->__anonCompField7.v[0]);
ensures \is_finite(self->__anonCompField7.v[1]);
ensures \is_finite(self->__anonCompField7.v[2]);
ensures \is_finite(self->__anonCompField7.v[3]);
*/
struct vector4 *vector4_divide(struct vector4 *self, struct vector4 const *vT)
{
  /*@ assert rte_360: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_361: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_362: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert
        rte_363: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField7.v[0],
                             vT->__anonCompField7.v[0]));
  */
  self->__anonCompField7.v[0] /= vT->__anonCompField7.v[0];
  /*@ assert rte_364: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_365: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_366: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert
        rte_367: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField7.v[1],
                             vT->__anonCompField7.v[1]));
  */
  self->__anonCompField7.v[1] /= vT->__anonCompField7.v[1];
  /*@ assert rte_368: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_369: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_370: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert
        rte_371: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField7.v[2],
                             vT->__anonCompField7.v[2]));
  */
  self->__anonCompField7.v[2] /= vT->__anonCompField7.v[2];
  /*@ assert rte_372: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_373: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_374: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert
        rte_375: is_nan_or_infinite:
        \is_finite(\div_float(self->__anonCompField7.v[3],
                             vT->__anonCompField7.v[3]));
  */
  self->__anonCompField7.v[3] /= vT->__anonCompField7.v[3];
  return self;
}

/*@
requires \valid_read(self);
requires \valid_read(vT);
requires \is_finite(self->__anonCompField7.__anonCompField6.x);
requires \is_finite(self->__anonCompField7.__anonCompField6.y);
requires \is_finite(self->__anonCompField7.__anonCompField6.z);
requires \is_finite(self->__anonCompField7.__anonCompField6.w);
requires \is_finite(vT->__anonCompField7.__anonCompField6.x);
requires \is_finite(vT->__anonCompField7.__anonCompField6.y);
requires \is_finite(vT->__anonCompField7.__anonCompField6.z);
requires \is_finite(vT->__anonCompField7.__anonCompField6.w);
requires \is_finite((float)(self->__anonCompField7.__anonCompField6.x * vT->__anonCompField7.__anonCompField6.x));
requires \is_finite((float)(self->__anonCompField7.__anonCompField6.y * vT->__anonCompField7.__anonCompField6.y));
requires \is_finite((float)(self->__anonCompField7.__anonCompField6.z * vT->__anonCompField7.__anonCompField6.z));
requires \is_finite((float)(self->__anonCompField7.__anonCompField6.w * vT->__anonCompField7.__anonCompField6.w));
requires \is_finite((float)(self->__anonCompField7.__anonCompField6.x * vT->__anonCompField7.__anonCompField6.x + self->__anonCompField7.__anonCompField6.y * vT->__anonCompField7.__anonCompField6.y));
requires \is_finite((float)((self->__anonCompField7.__anonCompField6.x * vT->__anonCompField7.__anonCompField6.x + self->__anonCompField7.__anonCompField6.y * vT->__anonCompField7.__anonCompField6.y) + self->__anonCompField7.__anonCompField6.z * vT->__anonCompField7.__anonCompField6.z));
requires \is_finite((float)(((self->__anonCompField7.__anonCompField6.x * vT->__anonCompField7.__anonCompField6.x + self->__anonCompField7.__anonCompField6.y * vT->__anonCompField7.__anonCompField6.y) + self->__anonCompField7.__anonCompField6.z * vT->__anonCompField7.__anonCompField6.z) + self->__anonCompField7.__anonCompField6.w * vT->__anonCompField7.__anonCompField6.w));
assigns \nothing;
ensures \is_finite(\result);
*/
float vector4_dot_product(struct vector4 const *self,
                          struct vector4 const *vT)
{
  float __retres;
  /*@ assert
        rte_376: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_377: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_378: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.x,
                             vT->__anonCompField7.__anonCompField6.x));
  */
  /*@ assert
        rte_379: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_380: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_381: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.y,
                             vT->__anonCompField7.__anonCompField6.y));
  */
  /*@ assert
        rte_382: is_nan_or_infinite:
        \is_finite(\add_float(\mul_float(self->__anonCompField7.__anonCompField6.x,
                                        vT->__anonCompField7.__anonCompField6.x),
                             \mul_float(self->__anonCompField7.__anonCompField6.y,
                                       vT->__anonCompField7.__anonCompField6.y)));
  */
  /*@ assert
        rte_383: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_384: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_385: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.z,
                             vT->__anonCompField7.__anonCompField6.z));
  */
  /*@ assert
        rte_386: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\mul_float(self->__anonCompField7.__anonCompField6.x,
                                                   vT->__anonCompField7.__anonCompField6.x),
                                        \mul_float(self->__anonCompField7.__anonCompField6.y,
                                                  vT->__anonCompField7.__anonCompField6.y)),
                             \mul_float(self->__anonCompField7.__anonCompField6.z,
                                       vT->__anonCompField7.__anonCompField6.z)));
  */
  /*@ assert
        rte_387: mem_access:
        \valid_read(&self->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_388: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_389: is_nan_or_infinite:
        \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.w,
                             vT->__anonCompField7.__anonCompField6.w));
  */
  /*@ assert
        rte_390: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\add_float(\mul_float(self->__anonCompField7.__anonCompField6.x,
                                                              vT->__anonCompField7.__anonCompField6.x),
                                                   \mul_float(self->__anonCompField7.__anonCompField6.y,
                                                             vT->__anonCompField7.__anonCompField6.y)),
                                        \mul_float(self->__anonCompField7.__anonCompField6.z,
                                                  vT->__anonCompField7.__anonCompField6.z)),
                             \mul_float(self->__anonCompField7.__anonCompField6.w,
                                       vT->__anonCompField7.__anonCompField6.w)));
  */
  __retres = ((self->__anonCompField7.__anonCompField6.x * vT->__anonCompField7.__anonCompField6.x + 
               self->__anonCompField7.__anonCompField6.y * vT->__anonCompField7.__anonCompField6.y) + 
              self->__anonCompField7.__anonCompField6.z * vT->__anonCompField7.__anonCompField6.z) + 
             self->__anonCompField7.__anonCompField6.w * vT->__anonCompField7.__anonCompField6.w;
  return __retres;
}

/*@
requires \valid(vR);
requires \valid_read(vT1);
requires \valid_read(vT2);
requires \separated(vR, vT1);
requires \separated(vR, vT2);
requires \is_finite(vT1->__anonCompField7.__anonCompField6.x);
requires \is_finite(vT1->__anonCompField7.__anonCompField6.y);
requires \is_finite(vT1->__anonCompField7.__anonCompField6.z);
requires \is_finite(vT1->__anonCompField7.__anonCompField6.w);
requires \is_finite(vT2->__anonCompField7.__anonCompField6.x);
requires \is_finite(vT2->__anonCompField7.__anonCompField6.y);
requires \is_finite(vT2->__anonCompField7.__anonCompField6.z);
requires \is_finite(vT2->__anonCompField7.__anonCompField6.w);
assigns vR->__anonCompField7.__anonCompField6.x, vR->__anonCompField7.__anonCompField6.y, vR->__anonCompField7.__anonCompField6.z, vR->__anonCompField7.__anonCompField6.w;
ensures \result == vR;
ensures \is_finite(vR->__anonCompField7.__anonCompField6.w);
ensures vR->__anonCompField7.__anonCompField6.w == 0.0f;
*/
struct vector4 *vector4_cross_product(struct vector4 *vR,
                                      struct vector4 const *vT1,
                                      struct vector4 const *vT2)
{
  /*@ assert
        rte_391: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_392: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_393: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_394: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.y,
                             vT2->__anonCompField7.__anonCompField6.z));
  */
  /*@ assert
        rte_395: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_396: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_397: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.z,
                             vT2->__anonCompField7.__anonCompField6.y));
  */
  /*@ assert
        rte_398: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.y,
                                        vT2->__anonCompField7.__anonCompField6.z),
                             \mul_float(vT1->__anonCompField7.__anonCompField6.z,
                                       vT2->__anonCompField7.__anonCompField6.y)));
  */
  vR->__anonCompField7.__anonCompField6.x = vT1->__anonCompField7.__anonCompField6.y * vT2->__anonCompField7.__anonCompField6.z - 
                                            vT1->__anonCompField7.__anonCompField6.z * vT2->__anonCompField7.__anonCompField6.y;
  /*@ assert
        rte_399: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_400: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_401: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_402: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.z,
                             vT2->__anonCompField7.__anonCompField6.x));
  */
  /*@ assert
        rte_403: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_404: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_405: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.x,
                             vT2->__anonCompField7.__anonCompField6.z));
  */
  /*@ assert
        rte_406: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.z,
                                        vT2->__anonCompField7.__anonCompField6.x),
                             \mul_float(vT1->__anonCompField7.__anonCompField6.x,
                                       vT2->__anonCompField7.__anonCompField6.z)));
  */
  vR->__anonCompField7.__anonCompField6.y = vT1->__anonCompField7.__anonCompField6.z * vT2->__anonCompField7.__anonCompField6.x - 
                                            vT1->__anonCompField7.__anonCompField6.x * vT2->__anonCompField7.__anonCompField6.z;
  /*@ assert
        rte_407: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.z);
  */
  /*@ assert
        rte_408: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_409: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_410: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.x,
                             vT2->__anonCompField7.__anonCompField6.y));
  */
  /*@ assert
        rte_411: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.y);
  */
  /*@ assert
        rte_412: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.x);
  */
  /*@ assert
        rte_413: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.y,
                             vT2->__anonCompField7.__anonCompField6.x));
  */
  /*@ assert
        rte_414: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.x,
                                        vT2->__anonCompField7.__anonCompField6.y),
                             \mul_float(vT1->__anonCompField7.__anonCompField6.y,
                                       vT2->__anonCompField7.__anonCompField6.x)));
  */
  vR->__anonCompField7.__anonCompField6.z = vT1->__anonCompField7.__anonCompField6.x * vT2->__anonCompField7.__anonCompField6.y - 
                                            vT1->__anonCompField7.__anonCompField6.y * vT2->__anonCompField7.__anonCompField6.x;
  /*@ assert
        rte_415: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_416: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_417: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_418: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.w,
                             vT2->__anonCompField7.__anonCompField6.w));
  */
  /*@ assert
        rte_419: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_420: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.w);
  */
  /*@ assert
        rte_421: is_nan_or_infinite:
        \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.w,
                             vT2->__anonCompField7.__anonCompField6.w));
  */
  /*@ assert
        rte_422: is_nan_or_infinite:
        \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.w,
                                        vT2->__anonCompField7.__anonCompField6.w),
                             \mul_float(vT1->__anonCompField7.__anonCompField6.w,
                                       vT2->__anonCompField7.__anonCompField6.w)));
  */
  vR->__anonCompField7.__anonCompField6.w = vT1->__anonCompField7.__anonCompField6.w * vT2->__anonCompField7.__anonCompField6.w - 
                                            vT1->__anonCompField7.__anonCompField6.w * vT2->__anonCompField7.__anonCompField6.w;
  return vR;
}

int main(void)
{
  int __retres;
  short tmp_39;
  short tmp_40;
  short tmp_41;
  short tmp_42;
  short tmp_43;
  short tmp_44;
  short tmp_45;
  short tmp_46;
  short tmp_47;
  short tmp_48;
  short tmp_49;
  short tmp_50;
  short tmp_51;
  short tmp_52;
  short tmp_53;
  short tmp_54;
  struct vector2 v1;
  struct vector2 v2;
  struct vector2 v3;
  struct vector2 v4;
  struct vector2 v5;
  struct vector3 v31;
  struct vector3 v32;
  struct vector3 v33;
  struct vector3 v34;
  struct vector4 v41;
  struct vector4 v42;
  struct vector4 v43;
  struct vector4 v44;
  float result1 = HYP_MIN(1.0f,2.0f);
  float result2 = HYP_MIN(2.0f,1.0f);
  /*@ assert rte_423: is_nan_or_infinite: \is_finite(1.0f); */
  float result3 = HYP_MIN(- 1.0f,1.0f);
  float result4 = HYP_MIN(0.0f,0.0f);
  float result5 = HYP_MIN(3.5f,3.5f);
  /*@ assert rte_424: is_nan_or_infinite: \is_finite(5.0f); */
  /*@ assert rte_425: is_nan_or_infinite: \is_finite(3.0f); */
  float result6 = HYP_MIN(- 5.0f,- 3.0f);
  float result7 = HYP_MIN(100.0f,200.0f);
  /*@ assert rte_426: is_nan_or_infinite: \is_finite(100.0f); */
  /*@ assert rte_427: is_nan_or_infinite: \is_finite(200.0f); */
  float result8 = HYP_MIN(- 100.0f,- 200.0f);
  float result9 = HYP_MAX(1.0f,2.0f);
  float result10 = HYP_MAX(2.0f,1.0f);
  /*@ assert rte_428: is_nan_or_infinite: \is_finite(1.0f); */
  float result11 = HYP_MAX(- 1.0f,1.0f);
  float result12 = HYP_MAX(0.0f,0.0f);
  float result13 = HYP_MAX(3.5f,3.5f);
  /*@ assert rte_429: is_nan_or_infinite: \is_finite(5.0f); */
  /*@ assert rte_430: is_nan_or_infinite: \is_finite(3.0f); */
  float result14 = HYP_MAX(- 5.0f,- 3.0f);
  float result15 = HYP_MAX(100.0f,200.0f);
  /*@ assert rte_431: is_nan_or_infinite: \is_finite(100.0f); */
  /*@ assert rte_432: is_nan_or_infinite: \is_finite(200.0f); */
  float result16 = HYP_MAX(- 100.0f,- 200.0f);
  float a = 1.0f;
  float b = 2.0f;
  float old_a = a;
  float old_b = b;
  HYP_SWAP(& a,& b);
  /*@ assert rte_433: is_nan_or_infinite: \is_finite(5.0f); */
  a = - 5.0f;
  b = 10.0f;
  old_a = a;
  old_b = b;
  HYP_SWAP(& a,& b);
  a = 0.0f;
  b = 0.0f;
  old_a = a;
  old_b = b;
  HYP_SWAP(& a,& b);
  float result23 = HYP_SQUARE(0.0f);
  float result24 = HYP_SQUARE(1.0f);
  /*@ assert rte_434: is_nan_or_infinite: \is_finite(1.0f); */
  float result25 = HYP_SQUARE(- 1.0f);
  float result26 = HYP_SQUARE(2.0f);
  /*@ assert rte_435: is_nan_or_infinite: \is_finite(2.0f); */
  float result27 = HYP_SQUARE(- 2.0f);
  float result28 = HYP_SQUARE(3.5f);
  /*@ assert rte_436: is_nan_or_infinite: \is_finite(3.5f); */
  float result29 = HYP_SQUARE(- 3.5f);
  float result30 = HYP_SQUARE(10.0f);
  float result31 = HYP_ABS(0.0f);
  float result32 = HYP_ABS(1.0f);
  /*@ assert rte_437: is_nan_or_infinite: \is_finite(1.0f); */
  float result33 = HYP_ABS(- 1.0f);
  float result34 = HYP_ABS(5.5f);
  /*@ assert rte_438: is_nan_or_infinite: \is_finite(5.5f); */
  float result35 = HYP_ABS(- 5.5f);
  float result36 = HYP_ABS(100.0f);
  /*@ assert rte_439: is_nan_or_infinite: \is_finite(100.0f); */
  float result37 = HYP_ABS(- 100.0f);
  float result38 =
    HYP_ABS(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f);
  float result39 = HYP_CLAMP(5.0f,0.0f,10.0f);
  /*@ assert rte_440: is_nan_or_infinite: \is_finite(5.0f); */
  float result40 = HYP_CLAMP(- 5.0f,0.0f,10.0f);
  float result41 = HYP_CLAMP(15.0f,0.0f,10.0f);
  /*@ assert rte_441: is_nan_or_infinite: \is_finite(10.0f); */
  float result42 = HYP_CLAMP(0.0f,- 10.0f,10.0f);
  /*@ assert rte_442: is_nan_or_infinite: \is_finite(15.0f); */
  /*@ assert rte_443: is_nan_or_infinite: \is_finite(10.0f); */
  float result43 = HYP_CLAMP(- 15.0f,- 10.0f,10.0f);
  /*@ assert rte_444: is_nan_or_infinite: \is_finite(10.0f); */
  float result44 = HYP_CLAMP(15.0f,- 10.0f,10.0f);
  float result45 = HYP_CLAMP(2.5f,2.5f,7.5f);
  float result46 = HYP_CLAMP(7.5f,2.5f,7.5f);
  tmp_39 = scalar_equals_epsilonf(1.0f,1.0f,0.0001f);
  int result47 = (int)tmp_39;
  tmp_40 = scalar_equals_epsilonf(1.0f,1.00001f,0.0001f);
  int result48 = (int)tmp_40;
  tmp_41 = scalar_equals_epsilonf(1.0f,1.1f,0.0001f);
  int result49 = (int)tmp_41;
  tmp_42 = scalar_equals_epsilonf(0.0f,0.0f,0.0001f);
  int result50 = (int)tmp_42;
  /*@ assert rte_445: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_446: is_nan_or_infinite: \is_finite(1.0f); */
  tmp_43 = scalar_equals_epsilonf(- 1.0f,- 1.0f,0.0001f);
  int result51 = (int)tmp_43;
  /*@ assert rte_447: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_448: is_nan_or_infinite: \is_finite(1.00001f); */
  tmp_44 = scalar_equals_epsilonf(- 1.0f,- 1.00001f,0.0001f);
  int result52 = (int)tmp_44;
  /*@ assert rte_449: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_450: is_nan_or_infinite: \is_finite(0.9f); */
  tmp_45 = scalar_equals_epsilonf(- 1.0f,- 0.9f,0.0001f);
  int result53 = (int)tmp_45;
  tmp_46 = scalar_equals_epsilonf(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f,
                                  3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f,
                                  0.0001f);
  int result54 = (int)tmp_46;
  tmp_47 = scalar_equalsf(1.0f,1.0f);
  int result55 = (int)tmp_47;
  tmp_48 = scalar_equalsf(1.0f,1.000001f);
  int result56 = (int)tmp_48;
  tmp_49 = scalar_equalsf(1.0f,1.1f);
  int result57 = (int)tmp_49;
  tmp_50 = scalar_equalsf(0.0f,0.0f);
  int result58 = (int)tmp_50;
  /*@ assert rte_451: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_452: is_nan_or_infinite: \is_finite(1.0f); */
  tmp_51 = scalar_equalsf(- 1.0f,- 1.0f);
  int result59 = (int)tmp_51;
  /*@ assert rte_453: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_454: is_nan_or_infinite: \is_finite(1.000001f); */
  tmp_52 = scalar_equalsf(- 1.0f,- 1.000001f);
  int result60 = (int)tmp_52;
  /*@ assert rte_455: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_456: is_nan_or_infinite: \is_finite(0.9f); */
  tmp_53 = scalar_equalsf(- 1.0f,- 0.9f);
  int result61 = (int)tmp_53;
  tmp_54 = scalar_equalsf(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f,
                          3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f);
  int result62 = (int)tmp_54;
  vector2_setf2(& v1,1.0f,2.0f);
  /*@ assert rte_457: is_nan_or_infinite: \is_finite(3.0f); */
  vector2_setf2(& v2,- 3.0f,4.5f);
  vector2_set(& v3,(struct vector2 const *)(& v1));
  int equals_result67 =
    vector2_equals((struct vector2 const *)(& v3),
                   (struct vector2 const *)(& v1));
  vector2_zero(& v4);
  int equals_result70 =
    vector2_equals((struct vector2 const *)(& v1),
                   (struct vector2 const *)(& v1));
  int equals_result71 =
    vector2_equals((struct vector2 const *)(& v1),
                   (struct vector2 const *)(& v2));
  v3.__anonCompField2.v[0] = 2.0f;
  /*@ assert rte_458: is_nan_or_infinite: \is_finite(3.0f); */
  v3.__anonCompField2.v[1] = - 3.0f;
  vector2_negate(& v3);
  v1.__anonCompField2.v[0] = 1.0f;
  v1.__anonCompField2.v[1] = 2.0f;
  v2.__anonCompField2.v[0] = 3.0f;
  v2.__anonCompField2.v[1] = 4.0f;
  vector2_add(& v1,(struct vector2 const *)(& v2));
  v1.__anonCompField2.v[0] = 1.0f;
  v1.__anonCompField2.v[1] = 2.0f;
  vector2_addf(& v1,5.0f);
  v1.__anonCompField2.v[0] = 5.0f;
  v1.__anonCompField2.v[1] = 8.0f;
  v2.__anonCompField2.v[0] = 2.0f;
  v2.__anonCompField2.v[1] = 3.0f;
  vector2_subtract(& v1,(struct vector2 const *)(& v2));
  v1.__anonCompField2.v[0] = 10.0f;
  v1.__anonCompField2.v[1] = 20.0f;
  vector2_subtractf(& v1,5.0f);
  v1.__anonCompField2.v[0] = 2.0f;
  v1.__anonCompField2.v[1] = 3.0f;
  v2.__anonCompField2.v[0] = 4.0f;
  v2.__anonCompField2.v[1] = 5.0f;
  vector2_multiply(& v1,(struct vector2 const *)(& v2));
  v1.__anonCompField2.v[0] = 2.0f;
  v1.__anonCompField2.v[1] = 3.0f;
  vector2_multiplyf(& v1,3.0f);
  v1.__anonCompField2.v[0] = 8.0f;
  v1.__anonCompField2.v[1] = 15.0f;
  v2.__anonCompField2.v[0] = 2.0f;
  v2.__anonCompField2.v[1] = 3.0f;
  vector2_divide(& v1,(struct vector2 const *)(& v2));
  v1.__anonCompField2.v[0] = 10.0f;
  v1.__anonCompField2.v[1] = 20.0f;
  vector2_dividef(& v1,2.0f);
  vector2_setf2(& v1,2.0f,3.0f);
  vector2_setf2(& v2,4.0f,5.0f);
  float dot2 =
    vector2_dot_product((struct vector2 const *)(& v1),
                        (struct vector2 const *)(& v2));
  vector2_setf2(& v1,1.0f,0.0f);
  vector2_setf2(& v2,0.0f,1.0f);
  vector2_cross_product(& v3,(struct vector2 const *)(& v1),
                        (struct vector2 const *)(& v2));
  struct vector2 const *ref_zero = vector2_get_reference_vector2(0);
  struct vector2 const *ref_unit_x = vector2_get_reference_vector2(1);
  vector3_setf3(& v31,1.0f,2.0f,3.0f);
  vector3_setf3(& v32,4.0f,5.0f,6.0f);
  vector3_set(& v33,(struct vector3 const *)(& v32));
  int equals_result100 =
    vector3_equals((struct vector3 const *)(& v33),
                   (struct vector3 const *)(& v32));
  vector3_zero(& v34);
  int equals_result104 =
    vector3_equals((struct vector3 const *)(& v31),
                   (struct vector3 const *)(& v31));
  int equals_result105 =
    vector3_equals((struct vector3 const *)(& v31),
                   (struct vector3 const *)(& v32));
  v33.__anonCompField5.v[0] = 2.0f;
  /*@ assert rte_459: is_nan_or_infinite: \is_finite(3.0f); */
  v33.__anonCompField5.v[1] = - 3.0f;
  v33.__anonCompField5.v[2] = 4.0f;
  vector3_negate(& v33);
  v31.__anonCompField5.v[0] = 1.0f;
  v31.__anonCompField5.v[1] = 2.0f;
  v31.__anonCompField5.v[2] = 3.0f;
  v32.__anonCompField5.v[0] = 4.0f;
  v32.__anonCompField5.v[1] = 5.0f;
  v32.__anonCompField5.v[2] = 6.0f;
  vector3_add(& v31,(struct vector3 const *)(& v32));
  v31.__anonCompField5.v[0] = 1.0f;
  v31.__anonCompField5.v[1] = 2.0f;
  v31.__anonCompField5.v[2] = 3.0f;
  vector3_addf(& v31,5.0f);
  v31.__anonCompField5.v[0] = 5.0f;
  v31.__anonCompField5.v[1] = 8.0f;
  v31.__anonCompField5.v[2] = 10.0f;
  v32.__anonCompField5.v[0] = 2.0f;
  v32.__anonCompField5.v[1] = 3.0f;
  v32.__anonCompField5.v[2] = 4.0f;
  vector3_subtract(& v31,(struct vector3 const *)(& v32));
  v31.__anonCompField5.v[0] = 10.0f;
  v31.__anonCompField5.v[1] = 20.0f;
  v31.__anonCompField5.v[2] = 30.0f;
  vector3_subtractf(& v31,5.0f);
  v31.__anonCompField5.v[0] = 2.0f;
  v31.__anonCompField5.v[1] = 3.0f;
  v31.__anonCompField5.v[2] = 4.0f;
  v32.__anonCompField5.v[0] = 5.0f;
  v32.__anonCompField5.v[1] = 6.0f;
  v32.__anonCompField5.v[2] = 7.0f;
  vector3_multiply(& v31,(struct vector3 const *)(& v32));
  v31.__anonCompField5.v[0] = 2.0f;
  v31.__anonCompField5.v[1] = 3.0f;
  v31.__anonCompField5.v[2] = 4.0f;
  vector3_multiplyf(& v31,3.0f);
  v31.__anonCompField5.v[0] = 8.0f;
  v31.__anonCompField5.v[1] = 15.0f;
  v31.__anonCompField5.v[2] = 21.0f;
  v32.__anonCompField5.v[0] = 2.0f;
  v32.__anonCompField5.v[1] = 3.0f;
  v32.__anonCompField5.v[2] = 7.0f;
  vector3_divide(& v31,(struct vector3 const *)(& v32));
  vector3_setf3(& v31,2.0f,3.0f,4.0f);
  vector3_setf3(& v32,5.0f,6.0f,7.0f);
  float dot3 =
    vector3_dot_product((struct vector3 const *)(& v31),
                        (struct vector3 const *)(& v32));
  vector3_setf3(& v31,1.0f,0.0f,0.0f);
  vector3_setf3(& v32,0.0f,1.0f,0.0f);
  vector3_cross_product(& v33,(struct vector3 const *)(& v31),
                        (struct vector3 const *)(& v32));
  struct vector3 const *ref_zero3 = vector3_get_reference_vector3(0);
  struct vector3 const *ref_unit_x3 = vector3_get_reference_vector3(1);
  vector4_setf4(& v41,1.0f,2.0f,3.0f,4.0f);
  vector4_setf4(& v42,5.0f,6.0f,7.0f,8.0f);
  vector4_set(& v43,(struct vector4 const *)(& v42));
  int equals_result144 =
    vector4_equals((struct vector4 const *)(& v43),
                   (struct vector4 const *)(& v42));
  vector4_zero(& v44);
  int equals_result149 =
    vector4_equals((struct vector4 const *)(& v41),
                   (struct vector4 const *)(& v41));
  int equals_result150 =
    vector4_equals((struct vector4 const *)(& v41),
                   (struct vector4 const *)(& v42));
  v43.__anonCompField7.v[0] = 2.0f;
  /*@ assert rte_460: is_nan_or_infinite: \is_finite(3.0f); */
  v43.__anonCompField7.v[1] = - 3.0f;
  v43.__anonCompField7.v[2] = 4.0f;
  /*@ assert rte_461: is_nan_or_infinite: \is_finite(5.0f); */
  v43.__anonCompField7.v[3] = - 5.0f;
  vector4_negate(& v43);
  v41.__anonCompField7.v[0] = 1.0f;
  v41.__anonCompField7.v[1] = 2.0f;
  v41.__anonCompField7.v[2] = 3.0f;
  v41.__anonCompField7.v[3] = 4.0f;
  v42.__anonCompField7.v[0] = 5.0f;
  v42.__anonCompField7.v[1] = 6.0f;
  v42.__anonCompField7.v[2] = 7.0f;
  v42.__anonCompField7.v[3] = 8.0f;
  vector4_add(& v41,(struct vector4 const *)(& v42));
  v41.__anonCompField7.v[0] = 1.0f;
  v41.__anonCompField7.v[1] = 2.0f;
  v41.__anonCompField7.v[2] = 3.0f;
  v41.__anonCompField7.v[3] = 4.0f;
  vector4_addf(& v41,5.0f);
  v41.__anonCompField7.v[0] = 10.0f;
  v41.__anonCompField7.v[1] = 20.0f;
  v41.__anonCompField7.v[2] = 30.0f;
  v41.__anonCompField7.v[3] = 40.0f;
  v42.__anonCompField7.v[0] = 1.0f;
  v42.__anonCompField7.v[1] = 2.0f;
  v42.__anonCompField7.v[2] = 3.0f;
  v42.__anonCompField7.v[3] = 4.0f;
  vector4_subtract(& v41,(struct vector4 const *)(& v42));
  vector4_setf4(& v41,10.0f,20.0f,30.0f,40.0f);
  vector4_subtractf(& v41,5.0f);
  v41.__anonCompField7.v[0] = 2.0f;
  v41.__anonCompField7.v[1] = 3.0f;
  v41.__anonCompField7.v[2] = 4.0f;
  v41.__anonCompField7.v[3] = 5.0f;
  v42.__anonCompField7.v[0] = 6.0f;
  v42.__anonCompField7.v[1] = 7.0f;
  v42.__anonCompField7.v[2] = 8.0f;
  v42.__anonCompField7.v[3] = 9.0f;
  vector4_multiply(& v41,(struct vector4 const *)(& v42));
  v41.__anonCompField7.v[0] = 2.0f;
  v41.__anonCompField7.v[1] = 3.0f;
  v41.__anonCompField7.v[2] = 4.0f;
  v41.__anonCompField7.v[3] = 5.0f;
  vector4_multiplyf(& v41,3.0f);
  v41.__anonCompField7.v[0] = 12.0f;
  v41.__anonCompField7.v[1] = 21.0f;
  v41.__anonCompField7.v[2] = 32.0f;
  v41.__anonCompField7.v[3] = 45.0f;
  v42.__anonCompField7.v[0] = 2.0f;
  v42.__anonCompField7.v[1] = 3.0f;
  v42.__anonCompField7.v[2] = 4.0f;
  v42.__anonCompField7.v[3] = 5.0f;
  vector4_divide(& v41,(struct vector4 const *)(& v42));
  vector4_setf4(& v41,2.0f,3.0f,4.0f,5.0f);
  vector4_setf4(& v42,6.0f,7.0f,8.0f,9.0f);
  float dot4 =
    vector4_dot_product((struct vector4 const *)(& v41),
                        (struct vector4 const *)(& v42));
  vector4_setf4(& v41,1.0f,0.0f,0.0f,1.0f);
  vector4_setf4(& v42,0.0f,1.0f,0.0f,1.0f);
  vector4_cross_product(& v43,(struct vector4 const *)(& v41),
                        (struct vector4 const *)(& v42));
  struct vector4 const *ref_zero4 = vector4_get_reference_vector4(0);
  struct vector4 const *ref_unit_x4 = vector4_get_reference_vector4(1);
  __retres = 0;
  return __retres;
}