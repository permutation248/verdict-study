#include "assert.h"
#include "stddef.h"
#include "stdint.h"

struct sshape_range {
   void const *ptr ;
   size_t size ;
};
typedef struct sshape_range sshape_range;
struct sshape_mat4_t {
   float m[4][4] ;
};
typedef struct sshape_mat4_t sshape_mat4_t;
struct sshape_vertex_t {
   float x ;
   float y ;
   float z ;
   uint32_t normal ;
   uint16_t u ;
   uint16_t v ;
   uint32_t color ;
};
typedef struct sshape_vertex_t sshape_vertex_t;
struct sshape_element_range_t {
   int base_element ;
   int num_elements ;
};
typedef struct sshape_element_range_t sshape_element_range_t;
struct sshape_sizes_item_t {
   uint32_t num ;
   uint32_t size ;
};
typedef struct sshape_sizes_item_t sshape_sizes_item_t;
struct sshape_sizes_t {
   sshape_sizes_item_t vertices ;
   sshape_sizes_item_t indices ;
};
typedef struct sshape_sizes_t sshape_sizes_t;
struct sshape_buffer_item_t {
   sshape_range buffer ;
   size_t data_size ;
   size_t shape_offset ;
};
typedef struct sshape_buffer_item_t sshape_buffer_item_t;
struct sshape_buffer_t {
   _Bool valid ;
   sshape_buffer_item_t vertices ;
   sshape_buffer_item_t indices ;
};
typedef struct sshape_buffer_t sshape_buffer_t;
struct _sshape_vec4_t {
   float x ;
   float y ;
   float z ;
   float w ;
};
typedef struct _sshape_vec4_t _sshape_vec4_t;
struct _sshape_vec2_t {
   float x ;
   float y ;
};
typedef struct _sshape_vec2_t _sshape_vec2_t;
struct __anonstruct_pack_inputs_1 {
   uint8_t x ;
   uint8_t y ;
   uint8_t z ;
   uint8_t w ;
   uint32_t expected ;
};
struct __anonstruct_fpack_inputs_2 {
   float x ;
   float y ;
   float z ;
   float w ;
   uint32_t expected ;
};
struct __anonstruct_bpack_inputs_3 {
   float x ;
   float y ;
   float z ;
   float w ;
   uint32_t expected ;
};
struct __anonstruct_vec4_inputs_4 {
   float x ;
   float y ;
   float z ;
   float w ;
};
struct __anonstruct_vec2_inputs_5 {
   float x ;
   float y ;
};
struct __anonstruct_sphere_verts_inputs_6 {
   uint32_t slices ;
   uint32_t stacks ;
   uint32_t expected ;
};
struct __anonstruct_cylinder_verts_inputs_7 {
   uint32_t slices ;
   uint32_t stacks ;
   uint32_t expected ;
};
struct __anonstruct_torus_verts_inputs_8 {
   uint32_t sides ;
   uint32_t rings ;
   uint32_t expected ;
};
struct __anonstruct_color4b_inputs_9 {
   uint8_t r ;
   uint8_t g ;
   uint8_t b ;
   uint8_t a ;
   uint32_t expected ;
};
struct __anonstruct_color3b_inputs_10 {
   uint8_t r ;
   uint8_t g ;
   uint8_t b ;
   uint32_t expected ;
};
struct __anonstruct_sphere_sizes_inputs_11 {
   uint32_t slices ;
   uint32_t stacks ;
};
struct __anonstruct_cylinder_sizes_inputs_12 {
   uint32_t slices ;
   uint32_t stacks ;
};
struct __anonstruct_torus_sizes_inputs_13 {
   uint32_t sides ;
   uint32_t rings ;
};
float _sshape_clamp(float v);
uint32_t _sshape_pack_ub4_ubyte4n(uint8_t x, uint8_t y, uint8_t z, uint8_t w);
uint32_t _sshape_pack_f4_ubyte4n(float x, float y, float z, float w);
uint32_t _sshape_pack_f4_byte4n(float x, float y, float z, float w);
uint16_t _sshape_pack_f_ushortn(float x);
_sshape_vec4_t _sshape_vec4(float x, float y, float z, float w);
_sshape_vec2_t _sshape_vec2(float x, float y);
_Bool _sshape_mat4_isnull(sshape_mat4_t const *m);
sshape_mat4_t _sshape_mat4_identity(void);
_sshape_vec4_t _sshape_mat4_mul(sshape_mat4_t const *m, _sshape_vec4_t v);
uint32_t _sshape_plane_num_vertices(uint32_t tiles);
uint32_t _sshape_plane_num_indices(uint32_t tiles);
uint32_t _sshape_box_num_vertices(uint32_t tiles);
uint32_t _sshape_box_num_indices(uint32_t tiles);
uint32_t _sshape_sphere_num_vertices(uint32_t slices, uint32_t stacks);
uint32_t _sshape_sphere_num_indices(uint32_t slices, uint32_t stacks);
uint32_t _sshape_cylinder_num_vertices(uint32_t slices, uint32_t stacks);
uint32_t _sshape_cylinder_num_indices(uint32_t slices, uint32_t stacks);
uint32_t _sshape_torus_num_vertices(uint32_t sides, uint32_t rings);
uint32_t _sshape_torus_num_indices(uint32_t sides, uint32_t rings);
_Bool _sshape_validate_buffer_item(sshape_buffer_item_t const *item,
                                   uint32_t build_size);
_Bool _sshape_validate_buffer(sshape_buffer_t const *buf,
                              uint32_t num_vertices, uint32_t num_indices);
void _sshape_advance_offset(sshape_buffer_item_t *item);
uint16_t _sshape_base_index(sshape_buffer_t const *buf);
uint32_t sshape_color_4b(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t sshape_color_3b(uint8_t r, uint8_t g, uint8_t b);
sshape_sizes_t sshape_plane_sizes(uint32_t tiles);
sshape_sizes_t sshape_box_sizes(uint32_t tiles);
sshape_sizes_t sshape_sphere_sizes(uint32_t slices, uint32_t stacks);
sshape_sizes_t sshape_cylinder_sizes(uint32_t slices, uint32_t stacks);
sshape_sizes_t sshape_torus_sizes(uint32_t sides, uint32_t rings);
sshape_element_range_t sshape_element_range(sshape_buffer_t const *buf);
int main(void);

/*@
assigns \nothing;
ensures \result >= 0.0f;
ensures \result <= 1.0f;
ensures (v >= 0.0f && v <= 1.0f) ==> \result == v;
ensures (v < 0.0f) ==> \result == 0.0f;
ensures (v > 1.0f) ==> \result == 1.0f;
*/
float _sshape_clamp(float v)
{
  float __retres;
  if (v < 0.0f) {
    __retres = 0.0f;
    goto return_label;
  }
  else 
    if (v > 1.0f) {
      __retres = 1.0f;
      goto return_label;
    }
    else {
      __retres = v;
      goto return_label;
    }
  return_label: return __retres;
}

/*@
assigns \nothing;
ensures \result == (((uint32_t)w * (uint32_t)0x1000000 + (uint32_t)z * (uint32_t)0x10000) + (uint32_t)y * (uint32_t)0x100) + (uint32_t)x;
*/
uint32_t _sshape_pack_ub4_ubyte4n(uint8_t x, uint8_t y, uint8_t z, uint8_t w)
{
  uint32_t __retres;
  __retres = (((uint32_t)w * (uint32_t)0x1000000 + (uint32_t)z * (uint32_t)0x10000) + 
              (uint32_t)y * (uint32_t)0x100) + (uint32_t)x;
  return __retres;
}

/*@
requires \is_finite(x);
requires \is_finite(y);
requires \is_finite(z);
requires \is_finite(w);
assigns \nothing;
ensures \result <= 0xFFFFFFFF;
*/
uint32_t _sshape_pack_f4_ubyte4n(float x, float y, float z, float w)
{
  uint32_t tmp;
  /*@ assert rte_14: is_nan_or_infinite: \is_finite(\mul_float(x, 255.0f)); */
  uint8_t x8 = (uint8_t)(x * 255.0f);
  /*@ assert rte_15: is_nan_or_infinite: \is_finite(\mul_float(y, 255.0f)); */
  uint8_t y8 = (uint8_t)(y * 255.0f);
  /*@ assert rte_16: is_nan_or_infinite: \is_finite(\mul_float(z, 255.0f)); */
  uint8_t z8 = (uint8_t)(z * 255.0f);
  /*@ assert rte_17: is_nan_or_infinite: \is_finite(\mul_float(w, 255.0f)); */
  uint8_t w8 = (uint8_t)(w * 255.0f);
  tmp = _sshape_pack_ub4_ubyte4n(x8,y8,z8,w8);
  return tmp;
}

/*@
requires \is_finite(x);
requires \is_finite(y);
requires \is_finite(z);
requires \is_finite(w);
assigns \nothing;
*/
uint32_t _sshape_pack_f4_byte4n(float x, float y, float z, float w)
{
  uint32_t tmp;
  /*@ assert rte_18: is_nan_or_infinite: \is_finite(\mul_float(x, 127.0f)); */
  int8_t x8 = (int8_t)(x * 127.0f);
  /*@ assert rte_19: is_nan_or_infinite: \is_finite(\mul_float(y, 127.0f)); */
  int8_t y8 = (int8_t)(y * 127.0f);
  /*@ assert rte_20: is_nan_or_infinite: \is_finite(\mul_float(z, 127.0f)); */
  int8_t z8 = (int8_t)(z * 127.0f);
  /*@ assert rte_21: is_nan_or_infinite: \is_finite(\mul_float(w, 127.0f)); */
  int8_t w8 = (int8_t)(w * 127.0f);
  tmp = _sshape_pack_ub4_ubyte4n((uint8_t)x8,(uint8_t)y8,(uint8_t)z8,
                                 (uint8_t)w8);
  return tmp;
}

/*@
requires \is_finite(x);
requires 0.0f <= x <= 1.0f;
assigns \nothing;
ensures \result <= 65535;
*/
uint16_t _sshape_pack_f_ushortn(float x)
{
  uint16_t __retres;
  /*@ assert rte_22: is_nan_or_infinite: \is_finite(\mul_float(x, 65535.0f)); */
  __retres = (uint16_t)(x * 65535.0f);
  return __retres;
}

/*@
assigns \nothing;
ensures \result.x == x;
ensures \result.y == y;
ensures \result.z == z;
ensures \result.w == w;
*/
_sshape_vec4_t _sshape_vec4(float x, float y, float z, float w)
{
  _sshape_vec4_t v = {.x = x, .y = y, .z = z, .w = w};
  return v;
}

/*@
assigns \nothing;
ensures \result.x == x;
ensures \result.y == y;
*/
_sshape_vec2_t _sshape_vec2(float x, float y)
{
  _sshape_vec2_t v = {.x = x, .y = y};
  return v;
}

/*@
requires \valid_read(m);
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
_Bool _sshape_mat4_isnull(sshape_mat4_t const *m)
{
  _Bool __retres;
  {
    int y = 0;
    /*@
    loop invariant 0 <= y <= 4;
    loop assigns y;
    loop variant 4 - y;
    */
    while (y < 4) {
      {
        int x = 0;
        /*@
        loop invariant 0 <= x <= 4;
        loop invariant 0 <= y < 4;
        loop assigns x;
        loop variant 4 - x;
        */
        while (x < 4) {
          /*@ assert rte_1: index_bound: 0 ≤ y; */
          /*@ assert rte_2: index_bound: y < 4; */
          /*@ assert rte_3: index_bound: 0 ≤ x; */
          /*@ assert rte_4: index_bound: x < 4; */
          /*@ assert rte_5: mem_access: \valid_read(&m->m[y][x]); */
          if (0.0f != m->m[y][x]) {
            __retres = (_Bool)0;
            goto return_label;
          }
          /*@ assert rte_6: signed_overflow: x + 1 ≤ 2147483647; */
          x ++;
        }
      }
      /*@ assert rte_7: signed_overflow: y + 1 ≤ 2147483647; */
      y ++;
    }
  }
  __retres = (_Bool)1;
  return_label: return __retres;
}

/*@
assigns \nothing;
ensures \result.m[0][0] == 1.0f;
ensures \result.m[1][1] == 1.0f;
ensures \result.m[2][2] == 1.0f;
ensures \result.m[3][3] == 1.0f;
ensures \result.m[1][0] == 0.0f;
ensures \result.m[1][2] == 0.0f;
ensures \result.m[1][3] == 0.0f;
ensures \result.m[2][0] == 0.0f;
ensures \result.m[2][1] == 0.0f;
ensures \result.m[2][3] == 0.0f;
ensures \result.m[3][0] == 0.0f;
ensures \result.m[3][1] == 0.0f;
ensures \result.m[3][2] == 0.0f;
*/
sshape_mat4_t _sshape_mat4_identity(void)
{
  sshape_mat4_t m =
    {.m = {{1.0f, 0.0f, 0.0f, 0.0f},
           {0.0f, 1.0f, 0.0f, 0.0f},
           {0.0f, 0.0f, 1.0f, 0.0f},
           {0.0f, 0.0f, 0.0f, 1.0f}}};
  return m;
}

/*@
requires m != \null;
requires \valid_read(&m->m[0][0]) && \valid_read(&m->m[0][1]) && \valid_read(&m->m[0][2]) && \valid_read(&m->m[0][3]);
requires \valid_read(&m->m[1][0]) && \valid_read(&m->m[1][1]) && \valid_read(&m->m[1][2]) && \valid_read(&m->m[1][3]);
requires \valid_read(&m->m[2][0]) && \valid_read(&m->m[2][1]) && \valid_read(&m->m[2][2]) && \valid_read(&m->m[2][3]);
requires \valid_read(&m->m[3][0]) && \valid_read(&m->m[3][1]) && \valid_read(&m->m[3][2]) && \valid_read(&m->m[3][3]);
requires \is_finite(m->m[0][0]) && \is_finite(m->m[0][1]) && \is_finite(m->m[0][2]) && \is_finite(m->m[0][3]);
requires \is_finite(m->m[1][0]) && \is_finite(m->m[1][1]) && \is_finite(m->m[1][2]) && \is_finite(m->m[1][3]);
requires \is_finite(m->m[2][0]) && \is_finite(m->m[2][1]) && \is_finite(m->m[2][2]) && \is_finite(m->m[2][3]);
requires \is_finite(m->m[3][0]) && \is_finite(m->m[3][1]) && \is_finite(m->m[3][2]) && \is_finite(m->m[3][3]);
requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w);
assigns \nothing;
ensures \is_finite(\result.x) && \is_finite(\result.y) && \is_finite(\result.z) && \is_finite(\result.w);
*/
_sshape_vec4_t _sshape_mat4_mul(sshape_mat4_t const *m, _sshape_vec4_t v)
{
  /*@ assert rte_23: mem_access: \valid_read(&m->m[0][0]); */
  /*@ assert
        rte_24: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][0], v.x));
  */
  /*@ assert rte_25: mem_access: \valid_read(&m->m[1][0]); */
  /*@ assert
        rte_26: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][0], v.y));
  */
  /*@ assert
        rte_27: is_nan_or_infinite:
        \is_finite(\add_float(\mul_float(m->m[0][0], v.x),
                             \mul_float(m->m[1][0], v.y)));
  */
  /*@ assert rte_28: mem_access: \valid_read(&m->m[2][0]); */
  /*@ assert
        rte_29: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][0], v.z));
  */
  /*@ assert
        rte_30: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\mul_float(m->m[0][0], v.x),
                                        \mul_float(m->m[1][0], v.y)),
                             \mul_float(m->m[2][0], v.z)));
  */
  /*@ assert rte_31: mem_access: \valid_read(&m->m[3][0]); */
  /*@ assert
        rte_32: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][0], v.w));
  */
  /*@ assert
        rte_33: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][0],
                                                              v.x),
                                                   \mul_float(m->m[1][0],
                                                             v.y)),
                                        \mul_float(m->m[2][0], v.z)),
                             \mul_float(m->m[3][0], v.w)));
  */
  /*@ assert rte_34: mem_access: \valid_read(&m->m[0][1]); */
  /*@ assert
        rte_35: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][1], v.x));
  */
  /*@ assert rte_36: mem_access: \valid_read(&m->m[1][1]); */
  /*@ assert
        rte_37: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][1], v.y));
  */
  /*@ assert
        rte_38: is_nan_or_infinite:
        \is_finite(\add_float(\mul_float(m->m[0][1], v.x),
                             \mul_float(m->m[1][1], v.y)));
  */
  /*@ assert rte_39: mem_access: \valid_read(&m->m[2][1]); */
  /*@ assert
        rte_40: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][1], v.z));
  */
  /*@ assert
        rte_41: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\mul_float(m->m[0][1], v.x),
                                        \mul_float(m->m[1][1], v.y)),
                             \mul_float(m->m[2][1], v.z)));
  */
  /*@ assert rte_42: mem_access: \valid_read(&m->m[3][1]); */
  /*@ assert
        rte_43: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][1], v.w));
  */
  /*@ assert
        rte_44: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][1],
                                                              v.x),
                                                   \mul_float(m->m[1][1],
                                                             v.y)),
                                        \mul_float(m->m[2][1], v.z)),
                             \mul_float(m->m[3][1], v.w)));
  */
  /*@ assert rte_45: mem_access: \valid_read(&m->m[0][2]); */
  /*@ assert
        rte_46: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][2], v.x));
  */
  /*@ assert rte_47: mem_access: \valid_read(&m->m[1][2]); */
  /*@ assert
        rte_48: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][2], v.y));
  */
  /*@ assert
        rte_49: is_nan_or_infinite:
        \is_finite(\add_float(\mul_float(m->m[0][2], v.x),
                             \mul_float(m->m[1][2], v.y)));
  */
  /*@ assert rte_50: mem_access: \valid_read(&m->m[2][2]); */
  /*@ assert
        rte_51: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][2], v.z));
  */
  /*@ assert
        rte_52: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\mul_float(m->m[0][2], v.x),
                                        \mul_float(m->m[1][2], v.y)),
                             \mul_float(m->m[2][2], v.z)));
  */
  /*@ assert rte_53: mem_access: \valid_read(&m->m[3][2]); */
  /*@ assert
        rte_54: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][2], v.w));
  */
  /*@ assert
        rte_55: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][2],
                                                              v.x),
                                                   \mul_float(m->m[1][2],
                                                             v.y)),
                                        \mul_float(m->m[2][2], v.z)),
                             \mul_float(m->m[3][2], v.w)));
  */
  /*@ assert rte_56: mem_access: \valid_read(&m->m[0][3]); */
  /*@ assert
        rte_57: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][3], v.x));
  */
  /*@ assert rte_58: mem_access: \valid_read(&m->m[1][3]); */
  /*@ assert
        rte_59: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][3], v.y));
  */
  /*@ assert
        rte_60: is_nan_or_infinite:
        \is_finite(\add_float(\mul_float(m->m[0][3], v.x),
                             \mul_float(m->m[1][3], v.y)));
  */
  /*@ assert rte_61: mem_access: \valid_read(&m->m[2][3]); */
  /*@ assert
        rte_62: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][3], v.z));
  */
  /*@ assert
        rte_63: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\mul_float(m->m[0][3], v.x),
                                        \mul_float(m->m[1][3], v.y)),
                             \mul_float(m->m[2][3], v.z)));
  */
  /*@ assert rte_64: mem_access: \valid_read(&m->m[3][3]); */
  /*@ assert
        rte_65: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][3], v.w));
  */
  /*@ assert
        rte_66: is_nan_or_infinite:
        \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][3],
                                                              v.x),
                                                   \mul_float(m->m[1][3],
                                                             v.y)),
                                        \mul_float(m->m[2][3], v.z)),
                             \mul_float(m->m[3][3], v.w)));
  */
  _sshape_vec4_t res =
    {.x = ((m->m[0][0] * v.x + m->m[1][0] * v.y) + m->m[2][0] * v.z) + 
          m->m[3][0] * v.w,
     .y = ((m->m[0][1] * v.x + m->m[1][1] * v.y) + m->m[2][1] * v.z) + 
          m->m[3][1] * v.w,
     .z = ((m->m[0][2] * v.x + m->m[1][2] * v.y) + m->m[2][2] * v.z) + 
          m->m[3][2] * v.w,
     .w = ((m->m[0][3] * v.x + m->m[1][3] * v.y) + m->m[2][3] * v.z) + 
          m->m[3][3] * v.w};
  return res;
}

/*@
requires tiles <= 65534;
assigns \nothing;
ensures \result == (tiles + 1) * (tiles + 1);
ensures \result >= 1;
*/
uint32_t _sshape_plane_num_vertices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = (tiles + (uint32_t)1) * (tiles + (uint32_t)1);
  return __retres;
}

/*@
requires tiles <= 26754;
assigns \nothing;
ensures \result == tiles * tiles * 6;
*/
uint32_t _sshape_plane_num_indices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = ((tiles * tiles) * (uint32_t)2) * (uint32_t)3;
  return __retres;
}

/*@
requires tiles <= 26752;
assigns \nothing;
ensures \result == ((tiles + 1) * (tiles + 1)) * 6;
ensures \result >= 6;
*/
uint32_t _sshape_box_num_vertices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = ((tiles + (uint32_t)1) * (tiles + (uint32_t)1)) * (uint32_t)6;
  return __retres;
}

/*@
requires tiles <= 10922;
assigns \nothing;
ensures \result == tiles * tiles * 36;
ensures tiles >= 1 ==> \result >= 36;
*/
uint32_t _sshape_box_num_indices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = (((tiles * tiles) * (uint32_t)2) * (uint32_t)6) * (uint32_t)3;
  return __retres;
}

/*@
requires slices <= 0xFFFFFFFE;
requires stacks <= 0xFFFFFFFE;
requires (slices + 1) * (stacks + 1) <= 0xFFFFFFFF;
assigns \nothing;
ensures \result == (slices + 1) * (stacks + 1);
ensures \result >= 1;
*/
uint32_t _sshape_sphere_num_vertices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (slices + (uint32_t)1) * (stacks + (uint32_t)1);
  return __retres;
}

/*@
assigns \nothing;
ensures (slices == 0) ==> \result == 0;
ensures (stacks == 1) ==> \result == 0;
ensures (stacks >= 1 && 6 * (integer)slices * ((integer)stacks - 1) <= 0xFFFFFFFF) ==> \result == 6 * slices * (stacks - 1);
*/
uint32_t _sshape_sphere_num_indices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (((uint32_t)2 * slices) * stacks - (uint32_t)2 * slices) * (uint32_t)3;
  return __retres;
}

/*@
requires slices <= 0xFFFFFFFE;
requires stacks <= 0xFFFFFFFA;
requires (slices + 1) * (stacks + 5) <= 0xFFFFFFFF;
assigns \nothing;
ensures \result == (slices + 1) * (stacks + 5);
*/
uint32_t _sshape_cylinder_num_vertices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (slices + (uint32_t)1) * (stacks + (uint32_t)5);
  return __retres;
}

/*@
assigns \nothing;
ensures 6 * (uint64_t)slices * ((uint64_t)stacks + 1) <= 0xFFFFFFFF ==> \result == 6 * slices * (stacks + 1);
ensures slices == 0 ==> \result == 0;
*/
uint32_t _sshape_cylinder_num_indices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (((uint32_t)2 * slices) * stacks + (uint32_t)2 * slices) * (uint32_t)3;
  return __retres;
}

/*@
requires (uint64_t)(sides + 1) * (uint64_t)(rings + 1) <= UINT32_MAX;
assigns \nothing;
ensures \result == (sides + (uint32_t)1) * (rings + (uint32_t)1);
*/
uint32_t _sshape_torus_num_vertices(uint32_t sides, uint32_t rings)
{
  uint32_t __retres;
  __retres = (sides + (uint32_t)1) * (rings + (uint32_t)1);
  return __retres;
}

/*@
requires sides * rings <= 715827882;
assigns \nothing;
ensures \result == sides * rings * 6;
*/
uint32_t _sshape_torus_num_indices(uint32_t sides, uint32_t rings)
{
  uint32_t __retres;
  __retres = ((sides * rings) * (uint32_t)2) * (uint32_t)3;
  return __retres;
}

/*@
requires \valid_read(item);
assigns \nothing;
ensures \result == (_Bool)0 || \result == (_Bool)1;
ensures (\result == (_Bool)1) ==> (item->buffer.ptr != (void const *)0);
ensures (\result == (_Bool)1) ==> (item->buffer.size != (size_t)0);
ensures (\result == (_Bool)1) ==> (item->shape_offset <= item->data_size);
*/
_Bool _sshape_validate_buffer_item(sshape_buffer_item_t const *item,
                                   uint32_t build_size)
{
  _Bool __retres;
  /*@ assert rte_8: mem_access: \valid_read(&item->buffer.ptr); */
  if ((void const *)0 == item->buffer.ptr) {
    __retres = (_Bool)0;
    goto return_label;
  }
  /*@ assert rte_9: mem_access: \valid_read(&item->buffer.size); */
  if ((size_t)0 == item->buffer.size) {
    __retres = (_Bool)0;
    goto return_label;
  }
  /*@ assert rte_10: mem_access: \valid_read(&item->data_size); */
  /*@ assert rte_11: mem_access: \valid_read(&item->buffer.size); */
  if (item->data_size + (size_t)build_size > item->buffer.size) {
    __retres = (_Bool)0;
    goto return_label;
  }
  /*@ assert rte_12: mem_access: \valid_read(&item->shape_offset); */
  /*@ assert rte_13: mem_access: \valid_read(&item->data_size); */
  if (item->shape_offset > item->data_size) {
    __retres = (_Bool)0;
    goto return_label;
  }
  __retres = (_Bool)1;
  return_label: return __retres;
}

/*@
requires \valid_read(buf);
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
_Bool _sshape_validate_buffer(sshape_buffer_t const *buf,
                              uint32_t num_vertices, uint32_t num_indices)
{
  _Bool __retres;
  _Bool tmp;
  _Bool tmp_0;
  tmp = _sshape_validate_buffer_item(& buf->vertices,
                                     (uint32_t)((unsigned long)num_vertices * sizeof(sshape_vertex_t)));
  if (! tmp) {
    __retres = (_Bool)0;
    goto return_label;
  }
  tmp_0 = _sshape_validate_buffer_item(& buf->indices,
                                       (uint32_t)((unsigned long)num_indices * sizeof(uint16_t)));
  if (! tmp_0) {
    __retres = (_Bool)0;
    goto return_label;
  }
  __retres = (_Bool)1;
  return_label: return __retres;
}

/*@
requires \valid(item);
assigns item->shape_offset;
ensures item->shape_offset == \old(item->data_size);
*/
void _sshape_advance_offset(sshape_buffer_item_t *item)
{
  /*@ assert rte_67: mem_access: \valid(&item->shape_offset); */
  /*@ assert rte_68: mem_access: \valid_read(&item->data_size); */
  item->shape_offset = item->data_size;
  return;
}

/*@
requires \valid_read(buf);
assigns \nothing;
ensures \result == (uint16_t)(buf->vertices.data_size / sizeof(sshape_vertex_t));
*/
uint16_t _sshape_base_index(sshape_buffer_t const *buf)
{
  uint16_t __retres;
  /*@ assert rte_69: mem_access: \valid_read(&buf->vertices.data_size); */
  __retres = (uint16_t)(buf->vertices.data_size / sizeof(sshape_vertex_t));
  return __retres;
}

/*@
assigns \nothing;
ensures \result == (((uint32_t)a * (uint32_t)0x1000000 + (uint32_t)b * (uint32_t)0x10000) + (uint32_t)g * (uint32_t)0x100) + (uint32_t)r;
*/
uint32_t sshape_color_4b(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  uint32_t tmp;
  tmp = _sshape_pack_ub4_ubyte4n(r,g,b,a);
  return tmp;
}

/*@
assigns \nothing;
ensures \result == (((uint32_t)255 * (uint32_t)0x1000000 + (uint32_t)b * (uint32_t)0x10000) + (uint32_t)g * (uint32_t)0x100) + (uint32_t)r;
*/
uint32_t sshape_color_3b(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t tmp;
  tmp = _sshape_pack_ub4_ubyte4n(r,g,b,(uint8_t)255);
  return tmp;
}

/*@
requires tiles >= 1;
requires tiles <= 26754;
assigns \nothing;
ensures \result.vertices.num == (tiles + 1) * (tiles + 1);
ensures \result.indices.num == tiles * tiles * 6;
ensures \result.vertices.num >= 4;
ensures \result.indices.num >= 6;
*/
sshape_sizes_t sshape_plane_sizes(uint32_t tiles)
{
  __FC_assert((tiles >= (uint32_t)1) != 0,
              "/tmp/verdict_55nexdna/sokol_shape.c",489,"tiles >= 1");
  sshape_sizes_t res =
    {.vertices = {.num = (uint32_t)0, .size = 0U},
     .indices = {.num = 0U, .size = 0U}};
  res.vertices.num = _sshape_plane_num_vertices(tiles);
  res.indices.num = _sshape_plane_num_indices(tiles);
  res.vertices.size = (uint32_t)((unsigned long)res.vertices.num * sizeof(sshape_vertex_t));
  res.indices.size = (uint32_t)((unsigned long)res.indices.num * sizeof(uint16_t));
  return res;
}

/*@
requires tiles >= 1;
requires tiles <= 10922;
assigns \nothing;
ensures \result.vertices.num == ((tiles + 1) * (tiles + 1)) * 6;
ensures \result.indices.num == tiles * tiles * 36;
ensures \result.vertices.num >= 6;
ensures \result.indices.num >= 36;
*/
sshape_sizes_t sshape_box_sizes(uint32_t tiles)
{
  __FC_assert((tiles >= (uint32_t)1) != 0,
              "/tmp/verdict_55nexdna/sokol_shape.c",501,"tiles >= 1");
  sshape_sizes_t res =
    {.vertices = {.num = (uint32_t)0, .size = 0U},
     .indices = {.num = 0U, .size = 0U}};
  res.vertices.num = _sshape_box_num_vertices(tiles);
  res.indices.num = _sshape_box_num_indices(tiles);
  res.vertices.size = (uint32_t)((unsigned long)res.vertices.num * sizeof(sshape_vertex_t));
  res.indices.size = (uint32_t)((unsigned long)res.indices.num * sizeof(uint16_t));
  return res;
}

/*@
requires slices >= 3;
requires stacks >= 2;
requires slices <= 0xFFFFFFFE;
requires stacks <= 0xFFFFFFFE;
requires (slices + 1) * (stacks + 1) <= 0xFFFFFFFF;
requires 6 * (integer)slices * ((integer)stacks - 1) <= 0xFFFFFFFF;
assigns \nothing;
ensures \result.vertices.num == (slices + 1) * (stacks + 1);
ensures \result.indices.num == 6 * slices * (stacks - 1);
ensures \result.vertices.num >= 1;
ensures \result.indices.num >= 6;
*/
sshape_sizes_t sshape_sphere_sizes(uint32_t slices, uint32_t stacks)
{
  int tmp;
  if (slices >= (uint32_t)3) 
    if (stacks >= (uint32_t)2) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_55nexdna/sokol_shape.c",513,
              "(slices >= 3) && (stacks >= 2)");
  sshape_sizes_t res =
    {.vertices = {.num = (uint32_t)0, .size = 0U},
     .indices = {.num = 0U, .size = 0U}};
  res.vertices.num = _sshape_sphere_num_vertices(slices,stacks);
  res.indices.num = _sshape_sphere_num_indices(slices,stacks);
  res.vertices.size = (uint32_t)((unsigned long)res.vertices.num * sizeof(sshape_vertex_t));
  res.indices.size = (uint32_t)((unsigned long)res.indices.num * sizeof(uint16_t));
  return res;
}

/*@
requires slices >= 3;
requires stacks >= 1;
requires slices <= 0xFFFFFFFE;
requires stacks <= 0xFFFFFFFA;
requires (slices + 1) * (stacks + 5) <= 0xFFFFFFFF;
requires 6 * (uint64_t)slices * ((uint64_t)stacks + 1) <= 0xFFFFFFFF;
assigns \nothing;
ensures \result.vertices.num == (slices + 1) * (stacks + 5);
ensures \result.indices.num == 6 * slices * (stacks + 1);
ensures \result.vertices.size == (uint32_t)((unsigned long)\result.vertices.num * sizeof(sshape_vertex_t));
ensures \result.indices.size == (uint32_t)((unsigned long)\result.indices.num * sizeof(uint16_t));
*/
sshape_sizes_t sshape_cylinder_sizes(uint32_t slices, uint32_t stacks)
{
  int tmp;
  if (slices >= (uint32_t)3) 
    if (stacks >= (uint32_t)1) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_55nexdna/sokol_shape.c",525,
              "(slices >= 3) && (stacks >= 1)");
  sshape_sizes_t res =
    {.vertices = {.num = (uint32_t)0, .size = 0U},
     .indices = {.num = 0U, .size = 0U}};
  res.vertices.num = _sshape_cylinder_num_vertices(slices,stacks);
  res.indices.num = _sshape_cylinder_num_indices(slices,stacks);
  res.vertices.size = (uint32_t)((unsigned long)res.vertices.num * sizeof(sshape_vertex_t));
  res.indices.size = (uint32_t)((unsigned long)res.indices.num * sizeof(uint16_t));
  return res;
}

/*@
requires sides >= 3;
requires rings >= 3;
requires (uint64_t)(sides + 1) * (uint64_t)(rings + 1) <= UINT32_MAX;
requires sides * rings <= 715827882;
assigns \nothing;
ensures \result.vertices.num == (sides + 1) * (rings + 1);
ensures \result.indices.num == sides * rings * 6;
*/
sshape_sizes_t sshape_torus_sizes(uint32_t sides, uint32_t rings)
{
  int tmp;
  if (sides >= (uint32_t)3) 
    if (rings >= (uint32_t)3) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_55nexdna/sokol_shape.c",537,
              "(sides >= 3) && (rings >= 3)");
  sshape_sizes_t res =
    {.vertices = {.num = (uint32_t)0, .size = 0U},
     .indices = {.num = 0U, .size = 0U}};
  res.vertices.num = _sshape_torus_num_vertices(sides,rings);
  res.indices.num = _sshape_torus_num_indices(sides,rings);
  res.vertices.size = (uint32_t)((unsigned long)res.vertices.num * sizeof(sshape_vertex_t));
  res.indices.size = (uint32_t)((unsigned long)res.indices.num * sizeof(uint16_t));
  return res;
}

/*@
requires buf != \null;
requires \valid_read(buf);
requires buf->valid == 0 || buf->valid == 1;
requires buf->valid == 1;
requires buf->indices.shape_offset < buf->indices.data_size;
requires (buf->indices.shape_offset & 1) == 0;
requires (buf->indices.data_size & 1) == 0;
assigns \nothing;
ensures \result.base_element == (int)(buf->indices.shape_offset / sizeof(uint16_t));
ensures \result.num_elements == (int)((buf->indices.data_size - buf->indices.shape_offset) / sizeof(uint16_t));
*/
sshape_element_range_t sshape_element_range(sshape_buffer_t const *buf)
{
  int tmp;
  if (buf) 
    /*@ assert rte_70: bool_value: buf->valid ≡ 0 ∨ buf->valid ≡ 1; */
    /*@ assert rte_71: mem_access: \valid_read(&buf->valid); */
    if (buf->valid) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_55nexdna/sokol_shape.c",549,
              "buf && buf->valid");
  /*@ assert rte_72: mem_access: \valid_read(&buf->indices.shape_offset); */
  /*@ assert rte_73: mem_access: \valid_read(&buf->indices.data_size); */
  __FC_assert((buf->indices.shape_offset < buf->indices.data_size) != 0,
              "/tmp/verdict_55nexdna/sokol_shape.c",550,
              "buf->indices.shape_offset < buf->indices.data_size");
  /*@ assert rte_74: mem_access: \valid_read(&buf->indices.shape_offset); */
  __FC_assert(((unsigned long)0 == (buf->indices.shape_offset & (sizeof(uint16_t) - (unsigned long)1))) != 0,
              "/tmp/verdict_55nexdna/sokol_shape.c",551,
              "0 == (buf->indices.shape_offset & (sizeof(uint16_t) - 1))");
  /*@ assert rte_75: mem_access: \valid_read(&buf->indices.data_size); */
  __FC_assert(((unsigned long)0 == (buf->indices.data_size & (sizeof(uint16_t) - (unsigned long)1))) != 0,
              "/tmp/verdict_55nexdna/sokol_shape.c",552,
              "0 == (buf->indices.data_size & (sizeof(uint16_t) - 1))");
  sshape_element_range_t range = {.base_element = 0, .num_elements = 0};
  /*@ assert rte_76: mem_access: \valid_read(&buf->indices.shape_offset); */
  range.base_element = (int)(buf->indices.shape_offset / sizeof(uint16_t));
  /*@ assert rte_77: bool_value: buf->valid ≡ 0 ∨ buf->valid ≡ 1; */
  /*@ assert rte_78: mem_access: \valid_read(&buf->valid); */
  if (buf->valid) 
    /*@ assert rte_79: mem_access: \valid_read(&buf->indices.data_size); */
    /*@ assert rte_80: mem_access: \valid_read(&buf->indices.shape_offset); */
    range.num_elements = (int)((buf->indices.data_size - buf->indices.shape_offset) / sizeof(uint16_t));
  else range.num_elements = 0;
  return range;
}

int main(void)
{
  int __retres;
  float clamp_results[10];
  uint32_t pack_results[8];
  uint32_t fpack_results[8];
  uint32_t bpack_results[7];
  uint16_t ushort_results[8];
  _sshape_vec4_t vec4_results[4];
  _sshape_vec2_t vec2_results[4];
  uint32_t plane_verts_results[5];
  uint32_t plane_indices_results[5];
  uint32_t box_verts_results[5];
  uint32_t box_indices_results[5];
  uint32_t sphere_verts_results[5];
  uint32_t sphere_indices_results[5];
  uint32_t cylinder_verts_results[5];
  uint32_t cylinder_indices_results[5];
  uint32_t torus_verts_results[5];
  uint32_t torus_indices_results[5];
  uint8_t buffer_data[100];
  uint8_t vertex_data[200];
  uint8_t index_data[200];
  sshape_buffer_item_t offset_item;
  sshape_buffer_item_t offset_item2;
  sshape_buffer_item_t offset_item3;
  sshape_buffer_item_t base_vertices;
  sshape_buffer_item_t base_indices;
  sshape_buffer_t base_buf;
  sshape_buffer_item_t base_vertices2;
  sshape_buffer_t base_buf2;
  sshape_buffer_item_t base_vertices3;
  sshape_buffer_t base_buf3;
  uint32_t color4b_results[7];
  uint32_t color3b_results[6];
  sshape_sizes_t plane_sizes_results[3];
  sshape_sizes_t box_sizes_results[3];
  sshape_sizes_t sphere_sizes_results[3];
  sshape_sizes_t cylinder_sizes_results[3];
  sshape_sizes_t torus_sizes_results[3];
  uint8_t element_vertex_data[500];
  uint8_t element_index_data[500];
  sshape_buffer_item_t element_vertices;
  sshape_buffer_item_t element_indices;
  sshape_buffer_t element_buf;
  sshape_buffer_item_t element_indices2;
  sshape_buffer_t element_buf2;
  sshape_buffer_item_t element_indices3;
  sshape_buffer_t element_buf3;
  int test_counter = 0;
  /*@ assert rte_81: is_nan_or_infinite: \is_finite(0.5f); */
  /*@ assert rte_82: is_nan_or_infinite: \is_finite(1.0f); */
  float clamp_inputs[10] =
    {- 0.5f, 0.0f, 0.5f, 1.0f, 1.5f, - 1.0f, 2.0f, 0.25f, 0.75f, 0.999f};
  clamp_results[0] = _sshape_clamp(clamp_inputs[0]);
  clamp_results[1] = _sshape_clamp(clamp_inputs[1]);
  clamp_results[2] = _sshape_clamp(clamp_inputs[2]);
  clamp_results[3] = _sshape_clamp(clamp_inputs[3]);
  clamp_results[4] = _sshape_clamp(clamp_inputs[4]);
  clamp_results[5] = _sshape_clamp(clamp_inputs[5]);
  clamp_results[6] = _sshape_clamp(clamp_inputs[6]);
  clamp_results[7] = _sshape_clamp(clamp_inputs[7]);
  clamp_results[8] = _sshape_clamp(clamp_inputs[8]);
  clamp_results[9] = _sshape_clamp(clamp_inputs[9]);
  /*@ assert rte_83: signed_overflow: test_counter + 10 ≤ 2147483647; */
  test_counter += 10;
  /*@ assert rte_84: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_85: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_86: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  struct __anonstruct_pack_inputs_1 pack_inputs[8] =
    {{.x = (uint8_t)0x00,
      .y = (uint8_t)0x00,
      .z = (uint8_t)0x00,
      .w = (uint8_t)0x00,
      .expected = (uint32_t)0x00000000},
     {.x = (uint8_t)0xFF,
      .y = (uint8_t)0x00,
      .z = (uint8_t)0x00,
      .w = (uint8_t)0x00,
      .expected = (uint32_t)0x000000FF},
     {.x = (uint8_t)0x00,
      .y = (uint8_t)0xFF,
      .z = (uint8_t)0x00,
      .w = (uint8_t)0x00,
      .expected = (uint32_t)0x0000FF00},
     {.x = (uint8_t)0x00,
      .y = (uint8_t)0x00,
      .z = (uint8_t)0xFF,
      .w = (uint8_t)0x00,
      .expected = (uint32_t)0x00FF0000},
     {.x = (uint8_t)0x00,
      .y = (uint8_t)0x00,
      .z = (uint8_t)0x00,
      .w = (uint8_t)0xFF,
      .expected = 0xFF000000},
     {.x = (uint8_t)0x12,
      .y = (uint8_t)0x34,
      .z = (uint8_t)0x56,
      .w = (uint8_t)0x78,
      .expected = (uint32_t)0x78563412},
     {.x = (uint8_t)0xAB,
      .y = (uint8_t)0xCD,
      .z = (uint8_t)0xEF,
      .w = (uint8_t)0x01,
      .expected = (uint32_t)0x01EFCDAB},
     {.x = (uint8_t)0x7F,
      .y = (uint8_t)0x80,
      .z = (uint8_t)0x81,
      .w = (uint8_t)0x82,
      .expected = 0x8281807F}};
  pack_results[0] = _sshape_pack_ub4_ubyte4n(pack_inputs[0].x,
                                             pack_inputs[0].y,
                                             pack_inputs[0].z,
                                             pack_inputs[0].w);
  pack_results[1] = _sshape_pack_ub4_ubyte4n(pack_inputs[1].x,
                                             pack_inputs[1].y,
                                             pack_inputs[1].z,
                                             pack_inputs[1].w);
  pack_results[2] = _sshape_pack_ub4_ubyte4n(pack_inputs[2].x,
                                             pack_inputs[2].y,
                                             pack_inputs[2].z,
                                             pack_inputs[2].w);
  pack_results[3] = _sshape_pack_ub4_ubyte4n(pack_inputs[3].x,
                                             pack_inputs[3].y,
                                             pack_inputs[3].z,
                                             pack_inputs[3].w);
  pack_results[4] = _sshape_pack_ub4_ubyte4n(pack_inputs[4].x,
                                             pack_inputs[4].y,
                                             pack_inputs[4].z,
                                             pack_inputs[4].w);
  pack_results[5] = _sshape_pack_ub4_ubyte4n(pack_inputs[5].x,
                                             pack_inputs[5].y,
                                             pack_inputs[5].z,
                                             pack_inputs[5].w);
  pack_results[6] = _sshape_pack_ub4_ubyte4n(pack_inputs[6].x,
                                             pack_inputs[6].y,
                                             pack_inputs[6].z,
                                             pack_inputs[6].w);
  pack_results[7] = _sshape_pack_ub4_ubyte4n(pack_inputs[7].x,
                                             pack_inputs[7].y,
                                             pack_inputs[7].z,
                                             pack_inputs[7].w);
  /*@ assert rte_87: signed_overflow: test_counter + 8 ≤ 2147483647; */
  test_counter += 8;
  /*@ assert rte_88: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_89: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_90: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  struct __anonstruct_fpack_inputs_2 fpack_inputs[8] =
    {{.x = 0.0f,
      .y = 0.0f,
      .z = 0.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x00000000},
     {.x = 1.0f,
      .y = 0.0f,
      .z = 0.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x000000FF},
     {.x = 0.0f,
      .y = 1.0f,
      .z = 0.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x0000FF00},
     {.x = 0.0f,
      .y = 0.0f,
      .z = 1.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x00FF0000},
     {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f, .expected = 0xFF000000},
     {.x = 0.5f,
      .y = 0.5f,
      .z = 0.5f,
      .w = 0.5f,
      .expected = (uint32_t)0x7F7F7F7F},
     {.x = 0.25f, .y = 0.5f, .z = 0.75f, .w = 1.0f, .expected = 0xFFBF7F3F},
     {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f, .expected = 0xFFFFFFFF}};
  fpack_results[0] = _sshape_pack_f4_ubyte4n(fpack_inputs[0].x,
                                             fpack_inputs[0].y,
                                             fpack_inputs[0].z,
                                             fpack_inputs[0].w);
  fpack_results[1] = _sshape_pack_f4_ubyte4n(fpack_inputs[1].x,
                                             fpack_inputs[1].y,
                                             fpack_inputs[1].z,
                                             fpack_inputs[1].w);
  fpack_results[2] = _sshape_pack_f4_ubyte4n(fpack_inputs[2].x,
                                             fpack_inputs[2].y,
                                             fpack_inputs[2].z,
                                             fpack_inputs[2].w);
  fpack_results[3] = _sshape_pack_f4_ubyte4n(fpack_inputs[3].x,
                                             fpack_inputs[3].y,
                                             fpack_inputs[3].z,
                                             fpack_inputs[3].w);
  fpack_results[4] = _sshape_pack_f4_ubyte4n(fpack_inputs[4].x,
                                             fpack_inputs[4].y,
                                             fpack_inputs[4].z,
                                             fpack_inputs[4].w);
  fpack_results[5] = _sshape_pack_f4_ubyte4n(fpack_inputs[5].x,
                                             fpack_inputs[5].y,
                                             fpack_inputs[5].z,
                                             fpack_inputs[5].w);
  fpack_results[6] = _sshape_pack_f4_ubyte4n(fpack_inputs[6].x,
                                             fpack_inputs[6].y,
                                             fpack_inputs[6].z,
                                             fpack_inputs[6].w);
  fpack_results[7] = _sshape_pack_f4_ubyte4n(fpack_inputs[7].x,
                                             fpack_inputs[7].y,
                                             fpack_inputs[7].z,
                                             fpack_inputs[7].w);
  /*@ assert rte_91: signed_overflow: test_counter + 8 ≤ 2147483647; */
  test_counter += 8;
  /*@ assert rte_92: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_93: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_94: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_95: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_96: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_97: is_nan_or_infinite: \is_finite(0.5f); */
  /*@ assert rte_98: is_nan_or_infinite: \is_finite(0.75f); */
  struct __anonstruct_bpack_inputs_3 bpack_inputs[7] =
    {{.x = 0.0f,
      .y = 0.0f,
      .z = 0.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x00000000},
     {.x = 1.0f,
      .y = 0.0f,
      .z = 0.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x0000007F},
     {.x = - 1.0f,
      .y = 0.0f,
      .z = 0.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x00000081},
     {.x = 0.0f,
      .y = 1.0f,
      .z = 0.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x00007F00},
     {.x = 0.0f,
      .y = 0.0f,
      .z = 1.0f,
      .w = 0.0f,
      .expected = (uint32_t)0x007F0000},
     {.x = 0.0f,
      .y = 0.0f,
      .z = 0.0f,
      .w = 1.0f,
      .expected = (uint32_t)0x7F000000},
     {.x = 0.5f,
      .y = - 0.5f,
      .z = 0.25f,
      .w = - 0.75f,
      .expected = 0xA11FC13F}};
  bpack_results[0] = _sshape_pack_f4_byte4n(bpack_inputs[0].x,
                                            bpack_inputs[0].y,
                                            bpack_inputs[0].z,
                                            bpack_inputs[0].w);
  bpack_results[1] = _sshape_pack_f4_byte4n(bpack_inputs[1].x,
                                            bpack_inputs[1].y,
                                            bpack_inputs[1].z,
                                            bpack_inputs[1].w);
  bpack_results[2] = _sshape_pack_f4_byte4n(bpack_inputs[2].x,
                                            bpack_inputs[2].y,
                                            bpack_inputs[2].z,
                                            bpack_inputs[2].w);
  bpack_results[3] = _sshape_pack_f4_byte4n(bpack_inputs[3].x,
                                            bpack_inputs[3].y,
                                            bpack_inputs[3].z,
                                            bpack_inputs[3].w);
  bpack_results[4] = _sshape_pack_f4_byte4n(bpack_inputs[4].x,
                                            bpack_inputs[4].y,
                                            bpack_inputs[4].z,
                                            bpack_inputs[4].w);
  bpack_results[5] = _sshape_pack_f4_byte4n(bpack_inputs[5].x,
                                            bpack_inputs[5].y,
                                            bpack_inputs[5].z,
                                            bpack_inputs[5].w);
  bpack_results[6] = _sshape_pack_f4_byte4n(bpack_inputs[6].x,
                                            bpack_inputs[6].y,
                                            bpack_inputs[6].z,
                                            bpack_inputs[6].w);
  /*@ assert rte_99: signed_overflow: test_counter + 7 ≤ 2147483647; */
  test_counter += 7;
  /*@ assert rte_100: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_101: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_102: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_103: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  float ushort_inputs[8] =
    {0.0f, 1.0f, 0.5f, 0.25f, 0.75f, 0.1f, 0.9f, 0.333f};
  ushort_results[0] = _sshape_pack_f_ushortn(ushort_inputs[0]);
  ushort_results[1] = _sshape_pack_f_ushortn(ushort_inputs[1]);
  ushort_results[2] = _sshape_pack_f_ushortn(ushort_inputs[2]);
  ushort_results[3] = _sshape_pack_f_ushortn(ushort_inputs[3]);
  ushort_results[4] = _sshape_pack_f_ushortn(ushort_inputs[4]);
  ushort_results[5] = _sshape_pack_f_ushortn(ushort_inputs[5]);
  ushort_results[6] = _sshape_pack_f_ushortn(ushort_inputs[6]);
  ushort_results[7] = _sshape_pack_f_ushortn(ushort_inputs[7]);
  /*@ assert rte_104: signed_overflow: test_counter + 8 ≤ 2147483647; */
  test_counter += 8;
  /*@ assert rte_105: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_106: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_107: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_108: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_109: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_110: is_nan_or_infinite: \is_finite(2.0f); */
  /*@ assert rte_111: is_nan_or_infinite: \is_finite(3.0f); */
  /*@ assert rte_112: is_nan_or_infinite: \is_finite(4.0f); */
  struct __anonstruct_vec4_inputs_4 vec4_inputs[4] =
    {{.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f},
     {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f},
     {.x = - 1.0f, .y = - 2.0f, .z = - 3.0f, .w = - 4.0f},
     {.x = 1.5f, .y = 2.5f, .z = 3.5f, .w = 4.5f}};
  vec4_results[0] = _sshape_vec4(vec4_inputs[0].x,vec4_inputs[0].y,
                                 vec4_inputs[0].z,vec4_inputs[0].w);
  vec4_results[1] = _sshape_vec4(vec4_inputs[1].x,vec4_inputs[1].y,
                                 vec4_inputs[1].z,vec4_inputs[1].w);
  vec4_results[2] = _sshape_vec4(vec4_inputs[2].x,vec4_inputs[2].y,
                                 vec4_inputs[2].z,vec4_inputs[2].w);
  vec4_results[3] = _sshape_vec4(vec4_inputs[3].x,vec4_inputs[3].y,
                                 vec4_inputs[3].z,vec4_inputs[3].w);
  /*@ assert rte_113: signed_overflow: test_counter + 16 ≤ 2147483647; */
  test_counter += 16;
  /*@ assert rte_114: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_115: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_116: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_117: is_nan_or_infinite: \is_finite(1.0f); */
  /*@ assert rte_118: is_nan_or_infinite: \is_finite(2.0f); */
  struct __anonstruct_vec2_inputs_5 vec2_inputs[4] =
    {{.x = 0.0f, .y = 0.0f},
     {.x = 1.0f, .y = 2.0f},
     {.x = - 1.0f, .y = - 2.0f},
     {.x = 1.5f, .y = 2.5f}};
  vec2_results[0] = _sshape_vec2(vec2_inputs[0].x,vec2_inputs[0].y);
  vec2_results[1] = _sshape_vec2(vec2_inputs[1].x,vec2_inputs[1].y);
  vec2_results[2] = _sshape_vec2(vec2_inputs[2].x,vec2_inputs[2].y);
  vec2_results[3] = _sshape_vec2(vec2_inputs[3].x,vec2_inputs[3].y);
  /*@ assert rte_119: signed_overflow: test_counter + 8 ≤ 2147483647; */
  test_counter += 8;
  /*@ assert rte_120: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_121: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_122: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  sshape_mat4_t zero_mat = {.m = {{(float)0}}};
  sshape_mat4_t identity_result = _sshape_mat4_identity();
  /*@ assert rte_123: signed_overflow: test_counter + 8 ≤ 2147483647; */
  test_counter += 8;
  sshape_mat4_t mul_mat =
    {.m = {{1.0f, 0.0f, 0.0f, 0.0f},
           {0.0f, 1.0f, 0.0f, 0.0f},
           {0.0f, 0.0f, 1.0f, 0.0f},
           {0.0f, 0.0f, 0.0f, 1.0f}}};
  _sshape_vec4_t mul_vec = _sshape_vec4(1.0f,2.0f,3.0f,4.0f);
  _sshape_vec4_t mul_result1 =
    _sshape_mat4_mul((sshape_mat4_t const *)(& mul_mat),mul_vec);
  sshape_mat4_t test_mat_mul =
    {.m = {{1.0f, 0.0f, 0.0f, 0.0f},
           {0.0f, 2.0f, 0.0f, 0.0f},
           {0.0f, 0.0f, 3.0f, 0.0f},
           {0.0f, 0.0f, 0.0f, 4.0f}}};
  _sshape_vec4_t mul_vec2 = _sshape_vec4(1.0f,1.0f,1.0f,1.0f);
  _sshape_vec4_t mul_result2 =
    _sshape_mat4_mul((sshape_mat4_t const *)(& test_mat_mul),mul_vec2);
  uint32_t plane_tiles_inputs[5] =
    {(uint32_t)1, (uint32_t)2, (uint32_t)3, (uint32_t)4, (uint32_t)10};
  plane_verts_results[0] = _sshape_plane_num_vertices(plane_tiles_inputs[0]);
  plane_verts_results[1] = _sshape_plane_num_vertices(plane_tiles_inputs[1]);
  plane_verts_results[2] = _sshape_plane_num_vertices(plane_tiles_inputs[2]);
  plane_verts_results[3] = _sshape_plane_num_vertices(plane_tiles_inputs[3]);
  plane_verts_results[4] = _sshape_plane_num_vertices(plane_tiles_inputs[4]);
  plane_indices_results[0] = _sshape_plane_num_indices(plane_tiles_inputs[0]);
  plane_indices_results[1] = _sshape_plane_num_indices(plane_tiles_inputs[1]);
  plane_indices_results[2] = _sshape_plane_num_indices(plane_tiles_inputs[2]);
  plane_indices_results[3] = _sshape_plane_num_indices(plane_tiles_inputs[3]);
  plane_indices_results[4] = _sshape_plane_num_indices(plane_tiles_inputs[4]);
  /*@ assert rte_124: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_125: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_126: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_127: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  uint32_t box_tiles_inputs[5] =
    {(uint32_t)1, (uint32_t)2, (uint32_t)3, (uint32_t)4, (uint32_t)10};
  box_verts_results[0] = _sshape_box_num_vertices(box_tiles_inputs[0]);
  box_verts_results[1] = _sshape_box_num_vertices(box_tiles_inputs[1]);
  box_verts_results[2] = _sshape_box_num_vertices(box_tiles_inputs[2]);
  box_verts_results[3] = _sshape_box_num_vertices(box_tiles_inputs[3]);
  box_verts_results[4] = _sshape_box_num_vertices(box_tiles_inputs[4]);
  /*@ assert rte_128: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_129: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_130: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_131: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  box_indices_results[0] = _sshape_box_num_indices(box_tiles_inputs[0]);
  box_indices_results[1] = _sshape_box_num_indices(box_tiles_inputs[1]);
  box_indices_results[2] = _sshape_box_num_indices(box_tiles_inputs[2]);
  box_indices_results[3] = _sshape_box_num_indices(box_tiles_inputs[3]);
  box_indices_results[4] = _sshape_box_num_indices(box_tiles_inputs[4]);
  /*@ assert rte_132: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_133: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_134: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_135: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  struct __anonstruct_sphere_verts_inputs_6 sphere_verts_inputs[5] =
    {{.slices = (uint32_t)3, .stacks = (uint32_t)2, .expected = (uint32_t)12},
     {.slices = (uint32_t)5, .stacks = (uint32_t)4, .expected = (uint32_t)30},
     {.slices = (uint32_t)10, .stacks = (uint32_t)8, .expected = (uint32_t)99},
     {.slices = (uint32_t)20,
      .stacks = (uint32_t)10,
      .expected = (uint32_t)231},
     {.slices = (uint32_t)1, .stacks = (uint32_t)1, .expected = (uint32_t)4}};
  sphere_verts_results[0] = _sshape_sphere_num_vertices(sphere_verts_inputs[0].slices,
                                                        sphere_verts_inputs[0].stacks);
  sphere_verts_results[1] = _sshape_sphere_num_vertices(sphere_verts_inputs[1].slices,
                                                        sphere_verts_inputs[1].stacks);
  sphere_verts_results[2] = _sshape_sphere_num_vertices(sphere_verts_inputs[2].slices,
                                                        sphere_verts_inputs[2].stacks);
  sphere_verts_results[3] = _sshape_sphere_num_vertices(sphere_verts_inputs[3].slices,
                                                        sphere_verts_inputs[3].stacks);
  sphere_verts_results[4] = _sshape_sphere_num_vertices(sphere_verts_inputs[4].slices,
                                                        sphere_verts_inputs[4].stacks);
  /*@ assert rte_136: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  sphere_indices_results[0] = _sshape_sphere_num_indices(sphere_verts_inputs[0].slices,
                                                         sphere_verts_inputs[0].stacks);
  sphere_indices_results[1] = _sshape_sphere_num_indices(sphere_verts_inputs[1].slices,
                                                         sphere_verts_inputs[1].stacks);
  sphere_indices_results[2] = _sshape_sphere_num_indices(sphere_verts_inputs[2].slices,
                                                         sphere_verts_inputs[2].stacks);
  sphere_indices_results[3] = _sshape_sphere_num_indices(sphere_verts_inputs[3].slices,
                                                         sphere_verts_inputs[3].stacks);
  sphere_indices_results[4] = _sshape_sphere_num_indices(sphere_verts_inputs[4].slices,
                                                         sphere_verts_inputs[4].stacks);
  /*@ assert rte_137: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_138: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_139: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_140: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  struct __anonstruct_cylinder_verts_inputs_7 cylinder_verts_inputs[5] =
    {{.slices = (uint32_t)3, .stacks = (uint32_t)1, .expected = (uint32_t)24},
     {.slices = (uint32_t)5, .stacks = (uint32_t)2, .expected = (uint32_t)42},
     {.slices = (uint32_t)10,
      .stacks = (uint32_t)5,
      .expected = (uint32_t)110},
     {.slices = (uint32_t)20,
      .stacks = (uint32_t)10,
      .expected = (uint32_t)315},
     {.slices = (uint32_t)1, .stacks = (uint32_t)1, .expected = (uint32_t)12}};
  cylinder_verts_results[0] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[0].slices,
                                                            cylinder_verts_inputs[0].stacks);
  cylinder_verts_results[1] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[1].slices,
                                                            cylinder_verts_inputs[1].stacks);
  cylinder_verts_results[2] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[2].slices,
                                                            cylinder_verts_inputs[2].stacks);
  cylinder_verts_results[3] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[3].slices,
                                                            cylinder_verts_inputs[3].stacks);
  cylinder_verts_results[4] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[4].slices,
                                                            cylinder_verts_inputs[4].stacks);
  /*@ assert rte_141: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_142: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_143: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_144: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  cylinder_indices_results[0] = _sshape_cylinder_num_indices(cylinder_verts_inputs[0].slices,
                                                             cylinder_verts_inputs[0].stacks);
  cylinder_indices_results[1] = _sshape_cylinder_num_indices(cylinder_verts_inputs[1].slices,
                                                             cylinder_verts_inputs[1].stacks);
  cylinder_indices_results[2] = _sshape_cylinder_num_indices(cylinder_verts_inputs[2].slices,
                                                             cylinder_verts_inputs[2].stacks);
  cylinder_indices_results[3] = _sshape_cylinder_num_indices(cylinder_verts_inputs[3].slices,
                                                             cylinder_verts_inputs[3].stacks);
  cylinder_indices_results[4] = _sshape_cylinder_num_indices(cylinder_verts_inputs[4].slices,
                                                             cylinder_verts_inputs[4].stacks);
  /*@ assert rte_145: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_146: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_147: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  struct __anonstruct_torus_verts_inputs_8 torus_verts_inputs[5] =
    {{.sides = (uint32_t)3, .rings = (uint32_t)3, .expected = (uint32_t)16},
     {.sides = (uint32_t)5, .rings = (uint32_t)5, .expected = (uint32_t)36},
     {.sides = (uint32_t)10, .rings = (uint32_t)8, .expected = (uint32_t)99},
     {.sides = (uint32_t)20, .rings = (uint32_t)15, .expected = (uint32_t)336},
     {.sides = (uint32_t)1, .rings = (uint32_t)1, .expected = (uint32_t)4}};
  torus_verts_results[0] = _sshape_torus_num_vertices(torus_verts_inputs[0].sides,
                                                      torus_verts_inputs[0].rings);
  torus_verts_results[1] = _sshape_torus_num_vertices(torus_verts_inputs[1].sides,
                                                      torus_verts_inputs[1].rings);
  torus_verts_results[2] = _sshape_torus_num_vertices(torus_verts_inputs[2].sides,
                                                      torus_verts_inputs[2].rings);
  torus_verts_results[3] = _sshape_torus_num_vertices(torus_verts_inputs[3].sides,
                                                      torus_verts_inputs[3].rings);
  torus_verts_results[4] = _sshape_torus_num_vertices(torus_verts_inputs[4].sides,
                                                      torus_verts_inputs[4].rings);
  /*@ assert rte_148: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_149: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  torus_indices_results[0] = _sshape_torus_num_indices(torus_verts_inputs[0].sides,
                                                       torus_verts_inputs[0].rings);
  torus_indices_results[1] = _sshape_torus_num_indices(torus_verts_inputs[1].sides,
                                                       torus_verts_inputs[1].rings);
  torus_indices_results[2] = _sshape_torus_num_indices(torus_verts_inputs[2].sides,
                                                       torus_verts_inputs[2].rings);
  torus_indices_results[3] = _sshape_torus_num_indices(torus_verts_inputs[3].sides,
                                                       torus_verts_inputs[3].rings);
  torus_indices_results[4] = _sshape_torus_num_indices(torus_verts_inputs[4].sides,
                                                       torus_verts_inputs[4].rings);
  /*@ assert rte_150: signed_overflow: test_counter + 5 ≤ 2147483647; */
  test_counter += 5;
  /*@ assert rte_151: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  /*@ assert rte_152: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  offset_item.buffer.ptr = (void const *)(buffer_data);
  offset_item.buffer.size = (size_t)100;
  offset_item.data_size = (size_t)50;
  offset_item.shape_offset = (size_t)30;
  _sshape_advance_offset(& offset_item);
  /*@ assert rte_153: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  offset_item2.buffer.ptr = (void const *)(buffer_data);
  offset_item2.buffer.size = (size_t)100;
  offset_item2.data_size = (size_t)0;
  offset_item2.shape_offset = (size_t)0;
  _sshape_advance_offset(& offset_item2);
  /*@ assert rte_154: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  offset_item3.buffer.ptr = (void const *)(buffer_data);
  offset_item3.buffer.size = (size_t)100;
  offset_item3.data_size = (size_t)100;
  offset_item3.shape_offset = (size_t)75;
  _sshape_advance_offset(& offset_item3);
  /*@ assert rte_155: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  base_vertices.buffer.ptr = (void const *)(vertex_data);
  base_vertices.buffer.size = (size_t)200;
  base_vertices.data_size = (size_t)120;
  base_vertices.shape_offset = (size_t)0;
  base_indices.buffer.ptr = (void const *)(index_data);
  base_indices.buffer.size = (size_t)200;
  base_indices.data_size = (size_t)0;
  base_indices.shape_offset = (size_t)0;
  base_buf.vertices = base_vertices;
  base_buf.indices = base_indices;
  base_buf.valid = (_Bool)1;
  uint16_t base_index1 =
    _sshape_base_index((sshape_buffer_t const *)(& base_buf));
  /*@ assert rte_156: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  base_vertices2.buffer.ptr = (void const *)(vertex_data);
  base_vertices2.buffer.size = (size_t)200;
  base_vertices2.data_size = (size_t)0;
  base_vertices2.shape_offset = (size_t)0;
  base_buf2.vertices = base_vertices2;
  base_buf2.indices = base_indices;
  base_buf2.valid = (_Bool)1;
  uint16_t base_index2 =
    _sshape_base_index((sshape_buffer_t const *)(& base_buf2));
  /*@ assert rte_157: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  base_vertices3.buffer.ptr = (void const *)(vertex_data);
  base_vertices3.buffer.size = (size_t)200;
  base_vertices3.data_size = (size_t)240;
  base_vertices3.shape_offset = (size_t)0;
  base_buf3.vertices = base_vertices3;
  base_buf3.indices = base_indices;
  base_buf3.valid = (_Bool)1;
  uint16_t base_index3 =
    _sshape_base_index((sshape_buffer_t const *)(& base_buf3));
  /*@ assert rte_158: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  struct __anonstruct_color4b_inputs_9 color4b_inputs[7] =
    {{.r = (uint8_t)0x00,
      .g = (uint8_t)0x00,
      .b = (uint8_t)0x00,
      .a = (uint8_t)0x00,
      .expected = (uint32_t)0x00000000},
     {.r = (uint8_t)0xFF,
      .g = (uint8_t)0x00,
      .b = (uint8_t)0x00,
      .a = (uint8_t)0x00,
      .expected = (uint32_t)0x000000FF},
     {.r = (uint8_t)0x00,
      .g = (uint8_t)0xFF,
      .b = (uint8_t)0x00,
      .a = (uint8_t)0x00,
      .expected = (uint32_t)0x0000FF00},
     {.r = (uint8_t)0x00,
      .g = (uint8_t)0x00,
      .b = (uint8_t)0xFF,
      .a = (uint8_t)0x00,
      .expected = (uint32_t)0x00FF0000},
     {.r = (uint8_t)0x00,
      .g = (uint8_t)0x00,
      .b = (uint8_t)0x00,
      .a = (uint8_t)0xFF,
      .expected = 0xFF000000},
     {.r = (uint8_t)0x12,
      .g = (uint8_t)0x34,
      .b = (uint8_t)0x56,
      .a = (uint8_t)0x78,
      .expected = (uint32_t)0x78563412},
     {.r = (uint8_t)0xAB,
      .g = (uint8_t)0xCD,
      .b = (uint8_t)0xEF,
      .a = (uint8_t)0x01,
      .expected = (uint32_t)0x01EFCDAB}};
  color4b_results[0] = sshape_color_4b(color4b_inputs[0].r,
                                       color4b_inputs[0].g,
                                       color4b_inputs[0].b,
                                       color4b_inputs[0].a);
  color4b_results[1] = sshape_color_4b(color4b_inputs[1].r,
                                       color4b_inputs[1].g,
                                       color4b_inputs[1].b,
                                       color4b_inputs[1].a);
  color4b_results[2] = sshape_color_4b(color4b_inputs[2].r,
                                       color4b_inputs[2].g,
                                       color4b_inputs[2].b,
                                       color4b_inputs[2].a);
  color4b_results[3] = sshape_color_4b(color4b_inputs[3].r,
                                       color4b_inputs[3].g,
                                       color4b_inputs[3].b,
                                       color4b_inputs[3].a);
  color4b_results[4] = sshape_color_4b(color4b_inputs[4].r,
                                       color4b_inputs[4].g,
                                       color4b_inputs[4].b,
                                       color4b_inputs[4].a);
  color4b_results[5] = sshape_color_4b(color4b_inputs[5].r,
                                       color4b_inputs[5].g,
                                       color4b_inputs[5].b,
                                       color4b_inputs[5].a);
  color4b_results[6] = sshape_color_4b(color4b_inputs[6].r,
                                       color4b_inputs[6].g,
                                       color4b_inputs[6].b,
                                       color4b_inputs[6].a);
  /*@ assert rte_159: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  struct __anonstruct_color3b_inputs_10 color3b_inputs[6] =
    {{.r = (uint8_t)0x00,
      .g = (uint8_t)0x00,
      .b = (uint8_t)0x00,
      .expected = 0xFF000000},
     {.r = (uint8_t)0xFF,
      .g = (uint8_t)0x00,
      .b = (uint8_t)0x00,
      .expected = 0xFF0000FF},
     {.r = (uint8_t)0x00,
      .g = (uint8_t)0xFF,
      .b = (uint8_t)0x00,
      .expected = 0xFF00FF00},
     {.r = (uint8_t)0x00,
      .g = (uint8_t)0x00,
      .b = (uint8_t)0xFF,
      .expected = 0xFFFF0000},
     {.r = (uint8_t)0x12,
      .g = (uint8_t)0x34,
      .b = (uint8_t)0x56,
      .expected = 0xFF563412},
     {.r = (uint8_t)0xAB,
      .g = (uint8_t)0xCD,
      .b = (uint8_t)0xEF,
      .expected = 0xFFEFCDAB}};
  color3b_results[0] = sshape_color_3b(color3b_inputs[0].r,
                                       color3b_inputs[0].g,
                                       color3b_inputs[0].b);
  color3b_results[1] = sshape_color_3b(color3b_inputs[1].r,
                                       color3b_inputs[1].g,
                                       color3b_inputs[1].b);
  color3b_results[2] = sshape_color_3b(color3b_inputs[2].r,
                                       color3b_inputs[2].g,
                                       color3b_inputs[2].b);
  color3b_results[3] = sshape_color_3b(color3b_inputs[3].r,
                                       color3b_inputs[3].g,
                                       color3b_inputs[3].b);
  color3b_results[4] = sshape_color_3b(color3b_inputs[4].r,
                                       color3b_inputs[4].g,
                                       color3b_inputs[4].b);
  color3b_results[5] = sshape_color_3b(color3b_inputs[5].r,
                                       color3b_inputs[5].g,
                                       color3b_inputs[5].b);
  /*@ assert rte_160: signed_overflow: test_counter + 6 ≤ 2147483647; */
  test_counter += 6;
  /*@ assert rte_161: signed_overflow: test_counter + 1 ≤ 2147483647; */
  test_counter ++;
  uint32_t plane_tiles_input[3] = {(uint32_t)1, (uint32_t)2, (uint32_t)3};
  plane_sizes_results[0] = sshape_plane_sizes(plane_tiles_input[0]);
  plane_sizes_results[1] = sshape_plane_sizes(plane_tiles_input[1]);
  plane_sizes_results[2] = sshape_plane_sizes(plane_tiles_input[2]);
  /*@ assert rte_162: signed_overflow: test_counter + 12 ≤ 2147483647; */
  test_counter += 12;
  uint32_t box_tiles_input[3] = {(uint32_t)1, (uint32_t)2, (uint32_t)3};
  box_sizes_results[0] = sshape_box_sizes(box_tiles_input[0]);
  box_sizes_results[1] = sshape_box_sizes(box_tiles_input[1]);
  box_sizes_results[2] = sshape_box_sizes(box_tiles_input[2]);
  /*@ assert rte_163: signed_overflow: test_counter + 12 ≤ 2147483647; */
  test_counter += 12;
  struct __anonstruct_sphere_sizes_inputs_11 sphere_sizes_inputs[3] =
    {{.slices = (uint32_t)3, .stacks = (uint32_t)2},
     {.slices = (uint32_t)5, .stacks = (uint32_t)4},
     {.slices = (uint32_t)10, .stacks = (uint32_t)8}};
  sphere_sizes_results[0] = sshape_sphere_sizes(sphere_sizes_inputs[0].slices,
                                                sphere_sizes_inputs[0].stacks);
  sphere_sizes_results[1] = sshape_sphere_sizes(sphere_sizes_inputs[1].slices,
                                                sphere_sizes_inputs[1].stacks);
  sphere_sizes_results[2] = sshape_sphere_sizes(sphere_sizes_inputs[2].slices,
                                                sphere_sizes_inputs[2].stacks);
  /*@ assert rte_164: signed_overflow: test_counter + 12 ≤ 2147483647; */
  test_counter += 12;
  struct __anonstruct_cylinder_sizes_inputs_12 cylinder_sizes_inputs[3] =
    {{.slices = (uint32_t)3, .stacks = (uint32_t)1},
     {.slices = (uint32_t)5, .stacks = (uint32_t)2},
     {.slices = (uint32_t)10, .stacks = (uint32_t)5}};
  cylinder_sizes_results[0] = sshape_cylinder_sizes(cylinder_sizes_inputs[0].slices,
                                                    cylinder_sizes_inputs[0].stacks);
  cylinder_sizes_results[1] = sshape_cylinder_sizes(cylinder_sizes_inputs[1].slices,
                                                    cylinder_sizes_inputs[1].stacks);
  cylinder_sizes_results[2] = sshape_cylinder_sizes(cylinder_sizes_inputs[2].slices,
                                                    cylinder_sizes_inputs[2].stacks);
  struct __anonstruct_torus_sizes_inputs_13 torus_sizes_inputs[3] =
    {{.sides = (uint32_t)3, .rings = (uint32_t)3},
     {.sides = (uint32_t)5, .rings = (uint32_t)5},
     {.sides = (uint32_t)10, .rings = (uint32_t)8}};
  torus_sizes_results[0] = sshape_torus_sizes(torus_sizes_inputs[0].sides,
                                              torus_sizes_inputs[0].rings);
  torus_sizes_results[1] = sshape_torus_sizes(torus_sizes_inputs[1].sides,
                                              torus_sizes_inputs[1].rings);
  torus_sizes_results[2] = sshape_torus_sizes(torus_sizes_inputs[2].sides,
                                              torus_sizes_inputs[2].rings);
  element_vertices.buffer.ptr = (void const *)(element_vertex_data);
  element_vertices.buffer.size = (size_t)500;
  element_vertices.data_size = (size_t)240;
  element_vertices.shape_offset = (size_t)0;
  element_indices.buffer.ptr = (void const *)(element_index_data);
  element_indices.buffer.size = (size_t)500;
  element_indices.data_size = (size_t)120;
  element_indices.shape_offset = (size_t)60;
  element_buf.vertices = element_vertices;
  element_buf.indices = element_indices;
  element_buf.valid = (_Bool)1;
  sshape_element_range_t element_range1 =
    sshape_element_range((sshape_buffer_t const *)(& element_buf));
  /*@ assert rte_165: signed_overflow: test_counter + 2 ≤ 2147483647; */
  test_counter += 2;
  element_indices2.buffer.ptr = (void const *)(element_index_data);
  element_indices2.buffer.size = (size_t)500;
  element_indices2.data_size = (size_t)0;
  element_indices2.shape_offset = (size_t)0;
  element_buf2.vertices = element_vertices;
  element_buf2.indices = element_indices2;
  element_buf2.valid = (_Bool)1;
  element_indices3.buffer.ptr = (void const *)(element_index_data);
  element_indices3.buffer.size = (size_t)500;
  element_indices3.data_size = (size_t)200;
  element_indices3.shape_offset = (size_t)100;
  element_buf3.vertices = element_vertices;
  element_buf3.indices = element_indices3;
  element_buf3.valid = (_Bool)1;
  sshape_element_range_t element_range3 =
    sshape_element_range((sshape_buffer_t const *)(& element_buf3));
  /*@ assert rte_166: signed_overflow: test_counter + 2 ≤ 2147483647; */
  test_counter += 2;
  __retres = 0;
  return __retres;
}