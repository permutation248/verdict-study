#include <errno.h>

#include <math.h>

struct float2 {
	float x ;
	float y ;
};

typedef struct float2 float2;

struct float3 {
	float x ;
	float y ;
	float z ;
};

typedef struct float3 float3;

struct float4 {
	float x ;
	float y ;
	float z ;
	float w ;
};

typedef struct float4 float4;

/*@
	requires \is_finite(a);
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result >= 0.0f;
	ensures (a >= 0.0f) ==> \result == a;
	ensures (a < 0.0f) ==> \result == -a;
*/
float M_ABS(float a)
{
	float tmp;
	if (a < 0.0f)
/*@ assert rte_1: is_nan_or_infinite: \is_finite(a); */
	tmp = - a;
	else tmp = a;
	return tmp;
}

/*@
	requires \is_finite(x);
	requires \is_finite(low);
	requires \is_finite(high);
	requires low <= high;
	assigns \nothing;
	ensures \is_finite(\result);
	ensures low <= \result <= high;
	ensures (x > high) ==> \result == high;
	ensures (x < low) ==> \result == low;
	ensures (low <= x && x <= high) ==> \result == x;
*/
float M_CLAMP(float x, float low, float high)
{
	float __retres;
	if (x > high) {
		__retres = high;
		goto return_label;
	}
	if (x < low) {
		__retres = low;
		goto return_label;
	}
	__retres = x;
	return_label: return __retres;
}

/*@
	requires x > -2147483648;
	assigns \nothing;
	ensures (x > 0 && (x & (x - 1)) != 0) ==> \result == 0;
	ensures (x > 0 && (x & (x - 1)) == 0) ==> \result == 1;
	ensures (x > 0 && (x & (x - 1)) != 0) ==> (\result == (_Bool)0);
	ensures (x > 0 && (x & (x - 1)) == 0) ==> (\result == (_Bool)1);
	ensures (x == 0) ==> (\result == (_Bool)0);
*/
_Bool M_ISPOWEROFTWO(int x)
{
	_Bool __retres;
	int tmp;
/*@ assert rte_42: signed_overflow: -2147483648 <= x - 1; */
	if ((x & (x - 1)) == 0)
	if (x != 0) tmp = 1; else tmp = 0;
	else tmp = 0;
	__retres = (_Bool)(tmp != 0);
	return __retres;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(\mul_float(A.x, B.x));
	requires \is_finite(\mul_float(A.y, B.y));
	requires \is_finite(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y)));
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result == \add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y));
*/
float M_DOT2(float2 A, float2 B)
{
	float __retres;
/*@ assert rte_32: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.x)); */
/*@ assert rte_33: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.y)); */
/*@ assert rte_34: is_nan_or_infinite: \is_finite(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y))); */
	__retres = A.x * B.x + A.y * B.y;
	return __retres;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z);
	requires \abs(A.x) <= 0x1.0p+40f && \abs(A.y) <= 0x1.0p+40f && \abs(A.z) <= 0x1.0p+40f;
	requires \abs(B.x) <= 0x1.0p+40f && \abs(B.y) <= 0x1.0p+40f && \abs(B.z) <= 0x1.0p+40f;
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result == \add_float(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y)), \mul_float(A.z, B.z));
*/
float M_DOT3(float3 A, float3 B)
{
	float __retres;
/*@ assert rte_150: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.x)); */
/*@ assert rte_151: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.y)); */
/*@ assert rte_152: is_nan_or_infinite: \is_finite(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y))); */
/*@ assert rte_153: is_nan_or_infinite: \is_finite(\mul_float(A.z, B.z)); */
/*@ assert rte_154: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y)), \mul_float(A.z, B.z))); */
	__retres = (A.x * B.x + A.y * B.y) + A.z * B.z;
	return __retres;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z) && \is_finite(A.w);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z) && \is_finite(B.w);
	requires \abs(A.x) <= 0x1.0p30f && \abs(A.y) <= 0x1.0p30f && \abs(A.z) <= 0x1.0p30f && \abs(A.w) <= 0x1.0p30f;
	requires \abs(B.x) <= 0x1.0p30f && \abs(B.y) <= 0x1.0p30f && \abs(B.z) <= 0x1.0p30f && \abs(B.w) <= 0x1.0p30f;
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result == ((A.x * B.x + A.y * B.y) + A.z * B.z) + A.w * B.w;
*/
float M_DOT4(float4 A, float4 B)
{
	float __retres;
/*@ assert rte_35: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.x)); */
/*@ assert rte_36: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.y)); */
/*@ assert rte_37: is_nan_or_infinite: \is_finite(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y))); */
/*@ assert rte_38: is_nan_or_infinite: \is_finite(\mul_float(A.z, B.z)); */
/*@ assert rte_39: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y)), \mul_float(A.z, B.z))); */
/*@ assert rte_40: is_nan_or_infinite: \is_finite(\mul_float(A.w, B.w)); */
/*@ assert rte_41: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(A.x, B.x), \mul_float(A.y, B.y)), \mul_float(A.z, B.z)), \mul_float(A.w, B.w))); */
	__retres = ((A.x * B.x + A.y * B.y) + A.z * B.z) + A.w * B.w;
	return __retres;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(\mul_float(A.x, B.y));
	requires \is_finite(\mul_float(A.y, B.x));
	requires \is_finite(\sub_float(\mul_float(A.x, B.y), \mul_float(A.y, B.x)));
	assigns \nothing;
	ensures \is_finite(\result);
	ensures \result == \sub_float(\mul_float(A.x, B.y), \mul_float(A.y, B.x));
*/
float M_CROSS2(float2 A, float2 B)
{
	float __retres;
/*@ assert rte_11: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.y)); */
/*@ assert rte_12: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.x)); */
/*@ assert rte_13: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(A.x, B.y), \mul_float(A.y, B.x))); */
	__retres = A.x * B.y - A.y * B.x;
	return __retres;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z);
	requires \is_finite(\mul_float(A.y, B.z));
	requires \is_finite(\mul_float(A.z, B.y));
	requires \is_finite(\sub_float(\mul_float(A.y, B.z), \mul_float(A.z, B.y)));
	requires \is_finite(\mul_float(A.z, B.x));
	requires \is_finite(\mul_float(A.x, B.z));
	requires \is_finite(\sub_float(\mul_float(A.z, B.x), \mul_float(A.x, B.z)));
	requires \is_finite(\mul_float(A.x, B.y));
	requires \is_finite(\mul_float(A.y, B.x));
	requires \is_finite(\sub_float(\mul_float(A.x, B.y), \mul_float(A.y, B.x)));
	assigns \nothing;
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \is_finite(\result.z);
	ensures \result.x == \sub_float(\mul_float(A.y, B.z), \mul_float(A.z, B.y));
	ensures \result.y == \sub_float(\mul_float(A.z, B.x), \mul_float(A.x, B.z));
	ensures \result.z == \sub_float(\mul_float(A.x, B.y), \mul_float(A.y, B.x));
*/
float3 M_CROSS3(float3 A, float3 B)
{
	float3 dest;
/*@ assert rte_14: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.z)); */
/*@ assert rte_15: is_nan_or_infinite: \is_finite(\mul_float(A.z, B.y)); */
/*@ assert rte_16: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(A.y, B.z), \mul_float(A.z, B.y))); */
	dest.x = A.y * B.z - A.z * B.y;
/*@ assert rte_17: is_nan_or_infinite: \is_finite(\mul_float(A.z, B.x)); */
/*@ assert rte_18: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.z)); */
/*@ assert rte_19: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(A.z, B.x), \mul_float(A.x, B.z))); */
	dest.y = A.z * B.x - A.x * B.z;
/*@ assert rte_20: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.y)); */
/*@ assert rte_21: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.x)); */
/*@ assert rte_22: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(A.x, B.y), \mul_float(A.y, B.x))); */
	dest.z = A.x * B.y - A.y * B.x;
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(\add_float(A.x, B.x));
	requires \is_finite(\add_float(A.y, B.y));
	assigns \nothing;
	ensures \result.x == \add_float(A.x, B.x);
	ensures \result.y == \add_float(A.y, B.y);
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
*/
float2 M_ADD2(float2 A, float2 B)
{
	float2 dest;
/*@ assert rte_2: is_nan_or_infinite: \is_finite(\add_float(A.x, B.x)); */
	dest.x = A.x + B.x;
/*@ assert rte_3: is_nan_or_infinite: \is_finite(\add_float(A.y, B.y)); */
	dest.y = A.y + B.y;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z);
	requires \is_finite(\add_float(A.x, B.x));
	requires \is_finite(\add_float(A.y, B.y));
	requires \is_finite(\add_float(A.z, B.z));
	assigns \nothing;
	ensures \is_finite(\result.x) && \is_finite(\result.y) && \is_finite(\result.z);
	ensures \result.x == A.x + B.x;
	ensures \result.y == A.y + B.y;
	ensures \result.z == A.z + B.z;
*/
float3 M_ADD3(float3 A, float3 B)
{
	float3 dest;
/*@ assert rte_4: is_nan_or_infinite: \is_finite(\add_float(A.x, B.x)); */
	dest.x = A.x + B.x;
/*@ assert rte_5: is_nan_or_infinite: \is_finite(\add_float(A.y, B.y)); */
	dest.y = A.y + B.y;
/*@ assert rte_6: is_nan_or_infinite: \is_finite(\add_float(A.z, B.z)); */
	dest.z = A.z + B.z;
	return dest;
}

/*@
	requires \is_finite(\add_float(A.w, B.w));
	requires \is_finite(\add_float(A.z, B.z));
	requires \is_finite(\add_float(A.y, B.y));
	requires \is_finite(\add_float(A.x, B.x));
	requires \is_finite(A.w) && \is_finite(B.w);
	requires \is_finite(A.z) && \is_finite(B.z);
	requires \is_finite(A.y) && \is_finite(B.y);
	requires \is_finite(A.x) && \is_finite(B.x);
	assigns \nothing;
	ensures \result.x == A.x + B.x;
	ensures \result.y == A.y + B.y;
	ensures \result.z == A.z + B.z;
	ensures \result.w == A.w + B.w;
*/
float4 M_ADD4(float4 A, float4 B)
{
	float4 dest;
/*@ assert rte_7: is_nan_or_infinite: \is_finite(\add_float(A.x, B.x)); */
	dest.x = A.x + B.x;
/*@ assert rte_8: is_nan_or_infinite: \is_finite(\add_float(A.y, B.y)); */
	dest.y = A.y + B.y;
/*@ assert rte_9: is_nan_or_infinite: \is_finite(\add_float(A.z, B.z)); */
	dest.z = A.z + B.z;
/*@ assert rte_10: is_nan_or_infinite: \is_finite(\add_float(A.w, B.w)); */
	dest.w = A.w + B.w;
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(\sub_float(A.x, B.x));
	requires \is_finite(\sub_float(A.y, B.y));
	assigns \nothing;
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \result.x == A.x - B.x;
	ensures \result.y == A.y - B.y;
*/
float2 M_SUB2(float2 A, float2 B)
{
	float2 dest;
/*@ assert rte_52: is_nan_or_infinite: \is_finite(\sub_float(A.x, B.x)); */
	dest.x = A.x - B.x;
/*@ assert rte_53: is_nan_or_infinite: \is_finite(\sub_float(A.y, B.y)); */
	dest.y = A.y - B.y;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z);
	requires \is_finite(\sub_float(A.x, B.x));
	requires \is_finite(\sub_float(A.y, B.y));
	requires \is_finite(\sub_float(A.z, B.z));
	assigns \nothing;
	ensures \result.x == \sub_float(A.x, B.x);
	ensures \result.y == \sub_float(A.y, B.y);
	ensures \result.z == \sub_float(A.z, B.z);
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \is_finite(\result.z);
*/
float3 M_SUB3(float3 A, float3 B)
{
	float3 dest;
/*@ assert rte_147: is_nan_or_infinite: \is_finite(\sub_float(A.x, B.x)); */
	dest.x = A.x - B.x;
/*@ assert rte_148: is_nan_or_infinite: \is_finite(\sub_float(A.y, B.y)); */
	dest.y = A.y - B.y;
/*@ assert rte_149: is_nan_or_infinite: \is_finite(\sub_float(A.z, B.z)); */
	dest.z = A.z - B.z;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z) && \is_finite(A.w);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z) && \is_finite(B.w);
	requires \is_finite(\sub_float(A.x, B.x));
	requires \is_finite(\sub_float(A.y, B.y));
	requires \is_finite(\sub_float(A.z, B.z));
	requires \is_finite(\sub_float(A.w, B.w));
	assigns \nothing;
	ensures \result.x == \sub_float(A.x, B.x);
	ensures \result.y == \sub_float(A.y, B.y);
	ensures \result.z == \sub_float(A.z, B.z);
	ensures \result.w == \sub_float(A.w, B.w);
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \is_finite(\result.z);
	ensures \is_finite(\result.w);
*/
float4 M_SUB4(float4 A, float4 B)
{
	float4 dest;
/*@ assert rte_54: is_nan_or_infinite: \is_finite(\sub_float(A.x, B.x)); */
	dest.x = A.x - B.x;
/*@ assert rte_55: is_nan_or_infinite: \is_finite(\sub_float(A.y, B.y)); */
	dest.y = A.y - B.y;
/*@ assert rte_56: is_nan_or_infinite: \is_finite(\sub_float(A.z, B.z)); */
	dest.z = A.z - B.z;
/*@ assert rte_57: is_nan_or_infinite: \is_finite(\sub_float(A.w, B.w)); */
	dest.w = A.w - B.w;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y);
	requires \is_finite(B.x) && \is_finite(B.y);
	requires \is_finite(\mul_float(A.x, B.x));
	requires \is_finite(\mul_float(A.y, B.y));
	assigns \nothing;
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \result.x == \mul_float(A.x, B.x);
	ensures \result.y == \mul_float(A.y, B.y);
*/
float2 M_MUL2(float2 A, float2 B)
{
	float2 dest;
/*@ assert rte_43: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.x)); */
	dest.x = A.x * B.x;
/*@ assert rte_44: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.y)); */
	dest.y = A.y * B.y;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z);
	requires \is_finite(\mul_float(A.x, B.x));
	requires \is_finite(\mul_float(A.y, B.y));
	requires \is_finite(\mul_float(A.z, B.z));
	assigns \nothing;
	ensures \is_finite(\result.x) && \result.x == A.x * B.x;
	ensures \is_finite(\result.y) && \result.y == A.y * B.y;
	ensures \is_finite(\result.z) && \result.z == A.z * B.z;
*/
float3 M_MUL3(float3 A, float3 B)
{
	float3 dest;
/*@ assert rte_45: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.x)); */
	dest.x = A.x * B.x;
/*@ assert rte_46: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.y)); */
	dest.y = A.y * B.y;
/*@ assert rte_47: is_nan_or_infinite: \is_finite(\mul_float(A.z, B.z)); */
	dest.z = A.z * B.z;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z) && \is_finite(A.w);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z) && \is_finite(B.w);
	requires \is_finite(\mul_float(A.x, B.x));
	requires \is_finite(\mul_float(A.y, B.y));
	requires \is_finite(\mul_float(A.z, B.z));
	requires \is_finite(\mul_float(A.w, B.w));
	assigns \nothing;
	ensures \result.x == \mul_float(A.x, B.x);
	ensures \result.y == \mul_float(A.y, B.y);
	ensures \result.z == \mul_float(A.z, B.z);
	ensures \result.w == \mul_float(A.w, B.w);
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \is_finite(\result.z);
	ensures \is_finite(\result.w);
*/
float4 M_MUL4(float4 A, float4 B)
{
	float4 dest;
/*@ assert rte_48: is_nan_or_infinite: \is_finite(\mul_float(A.x, B.x)); */
	dest.x = A.x * B.x;
/*@ assert rte_49: is_nan_or_infinite: \is_finite(\mul_float(A.y, B.y)); */
	dest.y = A.y * B.y;
/*@ assert rte_50: is_nan_or_infinite: \is_finite(\mul_float(A.z, B.z)); */
	dest.z = A.z * B.z;
/*@ assert rte_51: is_nan_or_infinite: \is_finite(\mul_float(A.w, B.w)); */
	dest.w = A.w * B.w;
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires B.x != 0.0f;
	requires B.y != 0.0f;
	requires \is_finite(\div_float(A.x, B.x));
	requires \is_finite(\div_float(A.y, B.y));
	assigns \nothing;
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \result.x == \div_float(A.x, B.x);
	ensures \result.y == \div_float(A.y, B.y);
*/
float2 M_DIV2(float2 A, float2 B)
{
	float2 dest;
/*@ assert rte_23: is_nan_or_infinite: \is_finite(\div_float(A.x, B.x)); */
	dest.x = A.x / B.x;
/*@ assert rte_24: is_nan_or_infinite: \is_finite(\div_float(A.y, B.y)); */
	dest.y = A.y / B.y;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z);
	requires B.x != 0.0f && B.y != 0.0f && B.z != 0.0f;
	requires \is_finite(\div_float(A.x, B.x));
	requires \is_finite(\div_float(A.y, B.y));
	requires \is_finite(\div_float(A.z, B.z));
	assigns \nothing;
	ensures \is_finite(\result.x) && \is_finite(\result.y) && \is_finite(\result.z);
	ensures \result.x == \div_float(A.x, B.x);
	ensures \result.y == \div_float(A.y, B.y);
	ensures \result.z == \div_float(A.z, B.z);
*/
float3 M_DIV3(float3 A, float3 B)
{
	float3 dest;
/*@ assert rte_25: is_nan_or_infinite: \is_finite(\div_float(A.x, B.x)); */
	dest.x = A.x / B.x;
/*@ assert rte_26: is_nan_or_infinite: \is_finite(\div_float(A.y, B.y)); */
	dest.y = A.y / B.y;
/*@ assert rte_27: is_nan_or_infinite: \is_finite(\div_float(A.z, B.z)); */
	dest.z = A.z / B.z;
	return dest;
}

/*@
	requires \is_finite(A.x) && \is_finite(A.y) && \is_finite(A.z) && \is_finite(A.w);
	requires \is_finite(B.x) && \is_finite(B.y) && \is_finite(B.z) && \is_finite(B.w);
	requires B.x != 0.0f && B.y != 0.0f && B.z != 0.0f && B.w != 0.0f;
	requires \is_finite(\div_float(A.x, B.x));
	requires \is_finite(\div_float(A.y, B.y));
	requires \is_finite(\div_float(A.z, B.z));
	requires \is_finite(\div_float(A.w, B.w));
	assigns \nothing;
	ensures \is_finite(\result.x);
	ensures \is_finite(\result.y);
	ensures \is_finite(\result.z);
	ensures \is_finite(\result.w);
	ensures \result.x == \div_float(A.x, B.x);
	ensures \result.y == \div_float(A.y, B.y);
	ensures \result.z == \div_float(A.z, B.z);
	ensures \result.w == \div_float(A.w, B.w);
*/
float4 M_DIV4(float4 A, float4 B)
{
	float4 dest;
/*@ assert rte_28: is_nan_or_infinite: \is_finite(\div_float(A.x, B.x)); */
	dest.x = A.x / B.x;
/*@ assert rte_29: is_nan_or_infinite: \is_finite(\div_float(A.y, B.y)); */
	dest.y = A.y / B.y;
/*@ assert rte_30: is_nan_or_infinite: \is_finite(\div_float(A.z, B.z)); */
	dest.z = A.z / B.z;
/*@ assert rte_31: is_nan_or_infinite: \is_finite(\div_float(A.w, B.w)); */
	dest.w = A.w / B.w;
	return dest;
}

/*@
	requires \is_finite(a);
	requires \is_finite(b);
	assigns \nothing;
	ensures \result == a || \result == b;
	ensures \result <= a;
	ensures \result <= b;
	ensures a < b ==> \result == a;
	ensures a >= b ==> \result == b;
*/
float M_MIN(float a, float b)
{
	float tmp;
	if (a < b) tmp = a; else tmp = b;
	return tmp;
}

/*@
	requires \is_finite(a);
	requires \is_finite(b);
	assigns \nothing;
	ensures \result == a || \result == b;
	ensures \result >= a;
	ensures \result >= b;
	ensures a > b ==> \result == a;
	ensures a <= b ==> \result == b;
*/
float M_MAX(float a, float b)
{
	float tmp;
	if (a > b) tmp = a; else tmp = b;
	return tmp;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	assigns \nothing;
	ensures \result.x == A.x || \result.x == B.x;
	ensures \result.y == A.y || \result.y == B.y;
	ensures \result.x <= A.x;
	ensures \result.x <= B.x;
	ensures \result.y <= A.y;
	ensures \result.y <= B.y;
	ensures A.x < B.x ==> \result.x == A.x;
	ensures A.x >= B.x ==> \result.x == B.x;
	ensures A.y < B.y ==> \result.y == A.y;
	ensures A.y >= B.y ==> \result.y == B.y;
*/
float2 M_MIN2(float2 A, float2 B)
{
	float tmp;
	float tmp_0;
	tmp = M_MIN(A.x,B.x);
	tmp_0 = M_MIN(A.y,B.y);
float2 dest = {.x = tmp, .y = tmp_0};
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(A.z);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(B.z);
	assigns \nothing;
	ensures \result.x == A.x || \result.x == B.x;
	ensures \result.y == A.y || \result.y == B.y;
	ensures \result.z == A.z || \result.z == B.z;
	ensures \result.x <= A.x;
	ensures \result.x <= B.x;
	ensures \result.y <= A.y;
	ensures \result.y <= B.y;
	ensures \result.z <= A.z;
	ensures \result.z <= B.z;
	ensures A.x < B.x ==> \result.x == A.x;
	ensures A.x >= B.x ==> \result.x == B.x;
	ensures A.y < B.y ==> \result.y == A.y;
	ensures A.y >= B.y ==> \result.y == B.y;
	ensures A.z < B.z ==> \result.z == A.z;
	ensures A.z >= B.z ==> \result.z == B.z;
*/
float3 M_MIN3(float3 A, float3 B)
{
	float tmp;
	float tmp_0;
	float tmp_1;
	tmp = M_MIN(A.x,B.x);
	tmp_0 = M_MIN(A.y,B.y);
	tmp_1 = M_MIN(A.z,B.z);
float3 dest = {.x = tmp, .y = tmp_0, .z = tmp_1};
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(A.z);
	requires \is_finite(A.w);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(B.z);
	requires \is_finite(B.w);
	assigns \nothing;
	ensures \result.x == A.x || \result.x == B.x;
	ensures \result.y == A.y || \result.y == B.y;
	ensures \result.z == A.z || \result.z == B.z;
	ensures \result.w == A.w || \result.w == B.w;
	ensures \result.x <= A.x;
	ensures \result.x <= B.x;
	ensures \result.y <= A.y;
	ensures \result.y <= B.y;
	ensures \result.z <= A.z;
	ensures \result.z <= B.z;
	ensures \result.w <= A.w;
	ensures \result.w <= B.w;
	ensures A.x < B.x ==> \result.x == A.x;
	ensures A.x >= B.x ==> \result.x == B.x;
	ensures A.y < B.y ==> \result.y == A.y;
	ensures A.y >= B.y ==> \result.y == B.y;
	ensures A.z < B.z ==> \result.z == A.z;
	ensures A.z >= B.z ==> \result.z == B.z;
	ensures A.w < B.w ==> \result.w == A.w;
	ensures A.w >= B.w ==> \result.w == B.w;
*/
float4 M_MIN4(float4 A, float4 B)
{
	float tmp;
	float tmp_0;
	float tmp_1;
	float tmp_2;
	tmp = M_MIN(A.x,B.x);
	tmp_0 = M_MIN(A.y,B.y);
	tmp_1 = M_MIN(A.z,B.z);
	tmp_2 = M_MIN(A.w,B.w);
float4 dest = {.x = tmp, .y = tmp_0, .z = tmp_1, .w = tmp_2};
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	assigns \nothing;
	ensures \result.x == A.x || \result.x == B.x;
	ensures \result.y == A.y || \result.y == B.y;
	ensures \result.x >= A.x;
	ensures \result.x >= B.x;
	ensures \result.y >= A.y;
	ensures \result.y >= B.y;
	ensures A.x > B.x ==> \result.x == A.x;
	ensures A.x <= B.x ==> \result.x == B.x;
	ensures A.y > B.y ==> \result.y == A.y;
	ensures A.y <= B.y ==> \result.y == B.y;
*/
float2 M_MAX2(float2 A, float2 B)
{
	float tmp;
	float tmp_0;
	tmp = M_MAX(A.x,B.x);
	tmp_0 = M_MAX(A.y,B.y);
float2 dest = {.x = tmp, .y = tmp_0};
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(A.z);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(B.z);
	assigns \nothing;
	ensures \result.x == A.x || \result.x == B.x;
	ensures \result.y == A.y || \result.y == B.y;
	ensures \result.z == A.z || \result.z == B.z;
	ensures \result.x >= A.x;
	ensures \result.x >= B.x;
	ensures \result.y >= A.y;
	ensures \result.y >= B.y;
	ensures \result.z >= A.z;
	ensures \result.z >= B.z;
	ensures A.x > B.x ==> \result.x == A.x;
	ensures A.x <= B.x ==> \result.x == B.x;
	ensures A.y > B.y ==> \result.y == A.y;
	ensures A.y <= B.y ==> \result.y == B.y;
	ensures A.z > B.z ==> \result.z == A.z;
	ensures A.z <= B.z ==> \result.z == B.z;
*/
float3 M_MAX3(float3 A, float3 B)
{
	float tmp;
	float tmp_0;
	float tmp_1;
	tmp = M_MAX(A.x,B.x);
	tmp_0 = M_MAX(A.y,B.y);
	tmp_1 = M_MAX(A.z,B.z);
float3 dest = {.x = tmp, .y = tmp_0, .z = tmp_1};
	return dest;
}

/*@
	requires \is_finite(A.x);
	requires \is_finite(A.y);
	requires \is_finite(A.z);
	requires \is_finite(A.w);
	requires \is_finite(B.x);
	requires \is_finite(B.y);
	requires \is_finite(B.z);
	requires \is_finite(B.w);
	assigns \nothing;
	ensures \result.x == A.x || \result.x == B.x;
	ensures \result.y == A.y || \result.y == B.y;
	ensures \result.z == A.z || \result.z == B.z;
	ensures \result.w == A.w || \result.w == B.w;
	ensures \result.x >= A.x;
	ensures \result.x >= B.x;
	ensures \result.y >= A.y;
	ensures \result.y >= B.y;
	ensures \result.z >= A.z;
	ensures \result.z >= B.z;
	ensures \result.w >= A.w;
	ensures \result.w >= B.w;
	ensures A.x > B.x ==> \result.x == A.x;
	ensures A.x <= B.x ==> \result.x == B.x;
	ensures A.y > B.y ==> \result.y == A.y;
	ensures A.y <= B.y ==> \result.y == B.y;
	ensures A.z > B.z ==> \result.z == A.z;
	ensures A.z <= B.z ==> \result.z == B.z;
	ensures A.w > B.w ==> \result.w == A.w;
	ensures A.w <= B.w ==> \result.w == B.w;
*/
float4 M_MAX4(float4 A, float4 B)
{
	float tmp;
	float tmp_0;
	float tmp_1;
	float tmp_2;
	tmp = M_MAX(A.x,B.x);
	tmp_0 = M_MAX(A.y,B.y);
	tmp_1 = M_MAX(A.z,B.z);
	tmp_2 = M_MAX(A.w,B.w);
float4 dest = {.x = tmp, .y = tmp_0, .z = tmp_1, .w = tmp_2};
	return dest;
}

/*@
	requires \is_finite(y0);
	requires \is_finite(y1);
	requires \is_finite(y2);
	requires \is_finite(y3);
	requires \is_finite(mu);
	requires -1.0f <= mu <= 1.0f;
	requires -0x1.0p+30f <= y0 <= 0x1.0p+30f;
	requires -0x1.0p+30f <= y1 <= 0x1.0p+30f;
	requires -0x1.0p+30f <= y2 <= 0x1.0p+30f;
	requires -0x1.0p+30f <= y3 <= 0x1.0p+30f;
	assigns \nothing;
	ensures \is_finite(\result);
*/
float m_interpolation_cubic(float y0, float y1, float y2, float y3, float mu)
{
	float __retres;
	float a0;
	float a1;
	float a2;
	float a3;
	float mu2;
/*@ assert rte_165: is_nan_or_infinite: \is_finite(\mul_float(mu, mu)); */
	mu2 = mu * mu;
/*@ assert rte_166: is_nan_or_infinite: \is_finite(\sub_float(y3, y2)); */
/*@ assert rte_167: is_nan_or_infinite: \is_finite(\sub_float(\sub_float(y3, y2), y0)); */
/*@ assert rte_168: is_nan_or_infinite: \is_finite(\add_float(\sub_float(\sub_float(y3, y2), y0), y1)); */
	a0 = ((y3 - y2) - y0) + y1;
/*@ assert rte_169: is_nan_or_infinite: \is_finite(\sub_float(y0, y1)); */
/*@ assert rte_170: is_nan_or_infinite: \is_finite(\sub_float(\sub_float(y0, y1), a0)); */
	a1 = (y0 - y1) - a0;
/*@ assert rte_171: is_nan_or_infinite: \is_finite(\sub_float(y2, y0)); */
	a2 = y2 - y0;
	a3 = y1;
/*@ assert rte_172: is_nan_or_infinite: \is_finite(\mul_float(a0, mu)); */
/*@ assert rte_173: is_nan_or_infinite: \is_finite(\mul_float(\mul_float(a0, mu), mu2)); */
/*@ assert rte_174: is_nan_or_infinite: \is_finite(\mul_float(a1, mu2)); */
/*@ assert rte_175: is_nan_or_infinite: \is_finite(\add_float(\mul_float(\mul_float(a0, mu), mu2), \mul_float(a1, mu2))); */
/*@ assert rte_176: is_nan_or_infinite: \is_finite(\mul_float(a2, mu)); */
/*@ assert rte_177: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(\mul_float(a0, mu), mu2), \mul_float(a1, mu2)), \mul_float(a2, mu))); */
/*@ assert rte_178: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(\mul_float (a0, mu), mu2), \mul_float(a1, mu2)), \mul_float(a2, mu)), a3)); */
	__retres = (((a0 * mu) * mu2 + a1 * mu2) + a2 * mu) + a3;
	return __retres;
}

/*@
	requires \valid(dest);
	assigns dest->x, dest->y, dest->z, dest->w;
	ensures dest->x == 0.0f;
	ensures dest->y == 0.0f;
	ensures dest->z == 0.0f;
	ensures dest->w == 0.99999999f;
*/
void m_quat_identity(float4 *dest)
{
/*@ assert rte_372: mem_access: \valid(&dest->x); */
	dest->x = 0.0f;
/*@ assert rte_373: mem_access: \valid(&dest->y); */
	dest->y = 0.0f;
/*@ assert rte_374: mem_access: \valid(&dest->z); */
	dest->z = 0.0f;
/*@ assert rte_375: mem_access: \valid(&dest->w); */
	dest->w = 0.99999999f;
	return;
}

/*@
	requires \valid(dest + (0..15));
	assigns dest[0..15];
*/
void m_mat4_identity(float *dest)
{
	int i = 0;
	/*@
		loop invariant 0 <= i <= 16;
		loop assigns i, dest[0..15];
		loop variant 16 - i;
	*/
	while (i < 16) {
		if (i / 4 == i % 4)
	/*@ assert rte_179: mem_access: \valid(dest + i); */
		*(dest + i) = 1.0f;
		else
	/*@ assert rte_180: mem_access: \valid(dest + i); */
		*(dest + i) = 0.0f;
	/*@ assert rte_181: signed_overflow: i + 1 <= 2147483647; */
		i ++;
	}
	return;
}

/*@
	requires zfar != znear ==> \is_finite(\div_float(\neg_float(\add_float(zfar, znear)), \sub_float(zfar, znear)));
	requires top != bottom ==> \is_finite(\div_float(\neg_float(\add_float(top, bottom)), \sub_float(top, bottom)));
	requires right != left ==> \is_finite(\div_float(\neg_float(\add_float(right, left)), \sub_float(right, left)));
	requires zfar != znear ==> \is_finite(\div_float(\neg_float(2.0f), \sub_float(zfar, znear)));
	requires top != bottom ==> \is_finite(\div_float(2.0f, \sub_float(top, bottom)));
	requires right != left ==> \is_finite(\div_float(2.0f, \sub_float(right, left)));
	requires zfar != znear ==> \is_finite(\add_float(zfar, znear));
	requires zfar != znear ==> \is_finite(\sub_float(zfar, znear));
	requires top != bottom ==> \is_finite(\add_float(top, bottom));
	requires top != bottom ==> \is_finite(\sub_float(top, bottom));
	requires right != left ==> \is_finite(\add_float(right, left));
	requires right != left ==> \is_finite(\sub_float(right, left));
	requires \is_finite(zfar);
	requires \is_finite(znear);
	requires \is_finite(top);
	requires \is_finite(bottom);
	requires \is_finite(right);
	requires \is_finite(left);
	requires \valid(dest + (0..15));
	assigns dest[0 .. 15];
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[0] == 2.0f / (right - left));
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[1] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[2] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[3] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[4] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[5] == 2.0f / (top - bottom));
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[6] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[7] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[8] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[9] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[10] == -2.0f / (zfar - znear));
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[11] == 0.0f);
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[12] == -(right + left) / (right - left));
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[13] == -(top + bottom) / (top - bottom));
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[14] == -(zfar + znear) / (zfar - znear));
	ensures (right != left && top != bottom && zfar != znear) ==> (dest[15] == 1.0f);
	ensures right == left ==>dest[0] == \old(dest[0]) && dest[1] == \old(dest[1]) &&dest[2] == \old(dest[2]) && dest[3] == \old(dest[3]) &&dest[4] == \old(dest[4]) && dest[5] == \old(dest[5]) &&dest[6] == \old(dest[6]) && dest[7] == \old(dest[7]) &&dest[8] == \old(dest[8]) && dest[9] == \old(dest[9]) &&dest[10] == \old(dest[10]) && dest[11] == \old(dest[11]) &&dest[12] == \old(dest[12]) && dest[13] == \old(dest[13]) &&dest[14] == \old(dest[14]) && dest[15] == \old(dest[15]);
	ensures (right != left && top == bottom) ==>dest[0] == \old(dest[0]) && dest[1] == \old(dest[1]) &&dest[2] == \old(dest[2]) && dest[3] == \old(dest[3]) &&dest[4] == \old(dest[4]) && dest[5] == \old(dest[5]) &&dest[6] == \old(dest[6]) && dest[7] == \old(dest[7]) &&dest[8] == \old(dest[8]) && dest[9] == \old(dest[9]) &&dest[10] == \old(dest[10]) && dest[11] == \old(dest[11]) &&dest[12] == \old(dest[12]) && dest[13] == \old(dest[13]) &&dest[14] == \old(dest[14]) && dest[15] == \old(dest[15]);
	ensures (right != left && top != bottom && zfar == znear) ==>dest[0] == \old(dest[0]) && dest[1] == \old(dest[1]) &&dest[2] == \old(dest[2]) && dest[3] == \old(dest[3]) &&dest[4] == \old(dest[4]) && dest[5] == \old(dest[5]) &&dest[6] == \old(dest[6]) && dest[7] == \old(dest[7]) &&dest[8] == \old(dest[8]) && dest[9] == \old(dest[9]) &&dest[10] == \old(dest[10]) && dest[11] == \old(dest[11]) &&dest[12] == \old(dest[12]) && dest[13] == \old(dest[13]) &&dest[14] == \old(dest[14]) && dest[15] == \old(dest[15]);
	ensures (right != left && top != bottom && zfar != znear) ==> dest[1] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[2] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[3] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[4] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[6] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[7] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[8] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[9] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[11] == 0.0f;
	ensures (right != left && top != bottom && zfar != znear) ==> dest[15] == 1.0f;
*/
void m_mat4_ortho(float *dest, float left, float right, float bottom,
float top, float znear, float zfar)
{
	if (right == left) goto return_label;
	else
	if (top == bottom) goto return_label;
	else
	if (zfar == znear) goto return_label;
/*@ assert rte_218: mem_access: \valid(dest + 0); */
/*@ assert rte_219: is_nan_or_infinite: \is_finite(\sub_float(right, left)); */
/*@ assert rte_220: is_nan_or_infinite: \is_finite(\div_float(2.0f, \sub_float(right, left))); */
	*(dest + 0) = 2.0f / (right - left);
/*@ assert rte_221: mem_access: \valid(dest + 1); */
	*(dest + 1) = 0.0f;
/*@ assert rte_222: mem_access: \valid(dest + 2); */
	*(dest + 2) = 0.0f;
/*@ assert rte_223: mem_access: \valid(dest + 3); */
	*(dest + 3) = 0.0f;
/*@ assert rte_224: mem_access: \valid(dest + 4); */
	*(dest + 4) = 0.0f;
/*@ assert rte_225: mem_access: \valid(dest + 5); */
/*@ assert rte_226: is_nan_or_infinite: \is_finite(\sub_float(top, bottom)); */
/*@ assert rte_227: is_nan_or_infinite: \is_finite(\div_float(2.0f, \sub_float(top, bottom))); */
	*(dest + 5) = 2.0f / (top - bottom);
/*@ assert rte_228: mem_access: \valid(dest + 6); */
	*(dest + 6) = 0.0f;
/*@ assert rte_229: mem_access: \valid(dest + 7); */
	*(dest + 7) = 0.0f;
/*@ assert rte_230: mem_access: \valid(dest + 8); */
	*(dest + 8) = 0.0f;
/*@ assert rte_231: mem_access: \valid(dest + 9); */
	*(dest + 9) = 0.0f;
/*@ assert rte_232: mem_access: \valid(dest + 10); */
/*@ assert rte_233: is_nan_or_infinite: \is_finite(2.0f); */
/*@ assert rte_234: is_nan_or_infinite: \is_finite(\sub_float(zfar, znear)); */
/*@ assert rte_235: is_nan_or_infinite: \is_finite(\div_float(\neg_float(2.0f), \sub_float(zfar, znear))); */
	*(dest + 10) = - 2.0f / (zfar - znear);
/*@ assert rte_236: mem_access: \valid(dest + 11); */
	*(dest + 11) = 0.0f;
/*@ assert rte_237: mem_access: \valid(dest + 12); */
/*@ assert rte_238: is_nan_or_infinite: \is_finite(\add_float(right, left)); */
/*@ assert rte_239: is_nan_or_infinite: \is_finite(\sub_float(right, left)); */
/*@ assert rte_240: is_nan_or_infinite: \is_finite(\div_float(\neg_float(\add_float(right, left)), \sub_float(right, left))); */
	*(dest + 12) = - (right + left) / (right - left);
/*@ assert rte_241: mem_access: \valid(dest + 13); */
/*@ assert rte_242: is_nan_or_infinite: \is_finite(\add_float(top, bottom)); */
/*@ assert rte_243: is_nan_or_infinite: \is_finite(\sub_float(top, bottom)); */
/*@ assert rte_244: is_nan_or_infinite: \is_finite(\div_float(\neg_float(\add_float(top, bottom)), \sub_float(top, bottom))); */
	*(dest + 13) = - (top + bottom) / (top - bottom);
/*@ assert rte_245: mem_access: \valid(dest + 14); */
/*@ assert rte_246: is_nan_or_infinite: \is_finite(\add_float(zfar, znear)); */
/*@ assert rte_247: is_nan_or_infinite: \is_finite(\sub_float(zfar, znear)); */
/*@ assert rte_248: is_nan_or_infinite: \is_finite(\div_float(\neg_float(\add_float(zfar, znear)), \sub_float(zfar, znear))); */
	*(dest + 14) = - (zfar + znear) / (zfar - znear);
/*@ assert rte_249: mem_access: \valid(dest + 15); */
	*(dest + 15) = 1.0f;
	return_label: return;
}

/*@
	requires \valid_read(translation);
	requires \valid(dest + (12 .. 14));
	assigns dest[12], dest[13], dest[14];
	assigns *(dest + 12), *(dest + 13), *(dest + 14);
	ensures dest[14] == translation->z;
	ensures dest[12] == \old(translation->x);
	ensures *(dest + 12) == \old(translation->x);
*/
void m_mat4_translation(float *dest, float3 const *translation)
{
/*@ assert rte_334: mem_access: \valid(dest + 12); */
/*@ assert rte_335: mem_access: \valid_read(&translation->x); */
	*(dest + 12) = translation->x;
/*@ assert rte_336: mem_access: \valid(dest + 13); */
/*@ assert rte_337: mem_access: \valid_read(&translation->y); */
	*(dest + 13) = translation->y;
/*@ assert rte_338: mem_access: \valid(dest + 14); */
/*@ assert rte_339: mem_access: \valid_read(&translation->z); */
	*(dest + 14) = translation->z;
	return;
}

/*@
	requires \valid_read(scale);
	requires \valid(dest + 10);
	requires \valid(dest + 5);
	requires \valid(dest + 0);
	assigns dest[0], dest[5], dest[10];
	ensures dest[10] == scale->z;
	ensures dest[0] == \old(scale->x);
*/
void m_mat4_scale(float *dest, float3 const *scale)
{
/*@ assert rte_286: mem_access: \valid(dest + 0); */
/*@ assert rte_287: mem_access: \valid_read(&scale->x); */
	*(dest + 0) = scale->x;
/*@ assert rte_288: mem_access: \valid(dest + 5); */
/*@ assert rte_289: mem_access: \valid_read(&scale->y); */
	*(dest + 5) = scale->y;
/*@ assert rte_290: mem_access: \valid(dest + 10); */
/*@ assert rte_291: mem_access: \valid_read(&scale->z); */
	*(dest + 10) = scale->z;
	return;
}

/*@
	requires \valid(dest + (0..15));
	requires \valid_read(src + (0..15));
	requires \separated(dest + (0..15), src + (0..15));
	assigns dest[0..15];
	ensures dest[0] == \old(src[0]);
	ensures dest[1] == \old(src[4]);
	ensures dest[2] == \old(src[8]);
	ensures dest[3] == \old(src[12]);
	ensures dest[4] == \old(src[1]);
	ensures dest[5] == \old(src[5]);
	ensures dest[6] == \old(src[9]);
	ensures dest[7] == \old(src[13]);
	ensures dest[8] == \old(src[2]);
	ensures dest[9] == \old(src[6]);
	ensures dest[10] == \old(src[10]);
	ensures dest[11] == \old(src[14]);
	ensures dest[12] == \old(src[3]);
	ensures dest[13] == \old(src[7]);
	ensures dest[14] == \old(src[11]);
	ensures dest[15] == \old(src[15]);
*/
void m_mat4_transpose(float *dest, float const *src)
{
/*@ assert rte_340: mem_access: \valid(dest + 0); */
/*@ assert rte_341: mem_access: \valid_read(src + 0); */
	*(dest + 0) = *(src + 0);
/*@ assert rte_342: mem_access: \valid(dest + 1); */
/*@ assert rte_343: mem_access: \valid_read(src + 4); */
	*(dest + 1) = *(src + 4);
/*@ assert rte_344: mem_access: \valid(dest + 2); */
/*@ assert rte_345: mem_access: \valid_read(src + 8); */
	*(dest + 2) = *(src + 8);
/*@ assert rte_346: mem_access: \valid(dest + 3); */
/*@ assert rte_347: mem_access: \valid_read(src + 12); */
	*(dest + 3) = *(src + 12);
/*@ assert rte_348: mem_access: \valid(dest + 4); */
/*@ assert rte_349: mem_access: \valid_read(src + 1); */
	*(dest + 4) = *(src + 1);
/*@ assert rte_350: mem_access: \valid(dest + 5); */
/*@ assert rte_351: mem_access: \valid_read(src + 5); */
	*(dest + 5) = *(src + 5);
/*@ assert rte_352: mem_access: \valid(dest + 6); */
/*@ assert rte_353: mem_access: \valid_read(src + 9); */
	*(dest + 6) = *(src + 9);
/*@ assert rte_354: mem_access: \valid(dest + 7); */
/*@ assert rte_355: mem_access: \valid_read(src + 13); */
	*(dest + 7) = *(src + 13);
/*@ assert rte_356: mem_access: \valid(dest + 8); */
/*@ assert rte_357: mem_access: \valid_read(src + 2); */
	*(dest + 8) = *(src + 2);
/*@ assert rte_358: mem_access: \valid(dest + 9); */
/*@ assert rte_359: mem_access: \valid_read(src + 6); */
	*(dest + 9) = *(src + 6);
/*@ assert rte_360: mem_access: \valid(dest + 10); */
/*@ assert rte_361: mem_access: \valid_read(src + 10); */
	*(dest + 10) = *(src + 10);
/*@ assert rte_362: mem_access: \valid(dest + 11); */
/*@ assert rte_363: mem_access: \valid_read(src + 14); */
	*(dest + 11) = *(src + 14);
/*@ assert rte_364: mem_access: \valid(dest + 12); */
/*@ assert rte_365: mem_access: \valid_read(src + 3); */
	*(dest + 12) = *(src + 3);
/*@ assert rte_366: mem_access: \valid(dest + 13); */
/*@ assert rte_367: mem_access: \valid_read(src + 7); */
	*(dest + 13) = *(src + 7);
/*@ assert rte_368: mem_access: \valid(dest + 14); */
/*@ assert rte_369: mem_access: \valid_read(src + 11); */
	*(dest + 14) = *(src + 11);
/*@ assert rte_370: mem_access: \valid(dest + 15); */
/*@ assert rte_371: mem_access: \valid_read(src + 15); */
	*(dest + 15) = *(src + 15);
	return;
}

/*@
	requires \valid(dest);
	requires \valid_read(src);
	requires \valid_read(matrix + (0..10));
	requires \is_finite(src->x);
	requires \is_finite(src->y);
	requires \is_finite(src->z);
	requires \is_finite(matrix[0]);
	requires \is_finite(matrix[1]);
	requires \is_finite(matrix[2]);
	requires \is_finite(matrix[4]);
	requires \is_finite(matrix[5]);
	requires \is_finite(matrix[6]);
	requires \is_finite(matrix[8]);
	requires \is_finite(matrix[9]);
	requires \is_finite(matrix[10]);
	requires \abs(matrix[0]) <= 1.0f;
	requires \abs(matrix[1]) <= 1.0f;
	requires \abs(matrix[2]) <= 1.0f;
	requires \abs(matrix[4]) <= 1.0f;
	requires \abs(matrix[5]) <= 1.0f;
	requires \abs(matrix[6]) <= 1.0f;
	requires \abs(matrix[8]) <= 1.0f;
	requires \abs(matrix[9]) <= 1.0f;
	requires \abs(matrix[10]) <= 1.0f;
	requires \abs(src->x) <= 0x1.fffffep+63f;
	requires \abs(src->y) <= 0x1.fffffep+63f;
	requires \abs(src->z) <= 0x1.fffffep+63f;
	assigns dest->x, dest->y, dest->z;
	ensures dest->x == \add_float(\add_float(\mul_float(\old(*(matrix + 0)), \old(src->x)), \mul_float(\old(*(matrix + 4)), \old(src->y))), \mul_float(\old(*(matrix + 8)), \old(src->z)));
	ensures \is_finite(dest->x);
	ensures \is_finite(dest->y);
	ensures \is_finite(dest->z);
*/
void m_mat4_rotate3(float3 *dest, float const *matrix, float3 const *src)
{
/*@ assert rte_250: mem_access: \valid(&dest->x); */
/*@ assert rte_251: mem_access: \valid_read(matrix + 0); */
/*@ assert rte_252: mem_access: \valid_read(&src->x); */
/*@ assert rte_253: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 0), src->x)); */
/*@ assert rte_254: mem_access: \valid_read(matrix + 4); */
/*@ assert rte_255: mem_access: \valid_read(&src->y); */
/*@ assert rte_256: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 4), src->y)); */
/*@ assert rte_257: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 0), src->x), \mul_float(*(matrix + 4), src->y))); */
/*@ assert rte_258: mem_access: \valid_read(matrix + 8); */
/*@ assert rte_259: mem_access: \valid_read(&src->z); */
/*@ assert rte_260: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 8), src->z)); */
/*@ assert rte_261: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 0), src->x), \mul_float(*(matrix + 4), src->y)), \mul_float(*(matrix + 8), src->z))); */
	dest->x = (*(matrix + 0) * src->x + *(matrix + 4) * src->y) + *(matrix + 8) * src->z;
/*@ assert rte_262: mem_access: \valid(&dest->y); */
/*@ assert rte_263: mem_access: \valid_read(matrix + 1); */
/*@ assert rte_264: mem_access: \valid_read(&src->x); */
/*@ assert rte_265: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 1), src->x)); */
/*@ assert rte_266: mem_access: \valid_read(matrix + 5); */
/*@ assert rte_267: mem_access: \valid_read(&src->y); */
/*@ assert rte_268: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 5), src->y)); */
/*@ assert rte_269: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 1), src->x), \mul_float(*(matrix + 5), src->y))); */
/*@ assert rte_270: mem_access: \valid_read(matrix + 9); */
/*@ assert rte_271: mem_access: \valid_read(&src->z); */
/*@ assert rte_272: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 9), src->z)); */
/*@ assert rte_273: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 1), src->x), \mul_float(*(matrix + 5), src->y)), \mul_float(*(matrix + 9), src->z))); */
	dest->y = (*(matrix + 1) * src->x + *(matrix + 5) * src->y) + *(matrix + 9) * src->z;
/*@ assert rte_274: mem_access: \valid(&dest->z); */
/*@ assert rte_275: mem_access: \valid_read(matrix + 2); */
/*@ assert rte_276: mem_access: \valid_read(&src->x); */
/*@ assert rte_277: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 2), src->x)); */
/*@ assert rte_278: mem_access: \valid_read(matrix + 6); */
/*@ assert rte_279: mem_access: \valid_read(&src->y); */
/*@ assert rte_280: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 6), src->y)); */
/*@ assert rte_281: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 2), src->x), \mul_float(*(matrix + 6), src->y))); */
/*@ assert rte_282: mem_access: \valid_read(matrix + 10); */
/*@ assert rte_283: mem_access: \valid_read(&src->z); */
/*@ assert rte_284: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 10), src->z)); */
/*@ assert rte_285: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 2), src->x), \mul_float(*(matrix + 6), src->y)), \mul_float(*(matrix + 10), src->z))); */
	dest->z = (*(matrix + 2) * src->x + *(matrix + 6) * src->y) + *(matrix + 10) * src->z;
	return;
}

/*@
	requires \abs(*(matrix + 10)) <= 1.0;
	requires \abs(*(matrix + 9)) <= 1.0;
	requires \abs(*(matrix + 8)) <= 1.0;
	requires \abs(*(matrix + 6)) <= 1.0;
	requires \abs(*(matrix + 5)) <= 1.0;
	requires \abs(*(matrix + 4)) <= 1.0;
	requires \abs(*(matrix + 2)) <= 1.0;
	requires \abs(*(matrix + 1)) <= 1.0;
	requires \abs(*(matrix + 0)) <= 1.0;
	requires \abs(src->z) <= 0x1.0p60;
	requires \abs(src->y) <= 0x1.0p60;
	requires \abs(src->x) <= 0x1.0p60;
	requires \is_finite(*(matrix + 10));
	requires \is_finite(*(matrix + 9));
	requires \is_finite(*(matrix + 8));
	requires \is_finite(*(matrix + 6));
	requires \is_finite(*(matrix + 5));
	requires \is_finite(*(matrix + 4));
	requires \is_finite(*(matrix + 2));
	requires \is_finite(*(matrix + 1));
	requires \is_finite(*(matrix + 0));
	requires \is_finite(src->z);
	requires \is_finite(src->y);
	requires \is_finite(src->x);
	requires \valid_read(matrix + 10);
	requires \valid_read(matrix + 9);
	requires \valid_read(matrix + 8);
	requires \valid_read(matrix + 6);
	requires \valid_read(matrix + 5);
	requires \valid_read(matrix + 4);
	requires \valid_read(matrix + 2);
	requires \valid_read(matrix + 1);
	requires \valid_read(matrix + 0);
	requires \valid_read(src);
	requires \valid(dest);
	assigns dest->x, dest->y, dest->z;
	ensures \is_finite(dest->x) && \is_finite(dest->y) && \is_finite(dest->z);
	ensures dest->x == (\old(*(matrix + 0)) * \old(src->x) + \old(*(matrix + 1)) * \old(src->y)) + \old(*(matrix + 2)) * \old(src->z);
*/
void m_mat4_inverse_rotate3(float3 *dest, float const *matrix,
float3 const *src)
{
/*@ assert rte_182: mem_access: \valid(&dest->x); */
/*@ assert rte_183: mem_access: \valid_read(matrix + 0); */
/*@ assert rte_184: mem_access: \valid_read(&src->x); */
/*@ assert rte_185: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 0), src->x)); */
/*@ assert rte_186: mem_access: \valid_read(matrix + 1); */
/*@ assert rte_187: mem_access: \valid_read(&src->y); */
/*@ assert rte_188: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 1), src->y)); */
/*@ assert rte_189: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 0), src->x), \mul_float(*(matrix + 1), src->y))); */
/*@ assert rte_190: mem_access: \valid_read(matrix + 2); */
/*@ assert rte_191: mem_access: \valid_read(&src->z); */
/*@ assert rte_192: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 2), src->z)); */
/*@ assert rte_193: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 0), src->x), \mul_float(*(matrix + 1), src->y)), \mul_float(*(matrix + 2), src->z))); */
	dest->x = (*(matrix + 0) * src->x + *(matrix + 1) * src->y) + *(matrix + 2) * src->z;
/*@ assert rte_194: mem_access: \valid(&dest->y); */
/*@ assert rte_195: mem_access: \valid_read(matrix + 4); */
/*@ assert rte_196: mem_access: \valid_read(&src->x); */
/*@ assert rte_197: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 4), src->x)); */
/*@ assert rte_198: mem_access: \valid_read(matrix + 5); */
/*@ assert rte_199: mem_access: \valid_read(&src->y); */
/*@ assert rte_200: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 5), src->y)); */
/*@ assert rte_201: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 4), src->x), \mul_float(*(matrix + 5), src->y))); */
/*@ assert rte_202: mem_access: \valid_read(matrix + 6); */
/*@ assert rte_203: mem_access: \valid_read(&src->z); */
/*@ assert rte_204: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 6), src->z)); */
/*@ assert rte_205: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 4), src->x), \mul_float(*(matrix + 5), src->y)), \mul_float(*(matrix + 6), src->z))); */
	dest->y = (*(matrix + 4) * src->x + *(matrix + 5) * src->y) + *(matrix + 6) * src->z;
/*@ assert rte_206: mem_access: \valid(&dest->z); */
/*@ assert rte_207: mem_access: \valid_read(matrix + 8); */
/*@ assert rte_208: mem_access: \valid_read(&src->x); */
/*@ assert rte_209: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 8), src->x)); */
/*@ assert rte_210: mem_access: \valid_read(matrix + 9); */
/*@ assert rte_211: mem_access: \valid_read(&src->y); */
/*@ assert rte_212: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 9), src->y)); */
/*@ assert rte_213: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 8), src->x), \mul_float(*(matrix + 9), src->y))); */
/*@ assert rte_214: mem_access: \valid_read(matrix + 10); */
/*@ assert rte_215: mem_access: \valid_read(&src->z); */
/*@ assert rte_216: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 10), src->z)); */
/*@ assert rte_217: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 8), src->x), \mul_float(*(matrix + 9), src->y)), \mul_float(*(matrix + 10), src->z))); */
	dest->z = (*(matrix + 8) * src->x + *(matrix + 9) * src->y) + *(matrix + 10) * src->z;
	return;
}

void m_mat4_transform3(float3 *dest, float const *matrix, float3 const *src)
{
/*@ assert rte_292: mem_access: \valid(&dest->x); */
/*@ assert rte_293: mem_access: \valid_read(matrix + 0); */
/*@ assert rte_294: mem_access: \valid_read(&src->x); */
/*@ assert rte_295: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 0), src->x)); */
/*@ assert rte_296: mem_access: \valid_read(matrix + 4); */
/*@ assert rte_297: mem_access: \valid_read(&src->y); */
/*@ assert rte_298: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 4), src->y)); */
/*@ assert rte_299: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 0), src->x), \mul_float(*(matrix + 4), src->y))); */
/*@ assert rte_300: mem_access: \valid_read(matrix + 8); */
/*@ assert rte_301: mem_access: \valid_read(&src->z); */
/*@ assert rte_302: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 8), src->z)); */
/*@ assert rte_303: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 0), src->x), \mul_float(*(matrix + 4), src->y)), \mul_float(*(matrix + 8), src->z))); */
/*@ assert rte_304: mem_access: \valid_read(matrix + 12); */
/*@ assert rte_305: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(*(matrix + 0), src->x), \mul_float(*(matrix + 4), src->y)), \mul_float(*(matrix + 8), src->z)), *(matrix + 12))); */
	dest->x = ((*(matrix + 0) * src->x + *(matrix + 4) * src->y) + *(matrix + 8) * src->z) + *(
	matrix + 12);
/*@ assert rte_306: mem_access: \valid(&dest->y); */
/*@ assert rte_307: mem_access: \valid_read(matrix + 1); */
/*@ assert rte_308: mem_access: \valid_read(&src->x); */
/*@ assert rte_309: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 1), src->x)); */
/*@ assert rte_310: mem_access: \valid_read(matrix + 5); */
/*@ assert rte_311: mem_access: \valid_read(&src->y); */
/*@ assert rte_312: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 5), src->y)); */
/*@ assert rte_313: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 1), src->x), \mul_float(*(matrix + 5), src->y))); */
/*@ assert rte_314: mem_access: \valid_read(matrix + 9); */
/*@ assert rte_315: mem_access: \valid_read(&src->z); */
/*@ assert rte_316: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 9), src->z)); */
/*@ assert rte_317: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 1), src->x), \mul_float(*(matrix + 5), src->y)), \mul_float(*(matrix + 9), src->z))); */
/*@ assert rte_318: mem_access: \valid_read(matrix + 13); */
/*@ assert rte_319: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(*(matrix + 1), src->x), \mul_float(*(matrix + 5), src->y)), \mul_float(*(matrix + 9), src->z)), *(matrix + 13))); */
	dest->y = ((*(matrix + 1) * src->x + *(matrix + 5) * src->y) + *(matrix + 9) * src->z) + *(
	matrix + 13);
/*@ assert rte_320: mem_access: \valid(&dest->z); */
/*@ assert rte_321: mem_access: \valid_read(matrix + 2); */
/*@ assert rte_322: mem_access: \valid_read(&src->x); */
/*@ assert rte_323: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 2), src->x)); */
/*@ assert rte_324: mem_access: \valid_read(matrix + 6); */
/*@ assert rte_325: mem_access: \valid_read(&src->y); */
/*@ assert rte_326: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 6), src->y)); */
/*@ assert rte_327: is_nan_or_infinite: \is_finite(\add_float(\mul_float(*(matrix + 2), src->x), \mul_float(*(matrix + 6), src->y))); */
/*@ assert rte_328: mem_access: \valid_read(matrix + 10); */
/*@ assert rte_329: mem_access: \valid_read(&src->z); */
/*@ assert rte_330: is_nan_or_infinite: \is_finite(\mul_float(*(matrix + 10), src->z)); */
/*@ assert rte_331: is_nan_or_infinite: \is_finite(\add_float(\add_float(\mul_float(*(matrix + 2), src->x), \mul_float(*(matrix + 6), src->y)), \mul_float(*(matrix + 10), src->z))); */
/*@ assert rte_332: mem_access: \valid_read(matrix + 14); */
/*@ assert rte_333: is_nan_or_infinite: \is_finite(\add_float(\add_float(\add_float(\mul_float(*(matrix + 2), src->x), \mul_float(*(matrix + 6), src->y)), \mul_float(*(matrix + 10), src->z)), *(matrix + 14))); */
	dest->z = ((*(matrix + 2) * src->x + *(matrix + 6) * src->y) + *(matrix + 10) * src->z) + *(
	matrix + 14);
	return;
}

int m_2d_line_to_line_intersection(float2 *dest, float2 *p11, float2 *p12,
float2 *p21, float2 *p22)
{
	int __retres;
	float2 DP;
	float2 QA;
	float2 QB;
	float d;
	float la;
	float lb;
/*@ assert rte_66: mem_access: \valid_read(&p21->x); */
/*@ assert rte_67: mem_access: \valid_read(&p11->x); */
/*@ assert rte_68: is_nan_or_infinite: \is_finite(\sub_float(p21->x, p11->x)); */
	DP.x = p21->x - p11->x;
/*@ assert rte_69: mem_access: \valid_read(&p21->y); */
/*@ assert rte_70: mem_access: \valid_read(&p11->y); */
/*@ assert rte_71: is_nan_or_infinite: \is_finite(\sub_float(p21->y, p11->y)); */
	DP.y = p21->y - p11->y;
/*@ assert rte_72: mem_access: \valid_read(&p12->x); */
/*@ assert rte_73: mem_access: \valid_read(&p11->x); */
/*@ assert rte_74: is_nan_or_infinite: \is_finite(\sub_float(p12->x, p11->x)); */
	QA.x = p12->x - p11->x;
/*@ assert rte_75: mem_access: \valid_read(&p12->y); */
/*@ assert rte_76: mem_access: \valid_read(&p11->y); */
/*@ assert rte_77: is_nan_or_infinite: \is_finite(\sub_float(p12->y, p11->y)); */
	QA.y = p12->y - p11->y;
/*@ assert rte_78: mem_access: \valid_read(&p22->x); */
/*@ assert rte_79: mem_access: \valid_read(&p21->x); */
/*@ assert rte_80: is_nan_or_infinite: \is_finite(\sub_float(p22->x, p21->x)); */
	QB.x = p22->x - p21->x;
/*@ assert rte_81: mem_access: \valid_read(&p22->y); */
/*@ assert rte_82: mem_access: \valid_read(&p21->y); */
/*@ assert rte_83: is_nan_or_infinite: \is_finite(\sub_float(p22->y, p21->y)); */
	QB.y = p22->y - p21->y;
/*@ assert rte_84: is_nan_or_infinite: \is_finite(\mul_float(QA.y, QB.x)); */
/*@ assert rte_85: is_nan_or_infinite: \is_finite(\mul_float(QB.y, QA.x)); */
/*@ assert rte_86: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(QA.y, QB.x), \mul_float(QB.y, QA.x))); */
	d = QA.y * QB.x - QB.y * QA.x;
	if (d == (float)0) {
		__retres = 0;
		goto return_label;
	}
/*@ assert rte_87: is_nan_or_infinite: \is_finite(\mul_float(QB.x, DP.y)); */
/*@ assert rte_88: is_nan_or_infinite: \is_finite(\mul_float(QB.y, DP.x)); */
/*@ assert rte_89: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(QB.x, DP.y), \mul_float(QB.y, DP.x))); */
/*@ assert rte_90: is_nan_or_infinite: \is_finite(\div_float(\sub_float(\mul_float(QB.x, DP.y), \mul_float(QB.y, DP.x)), d)); */
	la = (QB.x * DP.y - QB.y * DP.x) / d;
	if (la < (float)0) {
		__retres = 0;
		goto return_label;
	}
	else
	if (la > (float)1) {
		__retres = 0;
		goto return_label;
	}
/*@ assert rte_91: is_nan_or_infinite: \is_finite(\mul_float(QA.x, DP.y)); */
/*@ assert rte_92: is_nan_or_infinite: \is_finite(\mul_float(QA.y, DP.x)); */
/*@ assert rte_93: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(QA.x, DP.y), \mul_float(QA.y, DP.x))); */
/*@ assert rte_94: is_nan_or_infinite: \is_finite(\div_float(\sub_float(\mul_float(QA.x, DP.y), \mul_float(QA.y, DP.x)), d)); */
	lb = (QA.x * DP.y - QA.y * DP.x) / d;
	if (lb < (float)0) {
		__retres = 0;
		goto return_label;
	}
	else
	if (lb > (float)1) {
		__retres = 0;
		goto return_label;
	}
/*@ assert rte_95: mem_access: \valid(&dest->x); */
/*@ assert rte_96: mem_access: \valid_read(&p11->x); */
/*@ assert rte_97: is_nan_or_infinite: \is_finite(\mul_float(la, QA.x)); */
/*@ assert rte_98: is_nan_or_infinite: \is_finite(\add_float(p11->x, \mul_float(la, QA.x))); */
	dest->x = p11->x + la * QA.x;
/*@ assert rte_99: mem_access: \valid(&dest->y); */
/*@ assert rte_100: mem_access: \valid_read(&p11->y); */
/*@ assert rte_101: is_nan_or_infinite: \is_finite(\mul_float(la, QA.y)); */
/*@ assert rte_102: is_nan_or_infinite: \is_finite(\add_float(p11->y, \mul_float(la, QA.y))); */
	dest->y = p11->y + la * QA.y;
	__retres = 1;
	return_label: return __retres;
}

int m_2d_box_to_box_collision(float2 *min1, float2 *max1, float2 *min2,
float2 *max2)
{
	int tmp;
/*@ assert rte_58: mem_access: \valid_read(&min1->x); */
/*@ assert rte_59: mem_access: \valid_read(&max2->x); */
	if (min1->x > max2->x) tmp = 0;
	else
/*@ assert rte_60: mem_access: \valid_read(&max1->x); */
/*@ assert rte_61: mem_access: \valid_read(&min2->x); */
	if (max1->x < min2->x) tmp = 0;
	else
/*@ assert rte_62: mem_access: \valid_read(&min1->y); */
/*@ assert rte_63: mem_access: \valid_read(&max2->y); */
	if (min1->y > max2->y) tmp = 0;
	else
/*@ assert rte_64: mem_access: \valid_read(&max1->y); */
/*@ assert rte_65: mem_access: \valid_read(&min2->y); */
	if (max1->y < min2->y) tmp = 0; else tmp = 1;
	return tmp;
}

int m_3d_box_to_box_collision(float3 *min1, float3 *max1, float3 *min2,
float3 *max2)
{
	int tmp;
/*@ assert rte_103: mem_access: \valid_read(&min1->x); */
/*@ assert rte_104: mem_access: \valid_read(&max2->x); */
	if (min1->x > max2->x) tmp = 0;
	else
/*@ assert rte_105: mem_access: \valid_read(&max1->x); */
/*@ assert rte_106: mem_access: \valid_read(&min2->x); */
	if (max1->x < min2->x) tmp = 0;
	else
/*@ assert rte_107: mem_access: \valid_read(&min1->y); */
/*@ assert rte_108: mem_access: \valid_read(&max2->y); */
	if (min1->y > max2->y) tmp = 0;
	else
/*@ assert rte_109: mem_access: \valid_read(&max1->y); */
/*@ assert rte_110: mem_access: \valid_read(&min2->y); */
	if (max1->y < min2->y) tmp = 0;
	else
/*@ assert rte_111: mem_access: \valid_read(&min1->z); */
/*@ assert rte_112: mem_access: \valid_read(&max2->z); */
	if (min1->z > max2->z) tmp = 0;
	else
/*@ assert rte_113: mem_access: \valid_read(&max1->z); */
/*@ assert rte_114: mem_access: \valid_read(&min2->z); */
	if (max1->z < min2->z) tmp = 0; else tmp = 1;
	return tmp;
}

int m_3d_ray_box_intersection_in_out(float3 *ray_origin,
float3 *ray_direction, float3 *box_min,
float3 *box_max, float *in, float *out)
{
	int __retres;
	float3 idir;
	float3 tmin;
	float3 tmax;
	float3 tnear;
	float3 tfar;
	float tnx;
	float tny;
	float tfy;
	float in0;
	float out0;
/*@ assert rte_115: mem_access: \valid_read(&ray_direction->x); */
/*@ assert rte_116: is_nan_or_infinite: \is_finite(\div_float(1.0f, ray_direction->x)); */
	idir.x = 1.0f / ray_direction->x;
/*@ assert rte_117: mem_access: \valid_read(&ray_direction->y); */
/*@ assert rte_118: is_nan_or_infinite: \is_finite(\div_float(1.0f, ray_direction->y)); */
	idir.y = 1.0f / ray_direction->y;
/*@ assert rte_119: mem_access: \valid_read(&ray_direction->z); */
/*@ assert rte_120: is_nan_or_infinite: \is_finite(\div_float(1.0f, ray_direction->z)); */
	idir.z = 1.0f / ray_direction->z;
/*@ assert rte_121: mem_access: \valid_read(&box_min->x); */
/*@ assert rte_122: mem_access: \valid_read(&ray_origin->x); */
/*@ assert rte_123: is_nan_or_infinite: \is_finite(\sub_float(box_min->x, ray_origin->x)); */
/*@ assert rte_124: is_nan_or_infinite: \is_finite(\mul_float(\sub_float(box_min->x, ray_origin->x), idir.x)); */
	tmin.x = (box_min->x - ray_origin->x) * idir.x;
/*@ assert rte_125: mem_access: \valid_read(&box_min->y); */
/*@ assert rte_126: mem_access: \valid_read(&ray_origin->y); */
/*@ assert rte_127: is_nan_or_infinite: \is_finite(\sub_float(box_min->y, ray_origin->y)); */
/*@ assert rte_128: is_nan_or_infinite: \is_finite(\mul_float(\sub_float(box_min->y, ray_origin->y), idir.y)); */
	tmin.y = (box_min->y - ray_origin->y) * idir.y;
/*@ assert rte_129: mem_access: \valid_read(&box_min->z); */
/*@ assert rte_130: mem_access: \valid_read(&ray_origin->z); */
/*@ assert rte_131: is_nan_or_infinite: \is_finite(\sub_float(box_min->z, ray_origin->z)); */
/*@ assert rte_132: is_nan_or_infinite: \is_finite(\mul_float(\sub_float(box_min->z, ray_origin->z), idir.z)); */
	tmin.z = (box_min->z - ray_origin->z) * idir.z;
/*@ assert rte_133: mem_access: \valid_read(&box_max->x); */
/*@ assert rte_134: mem_access: \valid_read(&ray_origin->x); */
/*@ assert rte_135: is_nan_or_infinite: \is_finite(\sub_float(box_max->x, ray_origin->x)); */
/*@ assert rte_136: is_nan_or_infinite: \is_finite(\mul_float(\sub_float(box_max->x, ray_origin->x), idir.x)); */
	tmax.x = (box_max->x - ray_origin->x) * idir.x;
/*@ assert rte_137: mem_access: \valid_read(&box_max->y); */
/*@ assert rte_138: mem_access: \valid_read(&ray_origin->y); */
/*@ assert rte_139: is_nan_or_infinite: \is_finite(\sub_float(box_max->y, ray_origin->y)); */
/*@ assert rte_140: is_nan_or_infinite: \is_finite(\mul_float(\sub_float(box_max->y, ray_origin->y), idir.y)); */
	tmax.y = (box_max->y - ray_origin->y) * idir.y;
/*@ assert rte_141: mem_access: \valid_read(&box_max->z); */
/*@ assert rte_142: mem_access: \valid_read(&ray_origin->z); */
/*@ assert rte_143: is_nan_or_infinite: \is_finite(\sub_float(box_max->z, ray_origin->z)); */
/*@ assert rte_144: is_nan_or_infinite: \is_finite(\mul_float(\sub_float(box_max->z, ray_origin->z), idir.z)); */
	tmax.z = (box_max->z - ray_origin->z) * idir.z;
	tnear = M_MIN3(tmin,tmax);
	tfar = M_MAX3(tmin,tmax);
	tnx = M_MAX(tnear.x,0.0f);
	tny = M_MAX(tnear.y,tnear.z);
	tfy = M_MIN(tfar.y,tfar.z);
	in0 = M_MAX(tnx,tny);
	out0 = M_MIN(tfar.x,tfy);
	if (out0 > 0.0f)
	if (in0 < out0) {
	/*@ assert rte_145: mem_access: \valid(in); */
		*in = in0;
	/*@ assert rte_146: mem_access: \valid(out); */
		*out = out0;
		__retres = 1;
		goto return_label;
	}
	__retres = 0;
	return_label: return __retres;
}

int m_3d_ray_sphere_intersection_in_out(float3 *ray_origin,
float3 *ray_direction,
float3 *sphere_origin,
float sphere_radius2, float *in,
float *out)
{
	int __retres;
	float3 vec;
	float b;
	float det;
	float tmp;
/*@ assert rte_155: mem_access: \valid_read(sphere_origin); */
/*@ assert rte_156: mem_access: \valid_read(ray_origin); */
	vec = M_SUB3(*sphere_origin,*ray_origin);
/*@ assert rte_157: mem_access: \valid_read(ray_direction); */
	b = M_DOT3(vec,*ray_direction);
	tmp = M_DOT3(vec,vec);
/*@ assert rte_158: is_nan_or_infinite: \is_finite(\mul_float(b, b)); */
/*@ assert rte_159: is_nan_or_infinite: \is_finite(\sub_float(\mul_float(b, b), tmp)); */
/*@ assert rte_160: is_nan_or_infinite: \is_finite(\add_float(\sub_float(\mul_float(b, b), tmp), sphere_radius2)); */
	det = (b * b - tmp) + sphere_radius2;
	if (det < (float)0) {
		__retres = 0;
		goto return_label;
	}
	else det = sqrtf(det);
/*@ assert rte_161: mem_access: \valid(in); */
/*@ assert rte_162: is_nan_or_infinite: \is_finite(\sub_float(b, det)); */
	*in = b - det;
/*@ assert rte_163: mem_access: \valid(out); */
/*@ assert rte_164: is_nan_or_infinite: \is_finite(\add_float(b, det)); */
	*out = b + det;
	__retres = 1;
	return_label: return __retres;
}


