#include "assert.h"
#include "ctype.h"
#include "errno.h"
#include "locale.h"
#include "math.h"
#include "signal.h"
#include "stdarg.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "strings.h"
#include "time.h"
#include "wchar.h"

typedef float CGL_float;
typedef size_t CGL_sizei;
typedef _Bool CGL_bool;
typedef void CGL_void;
struct CGL_list {
   size_t size ;
   size_t capacity ;
   size_t item_size ;
   CGL_float increase_factor ;
   void *data ;
};
typedef struct CGL_list CGL_list;
CGL_void CGL_list_set_increase_factor(CGL_list *list,
                                      CGL_float increase_factor);
CGL_float CGL_list_get_increase_factor(CGL_list *list);
CGL_sizei CGL_list_get_item_size(CGL_list *list);
CGL_sizei CGL_list_get_size(CGL_list *list);
CGL_sizei CGL_list_get_capacity(CGL_list *list);
CGL_bool CGL_list_is_empty(CGL_list *list);
CGL_void CGL_list_clear(CGL_list *list);
CGL_float CGL_utils_sigmoid_derivative(CGL_float x);
CGL_float CGL_utils_relu(CGL_float x);
CGL_float CGL_utils_relu_derivative(CGL_float x);
CGL_float CGL_utils_tanh_derivative(CGL_float x);
CGL_float CGL_utils_step(CGL_float x);
CGL_float CGL_utils_step_derivative(CGL_float x);
CGL_float CGL_utils_relu_leaky(CGL_float x);
CGL_float CGL_utils_relu_leaky_derivative(CGL_float x);
CGL_float CGL_utils_relu_smooth(CGL_float x);
CGL_float CGL_utils_relu_smooth_derivative(CGL_float x);

/*@
requires \valid(list);
assigns list->increase_factor;
ensures list->increase_factor == increase_factor;
*/
CGL_void CGL_list_set_increase_factor(CGL_list *list,
                                      CGL_float increase_factor)
{
  /*@ assert rte_7: mem_access: \valid(&list->increase_factor); */
  list->increase_factor = increase_factor;
  return;
}

/*@
requires \valid_read(list);
assigns \nothing;
ensures \result == list->increase_factor;
*/
CGL_float CGL_list_get_increase_factor(CGL_list *list)
{
  CGL_float __retres;
  /*@ assert rte_3: mem_access: \valid_read(&list->increase_factor); */
  __retres = list->increase_factor;
  return __retres;
}

/*@
requires \valid_read(list);
assigns \nothing;
ensures \result == list->item_size;
*/
CGL_sizei CGL_list_get_item_size(CGL_list *list)
{
  CGL_sizei __retres;
  /*@ assert rte_4: mem_access: \valid_read(&list->item_size); */
  __retres = list->item_size;
  return __retres;
}

/*@
requires \valid_read(list);
assigns \nothing;
ensures \result == list->size;
*/
CGL_sizei CGL_list_get_size(CGL_list *list)
{
  CGL_sizei __retres;
  /*@ assert rte_5: mem_access: \valid_read(&list->size); */
  __retres = list->size;
  return __retres;
}

/*@
requires \valid_read(list);
assigns \nothing;
ensures \result == list->capacity;
*/
CGL_sizei CGL_list_get_capacity(CGL_list *list)
{
  CGL_sizei __retres;
  /*@ assert rte_2: mem_access: \valid_read(&list->capacity); */
  __retres = list->capacity;
  return __retres;
}

/*@
requires \valid_read(list);
assigns \nothing;
ensures \result == 0 || \result == 1;
ensures (\result == 1) <==> (list->size == 0);
ensures (\result == 0) <==> (list->size != 0);
*/
CGL_bool CGL_list_is_empty(CGL_list *list)
{
  CGL_bool __retres;
  /*@ assert rte_6: mem_access: \valid_read(&list->size); */
  __retres = (CGL_bool)(list->size == (size_t)0);
  return __retres;
}

/*@
requires \valid(list);
assigns list->size;
ensures list->size == 0;
*/
CGL_void CGL_list_clear(CGL_list *list)
{
  /*@ assert rte_1: mem_access: \valid(&list->size); */
  list->size = (size_t)0;
  return;
}

/*@
requires \is_finite(x);
assigns \nothing;
ensures \is_finite(\result);
ensures \result == x * (1.0f - x);
*/
CGL_float CGL_utils_sigmoid_derivative(CGL_float x)
{
  CGL_float __retres;
  /*@ assert rte_12: is_nan_or_infinite: \is_finite(\sub_float(1.0f, x)); */
  /*@ assert
        rte_13: is_nan_or_infinite: \is_finite(\mul_float(x, \sub_float(1.0f, x)));
  */
  __retres = x * (1.0f - x);
  return __retres;
}

/*@
assigns \nothing;
ensures x > 0.0f ==> \result == x;
ensures x <= 0.0f ==> \result == 0.0f;
ensures \result >= 0.0f;
*/
CGL_float CGL_utils_relu(CGL_float x)
{
  CGL_float tmp;
  if (x > 0.0f) tmp = x; else tmp = 0.0f;
  return tmp;
}

/*@
assigns \nothing;
ensures (x > 0.0f) ==> (\result == 1.0f);
ensures (x <= 0.0f) ==> (\result == 0.0f);
ensures \result == 0.0f || \result == 1.0f;
*/
CGL_float CGL_utils_relu_derivative(CGL_float x)
{
  float tmp;
  if (x > 0.0f) tmp = 1.0f; else tmp = 0.0f;
  return tmp;
}

/*@
requires \is_finite(x);
requires -0x1p63f <= x <= 0x1p63f;
assigns \nothing;
ensures \is_finite(\result);
ensures \result == 1.0f - x * x;
*/
CGL_float CGL_utils_tanh_derivative(CGL_float x)
{
  CGL_float __retres;
  /*@ assert rte_14: is_nan_or_infinite: \is_finite(\mul_float(x, x)); */
  /*@ assert
        rte_15: is_nan_or_infinite: \is_finite(\sub_float(1.0f, \mul_float(x, x)));
  */
  __retres = 1.0f - x * x;
  return __retres;
}

/*@
assigns \nothing;
ensures (x > 0.0f) ==> (\result == 1.0f);
ensures (x <= 0.0f) ==> (\result == 0.0f);
ensures \result == 0.0f || \result == 1.0f;
*/
CGL_float CGL_utils_step(CGL_float x)
{
  float tmp;
  if (x > 0.0f) tmp = 1.0f; else tmp = 0.0f;
  return tmp;
}

/*@
assigns \nothing;
ensures \result == 0.0f;
*/
CGL_float CGL_utils_step_derivative(CGL_float x)
{
  CGL_float __retres;
  __retres = 0.0f;
  return __retres;
}

/*@
requires \is_finite(x);
assigns \nothing;
ensures \is_finite(\result);
ensures x > 0.0f ==> \result == x;
ensures x <= 0.0f ==> \result == 0.01f * x;
*/
CGL_float CGL_utils_relu_leaky(CGL_float x)
{
  CGL_float tmp;
  if (x > 0.0f) tmp = x;
  else 
    /*@ assert rte_8: is_nan_or_infinite: \is_finite(\mul_float(0.01f, x)); */
    tmp = 0.01f * x;
  return tmp;
}

/*@
assigns \nothing;
ensures (x > 0.0f) ==> (\result == 1.0f);
ensures (x <= 0.0f) ==> (\result == 0.01f);
ensures \result == 1.0f || \result == 0.01f;
ensures \result > 0.0f;
*/
CGL_float CGL_utils_relu_leaky_derivative(CGL_float x)
{
  float tmp;
  if (x > 0.0f) tmp = 1.0f; else tmp = 0.01f;
  return tmp;
}

/*@
requires \is_finite(x);
assigns \nothing;
ensures \is_finite(\result);
ensures x > 0.0f ==> \result == x;
ensures x <= 0.0f ==> \result == (0.01f * x) * x;
ensures \result >= 0.0f;
*/
CGL_float CGL_utils_relu_smooth(CGL_float x)
{
  CGL_float tmp;
  if (x > 0.0f) tmp = x;
  else 
    /*@ assert rte_9: is_nan_or_infinite: \is_finite(\mul_float(0.01f, x)); */
    /*@ assert
        rte_10: is_nan_or_infinite:
          \is_finite(\mul_float(\mul_float(0.01f, x), x));
    */
    tmp = (0.01f * x) * x;
  return tmp;
}

/*@
requires \is_finite(x);
assigns \nothing;
ensures x > 0.0f ==> \result == 1.0f;
ensures x <= 0.0f ==> \result == 0.02f * x;
ensures \is_finite(\result);
*/
CGL_float CGL_utils_relu_smooth_derivative(CGL_float x)
{
  float tmp;
  if (x > 0.0f) tmp = 1.0f;
  else 
    /*@ assert rte_11: is_nan_or_infinite: \is_finite(\mul_float(0.02f, x)); */
    tmp = 0.02f * x;
  return tmp;
}