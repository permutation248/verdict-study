#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
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
float HYP_MIN(float a, float b)
{
  float tmp;
  if (a < b) tmp = a; else tmp = b;
  return tmp;
}

float HYP_MAX(float a, float b)
{
  float tmp;
  if (a > b) tmp = a; else tmp = b;
  return tmp;
}

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

float HYP_SQUARE(float number)
{
  float __retres;
  /*@ assert rte_5: is_nan_or_infinite: \is_finite(\mul_float(number, number)); */
  __retres = number * number;
  return __retres;
}

float HYP_ABS(float value)
{
  float tmp;
  if (value < 0.0f) 
    /*@ assert rte_6: is_nan_or_infinite: \is_finite(value); */
    tmp = - value;
  else tmp = value;
  return tmp;
}

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

short scalar_equalsf(float const f1, float const f2)
{
  short tmp;
  tmp = scalar_equals_epsilonf(f1,f2,1E-5f);
  return tmp;
}

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

struct vector2 *vector2_set(struct vector2 *self, struct vector2 const *vT)
{
  /*@ assert rte_10: mem_access: \valid(&self->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_11: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.x); */
  self->__anonCompField2.__anonCompField1.x = vT->__anonCompField2.__anonCompField1.x;
  /*@ assert rte_12: mem_access: \valid(&self->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_13: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.y); */
  self->__anonCompField2.__anonCompField1.y = vT->__anonCompField2.__anonCompField1.y;
  return self;
}

struct vector2 *vector2_setf2(struct vector2 *self, float xT, float yT)
{
  /*@ assert rte_8: mem_access: \valid(&self->__anonCompField2.__anonCompField1.x); */
  self->__anonCompField2.__anonCompField1.x = xT;
  /*@ assert rte_9: mem_access: \valid(&self->__anonCompField2.__anonCompField1.y); */
  self->__anonCompField2.__anonCompField1.y = yT;
  return self;
}

struct vector2 *vector2_zero(struct vector2 *self)
{
  struct vector2 *tmp;
  tmp = vector2_setf2(self,0.0f,0.0f);
  return tmp;
}

int vector2_equals(struct vector2 const *self, struct vector2 const *vT)
{
  int tmp_1;
  short tmp;
  /*@ assert rte_14: mem_access: \valid_read(&self->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_15: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.x); */
  tmp = scalar_equalsf(self->__anonCompField2.__anonCompField1.x,
                       vT->__anonCompField2.__anonCompField1.x);
  if (tmp) {
    short tmp_0;
    /*@ assert rte_16: mem_access: \valid_read(&self->__anonCompField2.__anonCompField1.y); */
    /*@ assert rte_17: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.y); */
    tmp_0 = scalar_equalsf(self->__anonCompField2.__anonCompField1.y,
                           vT->__anonCompField2.__anonCompField1.y);
    if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
  }
  else tmp_1 = 0;
  return tmp_1;
}

struct vector2 *vector2_negate(struct vector2 *self)
{
  /*@ assert rte_18: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_19: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_20: is_nan_or_infinite: \is_finite(self->__anonCompField2.v[0]); */
  self->__anonCompField2.v[0] = - self->__anonCompField2.v[0];
  /*@ assert rte_21: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_22: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_23: is_nan_or_infinite: \is_finite(self->__anonCompField2.v[1]); */
  self->__anonCompField2.v[1] = - self->__anonCompField2.v[1];
  return self;
}

struct vector2 *vector2_add(struct vector2 *self, struct vector2 const *vT)
{
  /*@ assert rte_24: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_25: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_26: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert rte_27: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0])); */
  self->__anonCompField2.v[0] += vT->__anonCompField2.v[0];
  /*@ assert rte_28: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_29: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_30: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert rte_31: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1])); */
  self->__anonCompField2.v[1] += vT->__anonCompField2.v[1];
  return self;
}

struct vector2 *vector2_addf(struct vector2 *self, float fT)
{
  /*@ assert rte_32: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_33: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_34: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField2.v[0], fT)); */
  self->__anonCompField2.v[0] += fT;
  /*@ assert rte_35: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_36: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_37: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField2.v[1], fT)); */
  self->__anonCompField2.v[1] += fT;
  return self;
}

struct vector2 *vector2_subtract(struct vector2 *self,
                                 struct vector2 const *vT)
{
  /*@ assert rte_38: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_39: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_40: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert rte_41: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0])); */
  self->__anonCompField2.v[0] -= vT->__anonCompField2.v[0];
  /*@ assert rte_42: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_43: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_44: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert rte_45: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1])); */
  self->__anonCompField2.v[1] -= vT->__anonCompField2.v[1];
  return self;
}

struct vector2 *vector2_subtractf(struct vector2 *self, float fT)
{
  /*@ assert rte_46: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_47: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_48: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField2.v[0], fT)); */
  self->__anonCompField2.v[0] -= fT;
  /*@ assert rte_49: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_50: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_51: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField2.v[1], fT)); */
  self->__anonCompField2.v[1] -= fT;
  return self;
}

struct vector2 *vector2_multiply(struct vector2 *self,
                                 struct vector2 const *vT)
{
  /*@ assert rte_52: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_53: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_54: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert rte_55: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0])); */
  self->__anonCompField2.v[0] *= vT->__anonCompField2.v[0];
  /*@ assert rte_56: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_57: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_58: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert rte_59: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1])); */
  self->__anonCompField2.v[1] *= vT->__anonCompField2.v[1];
  return self;
}

struct vector2 *vector2_multiplyf(struct vector2 *self, float fT)
{
  /*@ assert rte_60: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_61: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_62: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField2.v[0], fT)); */
  self->__anonCompField2.v[0] *= fT;
  /*@ assert rte_63: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_64: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_65: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField2.v[1], fT)); */
  self->__anonCompField2.v[1] *= fT;
  return self;
}

struct vector2 *vector2_divide(struct vector2 *self, struct vector2 const *vT)
{
  /*@ assert rte_66: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_67: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_68: mem_access: \valid_read(&vT->__anonCompField2.v[0]); */
  /*@ assert rte_69: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField2.v[0], vT->__anonCompField2.v[0])); */
  self->__anonCompField2.v[0] /= vT->__anonCompField2.v[0];
  /*@ assert rte_70: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_71: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_72: mem_access: \valid_read(&vT->__anonCompField2.v[1]); */
  /*@ assert rte_73: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField2.v[1], vT->__anonCompField2.v[1])); */
  self->__anonCompField2.v[1] /= vT->__anonCompField2.v[1];
  return self;
}

struct vector2 *vector2_dividef(struct vector2 *self, float fT)
{
  /*@ assert rte_74: mem_access: \valid(&self->__anonCompField2.v[0]); */
  /*@ assert rte_75: mem_access: \valid_read(&self->__anonCompField2.v[0]); */
  /*@ assert rte_76: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField2.v[0], fT)); */
  self->__anonCompField2.v[0] /= fT;
  /*@ assert rte_77: mem_access: \valid(&self->__anonCompField2.v[1]); */
  /*@ assert rte_78: mem_access: \valid_read(&self->__anonCompField2.v[1]); */
  /*@ assert rte_79: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField2.v[1], fT)); */
  self->__anonCompField2.v[1] /= fT;
  return self;
}

float vector2_dot_product(struct vector2 const *self,
                          struct vector2 const *vT)
{
  float __retres;
  /*@ assert rte_80: mem_access: \valid_read(&self->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_81: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_82: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField2.__anonCompField1.x, vT->__anonCompField2.__anonCompField1.x)); */
  /*@ assert rte_83: mem_access: \valid_read(&self->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_84: mem_access: \valid_read(&vT->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_85: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField2.__anonCompField1.y, vT->__anonCompField2.__anonCompField1.y)); */
  /*@ assert rte_86: is_nan_or_infinite: \is_finite(\add_float(\mul_float(self->__anonCompField2.__anonCompField1.x, vT->__anonCompField2.__anonCompField1.x), \mul_float(self->__anonCompField2.__anonCompField1.y, vT->__anonCompField2.__anonCompField1.y))); */
  __retres = self->__anonCompField2.__anonCompField1.x * vT->__anonCompField2.__anonCompField1.x + 
             self->__anonCompField2.__anonCompField1.y * vT->__anonCompField2.__anonCompField1.y;
  return __retres;
}

struct vector2 *vector2_cross_product(struct vector2 *vR,
                                      struct vector2 const *vT1,
                                      struct vector2 const *vT2)
{
  /*@ assert rte_87: mem_access: \valid(&vR->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_88: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_89: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_90: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.x, vT2->__anonCompField2.__anonCompField1.y)); */
  /*@ assert rte_91: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_92: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_93: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.y, vT2->__anonCompField2.__anonCompField1.x)); */
  /*@ assert rte_94: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField2.__anonCompField1.x, vT2->__anonCompField2.__anonCompField1.y), \mul_float(vT1->__anonCompField2.__anonCompField1.y, vT2->__anonCompField2.__anonCompField1.x))); */
  vR->__anonCompField2.__anonCompField1.x = vT1->__anonCompField2.__anonCompField1.x * vT2->__anonCompField2.__anonCompField1.y - 
                                            vT1->__anonCompField2.__anonCompField1.y * vT2->__anonCompField2.__anonCompField1.x;
  /*@ assert rte_95: mem_access: \valid(&vR->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_96: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_97: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_98: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.y, vT2->__anonCompField2.__anonCompField1.x)); */
  /*@ assert rte_99: mem_access: \valid_read(&vT1->__anonCompField2.__anonCompField1.x); */
  /*@ assert rte_100: mem_access: \valid_read(&vT2->__anonCompField2.__anonCompField1.y); */
  /*@ assert rte_101: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField2.__anonCompField1.x, vT2->__anonCompField2.__anonCompField1.y)); */
  /*@ assert rte_102: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField2.__anonCompField1.y, vT2->__anonCompField2.__anonCompField1.x), \mul_float(vT1->__anonCompField2.__anonCompField1.x, vT2->__anonCompField2.__anonCompField1.y))); */
  vR->__anonCompField2.__anonCompField1.y = vT1->__anonCompField2.__anonCompField1.y * vT2->__anonCompField2.__anonCompField1.x - 
                                            vT1->__anonCompField2.__anonCompField1.x * vT2->__anonCompField2.__anonCompField1.y;
  return vR;
}

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

struct vector3 *vector3_setf3(struct vector3 *self, float xT, float yT,
                              float zT)
{
  /*@ assert rte_103: mem_access: \valid(&self->__anonCompField5.__anonCompField3.x); */
  self->__anonCompField5.__anonCompField3.x = xT;
  /*@ assert rte_104: mem_access: \valid(&self->__anonCompField5.__anonCompField3.y); */
  self->__anonCompField5.__anonCompField3.y = yT;
  /*@ assert rte_105: mem_access: \valid(&self->__anonCompField5.__anonCompField3.z); */
  self->__anonCompField5.__anonCompField3.z = zT;
  return self;
}

struct vector3 *vector3_set(struct vector3 *self, struct vector3 const *vT)
{
  /*@ assert rte_106: mem_access: \valid(&self->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_107: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.x); */
  self->__anonCompField5.__anonCompField3.x = vT->__anonCompField5.__anonCompField3.x;
  /*@ assert rte_108: mem_access: \valid(&self->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_109: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.y); */
  self->__anonCompField5.__anonCompField3.y = vT->__anonCompField5.__anonCompField3.y;
  /*@ assert rte_110: mem_access: \valid(&self->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_111: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.z); */
  self->__anonCompField5.__anonCompField3.z = vT->__anonCompField5.__anonCompField3.z;
  return self;
}

struct vector3 *vector3_zero(struct vector3 *self)
{
  struct vector3 *tmp;
  tmp = vector3_setf3(self,0.0f,0.0f,0.0f);
  return tmp;
}

int vector3_equals(struct vector3 const *self, struct vector3 const *vT)
{
  int tmp_2;
  float tmp;
  /*@ assert rte_112: mem_access: \valid_read(&self->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_113: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_114: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.__anonCompField3.x, vT->__anonCompField5.__anonCompField3.x)); */
  tmp = HYP_ABS(self->__anonCompField5.__anonCompField3.x - vT->__anonCompField5.__anonCompField3.x);
  if (tmp < 1E-5f) {
    float tmp_0;
    /*@ assert rte_115: mem_access: \valid_read(&self->__anonCompField5.__anonCompField3.y); */
    /*@ assert rte_116: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.y); */
    /*@ assert rte_117: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.__anonCompField3.y, vT->__anonCompField5.__anonCompField3.y)); */
    tmp_0 = HYP_ABS(self->__anonCompField5.__anonCompField3.y - vT->__anonCompField5.__anonCompField3.y);
    if (tmp_0 < 1E-5f) {
      float tmp_1;
      /*@ assert rte_118: mem_access: \valid_read(&self->__anonCompField5.__anonCompField3.z); */
      /*@ assert rte_119: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.z); */
      /*@ assert rte_120: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.__anonCompField3.z, vT->__anonCompField5.__anonCompField3.z)); */
      tmp_1 = HYP_ABS(self->__anonCompField5.__anonCompField3.z - vT->__anonCompField5.__anonCompField3.z);
      if (tmp_1 < 1E-5f) tmp_2 = 1; else tmp_2 = 0;
    }
    else tmp_2 = 0;
  }
  else tmp_2 = 0;
  return tmp_2;
}

struct vector3 *vector3_negate(struct vector3 *self)
{
  /*@ assert rte_121: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_122: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_123: is_nan_or_infinite: \is_finite(self->__anonCompField5.v[0]); */
  self->__anonCompField5.v[0] = - self->__anonCompField5.v[0];
  /*@ assert rte_124: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_125: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_126: is_nan_or_infinite: \is_finite(self->__anonCompField5.v[1]); */
  self->__anonCompField5.v[1] = - self->__anonCompField5.v[1];
  /*@ assert rte_127: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_128: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_129: is_nan_or_infinite: \is_finite(self->__anonCompField5.v[2]); */
  self->__anonCompField5.v[2] = - self->__anonCompField5.v[2];
  return self;
}

struct vector3 *vector3_add(struct vector3 *self, struct vector3 const *vT)
{
  /*@ assert rte_130: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_131: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_132: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert rte_133: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField5.v[0], vT->__anonCompField5.v[0])); */
  self->__anonCompField5.v[0] += vT->__anonCompField5.v[0];
  /*@ assert rte_134: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_135: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_136: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert rte_137: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField5.v[1], vT->__anonCompField5.v[1])); */
  self->__anonCompField5.v[1] += vT->__anonCompField5.v[1];
  /*@ assert rte_138: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_139: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_140: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert rte_141: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField5.v[2], vT->__anonCompField5.v[2])); */
  self->__anonCompField5.v[2] += vT->__anonCompField5.v[2];
  return self;
}

struct vector3 *vector3_addf(struct vector3 *self, float f)
{
  /*@ assert rte_142: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_143: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_144: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField5.v[0], f)); */
  self->__anonCompField5.v[0] += f;
  /*@ assert rte_145: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_146: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_147: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField5.v[1], f)); */
  self->__anonCompField5.v[1] += f;
  /*@ assert rte_148: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_149: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_150: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField5.v[2], f)); */
  self->__anonCompField5.v[2] += f;
  return self;
}

struct vector3 *vector3_subtract(struct vector3 *self,
                                 struct vector3 const *vT)
{
  /*@ assert rte_151: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_152: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_153: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert rte_154: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.v[0], vT->__anonCompField5.v[0])); */
  self->__anonCompField5.v[0] -= vT->__anonCompField5.v[0];
  /*@ assert rte_155: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_156: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_157: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert rte_158: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.v[1], vT->__anonCompField5.v[1])); */
  self->__anonCompField5.v[1] -= vT->__anonCompField5.v[1];
  /*@ assert rte_159: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_160: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_161: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert rte_162: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.v[2], vT->__anonCompField5.v[2])); */
  self->__anonCompField5.v[2] -= vT->__anonCompField5.v[2];
  return self;
}

struct vector3 *vector3_subtractf(struct vector3 *self, float f)
{
  /*@ assert rte_163: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_164: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_165: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.v[0], f)); */
  self->__anonCompField5.v[0] -= f;
  /*@ assert rte_166: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_167: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_168: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.v[1], f)); */
  self->__anonCompField5.v[1] -= f;
  /*@ assert rte_169: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_170: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_171: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField5.v[2], f)); */
  self->__anonCompField5.v[2] -= f;
  return self;
}

struct vector3 *vector3_multiply(struct vector3 *self,
                                 struct vector3 const *vT)
{
  /*@ assert rte_172: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_173: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_174: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert rte_175: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.v[0], vT->__anonCompField5.v[0])); */
  self->__anonCompField5.v[0] *= vT->__anonCompField5.v[0];
  /*@ assert rte_176: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_177: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_178: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert rte_179: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.v[1], vT->__anonCompField5.v[1])); */
  self->__anonCompField5.v[1] *= vT->__anonCompField5.v[1];
  /*@ assert rte_180: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_181: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_182: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert rte_183: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.v[2], vT->__anonCompField5.v[2])); */
  self->__anonCompField5.v[2] *= vT->__anonCompField5.v[2];
  return self;
}

struct vector3 *vector3_multiplyf(struct vector3 *self, float f)
{
  /*@ assert rte_184: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_185: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_186: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.v[0], f)); */
  self->__anonCompField5.v[0] *= f;
  /*@ assert rte_187: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_188: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_189: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.v[1], f)); */
  self->__anonCompField5.v[1] *= f;
  /*@ assert rte_190: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_191: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_192: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.v[2], f)); */
  self->__anonCompField5.v[2] *= f;
  return self;
}

struct vector3 *vector3_divide(struct vector3 *self, struct vector3 const *vT)
{
  /*@ assert rte_193: mem_access: \valid(&self->__anonCompField5.v[0]); */
  /*@ assert rte_194: mem_access: \valid_read(&self->__anonCompField5.v[0]); */
  /*@ assert rte_195: mem_access: \valid_read(&vT->__anonCompField5.v[0]); */
  /*@ assert rte_196: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField5.v[0], vT->__anonCompField5.v[0])); */
  self->__anonCompField5.v[0] /= vT->__anonCompField5.v[0];
  /*@ assert rte_197: mem_access: \valid(&self->__anonCompField5.v[1]); */
  /*@ assert rte_198: mem_access: \valid_read(&self->__anonCompField5.v[1]); */
  /*@ assert rte_199: mem_access: \valid_read(&vT->__anonCompField5.v[1]); */
  /*@ assert rte_200: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField5.v[1], vT->__anonCompField5.v[1])); */
  self->__anonCompField5.v[1] /= vT->__anonCompField5.v[1];
  /*@ assert rte_201: mem_access: \valid(&self->__anonCompField5.v[2]); */
  /*@ assert rte_202: mem_access: \valid_read(&self->__anonCompField5.v[2]); */
  /*@ assert rte_203: mem_access: \valid_read(&vT->__anonCompField5.v[2]); */
  /*@ assert rte_204: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField5.v[2], vT->__anonCompField5.v[2])); */
  self->__anonCompField5.v[2] /= vT->__anonCompField5.v[2];
  return self;
}

float vector3_dot_product(struct vector3 const *self,
                          struct vector3 const *vT)
{
  float __retres;
  /*@ assert rte_205: mem_access: \valid_read(&self->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_206: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_207: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.__anonCompField3.x, vT->__anonCompField5.__anonCompField3.x)); */
  /*@ assert rte_208: mem_access: \valid_read(&self->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_209: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_210: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.__anonCompField3.y, vT->__anonCompField5.__anonCompField3.y)); */
  /*@ assert rte_211: is_nan_or_infinite: \is_finite(\add_float(\mul_float(self->__anonCompField5.__anonCompField3.x, vT->__anonCompField5.__anonCompField3.x), \mul_float(self->__anonCompField5.__anonCompField3.y, vT->__anonCompField5.__anonCompField3.y))); */
  /*@ assert rte_212: mem_access: \valid_read(&self->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_213: mem_access: \valid_read(&vT->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_214: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField5.__anonCompField3.z, vT->__anonCompField5.__anonCompField3.z)); */
  /*@ assert rte_215: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(self->__anonCompField5.__anonCompField3.x, vT->__anonCompField5.__anonCompField3.x), \mul_float(self->__anonCompField5.__anonCompField3.y, vT->__anonCompField5.__anonCompField3.y)), \mul_float(self->__anonCompField5.__anonCompField3.z, vT->__anonCompField5.__anonCompField3.z))); */
  __retres = (self->__anonCompField5.__anonCompField3.x * vT->__anonCompField5.__anonCompField3.x + 
              self->__anonCompField5.__anonCompField3.y * vT->__anonCompField5.__anonCompField3.y) + 
             self->__anonCompField5.__anonCompField3.z * vT->__anonCompField5.__anonCompField3.z;
  return __retres;
}

struct vector3 *vector3_cross_product(struct vector3 *vR,
                                      struct vector3 const *vT1,
                                      struct vector3 const *vT2)
{
  /*@ assert rte_216: mem_access: \valid(&vR->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_217: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_218: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_219: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.y, vT2->__anonCompField5.__anonCompField3.z)); */
  /*@ assert rte_220: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_221: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_222: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.z, vT2->__anonCompField5.__anonCompField3.y)); */
  /*@ assert rte_223: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField5.__anonCompField3.y, vT2->__anonCompField5.__anonCompField3.z), \mul_float(vT1->__anonCompField5.__anonCompField3.z, vT2->__anonCompField5.__anonCompField3.y))); */
  vR->__anonCompField5.__anonCompField3.x = vT1->__anonCompField5.__anonCompField3.y * vT2->__anonCompField5.__anonCompField3.z - 
                                            vT1->__anonCompField5.__anonCompField3.z * vT2->__anonCompField5.__anonCompField3.y;
  /*@ assert rte_224: mem_access: \valid(&vR->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_225: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_226: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_227: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.z, vT2->__anonCompField5.__anonCompField3.x)); */
  /*@ assert rte_228: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_229: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_230: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.x, vT2->__anonCompField5.__anonCompField3.z)); */
  /*@ assert rte_231: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField5.__anonCompField3.z, vT2->__anonCompField5.__anonCompField3.x), \mul_float(vT1->__anonCompField5.__anonCompField3.x, vT2->__anonCompField5.__anonCompField3.z))); */
  vR->__anonCompField5.__anonCompField3.y = vT1->__anonCompField5.__anonCompField3.z * vT2->__anonCompField5.__anonCompField3.x - 
                                            vT1->__anonCompField5.__anonCompField3.x * vT2->__anonCompField5.__anonCompField3.z;
  /*@ assert rte_232: mem_access: \valid(&vR->__anonCompField5.__anonCompField3.z); */
  /*@ assert rte_233: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_234: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_235: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.x, vT2->__anonCompField5.__anonCompField3.y)); */
  /*@ assert rte_236: mem_access: \valid_read(&vT1->__anonCompField5.__anonCompField3.y); */
  /*@ assert rte_237: mem_access: \valid_read(&vT2->__anonCompField5.__anonCompField3.x); */
  /*@ assert rte_238: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField5.__anonCompField3.y, vT2->__anonCompField5.__anonCompField3.x)); */
  /*@ assert rte_239: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField5.__anonCompField3.x, vT2->__anonCompField5.__anonCompField3.y), \mul_float(vT1->__anonCompField5.__anonCompField3.y, vT2->__anonCompField5.__anonCompField3.x))); */
  vR->__anonCompField5.__anonCompField3.z = vT1->__anonCompField5.__anonCompField3.x * vT2->__anonCompField5.__anonCompField3.y - 
                                            vT1->__anonCompField5.__anonCompField3.y * vT2->__anonCompField5.__anonCompField3.x;
  return vR;
}

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

struct vector4 *vector4_setf4(struct vector4 *self, float xT, float yT,
                              float zT, float wT)
{
  /*@ assert rte_240: mem_access: \valid(&self->__anonCompField7.__anonCompField6.x); */
  self->__anonCompField7.__anonCompField6.x = xT;
  /*@ assert rte_241: mem_access: \valid(&self->__anonCompField7.__anonCompField6.y); */
  self->__anonCompField7.__anonCompField6.y = yT;
  /*@ assert rte_242: mem_access: \valid(&self->__anonCompField7.__anonCompField6.z); */
  self->__anonCompField7.__anonCompField6.z = zT;
  /*@ assert rte_243: mem_access: \valid(&self->__anonCompField7.__anonCompField6.w); */
  self->__anonCompField7.__anonCompField6.w = wT;
  return self;
}

struct vector4 *vector4_set(struct vector4 *self, struct vector4 const *vT)
{
  /*@ assert rte_244: mem_access: \valid(&self->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_245: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.x); */
  self->__anonCompField7.__anonCompField6.x = vT->__anonCompField7.__anonCompField6.x;
  /*@ assert rte_246: mem_access: \valid(&self->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_247: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.y); */
  self->__anonCompField7.__anonCompField6.y = vT->__anonCompField7.__anonCompField6.y;
  /*@ assert rte_248: mem_access: \valid(&self->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_249: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.z); */
  self->__anonCompField7.__anonCompField6.z = vT->__anonCompField7.__anonCompField6.z;
  /*@ assert rte_250: mem_access: \valid(&self->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_251: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.w); */
  self->__anonCompField7.__anonCompField6.w = vT->__anonCompField7.__anonCompField6.w;
  return self;
}

struct vector4 *vector4_zero(struct vector4 *self)
{
  struct vector4 *tmp;
  tmp = vector4_setf4(self,0.0f,0.0f,0.0f,0.0f);
  return tmp;
}

int vector4_equals(struct vector4 const *self, struct vector4 const *vT)
{
  int tmp_3;
  float tmp;
  /*@ assert rte_252: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_253: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_254: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.x, vT->__anonCompField7.__anonCompField6.x)); */
  tmp = HYP_ABS(self->__anonCompField7.__anonCompField6.x - vT->__anonCompField7.__anonCompField6.x);
  if (tmp < 1E-5f) {
    float tmp_0;
    /*@ assert rte_255: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.y); */
    /*@ assert rte_256: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.y); */
    /*@ assert rte_257: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.y, vT->__anonCompField7.__anonCompField6.y)); */
    tmp_0 = HYP_ABS(self->__anonCompField7.__anonCompField6.y - vT->__anonCompField7.__anonCompField6.y);
    if (tmp_0 < 1E-5f) {
      float tmp_1;
      /*@ assert rte_258: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.z); */
      /*@ assert rte_259: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.z); */
      /*@ assert rte_260: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.z, vT->__anonCompField7.__anonCompField6.z)); */
      tmp_1 = HYP_ABS(self->__anonCompField7.__anonCompField6.z - vT->__anonCompField7.__anonCompField6.z);
      if (tmp_1 < 1E-5f) {
        float tmp_2;
        /*@ assert rte_261: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.w); */
        /*@ assert rte_262: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.w); */
        /*@ assert rte_263: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.w, vT->__anonCompField7.__anonCompField6.w)); */
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

struct vector4 *vector4_negate(struct vector4 *self)
{
  /*@ assert rte_264: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_265: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_266: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[0]); */
  self->__anonCompField7.v[0] = - self->__anonCompField7.v[0];
  /*@ assert rte_267: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_268: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_269: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[1]); */
  self->__anonCompField7.v[1] = - self->__anonCompField7.v[1];
  /*@ assert rte_270: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_271: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_272: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[2]); */
  self->__anonCompField7.v[2] = - self->__anonCompField7.v[2];
  /*@ assert rte_273: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_274: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_275: is_nan_or_infinite: \is_finite(self->__anonCompField7.v[3]); */
  self->__anonCompField7.v[3] = - self->__anonCompField7.v[3];
  return self;
}

struct vector4 *vector4_add(struct vector4 *self, struct vector4 const *vT)
{
  /*@ assert rte_276: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_277: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_278: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert rte_279: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[0], vT->__anonCompField7.v[0])); */
  self->__anonCompField7.v[0] += vT->__anonCompField7.v[0];
  /*@ assert rte_280: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_281: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_282: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert rte_283: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[1], vT->__anonCompField7.v[1])); */
  self->__anonCompField7.v[1] += vT->__anonCompField7.v[1];
  /*@ assert rte_284: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_285: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_286: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert rte_287: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[2], vT->__anonCompField7.v[2])); */
  self->__anonCompField7.v[2] += vT->__anonCompField7.v[2];
  /*@ assert rte_288: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_289: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_290: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert rte_291: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[3], vT->__anonCompField7.v[3])); */
  self->__anonCompField7.v[3] += vT->__anonCompField7.v[3];
  return self;
}

struct vector4 *vector4_addf(struct vector4 *self, float f)
{
  /*@ assert rte_292: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_293: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_294: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[0], f)); */
  self->__anonCompField7.v[0] += f;
  /*@ assert rte_295: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_296: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_297: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[1], f)); */
  self->__anonCompField7.v[1] += f;
  /*@ assert rte_298: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_299: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_300: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[2], f)); */
  self->__anonCompField7.v[2] += f;
  /*@ assert rte_301: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_302: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_303: is_nan_or_infinite: \is_finite(\add_float(self->__anonCompField7.v[3], f)); */
  self->__anonCompField7.v[3] += f;
  return self;
}

struct vector4 *vector4_subtract(struct vector4 *self,
                                 struct vector4 const *vT)
{
  /*@ assert rte_304: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_305: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_306: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert rte_307: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.v[0], vT->__anonCompField7.v[0])); */
  self->__anonCompField7.v[0] -= vT->__anonCompField7.v[0];
  /*@ assert rte_308: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_309: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_310: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert rte_311: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.v[1], vT->__anonCompField7.v[1])); */
  self->__anonCompField7.v[1] -= vT->__anonCompField7.v[1];
  /*@ assert rte_312: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_313: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_314: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert rte_315: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.v[2], vT->__anonCompField7.v[2])); */
  self->__anonCompField7.v[2] -= vT->__anonCompField7.v[2];
  /*@ assert rte_316: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_317: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_318: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert rte_319: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.v[3], vT->__anonCompField7.v[3])); */
  self->__anonCompField7.v[3] -= vT->__anonCompField7.v[3];
  return self;
}

struct vector4 *vector4_subtractf(struct vector4 *self, float f)
{
  /*@ assert rte_320: mem_access: \valid(&self->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_321: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_322: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.x, f)); */
  self->__anonCompField7.__anonCompField6.x -= f;
  /*@ assert rte_323: mem_access: \valid(&self->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_324: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_325: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.y, f)); */
  self->__anonCompField7.__anonCompField6.y -= f;
  /*@ assert rte_326: mem_access: \valid(&self->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_327: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_328: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.z, f)); */
  self->__anonCompField7.__anonCompField6.z -= f;
  /*@ assert rte_329: mem_access: \valid(&self->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_330: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_331: is_nan_or_infinite: \is_finite(\sub_float(self->__anonCompField7.__anonCompField6.w, f)); */
  self->__anonCompField7.__anonCompField6.w -= f;
  return self;
}

struct vector4 *vector4_multiply(struct vector4 *self,
                                 struct vector4 const *vT)
{
  /*@ assert rte_332: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_333: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_334: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert rte_335: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[0], vT->__anonCompField7.v[0])); */
  self->__anonCompField7.v[0] *= vT->__anonCompField7.v[0];
  /*@ assert rte_336: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_337: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_338: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert rte_339: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[1], vT->__anonCompField7.v[1])); */
  self->__anonCompField7.v[1] *= vT->__anonCompField7.v[1];
  /*@ assert rte_340: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_341: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_342: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert rte_343: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[2], vT->__anonCompField7.v[2])); */
  self->__anonCompField7.v[2] *= vT->__anonCompField7.v[2];
  /*@ assert rte_344: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_345: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_346: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert rte_347: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[3], vT->__anonCompField7.v[3])); */
  self->__anonCompField7.v[3] *= vT->__anonCompField7.v[3];
  return self;
}

struct vector4 *vector4_multiplyf(struct vector4 *self, float f)
{
  /*@ assert rte_348: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_349: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_350: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[0], f)); */
  self->__anonCompField7.v[0] *= f;
  /*@ assert rte_351: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_352: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_353: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[1], f)); */
  self->__anonCompField7.v[1] *= f;
  /*@ assert rte_354: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_355: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_356: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[2], f)); */
  self->__anonCompField7.v[2] *= f;
  /*@ assert rte_357: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_358: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_359: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.v[3], f)); */
  self->__anonCompField7.v[3] *= f;
  return self;
}

struct vector4 *vector4_divide(struct vector4 *self, struct vector4 const *vT)
{
  /*@ assert rte_360: mem_access: \valid(&self->__anonCompField7.v[0]); */
  /*@ assert rte_361: mem_access: \valid_read(&self->__anonCompField7.v[0]); */
  /*@ assert rte_362: mem_access: \valid_read(&vT->__anonCompField7.v[0]); */
  /*@ assert rte_363: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField7.v[0], vT->__anonCompField7.v[0])); */
  self->__anonCompField7.v[0] /= vT->__anonCompField7.v[0];
  /*@ assert rte_364: mem_access: \valid(&self->__anonCompField7.v[1]); */
  /*@ assert rte_365: mem_access: \valid_read(&self->__anonCompField7.v[1]); */
  /*@ assert rte_366: mem_access: \valid_read(&vT->__anonCompField7.v[1]); */
  /*@ assert rte_367: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField7.v[1], vT->__anonCompField7.v[1])); */
  self->__anonCompField7.v[1] /= vT->__anonCompField7.v[1];
  /*@ assert rte_368: mem_access: \valid(&self->__anonCompField7.v[2]); */
  /*@ assert rte_369: mem_access: \valid_read(&self->__anonCompField7.v[2]); */
  /*@ assert rte_370: mem_access: \valid_read(&vT->__anonCompField7.v[2]); */
  /*@ assert rte_371: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField7.v[2], vT->__anonCompField7.v[2])); */
  self->__anonCompField7.v[2] /= vT->__anonCompField7.v[2];
  /*@ assert rte_372: mem_access: \valid(&self->__anonCompField7.v[3]); */
  /*@ assert rte_373: mem_access: \valid_read(&self->__anonCompField7.v[3]); */
  /*@ assert rte_374: mem_access: \valid_read(&vT->__anonCompField7.v[3]); */
  /*@ assert rte_375: is_nan_or_infinite: \is_finite(\div_float(self->__anonCompField7.v[3], vT->__anonCompField7.v[3])); */
  self->__anonCompField7.v[3] /= vT->__anonCompField7.v[3];
  return self;
}

float vector4_dot_product(struct vector4 const *self,
                          struct vector4 const *vT)
{
  float __retres;
  /*@ assert rte_376: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_377: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_378: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.x, vT->__anonCompField7.__anonCompField6.x)); */
  /*@ assert rte_379: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_380: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_381: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.y, vT->__anonCompField7.__anonCompField6.y)); */
  /*@ assert rte_382: is_nan_or_infinite: \is_finite(\add_float(\mul_float(self->__anonCompField7.__anonCompField6.x, vT->__anonCompField7.__anonCompField6.x), \mul_float(self->__anonCompField7.__anonCompField6.y, vT->__anonCompField7.__anonCompField6.y))); */
  /*@ assert rte_383: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_384: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_385: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.z, vT->__anonCompField7.__anonCompField6.z)); */
  /*@ assert rte_386: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(self->__anonCompField7.__anonCompField6.x, vT->__anonCompField7.__anonCompField6.x), \mul_float(self->__anonCompField7.__anonCompField6.y, vT->__anonCompField7.__anonCompField6.y)), \mul_float(self->__anonCompField7.__anonCompField6.z, vT->__anonCompField7.__anonCompField6.z))); */
  /*@ assert rte_387: mem_access: \valid_read(&self->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_388: mem_access: \valid_read(&vT->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_389: is_nan_or_infinite: \is_finite(\mul_float(self->__anonCompField7.__anonCompField6.w, vT->__anonCompField7.__anonCompField6.w)); */
  /*@ assert rte_390: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(self->__anonCompField7.__anonCompField6.x, vT->__anonCompField7.__anonCompField6.x), \mul_float(self->__anonCompField7.__anonCompField6.y, vT->__anonCompField7.__anonCompField6.y)), \mul_float(self->__anonCompField7.__anonCompField6.z, vT->__anonCompField7.__anonCompField6.z)), \mul_float(self->__anonCompField7.__anonCompField6.w, vT->__anonCompField7.__anonCompField6.w))); */
  __retres = ((self->__anonCompField7.__anonCompField6.x * vT->__anonCompField7.__anonCompField6.x + 
               self->__anonCompField7.__anonCompField6.y * vT->__anonCompField7.__anonCompField6.y) + 
              self->__anonCompField7.__anonCompField6.z * vT->__anonCompField7.__anonCompField6.z) + 
             self->__anonCompField7.__anonCompField6.w * vT->__anonCompField7.__anonCompField6.w;
  return __retres;
}

struct vector4 *vector4_cross_product(struct vector4 *vR,
                                      struct vector4 const *vT1,
                                      struct vector4 const *vT2)
{
  /*@ assert rte_391: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_392: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_393: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_394: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.y, vT2->__anonCompField7.__anonCompField6.z)); */
  /*@ assert rte_395: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_396: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_397: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.z, vT2->__anonCompField7.__anonCompField6.y)); */
  /*@ assert rte_398: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.y, vT2->__anonCompField7.__anonCompField6.z), \mul_float(vT1->__anonCompField7.__anonCompField6.z, vT2->__anonCompField7.__anonCompField6.y))); */
  vR->__anonCompField7.__anonCompField6.x = vT1->__anonCompField7.__anonCompField6.y * vT2->__anonCompField7.__anonCompField6.z - 
                                            vT1->__anonCompField7.__anonCompField6.z * vT2->__anonCompField7.__anonCompField6.y;
  /*@ assert rte_399: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_400: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_401: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_402: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.z, vT2->__anonCompField7.__anonCompField6.x)); */
  /*@ assert rte_403: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_404: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_405: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.x, vT2->__anonCompField7.__anonCompField6.z)); */
  /*@ assert rte_406: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.z, vT2->__anonCompField7.__anonCompField6.x), \mul_float(vT1->__anonCompField7.__anonCompField6.x, vT2->__anonCompField7.__anonCompField6.z))); */
  vR->__anonCompField7.__anonCompField6.y = vT1->__anonCompField7.__anonCompField6.z * vT2->__anonCompField7.__anonCompField6.x - 
                                            vT1->__anonCompField7.__anonCompField6.x * vT2->__anonCompField7.__anonCompField6.z;
  /*@ assert rte_407: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.z); */
  /*@ assert rte_408: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_409: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_410: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.x, vT2->__anonCompField7.__anonCompField6.y)); */
  /*@ assert rte_411: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.y); */
  /*@ assert rte_412: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.x); */
  /*@ assert rte_413: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.y, vT2->__anonCompField7.__anonCompField6.x)); */
  /*@ assert rte_414: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.x, vT2->__anonCompField7.__anonCompField6.y), \mul_float(vT1->__anonCompField7.__anonCompField6.y, vT2->__anonCompField7.__anonCompField6.x))); */
  vR->__anonCompField7.__anonCompField6.z = vT1->__anonCompField7.__anonCompField6.x * vT2->__anonCompField7.__anonCompField6.y - 
                                            vT1->__anonCompField7.__anonCompField6.y * vT2->__anonCompField7.__anonCompField6.x;
  /*@ assert rte_415: mem_access: \valid(&vR->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_416: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_417: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_418: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.w, vT2->__anonCompField7.__anonCompField6.w)); */
  /*@ assert rte_419: mem_access: \valid_read(&vT1->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_420: mem_access: \valid_read(&vT2->__anonCompField7.__anonCompField6.w); */
  /*@ assert rte_421: is_nan_or_infinite: \is_finite(\mul_float(vT1->__anonCompField7.__anonCompField6.w, vT2->__anonCompField7.__anonCompField6.w)); */
  /*@ assert rte_422: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(vT1->__anonCompField7.__anonCompField6.w, vT2->__anonCompField7.__anonCompField6.w), \mul_float(vT1->__anonCompField7.__anonCompField6.w, vT2->__anonCompField7.__anonCompField6.w))); */
  vR->__anonCompField7.__anonCompField6.w = vT1->__anonCompField7.__anonCompField6.w * vT2->__anonCompField7.__anonCompField6.w - 
                                            vT1->__anonCompField7.__anonCompField6.w * vT2->__anonCompField7.__anonCompField6.w;
  return vR;
}



