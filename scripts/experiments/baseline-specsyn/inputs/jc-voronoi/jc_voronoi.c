#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
typedef float jcv_real;
struct jcv_point_;
typedef struct jcv_point_ jcv_point;
struct jcv_site_;
typedef struct jcv_site_ jcv_site;
struct jcv_edge_;
typedef struct jcv_edge_ jcv_edge;
struct jcv_graphedge_;
typedef struct jcv_graphedge_ jcv_graphedge;
struct jcv_point_ {
   jcv_real x ;
   jcv_real y ;
};
struct jcv_graphedge_ {
   struct jcv_graphedge_ *next ;
   struct jcv_edge_ *edge ;
   struct jcv_site_ *neighbor ;
   jcv_point pos[2] ;
   jcv_real angle ;
};
struct jcv_site_ {
   jcv_point p ;
   int index ;
   jcv_graphedge *edges ;
};
struct jcv_edge_ {
   struct jcv_edge_ *next ;
   jcv_site *sites[2] ;
   jcv_point pos[2] ;
   jcv_real a ;
   jcv_real b ;
   jcv_real c ;
};
struct jcv_halfedge_ {
   jcv_edge *edge ;
   struct jcv_halfedge_ *left ;
   struct jcv_halfedge_ *right ;
   jcv_point vertex ;
   jcv_real y ;
   int direction ;
   int pqpos ;
};
typedef struct jcv_halfedge_ jcv_halfedge;
int const JCV_DIRECTION_LEFT = 0;
int const JCV_DIRECTION_RIGHT = 1;
jcv_real const JCV_INVALID_VALUE = - 3.402823466e+38F;
jcv_real jcv_abs(jcv_real v)
{
  jcv_real tmp;
  if (v < (jcv_real)0) 
    /*@ assert rte_21: is_nan_or_infinite: \is_finite(v); */
    tmp = - v;
  else tmp = v;
  return tmp;
}

int jcv_real_eq(jcv_real a, jcv_real b)
{
  int __retres;
  jcv_real tmp;
  /*@ assert rte_22: is_nan_or_infinite: \is_finite(\sub_float(a, b)); */
  tmp = jcv_abs(a - b);
  __retres = (double)tmp < 0x1p-23;
  return __retres;
}

jcv_real jcv_real_to_int(jcv_real v)
{
  jcv_real tmp;
  if (sizeof(jcv_real) == (unsigned long)4) 
    /*@ assert rte_23: float_to_int: v < 2147483648; */
    /*@ assert rte_24: float_to_int: -2147483649 < v; */
    tmp = (jcv_real)((int)v);
  else 
    /*@ assert rte_25: float_to_int: v < 9223372036854775808; */
    /*@ assert rte_26: float_to_int: -9223372036854775809 < v; */
    tmp = (jcv_real)((long long)v);
  return tmp;
}

jcv_real jcv_floor(jcv_real v)
{
  jcv_real tmp_0;
  jcv_real i = jcv_real_to_int(v);
  if (v < i) 
    /*@ assert rte_27: is_nan_or_infinite: \is_finite(\sub_float(i, (jcv_real)1)); */
    tmp_0 = i - (jcv_real)1;
  else tmp_0 = i;
  return tmp_0;
}

jcv_real jcv_ceil(jcv_real v)
{
  jcv_real tmp_0;
  jcv_real i = jcv_real_to_int(v);
  if (v > i) 
    /*@ assert rte_28: is_nan_or_infinite: \is_finite(\add_float(i, (jcv_real)1)); */
    tmp_0 = i + (jcv_real)1;
  else tmp_0 = i;
  return tmp_0;
}

jcv_real jcv_min(jcv_real a, jcv_real b)
{
  jcv_real tmp;
  if (a < b) tmp = a; else tmp = b;
  return tmp;
}

jcv_real jcv_max(jcv_real a, jcv_real b)
{
  jcv_real tmp;
  if (a > b) tmp = a; else tmp = b;
  return tmp;
}

int jcv_point_cmp(jcv_point const *p1, jcv_point const *p2)
{
  int tmp_1;
  jcv_point const *s1 = p1;
  jcv_point const *s2 = p2;
  /*@ assert rte_29: mem_access: \valid_read(&s1->y); */
  /*@ assert rte_30: mem_access: \valid_read(&s2->y); */
  if (s1->y != s2->y) {
    int tmp;
    /*@ assert rte_31: mem_access: \valid_read(&s1->y); */
    /*@ assert rte_32: mem_access: \valid_read(&s2->y); */
    if (s1->y < s2->y) tmp = -1; else tmp = 1;
    tmp_1 = tmp;
  }
  else {
    int tmp_0;
    /*@ assert rte_33: mem_access: \valid_read(&s1->x); */
    /*@ assert rte_34: mem_access: \valid_read(&s2->x); */
    if (s1->x < s2->x) tmp_0 = -1; else tmp_0 = 1;
    tmp_1 = tmp_0;
  }
  return tmp_1;
}

int jcv_point_less(jcv_point const *pt1, jcv_point const *pt2)
{
  int tmp;
  /*@ assert rte_35: mem_access: \valid_read(&pt1->y); */
  /*@ assert rte_36: mem_access: \valid_read(&pt2->y); */
  if (pt1->y == pt2->y) 
    /*@ assert rte_37: mem_access: \valid_read(&pt1->x); */
    /*@ assert rte_38: mem_access: \valid_read(&pt2->x); */
    tmp = pt1->x < pt2->x;
  else 
    /*@ assert rte_39: mem_access: \valid_read(&pt1->y); */
    /*@ assert rte_40: mem_access: \valid_read(&pt2->y); */
    tmp = pt1->y < pt2->y;
  return tmp;
}

int jcv_point_eq(jcv_point const *pt1, jcv_point const *pt2)
{
  int tmp_1;
  int tmp;
  /*@ assert rte_41: mem_access: \valid_read(&pt1->y); */
  /*@ assert rte_42: mem_access: \valid_read(&pt2->y); */
  tmp = jcv_real_eq(pt1->y,pt2->y);
  if (tmp) {
    int tmp_0;
    /*@ assert rte_43: mem_access: \valid_read(&pt1->x); */
    /*@ assert rte_44: mem_access: \valid_read(&pt2->x); */
    tmp_0 = jcv_real_eq(pt1->x,pt2->x);
    if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
  }
  else tmp_1 = 0;
  return tmp_1;
}

int jcv_point_on_box_edge(jcv_point const *pt, jcv_point const *min,
                          jcv_point const *max)
{
  int tmp;
  /*@ assert rte_45: mem_access: \valid_read(&pt->x); */
  /*@ assert rte_46: mem_access: \valid_read(&min->x); */
  if (pt->x == min->x) tmp = 1;
  else 
    /*@ assert rte_47: mem_access: \valid_read(&pt->y); */
    /*@ assert rte_48: mem_access: \valid_read(&min->y); */
    if (pt->y == min->y) tmp = 1;
    else 
      /*@ assert rte_49: mem_access: \valid_read(&pt->x); */
      /*@ assert rte_50: mem_access: \valid_read(&max->x); */
      if (pt->x == max->x) tmp = 1;
      else 
        /*@ assert rte_51: mem_access: \valid_read(&pt->y); */
        /*@ assert rte_52: mem_access: \valid_read(&max->y); */
        if (pt->y == max->y) tmp = 1; else tmp = 0;
  return tmp;
}

int const JCV_EDGE_LEFT = 1;
int const JCV_EDGE_RIGHT = 2;
int const JCV_EDGE_BOTTOM = 4;
int const JCV_EDGE_TOP = 8;
int const JCV_CORNER_NONE = 0;
int const JCV_CORNER_TOP_LEFT = 1;
int const JCV_CORNER_BOTTOM_LEFT = 2;
int const JCV_CORNER_BOTTOM_RIGHT = 3;
int const JCV_CORNER_TOP_RIGHT = 4;
int jcv_is_corner(int corner)
{
  int __retres;
  __retres = corner != 0;
  return __retres;
}

int jcv_corner_rotate_90(int corner)
{
  int __retres;
  /*@ assert rte_53: signed_overflow: -2147483648 <= corner - 1; */
  corner --;
  /*@ assert rte_54: signed_overflow: corner + 1 <= 2147483647; */
  corner = (corner + 1) % 4;
  /*@ assert rte_55: signed_overflow: corner + 1 <= 2147483647; */
  __retres = corner + 1;
  return __retres;
}

jcv_point jcv_corner_to_point(int corner, jcv_point const *min,
                              jcv_point const *max)
{
  jcv_point p;
  if (corner == JCV_CORNER_TOP_LEFT) {
    /*@ assert rte_56: mem_access: \valid_read(&min->x); */
    p.x = min->x;
    /*@ assert rte_57: mem_access: \valid_read(&max->y); */
    p.y = max->y;
  }
  else 
    if (corner == JCV_CORNER_TOP_RIGHT) {
      /*@ assert rte_58: mem_access: \valid_read(&max->x); */
      p.x = max->x;
      /*@ assert rte_59: mem_access: \valid_read(&max->y); */
      p.y = max->y;
    }
    else 
      if (corner == JCV_CORNER_BOTTOM_LEFT) {
        /*@ assert rte_60: mem_access: \valid_read(&min->x); */
        p.x = min->x;
        /*@ assert rte_61: mem_access: \valid_read(&min->y); */
        p.y = min->y;
      }
      else 
        if (corner == JCV_CORNER_BOTTOM_RIGHT) {
          /*@ assert rte_62: mem_access: \valid_read(&max->x); */
          p.x = max->x;
          /*@ assert rte_63: mem_access: \valid_read(&min->y); */
          p.y = min->y;
        }
        else {
          p.x = JCV_INVALID_VALUE;
          p.y = JCV_INVALID_VALUE;
        }
  return p;
}

jcv_real jcv_point_dist_sq(jcv_point const *pt1, jcv_point const *pt2)
{
  jcv_real __retres;
  /*@ assert rte_64: mem_access: \valid_read(&pt1->x); */
  /*@ assert rte_65: mem_access: \valid_read(&pt2->x); */
  /*@ assert rte_66: is_nan_or_infinite: \is_finite(\sub_float(pt1->x, pt2->x)); */
  jcv_real diffx = pt1->x - pt2->x;
  /*@ assert rte_67: mem_access: \valid_read(&pt1->y); */
  /*@ assert rte_68: mem_access: \valid_read(&pt2->y); */
  /*@ assert rte_69: is_nan_or_infinite: \is_finite(\sub_float(pt1->y, pt2->y)); */
  jcv_real diffy = pt1->y - pt2->y;
  /*@ assert rte_70: is_nan_or_infinite: \is_finite(\mul_float(diffx, diffx)); */
  /*@ assert rte_71: is_nan_or_infinite: \is_finite(\mul_float(diffy, diffy)); */
  /*@ assert rte_72: is_nan_or_infinite: \is_finite(\add_float(\mul_float(diffx, diffx), \mul_float(diffy, diffy))); */
  __retres = diffx * diffx + diffy * diffy;
  return __retres;
}

void jcv_halfedge_link(jcv_halfedge *edge, jcv_halfedge *newedge)
{
  /*@ assert rte_7: mem_access: \valid(&newedge->left); */
  newedge->left = edge;
  /*@ assert rte_8: mem_access: \valid(&newedge->right); */
  /*@ assert rte_9: mem_access: \valid_read(&edge->right); */
  newedge->right = edge->right;
  /*@ assert rte_10: mem_access: \valid(&(edge->right)->left); */
  /*@ assert rte_11: mem_access: \valid_read(&edge->right); */
  (edge->right)->left = newedge;
  /*@ assert rte_12: mem_access: \valid(&edge->right); */
  edge->right = newedge;
  return;
}

void jcv_halfedge_unlink(jcv_halfedge *he)
{
  /*@ assert rte_13: mem_access: \valid(&(he->left)->right); */
  /*@ assert rte_14: mem_access: \valid_read(&he->left); */
  /*@ assert rte_15: mem_access: \valid_read(&he->right); */
  (he->left)->right = he->right;
  /*@ assert rte_16: mem_access: \valid(&(he->right)->left); */
  /*@ assert rte_17: mem_access: \valid_read(&he->right); */
  /*@ assert rte_18: mem_access: \valid_read(&he->left); */
  (he->right)->left = he->left;
  /*@ assert rte_19: mem_access: \valid(&he->left); */
  he->left = (struct jcv_halfedge_ *)0;
  /*@ assert rte_20: mem_access: \valid(&he->right); */
  he->right = (struct jcv_halfedge_ *)0;
  return;
}

jcv_site *jcv_halfedge_leftsite(jcv_halfedge const *he)
{
  jcv_site *__retres;
  /*@ assert rte_73: mem_access: \valid_read(&he->edge); */
  /*@ assert rte_74: mem_access: \valid_read(&he->direction); */
  /*@ assert rte_75: index_bound: 0 <= he->direction; */
  /*@ assert rte_76: index_bound: he->direction < 2; */
  /*@ assert rte_77: mem_access: \valid_read(&(he->edge)->sites[he->direction]); */
  __retres = (he->edge)->sites[he->direction];
  return __retres;
}

jcv_site *jcv_halfedge_rightsite(jcv_halfedge const *he)
{
  jcv_site *tmp;
  /*@ assert rte_78: mem_access: \valid_read(&he->edge); */
  if (he->edge) 
    /*@ assert rte_79: mem_access: \valid_read(&he->edge); */
    /*@ assert rte_80: mem_access: \valid_read(&he->direction); */
    /*@ assert rte_81: signed_overflow: 1 - he->direction <= 2147483647; */
    /*@ assert rte_82: index_bound: 0 <= (int)(1 - he->direction); */
    /*@ assert rte_83: index_bound: (int)(1 - he->direction) < 2; */
    /*@ assert rte_84: mem_access: \valid_read(&(he->edge)->sites[(int)(1 - he->direction)]); */
    tmp = (he->edge)->sites[1 - he->direction];
  else tmp = (jcv_site *)0;
  return tmp;
}

int jcv_halfedge_compare(jcv_halfedge const *he1, jcv_halfedge const *he2)
{
  int tmp;
  /*@ assert rte_1: mem_access: \valid_read(&he1->y); */
  /*@ assert rte_2: mem_access: \valid_read(&he2->y); */
  if (he1->y == he2->y) 
    /*@ assert rte_3: mem_access: \valid_read(&he1->vertex.x); */
    /*@ assert rte_4: mem_access: \valid_read(&he2->vertex.x); */
    tmp = he1->vertex.x > he2->vertex.x;
  else 
    /*@ assert rte_5: mem_access: \valid_read(&he1->y); */
    /*@ assert rte_6: mem_access: \valid_read(&he2->y); */
    tmp = he1->y > he2->y;
  return tmp;
}

int main(void)
{
  int __retres;
  int test_count = 0;
  {
    jcv_real abs_outputs[6];
    /*@ assert rte_85: is_nan_or_infinite: \is_finite(5.0f); */
    /*@ assert rte_86: is_nan_or_infinite: \is_finite(0.0f); */
    /*@ assert rte_87: is_nan_or_infinite: \is_finite(3.14159f); */
    jcv_real abs_inputs[6] =
      {5.0f, - 5.0f, 0.0f, - 0.0f, 3.14159f, - 3.14159f};
    abs_outputs[0] = jcv_abs(abs_inputs[0]);
    abs_outputs[1] = jcv_abs(abs_inputs[1]);
    abs_outputs[2] = jcv_abs(abs_inputs[2]);
    abs_outputs[3] = jcv_abs(abs_inputs[3]);
    abs_outputs[4] = jcv_abs(abs_inputs[4]);
    abs_outputs[5] = jcv_abs(abs_inputs[5]);
    /*@ assert rte_88: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_89: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_90: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_91: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_92: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_93: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_94: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_95: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_96: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    int eq_outputs[6];
    /*@ assert rte_97: is_nan_or_infinite: \is_finite(1.0f); */
    jcv_real eq_inputs_a[6] = {1.0f, 1.0f, 1.0f, 0.0f, - 1.0f, 1.0f};
    /*@ assert rte_98: is_nan_or_infinite: \is_finite(\mul_double((double)0x1p-23, (double)2)); */
    /*@ assert rte_99: is_nan_or_infinite: \is_finite(\add_double((double)(1.0f), \mul_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_100: is_nan_or_infinite: \is_finite((jcv_real)\add_double((double)(1.0f), \mul_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_101: is_nan_or_infinite: \is_finite(\mul_double((double)0x1p-23, (double)2)); */
    /*@ assert rte_102: is_nan_or_infinite: \is_finite(\add_double((double)(1.0f), \mul_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_103: is_nan_or_infinite: \is_finite((jcv_real)\add_double((double)(1.0f), \mul_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_104: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_105: is_nan_or_infinite: \is_finite(1.0f); */
    jcv_real eq_inputs_b[6] =
      {1.0f,
       (jcv_real)((double)1.0f + 0x1p-23 * (double)2),
       (jcv_real)((double)1.0f + 0x1p-23 * (double)2),
       0.0f,
       - 1.0f,
       - 1.0f};
    eq_outputs[0] = jcv_real_eq(eq_inputs_a[0],eq_inputs_b[0]);
    eq_outputs[1] = jcv_real_eq(eq_inputs_a[1],eq_inputs_b[1]);
    eq_outputs[2] = jcv_real_eq(eq_inputs_a[2],eq_inputs_b[2]);
    eq_outputs[3] = jcv_real_eq(eq_inputs_a[3],eq_inputs_b[3]);
    eq_outputs[4] = jcv_real_eq(eq_inputs_a[4],eq_inputs_b[4]);
    eq_outputs[5] = jcv_real_eq(eq_inputs_a[5],eq_inputs_b[5]);
    /*@ assert rte_106: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_107: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_108: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_109: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_110: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_111: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_112: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_113: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_114: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_real toint_outputs[9];
    /*@ assert rte_115: is_nan_or_infinite: \is_finite(5.7f); */
    /*@ assert rte_116: is_nan_or_infinite: \is_finite(100.3f); */
    /*@ assert rte_117: is_nan_or_infinite: \is_finite(3.14159f); */
    /*@ assert rte_118: is_nan_or_infinite: \is_finite(0.1f); */
    jcv_real toint_inputs[9] =
      {5.7f,
       - 5.7f,
       0.0f,
       100.3f,
       - 100.3f,
       3.14159f,
       - 3.14159f,
       0.1f,
       - 0.1f};
    toint_outputs[0] = jcv_real_to_int(toint_inputs[0]);
    toint_outputs[1] = jcv_real_to_int(toint_inputs[1]);
    toint_outputs[2] = jcv_real_to_int(toint_inputs[2]);
    toint_outputs[3] = jcv_real_to_int(toint_inputs[3]);
    toint_outputs[4] = jcv_real_to_int(toint_inputs[4]);
    toint_outputs[5] = jcv_real_to_int(toint_inputs[5]);
    toint_outputs[6] = jcv_real_to_int(toint_inputs[6]);
    toint_outputs[7] = jcv_real_to_int(toint_inputs[7]);
    toint_outputs[8] = jcv_real_to_int(toint_inputs[8]);
    /*@ assert rte_119: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_120: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_121: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_122: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_123: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_124: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_125: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_126: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_127: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_128: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_129: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_130: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_real floor_outputs[12];
    /*@ assert rte_131: is_nan_or_infinite: \is_finite(5.7f); */
    /*@ assert rte_132: is_nan_or_infinite: \is_finite(5.0f); */
    /*@ assert rte_133: is_nan_or_infinite: \is_finite(0.1f); */
    /*@ assert rte_134: is_nan_or_infinite: \is_finite(3.14159f); */
    /*@ assert rte_135: is_nan_or_infinite: \is_finite(100.3f); */
    jcv_real floor_inputs[12] =
      {5.7f,
       5.0f,
       5.1f,
       - 5.7f,
       - 5.0f,
       0.0f,
       0.1f,
       - 0.1f,
       3.14159f,
       - 3.14159f,
       100.3f,
       - 100.3f};
    floor_outputs[0] = jcv_floor(floor_inputs[0]);
    floor_outputs[1] = jcv_floor(floor_inputs[1]);
    floor_outputs[2] = jcv_floor(floor_inputs[2]);
    floor_outputs[3] = jcv_floor(floor_inputs[3]);
    floor_outputs[4] = jcv_floor(floor_inputs[4]);
    floor_outputs[5] = jcv_floor(floor_inputs[5]);
    floor_outputs[6] = jcv_floor(floor_inputs[6]);
    floor_outputs[7] = jcv_floor(floor_inputs[7]);
    floor_outputs[8] = jcv_floor(floor_inputs[8]);
    floor_outputs[9] = jcv_floor(floor_inputs[9]);
    floor_outputs[10] = jcv_floor(floor_inputs[10]);
    floor_outputs[11] = jcv_floor(floor_inputs[11]);
    /*@ assert rte_136: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_137: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_138: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_139: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_140: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_141: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_142: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_143: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_144: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_145: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_146: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_147: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_148: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_149: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_150: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_real ceil_outputs[12];
    /*@ assert rte_151: is_nan_or_infinite: \is_finite(5.7f); */
    /*@ assert rte_152: is_nan_or_infinite: \is_finite(5.0f); */
    /*@ assert rte_153: is_nan_or_infinite: \is_finite(0.1f); */
    /*@ assert rte_154: is_nan_or_infinite: \is_finite(3.14159f); */
    /*@ assert rte_155: is_nan_or_infinite: \is_finite(100.3f); */
    jcv_real ceil_inputs[12] =
      {5.7f,
       5.0f,
       5.1f,
       - 5.7f,
       - 5.0f,
       0.0f,
       0.1f,
       - 0.1f,
       3.14159f,
       - 3.14159f,
       100.3f,
       - 100.3f};
    ceil_outputs[0] = jcv_ceil(ceil_inputs[0]);
    ceil_outputs[1] = jcv_ceil(ceil_inputs[1]);
    ceil_outputs[2] = jcv_ceil(ceil_inputs[2]);
    ceil_outputs[3] = jcv_ceil(ceil_inputs[3]);
    ceil_outputs[4] = jcv_ceil(ceil_inputs[4]);
    ceil_outputs[5] = jcv_ceil(ceil_inputs[5]);
    ceil_outputs[6] = jcv_ceil(ceil_inputs[6]);
    ceil_outputs[7] = jcv_ceil(ceil_inputs[7]);
    ceil_outputs[8] = jcv_ceil(ceil_inputs[8]);
    ceil_outputs[9] = jcv_ceil(ceil_inputs[9]);
    ceil_outputs[10] = jcv_ceil(ceil_inputs[10]);
    ceil_outputs[11] = jcv_ceil(ceil_inputs[11]);
    /*@ assert rte_156: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_157: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_158: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_159: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_160: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_161: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_162: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_163: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_164: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_165: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_166: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_167: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_168: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_169: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_170: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_real min_outputs[12];
    /*@ assert rte_171: is_nan_or_infinite: \is_finite(5.0f); */
    /*@ assert rte_172: is_nan_or_infinite: \is_finite(3.14159f); */
    /*@ assert rte_173: is_nan_or_infinite: \is_finite(100.3f); */
    jcv_real min_inputs_a[12] =
      {5.0f,
       3.0f,
       5.0f,
       - 5.0f,
       5.0f,
       0.0f,
       0.0f,
       1.0f,
       3.14159f,
       - 3.14159f,
       100.3f,
       - 100.3f};
    /*@ assert rte_174: is_nan_or_infinite: \is_finite(3.0f); */
    /*@ assert rte_175: is_nan_or_infinite: \is_finite(2.71828f); */
    /*@ assert rte_176: is_nan_or_infinite: \is_finite(50.7f); */
    jcv_real min_inputs_b[12] =
      {3.0f,
       5.0f,
       5.0f,
       3.0f,
       - 3.0f,
       0.0f,
       1.0f,
       0.0f,
       2.71828f,
       - 2.71828f,
       50.7f,
       - 50.7f};
    min_outputs[0] = jcv_min(min_inputs_a[0],min_inputs_b[0]);
    min_outputs[1] = jcv_min(min_inputs_a[1],min_inputs_b[1]);
    min_outputs[2] = jcv_min(min_inputs_a[2],min_inputs_b[2]);
    min_outputs[3] = jcv_min(min_inputs_a[3],min_inputs_b[3]);
    min_outputs[4] = jcv_min(min_inputs_a[4],min_inputs_b[4]);
    min_outputs[5] = jcv_min(min_inputs_a[5],min_inputs_b[5]);
    min_outputs[6] = jcv_min(min_inputs_a[6],min_inputs_b[6]);
    min_outputs[7] = jcv_min(min_inputs_a[7],min_inputs_b[7]);
    min_outputs[8] = jcv_min(min_inputs_a[8],min_inputs_b[8]);
    min_outputs[9] = jcv_min(min_inputs_a[9],min_inputs_b[9]);
    min_outputs[10] = jcv_min(min_inputs_a[10],min_inputs_b[10]);
    min_outputs[11] = jcv_min(min_inputs_a[11],min_inputs_b[11]);
    /*@ assert rte_177: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_178: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_179: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_180: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_181: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_182: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_183: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_184: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_185: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_186: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_187: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_188: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_189: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_190: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_191: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_192: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_real max_outputs[12];
    /*@ assert rte_193: is_nan_or_infinite: \is_finite(5.0f); */
    /*@ assert rte_194: is_nan_or_infinite: \is_finite(3.14159f); */
    /*@ assert rte_195: is_nan_or_infinite: \is_finite(100.3f); */
    jcv_real max_inputs_a[12] =
      {5.0f,
       3.0f,
       5.0f,
       - 5.0f,
       5.0f,
       0.0f,
       0.0f,
       1.0f,
       3.14159f,
       - 3.14159f,
       100.3f,
       - 100.3f};
    /*@ assert rte_196: is_nan_or_infinite: \is_finite(3.0f); */
    /*@ assert rte_197: is_nan_or_infinite: \is_finite(2.71828f); */
    /*@ assert rte_198: is_nan_or_infinite: \is_finite(50.7f); */
    jcv_real max_inputs_b[12] =
      {3.0f,
       5.0f,
       5.0f,
       3.0f,
       - 3.0f,
       0.0f,
       1.0f,
       0.0f,
       2.71828f,
       - 2.71828f,
       50.7f,
       - 50.7f};
    max_outputs[0] = jcv_max(max_inputs_a[0],max_inputs_b[0]);
    max_outputs[1] = jcv_max(max_inputs_a[1],max_inputs_b[1]);
    max_outputs[2] = jcv_max(max_inputs_a[2],max_inputs_b[2]);
    max_outputs[3] = jcv_max(max_inputs_a[3],max_inputs_b[3]);
    max_outputs[4] = jcv_max(max_inputs_a[4],max_inputs_b[4]);
    max_outputs[5] = jcv_max(max_inputs_a[5],max_inputs_b[5]);
    max_outputs[6] = jcv_max(max_inputs_a[6],max_inputs_b[6]);
    max_outputs[7] = jcv_max(max_inputs_a[7],max_inputs_b[7]);
    max_outputs[8] = jcv_max(max_inputs_a[8],max_inputs_b[8]);
    max_outputs[9] = jcv_max(max_inputs_a[9],max_inputs_b[9]);
    max_outputs[10] = jcv_max(max_inputs_a[10],max_inputs_b[10]);
    max_outputs[11] = jcv_max(max_inputs_a[11],max_inputs_b[11]);
    /*@ assert rte_199: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_200: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_201: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_202: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_203: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_204: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_205: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_206: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_207: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_208: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_209: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_210: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_211: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_212: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_213: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_214: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    int cmp_outputs[8];
    /*@ assert rte_215: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_216: is_nan_or_infinite: \is_finite(2.0f); */
    jcv_point cmp_inputs_a[8] =
      {{.x = 1.0f, .y = 2.0f},
       {.x = 3.0f, .y = 4.0f},
       {.x = 1.0f, .y = 4.0f},
       {.x = 3.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = - 1.0f, .y = - 2.0f},
       {.x = 5.0f, .y = 3.0f}};
    /*@ assert rte_217: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_218: is_nan_or_infinite: \is_finite(2.0f); */
    jcv_point cmp_inputs_b[8] =
      {{.x = 3.0f, .y = 4.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = - 1.0f, .y = - 2.0f},
       {.x = 3.0f, .y = 5.0f}};
    cmp_outputs[0] = jcv_point_cmp((jcv_point const *)(cmp_inputs_a),
                                   (jcv_point const *)(cmp_inputs_b));
    cmp_outputs[1] = jcv_point_cmp((jcv_point const *)(& cmp_inputs_a[1]),
                                   (jcv_point const *)(& cmp_inputs_b[1]));
    cmp_outputs[2] = jcv_point_cmp((jcv_point const *)(& cmp_inputs_a[2]),
                                   (jcv_point const *)(& cmp_inputs_b[2]));
    cmp_outputs[3] = jcv_point_cmp((jcv_point const *)(& cmp_inputs_a[3]),
                                   (jcv_point const *)(& cmp_inputs_b[3]));
    cmp_outputs[4] = jcv_point_cmp((jcv_point const *)(& cmp_inputs_a[4]),
                                   (jcv_point const *)(& cmp_inputs_b[4]));
    cmp_outputs[5] = jcv_point_cmp((jcv_point const *)(& cmp_inputs_a[5]),
                                   (jcv_point const *)(& cmp_inputs_b[5]));
    cmp_outputs[6] = jcv_point_cmp((jcv_point const *)(& cmp_inputs_a[6]),
                                   (jcv_point const *)(& cmp_inputs_b[6]));
    cmp_outputs[7] = jcv_point_cmp((jcv_point const *)(& cmp_inputs_a[7]),
                                   (jcv_point const *)(& cmp_inputs_b[7]));
    /*@ assert rte_219: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_220: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_221: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_222: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_223: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_224: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_225: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_226: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_227: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_228: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_229: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    int less_outputs[8];
    /*@ assert rte_230: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_231: is_nan_or_infinite: \is_finite(2.0f); */
    jcv_point less_inputs_a[8] =
      {{.x = 1.0f, .y = 2.0f},
       {.x = 3.0f, .y = 4.0f},
       {.x = 1.0f, .y = 4.0f},
       {.x = 3.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = - 1.0f, .y = - 2.0f},
       {.x = 5.0f, .y = 3.0f}};
    /*@ assert rte_232: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_233: is_nan_or_infinite: \is_finite(2.0f); */
    jcv_point less_inputs_b[8] =
      {{.x = 3.0f, .y = 4.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = - 1.0f, .y = - 2.0f},
       {.x = 3.0f, .y = 5.0f}};
    less_outputs[0] = jcv_point_less((jcv_point const *)(less_inputs_a),
                                     (jcv_point const *)(less_inputs_b));
    less_outputs[1] = jcv_point_less((jcv_point const *)(& less_inputs_a[1]),
                                     (jcv_point const *)(& less_inputs_b[1]));
    less_outputs[2] = jcv_point_less((jcv_point const *)(& less_inputs_a[2]),
                                     (jcv_point const *)(& less_inputs_b[2]));
    less_outputs[3] = jcv_point_less((jcv_point const *)(& less_inputs_a[3]),
                                     (jcv_point const *)(& less_inputs_b[3]));
    less_outputs[4] = jcv_point_less((jcv_point const *)(& less_inputs_a[4]),
                                     (jcv_point const *)(& less_inputs_b[4]));
    less_outputs[5] = jcv_point_less((jcv_point const *)(& less_inputs_a[5]),
                                     (jcv_point const *)(& less_inputs_b[5]));
    less_outputs[6] = jcv_point_less((jcv_point const *)(& less_inputs_a[6]),
                                     (jcv_point const *)(& less_inputs_b[6]));
    less_outputs[7] = jcv_point_less((jcv_point const *)(& less_inputs_a[7]),
                                     (jcv_point const *)(& less_inputs_b[7]));
    /*@ assert rte_234: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_235: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_236: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_237: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_238: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_239: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_240: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_241: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    int eq_outputs_0[8];
    /*@ assert rte_242: is_nan_or_infinite: \is_finite(\div_double((double)0x1p-23, (double)2)); */
    /*@ assert rte_243: is_nan_or_infinite: \is_finite(\add_double((double)(1.0f), \div_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_244: is_nan_or_infinite: \is_finite((jcv_real)\add_double((double)(1.0f), \div_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_245: is_nan_or_infinite: \is_finite(\div_double((double)0x1p-23, (double)2)); */
    /*@ assert rte_246: is_nan_or_infinite: \is_finite(\add_double((double)(2.0f), \div_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_247: is_nan_or_infinite: \is_finite((jcv_real)\add_double((double)(2.0f), \div_double((double)0x1p-23, (double)2))); */
    /*@ assert rte_248: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_249: is_nan_or_infinite: \is_finite(2.0f); */
    jcv_point eq_inputs_a_0[8] =
      {{.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = (jcv_real)((double)1.0f + 0x1p-23 / (double)2), .y = 2.0f},
       {.x = 1.0f, .y = (jcv_real)((double)2.0f + 0x1p-23 / (double)2)},
       {.x = 3.0f, .y = 4.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = - 1.0f, .y = - 2.0f},
       {.x = 5.0f, .y = 3.0f}};
    /*@ assert rte_250: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_251: is_nan_or_infinite: \is_finite(2.0f); */
    jcv_point eq_inputs_b_0[8] =
      {{.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 1.0f, .y = 2.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = - 1.0f, .y = - 2.0f},
       {.x = 3.0f, .y = 5.0f}};
    eq_outputs_0[0] = jcv_point_eq((jcv_point const *)(eq_inputs_a_0),
                                   (jcv_point const *)(eq_inputs_b_0));
    eq_outputs_0[1] = jcv_point_eq((jcv_point const *)(& eq_inputs_a_0[1]),
                                   (jcv_point const *)(& eq_inputs_b_0[1]));
    eq_outputs_0[2] = jcv_point_eq((jcv_point const *)(& eq_inputs_a_0[2]),
                                   (jcv_point const *)(& eq_inputs_b_0[2]));
    eq_outputs_0[3] = jcv_point_eq((jcv_point const *)(& eq_inputs_a_0[3]),
                                   (jcv_point const *)(& eq_inputs_b_0[3]));
    eq_outputs_0[4] = jcv_point_eq((jcv_point const *)(& eq_inputs_a_0[4]),
                                   (jcv_point const *)(& eq_inputs_b_0[4]));
    eq_outputs_0[5] = jcv_point_eq((jcv_point const *)(& eq_inputs_a_0[5]),
                                   (jcv_point const *)(& eq_inputs_b_0[5]));
    eq_outputs_0[6] = jcv_point_eq((jcv_point const *)(& eq_inputs_a_0[6]),
                                   (jcv_point const *)(& eq_inputs_b_0[6]));
    eq_outputs_0[7] = jcv_point_eq((jcv_point const *)(& eq_inputs_a_0[7]),
                                   (jcv_point const *)(& eq_inputs_b_0[7]));
    /*@ assert rte_252: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_253: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_254: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    int box_outputs[12];
    jcv_point box_min = {.x = 0.0f, .y = 0.0f};
    jcv_point box_max = {.x = 10.0f, .y = 10.0f};
    /*@ assert rte_255: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_256: is_nan_or_infinite: \is_finite(1.0f); */
    jcv_point box_inputs[12] =
      {{.x = 0.0f, .y = 5.0f},
       {.x = 10.0f, .y = 5.0f},
       {.x = 5.0f, .y = 0.0f},
       {.x = 5.0f, .y = 10.0f},
       {.x = 5.0f, .y = 5.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 10.0f, .y = 10.0f},
       {.x = - 1.0f, .y = 5.0f},
       {.x = 0.0f, .y = 10.0f},
       {.x = 10.0f, .y = 0.0f},
       {.x = 5.0f, .y = - 1.0f},
       {.x = 5.0f, .y = 11.0f}};
    box_outputs[0] = jcv_point_on_box_edge((jcv_point const *)(box_inputs),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[1] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[1]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[2] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[2]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[3] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[3]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[4] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[4]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[5] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[5]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[6] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[6]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[7] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[7]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[8] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[8]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[9] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[9]),
                                           (jcv_point const *)(& box_min),
                                           (jcv_point const *)(& box_max));
    box_outputs[10] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[10]),
                                            (jcv_point const *)(& box_min),
                                            (jcv_point const *)(& box_max));
    box_outputs[11] = jcv_point_on_box_edge((jcv_point const *)(& box_inputs[11]),
                                            (jcv_point const *)(& box_min),
                                            (jcv_point const *)(& box_max));
    /*@ assert rte_257: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_258: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_259: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_260: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_261: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    int corner_outputs[14];
    int corner_inputs[14] =
      {JCV_CORNER_TOP_LEFT,
       JCV_CORNER_TOP_RIGHT,
       JCV_CORNER_BOTTOM_LEFT,
       JCV_CORNER_BOTTOM_RIGHT,
       JCV_CORNER_NONE,
       5,
       -1,
       0,
       1,
       2,
       3,
       4,
       6,
       10};
    corner_outputs[0] = jcv_is_corner(corner_inputs[0]);
    corner_outputs[1] = jcv_is_corner(corner_inputs[1]);
    corner_outputs[2] = jcv_is_corner(corner_inputs[2]);
    corner_outputs[3] = jcv_is_corner(corner_inputs[3]);
    corner_outputs[4] = jcv_is_corner(corner_inputs[4]);
    corner_outputs[5] = jcv_is_corner(corner_inputs[5]);
    corner_outputs[6] = jcv_is_corner(corner_inputs[6]);
    corner_outputs[7] = jcv_is_corner(corner_inputs[7]);
    corner_outputs[8] = jcv_is_corner(corner_inputs[8]);
    corner_outputs[9] = jcv_is_corner(corner_inputs[9]);
    corner_outputs[10] = jcv_is_corner(corner_inputs[10]);
    corner_outputs[11] = jcv_is_corner(corner_inputs[11]);
    corner_outputs[12] = jcv_is_corner(corner_inputs[12]);
    corner_outputs[13] = jcv_is_corner(corner_inputs[13]);
    /*@ assert rte_262: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_263: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_264: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_265: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    int rotate_outputs[14];
    int rotate_inputs[14] =
      {JCV_CORNER_TOP_LEFT,
       JCV_CORNER_TOP_RIGHT,
       JCV_CORNER_BOTTOM_RIGHT,
       JCV_CORNER_BOTTOM_LEFT,
       JCV_CORNER_NONE,
       5,
       1,
       4,
       2,
       3,
       6,
       10,
       0,
       -1};
    rotate_outputs[0] = jcv_corner_rotate_90(rotate_inputs[0]);
    rotate_outputs[1] = jcv_corner_rotate_90(rotate_inputs[1]);
    rotate_outputs[2] = jcv_corner_rotate_90(rotate_inputs[2]);
    rotate_outputs[3] = jcv_corner_rotate_90(rotate_inputs[3]);
    rotate_outputs[4] = jcv_corner_rotate_90(rotate_inputs[4]);
    rotate_outputs[5] = jcv_corner_rotate_90(rotate_inputs[5]);
    rotate_outputs[6] = jcv_corner_rotate_90(rotate_inputs[6]);
    rotate_outputs[7] = jcv_corner_rotate_90(rotate_inputs[7]);
    rotate_outputs[8] = jcv_corner_rotate_90(rotate_inputs[8]);
    rotate_outputs[9] = jcv_corner_rotate_90(rotate_inputs[9]);
    rotate_outputs[10] = jcv_corner_rotate_90(rotate_inputs[10]);
    rotate_outputs[11] = jcv_corner_rotate_90(rotate_inputs[11]);
    rotate_outputs[12] = jcv_corner_rotate_90(rotate_inputs[12]);
    rotate_outputs[13] = jcv_corner_rotate_90(rotate_inputs[13]);
    /*@ assert rte_266: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_267: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_268: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_269: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_270: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_271: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_272: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_273: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_274: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_275: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_276: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_277: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_278: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_point corner_outputs_0[14];
    jcv_point corner_min = {.x = 0.0f, .y = 0.0f};
    jcv_point corner_max = {.x = 10.0f, .y = 10.0f};
    int corner_inputs_0[14] =
      {JCV_CORNER_TOP_LEFT,
       JCV_CORNER_TOP_RIGHT,
       JCV_CORNER_BOTTOM_LEFT,
       JCV_CORNER_BOTTOM_RIGHT,
       JCV_CORNER_NONE,
       5,
       -1,
       0,
       1,
       2,
       3,
       4,
       6,
       10};
    corner_outputs_0[0] = jcv_corner_to_point(corner_inputs_0[0],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[1] = jcv_corner_to_point(corner_inputs_0[1],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[2] = jcv_corner_to_point(corner_inputs_0[2],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[3] = jcv_corner_to_point(corner_inputs_0[3],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[4] = jcv_corner_to_point(corner_inputs_0[4],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[5] = jcv_corner_to_point(corner_inputs_0[5],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[6] = jcv_corner_to_point(corner_inputs_0[6],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[7] = jcv_corner_to_point(corner_inputs_0[7],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[8] = jcv_corner_to_point(corner_inputs_0[8],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[9] = jcv_corner_to_point(corner_inputs_0[9],
                                              (jcv_point const *)(& corner_min),
                                              (jcv_point const *)(& corner_max));
    corner_outputs_0[10] = jcv_corner_to_point(corner_inputs_0[10],
                                               (jcv_point const *)(& corner_min),
                                               (jcv_point const *)(& corner_max));
    corner_outputs_0[11] = jcv_corner_to_point(corner_inputs_0[11],
                                               (jcv_point const *)(& corner_min),
                                               (jcv_point const *)(& corner_max));
    corner_outputs_0[12] = jcv_corner_to_point(corner_inputs_0[12],
                                               (jcv_point const *)(& corner_min),
                                               (jcv_point const *)(& corner_max));
    corner_outputs_0[13] = jcv_corner_to_point(corner_inputs_0[13],
                                               (jcv_point const *)(& corner_min),
                                               (jcv_point const *)(& corner_max));
    /*@ assert rte_279: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_280: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_281: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_282: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_283: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_284: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_285: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_286: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_287: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_288: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_289: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_290: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_291: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_292: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_real dist_outputs[12];
    /*@ assert rte_293: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_294: is_nan_or_infinite: \is_finite(1.0f); */
    /*@ assert rte_295: is_nan_or_infinite: \is_finite(3.0f); */
    jcv_point dist_inputs_a[12] =
      {{.x = 0.0f, .y = 0.0f},
       {.x = 3.0f, .y = 4.0f},
       {.x = 1.0f, .y = 1.0f},
       {.x = 2.0f, .y = 2.0f},
       {.x = - 1.0f, .y = - 1.0f},
       {.x = 1.0f, .y = 0.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 5.0f, .y = 12.0f},
       {.x = - 3.0f, .y = 4.0f},
       {.x = 1.5f, .y = 2.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 2.0f, .y = 3.0f}};
    jcv_point dist_inputs_b[12] =
      {{.x = 3.0f, .y = 4.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 2.0f, .y = 2.0f},
       {.x = 1.0f, .y = 1.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 0.0f, .y = 0.0f},
       {.x = 1.5f, .y = 2.0f},
       {.x = 1.0f, .y = 1.0f},
       {.x = 5.0f, .y = 7.0f}};
    dist_outputs[0] = jcv_point_dist_sq((jcv_point const *)(dist_inputs_a),
                                        (jcv_point const *)(dist_inputs_b));
    dist_outputs[1] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[1]),
                                        (jcv_point const *)(& dist_inputs_b[1]));
    dist_outputs[2] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[2]),
                                        (jcv_point const *)(& dist_inputs_b[2]));
    dist_outputs[3] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[3]),
                                        (jcv_point const *)(& dist_inputs_b[3]));
    dist_outputs[4] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[4]),
                                        (jcv_point const *)(& dist_inputs_b[4]));
    dist_outputs[5] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[5]),
                                        (jcv_point const *)(& dist_inputs_b[5]));
    dist_outputs[6] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[6]),
                                        (jcv_point const *)(& dist_inputs_b[6]));
    dist_outputs[7] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[7]),
                                        (jcv_point const *)(& dist_inputs_b[7]));
    dist_outputs[8] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[8]),
                                        (jcv_point const *)(& dist_inputs_b[8]));
    dist_outputs[9] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[9]),
                                        (jcv_point const *)(& dist_inputs_b[9]));
    dist_outputs[10] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[10]),
                                         (jcv_point const *)(& dist_inputs_b[10]));
    dist_outputs[11] = jcv_point_dist_sq((jcv_point const *)(& dist_inputs_a[11]),
                                         (jcv_point const *)(& dist_inputs_b[11]));
    /*@ assert rte_296: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_297: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_298: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_299: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_300: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_301: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_302: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_303: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_304: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_305: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_306: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_307: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_308: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_309: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    /*@ assert rte_310: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  {
    jcv_halfedge he;
    jcv_edge edge;
    jcv_site site1;
    jcv_site site2;
    site1.index = 1;
    site2.index = 2;
    edge.sites[0] = & site1;
    edge.sites[1] = & site2;
    he.edge = & edge;
    he.direction = JCV_DIRECTION_LEFT;
    jcv_site *left_result1 =
      jcv_halfedge_leftsite((jcv_halfedge const *)(& he));
    /*@ assert rte_311: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    jcv_site *right_result1 =
      jcv_halfedge_rightsite((jcv_halfedge const *)(& he));
    /*@ assert rte_312: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    he.direction = JCV_DIRECTION_RIGHT;
    jcv_site *left_result2 =
      jcv_halfedge_leftsite((jcv_halfedge const *)(& he));
    /*@ assert rte_313: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    jcv_site *right_result2 =
      jcv_halfedge_rightsite((jcv_halfedge const *)(& he));
    /*@ assert rte_314: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    he.edge = (jcv_edge *)0;
    jcv_site *right_result3 =
      jcv_halfedge_rightsite((jcv_halfedge const *)(& he));
    /*@ assert rte_315: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    he.edge = & edge;
    he.direction = JCV_DIRECTION_LEFT;
    edge.sites[0] = & site1;
    edge.sites[1] = & site1;
    jcv_site *left_result3 =
      jcv_halfedge_leftsite((jcv_halfedge const *)(& he));
    /*@ assert rte_316: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
    jcv_site *right_result4 =
      jcv_halfedge_rightsite((jcv_halfedge const *)(& he));
    /*@ assert rte_317: signed_overflow: test_count + 1 <= 2147483647; */
    test_count ++;
  }
  __retres = 0;
  return __retres;
}


