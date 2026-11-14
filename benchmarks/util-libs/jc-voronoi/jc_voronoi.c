#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <assert.h>


#define JCV_REAL_TYPE float

#define JCV_REAL_TYPE_EPSILON FLT_EPSILON

#define JCV_PI 3.14159265358979323846264338327950288f

#define JCV_FLT_MAX 3.402823466e+38F

#define JCV_EDGE_INTERSECT_THRESHOLD 1.0e-10F


typedef JCV_REAL_TYPE jcv_real;

typedef struct jcv_point_           jcv_point;
typedef struct jcv_rect_            jcv_rect;
typedef struct jcv_site_            jcv_site;
typedef struct jcv_edge_            jcv_edge;
typedef struct jcv_graphedge_       jcv_graphedge;
typedef struct jcv_delauney_edge_   jcv_delauney_edge;
typedef struct jcv_delauney_iter_   jcv_delauney_iter;
typedef struct jcv_diagram_         jcv_diagram;
typedef struct jcv_clipper_         jcv_clipper;
typedef struct jcv_context_internal_ jcv_context_internal;

/// Tests if a point is inside the final shape
typedef int (*jcv_clip_test_point_fn)(const jcv_clipper* clipper, const jcv_point p);
/** Given an edge, and the clipper, calculates the e->pos[0] and e->pos[1]
 * Returns 0 if not successful
 */
typedef int (*jcv_clip_edge_fn)(const jcv_clipper* clipper, jcv_edge* e);
/** Given the clipper, the site and the last edge,
 * closes any gaps in the polygon by adding new edges that follow the bounding shape
 * The internal context is use when allocating new edges.
 */
typedef void (*jcv_clip_fillgap_fn)(const jcv_clipper* clipper, jcv_context_internal* allocator, jcv_site* s);


struct jcv_point_
{
    jcv_real x;
    jcv_real y;
};

struct jcv_graphedge_
{
    struct jcv_graphedge_*  next;
    struct jcv_edge_*       edge;
    struct jcv_site_*       neighbor;
    jcv_point               pos[2];
    jcv_real                angle;
};

struct jcv_site_
{
    jcv_point       p;
    int             index;  // Index into the original list of points
    jcv_graphedge*  edges;  // The half edges owned by the cell
};

// The coefficients a, b and c are from the general line equation: ax * by + c = 0
struct jcv_edge_
{
    struct jcv_edge_*   next;
    jcv_site*           sites[2];
    jcv_point           pos[2];
    jcv_real            a;
    jcv_real            b;
    jcv_real            c;
};

struct jcv_delauney_iter_
{
    const jcv_edge*   sentinel;
    const jcv_edge*   current;
};

struct jcv_delauney_edge_
{
    const jcv_edge* edge;       // The voronoi edge separating the two sites
    const jcv_site* sites[2];
    jcv_point       pos[2];     // the positions of the two sites
};

struct jcv_rect_
{
    jcv_point   min;
    jcv_point   max;
};

struct jcv_clipper_
{
    jcv_clip_test_point_fn  test_fn;
    jcv_clip_edge_fn        clip_fn;
    jcv_clip_fillgap_fn     fill_fn;
    jcv_point               min;        // The bounding rect min
    jcv_point               max;        // The bounding rect max
    void*                   ctx;        // User defined context
};

struct jcv_diagram_
{
    jcv_context_internal*   internal;
    int                     numsites;
    jcv_point               min;
    jcv_point               max;
};


const int JCV_DIRECTION_LEFT  = 0;
const int JCV_DIRECTION_RIGHT = 1;
const jcv_real JCV_INVALID_VALUE = (jcv_real)-JCV_FLT_MAX;


// jcv_real



jcv_real jcv_abs(jcv_real v) {
    return (v < 0) ? -v : v;
}



int jcv_real_eq(jcv_real a, jcv_real b)
{
    return jcv_abs(a - b) < JCV_REAL_TYPE_EPSILON;
}



jcv_real jcv_real_to_int(jcv_real v) {
    return (sizeof(jcv_real) == 4) ? (jcv_real)(int)v : (jcv_real)(long long)v;
}


// Only used for calculating the initial bounding box

jcv_real jcv_floor(jcv_real v) {
    jcv_real i = jcv_real_to_int(v);
    return (v < i) ? i - 1 : i;
}


// Only used for calculating the initial bounding box

jcv_real jcv_ceil(jcv_real v) {
    jcv_real i = jcv_real_to_int(v);
    return (v > i) ? i + 1 : i;
}



jcv_real jcv_min(jcv_real a, jcv_real b) {
    return a < b ? a : b;
}



jcv_real jcv_max(jcv_real a, jcv_real b) {
    return a > b ? a : b;
}



// jcv_point



int jcv_point_cmp(const jcv_point* p1, const jcv_point* p2)
{
    const jcv_point* s1 = (const jcv_point*)p1;
    const jcv_point* s2 = (const jcv_point*)p2;
    return (s1->y != s2->y) ? (s1->y < s2->y ? -1 : 1) : (s1->x < s2->x ? -1 : 1);
}



int jcv_point_less( const jcv_point* pt1, const jcv_point* pt2 )
{
    return (pt1->y == pt2->y) ? (pt1->x < pt2->x) : pt1->y < pt2->y;
}



int jcv_point_eq( const jcv_point* pt1, const jcv_point* pt2 )
{
    return jcv_real_eq(pt1->y, pt2->y) && jcv_real_eq(pt1->x, pt2->x);
}



int jcv_point_on_box_edge( const jcv_point* pt, const jcv_point* min, const jcv_point* max )
{
    return pt->x == min->x || pt->y == min->y || pt->x == max->x || pt->y == max->y;
}


// corners


const int JCV_EDGE_LEFT    = 1;
const int JCV_EDGE_RIGHT   = 2;
const int JCV_EDGE_BOTTOM  = 4;
const int JCV_EDGE_TOP     = 8;

const int JCV_CORNER_NONE          = 0;
const int JCV_CORNER_TOP_LEFT      = 1;
const int JCV_CORNER_BOTTOM_LEFT   = 2;
const int JCV_CORNER_BOTTOM_RIGHT  = 3;
const int JCV_CORNER_TOP_RIGHT     = 4;



int jcv_is_corner(int corner)
{
    return corner != 0;
}



int jcv_corner_rotate_90(int corner)
{
    corner--;
    corner = (corner+1)%4;
    return corner + 1;
}



jcv_point jcv_corner_to_point(int corner, const jcv_point* min, const jcv_point* max )
{
    jcv_point p;
    if      (corner == JCV_CORNER_TOP_LEFT)     { p.x = min->x; p.y = max->y; }
    else if (corner == JCV_CORNER_TOP_RIGHT)    { p.x = max->x; p.y = max->y; }
    else if (corner == JCV_CORNER_BOTTOM_LEFT)  { p.x = min->x; p.y = min->y; }
    else if (corner == JCV_CORNER_BOTTOM_RIGHT) { p.x = max->x; p.y = min->y; }
    else                                        { p.x = JCV_INVALID_VALUE; p.y = JCV_INVALID_VALUE; }
    return p;
}



jcv_real jcv_point_dist_sq( const jcv_point* pt1, const jcv_point* pt2)
{
    jcv_real diffx = pt1->x - pt2->x;
    jcv_real diffy = pt1->y - pt2->y;
    return diffx * diffx + diffy * diffy;
}


struct jcv_halfedge_
{
    jcv_edge*               edge;
    struct jcv_halfedge_*   left;
    struct jcv_halfedge_*   right;
    jcv_point               vertex;
    jcv_real                y;
    int                     direction; // 0=left, 1=right
    int                     pqpos;
};
typedef struct jcv_halfedge_ jcv_halfedge;


// jcv_halfedge



void jcv_halfedge_link(jcv_halfedge* edge, jcv_halfedge* newedge)
{
    newedge->left = edge;
    newedge->right = edge->right;
    edge->right->left = newedge;
    edge->right = newedge;
}



void jcv_halfedge_unlink(jcv_halfedge* he)
{
    he->left->right = he->right;
    he->right->left = he->left;
    he->left  = 0;
    he->right = 0;
}


// void jcv_halfedge_delete(jcv_context_internal* internal, jcv_halfedge* he)
// {
//     he->right = internal->halfedgepool;
//     internal->halfedgepool = he;
// }



jcv_site* jcv_halfedge_leftsite(const jcv_halfedge* he)
{
    return he->edge->sites[he->direction];
}



jcv_site* jcv_halfedge_rightsite(const jcv_halfedge* he)
{
    return he->edge ? he->edge->sites[1 - he->direction] : 0;
}


// Keeps the priority queue sorted with events sorted in ascending order
// Return 1 if the edges needs to be swapped

int jcv_halfedge_compare( const jcv_halfedge* he1, const jcv_halfedge* he2 )
{
	return  (he1->y == he2->y) ? he1->vertex.x > he2->vertex.x : he1->y > he2->y;
}


int main() {
    
    int test_count = 0;
    
    {
        jcv_real abs_inputs[] = {5.0f, -5.0f, 0.0f, -0.0f, 3.14159f, -3.14159f};
        jcv_real abs_outputs[6];
        
        abs_outputs[0] = jcv_abs(abs_inputs[0]);
        abs_outputs[1] = jcv_abs(abs_inputs[1]);
        abs_outputs[2] = jcv_abs(abs_inputs[2]);
        abs_outputs[3] = jcv_abs(abs_inputs[3]);
        abs_outputs[4] = jcv_abs(abs_inputs[4]);
        abs_outputs[5] = jcv_abs(abs_inputs[5]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_real eq_inputs_a[] = {1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f};
        jcv_real eq_inputs_b[] = {1.0f, 1.0f + JCV_REAL_TYPE_EPSILON*2, 1.0f + JCV_REAL_TYPE_EPSILON*2, 0.0f, -1.0f, -1.0f};
        int eq_outputs[6];
        
        // 展开循环
        eq_outputs[0] = jcv_real_eq(eq_inputs_a[0], eq_inputs_b[0]);
        eq_outputs[1] = jcv_real_eq(eq_inputs_a[1], eq_inputs_b[1]);
        eq_outputs[2] = jcv_real_eq(eq_inputs_a[2], eq_inputs_b[2]);
        eq_outputs[3] = jcv_real_eq(eq_inputs_a[3], eq_inputs_b[3]);
        eq_outputs[4] = jcv_real_eq(eq_inputs_a[4], eq_inputs_b[4]);
        eq_outputs[5] = jcv_real_eq(eq_inputs_a[5], eq_inputs_b[5]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_real toint_inputs[] = {5.7f, -5.7f, 0.0f, 100.3f, -100.3f, 3.14159f, -3.14159f, 0.1f, -0.1f};
        jcv_real toint_outputs[9];
        
        // 展开循环
        toint_outputs[0] = jcv_real_to_int(toint_inputs[0]);
        toint_outputs[1] = jcv_real_to_int(toint_inputs[1]);
        toint_outputs[2] = jcv_real_to_int(toint_inputs[2]);
        toint_outputs[3] = jcv_real_to_int(toint_inputs[3]);
        toint_outputs[4] = jcv_real_to_int(toint_inputs[4]);
        toint_outputs[5] = jcv_real_to_int(toint_inputs[5]);
        toint_outputs[6] = jcv_real_to_int(toint_inputs[6]);
        toint_outputs[7] = jcv_real_to_int(toint_inputs[7]);
        toint_outputs[8] = jcv_real_to_int(toint_inputs[8]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_real floor_inputs[] = {5.7f, 5.0f, 5.1f, -5.7f, -5.0f, 0.0f, 0.1f, -0.1f, 3.14159f, -3.14159f, 100.3f, -100.3f};
        jcv_real floor_outputs[12];
        
        // 展开循环
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
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_real ceil_inputs[] = {5.7f, 5.0f, 5.1f, -5.7f, -5.0f, 0.0f, 0.1f, -0.1f, 3.14159f, -3.14159f, 100.3f, -100.3f};
        jcv_real ceil_outputs[12];
        
        // 展开循环
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
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_real min_inputs_a[] = {5.0f, 3.0f, 5.0f, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 3.14159f, -3.14159f, 100.3f, -100.3f};
        jcv_real min_inputs_b[] = {3.0f, 5.0f, 5.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 2.71828f, -2.71828f, 50.7f, -50.7f};
        jcv_real min_outputs[12];
        
        // 展开循环
        min_outputs[0] = jcv_min(min_inputs_a[0], min_inputs_b[0]);
        min_outputs[1] = jcv_min(min_inputs_a[1], min_inputs_b[1]);
        min_outputs[2] = jcv_min(min_inputs_a[2], min_inputs_b[2]);
        min_outputs[3] = jcv_min(min_inputs_a[3], min_inputs_b[3]);
        min_outputs[4] = jcv_min(min_inputs_a[4], min_inputs_b[4]);
        min_outputs[5] = jcv_min(min_inputs_a[5], min_inputs_b[5]);
        min_outputs[6] = jcv_min(min_inputs_a[6], min_inputs_b[6]);
        min_outputs[7] = jcv_min(min_inputs_a[7], min_inputs_b[7]);
        min_outputs[8] = jcv_min(min_inputs_a[8], min_inputs_b[8]);
        min_outputs[9] = jcv_min(min_inputs_a[9], min_inputs_b[9]);
        min_outputs[10] = jcv_min(min_inputs_a[10], min_inputs_b[10]);
        min_outputs[11] = jcv_min(min_inputs_a[11], min_inputs_b[11]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_real max_inputs_a[] = {5.0f, 3.0f, 5.0f, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 3.14159f, -3.14159f, 100.3f, -100.3f};
        jcv_real max_inputs_b[] = {3.0f, 5.0f, 5.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 2.71828f, -2.71828f, 50.7f, -50.7f};
        jcv_real max_outputs[12];
        
        // 展开循环
        max_outputs[0] = jcv_max(max_inputs_a[0], max_inputs_b[0]);
        max_outputs[1] = jcv_max(max_inputs_a[1], max_inputs_b[1]);
        max_outputs[2] = jcv_max(max_inputs_a[2], max_inputs_b[2]);
        max_outputs[3] = jcv_max(max_inputs_a[3], max_inputs_b[3]);
        max_outputs[4] = jcv_max(max_inputs_a[4], max_inputs_b[4]);
        max_outputs[5] = jcv_max(max_inputs_a[5], max_inputs_b[5]);
        max_outputs[6] = jcv_max(max_inputs_a[6], max_inputs_b[6]);
        max_outputs[7] = jcv_max(max_inputs_a[7], max_inputs_b[7]);
        max_outputs[8] = jcv_max(max_inputs_a[8], max_inputs_b[8]);
        max_outputs[9] = jcv_max(max_inputs_a[9], max_inputs_b[9]);
        max_outputs[10] = jcv_max(max_inputs_a[10], max_inputs_b[10]);
        max_outputs[11] = jcv_max(max_inputs_a[11], max_inputs_b[11]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;

        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_point cmp_inputs_a[] = {{1.0f, 2.0f}, {3.0f, 4.0f}, {1.0f, 4.0f}, {3.0f, 2.0f}, {1.0f, 2.0f}, {0.0f, 0.0f}, {-1.0f, -2.0f}, {5.0f, 3.0f}};
        jcv_point cmp_inputs_b[] = {{3.0f, 4.0f}, {1.0f, 2.0f}, {1.0f, 2.0f}, {1.0f, 2.0f}, {1.0f, 2.0f}, {0.0f, 0.0f}, {-1.0f, -2.0f}, {3.0f, 5.0f}};
        int cmp_outputs[8];
        
        // 展开循环
        cmp_outputs[0] = jcv_point_cmp(&cmp_inputs_a[0], &cmp_inputs_b[0]);
        cmp_outputs[1] = jcv_point_cmp(&cmp_inputs_a[1], &cmp_inputs_b[1]);
        cmp_outputs[2] = jcv_point_cmp(&cmp_inputs_a[2], &cmp_inputs_b[2]);
        cmp_outputs[3] = jcv_point_cmp(&cmp_inputs_a[3], &cmp_inputs_b[3]);
        cmp_outputs[4] = jcv_point_cmp(&cmp_inputs_a[4], &cmp_inputs_b[4]);
        cmp_outputs[5] = jcv_point_cmp(&cmp_inputs_a[5], &cmp_inputs_b[5]);
        cmp_outputs[6] = jcv_point_cmp(&cmp_inputs_a[6], &cmp_inputs_b[6]);
        cmp_outputs[7] = jcv_point_cmp(&cmp_inputs_a[7], &cmp_inputs_b[7]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_point less_inputs_a[] = {{1.0f, 2.0f}, {3.0f, 4.0f}, {1.0f, 4.0f}, {3.0f, 2.0f}, {1.0f, 2.0f}, {0.0f, 0.0f}, {-1.0f, -2.0f}, {5.0f, 3.0f}};
        jcv_point less_inputs_b[] = {{3.0f, 4.0f}, {1.0f, 2.0f}, {1.0f, 2.0f}, {1.0f, 2.0f}, {1.0f, 2.0f}, {0.0f, 0.0f}, {-1.0f, -2.0f}, {3.0f, 5.0f}};
        int less_outputs[8];
        
        // 展开循环
        less_outputs[0] = jcv_point_less(&less_inputs_a[0], &less_inputs_b[0]);
        less_outputs[1] = jcv_point_less(&less_inputs_a[1], &less_inputs_b[1]);
        less_outputs[2] = jcv_point_less(&less_inputs_a[2], &less_inputs_b[2]);
        less_outputs[3] = jcv_point_less(&less_inputs_a[3], &less_inputs_b[3]);
        less_outputs[4] = jcv_point_less(&less_inputs_a[4], &less_inputs_b[4]);
        less_outputs[5] = jcv_point_less(&less_inputs_a[5], &less_inputs_b[5]);
        less_outputs[6] = jcv_point_less(&less_inputs_a[6], &less_inputs_b[6]);
        less_outputs[7] = jcv_point_less(&less_inputs_a[7], &less_inputs_b[7]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_point eq_inputs_a[] = {{1.0f, 2.0f}, {1.0f, 2.0f}, {1.0f + JCV_REAL_TYPE_EPSILON/2, 2.0f}, 
                                   {1.0f, 2.0f + JCV_REAL_TYPE_EPSILON/2}, {3.0f, 4.0f}, {0.0f, 0.0f}, 
                                   {-1.0f, -2.0f}, {5.0f, 3.0f}};
        jcv_point eq_inputs_b[] = {{1.0f, 2.0f}, {1.0f, 2.0f}, {1.0f, 2.0f}, 
                                   {1.0f, 2.0f}, {1.0f, 2.0f}, {0.0f, 0.0f}, 
                                   {-1.0f, -2.0f}, {3.0f, 5.0f}};
        int eq_outputs[8];
        
        // 展开循环
        eq_outputs[0] = jcv_point_eq(&eq_inputs_a[0], &eq_inputs_b[0]);
        eq_outputs[1] = jcv_point_eq(&eq_inputs_a[1], &eq_inputs_b[1]);
        eq_outputs[2] = jcv_point_eq(&eq_inputs_a[2], &eq_inputs_b[2]);
        eq_outputs[3] = jcv_point_eq(&eq_inputs_a[3], &eq_inputs_b[3]);
        eq_outputs[4] = jcv_point_eq(&eq_inputs_a[4], &eq_inputs_b[4]);
        eq_outputs[5] = jcv_point_eq(&eq_inputs_a[5], &eq_inputs_b[5]);
        eq_outputs[6] = jcv_point_eq(&eq_inputs_a[6], &eq_inputs_b[6]);
        eq_outputs[7] = jcv_point_eq(&eq_inputs_a[7], &eq_inputs_b[7]);
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_point box_min = {0.0f, 0.0f};
        jcv_point box_max = {10.0f, 10.0f};
        
        jcv_point box_inputs[] = {{0.0f, 5.0f}, {10.0f, 5.0f}, {5.0f, 0.0f}, {5.0f, 10.0f},
                                  {5.0f, 5.0f}, {0.0f, 0.0f}, {10.0f, 10.0f}, {-1.0f, 5.0f},
                                  {0.0f, 10.0f}, {10.0f, 0.0f}, {5.0f, -1.0f}, {5.0f, 11.0f}};
        int box_outputs[12];
        
        // 展开循环
        box_outputs[0] = jcv_point_on_box_edge(&box_inputs[0], &box_min, &box_max);
        box_outputs[1] = jcv_point_on_box_edge(&box_inputs[1], &box_min, &box_max);
        box_outputs[2] = jcv_point_on_box_edge(&box_inputs[2], &box_min, &box_max);
        box_outputs[3] = jcv_point_on_box_edge(&box_inputs[3], &box_min, &box_max);
        box_outputs[4] = jcv_point_on_box_edge(&box_inputs[4], &box_min, &box_max);
        box_outputs[5] = jcv_point_on_box_edge(&box_inputs[5], &box_min, &box_max);
        box_outputs[6] = jcv_point_on_box_edge(&box_inputs[6], &box_min, &box_max);
        box_outputs[7] = jcv_point_on_box_edge(&box_inputs[7], &box_min, &box_max);
        box_outputs[8] = jcv_point_on_box_edge(&box_inputs[8], &box_min, &box_max);
        box_outputs[9] = jcv_point_on_box_edge(&box_inputs[9], &box_min, &box_max);
        box_outputs[10] = jcv_point_on_box_edge(&box_inputs[10], &box_min, &box_max);
        box_outputs[11] = jcv_point_on_box_edge(&box_inputs[11], &box_min, &box_max);
        
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        int corner_inputs[] = {JCV_CORNER_TOP_LEFT, JCV_CORNER_TOP_RIGHT, JCV_CORNER_BOTTOM_LEFT, 
                               JCV_CORNER_BOTTOM_RIGHT, JCV_CORNER_NONE, 5, -1, 0, 1, 2, 3, 4, 6, 10};
        int corner_outputs[14];
        
        // 展开循环
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
        
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        int rotate_inputs[] = {JCV_CORNER_TOP_LEFT, JCV_CORNER_TOP_RIGHT, JCV_CORNER_BOTTOM_RIGHT, 
                               JCV_CORNER_BOTTOM_LEFT, JCV_CORNER_NONE, 5, 1, 4, 2, 3, 6, 10, 0, -1};
        int rotate_outputs[14];
        
        // 展开循环
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
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
    }
    
    {
        jcv_point corner_min = {0.0f, 0.0f};
        jcv_point corner_max = {10.0f, 10.0f};
        
        int corner_inputs[] = {JCV_CORNER_TOP_LEFT, JCV_CORNER_TOP_RIGHT, JCV_CORNER_BOTTOM_LEFT, 
                               JCV_CORNER_BOTTOM_RIGHT, JCV_CORNER_NONE, 5, -1, 0, 1, 2, 3, 4, 6, 10};
        jcv_point corner_outputs[14];
        
        // 展开循环
        corner_outputs[0] = jcv_corner_to_point(corner_inputs[0], &corner_min, &corner_max);
        corner_outputs[1] = jcv_corner_to_point(corner_inputs[1], &corner_min, &corner_max);
        corner_outputs[2] = jcv_corner_to_point(corner_inputs[2], &corner_min, &corner_max);
        corner_outputs[3] = jcv_corner_to_point(corner_inputs[3], &corner_min, &corner_max);
        corner_outputs[4] = jcv_corner_to_point(corner_inputs[4], &corner_min, &corner_max);
        corner_outputs[5] = jcv_corner_to_point(corner_inputs[5], &corner_min, &corner_max);
        corner_outputs[6] = jcv_corner_to_point(corner_inputs[6], &corner_min, &corner_max);
        corner_outputs[7] = jcv_corner_to_point(corner_inputs[7], &corner_min, &corner_max);
        corner_outputs[8] = jcv_corner_to_point(corner_inputs[8], &corner_min, &corner_max);
        corner_outputs[9] = jcv_corner_to_point(corner_inputs[9], &corner_min, &corner_max);
        corner_outputs[10] = jcv_corner_to_point(corner_inputs[10], &corner_min, &corner_max);
        corner_outputs[11] = jcv_corner_to_point(corner_inputs[11], &corner_min, &corner_max);
        corner_outputs[12] = jcv_corner_to_point(corner_inputs[12], &corner_min, &corner_max);
        corner_outputs[13] = jcv_corner_to_point(corner_inputs[13], &corner_min, &corner_max);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
    }
    
    {
        jcv_point dist_inputs_a[] = {{0.0f, 0.0f}, {3.0f, 4.0f}, {1.0f, 1.0f}, {2.0f, 2.0f}, 
                                     {-1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {5.0f, 12.0f},
                                     {-3.0f, 4.0f}, {1.5f, 2.0f}, {0.0f, 0.0f}, {2.0f, 3.0f}};
        jcv_point dist_inputs_b[] = {{3.0f, 4.0f}, {0.0f, 0.0f}, {2.0f, 2.0f}, {1.0f, 1.0f},
                                     {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},
                                     {0.0f, 0.0f}, {1.5f, 2.0f}, {1.0f, 1.0f}, {5.0f, 7.0f}};
        jcv_real dist_outputs[12];
        
        // 展开循环
        dist_outputs[0] = jcv_point_dist_sq(&dist_inputs_a[0], &dist_inputs_b[0]);
        dist_outputs[1] = jcv_point_dist_sq(&dist_inputs_a[1], &dist_inputs_b[1]);
        dist_outputs[2] = jcv_point_dist_sq(&dist_inputs_a[2], &dist_inputs_b[2]);
        dist_outputs[3] = jcv_point_dist_sq(&dist_inputs_a[3], &dist_inputs_b[3]);
        dist_outputs[4] = jcv_point_dist_sq(&dist_inputs_a[4], &dist_inputs_b[4]);
        dist_outputs[5] = jcv_point_dist_sq(&dist_inputs_a[5], &dist_inputs_b[5]);
        dist_outputs[6] = jcv_point_dist_sq(&dist_inputs_a[6], &dist_inputs_b[6]);
        dist_outputs[7] = jcv_point_dist_sq(&dist_inputs_a[7], &dist_inputs_b[7]);
        dist_outputs[8] = jcv_point_dist_sq(&dist_inputs_a[8], &dist_inputs_b[8]);
        dist_outputs[9] = jcv_point_dist_sq(&dist_inputs_a[9], &dist_inputs_b[9]);
        dist_outputs[10] = jcv_point_dist_sq(&dist_inputs_a[10], &dist_inputs_b[10]);
        dist_outputs[11] = jcv_point_dist_sq(&dist_inputs_a[11], &dist_inputs_b[11]);
        
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        test_count++;
        
        test_count++;
        
        test_count++;
        
        test_count++;
    }
    
    {
        jcv_halfedge he;
        jcv_edge edge;
        jcv_site site1, site2;
        
        site1.index = 1;
        site2.index = 2;
        
        edge.sites[0] = &site1;
        edge.sites[1] = &site2;
        
        he.edge = &edge;
        
        he.direction = JCV_DIRECTION_LEFT;
        jcv_site* left_result1 = jcv_halfedge_leftsite(&he);
        test_count++;
        
        jcv_site* right_result1 = jcv_halfedge_rightsite(&he);
        test_count++;
        
        he.direction = JCV_DIRECTION_RIGHT;
        jcv_site* left_result2 = jcv_halfedge_leftsite(&he);
        test_count++;
        
        jcv_site* right_result2 = jcv_halfedge_rightsite(&he);
        test_count++;
        
        he.edge = 0;
        jcv_site* right_result3 = jcv_halfedge_rightsite(&he);
        test_count++;
        
        he.edge = &edge;
        he.direction = JCV_DIRECTION_LEFT;
        
        edge.sites[0] = &site1;
        edge.sites[1] = &site1;
        jcv_site* left_result3 = jcv_halfedge_leftsite(&he);
        test_count++;
        
        jcv_site* right_result4 = jcv_halfedge_rightsite(&he);
        test_count++;
    }
    
    return 0;
}
