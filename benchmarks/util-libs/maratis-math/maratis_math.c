/*======================================================================
 Maratis Tiny C Library
 version 1.0
------------------------------------------------------------------------
 Copyright (c) 2015 Anael Seghezzi <www.maratis3d.org>

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would
	be appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not
	be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source
	distribution.

========================================================================*/
/*
   Math with C/OpenCL portability:
   - vector manipulation
   - interpolation (cubic, catmullrom)
   - quaternion basics
   - matrix (projection, transformation...)
   - random number generator
   - 2d routines
   - 3d routines:
	  - voxeliser (tri-box overlap)
	  - raytracing (sphere, plane, box, triangle)

   to create the implementation,
   #define M_MATH_IMPLEMENTATION
   in *one* C/CPP file that includes this file.
*/

#include <math.h>
#include <stdbool.h>


#define M_QUAT_IDENTITY() {0.0f, 0.0f, 0.0f, 0.99999999f}

#define M_MAT4_IDENTITY() {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}


struct float2
{
	float x, y;
};
typedef struct float2 float2;

struct float3
{
	float x, y, z;
};
typedef struct float3 float3;

struct float4
{
	float x, y, z, w;
};
typedef struct float4 float4;



float M_ABS(float a)
{
	return (a < 0.0f) ? -a : a;
}



float M_CLAMP(float x, float low, float high)
{
	if (x > high)
		return high;
	if (x < low)
		return low;
	return x;
}



bool M_ISPOWEROFTWO(int x)
{
	return ((x & (x - 1)) == 0) && (x != 0);
}



float M_DOT2(float2 A, float2 B)
{
	return A.x * B.x + A.y * B.y;
}



float M_DOT3(float3 A, float3 B)
{
	return A.x * B.x + A.y * B.y + A.z * B.z;
}



float M_DOT4(float4 A, float4 B)
{
	return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
}


// 
// float M_LENGTH2(float2 src)
// {
// 	return sqrtf(src.x * src.x + src.y * src.y);
// }


// 
// float M_LENGTH3(float3 src)
// {
// 	return sqrtf(src.x * src.x + src.y * src.y + src.z * src.z);
// }


// 
// float M_LENGTH4(float4 src)
// {
// 	return sqrtf(src.x * src.x + src.y * src.y + src.z * src.z + src.w * src.w);
// }


// 
// float2 M_NORMALIZE2(float2 src)
// {
// 	float2 dest;
// 	float l = M_LENGTH2(src);
// 	if (l > 0.0f)
// 	{
// 		float inv = 1.0f / l;
// 		dest.x = src.x * inv;
// 		dest.y = src.y * inv;
// 	}
// 	else
// 	{
// 		dest.x = dest.y = 0.0f;
// 	}
// 	return dest;
// }


// 
// float3 M_NORMALIZE3(float3 src)
// {
// 	float3 dest;
// 	float l = M_LENGTH3(src);
// 	if (l > 0.0f)
// 	{
// 		float inv = 1.0f / l;
// 		dest.x = src.x * inv;
// 		dest.y = src.y * inv;
// 		dest.z = src.z * inv;
// 	}
// 	else
// 	{
// 		dest.x = dest.y = dest.z = 0.0f;
// 	}
// 	return dest;
// }


// 
// float4 M_NORMALIZE4(float4 src)
// {
// 	float4 dest;
// 	float l = M_LENGTH4(src);
// 	if (l > 0.0f)
// 	{
// 		float inv = 1.0f / l;
// 		dest.x = src.x * inv;
// 		dest.y = src.y * inv;
// 		dest.z = src.z * inv;
// 		dest.w = src.w * inv;
// 	}
// 	else
// 	{
// 		dest.x = dest.y = dest.z = dest.w = 0.0f;
// 	}
// 	return dest;
// }



float M_CROSS2(float2 A, float2 B)
{
	return A.x * B.y - A.y * B.x;
}



float3 M_CROSS3(float3 A, float3 B)
{
	float3 dest;
	dest.x = A.y * B.z - A.z * B.y;
	dest.y = A.z * B.x - A.x * B.z;
	dest.z = A.x * B.y - A.y * B.x;
	return dest;
}



float2 M_ADD2(float2 A, float2 B)
{
	float2 dest;
	dest.x = A.x + B.x;
	dest.y = A.y + B.y;
	return dest;
}



float3 M_ADD3(float3 A, float3 B)
{
	float3 dest;
	dest.x = A.x + B.x;
	dest.y = A.y + B.y;
	dest.z = A.z + B.z;
	return dest;
}



float4 M_ADD4(float4 A, float4 B)
{
	float4 dest;
	dest.x = A.x + B.x;
	dest.y = A.y + B.y;
	dest.z = A.z + B.z;
	dest.w = A.w + B.w;
	return dest;
}



float2 M_SUB2(float2 A, float2 B)
{
	float2 dest;
	dest.x = A.x - B.x;
	dest.y = A.y - B.y;
	return dest;
}



float3 M_SUB3(float3 A, float3 B)
{
	float3 dest;
	dest.x = A.x - B.x;
	dest.y = A.y - B.y;
	dest.z = A.z - B.z;
	return dest;
}



float4 M_SUB4(float4 A, float4 B)
{
	float4 dest;
	dest.x = A.x - B.x;
	dest.y = A.y - B.y;
	dest.z = A.z - B.z;
	dest.w = A.w - B.w;
	return dest;
}



float2 M_MUL2(float2 A, float2 B)
{
	float2 dest;
	dest.x = A.x * B.x;
	dest.y = A.y * B.y;
	return dest;
}



float3 M_MUL3(float3 A, float3 B)
{
	float3 dest;
	dest.x = A.x * B.x;
	dest.y = A.y * B.y;
	dest.z = A.z * B.z;
	return dest;
}



float4 M_MUL4(float4 A, float4 B)
{
	float4 dest;
	dest.x = A.x * B.x;
	dest.y = A.y * B.y;
	dest.z = A.z * B.z;
	dest.w = A.w * B.w;
	return dest;
}



float2 M_DIV2(float2 A, float2 B)
{
	float2 dest;
	dest.x = A.x / B.x;
	dest.y = A.y / B.y;
	return dest;
}



float3 M_DIV3(float3 A, float3 B)
{
	float3 dest;
	dest.x = A.x / B.x;
	dest.y = A.y / B.y;
	dest.z = A.z / B.z;
	return dest;
}



float4 M_DIV4(float4 A, float4 B)
{
	float4 dest;
	dest.x = A.x / B.x;
	dest.y = A.y / B.y;
	dest.z = A.z / B.z;
	dest.w = A.w / B.w;
	return dest;
}



float M_MIN(float a, float b) { return a < b ? a : b; }



float M_MAX(float a, float b) { return a > b ? a : b; }



float2 M_MIN2(float2 A, float2 B)
{
	float2 dest = {M_MIN(A.x, B.x), M_MIN(A.y, B.y)};
	return dest;
}



float3 M_MIN3(float3 A, float3 B)
{
	float3 dest = {M_MIN(A.x, B.x), M_MIN(A.y, B.y), M_MIN(A.z, B.z)};
	return dest;
}



float4 M_MIN4(float4 A, float4 B)
{
	float4 dest = {M_MIN(A.x, B.x), M_MIN(A.y, B.y), M_MIN(A.z, B.z), M_MIN(A.w, B.w)};
	return dest;
}



float2 M_MAX2(float2 A, float2 B)
{
	float2 dest = {M_MAX(A.x, B.x), M_MAX(A.y, B.y)};
	return dest;
}



float3 M_MAX3(float3 A, float3 B)
{
	float3 dest = {M_MAX(A.x, B.x), M_MAX(A.y, B.y), M_MAX(A.z, B.z)};
	return dest;
}



float4 M_MAX4(float4 A, float4 B)
{
	float4 dest = {M_MAX(A.x, B.x), M_MAX(A.y, B.y), M_MAX(A.z, B.z), M_MAX(A.w, B.w)};
	return dest;
}



float m_interpolation_cubic(float y0, float y1, float y2, float y3, float mu)
{
	float a0, a1, a2, a3, mu2;
	mu2 = mu * mu;
	a0 = y3 - y2 - y0 + y1;
	a1 = y0 - y1 - a0;
	a2 = y2 - y0;
	a3 = y1;
	return (a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
}


// 
// float m_interpolation_catmullrom(float y0, float y1, float y2, float y3, float mu)
// {
// 	float a0, a1, a2, a3, mu2;
// 	mu2 = mu * mu;
// 	a0 = -0.5f * y0 + 1.5f * y1 - 1.5f * y2 + 0.5f * y3;
// 	a1 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
// 	a2 = -0.5f * y0 + 0.5f * y2;
// 	a3 = y1;
// 	return (a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
// }



void m_quat_identity(float4 *dest)
{
	// float4 ident = {0.0f, 0.0f, 0.0f, 0.99999999f};
	dest->x = 0.0f;
	dest->y = 0.0f;
	dest->z = 0.0f;
	dest->w = 0.99999999f;
}


// void m_quat_normalize(float4 *dest, const float4 *src)
// {
// 	float l = M_LENGHT4(*src);
// 	if (l > 0.00000001f)
// 	{
// 		float m = 1.0f / l;
// 		dest->x = src->x * m;
// 		dest->y = src->y * m;
// 		dest->z = src->z * m;
// 		dest->w = src->w * m;
// 	}
// 	else
// 	{
// 		m_quat_identity(dest);
// 	}
// }


// 
// void m_quat_mul(float4 *dest, const float4 *A, const float4 *B)
// {
// 	dest->x = (B->w * A->x) + (B->x * A->w) + (B->y * A->z) - (B->z * A->y);
// 	dest->y = (B->w * A->y) + (B->y * A->w) + (B->z * A->x) - (B->x * A->z);
// 	dest->z = (B->w * A->z) + (B->z * A->w) + (B->x * A->y) - (B->y * A->x);
// 	dest->w = (B->w * A->w) - (B->x * A->x) - (B->y * A->y) - (B->z * A->z);
// }



void m_mat4_identity(float *dest)
{
	
	for (int i = 0; i < 16; i++)
		if(i/4 == i%4)
			dest[i] = 1.0f;
		else
			dest[i] = 0.0f;
}



void m_mat4_ortho(float *dest, float left, float right, float bottom, float top, float znear, float zfar)
{
	if (right == left || top == bottom || zfar == znear) // invalid
		return;

	dest[0] = 2.0f / (right - left);
	dest[1] = 0.0f;
	dest[2] = 0.0f;
	dest[3] = 0.0f;

	dest[4] = 0.0f;
	dest[5] = 2.0f / (top - bottom);
	dest[6] = 0.0f;
	dest[7] = 0.0f;

	dest[8] = 0.0f;
	dest[9] = 0.0f;
	dest[10] = -2.0f / (zfar - znear);
	dest[11] = 0.0f;

	dest[12] = -(right + left) / (right - left);
	dest[13] = -(top + bottom) / (top - bottom);
	dest[14] = -(zfar + znear) / (zfar - znear);
	dest[15] = 1.0f;
}



void m_mat4_translation(float *dest, const float3 *translation)
{
	dest[12] = translation->x;
	dest[13] = translation->y;
	dest[14] = translation->z;
}



void m_mat4_scale(float *dest, const float3 *scale)
{
	dest[0] = scale->x;
	dest[5] = scale->y;
	dest[10] = scale->z;
}


// 
// void m_mat4_mul(float *dest, const float *A, const float *B)
// {
// 	dest[0] = A[0] * B[0] + A[4] * B[1] + A[8] * B[2] + A[12] * B[3];
// 	dest[1] = A[1] * B[0] + A[5] * B[1] + A[9] * B[2] + A[13] * B[3];
// 	dest[2] = A[2] * B[0] + A[6] * B[1] + A[10] * B[2] + A[14] * B[3];
// 	dest[3] = A[3] * B[0] + A[7] * B[1] + A[11] * B[2] + A[15] * B[3];
// 	dest[4] = A[0] * B[4] + A[4] * B[5] + A[8] * B[6] + A[12] * B[7];
// 	dest[5] = A[1] * B[4] + A[5] * B[5] + A[9] * B[6] + A[13] * B[7];
// 	dest[6] = A[2] * B[4] + A[6] * B[5] + A[10] * B[6] + A[14] * B[7];
// 	dest[7] = A[3] * B[4] + A[7] * B[5] + A[11] * B[6] + A[15] * B[7];
// 	dest[8] = A[0] * B[8] + A[4] * B[9] + A[8] * B[10] + A[12] * B[11];
// 	dest[9] = A[1] * B[8] + A[5] * B[9] + A[9] * B[10] + A[13] * B[11];
// 	dest[10] = A[2] * B[8] + A[6] * B[9] + A[10] * B[10] + A[14] * B[11];
// 	dest[11] = A[3] * B[8] + A[7] * B[9] + A[11] * B[10] + A[15] * B[11];
// 	dest[12] = A[0] * B[12] + A[4] * B[13] + A[8] * B[14] + A[12] * B[15];
// 	dest[13] = A[1] * B[12] + A[5] * B[13] + A[9] * B[14] + A[13] * B[15];
// 	dest[14] = A[2] * B[12] + A[6] * B[13] + A[10] * B[14] + A[14] * B[15];
// 	dest[15] = A[3] * B[12] + A[7] * B[13] + A[11] * B[14] + A[15] * B[15];
// }



void m_mat4_transpose(float *dest, const float *src)
{
	dest[0] = src[0];
	dest[1] = src[4];
	dest[2] = src[8];
	dest[3] = src[12];
	dest[4] = src[1];
	dest[5] = src[5];
	dest[6] = src[9];
	dest[7] = src[13];
	dest[8] = src[2];
	dest[9] = src[6];
	dest[10] = src[10];
	dest[11] = src[14];
	dest[12] = src[3];
	dest[13] = src[7];
	dest[14] = src[11];
	dest[15] = src[15];
}



void m_mat4_rotate3(float3 *dest, const float *matrix, const float3 *src)
{
	dest->x = matrix[0] * src->x + matrix[4] * src->y + matrix[8] * src->z;
	dest->y = matrix[1] * src->x + matrix[5] * src->y + matrix[9] * src->z;
	dest->z = matrix[2] * src->x + matrix[6] * src->y + matrix[10] * src->z;
}



void m_mat4_inverse_rotate3(float3 *dest, const float *matrix, const float3 *src)
{
	dest->x = matrix[0] * src->x + matrix[1] * src->y + matrix[2] * src->z;
	dest->y = matrix[4] * src->x + matrix[5] * src->y + matrix[6] * src->z;
	dest->z = matrix[8] * src->x + matrix[9] * src->y + matrix[10] * src->z;
}



void m_mat4_transform3(float3 *dest, const float *matrix, const float3 *src)
{
	dest->x = matrix[0] * src->x + matrix[4] * src->y + matrix[8] * src->z + matrix[12];
	dest->y = matrix[1] * src->x + matrix[5] * src->y + matrix[9] * src->z + matrix[13];
	dest->z = matrix[2] * src->x + matrix[6] * src->y + matrix[10] * src->z + matrix[14];
}


// 
// void m_mat4_transform4(float4 *dest, const float *matrix, const float4 *src)
// {
// 	dest->x = matrix[0] * src->x + matrix[4] * src->y + matrix[8] * src->z + matrix[12] * src->w;
// 	dest->y = matrix[1] * src->x + matrix[5] * src->y + matrix[9] * src->z + matrix[13] * src->w;
// 	dest->z = matrix[2] * src->x + matrix[6] * src->y + matrix[10] * src->z + matrix[14] * src->w;
// 	dest->w = matrix[3] * src->x + matrix[7] * src->y + matrix[11] * src->z + matrix[15] * src->w;
// }


// float m_2d_polygon_area(float2 *points, int count)
// {
// 	float fx, fy, a;
// 	int p;
// 	if (count < 3)
// 		return 0;
// 	fx = points[count - 1].x * points[0].y;
// 	fy = points[count - 1].y * points[0].x;

// 	for (p = 1; p < count; p++)
// 	{
// 		fx += points[p - 1].x * points[p].y;
// 		fy += points[p - 1].y * points[p].x;
// 	}
// 	a = (fx - fy) * 0.5f;
// 	return M_ABS(a);
// }


// float m_2d_polyline_length(float2 *points, int count)
// {
// 	float l = 0;
// 	int p;
// 	for (p = 1; p < count; p++)
// 	{
// 		float2 v;
// 		v = M_SUB2(points[p], points[p - 1]);
// 		l += M_LENGHT2(v);
// 	}
// 	return l;
// }



int m_2d_line_to_line_intersection(float2 *dest, float2 *p11, float2 *p12, float2 *p21, float2 *p22)
{
	float2 DP, QA, QB;
	float d, la, lb;

	DP.x = p21->x - p11->x;
	DP.y = p21->y - p11->y;
	QA.x = p12->x - p11->x;
	QA.y = p12->y - p11->y;
	QB.x = p22->x - p21->x;
	QB.y = p22->y - p21->y;

	d = QA.y * QB.x - QB.y * QA.x;
	if (d == 0)
		return 0;

	la = (QB.x * DP.y - QB.y * DP.x) / d;
	if (la < 0 || la > 1)
		return 0;

	lb = (QA.x * DP.y - QA.y * DP.x) / d;
	if (lb < 0 || lb > 1)
		return 0;

	dest->x = p11->x + la * QA.x;
	dest->y = p11->y + la * QA.y;
	return 1;
}



int m_2d_box_to_box_collision(float2 *min1, float2 *max1, float2 *min2, float2 *max2)
{
	return !(
		(min1->x > max2->x) || (max1->x < min2->x) ||
		(min1->y > max2->y) || (max1->y < min2->y));
}


// float m_3d_polyline_length(float3 *points, int count)
// {
// 	float l = 0;
// 	int p;
// 	for (p = 1; p < count; p++)
// 	{
// 		float3 v = M_ADD3(points[p], points[p - 1]);
// 		l += M_LENGHT3(v);
// 	}
// 	return l;
// }



int m_3d_box_to_box_collision(float3 *min1, float3 *max1, float3 *min2, float3 *max2)
{
	return !(
		(min1->x > max2->x) || (max1->x < min2->x) ||
		(min1->y > max2->y) || (max1->y < min2->y) ||
		(min1->z > max2->z) || (max1->z < min2->z));
}



int m_3d_ray_box_intersection_in_out(float3 *ray_origin, float3 *ray_direction, float3 *box_min, float3 *box_max, float *in, float *out)
{
	float3 idir;
	float3 tmin, tmax;
	float3 tnear, tfar;
	float tnx, tny, tfy;
	float in0, out0;

	idir.x = 1.0f / ray_direction->x;
	idir.y = 1.0f / ray_direction->y;
	idir.z = 1.0f / ray_direction->z;
	tmin.x = (box_min->x - ray_origin->x) * idir.x;
	tmin.y = (box_min->y - ray_origin->y) * idir.y;
	tmin.z = (box_min->z - ray_origin->z) * idir.z;
	tmax.x = (box_max->x - ray_origin->x) * idir.x;
	tmax.y = (box_max->y - ray_origin->y) * idir.y;
	tmax.z = (box_max->z - ray_origin->z) * idir.z;

	tnear = M_MIN3(tmin, tmax);
	tfar = M_MAX3(tmin, tmax);

	tnx = M_MAX(tnear.x, 0.0f);
	tny = M_MAX(tnear.y, tnear.z);
	tfy = M_MIN(tfar.y, tfar.z);

	in0 = M_MAX(tnx, tny);
	out0 = M_MIN(tfar.x, tfy);

	if (out0 > 0.0f && in0 < out0)
	{
		*in = in0;
		*out = out0;
		return 1;
	}
	return 0;
}



int m_3d_ray_sphere_intersection_in_out(float3 *ray_origin, float3 *ray_direction, float3 *sphere_origin, float sphere_radius2, float *in, float *out)
{
	float3 vec;
	float b, det;

	vec = M_SUB3(*sphere_origin, *ray_origin);
	b = M_DOT3(vec, *ray_direction);
	det = b * b - M_DOT3(vec, vec) + sphere_radius2;

	if (det < 0)
		return 0;
	else
		det = sqrtf(det);
	*in = b - det;
	*out = b + det;
	return 1;
}


// 
// float m_3d_ray_plane_intersection(float3 *ray_origin, float3 *ray_direction, float3 *plane_origin, float3 *plane_normal)
// {
// 	float constant = -M_DOT3(*plane_normal, *plane_origin);
// 	float normal_dot = M_DOT3(*plane_normal, *ray_direction);
// 	float plane_dist = M_DOT3(*plane_normal, *ray_origin) + constant;
// 	return -plane_dist / normal_dot;
// }


// 
// float m_3d_ray_sphere_intersection(float3 *ray_origin, float3 *ray_direction, float3 *sphere_origin, float sphere_radius2)
// {
// 	float3 vec;
// 	float b, det;

// 	vec = M_SUB3(*sphere_origin, *ray_origin);
// 	b = M_DOT3(vec, *ray_direction);
// 	det = b * b - M_DOT3(vec, vec) + sphere_radius2;

// 	if (det < 0)
// 		return 0;
// 	else
// 		det = sqrtf(det);
// 	return b - det;
// }


/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/

// #define M_FIND_MIN_MAX_3(x0, x1, x2, min, max) \
// 	min = max = x0;                            \
// 	if (x1 < min)                              \
// 		min = x1;                              \
// 	if (x1 > max)                              \
// 		max = x1;                              \
// 	if (x2 < min)                              \
// 		min = x2;                              \
// 	if (x2 > max)                              \
// 		max = x2;


// int m__plane_box_overlap(float3 *normal, float3 *vert, float3 *maxbox)
// {
// 	float3 vmin, vmax;
// 	if (normal->x > 0.0f)
// 	{
// 		vmin.x = -maxbox->x - vert->x;
// 		vmax.x = maxbox->x - vert->x;
// 	}
// 	else
// 	{
// 		vmin.x = maxbox->x - vert->x;
// 		vmax.x = -maxbox->x - vert->x;
// 	}
// 	if (normal->y > 0.0f)
// 	{
// 		vmin.y = -maxbox->y - vert->y;
// 		vmax.y = maxbox->y - vert->y;
// 	}
// 	else
// 	{
// 		vmin.y = maxbox->y - vert->y;
// 		vmax.y = -maxbox->y - vert->y;
// 	}
// 	if (normal->z > 0.0f)
// 	{
// 		vmin.z = -maxbox->z - vert->z;
// 		vmax.z = maxbox->z - vert->z;
// 	}
// 	else
// 	{
// 		vmin.z = maxbox->z - vert->z;
// 		vmax.z = -maxbox->z - vert->z;
// 	}
// 	if (M_DOT3(*normal, vmin) > 0.0f)
// 		return 0;
// 	if (M_DOT3(*normal, vmax) >= 0.0f)
// 		return 1;
// 	return 0;
// }

// /*------------------------------------------------------------
//   X-axis tests
// ------------------------------------------------------------*/

// int M_AXISTEST_X01(float a, float b, float fa, float fb, float3 v0, float3 v1, float3 v2, const float3 *boxhalfsize)
// {
// 	float p0 = a * v0.y - b * v0.z;
// 	float p2 = a * v2.y - b * v2.z;
// 	float min, max;
// 	if (p0 < p2)
// 	{
// 		min = p0;
// 		max = p2;
// 	}
// 	else
// 	{
// 		min = p2;
// 		max = p0;
// 	}
// 	float rad = fa * boxhalfsize->y + fb * boxhalfsize->z;
// 	if (min > rad || max < -rad)
// 		return 0;
// 	return 1;
// }

// int M_AXISTEST_X2(float a, float b, float fa, float fb, float3 v0, float3 v1, float3 v2, const float3 *boxhalfsize)
// {
// 	float p0 = a * v0.y - b * v0.z;
// 	float p1 = a * v1.y - b * v1.z;
// 	float min, max;
// 	if (p0 < p1)
// 	{
// 		min = p0;
// 		max = p1;
// 	}
// 	else
// 	{
// 		min = p1;
// 		max = p0;
// 	}
// 	float rad = fa * boxhalfsize->y + fb * boxhalfsize->z;
// 	if (min > rad || max < -rad)
// 		return 0;
// 	return 1;
// }

// /*------------------------------------------------------------
//   Y-axis tests
// ------------------------------------------------------------*/

// int M_AXISTEST_Y02(float a, float b, float fa, float fb, float3 v0, float3 v1, float3 v2, const float3 *boxhalfsize)
// {
// 	float p0 = -a * v0.x + b * v0.z;
// 	float p2 = -a * v2.x + b * v2.z;
// 	float min, max;
// 	if (p0 < p2)
// 	{
// 		min = p0;
// 		max = p2;
// 	}
// 	else
// 	{
// 		min = p2;
// 		max = p0;
// 	}
// 	float rad = fa * boxhalfsize->x + fb * boxhalfsize->z;
// 	if (min > rad || max < -rad)
// 		return 0;
// 	return 1;
// }

// int M_AXISTEST_Y1(float a, float b, float fa, float fb, float3 v0, float3 v1, float3 v2, const float3 *boxhalfsize)
// {
// 	float p0 = -a * v0.x + b * v0.z;
// 	float p1 = -a * v1.x + b * v1.z;
// 	float min, max;
// 	if (p0 < p1)
// 	{
// 		min = p0;
// 		max = p1;
// 	}
// 	else
// 	{
// 		min = p1;
// 		max = p0;
// 	}
// 	float rad = fa * boxhalfsize->x + fb * boxhalfsize->z;
// 	if (min > rad || max < -rad)
// 		return 0;
// 	return 1;
// }

// /*------------------------------------------------------------
//   Z-axis tests
// ------------------------------------------------------------*/

// int M_AXISTEST_Z12(float a, float b, float fa, float fb, float3 v0, float3 v1, float3 v2, const float3 *boxhalfsize)
// {
// 	float p1 = a * v1.x - b * v1.y;
// 	float p2 = a * v2.x - b * v2.y;
// 	float min, max;
// 	if (p2 < p1)
// 	{
// 		min = p2;
// 		max = p1;
// 	}
// 	else
// 	{
// 		min = p1;
// 		max = p2;
// 	}
// 	float rad = fa * boxhalfsize->x + fb * boxhalfsize->y;
// 	if (min > rad || max < -rad)
// 		return 0;
// 	return 1;
// }

// int M_AXISTEST_Z0(float a, float b, float fa, float fb, float3 v0, float3 v1, float3 v2, const float3 *boxhalfsize)
// {
// 	float p0 = a * v0.x - b * v0.y;
// 	float p1 = a * v1.x - b * v1.y;
// 	float min, max;
// 	if (p0 < p1)
// 	{
// 		min = p0;
// 		max = p1;
// 	}
// 	else
// 	{
// 		min = p1;
// 		max = p0;
// 	}
// 	float rad = fa * boxhalfsize->x + fb * boxhalfsize->y;
// 	if (min > rad || max < -rad)
// 		return 0;
// 	return 1;
// }

// int m_3d_tri_box_overlap(float3 *boxcenter, float3 *boxhalfsize, float3 *vert1, float3 *vert2, float3 *vert3)
// {
// 	/*    use separating axis theorem to test overlap between triangle and box */
// 	/*    need to test for overlap in these directions: */
// 	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
// 	/*       we do not even need to test these) */
// 	/*    2) normal of the triangle */
// 	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
// 	/*       this gives 3x3=9 more tests */

// 	float3 v0, v1, v2;
// 	float3 normal, e0, e1, e2;
// 	float min, max, p0, p1, p2, rad, fex, fey, fez;

// 	/* This is the fastest branch on Sun */
// 	/* move everything so that the boxcenter is in (0,0,0) */
// 	v0 = M_SUB3(*vert1, *boxcenter);
// 	v1 = M_SUB3(*vert2, *boxcenter);
// 	v2 = M_SUB3(*vert3, *boxcenter);

// 	/* compute triangle edges */
// 	e0 = M_SUB3(v1, v0); /* tri edge 0 */
// 	e1 = M_SUB3(v2, v1); /* tri edge 1 */
// 	e2 = M_SUB3(v0, v2); /* tri edge 2 */

// 	/* Bullet 3:  */
// 	/*  test the 9 tests first (this was faster) */
// 	fex = M_ABS(e0.x);
// 	fey = M_ABS(e0.y);
// 	fez = M_ABS(e0.z);
// 	M_AXISTEST_X01(e0.z, e0.y, fez, fey, v0, v1, v2, boxhalfsize);
// 	M_AXISTEST_Y02(e0.z, e0.x, fez, fex, v0, v1, v2, boxhalfsize);
// 	M_AXISTEST_Z12(e0.y, e0.x, fey, fex, v0, v1, v2, boxhalfsize);

// 	fex = M_ABS(e1.x);
// 	fey = M_ABS(e1.y);
// 	fez = M_ABS(e1.z);
// 	M_AXISTEST_X01(e1.z, e1.y, fez, fey, v0, v1, v2, boxhalfsize);
// 	M_AXISTEST_Y02(e1.z, e1.x, fez, fex, v0, v1, v2, boxhalfsize);
// 	M_AXISTEST_Z0(e1.y, e1.x, fey, fex, v0, v1, v2, boxhalfsize);

// 	fex = M_ABS(e2.x);
// 	fey = M_ABS(e2.y);
// 	fez = M_ABS(e2.z);
// 	M_AXISTEST_X2(e2.z, e2.y, fez, fey, v0, v1, v2, boxhalfsize);
// 	M_AXISTEST_Y1(e2.z, e2.x, fez, fex, v0, v1, v2, boxhalfsize);
// 	M_AXISTEST_Z12(e2.y, e2.x, fey, fex, v0, v1, v2, boxhalfsize);

// 	/* Bullet 1: */
// 	/*  first test overlap in the {x,y,z}-directions */
// 	/*  find min, max of the triangle each direction, and test for overlap in */
// 	/*  that direction -- this is equivalent to testing a minimal AABB around */
// 	/*  the triangle against the AABB */

// 	/* test in X-direction */
// 	M_FIND_MIN_MAX_3(v0.x, v1.x, v2.x, min, max);
// 	if (min > boxhalfsize->x || max < -boxhalfsize->x)
// 		return 0;

// 	/* test in Y-direction */
// 	M_FIND_MIN_MAX_3(v0.y, v1.y, v2.y, min, max);
// 	if (min > boxhalfsize->y || max < -boxhalfsize->y)
// 		return 0;

// 	/* test in Z-direction */
// 	M_FIND_MIN_MAX_3(v0.z, v1.z, v2.z, min, max);
// 	if (min > boxhalfsize->z || max < -boxhalfsize->z)
// 		return 0;

// 	/* Bullet 2: */
// 	/*  test if the box intersects the plane of the triangle */
// 	/*  compute plane equation of triangle: normal*x+d=0 */
// 	normal = M_CROSS3(e0, e1);
// 	if (!m__plane_box_overlap(&normal, &v0, boxhalfsize))
// 		return 0;

// 	return 1; /* box and triangle overlaps */
// }