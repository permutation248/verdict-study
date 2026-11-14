#include <assert.h>
#include <stddef.h>
#include <stdint.h>
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

uint32_t _sshape_pack_ub4_ubyte4n(uint8_t x, uint8_t y, uint8_t z, uint8_t w)
{
  uint32_t __retres;
  __retres = (((uint32_t)w * (uint32_t)0x1000000 + (uint32_t)z * (uint32_t)0x10000) + 
              (uint32_t)y * (uint32_t)0x100) + (uint32_t)x;
  return __retres;
}

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

uint16_t _sshape_pack_f_ushortn(float x)
{
  uint16_t __retres;
  /*@ assert rte_22: is_nan_or_infinite: \is_finite(\mul_float(x, 65535.0f)); */
  __retres = (uint16_t)(x * 65535.0f);
  return __retres;
}

_sshape_vec4_t _sshape_vec4(float x, float y, float z, float w)
{
  _sshape_vec4_t v = {.x = x, .y = y, .z = z, .w = w};
  return v;
}

_sshape_vec2_t _sshape_vec2(float x, float y)
{
  _sshape_vec2_t v = {.x = x, .y = y};
  return v;
}

_Bool _sshape_mat4_isnull(sshape_mat4_t const *m)
{
  _Bool __retres;
  {
    int y = 0;
    while (y < 4) {
      {
        int x = 0;
        while (x < 4) {
          /*@ assert rte_1: index_bound: 0 <= y; */
          /*@ assert rte_2: index_bound: y < 4; */
          /*@ assert rte_3: index_bound: 0 <= x; */
          /*@ assert rte_4: index_bound: x < 4; */
          /*@ assert rte_5: mem_access: \valid_read(&m->m[y][x]); */
          if (0.0f != m->m[y][x]) {
            __retres = (_Bool)0;
            goto return_label;
          }
          /*@ assert rte_6: signed_overflow: x + 1 <= 2147483647; */
          x ++;
        }
      }
      /*@ assert rte_7: signed_overflow: y + 1 <= 2147483647; */
      y ++;
    }
  }
  __retres = (_Bool)1;
  return_label: return __retres;
}

sshape_mat4_t _sshape_mat4_identity(void)
{
  sshape_mat4_t m =
    {.m = {{1.0f, 0.0f, 0.0f, 0.0f},
           {0.0f, 1.0f, 0.0f, 0.0f},
           {0.0f, 0.0f, 1.0f, 0.0f},
           {0.0f, 0.0f, 0.0f, 1.0f}}};
  return m;
}

_sshape_vec4_t _sshape_mat4_mul(sshape_mat4_t const *m, _sshape_vec4_t v)
{
  /*@ assert rte_23: mem_access: \valid_read(&m->m[0][0]); */
  /*@ assert rte_24: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][0], v.x)); */
  /*@ assert rte_25: mem_access: \valid_read(&m->m[1][0]); */
  /*@ assert rte_26: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][0], v.y)); */
  /*@ assert rte_27: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m->m[0][0], v.x), \mul_float(m->m[1][0], v.y))); */
  /*@ assert rte_28: mem_access: \valid_read(&m->m[2][0]); */
  /*@ assert rte_29: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][0], v.z)); */
  /*@ assert rte_30: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(m->m[0][0], v.x), \mul_float(m->m[1][0], v.y)), \mul_float(m->m[2][0], v.z))); */
  /*@ assert rte_31: mem_access: \valid_read(&m->m[3][0]); */
  /*@ assert rte_32: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][0], v.w)); */
  /*@ assert rte_33: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][0], v.x), \mul_float(m->m[1][0], v.y)), \mul_float(m->m[2][0], v.z)), \mul_float(m->m[3][0], v.w))); */
  /*@ assert rte_34: mem_access: \valid_read(&m->m[0][1]); */
  /*@ assert rte_35: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][1], v.x)); */
  /*@ assert rte_36: mem_access: \valid_read(&m->m[1][1]); */
  /*@ assert rte_37: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][1], v.y)); */
  /*@ assert rte_38: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m->m[0][1], v.x), \mul_float(m->m[1][1], v.y))); */
  /*@ assert rte_39: mem_access: \valid_read(&m->m[2][1]); */
  /*@ assert rte_40: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][1], v.z)); */
  /*@ assert rte_41: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(m->m[0][1], v.x), \mul_float(m->m[1][1], v.y)), \mul_float(m->m[2][1], v.z))); */
  /*@ assert rte_42: mem_access: \valid_read(&m->m[3][1]); */
  /*@ assert rte_43: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][1], v.w)); */
  /*@ assert rte_44: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][1], v.x), \mul_float(m->m[1][1], v.y)), \mul_float(m->m[2][1], v.z)), \mul_float(m->m[3][1], v.w))); */
  /*@ assert rte_45: mem_access: \valid_read(&m->m[0][2]); */
  /*@ assert rte_46: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][2], v.x)); */
  /*@ assert rte_47: mem_access: \valid_read(&m->m[1][2]); */
  /*@ assert rte_48: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][2], v.y)); */
  /*@ assert rte_49: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m->m[0][2], v.x), \mul_float(m->m[1][2], v.y))); */
  /*@ assert rte_50: mem_access: \valid_read(&m->m[2][2]); */
  /*@ assert rte_51: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][2], v.z)); */
  /*@ assert rte_52: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(m->m[0][2], v.x), \mul_float(m->m[1][2], v.y)), \mul_float(m->m[2][2], v.z))); */
  /*@ assert rte_53: mem_access: \valid_read(&m->m[3][2]); */
  /*@ assert rte_54: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][2], v.w)); */
  /*@ assert rte_55: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][2], v.x), \mul_float(m->m[1][2], v.y)), \mul_float(m->m[2][2], v.z)), \mul_float(m->m[3][2], v.w))); */
  /*@ assert rte_56: mem_access: \valid_read(&m->m[0][3]); */
  /*@ assert rte_57: is_nan_or_infinite: \is_finite(\mul_float(m->m[0][3], v.x)); */
  /*@ assert rte_58: mem_access: \valid_read(&m->m[1][3]); */
  /*@ assert rte_59: is_nan_or_infinite: \is_finite(\mul_float(m->m[1][3], v.y)); */
  /*@ assert rte_60: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m->m[0][3], v.x), \mul_float(m->m[1][3], v.y))); */
  /*@ assert rte_61: mem_access: \valid_read(&m->m[2][3]); */
  /*@ assert rte_62: is_nan_or_infinite: \is_finite(\mul_float(m->m[2][3], v.z)); */
  /*@ assert rte_63: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(m->m[0][3], v.x), \mul_float(m->m[1][3], v.y)), \mul_float(m->m[2][3], v.z))); */
  /*@ assert rte_64: mem_access: \valid_read(&m->m[3][3]); */
  /*@ assert rte_65: is_nan_or_infinite: \is_finite(\mul_float(m->m[3][3], v.w)); */
  /*@ assert rte_66: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(m->m[0][3], v.x), \mul_float(m->m[1][3], v.y)), \mul_float(m->m[2][3], v.z)), \mul_float(m->m[3][3], v.w))); */
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

uint32_t _sshape_plane_num_vertices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = (tiles + (uint32_t)1) * (tiles + (uint32_t)1);
  return __retres;
}

uint32_t _sshape_plane_num_indices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = ((tiles * tiles) * (uint32_t)2) * (uint32_t)3;
  return __retres;
}

uint32_t _sshape_box_num_vertices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = ((tiles + (uint32_t)1) * (tiles + (uint32_t)1)) * (uint32_t)6;
  return __retres;
}

uint32_t _sshape_box_num_indices(uint32_t tiles)
{
  uint32_t __retres;
  __retres = (((tiles * tiles) * (uint32_t)2) * (uint32_t)6) * (uint32_t)3;
  return __retres;
}

uint32_t _sshape_sphere_num_vertices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (slices + (uint32_t)1) * (stacks + (uint32_t)1);
  return __retres;
}

uint32_t _sshape_sphere_num_indices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (((uint32_t)2 * slices) * stacks - (uint32_t)2 * slices) * (uint32_t)3;
  return __retres;
}

uint32_t _sshape_cylinder_num_vertices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (slices + (uint32_t)1) * (stacks + (uint32_t)5);
  return __retres;
}

uint32_t _sshape_cylinder_num_indices(uint32_t slices, uint32_t stacks)
{
  uint32_t __retres;
  __retres = (((uint32_t)2 * slices) * stacks + (uint32_t)2 * slices) * (uint32_t)3;
  return __retres;
}

uint32_t _sshape_torus_num_vertices(uint32_t sides, uint32_t rings)
{
  uint32_t __retres;
  __retres = (sides + (uint32_t)1) * (rings + (uint32_t)1);
  return __retres;
}

uint32_t _sshape_torus_num_indices(uint32_t sides, uint32_t rings)
{
  uint32_t __retres;
  __retres = ((sides * rings) * (uint32_t)2) * (uint32_t)3;
  return __retres;
}

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

void _sshape_advance_offset(sshape_buffer_item_t *item)
{
  /*@ assert rte_67: mem_access: \valid(&item->shape_offset); */
  /*@ assert rte_68: mem_access: \valid_read(&item->data_size); */
  item->shape_offset = item->data_size;
  return;
}

uint16_t _sshape_base_index(sshape_buffer_t const *buf)
{
  uint16_t __retres;
  /*@ assert rte_69: mem_access: \valid_read(&buf->vertices.data_size); */
  __retres = (uint16_t)(buf->vertices.data_size / sizeof(sshape_vertex_t));
  return __retres;
}

uint32_t sshape_color_4b(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  uint32_t tmp;
  tmp = _sshape_pack_ub4_ubyte4n(r,g,b,a);
  return tmp;
}

uint32_t sshape_color_3b(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t tmp;
  tmp = _sshape_pack_ub4_ubyte4n(r,g,b,(uint8_t)255);
  return tmp;
}

sshape_sizes_t sshape_plane_sizes(uint32_t tiles)
{
  __FC_assert((tiles >= (uint32_t)1) != 0,
              "/tmp/verdict_m0szgylc/sokol_shape.c",489,"tiles >= 1");
  sshape_sizes_t res =
    {.vertices = {.num = (uint32_t)0, .size = 0U},
     .indices = {.num = 0U, .size = 0U}};
  res.vertices.num = _sshape_plane_num_vertices(tiles);
  res.indices.num = _sshape_plane_num_indices(tiles);
  res.vertices.size = (uint32_t)((unsigned long)res.vertices.num * sizeof(sshape_vertex_t));
  res.indices.size = (uint32_t)((unsigned long)res.indices.num * sizeof(uint16_t));
  return res;
}

sshape_sizes_t sshape_box_sizes(uint32_t tiles)
{
  __FC_assert((tiles >= (uint32_t)1) != 0,
              "/tmp/verdict_m0szgylc/sokol_shape.c",501,"tiles >= 1");
  sshape_sizes_t res =
    {.vertices = {.num = (uint32_t)0, .size = 0U},
     .indices = {.num = 0U, .size = 0U}};
  res.vertices.num = _sshape_box_num_vertices(tiles);
  res.indices.num = _sshape_box_num_indices(tiles);
  res.vertices.size = (uint32_t)((unsigned long)res.vertices.num * sizeof(sshape_vertex_t));
  res.indices.size = (uint32_t)((unsigned long)res.indices.num * sizeof(uint16_t));
  return res;
}

sshape_sizes_t sshape_sphere_sizes(uint32_t slices, uint32_t stacks)
{
  int tmp;
  if (slices >= (uint32_t)3) 
    if (stacks >= (uint32_t)2) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_m0szgylc/sokol_shape.c",513,
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

sshape_sizes_t sshape_cylinder_sizes(uint32_t slices, uint32_t stacks)
{
  int tmp;
  if (slices >= (uint32_t)3) 
    if (stacks >= (uint32_t)1) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_m0szgylc/sokol_shape.c",525,
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

sshape_sizes_t sshape_torus_sizes(uint32_t sides, uint32_t rings)
{
  int tmp;
  if (sides >= (uint32_t)3) 
    if (rings >= (uint32_t)3) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_m0szgylc/sokol_shape.c",537,
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

sshape_element_range_t sshape_element_range(sshape_buffer_t const *buf)
{
  int tmp;
  if (buf) 
    /*@ assert rte_70: bool_value: buf->valid == 0 || buf->valid == 1; */
    /*@ assert rte_71: mem_access: \valid_read(&buf->valid); */
    if (buf->valid) tmp = 1; else tmp = 0;
  else tmp = 0;
  __FC_assert(tmp != 0,"/tmp/verdict_m0szgylc/sokol_shape.c",549,
              "buf && buf->valid");
  /*@ assert rte_72: mem_access: \valid_read(&buf->indices.shape_offset); */
  /*@ assert rte_73: mem_access: \valid_read(&buf->indices.data_size); */
  __FC_assert((buf->indices.shape_offset < buf->indices.data_size) != 0,
              "/tmp/verdict_m0szgylc/sokol_shape.c",550,
              "buf->indices.shape_offset < buf->indices.data_size");
  /*@ assert rte_74: mem_access: \valid_read(&buf->indices.shape_offset); */
  __FC_assert(((unsigned long)0 == (buf->indices.shape_offset & (sizeof(uint16_t) - (unsigned long)1))) != 0,
              "/tmp/verdict_m0szgylc/sokol_shape.c",551,
              "0 == (buf->indices.shape_offset & (sizeof(uint16_t) - 1))");
  /*@ assert rte_75: mem_access: \valid_read(&buf->indices.data_size); */
  __FC_assert(((unsigned long)0 == (buf->indices.data_size & (sizeof(uint16_t) - (unsigned long)1))) != 0,
              "/tmp/verdict_m0szgylc/sokol_shape.c",552,
              "0 == (buf->indices.data_size & (sizeof(uint16_t) - 1))");
  sshape_element_range_t range = {.base_element = 0, .num_elements = 0};
  /*@ assert rte_76: mem_access: \valid_read(&buf->indices.shape_offset); */
  range.base_element = (int)(buf->indices.shape_offset / sizeof(uint16_t));
  /*@ assert rte_77: bool_value: buf->valid == 0 || buf->valid == 1; */
  /*@ assert rte_78: mem_access: \valid_read(&buf->valid); */
  if (buf->valid) 
    /*@ assert rte_79: mem_access: \valid_read(&buf->indices.data_size); */
    /*@ assert rte_80: mem_access: \valid_read(&buf->indices.shape_offset); */
    range.num_elements = (int)((buf->indices.data_size - buf->indices.shape_offset) / sizeof(uint16_t));
  else range.num_elements = 0;
  return range;
}



