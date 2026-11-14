#include <stdint.h>
#include <math.h>

typedef float zpl_f32;
typedef double zpl_f64;
typedef int32_t zpl_i32;
typedef int64_t zpl_i64;
typedef uint16_t zpl_half;
typedef uint8_t zpl_b8;

struct zpl_vec2 { zpl_f32 x, y; };
typedef struct zpl_vec2 zpl_vec2;

struct zpl_vec3 { zpl_f32 x, y, z; };
typedef struct zpl_vec3 zpl_vec3;

struct zpl_vec4{ zpl_f32 x, y, z, w; };
typedef struct zpl_vec4 zpl_vec4;
typedef zpl_vec4 zpl_quat;

struct zpl_mat2 { zpl_vec2 col[2]; };
typedef struct zpl_mat2 zpl_mat2;

struct zpl_mat3 { zpl_vec3 col[3]; };
typedef struct zpl_mat3 zpl_mat3;

struct zpl_mat4 { zpl_vec4 col[4]; };
typedef struct zpl_mat4 zpl_mat4;

struct zpl_rect2 { zpl_vec2 pos, dim; };
typedef struct zpl_rect2 zpl_rect2;

struct zpl_rect3 { zpl_vec3 pos, dim; };
typedef struct zpl_rect3 zpl_rect3;

struct zpl_aabb2 { zpl_vec2 min, max; };
typedef struct zpl_aabb2 zpl_aabb2;

struct zpl_aabb3 { zpl_vec3 min, max; };
typedef struct zpl_aabb3 zpl_aabb3;

#define ZPL_TAU 6.28318530717958647692f

typedef zpl_f32 zpl_float2[2];
typedef zpl_f32 zpl_float3[3];
typedef zpl_f32 zpl_float4[4];


zpl_f32 zpl_sqrt(zpl_f32 x) { return sqrtf(x); }

zpl_f32 zpl_rsqrt(zpl_f32 x) { return 1.0f / sqrtf(x); }

zpl_f32 zpl_sin(zpl_f32 x) { return sinf(x); }

zpl_f32 zpl_cos(zpl_f32 x) { return cosf(x); }

/*  assigns \nothing; */
zpl_f32 zpl_to_radians(zpl_f32 degrees) { return degrees * 0x1.921fb6p1f / 360.0f; }

/*  assigns \nothing; */
zpl_f32 zpl_to_degrees(zpl_f32 radians) { return radians * 360.0f / 0x1.921fb6p1f; }


zpl_f32 zpl_round(zpl_f32 x) { return floorf(x + 0.5f); }


zpl_f32 zpl_remainder(zpl_f32 x, zpl_f32 y) { return x - (zpl_round(x / y) * y); }


zpl_vec2 zpl_vec2f_zero(void) {
    zpl_vec2 v = { 0, 0 };
    return v;
}


zpl_vec2 zpl_vec2f(zpl_f32 x, zpl_f32 y) {
    zpl_vec2 v;
    v.x = x;
    v.y = y;
    return v;
}


zpl_vec2 zpl_vec2fv(zpl_f32 x[2]) {
    zpl_vec2 v;
    v.x = x[0];
    v.y = x[1];
    return v;
}


zpl_vec3 zpl_vec3f_zero(void) {
    zpl_vec3 v = { 0, 0, 0 };
    return v;
}


zpl_vec3 zpl_vec3f(zpl_f32 x, zpl_f32 y, zpl_f32 z) {
    zpl_vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}


zpl_vec4 zpl_vec4f_zero(void) {
    zpl_vec4 v = { 0, 0, 0, 0 };
    return v;
}


zpl_vec4 zpl_vec4f(zpl_f32 x, zpl_f32 y, zpl_f32 z, zpl_f32 w) {
    zpl_vec4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}


zpl_f32 zpl_vec2_dot(zpl_vec2 v0, zpl_vec2 v1) { 
    return v0.x * v1.x + v0.y * v1.y; 
}


zpl_f32 zpl_vec3_dot(zpl_vec3 v0, zpl_vec3 v1) { 
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; 
}


zpl_f32 zpl_vec4_dot(zpl_vec4 v0, zpl_vec4 v1) {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
}


zpl_f32 zpl_vec2_mag2(zpl_vec2 v) { 
    return zpl_vec2_dot(v, v); 
}



zpl_f32 zpl_vec3_mag2(zpl_vec3 v) { 
    return zpl_vec3_dot(v, v); 
}



zpl_f32 zpl_vec4_mag2(zpl_vec4 v) { 
    return zpl_vec4_dot(v, v); 
}



void zpl_vec2_add(zpl_vec2 *d, zpl_vec2 v0, zpl_vec2 v1) { 
    d->x = v0.x + v1.x; 
    d->y = v0.y + v1.y; 
}


void zpl_vec2_sub(zpl_vec2 *d, zpl_vec2 v0, zpl_vec2 v1) { 
    d->x = v0.x - v1.x; 
    d->y = v0.y - v1.y; 
}


void zpl_vec2_mul(zpl_vec2 *d, zpl_vec2 v, zpl_f32 s) { 
    d->x = v.x * s; 
    d->y = v.y * s; 
}


void zpl_vec2_div(zpl_vec2 *d, zpl_vec2 v, zpl_f32 s) { 
    d->x = v.x / s; 
    d->y = v.y / s; 
}



void zpl_vec3_add(zpl_vec3 *d, zpl_vec3 v0, zpl_vec3 v1) { 
    d->x = v0.x + v1.x; 
    d->y = v0.y + v1.y; 
    d->z = v0.z + v1.z; 
}


void zpl_vec3_cross(zpl_vec3 *d, zpl_vec3 v0, zpl_vec3 v1) {
    d->x = v0.y * v1.z - v0.z * v1.y;
    d->y = v0.z * v1.x - v0.x * v1.z;
    d->z = v0.x * v1.y - v0.y * v1.x;
}


void zpl_vec4_add(zpl_vec4 *d, zpl_vec4 v0, zpl_vec4 v1) {
    d->x = v0.x + v1.x;
    d->y = v0.y + v1.y;
    d->z = v0.z + v1.z;
    d->w = v0.w + v1.w;
}


void zpl_vec4_mul(zpl_vec4 *d, zpl_vec4 v, zpl_f32 s) {
    d->x = v.x * s;
    d->y = v.y * s;
    d->z = v.z * s;
    d->w = v.w * s;
}


void zpl_mat2_identity(zpl_mat2 *m) { 
    m->col[0] = zpl_vec2f(1,0); 
    m->col[1] = zpl_vec2f(0,1); 
}


void zpl_mat2_mul(zpl_mat2 *out, zpl_mat2 *m1, zpl_mat2 *m2) {
    zpl_mat2 temp;
    temp.col[0].x = m1->col[0].x * m2->col[0].x + m1->col[1].x * m2->col[0].y;
    temp.col[0].y = m1->col[0].y * m2->col[0].x + m1->col[1].y * m2->col[0].y;
    temp.col[1].x = m1->col[0].x * m2->col[1].x + m1->col[1].x * m2->col[1].y;
    temp.col[1].y = m1->col[0].y * m2->col[1].x + m1->col[1].y * m2->col[1].y;
    *out = temp;
}


void zpl_mat4_identity(zpl_mat4 *m) {
    m->col[0] = zpl_vec4f(1, 0, 0, 0);
    m->col[1] = zpl_vec4f(0, 1, 0, 0);
    m->col[2] = zpl_vec4f(0, 0, 1, 0);
    m->col[3] = zpl_vec4f(0, 0, 0, 1);
}


void zpl_float44_identity(zpl_f32 m[4][4]) {
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}


void zpl_float44_transpose(zpl_f32 (*vec)[4]) {
    zpl_f32 tmp;
    
    tmp = vec[1][0]; vec[1][0] = vec[0][1]; vec[0][1] = tmp;
    tmp = vec[2][0]; vec[2][0] = vec[0][2]; vec[0][2] = tmp;
    tmp = vec[3][0]; vec[3][0] = vec[0][3]; vec[0][3] = tmp;
    tmp = vec[2][1]; vec[2][1] = vec[1][2]; vec[1][2] = tmp;
    tmp = vec[3][1]; vec[3][1] = vec[1][3]; vec[1][3] = tmp;
    tmp = vec[3][2]; vec[3][2] = vec[2][3]; vec[2][3] = tmp;
}


void zpl_float44_mul_vec4(zpl_vec4 *out, zpl_f32 m[4][4], zpl_vec4 v) {
    out->x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;
    out->y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;
    out->z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;
    out->w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;
}


void zpl_mat4_to_translate(zpl_mat4 *out, zpl_vec3 v) {
    zpl_mat4_identity(out);
    out->col[3].x = v.x;
    out->col[3].y = v.y;
    out->col[3].z = v.z;
}


void zpl_mat4_to_scale(zpl_mat4 *out, zpl_vec3 v) {
    zpl_mat4_identity(out);
    out->col[0].x = v.x;
    out->col[1].y = v.y;
    out->col[2].z = v.z;
}


void zpl_mat4_to_scalef(zpl_mat4 *out, zpl_f32 s) {
    zpl_mat4_identity(out);
    out->col[0].x = s;
    out->col[1].y = s;
    out->col[2].z = s;
}


void zpl_mat4_ortho2d(zpl_mat4 *out, zpl_f32 left, zpl_f32 right, zpl_f32 bottom, zpl_f32 top) {
    zpl_mat4_identity(out);
    
    out->col[0].x = 2.0f / (right - left);
    out->col[1].y = 2.0f / (top - bottom);
    out->col[2].z = -1.0f;
    out->col[3].x = -(right + left) / (right - left);
    out->col[3].y = -(top + bottom) / (top - bottom);
}



zpl_quat zpl_quat_identity(void) {
    zpl_quat q = { 0, 0, 0, 1 };
    return q;
}


void zpl_quat_mul(zpl_quat *d, zpl_quat q0, zpl_quat q1) {
    d->x = q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y;
    d->y = q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x;
    d->z = q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w;
    d->w = q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
}



zpl_rect2 zpl_rect2f(zpl_vec2 pos, zpl_vec2 dim) {
    zpl_rect2 r;
    r.pos = pos;
    r.dim = dim;
    return r;
}


zpl_aabb2 zpl_aabb2f(zpl_f32 minx, zpl_f32 miny, zpl_f32 maxx, zpl_f32 maxy) {
    zpl_aabb2 r;
    r.min = zpl_vec2f(minx, miny);
    r.max = zpl_vec2f(maxx, maxy);
    return r;
}


int zpl_aabb2_contains(zpl_aabb2 a, zpl_f32 x, zpl_f32 y) {
    return (x >= a.min.x && x <= a.max.x && y >= a.min.y && y <= a.max.y);
}


zpl_f32 zpl_lerp(zpl_f32 a, zpl_f32 b, zpl_f32 t) { 
    return a * (1.0f - t) + b * t; 
}


void zpl_vec2_lerp(zpl_vec2 *d, zpl_vec2 a, zpl_vec2 b, zpl_f32 t) { 
    zpl_vec2 db;
    zpl_vec2_sub(&db, b, a);
    zpl_vec2_mul(&db, db, t);
    zpl_vec2_add(d, a, db);
}


void zpl_vec3_sub(zpl_vec3 *d, zpl_vec3 v0, zpl_vec3 v1) { 
    d->x = v0.x - v1.x; 
    d->y = v0.y - v1.y; 
    d->z = v0.z - v1.z; 
}


void zpl_vec3_mul(zpl_vec3 *d, zpl_vec3 v, zpl_f32 s) { 
    d->x = v.x * s; 
    d->y = v.y * s; 
    d->z = v.z * s; 
}


void zpl_vec3_div(zpl_vec3 *d, zpl_vec3 v, zpl_f32 s) { 
    d->x = v.x / s; 
    d->y = v.y / s; 
    d->z = v.z / s; 
}


void zpl_vec4_sub(zpl_vec4 *d, zpl_vec4 v0, zpl_vec4 v1) {
    d->x = v0.x - v1.x;
    d->y = v0.y - v1.y;
    d->z = v0.z - v1.z;
    d->w = v0.w - v1.w;
}


void zpl_vec4_div(zpl_vec4 *d, zpl_vec4 v, zpl_f32 s) {
    d->x = v.x / s;
    d->y = v.y / s;
    d->z = v.z / s;
    d->w = v.w / s;
}


void zpl_vec2_addeq(zpl_vec2 *d, zpl_vec2 v) { 
    d->x += v.x; 
    d->y += v.y; 
}


void zpl_vec2_subeq(zpl_vec2 *d, zpl_vec2 v) { 
    d->x -= v.x; 
    d->y -= v.y; 
}


void zpl_vec2_muleq(zpl_vec2 *d, zpl_f32 s) { 
    d->x *= s; 
    d->y *= s; 
}


void zpl_vec2_diveq(zpl_vec2 *d, zpl_f32 s) { 
    d->x /= s; 
    d->y /= s; 
}


void zpl_vec3_addeq(zpl_vec3 *d, zpl_vec3 v) { 
    d->x += v.x; 
    d->y += v.y; 
    d->z += v.z; 
}


void zpl_vec3_subeq(zpl_vec3 *d, zpl_vec3 v) { 
    d->x -= v.x; 
    d->y -= v.y; 
    d->z -= v.z; 
}


void zpl_vec3_muleq(zpl_vec3 *d, zpl_f32 s) { 
    d->x *= s; 
    d->y *= s; 
    d->z *= s; 
}


void zpl_vec3_diveq(zpl_vec3 *d, zpl_f32 s) { 
    d->x /= s; 
    d->y /= s; 
    d->z /= s; 
}


void zpl_vec4_addeq(zpl_vec4 *d, zpl_vec4 v) { 
    d->x += v.x; 
    d->y += v.y; 
    d->z += v.z; 
    d->w += v.w; 
}


void zpl_vec4_subeq(zpl_vec4 *d, zpl_vec4 v) { 
    d->x -= v.x; 
    d->y -= v.y; 
    d->z -= v.z; 
    d->w -= v.w; 
}


void zpl_vec4_muleq(zpl_vec4 *d, zpl_f32 s) { 
    d->x *= s; 
    d->y *= s; 
    d->z *= s; 
    d->w *= s; 
}


void zpl_vec4_diveq(zpl_vec4 *d, zpl_f32 s) { 
    d->x /= s; 
    d->y /= s; 
    d->z /= s; 
    d->w /= s; 
}


void zpl_vec3_norm(zpl_vec3 *d, zpl_vec3 v) {
    zpl_f32 inv_mag = zpl_rsqrt(zpl_vec3_dot(v, v));
    zpl_vec3_mul(d, v, inv_mag);
}


void zpl_vec4_norm(zpl_vec4 *d, zpl_vec4 v) {
    zpl_f32 inv_mag = zpl_rsqrt(zpl_vec4_dot(v, v));
    zpl_vec4_mul(d, v, inv_mag);
}


// void zpl_vec2_norm0(zpl_vec2 *d, zpl_vec2 v) {
//     zpl_f32 mag = zpl_vec2_mag(v);
//     if (mag > 0)
//         zpl_vec2_div(d, v, mag);
//     else
//         *d = zpl_vec2f_zero();
// }


// void zpl_vec3_norm0(zpl_vec3 *d, zpl_vec3 v) {
//     zpl_f32 mag = zpl_vec3_mag(v);
//     if (mag > 0)
//         zpl_vec3_div(d, v, mag);
//     else
//         *d = zpl_vec3f_zero();
// }


// void zpl_vec4_norm0(zpl_vec4 *d, zpl_vec4 v) {
//     zpl_f32 mag = zpl_vec4_mag(v);
//     if (mag > 0)
//         zpl_vec4_div(d, v, mag);
//     else
//         *d = zpl_vec4f_zero();
// }


void zpl_vec2_cross(zpl_f32 *d, zpl_vec2 v0, zpl_vec2 v1) { 
    *d = v0.x * v1.y - v1.x * v0.y; 
}


void zpl_mat2_transpose(zpl_mat2 *m) { 
    zpl_f32 tmp = m->col[0].y;
    m->col[0].y = m->col[1].x;
    m->col[1].x = tmp;
}


void zpl_mat4_transpose(zpl_mat4 *m) { 
    zpl_float44_transpose((zpl_f32(*)[4])m); 
}


void zpl_mat2_copy(zpl_mat2* out, zpl_mat2* m) {
    *out = *m;
}


void zpl_mat4_copy(zpl_mat4* out, zpl_mat4* m) {
    *out = *m;
}


void zpl_mat4_mul_vec4(zpl_vec4 *out, zpl_mat4 *m, zpl_vec4 in) { 
    zpl_float44_mul_vec4(out, (zpl_f32(*)[4])m, in); 
}


zpl_float2 *zpl_float22_m(zpl_mat2 *m) { return (zpl_float2 *)m; }


zpl_float4 *zpl_float44_m(zpl_mat4 *m) { return (zpl_float4 *)m; }