#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include <float.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <errno.h>
#include <time.h>


  /*
   * CGL types
   */


typedef unsigned char CGL_ubyte;
typedef unsigned short CGL_ushort;
typedef unsigned int CGL_uint;
typedef unsigned long CGL_ulong;
typedef unsigned long long CGL_ulonglong;
typedef char CGL_byte;
typedef short CGL_short;
typedef int CGL_int;
typedef long CGL_long;
typedef long long CGL_longlong;
typedef float CGL_float;
typedef double CGL_double;
typedef long double CGL_longdouble;
typedef size_t CGL_sizei;
typedef bool CGL_bool;
typedef void CGL_void;

#define CGL_TRUE true
#define CGL_FALSE false


#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })



// math

struct CGL_vec2
{
	CGL_float x;
	CGL_float y;
};
typedef struct CGL_vec2 CGL_vec2;


struct CGL_vec3
{
	CGL_float x;
	CGL_float y;
	CGL_float z;
};
typedef struct CGL_vec3 CGL_vec3;


struct CGL_vec4
{
	CGL_float x;
	CGL_float y;
	CGL_float z;
	CGL_float w;
};
typedef struct CGL_vec4 CGL_vec4;
typedef struct CGL_vec4 CGL_color;


struct CGL_ivec4
{
	CGL_int x;
	CGL_int y;
	CGL_int z;
	CGL_int w;
};
typedef struct CGL_ivec4 CGL_ivec4;


struct CGL_mat3
{
	CGL_float m[9];
};
typedef struct CGL_mat3 CGL_mat3;


struct CGL_mat4
{
	CGL_float m[16];
};
typedef struct CGL_mat4 CGL_mat4;


struct CGL_quat
{
	CGL_vec3 vec;
	CGL_float w;
};
typedef struct CGL_quat CGL_quat;

// math functions with macros

#define CGL_PI (3.14159265358979323846f)
#define CGL_2PI (6.28318530717958647692f)
#define CGL_PI_2 (1.57079632679489661923f)
#define CGL_E (2.71828182845904523536f)
#define CGL_SQRT2 (1.41421356237309504880f)
#define CGL_SQRT3 (1.73205080756887729352f)
#define CGL_SQRT5 (2.23606797749978969640f)
#define CGL_SQRT6 (2.44948974278317809820f)
#define CGL_SQRT7 (2.64575131106459059050f)
#define CGL_SQRT8 (2.82842712474619009760f)
#define CGL_deg_to_rad(deg) ((deg) * (CGL_PI / 180.0f))
#define CGL_rad_to_deg(rad) ((rad) * (180.0f / CGL_PI))

#define CGL_float_lerp(a, b, t) (((a) * (1.0f - (t))) + ((b) * (t)))


struct CGL_list
{
	size_t size;
	size_t capacity;
	size_t item_size;
	CGL_float increase_factor;
	void* data;
};
typedef struct CGL_list CGL_list;


struct CGL_image
{
	void* data;
	CGL_int height;
	CGL_int width;
	CGL_int bytes_per_channel;
	CGL_int channels;
};
typedef struct CGL_image CGL_image;


struct CGL_mesh_vertex
{
	CGL_vec4 position;
	CGL_vec4 normal;
	CGL_vec4 texture_coordinates;
	CGL_vec4 bone_wieghts;
	CGL_ivec4 bone_ids;
};
typedef struct CGL_mesh_vertex CGL_mesh_vertex;


struct CGL_mesh_cpu
{
	size_t index_count;
	size_t index_count_used;
	uint32_t* indices;
	size_t vertex_count;
	size_t vertex_count_used;
	CGL_mesh_vertex* vertices;
};
typedef struct CGL_mesh_cpu CGL_mesh_cpu;



CGL_void CGL_list_set_increase_factor(CGL_list* list, CGL_float increase_factor)
{
	list->increase_factor = increase_factor;
}



CGL_float CGL_list_get_increase_factor(CGL_list* list)
{
	return list->increase_factor;
}



CGL_sizei CGL_list_get_item_size(CGL_list* list)
{
	return list->item_size;
}



CGL_sizei CGL_list_get_size(CGL_list* list)
{
	return list->size;
}



CGL_sizei CGL_list_get_capacity(CGL_list* list)
{
	return list->capacity;
}



CGL_bool CGL_list_is_empty(CGL_list* list)
{
	return list->size == 0;
}



CGL_void CGL_list_clear(CGL_list* list)
{
	list->size = 0;
}



CGL_float CGL_utils_sigmoid_derivative(CGL_float x)
{
	return x * (1.0f - x);
}



CGL_float CGL_utils_relu(CGL_float x)
{
	return x > 0.0f ? x : 0.0f;
}



CGL_float CGL_utils_relu_derivative(CGL_float x)
{
	return x > 0.0f ? 1.0f : 0.0f;
}



CGL_float CGL_utils_tanh_derivative(CGL_float x)
{
	return 1.0f - x * x;
}



CGL_float CGL_utils_step(CGL_float x)
{
	return x > 0.0f ? 1.0f : 0.0f;
}



CGL_float CGL_utils_step_derivative(CGL_float x)
{
	(void)x;
	return 0.0f;
}



CGL_float CGL_utils_relu_leaky(CGL_float x)
{
	return x > 0.0f ? x : 0.01f * x;
}



CGL_float CGL_utils_relu_leaky_derivative(CGL_float x)
{
	return x > 0.0f ? 1.0f : 0.01f;
}



CGL_float CGL_utils_relu_smooth(CGL_float x)
{
	return x > 0.0f ? x : 0.01f * x * x;
}



CGL_float CGL_utils_relu_smooth_derivative(CGL_float x)
{
	return x > 0.0f ? 1.0f : 0.02f * x;
}
