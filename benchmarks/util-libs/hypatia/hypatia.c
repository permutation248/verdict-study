#include <math.h>
#include <stdio.h>
#include <stdint.h>


#define HYP_FLOAT float


#define HYP_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f
#define HYP_TAU 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359f
#define HYP_PI_HALF 1.5707963267948966f
#define HYP_PI_SQUARED 9.8696044010893586f
#define HYP_E 2.71828182845904523536028747135266249775724709369995f
#define HYP_RAD_PER_DEG 0.0174532925199432957692369076848861f
#define HYP_DEG_PER_RAD 57.2957795130823208767981548141052f
#define HYP_PIOVER180  HYP_RAD_PER_DEG
#define HYP_PIUNDER180 HYP_DEG_PER_RAD
#define HYP_EPSILON 1E-5f


/** @brief A function that returns the minimum of \a a and \a b */

HYP_FLOAT HYP_MIN(HYP_FLOAT a, HYP_FLOAT b)
{
	return (a < b) ? a : b;
}


/** @brief A macro that returns the maximum of \a a and \a b */

HYP_FLOAT HYP_MAX(HYP_FLOAT a, HYP_FLOAT b)
{
	return (a > b) ? a : b;
}


/** @brief A macro that swaps \a a and \a b */

void HYP_SWAP(HYP_FLOAT *a, HYP_FLOAT *b)
{
	HYP_FLOAT f = *a; *a = *b; *b = f;
}


/** @brief A macro that converts an angle in degress to an angle in radians */
#define HYP_DEG_TO_RAD(angle)  ((angle) * HYP_RAD_PER_DEG)
/** @brief A macro that converts an angle in radians to an angle in degrees */
#define HYP_DEG_TO_RAD(angle)  ((angle) * HYP_RAD_PER_DEG)


/** @brief A macro that squares a value squared */

HYP_FLOAT HYP_SQUARE(HYP_FLOAT number)
{
	return number * number;
}


/** @brief A macro that returns the absolute value */

HYP_FLOAT HYP_ABS(HYP_FLOAT value)
{
	return (value < 0.0f) ? -value : value;
}


/** @brief A macro that constrains the value between two limits \a a and \a b */

HYP_FLOAT HYP_CLAMP(HYP_FLOAT value, HYP_FLOAT start, HYP_FLOAT limit)
{
	return ((value < start) ? start : (value > limit) ? limit : value);
}


#define HYP_REF_VECTOR2_ZERO 0
#define HYP_REF_VECTOR2_UNIT_X 1
#define HYP_REF_VECTOR2_UNIT_Y 2
#define HYP_REF_VECTOR2_UNIT_X_NEGATIVE 3
#define HYP_REF_VECTOR2_UNIT_Y_NEGATIVE 4
#define HYP_REF_VECTOR2_ONE 5

#define HYP_REF_VECTOR3_ZERO 0
#define HYP_REF_VECTOR3_UNIT_X 1
#define HYP_REF_VECTOR3_UNIT_Y 2
#define HYP_REF_VECTOR3_UNIT_Z 3
#define HYP_REF_VECTOR3_UNIT_X_NEGATIVE 4
#define HYP_REF_VECTOR3_UNIT_Y_NEGATIVE 5
#define HYP_REF_VECTOR3_UNIT_Z_NEGATIVE 6
#define HYP_REF_VECTOR3_ONE 7

#define HYP_REF_VECTOR3_ZERO 0
#define HYP_REF_VECTOR3_UNIT_X 1
#define HYP_REF_VECTOR3_UNIT_Y 2
#define HYP_REF_VECTOR3_UNIT_Z 3
#define HYP_REF_VECTOR3_UNIT_X_NEGATIVE 4
#define HYP_REF_VECTOR3_UNIT_Y_NEGATIVE 5
#define HYP_REF_VECTOR3_UNIT_Z_NEGATIVE 6
#define HYP_REF_VECTOR3_ONE 7

#define HYP_REF_VECTOR4_ZERO 0
#define HYP_REF_VECTOR4_UNIT_X 1
#define HYP_REF_VECTOR4_UNIT_Y 2
#define HYP_REF_VECTOR4_UNIT_Z 3
#define HYP_REF_VECTOR4_UNIT_X_NEGATIVE 4
#define HYP_REF_VECTOR4_UNIT_Y_NEGATIVE 5
#define HYP_REF_VECTOR4_UNIT_Z_NEGATIVE 6
#define HYP_REF_VECTOR4_ONE 7


struct vector2 {
	union {
		HYP_FLOAT v[2];
		struct {
			HYP_FLOAT x, y;
		};
	};
};

typedef struct vector2 vec2;

struct vector3 {
	union {
		HYP_FLOAT v[3];
		struct {
			HYP_FLOAT x, y, z;
		};
		struct {
			HYP_FLOAT yaw, pitch, roll;
		};
	};
};

typedef struct vector3 vec3;


struct vector4 {
	union {
		HYP_FLOAT v[4];
		struct {
			HYP_FLOAT x, y, z, w;
		};
	};
};

typedef struct vector4 vec4;


struct matrix2 {
	union {
		HYP_FLOAT m[4]; /* row-major numbering */
		struct {
			/* reference the matrix [row][column] */
			HYP_FLOAT m22[2][2];
		};
		struct {
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i02;
			HYP_FLOAT i01, i03;
		};
		struct {
			/* col-row */
			HYP_FLOAT c00, c10;
			HYP_FLOAT c01, c11;
		};
		struct {
			/* row-col */
			HYP_FLOAT r00, r01;
			HYP_FLOAT r10, r11;
		};
	};
};

typedef struct matrix2 mat2;


struct matrix3 {
	union {
		HYP_FLOAT m[9]; /* row-major numbering */
		struct {
			/* reference the matrix [row][column] */
			HYP_FLOAT m33[3][3];
		};
		struct {
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i03, i06;
			HYP_FLOAT i01, i04, i07;
			HYP_FLOAT i02, i05, i08;
		};
		struct {
			/* col-row */
			HYP_FLOAT c00, c10, c20;
			HYP_FLOAT c01, c11, c21;
			HYP_FLOAT c02, c12, c22;
		};
		struct {
			/* row-col */
			HYP_FLOAT r00, r01, r02;
			HYP_FLOAT r10, r11, r12;
			HYP_FLOAT r20, r21, r22;
		};
	};
};

typedef struct matrix3 mat3;


struct matrix4 {
	union {
		HYP_FLOAT m[16]; /* row-major numbering */
		struct {
			/* reference the matrix [row][column] */
			HYP_FLOAT m44[4][4];
		};
		struct {
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i04, i08, i12;
			HYP_FLOAT i01, i05, i09, i13;
			HYP_FLOAT i02, i06, i10, i14;
			HYP_FLOAT i03, i07, i11, i15;
		};
		struct {
			/* col-row */
			HYP_FLOAT c00, c10, c20, c30;
			HYP_FLOAT c01, c11, c21, c31;
			HYP_FLOAT c02, c12, c22, c32;
			HYP_FLOAT c03, c13, c23, c33;
		};
		struct {
			/* row-col */
			HYP_FLOAT r00, r01, r02, r03;
			HYP_FLOAT r10, r11, r12, r13;
			HYP_FLOAT r20, r21, r22, r23;
			HYP_FLOAT r30, r31, r32, r33;
		};
	};
};

typedef struct matrix4 mat4;


struct quaternion {
	union {
		HYP_FLOAT q[4];
		struct {
			HYP_FLOAT x, y, z, w;
		};
		struct {
			HYP_FLOAT i, j, k, a;
		};
	};
};

typedef struct quaternion quat;


static struct vector2 _vector2_zero = { { {0.0f, 0.0f} } };
static struct vector2 _vector2_one = { { {1.0f, 1.0f} } };
static struct vector2 _vector2_unit_x = { { {1.0f, 0.0f} } };
static struct vector2 _vector2_unit_y = { { {0.0f, 1.0f} } };
static struct vector2 _vector2_unit_x_negative = { { {-1.0f, 0.0f} } };
static struct vector2 _vector2_unit_y_negative = { { {0.0f, -1.0f} } };



/**
 * @brief This checks for mathematical equality within a custom epsilon.
 *
 */

short scalar_equals_epsilonf(const HYP_FLOAT f1, const HYP_FLOAT f2, const HYP_FLOAT epsilon)
{
	if ((HYP_ABS(f1 - f2) < epsilon) == 0) {
		return 0;
	}

	return 1;
}


/**
 * @brief This checks for mathematical equality within HYP_EPSILON.
 *
 */

short scalar_equalsf(const HYP_FLOAT f1, const HYP_FLOAT f2)
{
	return scalar_equals_epsilonf(f1, f2, HYP_EPSILON);
}

#define scalar_equals scalar_equalsf



const struct vector2 *vector2_get_reference_vector2(int id)
{
	switch (id) {
	case HYP_REF_VECTOR2_ZERO:
		return &_vector2_zero;
	case HYP_REF_VECTOR2_ONE:
		return &_vector2_one;
	case HYP_REF_VECTOR2_UNIT_X:
		return &_vector2_unit_x;
	case HYP_REF_VECTOR2_UNIT_Y:
		return &_vector2_unit_y;
	case HYP_REF_VECTOR2_UNIT_X_NEGATIVE:
		return &_vector2_unit_x_negative;
	case HYP_REF_VECTOR2_UNIT_Y_NEGATIVE:
		return &_vector2_unit_y_negative;
	default:
		return &_vector2_zero;
	}
}



struct vector2 *vector2_set(struct vector2 *self, const struct vector2 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	return self;
}



struct vector2 *vector2_setf2(struct vector2 *self, HYP_FLOAT xT, HYP_FLOAT yT)
{
	self->x = xT;
	self->y = yT;
	return self;
}



struct vector2 *vector2_zero(struct vector2 *self)
{
	return vector2_setf2(self, 0.0f, 0.0f);
}




int vector2_equals(const struct vector2 *self, const struct vector2 *vT)
{
	return scalar_equals(self->x, vT->x) && scalar_equals(self->y, vT->y);
}



struct vector2 *vector2_negate(struct vector2 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	return self;
}



struct vector2 *vector2_add(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	return self;
}



struct vector2 *vector2_addf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] += fT;
	self->v[1] += fT;
	return self;
}



struct vector2 *vector2_subtract(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	return self;
}



struct vector2 *vector2_subtractf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] -= fT;
	self->v[1] -= fT;
	return self;
}



struct vector2 *vector2_multiply(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	return self;
}



struct vector2 *vector2_multiplyf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] *= fT;
	self->v[1] *= fT;
	return self;
}




struct vector2 *vector2_divide(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	return self;
}



struct vector2 *vector2_dividef(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] /= fT;
	self->v[1] /= fT;
	return self;
}



HYP_FLOAT vector2_dot_product(const struct vector2 *self, const struct vector2 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y);
}



struct vector2 *vector2_cross_product(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2)
{
	vR->x = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	vR->y = (vT1->y * vT2->x) - (vT1->x * vT2->y);
	return vR;
}


// /**
//  * @brief Multiply a vector by a matrix, returns a vector
//  *
//  * @param self The vector being multiplied
//  * @param mT The matrix used to do the multiplication
//  */
// struct vector2 *vector2_multiplym2(struct vector2 *self, const struct matrix2 *mT)
// {
// 	struct vector2 vR;

// 	vector2_zero(&vR);

// 	matrix2_multiplyv2(mT, self, &vR);

// 	vector2_set(self, &vR);

// 	return self;
// }


// /**
//  * @brief Multiply a vector by a matrix, returns a vector
//  *
//  * @param self The vector being multiplied
//  * @param mT The matrix used to do the multiplication
//  */
// struct vector2 *vector2_multiplym3(struct vector2 *self, const struct matrix3 *mT)
// {
// 	struct vector2 vR;

// 	vector2_zero(&vR);

// 	matrix3_multiplyv2(mT, self, &vR);

// 	vector2_set(self, &vR);

// 	return self;
// }


static struct vector3 _vector3_zero = { { {0.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_one = { { {1.0f, 1.0f, 1.0f} } };
static struct vector3 _vector3_unit_x = { { {1.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_unit_y = { { {0.0f, 1.0f, 0.0f} } };
static struct vector3 _vector3_unit_z = { { {0.0f, 0.0f, 1.0f} } };
static struct vector3 _vector3_unit_x_negative = { { {-1.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_unit_y_negative = { { {0.0f, -1.0f,  0.0f} } };
static struct vector3 _vector3_unit_z_negative = { { {0.0f,  0.0f, -1.0f} } };



const struct vector3 *vector3_get_reference_vector3(int id)
{
	switch (id) {
	case HYP_REF_VECTOR3_ZERO:
		return &_vector3_zero;
	case HYP_REF_VECTOR3_ONE:
		return &_vector3_one;
	case HYP_REF_VECTOR3_UNIT_X:
		return &_vector3_unit_x;
	case HYP_REF_VECTOR3_UNIT_Y:
		return &_vector3_unit_y;
	case HYP_REF_VECTOR3_UNIT_Z:
		return &_vector3_unit_z;
	case HYP_REF_VECTOR3_UNIT_X_NEGATIVE:
		return &_vector3_unit_x_negative;
	case HYP_REF_VECTOR3_UNIT_Y_NEGATIVE:
		return &_vector3_unit_y_negative;
	case HYP_REF_VECTOR3_UNIT_Z_NEGATIVE:
		return &_vector3_unit_z_negative;
	default:
		/* undefined case */
		return &_vector3_zero;
	}
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with specific values
 */

struct vector3 *vector3_setf3(struct vector3 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT)
{
	self->x = xT;
	self->y = yT;
	self->z = zT;
	return self;
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with values from another vector
 */

struct vector3 *vector3_set(struct vector3 *self, const struct vector3 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	self->z = vT->z;
	return self;
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with zeros
 */

struct vector3 *vector3_zero(struct vector3 *self)
{
	return vector3_setf3(self, 0.0f, 0.0f, 0.0f);
}


/**
 * @ingroup vector3
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */

int vector3_equals(const struct vector3 *self, const struct vector3 *vT)
{
	return  HYP_ABS(self->x - vT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - vT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - vT->z) < HYP_EPSILON;
}


/**
 * @ingroup vector3
 * @brief switches the sign on each component of the vector
 */

struct vector3 *vector3_negate(struct vector3 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	self->v[2] = -self->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief adds vectors using component-wise addition
 */

struct vector3 *vector3_add(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	self->v[2] += vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief add to each component of the vector using a scalar
 */

struct vector3 *vector3_addf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] += f;
	self->v[1] += f;
	self->v[2] += f;
	return self;
}


/**
 * @ingroup vector3
 * @brief subtract two vectors using component-wise subtraction
 */

struct vector3 *vector3_subtract(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	self->v[2] -= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief subtract each vector's component by a scalar
 */

struct vector3 *vector3_subtractf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] -= f;
	self->v[1] -= f;
	self->v[2] -= f;
	return self;
}


/**
 * @ingroup vector3
 * @brief multiplies two vectors using component-wise multiplication
 */

struct vector3 *vector3_multiply(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	self->v[2] *= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief multiplies each component of the vector by a scalar
 */

struct vector3 *vector3_multiplyf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] *= f;
	self->v[1] *= f;
	self->v[2] *= f;
	return self;
}


/**
 * @ingroup vector3
 * @brief divides one vector into another using component-wise division
 *
 */

struct vector3 *vector3_divide(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	self->v[2] /= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief computes the dot product of two vectors
 */

HYP_FLOAT vector3_dot_product(const struct vector3 *self, const struct vector3 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z);
}


/**
 * @ingroup vector3
 * @brief computes the cross-product between two vectors
 */

struct vector3 *vector3_cross_product(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2)
{
	vR->x = (vT1->y * vT2->z) - (vT1->z * vT2->y);
	vR->y = (vT1->z * vT2->x) - (vT1->x * vT2->z);
	vR->z = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	return vR;
}


static struct vector4 _vector4_zero = { { {0.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_one = { { {1.0f, 1.0f, 1.0f, 1.0f} } };
static struct vector4 _vector4_unit_x = { { {1.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_y = { { {0.0f, 1.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_z = { { {0.0f, 0.0f, 1.0f, 0.0f} } };
static struct vector4 _vector4_unit_x_negative = { { {-1.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_y_negative = { { {0.0f, -1.0f,  0.0f, 0.0f} } };
static struct vector4 _vector4_unit_z_negative = { { {0.0f,  0.0f, -1.0f, 0.0f} } };



const struct vector4 *vector4_get_reference_vector4(int id)
{
	switch (id) {
	case HYP_REF_VECTOR4_ZERO:
		return &_vector4_zero;
	case HYP_REF_VECTOR4_ONE:
		return &_vector4_one;
	case HYP_REF_VECTOR4_UNIT_X:
		return &_vector4_unit_x;
	case HYP_REF_VECTOR4_UNIT_Y:
		return &_vector4_unit_y;
	case HYP_REF_VECTOR4_UNIT_Z:
		return &_vector4_unit_z;
	case HYP_REF_VECTOR4_UNIT_X_NEGATIVE:
		return &_vector4_unit_x_negative;
	case HYP_REF_VECTOR4_UNIT_Y_NEGATIVE:
		return &_vector4_unit_y_negative;
	case HYP_REF_VECTOR4_UNIT_Z_NEGATIVE:
		return &_vector4_unit_z_negative;
	default:
		/* undefined case */
		return &_vector4_zero;
	}
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with specific values
 */

struct vector4 *vector4_setf4(struct vector4 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT, HYP_FLOAT wT)
{
	self->x = xT;
	self->y = yT;
	self->z = zT;
	self->w = wT;
	return self;
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with values from another vector
 */

struct vector4 *vector4_set(struct vector4 *self, const struct vector4 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	self->z = vT->z;
	self->w = vT->w;
	return self;
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with zeros
 */

struct vector4 *vector4_zero(struct vector4 *self)
{
	return vector4_setf4(self, 0.0f, 0.0f, 0.0f, 0.0f);
}


/**
 * @ingroup vector4
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */

int vector4_equals(const struct vector4 *self, const struct vector4 *vT)
{
	return  HYP_ABS(self->x - vT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - vT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - vT->z) < HYP_EPSILON &&
		HYP_ABS(self->w - vT->w) < HYP_EPSILON;
}


/**
 * @ingroup vector4
 * @brief switches the sign on each component of the vector
 */

struct vector4 *vector4_negate(struct vector4 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	self->v[2] = -self->v[2];
	self->v[3] = -self->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief adds vectors using component-wise addition
 */

struct vector4 *vector4_add(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	self->v[2] += vT->v[2];
	self->v[3] += vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief add to each component of the vector using a scalar
 */

struct vector4 *vector4_addf(struct vector4 *self, HYP_FLOAT f)
{
	self->v[0] += f;
	self->v[1] += f;
	self->v[2] += f;
	self->v[3] += f;
	return self;
}


/**
 * @ingroup vector4
 * @brief subtract two vectors using component-wise subtraction
 */

struct vector4 *vector4_subtract(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	self->v[2] -= vT->v[2];
	self->v[3] -= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief subtract each vector's component by a scalar
 */

struct vector4 *vector4_subtractf(struct vector4 *self, HYP_FLOAT f)
{
	self->x -= f;
	self->y -= f;
	self->z -= f;
	self->w -= f;
	return self;
}


/**
 * @ingroup vector4
 * @brief multiplies two vectors using component-wise multiplication
 */

struct vector4 *vector4_multiply(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	self->v[2] *= vT->v[2];
	self->v[3] *= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief multiplies each component of the vector by a scalar
 */

struct vector4 *vector4_multiplyf(struct vector4 *self, HYP_FLOAT f)
{
	self->v[0] *= f;
	self->v[1] *= f;
	self->v[2] *= f;
	self->v[3] *= f;
	return self;
}


/**
 * @ingroup vector4
 * @brief divides one vector into another using component-wise division
 *
 */

struct vector4 *vector4_divide(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	self->v[2] /= vT->v[2];
	self->v[3] /= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief computes the dot product of two vectors
 */

HYP_FLOAT vector4_dot_product(const struct vector4 *self, const struct vector4 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z) + (self->w * vT->w);
}


/**
 * @ingroup vector4
 * @brief computes the cross-product between two vectors
 */

struct vector4 *vector4_cross_product(struct vector4 *vR, const struct vector4 *vT1, const struct vector4 *vT2)
{
	vR->x = (vT1->y * vT2->z) - (vT1->z * vT2->y);
	vR->y = (vT1->z * vT2->x) - (vT1->x * vT2->z);
	vR->z = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	vR->w = (vT1->w * vT2->w) - (vT1->w * vT2->w);
	return vR;
}



int main(void) {
    // printf("Starting Hypatia library tests...\n");
    
    /* Test HYP_MIN */
    HYP_FLOAT result1 = HYP_MIN(1.0f, 2.0f);
    HYP_FLOAT result2 = HYP_MIN(2.0f, 1.0f);
    HYP_FLOAT result3 = HYP_MIN(-1.0f, 1.0f);
    HYP_FLOAT result4 = HYP_MIN(0.0f, 0.0f);
    HYP_FLOAT result5 = HYP_MIN(3.5f, 3.5f);
    HYP_FLOAT result6 = HYP_MIN(-5.0f, -3.0f);
    HYP_FLOAT result7 = HYP_MIN(100.0f, 200.0f);
    HYP_FLOAT result8 = HYP_MIN(-100.0f, -200.0f);
    // printf("HYP_MIN tests passed.\n");
    
    /* Test HYP_MAX */
    HYP_FLOAT result9 = HYP_MAX(1.0f, 2.0f);
    HYP_FLOAT result10 = HYP_MAX(2.0f, 1.0f);
    HYP_FLOAT result11 = HYP_MAX(-1.0f, 1.0f);
    HYP_FLOAT result12 = HYP_MAX(0.0f, 0.0f);
    HYP_FLOAT result13 = HYP_MAX(3.5f, 3.5f);
    HYP_FLOAT result14 = HYP_MAX(-5.0f, -3.0f);
    HYP_FLOAT result15 = HYP_MAX(100.0f, 200.0f);
    HYP_FLOAT result16 = HYP_MAX(-100.0f, -200.0f);
    // printf("HYP_MAX tests passed.\n");
    
    /* Test HYP_SWAP */
    HYP_FLOAT a = 1.0f, b = 2.0f;
    HYP_FLOAT old_a = a, old_b = b;
    HYP_SWAP(&a, &b);
    
    a = -5.0f; b = 10.0f;
    old_a = a; old_b = b;
    HYP_SWAP(&a, &b);
    
    a = 0.0f; b = 0.0f;
    old_a = a; old_b = b;
    HYP_SWAP(&a, &b);
    // printf("HYP_SWAP tests passed.\n");
    
    /* Test HYP_SQUARE */
    HYP_FLOAT result23 = HYP_SQUARE(0.0f);
    HYP_FLOAT result24 = HYP_SQUARE(1.0f);
    HYP_FLOAT result25 = HYP_SQUARE(-1.0f);
    HYP_FLOAT result26 = HYP_SQUARE(2.0f);
    HYP_FLOAT result27 = HYP_SQUARE(-2.0f);
    HYP_FLOAT result28 = HYP_SQUARE(3.5f);
    HYP_FLOAT result29 = HYP_SQUARE(-3.5f);
    HYP_FLOAT result30 = HYP_SQUARE(10.0f);
    // printf("HYP_SQUARE tests passed.\n");
    
    /* Test HYP_ABS */
    HYP_FLOAT result31 = HYP_ABS(0.0f);
    HYP_FLOAT result32 = HYP_ABS(1.0f);
    HYP_FLOAT result33 = HYP_ABS(-1.0f);
    HYP_FLOAT result34 = HYP_ABS(5.5f);
    HYP_FLOAT result35 = HYP_ABS(-5.5f);
    HYP_FLOAT result36 = HYP_ABS(100.0f);
    HYP_FLOAT result37 = HYP_ABS(-100.0f);
    HYP_FLOAT result38 = HYP_ABS(HYP_PI);
    // printf("HYP_ABS tests passed.\n");
    
    /* Test HYP_CLAMP */
    HYP_FLOAT result39 = HYP_CLAMP(5.0f, 0.0f, 10.0f);
    HYP_FLOAT result40 = HYP_CLAMP(-5.0f, 0.0f, 10.0f);
    HYP_FLOAT result41 = HYP_CLAMP(15.0f, 0.0f, 10.0f);
    HYP_FLOAT result42 = HYP_CLAMP(0.0f, -10.0f, 10.0f);
    HYP_FLOAT result43 = HYP_CLAMP(-15.0f, -10.0f, 10.0f);
    HYP_FLOAT result44 = HYP_CLAMP(15.0f, -10.0f, 10.0f);
    HYP_FLOAT result45 = HYP_CLAMP(2.5f, 2.5f, 7.5f);
    HYP_FLOAT result46 = HYP_CLAMP(7.5f, 2.5f, 7.5f);
    // printf("HYP_CLAMP tests passed.\n");
    
    /* Test scalar_equals_epsilonf */
    int result47 = scalar_equals_epsilonf(1.0f, 1.0f, 0.0001f);
    int result48 = scalar_equals_epsilonf(1.0f, 1.00001f, 0.0001f);
    int result49 = scalar_equals_epsilonf(1.0f, 1.1f, 0.0001f);
    int result50 = scalar_equals_epsilonf(0.0f, 0.0f, 0.0001f);
    int result51 = scalar_equals_epsilonf(-1.0f, -1.0f, 0.0001f);
    int result52 = scalar_equals_epsilonf(-1.0f, -1.00001f, 0.0001f);
    int result53 = scalar_equals_epsilonf(-1.0f, -0.9f, 0.0001f);
    int result54 = scalar_equals_epsilonf(HYP_PI, HYP_PI, 0.0001f);
    // printf("scalar_equals_epsilonf tests passed.\n");
    
    /* Test scalar_equalsf */
    int result55 = scalar_equalsf(1.0f, 1.0f);
    int result56 = scalar_equalsf(1.0f, 1.000001f);
  /* Within HYP_EPSILON (1E-5f) */
    int result57 = scalar_equalsf(1.0f, 1.1f);
    int result58 = scalar_equalsf(0.0f, 0.0f);
    int result59 = scalar_equalsf(-1.0f, -1.0f);
    int result60 = scalar_equalsf(-1.0f, -1.000001f);
    int result61 = scalar_equalsf(-1.0f, -0.9f);
    int result62 = scalar_equalsf(HYP_PI, HYP_PI);
    // printf("scalar_equalsf tests passed.\n");
    
    /* Test vector2_functions */
    struct vector2 v1, v2, v3, v4, v5;
    
    /* Test vector2_setf2 */
    vector2_setf2(&v1, 1.0f, 2.0f);
    
    vector2_setf2(&v2, -3.0f, 4.5f);
    
    /* Test vector2_set */
    vector2_set(&v3, &v1);
    int equals_result67 = vector2_equals(&v3, &v1);
    
    /* Test vector2_zero */
    vector2_zero(&v4);
    
    /* Test vector2_equals */
    int equals_result70 = vector2_equals(&v1, &v1);
    int equals_result71 = vector2_equals(&v1, &v2);
    
    /* Test vector2_negate */
    v3.v[0] = 2.0f;
    v3.v[1] = -3.0f;
    vector2_negate(&v3);
    
    /* Test vector2_add */
    v1.v[0] = 1.0f; v1.v[1] = 2.0f;
    v2.v[0] = 3.0f; v2.v[1] = 4.0f;
    vector2_add(&v1, &v2);
    
    /* Test vector2_addf */
    v1.v[0] = 1.0f; v1.v[1] = 2.0f;
    vector2_addf(&v1, 5.0f);
    
    /* Test vector2_subtract */
    v1.v[0] = 5.0f; v1.v[1] = 8.0f;
    v2.v[0] = 2.0f; v2.v[1] = 3.0f;
    vector2_subtract(&v1, &v2);
    
    /* Test vector2_subtractf */
    v1.v[0] = 10.0f; v1.v[1] = 20.0f;
    vector2_subtractf(&v1, 5.0f);
    
    /* Test vector2_multiply */
    v1.v[0] = 2.0f; v1.v[1] = 3.0f;
    v2.v[0] = 4.0f; v2.v[1] = 5.0f;
    vector2_multiply(&v1, &v2);
    
    /* Test vector2_multiplyf */
    v1.v[0] = 2.0f; v1.v[1] = 3.0f;
    vector2_multiplyf(&v1, 3.0f);
    
    /* Test vector2_divide */
    v1.v[0] = 8.0f; v1.v[1] = 15.0f;
    v2.v[0] = 2.0f; v2.v[1] = 3.0f;
    vector2_divide(&v1, &v2);
    
    /* Test vector2_dividef */
    v1.v[0] = 10.0f; v1.v[1] = 20.0f;
    vector2_dividef(&v1, 2.0f);
    
    /* Test vector2_dot_product */
    vector2_setf2(&v1, 2.0f, 3.0f);
    vector2_setf2(&v2, 4.0f, 5.0f);
    HYP_FLOAT dot2 = vector2_dot_product(&v1, &v2);
  /* 2*4 + 3*5 = 8 + 15 = 23 */
    
    /* Test vector2_cross_product */
    vector2_setf2(&v1, 1.0f, 0.0f);
    vector2_setf2(&v2, 0.0f, 1.0f);
    vector2_cross_product(&v3, &v1, &v2);
  /* (1*1) - (0*0) = 1 */
  /* (0*0) - (1*1) = -1 */
    
    /* Test vector2_get_reference_vector2 */
    const struct vector2 *ref_zero = vector2_get_reference_vector2(HYP_REF_VECTOR2_ZERO);
    
    const struct vector2 *ref_unit_x = vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_X);
    
    // printf("vector2 tests passed.\n");
    
    /* Test vector3_functions */
    struct vector3 v31, v32, v33, v34;
    
    /* Test vector3_setf3 */
    vector3_setf3(&v31, 1.0f, 2.0f, 3.0f);
    
    /* Test vector3_set */
    vector3_setf3(&v32, 4.0f, 5.0f, 6.0f);
    vector3_set(&v33, &v32);
    int equals_result100 = vector3_equals(&v33, &v32);
    
    /* Test vector3_zero */
    vector3_zero(&v34);
    
    /* Test vector3_equals */
    int equals_result104 = vector3_equals(&v31, &v31);
    int equals_result105 = vector3_equals(&v31, &v32);
    
    /* Test vector3_negate */
    v33.v[0] = 2.0f; v33.v[1] = -3.0f; v33.v[2] = 4.0f;
    vector3_negate(&v33);
    
    /* Test vector3_add */
    v31.v[0] = 1.0f; v31.v[1] = 2.0f; v31.v[2] = 3.0f;
    v32.v[0] = 4.0f; v32.v[1] = 5.0f; v32.v[2] = 6.0f;
    vector3_add(&v31, &v32);
    
    /* Test vector3_addf */
    v31.v[0] = 1.0f; v31.v[1] = 2.0f; v31.v[2] = 3.0f;
    vector3_addf(&v31, 5.0f);
    
    /* Test vector3_subtract */
    v31.v[0] = 5.0f; v31.v[1] = 8.0f; v31.v[2] = 10.0f;
    v32.v[0] = 2.0f; v32.v[1] = 3.0f; v32.v[2] = 4.0f;
    vector3_subtract(&v31, &v32);
    
    /* Test vector3_subtractf */
    v31.v[0] = 10.0f; v31.v[1] = 20.0f; v31.v[2] = 30.0f;
    vector3_subtractf(&v31, 5.0f);
    
    /* Test vector3_multiply */
    v31.v[0] = 2.0f; v31.v[1] = 3.0f; v31.v[2] = 4.0f;
    v32.v[0] = 5.0f; v32.v[1] = 6.0f; v32.v[2] = 7.0f;
    vector3_multiply(&v31, &v32);
    
    /* Test vector3_multiplyf */
    v31.v[0] = 2.0f; v31.v[1] = 3.0f; v31.v[2] = 4.0f;
    vector3_multiplyf(&v31, 3.0f);
    
    /* Test vector3_divide */
    v31.v[0] = 8.0f; v31.v[1] = 15.0f; v31.v[2] = 21.0f;
    v32.v[0] = 2.0f; v32.v[1] = 3.0f; v32.v[2] = 7.0f;
    vector3_divide(&v31, &v32);
    
    /* Test vector3_dot_product */
    vector3_setf3(&v31, 2.0f, 3.0f, 4.0f);
    vector3_setf3(&v32, 5.0f, 6.0f, 7.0f);
    HYP_FLOAT dot3 = vector3_dot_product(&v31, &v32);
  /* 2*5 + 3*6 + 4*7 = 10 + 18 + 28 = 56 */
    
    /* Test vector3_cross_product */
    vector3_setf3(&v31, 1.0f, 0.0f, 0.0f);
    vector3_setf3(&v32, 0.0f, 1.0f, 0.0f);
    vector3_cross_product(&v33, &v31, &v32);
  /* (0*0) - (0*1) = 0 */
  /* (0*0) - (1*0) = 0 */
  /* (1*1) - (0*0) = 1 */
    
    /* Test vector3_get_reference_vector3 */
    const struct vector3 *ref_zero3 = vector3_get_reference_vector3(HYP_REF_VECTOR3_ZERO);
    
    const struct vector3 *ref_unit_x3 = vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_X);
    
    // printf("vector3 tests passed.\n");
    
    /* Test vector4_functions */
    struct vector4 v41, v42, v43, v44;
    
    /* Test vector4_setf4 */
    vector4_setf4(&v41, 1.0f, 2.0f, 3.0f, 4.0f);
    
    /* Test vector4_set */
    vector4_setf4(&v42, 5.0f, 6.0f, 7.0f, 8.0f);
    vector4_set(&v43, &v42);
    int equals_result144 = vector4_equals(&v43, &v42);
    
    /* Test vector4_zero */
    vector4_zero(&v44);
    
    /* Test vector4_equals */
    int equals_result149 = vector4_equals(&v41, &v41);
    int equals_result150 = vector4_equals(&v41, &v42);
    
    /* Test vector4_negate */
    v43.v[0] = 2.0f; v43.v[1] = -3.0f; v43.v[2] = 4.0f; v43.v[3] = -5.0f;
    vector4_negate(&v43);
    
    /* Test vector4_add */
    v41.v[0] = 1.0f; v41.v[1] = 2.0f; v41.v[2] = 3.0f; v41.v[3] = 4.0f;
    v42.v[0] = 5.0f; v42.v[1] = 6.0f; v42.v[2] = 7.0f; v42.v[3] = 8.0f;
    vector4_add(&v41, &v42);
    
    /* Test vector4_addf */
    v41.v[0] = 1.0f; v41.v[1] = 2.0f; v41.v[2] = 3.0f; v41.v[3] = 4.0f;
    vector4_addf(&v41, 5.0f);
    
    /* Test vector4_subtract */
    v41.v[0] = 10.0f; v41.v[1] = 20.0f; v41.v[2] = 30.0f; v41.v[3] = 40.0f;
    v42.v[0] = 1.0f; v42.v[1] = 2.0f; v42.v[2] = 3.0f; v42.v[3] = 4.0f;
    vector4_subtract(&v41, &v42);
    
    /* Test vector4_subtractf */
    vector4_setf4(&v41, 10.0f, 20.0f, 30.0f, 40.0f);
    vector4_subtractf(&v41, 5.0f);
    
    /* Test vector4_multiply */
    v41.v[0] = 2.0f; v41.v[1] = 3.0f; v41.v[2] = 4.0f; v41.v[3] = 5.0f;
    v42.v[0] = 6.0f; v42.v[1] = 7.0f; v42.v[2] = 8.0f; v42.v[3] = 9.0f;
    vector4_multiply(&v41, &v42);
    
    /* Test vector4_multiplyf */
    v41.v[0] = 2.0f; v41.v[1] = 3.0f; v41.v[2] = 4.0f; v41.v[3] = 5.0f;
    vector4_multiplyf(&v41, 3.0f);
    
    /* Test vector4_divide */
    v41.v[0] = 12.0f; v41.v[1] = 21.0f; v41.v[2] = 32.0f; v41.v[3] = 45.0f;
    v42.v[0] = 2.0f; v42.v[1] = 3.0f; v42.v[2] = 4.0f; v42.v[3] = 5.0f;
    vector4_divide(&v41, &v42);
    
    /* Test vector4_dot_product */
    vector4_setf4(&v41, 2.0f, 3.0f, 4.0f, 5.0f);
    vector4_setf4(&v42, 6.0f, 7.0f, 8.0f, 9.0f);
    HYP_FLOAT dot4 = vector4_dot_product(&v41, &v42);
  /* 2*6 + 3*7 + 4*8 + 5*9 = 12 + 21 + 32 + 45 = 110 */
    
    /* Test vector4_cross_product */
    vector4_setf4(&v41, 1.0f, 0.0f, 0.0f, 1.0f);
    vector4_setf4(&v42, 0.0f, 1.0f, 0.0f, 1.0f);
    vector4_cross_product(&v43, &v41, &v42);
    
    /* Test vector4_get_reference_vector4 */
    const struct vector4 *ref_zero4 = vector4_get_reference_vector4(HYP_REF_VECTOR4_ZERO);
    
    const struct vector4 *ref_unit_x4 = vector4_get_reference_vector4(HYP_REF_VECTOR4_UNIT_X);
    
    // printf("vector4 tests passed.\n");
    
    // printf("All tests completed.\n");
    return 0;
}
