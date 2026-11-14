#include <errno.h>

#include <math.h>

typedef float zpl_f32;

struct zpl_vec2 {
	zpl_f32 x ;
	zpl_f32 y ;
};

typedef struct zpl_vec2 zpl_vec2;

struct zpl_vec3 {
	zpl_f32 x ;
	zpl_f32 y ;
	zpl_f32 z ;
};

typedef struct zpl_vec3 zpl_vec3;

struct zpl_vec4 {
	zpl_f32 x ;
	zpl_f32 y ;
	zpl_f32 z ;
	zpl_f32 w ;
};

typedef struct zpl_vec4 zpl_vec4;

typedef zpl_vec4 zpl_quat;

struct zpl_mat2 {
	zpl_vec2 col[2] ;
};

typedef struct zpl_mat2 zpl_mat2;

struct zpl_mat4 {
	zpl_vec4 col[4] ;
};

typedef struct zpl_mat4 zpl_mat4;

struct zpl_rect2 {
	zpl_vec2 pos ;
	zpl_vec2 dim ;
};

typedef struct zpl_rect2 zpl_rect2;

struct zpl_aabb2 {
	zpl_vec2 min ;
	zpl_vec2 max ;
};

typedef struct zpl_aabb2 zpl_aabb2;

typedef zpl_f32 zpl_float2[2];

typedef zpl_f32 zpl_float4[4];

/*@
	requires \is_finite(x);
	requires x >= 0.0f;
	assigns \nothing;
	ensures \result >= 0.0f;
	ensures (x >= 0.0f) ==> (\result * \result <= x + 0x1p-20f && x <= \result * \result + 0x1p-20f);
	ensures (x >= 0.0f) ==> (\result >= 0.0f && \result * \result <= x * (1.0f + 0x1p-23f) + 0x1p-126f);
	ensures (x == 0.0f) ==> (\result == 0.0f);
	ensures (x > 0.0f) ==> (\result > 0.0f);
*/
zpl_f32 zpl_sqrt(zpl_f32 x)
{
	zpl_f32 tmp;
	tmp = sqrtf(x);
	return tmp;
}

/*@
	requires \is_finite(x);
	requires x > 0.0f;
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result > 0.0f;
	ensures \result * \result * x >= 1.0f - 0x1p-23f;
	ensures \result * \result * x <= 1.0f + 0x1p-23f;
*/
zpl_f32 zpl_rsqrt(zpl_f32 x)
{
	zpl_f32 __retres;
	float tmp;
	tmp = sqrtf(x);
/*@ assert rte_306: is_nan_or_infinite: \is_finite(\div_float(1.0f, tmp)); */
	__retres = 1.0f / tmp;
	return __retres;
}

/*@
	requires \is_finite(x);
	requires x >= -0x1p127f && x <= 0x1p127f;
	assigns \nothing;
	ensures \is_finite(x) ==> \is_finite(\result);
	ensures \is_finite(x) ==> -1.0f <= \result <= 1.0f;
	ensures !\is_finite(x) ==> !\is_finite(\result);
*/
zpl_f32 zpl_sin(zpl_f32 x)
{
	zpl_f32 tmp;
	tmp = sinf(x);
	return tmp;
}

/*@
	requires \is_finite(x);
	requires \abs(x) < 0x1p127f;
	assigns \nothing;
	ensures \is_finite(x) ==> \is_finite(\result);
	ensures \is_finite(x) ==> -1.0f <= \result <= 1.0f;
	ensures !\is_finite(x) ==> !\is_finite(\result);
*/
zpl_f32 zpl_cos(zpl_f32 x)
{
	zpl_f32 tmp;
	tmp = cosf(x);
	return tmp;
}

/*@
	requires \is_finite(degrees);
	requires (float)(-0x1.45f306p+125f) <= degrees <= (float)(0x1.45f306p+125f);
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result == \div_float(\mul_float(degrees, 0x1.921fb6p1f), 360.0f);
	ensures degrees == 0.0f ==> \result == 0.0f;
	ensures degrees > 0.0f ==> \result > 0.0f;
	ensures degrees < 0.0f ==> \result < 0.0f;
*/
zpl_f32 zpl_to_radians(zpl_f32 degrees)
{
	zpl_f32 __retres;
/*@ assert rte_199: is_nan_or_infinite: \is_finite(\mul_float(degrees, 0x1.921fb6p1f)); */
/*@ assert rte_200: is_nan_or_infinite: \is_finite(\div_float(\mul_float(degrees, 0x1.921fb6p1f), 360.0f)); */
	__retres = (degrees * 0x1.921fb6p1f) / 360.0f;
	return __retres;
}

/*@
	requires \is_finite(radians);
	requires -0x1.745d1ap23f <= radians <= 0x1.745d1ap23f;
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result == (radians * 360.0f) / 0x1.921fb6p1f;
	ensures radians > 0.0f ==> \result > 0.0f;
	ensures radians < 0.0f ==> \result < 0.0f;
	ensures radians == 0.0f ==> \result == 0.0f;
	ensures \result >= -0x1.68p9f ==> \true;
*/
zpl_f32 zpl_to_degrees(zpl_f32 radians)
{
	zpl_f32 __retres;
/*@ assert rte_197: is_nan_or_infinite: \is_finite(\mul_float(radians, 360.0f)); */
/*@ assert rte_198: is_nan_or_infinite: \is_finite(\div_float(\mul_float(radians, 360.0f), 0x1.921fb6p1f)); */
	__retres = (radians * 360.0f) / 0x1.921fb6p1f;
	return __retres;
}

/*@
	requires \is_finite(x);
	requires x <= 0x1.fffffep+127f - 0.5f;
	assigns \result \from x;
	ensures \is_finite(\result);
	ensures \result - \result == 0.0;
*/
zpl_f32 zpl_round(zpl_f32 x)
{
	zpl_f32 tmp;
/*@ assert rte_193: is_nan_or_infinite: \is_finite(\add_float(x, 0.5f)); */
	tmp = floorf(x + 0.5f);
	return tmp;
}

/*@
	requires x / y <= 0x1.fffffep+127f - 0.5f;
	requires \is_finite(x);
	requires \is_finite(y);
	requires y != 0.0f;
	requires \is_finite(\div_float(x, y));
	requires \div_float(x, y) <= 0x1.fffffep+127f - 0.5f;
	requires \is_finite(\mul_float(\div_float(x, y), y));
	requires \is_finite(\sub_float(x, \mul_float(\div_float(x, y), y)));
	assigns \result \from x, y;
	ensures \is_finite(\result);
	ensures \result - \result == 0.0f;
*/
zpl_f32 zpl_remainder(zpl_f32 x, zpl_f32 y)
{
	zpl_f32 __retres;
	zpl_f32 tmp;
/*@ assert rte_194: is_nan_or_infinite: \is_finite(\div_float(x, y)); */
	tmp = zpl_round(x / y);
	;
/*@ assert rte_195: is_nan_or_infinite: \is_finite(\mul_float(tmp, y)); */
/*@ assert rte_196: is_nan_or_infinite: \is_finite(\sub_float(x, \mul_float(tmp, y))); */
	__retres = x - tmp * y;
	return __retres;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result.x == (zpl_f32)0;
	ensures \result.y == (zpl_f32)0;
*/
zpl_vec2 zpl_vec2f_zero(void)
{
zpl_vec2 v = {.x = (zpl_f32)0, .y = (zpl_f32)0};
	return v;
}

/*@
	requires \is_finite(x);
	requires \is_finite(y);
	assigns \nothing;
	ensures \result.x == x;
	ensures \result.y == y;
*/
zpl_vec2 zpl_vec2f(zpl_f32 x, zpl_f32 y)
{
	zpl_vec2 v;
	v.x = x;
	v.y = y;
	return v;
}

/*@
	requires \valid_read(x + (0..1));
	assigns \nothing;
	ensures \result.x == x[0];
	ensures \result.y == x[1];
*/
zpl_vec2 zpl_vec2fv(zpl_f32 x[2])
{
	zpl_vec2 v;
/*@ assert rte_248: mem_access: \valid_read(x + 0); */
	v.x = *(x + 0);
/*@ assert rte_249: mem_access: \valid_read(x + 1); */
	v.y = *(x + 1);
	return v;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result.x == 0.0;
	ensures \result.y == 0.0;
	ensures \result.z == 0.0;
*/
zpl_vec3 zpl_vec3f_zero(void)
{
zpl_vec3 v = {.x = (zpl_f32)0, .y = (zpl_f32)0, .z = (zpl_f32)0};
	return v;
}

/*@
	requires \is_finite(x);
	requires \is_finite(y);
	requires \is_finite(z);
	assigns \nothing;
	ensures \result.x == x;
	ensures \result.y == y;
	ensures \result.z == z;
*/
zpl_vec3 zpl_vec3f(zpl_f32 x, zpl_f32 y, zpl_f32 z)
{
	zpl_vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result.x == 0.0f;
	ensures \result.y == 0.0f;
	ensures \result.z == 0.0f;
	ensures \result.w == 0.0f;
*/
zpl_vec4 zpl_vec4f_zero(void)
{
	zpl_vec4 v =
{.x = (zpl_f32)0, .y = (zpl_f32)0, .z = (zpl_f32)0, .w = (zpl_f32)0};
	return v;
}

/*@
	requires \is_finite(x);
	requires \is_finite(y);
	requires \is_finite(z);
	requires \is_finite(w);
	assigns \nothing;
	ensures \result.x == x;
	ensures \result.y == y;
	ensures \result.z == z;
	ensures \result.w == w;
*/
zpl_vec4 zpl_vec4f(zpl_f32 x, zpl_f32 y, zpl_f32 z, zpl_f32 w)
{
	zpl_vec4 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

/*@
	requires \is_finite(v0.x);
	requires \is_finite(v0.y);
	requires \is_finite(v1.x);
	requires \is_finite(v1.y);
	requires \is_finite((float)(v0.x * v1.x));
	requires \is_finite((float)(v0.y * v1.y));
	requires \is_finite((float)((float)(v0.x * v1.x) + (float)(v0.y * v1.y)));
	assigns \nothing;
	ensures \is_finite(v0.x) && \is_finite(v1.x) && \is_finite(v0.y) && \is_finite(v1.y) &&\is_finite(\mul_float(v0.x, v1.x)) && \is_finite(\mul_float(v0.y, v1.y)) &&\is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)))==> \result == \add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y));
	ensures \is_finite(v0.x) && \is_finite(v1.x) && \is_finite(v0.y) && \is_finite(v1.y) &&\is_finite(\mul_float(v0.x, v1.x)) && \is_finite(\mul_float(v0.y, v1.y)) &&\is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)))==> \is_finite(\result);
*/
zpl_f32 zpl_vec2_dot(zpl_vec2 v0, zpl_vec2 v1)
{
	zpl_f32 __retres;
/*@ assert rte_233: is_nan_or_infinite: \is_finite(\mul_float(v0.x, v1.x)); */
/*@ assert rte_234: is_nan_or_infinite: \is_finite(\mul_float(v0.y, v1.y)); */
/*@ assert rte_235: is_nan_or_infinite: \is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y))); */
	__retres = v0.x * v1.x + v0.y * v1.y;
	return __retres;
}

/*@
	requires \is_finite(v0.x) && \is_finite(v0.y) && \is_finite(v0.z);
	requires \is_finite(v1.x) && \is_finite(v1.y) && \is_finite(v1.z);
	requires \is_finite(\mul_float(v0.x, v1.x));
	requires \is_finite(\mul_float(v0.y, v1.y));
	requires \is_finite(\mul_float(v0.z, v1.z));
	requires \is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)));
	requires \is_finite(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z)));
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result == \add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z));
	ensures (\is_finite(v0.x) && \is_finite(v1.x) &&\is_finite(v0.y) && \is_finite(v1.y) &&\is_finite(v0.z) && \is_finite(v1.z) &&\is_finite(\mul_float(v0.x, v1.x)) &&\is_finite(\mul_float(v0.y, v1.y)) &&\is_finite(\mul_float(v0.z, v1.z)) &&\is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y))) &&\is_finite(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z))))==>\result == \add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z));
*/
zpl_f32 zpl_vec3_dot(zpl_vec3 v0, zpl_vec3 v1)
{
	zpl_f32 __retres;
/*@ assert rte_292: is_nan_or_infinite: \is_finite(\mul_float(v0.x, v1.x)); */
/*@ assert rte_293: is_nan_or_infinite: \is_finite(\mul_float(v0.y, v1.y)); */
/*@ assert rte_294: is_nan_or_infinite: \is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y))); */
/*@ assert rte_295: is_nan_or_infinite: \is_finite(\mul_float(v0.z, v1.z)); */
/*@ assert rte_296: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z))); */
	__retres = (v0.x * v1.x + v0.y * v1.y) + v0.z * v1.z;
	return __retres;
}

/*@
	requires \is_finite(v0.x) && \is_finite(v0.y) && \is_finite(v0.z) && \is_finite(v0.w);
	requires \is_finite(v1.x) && \is_finite(v1.y) && \is_finite(v1.z) && \is_finite(v1.w);
	requires \is_finite(\mul_float(v0.x, v1.x));
	requires \is_finite(\mul_float(v0.y, v1.y));
	requires \is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)));
	requires \is_finite(\mul_float(v0.z, v1.z));
	requires \is_finite(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z)));
	requires \is_finite(\mul_float(v0.w, v1.w));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z)), \mul_float(v0.w, v1.w)));
	assigns \nothing;
	ensures \is_finite(v0.x) && \is_finite(v0.y) && \is_finite(v0.z) && \is_finite(v0.w) &&\is_finite(v1.x) && \is_finite(v1.y) && \is_finite(v1.z) && \is_finite(v1.w) &&\is_finite(\mul_float(v0.x, v1.x)) &&\is_finite(\mul_float(v0.y, v1.y)) &&\is_finite(\mul_float(v0.z, v1.z)) &&\is_finite(\mul_float(v0.w, v1.w)) &&\is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y))) &&\is_finite(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z))) &&\is_finite(\add_float(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z)), \mul_float(v0.w, v1.w)))==>\result == \add_float(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z)), \mul_float(v0.w, v1.w));
	ensures \is_finite(v0.x) && \is_finite(v0.y) && \is_finite(v0.z) && \is_finite(v0.w) &&\is_finite(v1.x) && \is_finite(v1.y) && \is_finite(v1.z) && \is_finite(v1.w) &&\is_finite(\mul_float(v0.x, v1.x)) &&\is_finite(\mul_float(v0.y, v1.y)) &&\is_finite(\mul_float(v0.z, v1.z)) &&\is_finite(\mul_float(v0.w, v1.w)) &&\is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y))) &&\is_finite(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z))) &&\is_finite(\add_float(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z)), \mul_float(v0.w, v1.w)))==>\is_finite(\result);
*/
zpl_f32 zpl_vec4_dot(zpl_vec4 v0, zpl_vec4 v1)
{
	zpl_f32 __retres;
/*@ assert rte_368: is_nan_or_infinite: \is_finite(\mul_float(v0.x, v1.x)); */
/*@ assert rte_369: is_nan_or_infinite: \is_finite(\mul_float(v0.y, v1.y)); */
/*@ assert rte_370: is_nan_or_infinite: \is_finite(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y))); */
/*@ assert rte_371: is_nan_or_infinite: \is_finite(\mul_float(v0.z, v1.z)); */
/*@ assert rte_372: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z))); */
/*@ assert rte_373: is_nan_or_infinite: \is_finite(\mul_float(v0.w, v1.w)); */
/*@ assert rte_374: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(v0.x, v1.x), \mul_float(v0.y, v1.y)), \mul_float(v0.z, v1.z)), \mul_float(v0.w, v1.w))); */
	__retres = ((v0.x * v1.x + v0.y * v1.y) + v0.z * v1.z) + v0.w * v1.w;
	return __retres;
}

/*@
	requires \is_finite(v.x);
	requires \is_finite(v.y);
	requires \is_finite((float)(v.x * v.x));
	requires \is_finite((float)(v.y * v.y));
	requires \is_finite((float)((float)(v.x * v.x) + (float)(v.y * v.y)));
	assigns \nothing;
	ensures \is_finite(v.x) && \is_finite(v.y) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)))==> \result == \add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y));
	ensures \is_finite(v.x) && \is_finite(v.y) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)))==> \is_finite(\result);
*/
zpl_f32 zpl_vec2_mag2(zpl_vec2 v)
{
	zpl_f32 tmp;
	tmp = zpl_vec2_dot(v,v);
	return tmp;
}

/*@
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z);
	requires \is_finite(\mul_float(v.x, v.x));
	requires \is_finite(\mul_float(v.y, v.y));
	requires \is_finite(\mul_float(v.z, v.z));
	requires \is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)));
	requires \is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)));
	assigns \nothing;
	ensures \is_finite(\result) ==> \result == \add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z));
	ensures (\is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(\mul_float(v.x, v.x)) && \is_finite(\mul_float(v.y, v.y)) && \is_finite(\mul_float(v.z, v.z)) && \is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) && \is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)))) ==> \is_finite(\result);
	ensures (\is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(\mul_float(v.x, v.x)) && \is_finite(\mul_float(v.y, v.y)) && \is_finite(\mul_float(v.z, v.z)) && \is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) && \is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)))) ==> \result == \add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z));
*/
zpl_f32 zpl_vec3_mag2(zpl_vec3 v)
{
	zpl_f32 tmp;
	tmp = zpl_vec3_dot(v,v);
	return tmp;
}

/*@
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w);
	requires \is_finite(\mul_float(v.x, v.x));
	requires \is_finite(\mul_float(v.y, v.y));
	requires \is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)));
	requires \is_finite(\mul_float(v.z, v.z));
	requires \is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)));
	requires \is_finite(\mul_float(v.w, v.w));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w)));
	assigns \nothing;
	ensures \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\mul_float(v.z, v.z)) &&\is_finite(\mul_float(v.w, v.w)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) &&\is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z))) &&\is_finite(\add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w)))==>\result == \add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w));
	ensures \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\mul_float(v.z, v.z)) &&\is_finite(\mul_float(v.w, v.w)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) &&\is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z))) &&\is_finite(\add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w)))==>\is_finite(\result);
*/
zpl_f32 zpl_vec4_mag2(zpl_vec4 v)
{
	zpl_f32 tmp;
	tmp = zpl_vec4_dot(v,v);
	return tmp;
}

/*@
	requires \valid(d);
	requires \is_finite(v0.x) && \is_finite(v1.x);
	requires \is_finite(\add_float(v0.x, v1.x));
	requires \is_finite(v0.y) && \is_finite(v1.y);
	requires \is_finite(\add_float(v0.y, v1.y));
	assigns d->x, d->y;
	ensures \is_finite(v0.x) && \is_finite(v1.x) && \is_finite(\add_float(v0.x, v1.x)) ==>d->x == \add_float(v0.x, v1.x);
	ensures \is_finite(v0.y) && \is_finite(v1.y) && \is_finite(\add_float(v0.y, v1.y)) ==>d->y == \add_float(v0.y, v1.y);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
*/
void zpl_vec2_add(zpl_vec2 *d, zpl_vec2 v0, zpl_vec2 v1)
{
/*@ assert rte_229: mem_access: \valid(&d->x); */
/*@ assert rte_230: is_nan_or_infinite: \is_finite(\add_float(v0.x, v1.x)); */
	d->x = v0.x + v1.x;
/*@ assert rte_231: mem_access: \valid(&d->y); */
/*@ assert rte_232: is_nan_or_infinite: \is_finite(\add_float(v0.y, v1.y)); */
	d->y = v0.y + v1.y;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v0.x) && \is_finite(v1.x);
	requires \is_finite(v0.y) && \is_finite(v1.y);
	requires \is_finite(\sub_float(v0.x, v1.x));
	requires \is_finite(\sub_float(v0.y, v1.y));
	assigns d->x, d->y;
	ensures \is_finite(\sub_float(v0.x, v1.x)) ==> d->x == \sub_float(v0.x, v1.x);
	ensures \is_finite(\sub_float(v0.y, v1.y)) ==> d->y == \sub_float(v0.y, v1.y);
	ensures d->x == \sub_float(v0.x, v1.x);
	ensures d->y == \sub_float(v0.y, v1.y);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
*/
void zpl_vec2_sub(zpl_vec2 *d, zpl_vec2 v0, zpl_vec2 v1)
{
/*@ assert rte_221: mem_access: \valid(&d->x); */
/*@ assert rte_222: is_nan_or_infinite: \is_finite(\sub_float(v0.x, v1.x)); */
	d->x = v0.x - v1.x;
/*@ assert rte_223: mem_access: \valid(&d->y); */
/*@ assert rte_224: is_nan_or_infinite: \is_finite(\sub_float(v0.y, v1.y)); */
	d->y = v0.y - v1.y;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v.x);
	requires \is_finite(v.y);
	requires \is_finite(s);
	requires \is_finite(\mul_float(v.x, s));
	requires \is_finite(\mul_float(v.y, s));
	assigns d->x, d->y;
	ensures \is_finite(v.x) && \is_finite(s) && \is_finite(\mul_float(v.x, s)) ==>d->x == \mul_float(v.x, s);
	ensures \is_finite(v.y) && \is_finite(s) && \is_finite(\mul_float(v.y, s)) ==>d->y == \mul_float(v.y, s);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
*/
void zpl_vec2_mul(zpl_vec2 *d, zpl_vec2 v, zpl_f32 s)
{
/*@ assert rte_225: mem_access: \valid(&d->x); */
/*@ assert rte_226: is_nan_or_infinite: \is_finite(\mul_float(v.x, s)); */
	d->x = v.x * s;
/*@ assert rte_227: mem_access: \valid(&d->y); */
/*@ assert rte_228: is_nan_or_infinite: \is_finite(\mul_float(v.y, s)); */
	d->y = v.y * s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v.x);
	requires \is_finite(v.y);
	requires \is_finite(s);
	requires s != 0.0f;
	requires \abs(v.x) <= \abs(s) * 3.40282347e+38f;
	requires \abs(v.y) <= \abs(s) * 3.40282347e+38f;
	assigns d->x, d->y;
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures (\is_finite(v.x) && \is_finite(s) && s != 0.0f) ==>d->x == v.x / s;
	ensures (\is_finite(v.y) && \is_finite(s) && s != 0.0f) ==>d->y == v.y / s;
	ensures (s > 0.0f && v.x >= 0.0f) ==> d->x >= 0.0f;
	ensures (s > 0.0f && v.x <= 0.0f) ==> d->x <= 0.0f;
	ensures (s < 0.0f && v.x >= 0.0f) ==> d->x <= 0.0f;
	ensures (s < 0.0f && v.x <= 0.0f) ==> d->x >= 0.0f;
	ensures (s > 0.0f && v.y >= 0.0f) ==> d->y >= 0.0f;
	ensures (s > 0.0f && v.y <= 0.0f) ==> d->y <= 0.0f;
	ensures (s < 0.0f && v.y >= 0.0f) ==> d->y <= 0.0f;
	ensures (s < 0.0f && v.y <= 0.0f) ==> d->y >= 0.0f;
*/
void zpl_vec2_div(zpl_vec2 *d, zpl_vec2 v, zpl_f32 s)
{
/*@ assert rte_211: mem_access: \valid(&d->x); */
/*@ assert rte_212: is_nan_or_infinite: \is_finite(\div_float(v.x, s)); */
	d->x = v.x / s;
/*@ assert rte_213: mem_access: \valid(&d->y); */
/*@ assert rte_214: is_nan_or_infinite: \is_finite(\div_float(v.y, s)); */
	d->y = v.y / s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v0.x) && \is_finite(v1.x);
	requires \is_finite(v0.y) && \is_finite(v1.y);
	requires \is_finite(v0.z) && \is_finite(v1.z);
	requires \is_finite(\add_float(v0.x, v1.x));
	requires \is_finite(\add_float(v0.y, v1.y));
	requires \is_finite(\add_float(v0.z, v1.z));
	assigns d->x, d->y, d->z;
	ensures \is_finite(v0.x) && \is_finite(v1.x) && \is_finite(\add_float(v0.x, v1.x))==> d->x == \add_float(v0.x, v1.x);
	ensures \is_finite(v0.y) && \is_finite(v1.y) && \is_finite(\add_float(v0.y, v1.y))==> d->y == \add_float(v0.y, v1.y);
	ensures \is_finite(v0.z) && \is_finite(v1.z) && \is_finite(\add_float(v0.z, v1.z))==> d->z == \add_float(v0.z, v1.z);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures \is_finite(d->z);
*/
void zpl_vec3_add(zpl_vec3 *d, zpl_vec3 v0, zpl_vec3 v1)
{
/*@ assert rte_250: mem_access: \valid(&d->x); */
/*@ assert rte_251: is_nan_or_infinite: \is_finite(\add_float(v0.x, v1.x)); */
	d->x = v0.x + v1.x;
/*@ assert rte_252: mem_access: \valid(&d->y); */
/*@ assert rte_253: is_nan_or_infinite: \is_finite(\add_float(v0.y, v1.y)); */
	d->y = v0.y + v1.y;
/*@ assert rte_254: mem_access: \valid(&d->z); */
/*@ assert rte_255: is_nan_or_infinite: \is_finite(\add_float(v0.z, v1.z)); */
	d->z = v0.z + v1.z;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v0.x) && \is_finite(v0.y) && \is_finite(v0.z);
	requires \is_finite(v1.x) && \is_finite(v1.y) && \is_finite(v1.z);
	requires \is_finite(\mul_float(v0.y, v1.z));
	requires \is_finite(\mul_float(v0.z, v1.y));
	requires \is_finite(\sub_float(\mul_float(v0.y, v1.z), \mul_float(v0.z, v1.y)));
	requires \is_finite(\mul_float(v0.z, v1.x));
	requires \is_finite(\mul_float(v0.x, v1.z));
	requires \is_finite(\sub_float(\mul_float(v0.z, v1.x), \mul_float(v0.x, v1.z)));
	requires \is_finite(\mul_float(v0.x, v1.y));
	requires \is_finite(\mul_float(v0.y, v1.x));
	requires \is_finite(\sub_float(\mul_float(v0.x, v1.y), \mul_float(v0.y, v1.x)));
	assigns d->x, d->y, d->z;
	ensures \is_finite(v0.y) && \is_finite(v1.z) && \is_finite(v0.z) && \is_finite(v1.y) ==>d->x == \sub_float(\mul_float(v0.y, v1.z), \mul_float(v0.z, v1.y));
	ensures \is_finite(v0.z) && \is_finite(v1.x) && \is_finite(v0.x) && \is_finite(v1.z) ==>d->y == \sub_float(\mul_float(v0.z, v1.x), \mul_float(v0.x, v1.z));
	ensures \is_finite(v0.x) && \is_finite(v1.y) && \is_finite(v0.y) && \is_finite(v1.x) ==>d->z == \sub_float(\mul_float(v0.x, v1.y), \mul_float(v0.y, v1.x));
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures \is_finite(d->z);
*/
void zpl_vec3_cross(zpl_vec3 *d, zpl_vec3 v0, zpl_vec3 v1)
{
/*@ assert rte_265: mem_access: \valid(&d->x); */
/*@ assert rte_266: is_nan_or_infinite: \is_finite(\mul_float(v0.y, v1.z)); */
/*@ assert rte_267: is_nan_or_infinite: \is_finite(\mul_float(v0.z, v1.y)); */
/*@ assert rte_268: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(v0.y, v1.z), \mul_float(v0.z, v1.y))); */
	d->x = v0.y * v1.z - v0.z * v1.y;
/*@ assert rte_269: mem_access: \valid(&d->y); */
/*@ assert rte_270: is_nan_or_infinite: \is_finite(\mul_float(v0.z, v1.x)); */
/*@ assert rte_271: is_nan_or_infinite: \is_finite(\mul_float(v0.x, v1.z)); */
/*@ assert rte_272: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(v0.z, v1.x), \mul_float(v0.x, v1.z))); */
	d->y = v0.z * v1.x - v0.x * v1.z;
/*@ assert rte_273: mem_access: \valid(&d->z); */
/*@ assert rte_274: is_nan_or_infinite: \is_finite(\mul_float(v0.x, v1.y)); */
/*@ assert rte_275: is_nan_or_infinite: \is_finite(\mul_float(v0.y, v1.x)); */
/*@ assert rte_276: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(v0.x, v1.y), \mul_float(v0.y, v1.x))); */
	d->z = v0.x * v1.y - v0.y * v1.x;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(\add_float(v0.x, v1.x));
	requires \is_finite(\add_float(v0.y, v1.y));
	requires \is_finite(\add_float(v0.z, v1.z));
	requires \is_finite(\add_float(v0.w, v1.w));
	assigns d->x, d->y, d->z, d->w;
	ensures \valid(d);
	ensures \is_finite(v0.x) && \is_finite(v1.x) &&@         \is_finite(\add_float(v0.x, v1.x))@         ==> d->x == \add_float(v0.x, v1.x);
	ensures \is_finite(v0.y) && \is_finite(v1.y) &&@         \is_finite(\add_float(v0.y, v1.y))@         ==> d->y == \add_float(v0.y, v1.y);
	ensures \is_finite(v0.z) && \is_finite(v1.z) &&@         \is_finite(\add_float(v0.z, v1.z))@         ==> d->z == \add_float(v0.z, v1.z);
	ensures \is_finite(v0.w) && \is_finite(v1.w) &&@         \is_finite(\add_float(v0.w, v1.w))@         ==> d->w == \add_float(v0.w, v1.w);
	ensures \is_finite(\add_float(v0.x, v1.x)) ==> \is_finite(d->x);
	ensures \is_finite(\add_float(v0.y, v1.y)) ==> \is_finite(d->y);
	ensures \is_finite(\add_float(v0.z, v1.z)) ==> \is_finite(d->z);
	ensures \is_finite(\add_float(v0.w, v1.w)) ==> \is_finite(d->w);
*/
void zpl_vec4_add(zpl_vec4 *d, zpl_vec4 v0, zpl_vec4 v1)
{
/*@ assert rte_328: mem_access: \valid(&d->x); */
/*@ assert rte_329: is_nan_or_infinite: \is_finite(\add_float(v0.x, v1.x)); */
	d->x = v0.x + v1.x;
/*@ assert rte_330: mem_access: \valid(&d->y); */
/*@ assert rte_331: is_nan_or_infinite: \is_finite(\add_float(v0.y, v1.y)); */
	d->y = v0.y + v1.y;
/*@ assert rte_332: mem_access: \valid(&d->z); */
/*@ assert rte_333: is_nan_or_infinite: \is_finite(\add_float(v0.z, v1.z)); */
	d->z = v0.z + v1.z;
/*@ assert rte_334: mem_access: \valid(&d->w); */
/*@ assert rte_335: is_nan_or_infinite: \is_finite(\add_float(v0.w, v1.w)); */
	d->w = v0.w + v1.w;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w);
	requires \is_finite(s);
	requires \is_finite(\mul_float(v.x, s));
	requires \is_finite(\mul_float(v.y, s));
	requires \is_finite(\mul_float(v.z, s));
	requires \is_finite(\mul_float(v.w, s));
	assigns d->x, d->y, d->z, d->w;
	ensures (\is_finite(v.x) && \is_finite(s) && \is_finite(\mul_float(v.x, s))) ==>d->x == \mul_float(v.x, s);
	ensures (\is_finite(v.y) && \is_finite(s) && \is_finite(\mul_float(v.y, s))) ==>d->y == \mul_float(v.y, s);
	ensures (\is_finite(v.z) && \is_finite(s) && \is_finite(\mul_float(v.z, s))) ==>d->z == \mul_float(v.z, s);
	ensures (\is_finite(v.w) && \is_finite(s) && \is_finite(\mul_float(v.w, s))) ==>d->w == \mul_float(v.w, s);
	ensures \is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z) && \is_finite(d->w);
*/
void zpl_vec4_mul(zpl_vec4 *d, zpl_vec4 v, zpl_f32 s)
{
/*@ assert rte_387: mem_access: \valid(&d->x); */
/*@ assert rte_388: is_nan_or_infinite: \is_finite(\mul_float(v.x, s)); */
	d->x = v.x * s;
/*@ assert rte_389: mem_access: \valid(&d->y); */
/*@ assert rte_390: is_nan_or_infinite: \is_finite(\mul_float(v.y, s)); */
	d->y = v.y * s;
/*@ assert rte_391: mem_access: \valid(&d->z); */
/*@ assert rte_392: is_nan_or_infinite: \is_finite(\mul_float(v.z, s)); */
	d->z = v.z * s;
/*@ assert rte_393: mem_access: \valid(&d->w); */
/*@ assert rte_394: is_nan_or_infinite: \is_finite(\mul_float(v.w, s)); */
	d->w = v.w * s;
	return;
}

/*@
	requires \valid(m);
	assigns m->col[0].x, m->col[0].y, m->col[1].x, m->col[1].y;
	ensures m->col[0].x == 1.0f;
	ensures m->col[0].y == 0.0f;
	ensures m->col[1].x == 0.0f;
	ensures m->col[1].y == 1.0f;
*/
void zpl_mat2_identity(zpl_mat2 *m)
{
/*@ assert rte_23: mem_access: \valid(&m->col[0]); */
	m->col[0] = zpl_vec2f((zpl_f32)1,(zpl_f32)0);
/*@ assert rte_24: mem_access: \valid(&m->col[1]); */
	m->col[1] = zpl_vec2f((zpl_f32)0,(zpl_f32)1);
	return;
}

/*@
	requires \is_finite(\add_float(\mul_float(m1->col[0].y, m2->col[1].x), \mul_float(m1->col[1].y, m2->col[1].y)));
	requires \is_finite(\mul_float(m1->col[1].y, m2->col[1].y));
	requires \is_finite(\mul_float(m1->col[0].y, m2->col[1].x));
	requires \is_finite(\add_float(\mul_float(m1->col[0].x, m2->col[1].x), \mul_float(m1->col[1].x, m2->col[1].y)));
	requires \is_finite(\mul_float(m1->col[1].x, m2->col[1].y));
	requires \is_finite(\mul_float(m1->col[0].x, m2->col[1].x));
	requires \is_finite(\add_float(\mul_float(m1->col[0].y, m2->col[0].x), \mul_float(m1->col[1].y, m2->col[0].y)));
	requires \is_finite(\mul_float(m1->col[1].y, m2->col[0].y));
	requires \is_finite(\mul_float(m1->col[0].y, m2->col[0].x));
	requires \is_finite(\add_float(\mul_float(m1->col[0].x, m2->col[0].x), \mul_float(m1->col[1].x, m2->col[0].y)));
	requires \is_finite(\mul_float(m1->col[1].x, m2->col[0].y));
	requires \is_finite(\mul_float(m1->col[0].x, m2->col[0].x));
	requires \is_finite(m2->col[1].y);
	requires \is_finite(m2->col[1].x);
	requires \is_finite(m2->col[0].y);
	requires \is_finite(m2->col[0].x);
	requires \is_finite(m1->col[1].y);
	requires \is_finite(m1->col[1].x);
	requires \is_finite(m1->col[0].y);
	requires \is_finite(m1->col[0].x);
	requires \valid_read(m2);
	requires \valid_read(m1);
	requires \valid(out);
	assigns *out;
	ensures \valid(out);
	ensures \is_finite(out->col[0].x);
	ensures \is_finite(out->col[0].y);
	ensures \is_finite(out->col[1].x);
	ensures \is_finite(out->col[1].y);
*/
void zpl_mat2_mul(zpl_mat2 *out, zpl_mat2 *m1, zpl_mat2 *m2)
{
	zpl_mat2 temp;
/*@ assert rte_25: mem_access: \valid_read(&m1->col[0].x); */
/*@ assert rte_26: mem_access: \valid_read(&m2->col[0].x); */
/*@ assert rte_27: is_nan_or_infinite: \is_finite(\mul_float(m1->col[0].x, m2->col[0].x)); */
/*@ assert rte_28: mem_access: \valid_read(&m1->col[1].x); */
/*@ assert rte_29: mem_access: \valid_read(&m2->col[0].y); */
/*@ assert rte_30: is_nan_or_infinite: \is_finite(\mul_float(m1->col[1].x, m2->col[0].y)); */
/*@ assert rte_31: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m1->col[0].x, m2->col[0].x), \mul_float(m1->col[1].x, m2->col[0].y))); */
	temp.col[0].x = m1->col[0].x * m2->col[0].x + m1->col[1].x * m2->col[0].y;
/*@ assert rte_32: mem_access: \valid_read(&m1->col[0].y); */
/*@ assert rte_33: mem_access: \valid_read(&m2->col[0].x); */
/*@ assert rte_34: is_nan_or_infinite: \is_finite(\mul_float(m1->col[0].y, m2->col[0].x)); */
/*@ assert rte_35: mem_access: \valid_read(&m1->col[1].y); */
/*@ assert rte_36: mem_access: \valid_read(&m2->col[0].y); */
/*@ assert rte_37: is_nan_or_infinite: \is_finite(\mul_float(m1->col[1].y, m2->col[0].y)); */
/*@ assert rte_38: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m1->col[0].y, m2->col[0].x), \mul_float(m1->col[1].y, m2->col[0].y))); */
	temp.col[0].y = m1->col[0].y * m2->col[0].x + m1->col[1].y * m2->col[0].y;
/*@ assert rte_39: mem_access: \valid_read(&m1->col[0].x); */
/*@ assert rte_40: mem_access: \valid_read(&m2->col[1].x); */
/*@ assert rte_41: is_nan_or_infinite: \is_finite(\mul_float(m1->col[0].x, m2->col[1].x)); */
/*@ assert rte_42: mem_access: \valid_read(&m1->col[1].x); */
/*@ assert rte_43: mem_access: \valid_read(&m2->col[1].y); */
/*@ assert rte_44: is_nan_or_infinite: \is_finite(\mul_float(m1->col[1].x, m2->col[1].y)); */
/*@ assert rte_45: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m1->col[0].x, m2->col[1].x), \mul_float(m1->col[1].x, m2->col[1].y))); */
	temp.col[1].x = m1->col[0].x * m2->col[1].x + m1->col[1].x * m2->col[1].y;
/*@ assert rte_46: mem_access: \valid_read(&m1->col[0].y); */
/*@ assert rte_47: mem_access: \valid_read(&m2->col[1].x); */
/*@ assert rte_48: is_nan_or_infinite: \is_finite(\mul_float(m1->col[0].y, m2->col[1].x)); */
/*@ assert rte_49: mem_access: \valid_read(&m1->col[1].y); */
/*@ assert rte_50: mem_access: \valid_read(&m2->col[1].y); */
/*@ assert rte_51: is_nan_or_infinite: \is_finite(\mul_float(m1->col[1].y, m2->col[1].y)); */
/*@ assert rte_52: is_nan_or_infinite: \is_finite(\add_float(\mul_float(m1->col[0].y, m2->col[1].x), \mul_float(m1->col[1].y, m2->col[1].y))); */
	temp.col[1].y = m1->col[0].y * m2->col[1].x + m1->col[1].y * m2->col[1].y;
/*@ assert rte_53: mem_access: \valid(out); */
	*out = temp;
	return;
}

/*@
	requires \valid(m);
	assigns m->col[0], m->col[1], m->col[2], m->col[3];
	ensures \valid(m) ==>m->col[0].x == 1.0f &&m->col[0].y == 0.0f &&m->col[0].z == 0.0f &&m->col[0].w == 0.0f;
	ensures \valid(m) ==>m->col[1].x == 0.0f &&m->col[1].y == 1.0f &&m->col[1].z == 0.0f &&m->col[1].w == 0.0f;
	ensures \valid(m) ==>m->col[2].x == 0.0f &&m->col[2].y == 0.0f &&m->col[2].z == 1.0f &&m->col[2].w == 0.0f;
	ensures \valid(m) ==>m->col[3].x == 0.0f &&m->col[3].y == 0.0f &&m->col[3].z == 0.0f &&m->col[3].w == 1.0f;
*/
void zpl_mat4_identity(zpl_mat4 *m)
{
/*@ assert rte_108: mem_access: \valid(&m->col[0]); */
	m->col[0] = zpl_vec4f((zpl_f32)1,(zpl_f32)0,(zpl_f32)0,(zpl_f32)0);
/*@ assert rte_109: mem_access: \valid(&m->col[1]); */
	m->col[1] = zpl_vec4f((zpl_f32)0,(zpl_f32)1,(zpl_f32)0,(zpl_f32)0);
/*@ assert rte_110: mem_access: \valid(&m->col[2]); */
	m->col[2] = zpl_vec4f((zpl_f32)0,(zpl_f32)0,(zpl_f32)1,(zpl_f32)0);
/*@ assert rte_111: mem_access: \valid(&m->col[3]); */
	m->col[3] = zpl_vec4f((zpl_f32)0,(zpl_f32)0,(zpl_f32)0,(zpl_f32)1);
	return;
}

/*@
	requires \valid((zpl_f32 *)(m + 0) + (0..3));
	requires \valid((zpl_f32 *)(m + 1) + (0..3));
	requires \valid((zpl_f32 *)(m + 2) + (0..3));
	requires \valid((zpl_f32 *)(m + 3) + (0..3));
	assigns m[0][0], m[0][1], m[0][2], m[0][3],@         m[1][0], m[1][1], m[1][2], m[1][3],@         m[2][0], m[2][1], m[2][2], m[2][3],@         m[3][0], m[3][1], m[3][2], m[3][3];
	ensures m[0][0] == 1.0f;
	ensures m[0][1] == 0.0f;
	ensures m[0][2] == 0.0f;
	ensures m[0][3] == 0.0f;
	ensures m[1][0] == 0.0f;
	ensures m[1][1] == 1.0f;
	ensures m[1][2] == 0.0f;
	ensures m[1][3] == 0.0f;
	ensures m[2][0] == 0.0f;
	ensures m[2][1] == 0.0f;
	ensures m[2][2] == 1.0f;
	ensures m[2][3] == 0.0f;
	ensures m[3][0] == 0.0f;
	ensures m[3][1] == 0.0f;
	ensures m[3][2] == 0.0f;
	ensures m[3][3] == 1.0f;
	ensures \forall int i; 0 <= i <= 3 ==> m[i][i] == 1.0f;
	ensures \forall int i, j; 0 <= i <= 3 && 0 <= j <= 3 && i != j ==> m[i][j] == 0.0f;
*/
void zpl_float44_identity(zpl_f32 m[4][4])
{
/*@ assert rte_1: mem_access: \valid((zpl_f32 *)*(m + 0)); */
	(*(m + 0))[0] = (zpl_f32)1;
/*@ assert rte_2: mem_access: \valid(&(*(m + 0))[1]); */
	(*(m + 0))[1] = (zpl_f32)0;
/*@ assert rte_3: mem_access: \valid(&(*(m + 0))[2]); */
	(*(m + 0))[2] = (zpl_f32)0;
/*@ assert rte_4: mem_access: \valid(&(*(m + 0))[3]); */
	(*(m + 0))[3] = (zpl_f32)0;
/*@ assert rte_5: mem_access: \valid((zpl_f32 *)*(m + 1)); */
	(*(m + 1))[0] = (zpl_f32)0;
/*@ assert rte_6: mem_access: \valid(&(*(m + 1))[1]); */
	(*(m + 1))[1] = (zpl_f32)1;
/*@ assert rte_7: mem_access: \valid(&(*(m + 1))[2]); */
	(*(m + 1))[2] = (zpl_f32)0;
/*@ assert rte_8: mem_access: \valid(&(*(m + 1))[3]); */
	(*(m + 1))[3] = (zpl_f32)0;
/*@ assert rte_9: mem_access: \valid((zpl_f32 *)*(m + 2)); */
	(*(m + 2))[0] = (zpl_f32)0;
/*@ assert rte_10: mem_access: \valid(&(*(m + 2))[1]); */
	(*(m + 2))[1] = (zpl_f32)0;
/*@ assert rte_11: mem_access: \valid(&(*(m + 2))[2]); */
	(*(m + 2))[2] = (zpl_f32)1;
/*@ assert rte_12: mem_access: \valid(&(*(m + 2))[3]); */
	(*(m + 2))[3] = (zpl_f32)0;
/*@ assert rte_13: mem_access: \valid((zpl_f32 *)*(m + 3)); */
	(*(m + 3))[0] = (zpl_f32)0;
/*@ assert rte_14: mem_access: \valid(&(*(m + 3))[1]); */
	(*(m + 3))[1] = (zpl_f32)0;
/*@ assert rte_15: mem_access: \valid(&(*(m + 3))[2]); */
	(*(m + 3))[2] = (zpl_f32)0;
/*@ assert rte_16: mem_access: \valid(&(*(m + 3))[3]); */
	(*(m + 3))[3] = (zpl_f32)1;
	return;
}

/*@
	requires \valid(vec + (0..3));
	requires \valid(vec[0] + (0..3));
	requires \valid(vec[1] + (0..3));
	requires \valid(vec[2] + (0..3));
	requires \valid(vec[3] + (0..3));
	requires \separated(vec[0]+(0..3), vec[1]+(0..3));
	requires \separated(vec[0]+(0..3), vec[2]+(0..3));
	requires \separated(vec[0]+(0..3), vec[3]+(0..3));
	requires \separated(vec[1]+(0..3), vec[2]+(0..3));
	requires \separated(vec[1]+(0..3), vec[3]+(0..3));
	requires \separated(vec[2]+(0..3), vec[3]+(0..3));
	assigns vec[0][1], vec[1][0],vec[0][2], vec[2][0],vec[0][3], vec[3][0],vec[1][2], vec[2][1],vec[1][3], vec[3][1],vec[2][3], vec[3][2];
	ensures vec[0][1] == \old(vec[1][0]);
	ensures vec[1][0] == \old(vec[0][1]);
	ensures vec[0][2] == \old(vec[2][0]);
	ensures vec[2][0] == \old(vec[0][2]);
	ensures vec[0][3] == \old(vec[3][0]);
	ensures vec[3][0] == \old(vec[0][3]);
	ensures vec[1][2] == \old(vec[2][1]);
	ensures vec[2][1] == \old(vec[1][2]);
	ensures vec[1][3] == \old(vec[3][1]);
	ensures vec[3][1] == \old(vec[1][3]);
	ensures vec[2][3] == \old(vec[3][2]);
	ensures vec[3][2] == \old(vec[2][3]);
	ensures vec[0][0] == \old(vec[0][0]);
	ensures vec[1][1] == \old(vec[1][1]);
	ensures vec[2][2] == \old(vec[2][2]);
	ensures vec[3][3] == \old(vec[3][3]);
*/
void zpl_float44_transpose(zpl_f32 (*vec)[4])
{
	zpl_f32 tmp;
/*@ assert rte_137: mem_access: \valid_read((zpl_f32 *)*(vec + 1)); */
	tmp = (*(vec + 1))[0];
/*@ assert rte_138: mem_access: \valid((zpl_f32 *)*(vec + 1)); */
/*@ assert rte_139: mem_access: \valid_read(&(*(vec + 0))[1]); */
	(*(vec + 1))[0] = (*(vec + 0))[1];
/*@ assert rte_140: mem_access: \valid(&(*(vec + 0))[1]); */
	(*(vec + 0))[1] = tmp;
/*@ assert rte_141: mem_access: \valid_read((zpl_f32 *)*(vec + 2)); */
	tmp = (*(vec + 2))[0];
/*@ assert rte_142: mem_access: \valid((zpl_f32 *)*(vec + 2)); */
/*@ assert rte_143: mem_access: \valid_read(&(*(vec + 0))[2]); */
	(*(vec + 2))[0] = (*(vec + 0))[2];
/*@ assert rte_144: mem_access: \valid(&(*(vec + 0))[2]); */
	(*(vec + 0))[2] = tmp;
/*@ assert rte_145: mem_access: \valid_read((zpl_f32 *)*(vec + 3)); */
	tmp = (*(vec + 3))[0];
/*@ assert rte_146: mem_access: \valid((zpl_f32 *)*(vec + 3)); */
/*@ assert rte_147: mem_access: \valid_read(&(*(vec + 0))[3]); */
	(*(vec + 3))[0] = (*(vec + 0))[3];
/*@ assert rte_148: mem_access: \valid(&(*(vec + 0))[3]); */
	(*(vec + 0))[3] = tmp;
/*@ assert rte_149: mem_access: \valid_read(&(*(vec + 2))[1]); */
	tmp = (*(vec + 2))[1];
/*@ assert rte_150: mem_access: \valid(&(*(vec + 2))[1]); */
/*@ assert rte_151: mem_access: \valid_read(&(*(vec + 1))[2]); */
	(*(vec + 2))[1] = (*(vec + 1))[2];
/*@ assert rte_152: mem_access: \valid(&(*(vec + 1))[2]); */
	(*(vec + 1))[2] = tmp;
/*@ assert rte_153: mem_access: \valid_read(&(*(vec + 3))[1]); */
	tmp = (*(vec + 3))[1];
/*@ assert rte_154: mem_access: \valid(&(*(vec + 3))[1]); */
/*@ assert rte_155: mem_access: \valid_read(&(*(vec + 1))[3]); */
	(*(vec + 3))[1] = (*(vec + 1))[3];
/*@ assert rte_156: mem_access: \valid(&(*(vec + 1))[3]); */
	(*(vec + 1))[3] = tmp;
/*@ assert rte_157: mem_access: \valid_read(&(*(vec + 3))[2]); */
	tmp = (*(vec + 3))[2];
/*@ assert rte_158: mem_access: \valid(&(*(vec + 3))[2]); */
/*@ assert rte_159: mem_access: \valid_read(&(*(vec + 2))[3]); */
	(*(vec + 3))[2] = (*(vec + 2))[3];
/*@ assert rte_160: mem_access: \valid(&(*(vec + 2))[3]); */
	(*(vec + 2))[3] = tmp;
	return;
}

/*@
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m[0][3], v.x), \mul_float(m[1][3], v.y)), \mul_float(m[2][3], v.z)), \mul_float(m[3][3], v.w)));
	requires \is_finite(\add_float(\add_float(\mul_float(m[0][3], v.x), \mul_float(m[1][3], v.y)), \mul_float(m[2][3], v.z)));
	requires \is_finite(\add_float(\mul_float(m[0][3], v.x), \mul_float(m[1][3], v.y)));
	requires \is_finite(\mul_float(m[3][3], v.w));
	requires \is_finite(\mul_float(m[2][3], v.z));
	requires \is_finite(\mul_float(m[1][3], v.y));
	requires \is_finite(\mul_float(m[0][3], v.x));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m[0][2], v.x), \mul_float(m[1][2], v.y)), \mul_float(m[2][2], v.z)), \mul_float(m[3][2], v.w)));
	requires \is_finite(\add_float(\add_float(\mul_float(m[0][2], v.x), \mul_float(m[1][2], v.y)), \mul_float(m[2][2], v.z)));
	requires \is_finite(\add_float(\mul_float(m[0][2], v.x), \mul_float(m[1][2], v.y)));
	requires \is_finite(\mul_float(m[3][2], v.w));
	requires \is_finite(\mul_float(m[2][2], v.z));
	requires \is_finite(\mul_float(m[1][2], v.y));
	requires \is_finite(\mul_float(m[0][2], v.x));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m[0][1], v.x), \mul_float(m[1][1], v.y)), \mul_float(m[2][1], v.z)), \mul_float(m[3][1], v.w)));
	requires \is_finite(\add_float(\add_float(\mul_float(m[0][1], v.x), \mul_float(m[1][1], v.y)), \mul_float(m[2][1], v.z)));
	requires \is_finite(\add_float(\mul_float(m[0][1], v.x), \mul_float(m[1][1], v.y)));
	requires \is_finite(\mul_float(m[3][1], v.w));
	requires \is_finite(\mul_float(m[2][1], v.z));
	requires \is_finite(\mul_float(m[1][1], v.y));
	requires \is_finite(\mul_float(m[0][1], v.x));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m[0][0], v.x), \mul_float(m[1][0], v.y)), \mul_float(m[2][0], v.z)), \mul_float(m[3][0], v.w)));
	requires \is_finite(\add_float(\add_float(\mul_float(m[0][0], v.x), \mul_float(m[1][0], v.y)), \mul_float(m[2][0], v.z)));
	requires \is_finite(\add_float(\mul_float(m[0][0], v.x), \mul_float(m[1][0], v.y)));
	requires \is_finite(\mul_float(m[3][0], v.w));
	requires \is_finite(\mul_float(m[2][0], v.z));
	requires \is_finite(\mul_float(m[1][0], v.y));
	requires \is_finite(\mul_float(m[0][0], v.x));
	requires \is_finite(m[3][3]);
	requires \is_finite(m[2][3]);
	requires \is_finite(m[1][3]);
	requires \is_finite(m[0][3]);
	requires \is_finite(m[3][2]);
	requires \is_finite(m[2][2]);
	requires \is_finite(m[1][2]);
	requires \is_finite(m[0][2]);
	requires \is_finite(m[3][1]);
	requires \is_finite(m[2][1]);
	requires \is_finite(m[1][1]);
	requires \is_finite(m[0][1]);
	requires \is_finite(m[3][0]);
	requires \is_finite(m[2][0]);
	requires \is_finite(m[1][0]);
	requires \is_finite(m[0][0]);
	requires \is_finite(v.w);
	requires \is_finite(v.z);
	requires \is_finite(v.y);
	requires \is_finite(v.x);
	requires \separated(out, (zpl_f32 *)m);
	requires \valid_read(m[3] + (0..3));
	requires \valid_read(m[2] + (0..3));
	requires \valid_read(m[1] + (0..3));
	requires \valid_read(m[0] + (0..3));
	requires \valid_read(m + (0..3));
	requires \valid(out);
	assigns out->x, out->y, out->z, out->w;
	assigns *out;
	ensures \is_finite(out->x) && \is_finite(out->y) &&\is_finite(out->z) && \is_finite(out->w);
	ensures out->x ==(((\old((*(m + 0))[0]) * v.x + \old((*(m + 1))[0]) * v.y) +\old((*(m + 2))[0]) * v.z) +\old((*(m + 3))[0]) * v.w);
	ensures out->x ==(((\old((*(m + 0))[0]) * \old(v.x) + \old((*(m + 1))[0]) * \old(v.y)) +\old((*(m + 2))[0]) * \old(v.z)) +\old((*(m + 3))[0]) * \old(v.w));
*/
void zpl_float44_mul_vec4(zpl_vec4 *out, zpl_f32 m[4][4], zpl_vec4 v)
{
/*@ assert rte_60: mem_access: \valid(&out->x); */
/*@ assert rte_61: mem_access: \valid_read((zpl_f32 *)*(m + 0)); */
/*@ assert rte_62: is_nan_or_infinite: \is_finite(\mul_float((*(m + 0))[0], v.x)); */
/*@ assert rte_63: mem_access: \valid_read((zpl_f32 *)*(m + 1)); */
/*@ assert rte_64: is_nan_or_infinite: \is_finite(\mul_float((*(m + 1))[0], v.y)); */
/*@ assert rte_65: is_nan_or_infinite: \is_finite(\add_float(\mul_float((*(m + 0))[0], v.x), \mul_float((*(m + 1))[0], v.y))); */
/*@ assert rte_66: mem_access: \valid_read((zpl_f32 *)*(m + 2)); */
/*@ assert rte_67: is_nan_or_infinite: \is_finite(\mul_float((*(m + 2))[0], v.z)); */
/*@ assert rte_68: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float((*(m + 0))[0], v.x), \mul_float((*(m + 1))[0], v.y)), \mul_float((*(m + 2))[0], v.z))); */
/*@ assert rte_69: mem_access: \valid_read((zpl_f32 *)*(m + 3)); */
/*@ assert rte_70: is_nan_or_infinite: \is_finite(\mul_float((*(m + 3))[0], v.w)); */
/*@ assert rte_71: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float((*(m + 0))[0], v.x), \mul_float((*(m + 1))[0], v.y)), \mul_float((*(m + 2))[0], v.z)), \mul_float((*(m + 3))[0], v.w))); */
	out->x = (((*(m + 0))[0] * v.x + (*(m + 1))[0] * v.y) + (*(m + 2))[0] * v.z) +
	(*(m + 3))[0] * v.w;
/*@ assert rte_72: mem_access: \valid(&out->y); */
/*@ assert rte_73: mem_access: \valid_read(&(*(m + 0))[1]); */
/*@ assert rte_74: is_nan_or_infinite: \is_finite(\mul_float((*(m + 0))[1], v.x)); */
/*@ assert rte_75: mem_access: \valid_read(&(*(m + 1))[1]); */
/*@ assert rte_76: is_nan_or_infinite: \is_finite(\mul_float((*(m + 1))[1], v.y)); */
/*@ assert rte_77: is_nan_or_infinite: \is_finite(\add_float(\mul_float((*(m + 0))[1], v.x), \mul_float((*(m + 1))[1], v.y))); */
/*@ assert rte_78: mem_access: \valid_read(&(*(m + 2))[1]); */
/*@ assert rte_79: is_nan_or_infinite: \is_finite(\mul_float((*(m + 2))[1], v.z)); */
/*@ assert rte_80: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float((*(m + 0))[1], v.x), \mul_float((*(m + 1))[1], v.y)), \mul_float((*(m + 2))[1], v.z))); */
/*@ assert rte_81: mem_access: \valid_read(&(*(m + 3))[1]); */
/*@ assert rte_82: is_nan_or_infinite: \is_finite(\mul_float((*(m + 3))[1], v.w)); */
/*@ assert rte_83: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float((*(m + 0))[1], v.x), \mul_float((*(m + 1))[1], v.y)), \mul_float((*(m + 2))[1], v.z)), \mul_float((*(m + 3))[1], v.w))); */
	out->y = (((*(m + 0))[1] * v.x + (*(m + 1))[1] * v.y) + (*(m + 2))[1] * v.z) +
	(*(m + 3))[1] * v.w;
/*@ assert rte_84: mem_access: \valid(&out->z); */
/*@ assert rte_85: mem_access: \valid_read(&(*(m + 0))[2]); */
/*@ assert rte_86: is_nan_or_infinite: \is_finite(\mul_float((*(m + 0))[2], v.x)); */
/*@ assert rte_87: mem_access: \valid_read(&(*(m + 1))[2]); */
/*@ assert rte_88: is_nan_or_infinite: \is_finite(\mul_float((*(m + 1))[2], v.y)); */
/*@ assert rte_89: is_nan_or_infinite: \is_finite(\add_float(\mul_float((*(m + 0))[2], v.x), \mul_float((*(m + 1))[2], v.y))); */
/*@ assert rte_90: mem_access: \valid_read(&(*(m + 2))[2]); */
/*@ assert rte_91: is_nan_or_infinite: \is_finite(\mul_float((*(m + 2))[2], v.z)); */
/*@ assert rte_92: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float((*(m + 0))[2], v.x), \mul_float((*(m + 1))[2], v.y)), \mul_float((*(m + 2))[2], v.z))); */
/*@ assert rte_93: mem_access: \valid_read(&(*(m + 3))[2]); */
/*@ assert rte_94: is_nan_or_infinite: \is_finite(\mul_float((*(m + 3))[2], v.w)); */
/*@ assert rte_95: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float((*(m + 0))[2], v.x), \mul_float((*(m + 1))[2], v.y)), \mul_float((*(m + 2))[2], v.z)), \mul_float((*(m + 3))[2], v.w))); */
	out->z = (((*(m + 0))[2] * v.x + (*(m + 1))[2] * v.y) + (*(m + 2))[2] * v.z) +
	(*(m + 3))[2] * v.w;
/*@ assert rte_96: mem_access: \valid(&out->w); */
/*@ assert rte_97: mem_access: \valid_read(&(*(m + 0))[3]); */
/*@ assert rte_98: is_nan_or_infinite: \is_finite(\mul_float((*(m + 0))[3], v.x)); */
/*@ assert rte_99: mem_access: \valid_read(&(*(m + 1))[3]); */
/*@ assert rte_100: is_nan_or_infinite: \is_finite(\mul_float((*(m + 1))[3], v.y)); */
/*@ assert rte_101: is_nan_or_infinite: \is_finite(\add_float(\mul_float((*(m + 0))[3], v.x), \mul_float((*(m + 1))[3], v.y))); */
/*@ assert rte_102: mem_access: \valid_read(&(*(m + 2))[3]); */
/*@ assert rte_103: is_nan_or_infinite: \is_finite(\mul_float((*(m + 2))[3], v.z)); */
/*@ assert rte_104: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float((*(m + 0))[3], v.x), \mul_float((*(m + 1))[3], v.y)), \mul_float((*(m + 2))[3], v.z))); */
/*@ assert rte_105: mem_access: \valid_read(&(*(m + 3))[3]); */
/*@ assert rte_106: is_nan_or_infinite: \is_finite(\mul_float((*(m + 3))[3], v.w)); */
/*@ assert rte_107: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float((*(m + 0))[3], v.x), \mul_float((*(m + 1))[3], v.y)), \mul_float((*(m + 2))[3], v.z)), \mul_float((*(m + 3))[3], v.w))); */
	out->w = (((*(m + 0))[3] * v.x + (*(m + 1))[3] * v.y) + (*(m + 2))[3] * v.z) +
	(*(m + 3))[3] * v.w;
	return;
}

/*@
	requires \valid(out);
	requires \is_finite(v.x);
	requires \is_finite(v.y);
	requires \is_finite(v.z);
	assigns out->col[0], out->col[1], out->col[2], out->col[3];
	ensures \valid(out) ==> out->col[0].x == 1.0f && out->col[0].y == 0.0f && out->col[0].z == 0.0f && out->col[0].w == 0.0f;
	ensures \valid(out) ==> out->col[1].x == 0.0f && out->col[1].y == 1.0f && out->col[1].z == 0.0f && out->col[1].w == 0.0f;
	ensures \valid(out) ==> out->col[2].x == 0.0f && out->col[2].y == 0.0f && out->col[2].z == 1.0f && out->col[2].w == 0.0f;
	ensures \valid(out) ==> out->col[3].x == v.x;
	ensures \valid(out) ==> out->col[3].y == v.y;
	ensures \valid(out) ==> out->col[3].z == v.z;
	ensures \valid(out) ==> out->col[3].w == 1.0f;
*/
void zpl_mat4_to_translate(zpl_mat4 *out, zpl_vec3 v)
{
	zpl_mat4_identity(out);
/*@ assert rte_134: mem_access: \valid(&out->col[3].x); */
	out->col[3].x = v.x;
/*@ assert rte_135: mem_access: \valid(&out->col[3].y); */
	out->col[3].y = v.y;
/*@ assert rte_136: mem_access: \valid(&out->col[3].z); */
	out->col[3].z = v.z;
	return;
}

/*@
	requires \valid(out);
	requires \is_finite(v.x);
	requires \is_finite(v.y);
	requires \is_finite(v.z);
	assigns out->col[0], out->col[1], out->col[2], out->col[3];
	ensures \valid(out) ==> out->col[0].x == v.x;
	ensures \valid(out) ==> out->col[0].y == 0.0f;
	ensures \valid(out) ==> out->col[0].z == 0.0f;
	ensures \valid(out) ==> out->col[0].w == 0.0f;
	ensures \valid(out) ==> out->col[1].x == 0.0f;
	ensures \valid(out) ==> out->col[1].y == v.y;
	ensures \valid(out) ==> out->col[1].z == 0.0f;
	ensures \valid(out) ==> out->col[1].w == 0.0f;
	ensures \valid(out) ==> out->col[2].x == 0.0f;
	ensures \valid(out) ==> out->col[2].y == 0.0f;
	ensures \valid(out) ==> out->col[2].z == v.z;
	ensures \valid(out) ==> out->col[2].w == 0.0f;
	ensures \valid(out) ==> out->col[3].x == 0.0f;
	ensures \valid(out) ==> out->col[3].y == 0.0f;
	ensures \valid(out) ==> out->col[3].z == 0.0f;
	ensures \valid(out) ==> out->col[3].w == 1.0f;
*/
void zpl_mat4_to_scale(zpl_mat4 *out, zpl_vec3 v)
{
	zpl_mat4_identity(out);
/*@ assert rte_128: mem_access: \valid(&out->col[0].x); */
	out->col[0].x = v.x;
/*@ assert rte_129: mem_access: \valid(&out->col[1].y); */
	out->col[1].y = v.y;
/*@ assert rte_130: mem_access: \valid(&out->col[2].z); */
	out->col[2].z = v.z;
	return;
}

/*@
	requires \valid(out);
	requires \is_finite(s);
	assigns out->col[0], out->col[1], out->col[2], out->col[3];
	ensures \valid(out) ==> out->col[0].x == s;
	ensures \valid(out) ==> out->col[0].y == 0.0f;
	ensures \valid(out) ==> out->col[0].z == 0.0f;
	ensures \valid(out) ==> out->col[0].w == 0.0f;
	ensures \valid(out) ==> out->col[1].x == 0.0f;
	ensures \valid(out) ==> out->col[1].y == s;
	ensures \valid(out) ==> out->col[1].z == 0.0f;
	ensures \valid(out) ==> out->col[1].w == 0.0f;
	ensures \valid(out) ==> out->col[2].x == 0.0f;
	ensures \valid(out) ==> out->col[2].y == 0.0f;
	ensures \valid(out) ==> out->col[2].z == s;
	ensures \valid(out) ==> out->col[2].w == 0.0f;
	ensures \valid(out) ==> out->col[3].x == 0.0f;
	ensures \valid(out) ==> out->col[3].y == 0.0f;
	ensures \valid(out) ==> out->col[3].z == 0.0f;
	ensures \valid(out) ==> out->col[3].w == 1.0f;
*/
void zpl_mat4_to_scalef(zpl_mat4 *out, zpl_f32 s)
{
	zpl_mat4_identity(out);
/*@ assert rte_131: mem_access: \valid(&out->col[0].x); */
	out->col[0].x = s;
/*@ assert rte_132: mem_access: \valid(&out->col[1].y); */
	out->col[1].y = s;
/*@ assert rte_133: mem_access: \valid(&out->col[2].z); */
	out->col[2].z = s;
	return;
}

/*@
	requires \valid(out);
	requires \is_finite(left);
	requires \is_finite(right);
	requires \is_finite(bottom);
	requires \is_finite(top);
	requires \is_finite(\sub_float(right, left));
	requires \is_finite(\sub_float(top, bottom));
	requires \is_finite(\div_float(2.0f, \sub_float(right, left)));
	requires \is_finite(\div_float(2.0f, \sub_float(top, bottom)));
	requires \is_finite(\add_float(right, left));
	requires \is_finite(\add_float(top, bottom));
	requires \is_finite(\div_float(\neg_float(\add_float(right, left)), \sub_float(right, left)));
	requires \is_finite(\div_float(\neg_float(\add_float(top, bottom)), \sub_float(top, bottom)));
	assigns out->col[0], out->col[1], out->col[2], out->col[3];
	ensures \valid(out) ==> out->col[0].x == \div_float(2.0f, \sub_float(right, left));
	ensures \valid(out) ==> out->col[0].y == 0.0f;
	ensures \valid(out) ==> out->col[0].z == 0.0f;
	ensures \valid(out) ==> out->col[0].w == 0.0f;
	ensures \valid(out) ==> out->col[1].x == 0.0f;
	ensures \valid(out) ==> out->col[1].y == \div_float(2.0f, \sub_float(top, bottom));
	ensures \valid(out) ==> out->col[1].z == 0.0f;
	ensures \valid(out) ==> out->col[1].w == 0.0f;
	ensures \valid(out) ==> out->col[2].x == 0.0f;
	ensures \valid(out) ==> out->col[2].y == 0.0f;
	ensures \valid(out) ==> out->col[2].z == -1.0f;
	ensures \valid(out) ==> out->col[2].w == 0.0f;
	ensures \valid(out) ==> out->col[3].x == \div_float(\neg_float(\add_float(right, left)), \sub_float(right, left));
	ensures \valid(out) ==> out->col[3].y == \div_float(\neg_float(\add_float(top, bottom)), \sub_float(top, bottom));
	ensures \valid(out) ==> out->col[3].z == 0.0f;
	ensures \valid(out) ==> out->col[3].w == 1.0f;
*/
void zpl_mat4_ortho2d(zpl_mat4 *out, zpl_f32 left, zpl_f32 right,
zpl_f32 bottom, zpl_f32 top)
{
	zpl_mat4_identity(out);
/*@ assert rte_112: mem_access: \valid(&out->col[0].x); */
/*@ assert rte_113: is_nan_or_infinite: \is_finite(\sub_float(right, left)); */
/*@ assert rte_114: is_nan_or_infinite: \is_finite(\div_float(2.0f, \sub_float(right, left))); */
	out->col[0].x = 2.0f / (right - left);
/*@ assert rte_115: mem_access: \valid(&out->col[1].y); */
/*@ assert rte_116: is_nan_or_infinite: \is_finite(\sub_float(top, bottom)); */
/*@ assert rte_117: is_nan_or_infinite: \is_finite(\div_float(2.0f, \sub_float(top, bottom))); */
	out->col[1].y = 2.0f / (top - bottom);
/*@ assert rte_118: mem_access: \valid(&out->col[2].z); */
/*@ assert rte_119: is_nan_or_infinite: \is_finite(1.0f); */
	out->col[2].z = - 1.0f;
/*@ assert rte_120: mem_access: \valid(&out->col[3].x); */
/*@ assert rte_121: is_nan_or_infinite: \is_finite(\add_float(right, left)); */
/*@ assert rte_122: is_nan_or_infinite: \is_finite(\sub_float(right, left)); */
/*@ assert rte_123: is_nan_or_infinite: \is_finite(\div_float(\neg_float(\add_float(right, left)), \sub_float(right, left))); */
	out->col[3].x = - (right + left) / (right - left);
/*@ assert rte_124: mem_access: \valid(&out->col[3].y); */
/*@ assert rte_125: is_nan_or_infinite: \is_finite(\add_float(top, bottom)); */
/*@ assert rte_126: is_nan_or_infinite: \is_finite(\sub_float(top, bottom)); */
/*@ assert rte_127: is_nan_or_infinite: \is_finite(\div_float(\neg_float(\add_float(top, bottom)), \sub_float(top, bottom))); */
	out->col[3].y = - (top + bottom) / (top - bottom);
	return;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result.x == 0.0f;
	ensures \result.y == 0.0f;
	ensures \result.z == 0.0f;
	ensures \result.w == 1.0f;
*/
zpl_quat zpl_quat_identity(void)
{
	zpl_quat q =
{.x = (zpl_f32)0, .y = (zpl_f32)0, .z = (zpl_f32)0, .w = (zpl_f32)1};
	return q;
}

/*@
	requires \valid(d);
	requires \is_finite(q0.x) && \is_finite(q0.y) && \is_finite(q0.z) && \is_finite(q0.w);
	requires \is_finite(q1.x) && \is_finite(q1.y) && \is_finite(q1.z) && \is_finite(q1.w);
	requires \abs(q0.x) <= 0x1p62 && \abs(q0.y) <= 0x1p62 && \abs(q0.z) <= 0x1p62 && \abs(q0.w) <= 0x1p62;
	requires \abs(q1.x) <= 0x1p62 && \abs(q1.y) <= 0x1p62 && \abs(q1.z) <= 0x1p62 && \abs(q1.w) <= 0x1p62;
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(d->x) && \is_finite(d->y) &&\is_finite(d->z) && \is_finite(d->w);
	ensures (\is_finite(q0.w) && \is_finite(q1.x) &&\is_finite(q0.x) && \is_finite(q1.w) &&\is_finite(q0.y) && \is_finite(q1.z) &&\is_finite(q0.z) && \is_finite(q1.y)) ==>\is_finite(d->x);
	ensures (\is_finite(q0.w) && \is_finite(q1.y) &&\is_finite(q0.x) && \is_finite(q1.z) &&\is_finite(q0.y) && \is_finite(q1.w) &&\is_finite(q0.z) && \is_finite(q1.x)) ==>\is_finite(d->y);
	ensures (\is_finite(q0.w) && \is_finite(q1.z) &&\is_finite(q0.x) && \is_finite(q1.y) &&\is_finite(q0.y) && \is_finite(q1.x) &&\is_finite(q0.z) && \is_finite(q1.w)) ==>\is_finite(d->z);
	ensures (\is_finite(q0.w) && \is_finite(q1.w) &&\is_finite(q0.x) && \is_finite(q1.x) &&\is_finite(q0.y) && \is_finite(q1.y) &&\is_finite(q0.z) && \is_finite(q1.z)) ==>\is_finite(d->w);
	ensures d->x == (float)(((float)((float)(q0.w * q1.x) +(float)(q0.x * q1.w)) +(float)(q0.y * q1.z)) -(float)(q0.z * q1.y));
	ensures d->y == (float)(((float)((float)(q0.w * q1.y) -(float)(q0.x * q1.z)) +(float)(q0.y * q1.w)) +(float)(q0.z * q1.x));
	ensures d->z == (float)(((float)((float)(q0.w * q1.z) +(float)(q0.x * q1.y)) -(float)(q0.y * q1.x)) +(float)(q0.z * q1.w));
	ensures d->w == (float)(((float)((float)(q0.w * q1.w) -(float)(q0.x * q1.x)) -(float)(q0.y * q1.y)) -(float)(q0.z * q1.z));
*/
void zpl_quat_mul(zpl_quat *d, zpl_quat q0, zpl_quat q1)
{
/*@ assert rte_161: mem_access: \valid(&d->x); */
/*@ assert rte_162: is_nan_or_infinite: \is_finite(\mul_float(q0.w, q1.x)); */
/*@ assert rte_163: is_nan_or_infinite: \is_finite(\mul_float(q0.x, q1.w)); */
/*@ assert rte_164: is_nan_or_infinite: \is_finite(\add_float(\mul_float(q0.w, q1.x), \mul_float(q0.x, q1.w))); */
/*@ assert rte_165: is_nan_or_infinite: \is_finite(\mul_float(q0.y, q1.z)); */
/*@ assert rte_166: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(q0.w, q1.x), \mul_float(q0.x, q1.w)), \mul_float(q0.y, q1.z))); */
/*@ assert rte_167: is_nan_or_infinite: \is_finite(\mul_float(q0.z, q1.y)); */
/*@ assert rte_168: is_nan_or_infinite: \is_finite(\sub_float(\add_float(\add_float(\mul_float(q0.w, q1.x), \mul_float(q0.x, q1.w)), \mul_float(q0.y, q1.z)), \mul_float(q0.z, q1.y))); */
	d->x = ((q0.w * q1.x + q0.x * q1.w) + q0.y * q1.z) - q0.z * q1.y;
/*@ assert rte_169: mem_access: \valid(&d->y); */
/*@ assert rte_170: is_nan_or_infinite: \is_finite(\mul_float(q0.w, q1.y)); */
/*@ assert rte_171: is_nan_or_infinite: \is_finite(\mul_float(q0.x, q1.z)); */
/*@ assert rte_172: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(q0.w, q1.y), \mul_float(q0.x, q1.z))); */
/*@ assert rte_173: is_nan_or_infinite: \is_finite(\mul_float(q0.y, q1.w)); */
/*@ assert rte_174: is_nan_or_infinite: \is_finite(\add_float(\sub_float(\mul_float(q0.w, q1.y), \mul_float(q0.x, q1.z)), \mul_float(q0.y, q1.w))); */
/*@ assert rte_175: is_nan_or_infinite: \is_finite(\mul_float(q0.z, q1.x)); */
/*@ assert rte_176: is_nan_or_infinite: \is_finite(\add_float(\add_float(\sub_float(\mul_float(q0.w, q1.y), \mul_float(q0.x, q1.z)), \mul_float(q0.y, q1.w)), \mul_float(q0.z, q1.x))); */
	d->y = ((q0.w * q1.y - q0.x * q1.z) + q0.y * q1.w) + q0.z * q1.x;
/*@ assert rte_177: mem_access: \valid(&d->z); */
/*@ assert rte_178: is_nan_or_infinite: \is_finite(\mul_float(q0.w, q1.z)); */
/*@ assert rte_179: is_nan_or_infinite: \is_finite(\mul_float(q0.x, q1.y)); */
/*@ assert rte_180: is_nan_or_infinite: \is_finite(\add_float(\mul_float(q0.w, q1.z), \mul_float(q0.x, q1.y))); */
/*@ assert rte_181: is_nan_or_infinite: \is_finite(\mul_float(q0.y, q1.x)); */
/*@ assert rte_182: is_nan_or_infinite: \is_finite(\sub_float(\add_float(\mul_float(q0.w, q1.z), \mul_float(q0.x, q1.y)), \mul_float(q0.y, q1.x))); */
/*@ assert rte_183: is_nan_or_infinite: \is_finite(\mul_float(q0.z, q1.w)); */
/*@ assert rte_184: is_nan_or_infinite: \is_finite(\add_float(\sub_float(\add_float(\mul_float(q0.w, q1.z), \mul_float(q0.x, q1.y)), \mul_float(q0.y, q1.x)), \mul_float(q0.z, q1.w))); */
	d->z = ((q0.w * q1.z + q0.x * q1.y) - q0.y * q1.x) + q0.z * q1.w;
/*@ assert rte_185: mem_access: \valid(&d->w); */
/*@ assert rte_186: is_nan_or_infinite: \is_finite(\mul_float(q0.w, q1.w)); */
/*@ assert rte_187: is_nan_or_infinite: \is_finite(\mul_float(q0.x, q1.x)); */
/*@ assert rte_188: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(q0.w, q1.w), \mul_float(q0.x, q1.x))); */
/*@ assert rte_189: is_nan_or_infinite: \is_finite(\mul_float(q0.y, q1.y)); */
/*@ assert rte_190: is_nan_or_infinite: \is_finite(\sub_float(\sub_float(\mul_float(q0.w, q1.w), \mul_float(q0.x, q1.x)), \mul_float(q0.y, q1.y))); */
/*@ assert rte_191: is_nan_or_infinite: \is_finite(\mul_float(q0.z, q1.z)); */
/*@ assert rte_192: is_nan_or_infinite: \is_finite(\sub_float(\sub_float(\sub_float(\mul_float(q0.w, q1.w), \mul_float(q0.x, q1.x)), \mul_float(q0.y, q1.y)), \mul_float(q0.z, q1.z))); */
	d->w = ((q0.w * q1.w - q0.x * q1.x) - q0.y * q1.y) - q0.z * q1.z;
	return;
}

/*@
	requires \is_finite((double)pos.x);
	requires \is_finite((double)pos.y);
	requires \is_finite((double)dim.x);
	requires \is_finite((double)dim.y);
	assigns \nothing;
	ensures \result.pos.x == pos.x;
	ensures \result.pos.y == pos.y;
	ensures \result.dim.x == dim.x;
	ensures \result.dim.y == dim.y;
*/
zpl_rect2 zpl_rect2f(zpl_vec2 pos, zpl_vec2 dim)
{
	zpl_rect2 r;
	r.pos = pos;
	r.dim = dim;
	return r;
}

/*@
	requires \is_finite(minx);
	requires \is_finite(miny);
	requires \is_finite(maxx);
	requires \is_finite(maxy);
	assigns \nothing;
	ensures \result.min.x == minx;
	ensures \result.min.y == miny;
	ensures \result.max.x == maxx;
	ensures \result.max.y == maxy;
*/
zpl_aabb2 zpl_aabb2f(zpl_f32 minx, zpl_f32 miny, zpl_f32 maxx, zpl_f32 maxy)
{
	zpl_aabb2 r;
	r.min = zpl_vec2f(minx,miny);
	r.max = zpl_vec2f(maxx,maxy);
	return r;
}

/*@
	requires \is_finite(a.min.x) && \is_finite(a.max.x);
	requires \is_finite(a.min.y) && \is_finite(a.max.y);
	requires \is_finite(x);
	requires \is_finite(y);
	requires a.min.x <= a.max.x;
	requires a.min.y <= a.max.y;
	assigns \nothing;
	ensures (x >= a.min.x && x <= a.max.x && y >= a.min.y && y <= a.max.y) ==> \result == 1;
	ensures !(x >= a.min.x && x <= a.max.x && y >= a.min.y && y <= a.max.y) ==> \result == 0;
	ensures \result == 0 || \result == 1;
*/
int zpl_aabb2_contains(zpl_aabb2 a, zpl_f32 x, zpl_f32 y)
{
	int tmp;
	if (x >= a.min.x)
	if (x <= a.max.x)
	if (y >= a.min.y)
	if (y <= a.max.y) tmp = 1; else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	return tmp;
}

/*@
	requires \is_finite(\add_float(\mul_float(a, \sub_float(1.0f, t)), \mul_float(b, t)));
	requires \is_finite(\mul_float(b, t));
	requires \is_finite(\mul_float(a, \sub_float(1.0f, t)));
	requires \is_finite(\sub_float(1.0f, t));
	requires \is_finite(t);
	requires \is_finite(b);
	requires \is_finite(a);
	assigns \nothing;
	ensures (t == 0.0f) ==> (\result == a);
	ensures (t == 1.0f) ==> (\result == b);
	ensures (0.0f < t < 1.0f) ==> (\is_finite(\result));
	ensures \is_finite(\result);
	ensures \result == a * (1.0f - t) + b * t;
*/
zpl_f32 zpl_lerp(zpl_f32 a, zpl_f32 b, zpl_f32 t)
{
	zpl_f32 __retres;
/*@ assert rte_17: is_nan_or_infinite: \is_finite(\sub_float(1.0f, t)); */
/*@ assert rte_18: is_nan_or_infinite: \is_finite(\mul_float(a, \sub_float(1.0f, t))); */
/*@ assert rte_19: is_nan_or_infinite: \is_finite(\mul_float(b, t)); */
/*@ assert rte_20: is_nan_or_infinite: \is_finite(\add_float(\mul_float(a, \sub_float(1.0f, t)), \mul_float(b, t))); */
	__retres = a * (1.0f - t) + b * t;
	return __retres;
}

/*@
	requires \valid(d);
	requires \is_finite(a.x) && \is_finite(a.y);
	requires \is_finite(b.x) && \is_finite(b.y);
	requires \is_finite(t);
	requires \is_finite(\sub_float(b.x, a.x));
	requires \is_finite(\sub_float(b.y, a.y));
	requires \is_finite(\mul_float(\sub_float(b.x, a.x), t));
	requires \is_finite(\mul_float(\sub_float(b.y, a.y), t));
	requires \is_finite(\add_float(a.x, \mul_float(\sub_float(b.x, a.x), t)));
	requires \is_finite(\add_float(a.y, \mul_float(\sub_float(b.y, a.y), t)));
	assigns d->x, d->y;
	ensures \is_finite(a.x) && \is_finite(b.x) && \is_finite(t) &&\is_finite(\sub_float(b.x, a.x)) &&\is_finite(\mul_float(\sub_float(b.x, a.x), t)) &&\is_finite(\add_float(a.x, \mul_float(\sub_float(b.x, a.x), t))) ==>d->x == \add_float(a.x, \mul_float(\sub_float(b.x, a.x), t));
	ensures \is_finite(a.y) && \is_finite(b.y) && \is_finite(t) &&\is_finite(\sub_float(b.y, a.y)) &&\is_finite(\mul_float(\sub_float(b.y, a.y), t)) &&\is_finite(\add_float(a.y, \mul_float(\sub_float(b.y, a.y), t))) ==>d->y == \add_float(a.y, \mul_float(\sub_float(b.y, a.y), t));
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
*/
void zpl_vec2_lerp(zpl_vec2 *d, zpl_vec2 a, zpl_vec2 b, zpl_f32 t)
{
	zpl_vec2 db;
	zpl_vec2_sub(& db,b,a);
	zpl_vec2_mul(& db,db,t);
	zpl_vec2_add(d,a,db);
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v0.x) && \is_finite(v1.x);
	requires \is_finite(v0.y) && \is_finite(v1.y);
	requires \is_finite(v0.z) && \is_finite(v1.z);
	requires \is_finite(\sub_float(v0.x, v1.x));
	requires \is_finite(\sub_float(v0.y, v1.y));
	requires \is_finite(\sub_float(v0.z, v1.z));
	assigns d->x, d->y, d->z;
	ensures \is_finite(v0.x) && \is_finite(v1.x) ==>d->x == \sub_float(v0.x, v1.x);
	ensures \is_finite(v0.y) && \is_finite(v1.y) ==>d->y == \sub_float(v0.y, v1.y);
	ensures \is_finite(v0.z) && \is_finite(v1.z) ==>d->z == \sub_float(v0.z, v1.z);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures \is_finite(d->z);
*/
void zpl_vec3_sub(zpl_vec3 *d, zpl_vec3 v0, zpl_vec3 v1)
{
/*@ assert rte_313: mem_access: \valid(&d->x); */
/*@ assert rte_314: is_nan_or_infinite: \is_finite(\sub_float(v0.x, v1.x)); */
	d->x = v0.x - v1.x;
/*@ assert rte_315: mem_access: \valid(&d->y); */
/*@ assert rte_316: is_nan_or_infinite: \is_finite(\sub_float(v0.y, v1.y)); */
	d->y = v0.y - v1.y;
/*@ assert rte_317: mem_access: \valid(&d->z); */
/*@ assert rte_318: is_nan_or_infinite: \is_finite(\sub_float(v0.z, v1.z)); */
	d->z = v0.z - v1.z;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z);
	requires \is_finite(s);
	requires \abs(v.x) * \abs(s) <= 0x1.fffffep127;
	requires \abs(v.y) * \abs(s) <= 0x1.fffffep127;
	requires \abs(v.z) * \abs(s) <= 0x1.fffffep127;
	assigns *d;
	ensures \is_finite(v.x) && \is_finite(s) && \abs(v.x) * \abs(s) <= FLT_MAX@         ==> d->x == v.x * s;
	ensures \is_finite(v.y) && \is_finite(s) && \abs(v.y) * \abs(s) <= FLT_MAX@         ==> d->y == v.y * s;
	ensures \is_finite(v.z) && \is_finite(s) && \abs(v.z) * \abs(s) <= FLT_MAX@         ==> d->z == v.z * s;
	ensures \is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z);
*/
void zpl_vec3_mul(zpl_vec3 *d, zpl_vec3 v, zpl_f32 s)
{
/*@ assert rte_307: mem_access: \valid(&d->x); */
/*@ assert rte_308: is_nan_or_infinite: \is_finite(\mul_float(v.x, s)); */
	d->x = v.x * s;
/*@ assert rte_309: mem_access: \valid(&d->y); */
/*@ assert rte_310: is_nan_or_infinite: \is_finite(\mul_float(v.y, s)); */
	d->y = v.y * s;
/*@ assert rte_311: mem_access: \valid(&d->z); */
/*@ assert rte_312: is_nan_or_infinite: \is_finite(\mul_float(v.z, s)); */
	d->z = v.z * s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z);
	requires \is_finite(s);
	requires s != 0.0f;
	requires \is_finite(\div_float(v.x, s));
	requires \is_finite(\div_float(v.y, s));
	requires \is_finite(\div_float(v.z, s));
	assigns d->x, d->y, d->z;
	ensures \is_finite(s) && s != 0.0f && \is_finite(v.x) && \is_finite(\div_float(v.x, s))==> d->x == \div_float(v.x, s);
	ensures \is_finite(s) && s != 0.0f && \is_finite(v.y) && \is_finite(\div_float(v.y, s))==> d->y == \div_float(v.y, s);
	ensures \is_finite(s) && s != 0.0f && \is_finite(v.z) && \is_finite(\div_float(v.z, s))==> d->z == \div_float(v.z, s);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures \is_finite(d->z);
*/
void zpl_vec3_div(zpl_vec3 *d, zpl_vec3 v, zpl_f32 s)
{
/*@ assert rte_277: mem_access: \valid(&d->x); */
/*@ assert rte_278: is_nan_or_infinite: \is_finite(\div_float(v.x, s)); */
	d->x = v.x / s;
/*@ assert rte_279: mem_access: \valid(&d->y); */
/*@ assert rte_280: is_nan_or_infinite: \is_finite(\div_float(v.y, s)); */
	d->y = v.y / s;
/*@ assert rte_281: mem_access: \valid(&d->z); */
/*@ assert rte_282: is_nan_or_infinite: \is_finite(\div_float(v.z, s)); */
	d->z = v.z / s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(\sub_float(v0.x, v1.x));
	requires \is_finite(\sub_float(v0.y, v1.y));
	requires \is_finite(\sub_float(v0.z, v1.z));
	requires \is_finite(\sub_float(v0.w, v1.w));
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(v0.x) && \is_finite(v1.x) ==>d->x == \sub_float(v0.x, v1.x);
	ensures \is_finite(v0.y) && \is_finite(v1.y) ==>d->y == \sub_float(v0.y, v1.y);
	ensures \is_finite(v0.z) && \is_finite(v1.z) ==>d->z == \sub_float(v0.z, v1.z);
	ensures \is_finite(v0.w) && \is_finite(v1.w) ==>d->w == \sub_float(v0.w, v1.w);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures \is_finite(d->z);
	ensures \is_finite(d->w);
	ensures d->x == \sub_float(v0.x, v1.x);
	ensures d->y == \sub_float(v0.y, v1.y);
	ensures d->z == \sub_float(v0.z, v1.z);
	ensures d->w == \sub_float(v0.w, v1.w);
*/
void zpl_vec4_sub(zpl_vec4 *d, zpl_vec4 v0, zpl_vec4 v1)
{
/*@ assert rte_395: mem_access: \valid(&d->x); */
/*@ assert rte_396: is_nan_or_infinite: \is_finite(\sub_float(v0.x, v1.x)); */
	d->x = v0.x - v1.x;
/*@ assert rte_397: mem_access: \valid(&d->y); */
/*@ assert rte_398: is_nan_or_infinite: \is_finite(\sub_float(v0.y, v1.y)); */
	d->y = v0.y - v1.y;
/*@ assert rte_399: mem_access: \valid(&d->z); */
/*@ assert rte_400: is_nan_or_infinite: \is_finite(\sub_float(v0.z, v1.z)); */
	d->z = v0.z - v1.z;
/*@ assert rte_401: mem_access: \valid(&d->w); */
/*@ assert rte_402: is_nan_or_infinite: \is_finite(\sub_float(v0.w, v1.w)); */
	d->w = v0.w - v1.w;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w);
	requires \is_finite(s);
	requires s != 0.0f;
	requires \abs(v.x) <= \abs(s) * 3.40282346638528859811704183484516925440e+38f;
	requires \abs(v.y) <= \abs(s) * 3.40282346638528859811704183484516925440e+38f;
	requires \abs(v.z) <= \abs(s) * 3.40282346638528859811704183484516925440e+38f;
	requires \abs(v.w) <= \abs(s) * 3.40282346638528859811704183484516925440e+38f;
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(v.x) && \is_finite(s) && s != 0.0f ==>\is_finite(d->x) && d->x == \div_float(v.x, s);
	ensures \is_finite(v.y) && \is_finite(s) && s != 0.0f ==>\is_finite(d->y) && d->y == \div_float(v.y, s);
	ensures \is_finite(v.z) && \is_finite(s) && s != 0.0f ==>\is_finite(d->z) && d->z == \div_float(v.z, s);
	ensures \is_finite(v.w) && \is_finite(s) && s != 0.0f ==>\is_finite(d->w) && d->w == \div_float(v.w, s);
*/
void zpl_vec4_div(zpl_vec4 *d, zpl_vec4 v, zpl_f32 s)
{
/*@ assert rte_348: mem_access: \valid(&d->x); */
/*@ assert rte_349: is_nan_or_infinite: \is_finite(\div_float(v.x, s)); */
	d->x = v.x / s;
/*@ assert rte_350: mem_access: \valid(&d->y); */
/*@ assert rte_351: is_nan_or_infinite: \is_finite(\div_float(v.y, s)); */
	d->y = v.y / s;
/*@ assert rte_352: mem_access: \valid(&d->z); */
/*@ assert rte_353: is_nan_or_infinite: \is_finite(\div_float(v.z, s)); */
	d->z = v.z / s;
/*@ assert rte_354: mem_access: \valid(&d->w); */
/*@ assert rte_355: is_nan_or_infinite: \is_finite(\div_float(v.w, s)); */
	d->w = v.w / s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x);
	requires \is_finite(v.x);
	requires \is_finite(\add_float(d->x, v.x));
	requires \is_finite(d->y);
	requires \is_finite(v.y);
	requires \is_finite(\add_float(d->y, v.y));
	assigns d->x, d->y;
	ensures \is_finite(\old(d->x)) && \is_finite(v.x) && \is_finite(\add_float(\old(d->x), v.x))==> d->x == \add_float(\old(d->x), v.x);
	ensures \is_finite(\old(d->y)) && \is_finite(v.y) && \is_finite(\add_float(\old(d->y), v.y))==> d->y == \add_float(\old(d->y), v.y);
	ensures \is_finite(\add_float(\old(d->x), v.x)) ==> \is_finite(d->x);
	ensures \is_finite(\add_float(\old(d->y), v.y)) ==> \is_finite(d->y);
*/
void zpl_vec2_addeq(zpl_vec2 *d, zpl_vec2 v)
{
/*@ assert rte_201: mem_access: \valid(&d->x); */
/*@ assert rte_202: mem_access: \valid_read(&d->x); */
/*@ assert rte_203: is_nan_or_infinite: \is_finite(\add_float(d->x, v.x)); */
	d->x += v.x;
/*@ assert rte_204: mem_access: \valid(&d->y); */
/*@ assert rte_205: mem_access: \valid_read(&d->y); */
/*@ assert rte_206: is_nan_or_infinite: \is_finite(\add_float(d->y, v.y)); */
	d->y += v.y;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x);
	requires \is_finite(d->y);
	requires \is_finite(v.x);
	requires \is_finite(v.y);
	requires \is_finite(\sub_float(d->x, v.x));
	requires \is_finite(\sub_float(d->y, v.y));
	assigns d->x, d->y;
	ensures \is_finite(\old(d->x)) && \is_finite(\old(v.x)) && \is_finite(\sub_float(\old(d->x), \old(v.x)))==> d->x == \sub_float(\old(d->x), \old(v.x));
	ensures \is_finite(\old(d->y)) && \is_finite(\old(v.y)) && \is_finite(\sub_float(\old(d->y), \old(v.y)))==> d->y == \sub_float(\old(d->y), \old(v.y));
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
*/
void zpl_vec2_subeq(zpl_vec2 *d, zpl_vec2 v)
{
/*@ assert rte_242: mem_access: \valid(&d->x); */
/*@ assert rte_243: mem_access: \valid_read(&d->x); */
/*@ assert rte_244: is_nan_or_infinite: \is_finite(\sub_float(d->x, v.x)); */
	d->x -= v.x;
/*@ assert rte_245: mem_access: \valid(&d->y); */
/*@ assert rte_246: mem_access: \valid_read(&d->y); */
/*@ assert rte_247: is_nan_or_infinite: \is_finite(\sub_float(d->y, v.y)); */
	d->y -= v.y;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x);
	requires \is_finite(d->y);
	requires \is_finite(s);
	requires \is_finite(\mul_float(d->x, s));
	requires \is_finite(\mul_float(d->y, s));
	assigns d->x, d->y;
	ensures \is_finite(\old(d->x)) && \is_finite(s) && \is_finite(\mul_float(\old(d->x), s))==> d->x == \mul_float(\old(d->x), s);
	ensures \is_finite(\old(d->y)) && \is_finite(s) && \is_finite(\mul_float(\old(d->y), s))==> d->y == \mul_float(\old(d->y), s);
	ensures \is_finite(\old(d->x)) && \is_finite(s) && \is_finite(\mul_float(\old(d->x), s))==> \is_finite(d->x);
	ensures \is_finite(\old(d->y)) && \is_finite(s) && \is_finite(\mul_float(\old(d->y), s))==> \is_finite(d->y);
*/
void zpl_vec2_muleq(zpl_vec2 *d, zpl_f32 s)
{
/*@ assert rte_236: mem_access: \valid(&d->x); */
/*@ assert rte_237: mem_access: \valid_read(&d->x); */
/*@ assert rte_238: is_nan_or_infinite: \is_finite(\mul_float(d->x, s)); */
	d->x *= s;
/*@ assert rte_239: mem_access: \valid(&d->y); */
/*@ assert rte_240: mem_access: \valid_read(&d->y); */
/*@ assert rte_241: is_nan_or_infinite: \is_finite(\mul_float(d->y, s)); */
	d->y *= s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x);
	requires \is_finite(d->y);
	requires \is_finite(s);
	requires s != 0.0f;
	requires \is_finite(\div_float(d->x, s));
	requires \is_finite(\div_float(d->y, s));
	assigns d->x, d->y;
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures d->x == \div_float(\old(d->x), s);
	ensures d->y == \div_float(\old(d->y), s);
	ensures (s > 0.0f && \old(d->x) > 0.0f) ==> d->x > 0.0f;
	ensures (s > 0.0f && \old(d->x) < 0.0f) ==> d->x < 0.0f;
	ensures (s < 0.0f && \old(d->x) > 0.0f) ==> d->x < 0.0f;
	ensures (s < 0.0f && \old(d->x) < 0.0f) ==> d->x > 0.0f;
	ensures \old(d->x) == 0.0f ==> d->x == 0.0f;
	ensures (s > 0.0f && \old(d->y) > 0.0f) ==> d->y > 0.0f;
	ensures (s > 0.0f && \old(d->y) < 0.0f) ==> d->y < 0.0f;
	ensures (s < 0.0f && \old(d->y) > 0.0f) ==> d->y < 0.0f;
	ensures (s < 0.0f && \old(d->y) < 0.0f) ==> d->y > 0.0f;
	ensures \old(d->y) == 0.0f ==> d->y == 0.0f;
*/
void zpl_vec2_diveq(zpl_vec2 *d, zpl_f32 s)
{
/*@ assert rte_215: mem_access: \valid(&d->x); */
/*@ assert rte_216: mem_access: \valid_read(&d->x); */
/*@ assert rte_217: is_nan_or_infinite: \is_finite(\div_float(d->x, s)); */
	d->x /= s;
/*@ assert rte_218: mem_access: \valid(&d->y); */
/*@ assert rte_219: mem_access: \valid_read(&d->y); */
/*@ assert rte_220: is_nan_or_infinite: \is_finite(\div_float(d->y, s)); */
	d->y /= s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z);
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z);
	requires \is_finite(\add_float(d->x, v.x));
	requires \is_finite(\add_float(d->y, v.y));
	requires \is_finite(\add_float(d->z, v.z));
	assigns d->x, d->y, d->z;
	ensures \is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z);
	ensures (\is_finite(\old(d->x)) && \is_finite(v.x)&& \is_finite(\add_float(\old(d->x), v.x)))==> d->x == \old(d->x) + v.x;
	ensures (\is_finite(\old(d->y)) && \is_finite(v.y)&& \is_finite(\add_float(\old(d->y), v.y)))==> d->y == \old(d->y) + v.y;
	ensures (\is_finite(\old(d->z)) && \is_finite(v.z)&& \is_finite(\add_float(\old(d->z), v.z)))==> d->z == \old(d->z) + v.z;
*/
void zpl_vec3_addeq(zpl_vec3 *d, zpl_vec3 v)
{
/*@ assert rte_256: mem_access: \valid(&d->x); */
/*@ assert rte_257: mem_access: \valid_read(&d->x); */
/*@ assert rte_258: is_nan_or_infinite: \is_finite(\add_float(d->x, v.x)); */
	d->x += v.x;
/*@ assert rte_259: mem_access: \valid(&d->y); */
/*@ assert rte_260: mem_access: \valid_read(&d->y); */
/*@ assert rte_261: is_nan_or_infinite: \is_finite(\add_float(d->y, v.y)); */
	d->y += v.y;
/*@ assert rte_262: mem_access: \valid(&d->z); */
/*@ assert rte_263: mem_access: \valid_read(&d->z); */
/*@ assert rte_264: is_nan_or_infinite: \is_finite(\add_float(d->z, v.z)); */
	d->z += v.z;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x);
	requires \is_finite(d->y);
	requires \is_finite(d->z);
	requires \is_finite(v.x);
	requires \is_finite(v.y);
	requires \is_finite(v.z);
	requires \is_finite(\sub_float(d->x, v.x));
	requires \is_finite(\sub_float(d->y, v.y));
	requires \is_finite(\sub_float(d->z, v.z));
	assigns d->x, d->y, d->z;
	ensures \is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z);
	ensures d->x == \sub_float(\old(d->x), v.x);
	ensures d->y == \sub_float(\old(d->y), v.y);
	ensures d->z == \sub_float(\old(d->z), v.z);
*/
void zpl_vec3_subeq(zpl_vec3 *d, zpl_vec3 v)
{
/*@ assert rte_319: mem_access: \valid(&d->x); */
/*@ assert rte_320: mem_access: \valid_read(&d->x); */
/*@ assert rte_321: is_nan_or_infinite: \is_finite(\sub_float(d->x, v.x)); */
	d->x -= v.x;
/*@ assert rte_322: mem_access: \valid(&d->y); */
/*@ assert rte_323: mem_access: \valid_read(&d->y); */
/*@ assert rte_324: is_nan_or_infinite: \is_finite(\sub_float(d->y, v.y)); */
	d->y -= v.y;
/*@ assert rte_325: mem_access: \valid(&d->z); */
/*@ assert rte_326: mem_access: \valid_read(&d->z); */
/*@ assert rte_327: is_nan_or_infinite: \is_finite(\sub_float(d->z, v.z)); */
	d->z -= v.z;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x);
	requires \is_finite(d->y);
	requires \is_finite(d->z);
	requires \is_finite(s);
	requires \is_finite(\mul_float(d->x, s));
	requires \is_finite(\mul_float(d->y, s));
	requires \is_finite(\mul_float(d->z, s));
	assigns d->x, d->y, d->z;
	ensures \is_finite(d->x) ==> d->x == \mul_float(\old(d->x), s);
	ensures \is_finite(d->y) ==> d->y == \mul_float(\old(d->y), s);
	ensures \is_finite(d->z) ==> d->z == \mul_float(\old(d->z), s);
	ensures \old(\is_finite(d->x)) && \old(\is_finite(s)) && \old(\is_finite(\mul_float(d->x, s))) ==> \is_finite(d->x);
	ensures \old(\is_finite(d->y)) && \old(\is_finite(s)) && \old(\is_finite(\mul_float(d->y, s))) ==> \is_finite(d->y);
	ensures \old(\is_finite(d->z)) && \old(\is_finite(s)) && \old(\is_finite(\mul_float(d->z, s))) ==> \is_finite(d->z);
*/
void zpl_vec3_muleq(zpl_vec3 *d, zpl_f32 s)
{
/*@ assert rte_297: mem_access: \valid(&d->x); */
/*@ assert rte_298: mem_access: \valid_read(&d->x); */
/*@ assert rte_299: is_nan_or_infinite: \is_finite(\mul_float(d->x, s)); */
	d->x *= s;
/*@ assert rte_300: mem_access: \valid(&d->y); */
/*@ assert rte_301: mem_access: \valid_read(&d->y); */
/*@ assert rte_302: is_nan_or_infinite: \is_finite(\mul_float(d->y, s)); */
	d->y *= s;
/*@ assert rte_303: mem_access: \valid(&d->z); */
/*@ assert rte_304: mem_access: \valid_read(&d->z); */
/*@ assert rte_305: is_nan_or_infinite: \is_finite(\mul_float(d->z, s)); */
	d->z *= s;
	return;
}

/*@
	requires \valid(d);
	requires s != 0.0f;
	requires \is_finite((double)s);
	requires \is_finite((double)d->x);
	requires \is_finite((double)d->y);
	requires \is_finite((double)d->z);
	requires \is_finite((double)(d->x / s));
	requires \is_finite((double)(d->y / s));
	requires \is_finite((double)(d->z / s));
	assigns d->x, d->y, d->z;
	ensures \is_finite((double)d->x);
	ensures \is_finite((double)d->y);
	ensures \is_finite((double)d->z);
	ensures (\is_finite((double)\old(d->x)) && \is_finite((double)s) && s != 0.0f && \is_finite((double)\div_float(\old(d->x), s))) ==>d->x == \div_float(\old(d->x), s);
	ensures (\is_finite((double)\old(d->y)) && \is_finite((double)s) && s != 0.0f && \is_finite((double)\div_float(\old(d->y), s))) ==>d->y == \div_float(\old(d->y), s);
	ensures (\is_finite((double)\old(d->z)) && \is_finite((double)s) && s != 0.0f && \is_finite((double)\div_float(\old(d->z), s))) ==>d->z == \div_float(\old(d->z), s);
*/
void zpl_vec3_diveq(zpl_vec3 *d, zpl_f32 s)
{
/*@ assert rte_283: mem_access: \valid(&d->x); */
/*@ assert rte_284: mem_access: \valid_read(&d->x); */
/*@ assert rte_285: is_nan_or_infinite: \is_finite(\div_float(d->x, s)); */
	d->x /= s;
/*@ assert rte_286: mem_access: \valid(&d->y); */
/*@ assert rte_287: mem_access: \valid_read(&d->y); */
/*@ assert rte_288: is_nan_or_infinite: \is_finite(\div_float(d->y, s)); */
	d->y /= s;
/*@ assert rte_289: mem_access: \valid(&d->z); */
/*@ assert rte_290: mem_access: \valid_read(&d->z); */
/*@ assert rte_291: is_nan_or_infinite: \is_finite(\div_float(d->z, s)); */
	d->z /= s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x);
	requires \is_finite(v.x);
	requires \is_finite(\add_float(d->x, v.x));
	requires \is_finite(d->y);
	requires \is_finite(v.y);
	requires \is_finite(\add_float(d->y, v.y));
	requires \is_finite(d->z);
	requires \is_finite(v.z);
	requires \is_finite(\add_float(d->z, v.z));
	requires \is_finite(d->w);
	requires \is_finite(v.w);
	requires \is_finite(\add_float(d->w, v.w));
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(\old(d->x)) && \is_finite(v.x) &&\is_finite(\add_float(\old(d->x), v.x)) ==>d->x == \add_float(\old(d->x), v.x);
	ensures \is_finite(\old(d->y)) && \is_finite(v.y) &&\is_finite(\add_float(\old(d->y), v.y)) ==>d->y == \add_float(\old(d->y), v.y);
	ensures \is_finite(\old(d->z)) && \is_finite(v.z) &&\is_finite(\add_float(\old(d->z), v.z)) ==>d->z == \add_float(\old(d->z), v.z);
	ensures \is_finite(\old(d->w)) && \is_finite(v.w) &&\is_finite(\add_float(\old(d->w), v.w)) ==>d->w == \add_float(\old(d->w), v.w);
	ensures \is_finite(d->x);
	ensures \is_finite(d->y);
	ensures \is_finite(d->z);
	ensures \is_finite(d->w);
*/
void zpl_vec4_addeq(zpl_vec4 *d, zpl_vec4 v)
{
/*@ assert rte_336: mem_access: \valid(&d->x); */
/*@ assert rte_337: mem_access: \valid_read(&d->x); */
/*@ assert rte_338: is_nan_or_infinite: \is_finite(\add_float(d->x, v.x)); */
	d->x += v.x;
/*@ assert rte_339: mem_access: \valid(&d->y); */
/*@ assert rte_340: mem_access: \valid_read(&d->y); */
/*@ assert rte_341: is_nan_or_infinite: \is_finite(\add_float(d->y, v.y)); */
	d->y += v.y;
/*@ assert rte_342: mem_access: \valid(&d->z); */
/*@ assert rte_343: mem_access: \valid_read(&d->z); */
/*@ assert rte_344: is_nan_or_infinite: \is_finite(\add_float(d->z, v.z)); */
	d->z += v.z;
/*@ assert rte_345: mem_access: \valid(&d->w); */
/*@ assert rte_346: mem_access: \valid_read(&d->w); */
/*@ assert rte_347: is_nan_or_infinite: \is_finite(\add_float(d->w, v.w)); */
	d->w += v.w;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(d->x) && \is_finite(v.x) &&\is_finite(\sub_float(d->x, v.x));
	requires \is_finite(d->y) && \is_finite(v.y) &&\is_finite(\sub_float(d->y, v.y));
	requires \is_finite(d->z) && \is_finite(v.z) &&\is_finite(\sub_float(d->z, v.z));
	requires \is_finite(d->w) && \is_finite(v.w) &&\is_finite(\sub_float(d->w, v.w));
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(d->x) &&\is_finite(d->y) &&\is_finite(d->z) &&\is_finite(d->w);
	ensures (\is_finite(\old(d->x)) && \is_finite(v.x) &&\is_finite(\sub_float(\old(d->x), v.x))) ==>d->x == \sub_float(\old(d->x), v.x);
	ensures (\is_finite(\old(d->y)) && \is_finite(v.y) &&\is_finite(\sub_float(\old(d->y), v.y))) ==>d->y == \sub_float(\old(d->y), v.y);
	ensures (\is_finite(\old(d->z)) && \is_finite(v.z) &&\is_finite(\sub_float(\old(d->z), v.z))) ==>d->z == \sub_float(\old(d->z), v.z);
	ensures (\is_finite(\old(d->w)) && \is_finite(v.w) &&\is_finite(\sub_float(\old(d->w), v.w))) ==>d->w == \sub_float(\old(d->w), v.w);
*/
void zpl_vec4_subeq(zpl_vec4 *d, zpl_vec4 v)
{
/*@ assert rte_403: mem_access: \valid(&d->x); */
/*@ assert rte_404: mem_access: \valid_read(&d->x); */
/*@ assert rte_405: is_nan_or_infinite: \is_finite(\sub_float(d->x, v.x)); */
	d->x -= v.x;
/*@ assert rte_406: mem_access: \valid(&d->y); */
/*@ assert rte_407: mem_access: \valid_read(&d->y); */
/*@ assert rte_408: is_nan_or_infinite: \is_finite(\sub_float(d->y, v.y)); */
	d->y -= v.y;
/*@ assert rte_409: mem_access: \valid(&d->z); */
/*@ assert rte_410: mem_access: \valid_read(&d->z); */
/*@ assert rte_411: is_nan_or_infinite: \is_finite(\sub_float(d->z, v.z)); */
	d->z -= v.z;
/*@ assert rte_412: mem_access: \valid(&d->w); */
/*@ assert rte_413: mem_access: \valid_read(&d->w); */
/*@ assert rte_414: is_nan_or_infinite: \is_finite(\sub_float(d->w, v.w)); */
	d->w -= v.w;
	return;
}

/*@
	requires \is_finite(\mul_float(d->w, s));
	requires \is_finite(\mul_float(d->z, s));
	requires \is_finite(\mul_float(d->y, s));
	requires \is_finite(\mul_float(d->x, s));
	requires \is_finite(d->w);
	requires \is_finite(d->z);
	requires \is_finite(d->y);
	requires \is_finite(d->x);
	requires \is_finite(s);
	requires \valid(d);
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(d->x) && \is_finite(d->y) &&@          \is_finite(d->z) && \is_finite(d->w);
	ensures d->x == \mul_float(\old(d->x), s);
	ensures d->y == \mul_float(\old(d->y), s);
	ensures d->z == \mul_float(\old(d->z), s);
	ensures d->w == \mul_float(\old(d->w), s);
*/
void zpl_vec4_muleq(zpl_vec4 *d, zpl_f32 s)
{
/*@ assert rte_375: mem_access: \valid(&d->x); */
/*@ assert rte_376: mem_access: \valid_read(&d->x); */
/*@ assert rte_377: is_nan_or_infinite: \is_finite(\mul_float(d->x, s)); */
	d->x *= s;
/*@ assert rte_378: mem_access: \valid(&d->y); */
/*@ assert rte_379: mem_access: \valid_read(&d->y); */
/*@ assert rte_380: is_nan_or_infinite: \is_finite(\mul_float(d->y, s)); */
	d->y *= s;
/*@ assert rte_381: mem_access: \valid(&d->z); */
/*@ assert rte_382: mem_access: \valid_read(&d->z); */
/*@ assert rte_383: is_nan_or_infinite: \is_finite(\mul_float(d->z, s)); */
	d->z *= s;
/*@ assert rte_384: mem_access: \valid(&d->w); */
/*@ assert rte_385: mem_access: \valid_read(&d->w); */
/*@ assert rte_386: is_nan_or_infinite: \is_finite(\mul_float(d->w, s)); */
	d->w *= s;
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(s);
	requires s != 0.0f;
	requires \is_finite(d->x);
	requires \is_finite(d->y);
	requires \is_finite(d->z);
	requires \is_finite(d->w);
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->x)) ==>\is_finite(d->x);
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->y)) ==>\is_finite(d->y);
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->z)) ==>\is_finite(d->z);
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->w)) ==>\is_finite(d->w);
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->x)) ==>d->x == \old(d->x) / s;
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->y)) ==>d->y == \old(d->y) / s;
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->z)) ==>d->z == \old(d->z) / s;
	ensures \is_finite(s) && s != 0.0f && \is_finite(\old(d->w)) ==>d->w == \old(d->w) / s;
*/
void zpl_vec4_diveq(zpl_vec4 *d, zpl_f32 s)
{
/*@ assert rte_356: mem_access: \valid(&d->x); */
/*@ assert rte_357: mem_access: \valid_read(&d->x); */
/*@ assert rte_358: is_nan_or_infinite: \is_finite(\div_float(d->x, s)); */
	d->x /= s;
/*@ assert rte_359: mem_access: \valid(&d->y); */
/*@ assert rte_360: mem_access: \valid_read(&d->y); */
/*@ assert rte_361: is_nan_or_infinite: \is_finite(\div_float(d->y, s)); */
	d->y /= s;
/*@ assert rte_362: mem_access: \valid(&d->z); */
/*@ assert rte_363: mem_access: \valid_read(&d->z); */
/*@ assert rte_364: is_nan_or_infinite: \is_finite(\div_float(d->z, s)); */
	d->z /= s;
/*@ assert rte_365: mem_access: \valid(&d->w); */
/*@ assert rte_366: mem_access: \valid_read(&d->w); */
/*@ assert rte_367: is_nan_or_infinite: \is_finite(\div_float(d->w, s)); */
	d->w /= s;
	return;
}

/*@
	requires \add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)) > 0.0f;
	requires \is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)));
	requires \is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)));
	requires \is_finite(\mul_float(v.z, v.z));
	requires \is_finite(\mul_float(v.y, v.y));
	requires \is_finite(\mul_float(v.x, v.x));
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z);
	requires \valid(d);
	assigns *d;
	ensures \is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z);
	ensures (\is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\mul_float(v.z, v.z)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) &&\is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z))) &&\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)) > 0.0f)==>(\add_float(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y)), \mul_float(d->z, d->z)) >= 1.0f - 0x1p-22f &&\add_float(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y)), \mul_float(d->z, d->z)) <= 1.0f + 0x1p-22f);
	ensures (\is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\mul_float(v.z, v.z)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) &&\is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z))) &&\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)) > 0.0f)==>\is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z);
	ensures (\is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\mul_float(v.z, v.z)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) &&\is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z))) &&\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)) > 0.0f &&\is_finite(\mul_float(d->x, d->x)) &&\is_finite(\mul_float(d->y, d->y)) &&\is_finite(\mul_float(d->z, d->z)) &&\is_finite(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y))) &&\is_finite(\add_float(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y)), \mul_float(d->z, d->z))))==>\add_float(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y)), \mul_float(d->z, d->z)) >= 1.0f - 0x1p-22f &&\add_float(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y)), \mul_float(d->z, d->z)) <= 1.0f + 0x1p-22f;
*/
void zpl_vec3_norm(zpl_vec3 *d, zpl_vec3 v)
{
	zpl_f32 tmp_0;
	zpl_f32 tmp;
	tmp = zpl_vec3_dot(v,v);
	tmp_0 = zpl_rsqrt(tmp);
	zpl_f32 inv_mag = tmp_0;
	zpl_vec3_mul(d,v,inv_mag);
	return;
}

/*@
	requires \add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w)) > 0.0f;
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w)));
	requires \is_finite(\mul_float(v.w, v.w));
	requires \is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)));
	requires \is_finite(\mul_float(v.z, v.z));
	requires \is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)));
	requires \is_finite(\mul_float(v.y, v.y));
	requires \is_finite(\mul_float(v.x, v.x));
	requires \is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w);
	requires \valid(d);
	assigns d->x, d->y, d->z, d->w;
	ensures \is_finite(d->x) && \is_finite(d->y) && \is_finite(d->z) && \is_finite(d->w);
	ensures (\is_finite(v.x) && \is_finite(v.y) && \is_finite(v.z) && \is_finite(v.w) &&\is_finite(\mul_float(v.x, v.x)) &&\is_finite(\mul_float(v.y, v.y)) &&\is_finite(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y))) &&\is_finite(\mul_float(v.z, v.z)) &&\is_finite(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z))) &&\is_finite(\mul_float(v.w, v.w)) &&\is_finite(\add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w))) &&\add_float(\add_float(\add_float(\mul_float(v.x, v.x), \mul_float(v.y, v.y)), \mul_float(v.z, v.z)), \mul_float(v.w, v.w)) > 0.0f &&\is_finite(\mul_float(v.x, d->x)) &&\is_finite(\mul_float(v.y, d->y)) &&\is_finite(\mul_float(v.z, d->z)) &&\is_finite(\mul_float(v.w, d->w)))==> \add_float(\add_float(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y)), \mul_float(d->z, d->z)), \mul_float(d->w, d->w)) >= 1.0f - 0x1p-23f &&\add_float(\add_float(\add_float(\mul_float(d->x, d->x), \mul_float(d->y, d->y)), \mul_float(d->z, d->z)), \mul_float(d->w, d->w)) <= 1.0f + 0x1p-23f;
*/
void zpl_vec4_norm(zpl_vec4 *d, zpl_vec4 v)
{
	zpl_f32 tmp_0;
	zpl_f32 tmp;
	tmp = zpl_vec4_dot(v,v);
	tmp_0 = zpl_rsqrt(tmp);
	zpl_f32 inv_mag = tmp_0;
	zpl_vec4_mul(d,v,inv_mag);
	return;
}

/*@
	requires \valid(d);
	requires \is_finite(v0.x);
	requires \is_finite(v0.y);
	requires \is_finite(v1.x);
	requires \is_finite(v1.y);
	requires \is_finite(\mul_float(v0.x, v1.y));
	requires \is_finite(\mul_float(v1.x, v0.y));
	requires \is_finite(\sub_float(\mul_float(v0.x, v1.y), \mul_float(v1.x, v0.y)));
	assigns *d;
	ensures \is_finite(*d);
	ensures *d == \sub_float(\mul_float(v0.x, v1.y), \mul_float(v1.x, v0.y));
	ensures (\is_finite(\mul_float(v0.x, v1.y)) && \is_finite(\mul_float(v1.x, v0.y)))@         ==> \is_finite(*d);
	ensures (\mul_float(v0.x, v1.y) == \mul_float(v1.x, v0.y))@         ==> *d == 0.0f;
	ensures (\mul_float(v0.x, v1.y) > \mul_float(v1.x, v0.y))@         ==> *d > 0.0f;
	ensures (\mul_float(v0.x, v1.y) < \mul_float(v1.x, v0.y))@         ==> *d < 0.0f;
*/
void zpl_vec2_cross(zpl_f32 *d, zpl_vec2 v0, zpl_vec2 v1)
{
/*@ assert rte_207: mem_access: \valid(d); */
/*@ assert rte_208: is_nan_or_infinite: \is_finite(\mul_float(v0.x, v1.y)); */
/*@ assert rte_209: is_nan_or_infinite: \is_finite(\mul_float(v1.x, v0.y)); */
/*@ assert rte_210: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(v0.x, v1.y), \mul_float(v1.x, v0.y))); */
	*d = v0.x * v1.y - v1.x * v0.y;
	return;
}

/*@
	requires \valid(m);
	assigns m->col[0].y, m->col[1].x;
	ensures m->col[0].y == \old(m->col[1].x);
	ensures m->col[1].x == \old(m->col[0].y);
	ensures m->col[0].x == \old(m->col[0].x);
	ensures m->col[1].y == \old(m->col[1].y);
*/
void zpl_mat2_transpose(zpl_mat2 *m)
{
/*@ assert rte_54: mem_access: \valid_read(&m->col[0].y); */
	zpl_f32 tmp = m->col[0].y;
/*@ assert rte_55: mem_access: \valid(&m->col[0].y); */
/*@ assert rte_56: mem_access: \valid_read(&m->col[1].x); */
	m->col[0].y = m->col[1].x;
/*@ assert rte_57: mem_access: \valid(&m->col[1].x); */
	m->col[1].x = tmp;
	return;
}

/*@
	requires \valid(m);
	assigns m->col[0].y, m->col[1].x,m->col[0].z, m->col[2].x,m->col[0].w, m->col[3].x,m->col[1].z, m->col[2].y,m->col[1].w, m->col[3].y,m->col[2].w, m->col[3].z;
	ensures m->col[0].y == \old(m->col[1].x);
	ensures m->col[1].x == \old(m->col[0].y);
	ensures m->col[0].z == \old(m->col[2].x);
	ensures m->col[2].x == \old(m->col[0].z);
	ensures m->col[0].w == \old(m->col[3].x);
	ensures m->col[3].x == \old(m->col[0].w);
	ensures m->col[1].z == \old(m->col[2].y);
	ensures m->col[2].y == \old(m->col[1].z);
	ensures m->col[1].w == \old(m->col[3].y);
	ensures m->col[3].y == \old(m->col[1].w);
	ensures m->col[2].w == \old(m->col[3].z);
	ensures m->col[3].z == \old(m->col[2].w);
	ensures m->col[0].x == \old(m->col[0].x);
	ensures m->col[1].y == \old(m->col[1].y);
	ensures m->col[2].z == \old(m->col[2].z);
	ensures m->col[3].w == \old(m->col[3].w);
*/
void zpl_mat4_transpose(zpl_mat4 *m)
{
	zpl_float44_transpose((zpl_f32 (*)[4])m);
	return;
}

/*@
	requires \separated(out, m);
	requires \valid_read(m);
	requires \valid(out);
	assigns *out;
	ensures \valid(out);
*/
void zpl_mat2_copy(zpl_mat2 *out, zpl_mat2 *m)
{
/*@ assert rte_21: mem_access: \valid(out); */
/*@ assert rte_22: mem_access: \valid_read(m); */
	*out = *m;
	return;
}

/*@
	requires \separated(out, m);
	requires \valid_read(m);
	requires \valid(out);
	assigns *out;
	ensures \valid(out);
	ensures *out == \old(*m);
*/
void zpl_mat4_copy(zpl_mat4 *out, zpl_mat4 *m)
{
/*@ assert rte_58: mem_access: \valid(out); */
/*@ assert rte_59: mem_access: \valid_read(m); */
	*out = *m;
	return;
}

/*@
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m->col[0].w, in.x), \mul_float(m->col[1].w, in.y)), \mul_float(m->col[2].w, in.z)), \mul_float(m->col[3].w, in.w)));
	requires \is_finite(\mul_float(m->col[3].w, in.w));
	requires \is_finite(\add_float(\add_float(\mul_float(m->col[0].w, in.x), \mul_float(m->col[1].w, in.y)), \mul_float(m->col[2].w, in.z)));
	requires \is_finite(\mul_float(m->col[2].w, in.z));
	requires \is_finite(\add_float(\mul_float(m->col[0].w, in.x), \mul_float(m->col[1].w, in.y)));
	requires \is_finite(\mul_float(m->col[1].w, in.y));
	requires \is_finite(\mul_float(m->col[0].w, in.x));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m->col[0].z, in.x), \mul_float(m->col[1].z, in.y)), \mul_float(m->col[2].z, in.z)), \mul_float(m->col[3].z, in.w)));
	requires \is_finite(\mul_float(m->col[3].z, in.w));
	requires \is_finite(\add_float(\add_float(\mul_float(m->col[0].z, in.x), \mul_float(m->col[1].z, in.y)), \mul_float(m->col[2].z, in.z)));
	requires \is_finite(\mul_float(m->col[2].z, in.z));
	requires \is_finite(\add_float(\mul_float(m->col[0].z, in.x), \mul_float(m->col[1].z, in.y)));
	requires \is_finite(\mul_float(m->col[1].z, in.y));
	requires \is_finite(\mul_float(m->col[0].z, in.x));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m->col[0].y, in.x), \mul_float(m->col[1].y, in.y)), \mul_float(m->col[2].y, in.z)), \mul_float(m->col[3].y, in.w)));
	requires \is_finite(\mul_float(m->col[3].y, in.w));
	requires \is_finite(\add_float(\add_float(\mul_float(m->col[0].y, in.x), \mul_float(m->col[1].y, in.y)), \mul_float(m->col[2].y, in.z)));
	requires \is_finite(\mul_float(m->col[2].y, in.z));
	requires \is_finite(\add_float(\mul_float(m->col[0].y, in.x), \mul_float(m->col[1].y, in.y)));
	requires \is_finite(\mul_float(m->col[1].y, in.y));
	requires \is_finite(\mul_float(m->col[0].y, in.x));
	requires \is_finite(\add_float(\add_float(\add_float(\mul_float(m->col[0].x, in.x), \mul_float(m->col[1].x, in.y)), \mul_float(m->col[2].x, in.z)), \mul_float(m->col[3].x, in.w)));
	requires \is_finite(\mul_float(m->col[3].x, in.w));
	requires \is_finite(\add_float(\add_float(\mul_float(m->col[0].x, in.x), \mul_float(m->col[1].x, in.y)), \mul_float(m->col[2].x, in.z)));
	requires \is_finite(\mul_float(m->col[2].x, in.z));
	requires \is_finite(\add_float(\mul_float(m->col[0].x, in.x), \mul_float(m->col[1].x, in.y)));
	requires \is_finite(\mul_float(m->col[1].x, in.y));
	requires \is_finite(\mul_float(m->col[0].x, in.x));
	requires \is_finite(m->col[3].w);
	requires \is_finite(m->col[3].z);
	requires \is_finite(m->col[3].y);
	requires \is_finite(m->col[3].x);
	requires \is_finite(m->col[2].w);
	requires \is_finite(m->col[2].z);
	requires \is_finite(m->col[2].y);
	requires \is_finite(m->col[2].x);
	requires \is_finite(m->col[1].w);
	requires \is_finite(m->col[1].z);
	requires \is_finite(m->col[1].y);
	requires \is_finite(m->col[1].x);
	requires \is_finite(m->col[0].w);
	requires \is_finite(m->col[0].z);
	requires \is_finite(m->col[0].y);
	requires \is_finite(m->col[0].x);
	requires \is_finite(in.w);
	requires \is_finite(in.z);
	requires \is_finite(in.y);
	requires \is_finite(in.x);
	requires \separated(out, (zpl_f32 *)m);
	requires \valid_read(m);
	requires \valid(out);
	assigns out->x, out->y, out->z, out->w;
	assigns *out;
	ensures \is_finite(out->x) && \is_finite(out->y) &&\is_finite(out->z) && \is_finite(out->w);
	ensures out->x == ((((\old(m->col[0].x) * \old(in.x)) +(\old(m->col[1].x) * \old(in.y))) +(\old(m->col[2].x) * \old(in.z))) +(\old(m->col[3].x) * \old(in.w)));
*/
void zpl_mat4_mul_vec4(zpl_vec4 *out, zpl_mat4 *m, zpl_vec4 in)
{
	zpl_float44_mul_vec4(out,(zpl_f32 (*)[4])m,in);
	return;
}

/*@
	requires \valid(m);
	assigns \nothing;
	ensures \result == (zpl_float2 *)m;
	ensures \result != \null;
	ensures \valid(\result + (0..1));
*/
zpl_float2 *zpl_float22_m(zpl_mat2 *m)
{
	zpl_float2 *__retres;
	__retres = (zpl_float2 *)m;
	return __retres;
}

/*@
	requires \valid(m);
	assigns \nothing;
	ensures \result == (zpl_float4 *)m;
	ensures (zpl_f32 *)\result == (zpl_f32 *)m;
	ensures \valid(m) ==> \result != \null;
	ensures \valid(m) ==> \valid(\result + (0..3));
*/
zpl_float4 *zpl_float44_m(zpl_mat4 *m)
{
	zpl_float4 *__retres;
	__retres = (zpl_float4 *)m;
	return __retres;
}


