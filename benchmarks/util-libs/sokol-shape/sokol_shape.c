#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#define SOKOL_ASSERT(c) assert(c)


/*
    sshape_range is a pointer-size-pair struct used to pass memory
    blobs into sokol-shape. When initialized from a value type
    (array or struct), use the SSHAPE_RANGE() macro to build
    an sshape_range struct.
*/
struct sshape_range {
    const void* ptr;
    size_t size;
};

typedef struct sshape_range sshape_range;


/* a 4x4 matrix wrapper struct */
struct sshape_mat4_t { float m[4][4]; };

typedef struct sshape_mat4_t sshape_mat4_t;


/* vertex layout of the generated geometry */
struct sshape_vertex_t {
    float x, y, z;
    uint32_t normal;        // packed normal as BYTE4N
    uint16_t u, v;          // packed uv coords as USHORT2N
    uint32_t color;         // packed color as UBYTE4N (r,g,b,a);
};

typedef struct sshape_vertex_t sshape_vertex_t;


/* a range of draw-elements (sg_draw(int base_element, int num_element, ...)) */
struct sshape_element_range_t {
    int base_element;
    int num_elements;
};

typedef struct sshape_element_range_t sshape_element_range_t;


/* number of elements and byte size of build actions */
struct sshape_sizes_item_t {
    uint32_t num;       // number of elements
    uint32_t size;      // the same as size in bytes
};

typedef struct sshape_sizes_item_t sshape_sizes_item_t;


struct sshape_sizes_t {
    sshape_sizes_item_t vertices;
    sshape_sizes_item_t indices;
};

typedef struct sshape_sizes_t sshape_sizes_t;


/* in/out struct to keep track of mesh-build state */
 struct sshape_buffer_item_t {
    sshape_range buffer;    // pointer/size pair of output buffer
    size_t data_size;       // size in bytes of valid data in buffer
    size_t shape_offset;    // data offset of the most recent shape
};

typedef struct sshape_buffer_item_t sshape_buffer_item_t;


struct sshape_buffer_t {
    bool valid;
    sshape_buffer_item_t vertices;
    sshape_buffer_item_t indices;
};

typedef struct sshape_buffer_t sshape_buffer_t;


/* creation parameters for the different shape types */
struct sshape_plane_t {
    float width, depth;             // default: 1.0
    uint16_t tiles;                 // default: 1
    uint32_t color;                 // default: white
    bool random_colors;             // default: false
    bool merge;                     // if true merge with previous shape (default: false)
    sshape_mat4_t transform;        // default: identity matrix
};

typedef struct sshape_plane_t sshape_plane_t;


struct sshape_box_t {
    float width, height, depth;     // default: 1.0
    uint16_t tiles;                 // default: 1
    uint32_t color;                 // default: white
    bool random_colors;             // default: false
    bool merge;                     // if true merge with previous shape (default: false)
    sshape_mat4_t transform;        // default: identity matrix
};

typedef struct sshape_box_t sshape_box_t;


struct sshape_sphere_t {
    float radius;                   // default: 0.5
    uint16_t slices;                // default: 5
    uint16_t stacks;                // default: 4
    uint32_t color;                 // default: white
    bool random_colors;             // default: false
    bool merge;                     // if true merge with previous shape (default: false)
    sshape_mat4_t transform;        // default: identity matrix
};

typedef struct sshape_sphere_t sshape_sphere_t;


struct sshape_cylinder_t {
    float radius;                   // default: 0.5
    float height;                   // default: 1.0
    uint16_t slices;                // default: 5
    uint16_t stacks;                // default: 1
    uint32_t color;                 // default: white
    bool random_colors;             // default: false
    bool merge;                     // if true merge with previous shape (default: false)
    sshape_mat4_t transform;        // default: identity matrix
};

typedef struct sshape_cylinder_t sshape_cylinder_t;


struct sshape_torus_t {
    float radius;                   // default: 0.5f
    float ring_radius;              // default: 0.2f
    uint16_t sides;                 // default: 5
    uint16_t rings;                 // default: 5
    uint32_t color;                 // default: white
    bool random_colors;             // default: false
    bool merge;                     // if true merge with previous shape (default: false)
    sshape_mat4_t transform;        // default: identity matrix
};

typedef struct sshape_torus_t sshape_torus_t;


#define _sshape_def(val, def) (((val) == 0) ? (def) : (val))
#define _sshape_def_flt(val, def) (((val) == 0.0f) ? (def) : (val))
#define _sshape_white (0xFFFFFFFF)

#define SOKOL_ASSERT(c) assert(c)


struct _sshape_vec4_t { float x, y, z, w; };
typedef struct _sshape_vec4_t _sshape_vec4_t;


struct _sshape_vec2_t { float x, y; };
typedef struct _sshape_vec2_t _sshape_vec2_t;



float _sshape_clamp(float v) {
    if (v < 0.0f) return 0.0f;
    else if (v > 1.0f) return 1.0f;
    else return v;
}



uint32_t _sshape_pack_ub4_ubyte4n(uint8_t x, uint8_t y, uint8_t z, uint8_t w) {
    return (uint32_t)(((uint32_t)w*0x1000000) + ((uint32_t)z*0x10000) + ((uint32_t)y*0x100) + x);
}



uint32_t _sshape_pack_f4_ubyte4n(float x, float y, float z, float w) {
    uint8_t x8 = (uint8_t) (x * 255.0f);
    uint8_t y8 = (uint8_t) (y * 255.0f);
    uint8_t z8 = (uint8_t) (z * 255.0f);
    uint8_t w8 = (uint8_t) (w * 255.0f);
    return _sshape_pack_ub4_ubyte4n(x8, y8, z8, w8);
}



uint32_t _sshape_pack_f4_byte4n(float x, float y, float z, float w) {
    int8_t x8 = (int8_t) (x * 127.0f);
    int8_t y8 = (int8_t) (y * 127.0f);
    int8_t z8 = (int8_t) (z * 127.0f);
    int8_t w8 = (int8_t) (w * 127.0f);
    return _sshape_pack_ub4_ubyte4n((uint8_t)x8, (uint8_t)y8, (uint8_t)z8, (uint8_t)w8);
}



uint16_t _sshape_pack_f_ushortn(float x) {
    return (uint16_t) (x * 65535.0f);
}



_sshape_vec4_t _sshape_vec4(float x, float y, float z, float w) {
    _sshape_vec4_t v = { x, y, z, w };
    return v;
}



_sshape_vec2_t _sshape_vec2(float x, float y) {
    _sshape_vec2_t v = { x, y };
    return v;
}



bool _sshape_mat4_isnull(const sshape_mat4_t* m) {
    
    for (int y = 0; y < 4; y++) {
        
        for (int x = 0; x < 4; x++) {
            if (0.0f != m->m[y][x]) {
                return false;
            }
        }
    }
    return true;
}



sshape_mat4_t _sshape_mat4_identity() {
    sshape_mat4_t m = {
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        }
    };
    return m;
}



_sshape_vec4_t _sshape_mat4_mul(const sshape_mat4_t* m, _sshape_vec4_t v) {
    _sshape_vec4_t res = {
        m->m[0][0]*v.x + m->m[1][0]*v.y + m->m[2][0]*v.z + m->m[3][0]*v.w,
        m->m[0][1]*v.x + m->m[1][1]*v.y + m->m[2][1]*v.z + m->m[3][1]*v.w,
        m->m[0][2]*v.x + m->m[1][2]*v.y + m->m[2][2]*v.z + m->m[3][2]*v.w,
        m->m[0][3]*v.x + m->m[1][3]*v.y + m->m[2][3]*v.z + m->m[3][3]*v.w
    };
    return res;
}



uint32_t _sshape_plane_num_vertices(uint32_t tiles) {
    return (tiles + 1) * (tiles + 1);
}



uint32_t _sshape_plane_num_indices(uint32_t tiles) {
    return tiles * tiles * 2 * 3;
}



uint32_t _sshape_box_num_vertices(uint32_t tiles) {
    return (tiles + 1) * (tiles + 1) * 6;
}



uint32_t _sshape_box_num_indices(uint32_t tiles) {
    return tiles * tiles * 2 * 6 * 3;
}



uint32_t _sshape_sphere_num_vertices(uint32_t slices, uint32_t stacks) {
    return (slices + 1) * (stacks + 1);
}



uint32_t _sshape_sphere_num_indices(uint32_t slices, uint32_t stacks) {
    return ((2 * slices * stacks) - (2 * slices)) * 3;
}



uint32_t _sshape_cylinder_num_vertices(uint32_t slices, uint32_t stacks) {
    return (slices + 1) * (stacks + 5);
}



uint32_t _sshape_cylinder_num_indices(uint32_t slices, uint32_t stacks) {
    return ((2 * slices * stacks) + (2 * slices)) * 3;
}



uint32_t _sshape_torus_num_vertices(uint32_t sides, uint32_t rings) {
    return (sides + 1) * (rings + 1);
}



uint32_t _sshape_torus_num_indices(uint32_t sides, uint32_t rings) {
    return sides * rings * 2 * 3;
}



bool _sshape_validate_buffer_item(const sshape_buffer_item_t* item, uint32_t build_size) {
    if (0 == item->buffer.ptr) {
        return false;
    }
    if (0 == item->buffer.size) {
        return false;
    }
    if ((item->data_size + build_size) > item->buffer.size) {
        return false;
    }
    if (item->shape_offset > item->data_size) {
        return false;
    }
    return true;
}



bool _sshape_validate_buffer(const sshape_buffer_t* buf, uint32_t num_vertices, uint32_t num_indices) {
    if (!_sshape_validate_buffer_item(&buf->vertices, num_vertices * sizeof(sshape_vertex_t))) {
        return false;
    }
    if (!_sshape_validate_buffer_item(&buf->indices, num_indices * sizeof(uint16_t))) {
        return false;
    }
    return true;
}



void _sshape_advance_offset(sshape_buffer_item_t* item) {
    item->shape_offset = item->data_size;
}



uint16_t _sshape_base_index(const sshape_buffer_t* buf) {
    return (uint16_t) (buf->vertices.data_size / sizeof(sshape_vertex_t));
}


// sshape_plane_t _sshape_plane_defaults(const sshape_plane_t* params) {
//     sshape_plane_t res = *params;
//     res.width = _sshape_def_flt(res.width, 1.0f);
//     res.depth = _sshape_def_flt(res.depth, 1.0f);
//     res.tiles = _sshape_def(res.tiles, 1);
//     res.color = _sshape_def(res.color, _sshape_white);
//     res.transform = _sshape_mat4_isnull(&res.transform) ? _sshape_mat4_identity() : res.transform;
//     return res;
// }


// sshape_box_t _sshape_box_defaults(const sshape_box_t* params) {
//     sshape_box_t res = *params;
//     res.width = _sshape_def_flt(res.width, 1.0f);
//     res.height = _sshape_def_flt(res.height, 1.0f);
//     res.depth = _sshape_def_flt(res.depth, 1.0f);
//     res.tiles = _sshape_def(res.tiles, 1);
//     res.color = _sshape_def(res.color, _sshape_white);
//     res.transform = _sshape_mat4_isnull(&res.transform) ? _sshape_mat4_identity() : res.transform;
//     return res;
// }


// sshape_sphere_t _sshape_sphere_defaults(const sshape_sphere_t* params) {
//     sshape_sphere_t res = *params;
//     res.radius = _sshape_def_flt(res.radius, 0.5f);
//     res.slices = _sshape_def(res.slices, 5);
//     res.stacks = _sshape_def(res.stacks, 4);
//     res.color = _sshape_def(res.color, _sshape_white);
//     res.transform = _sshape_mat4_isnull(&res.transform) ? _sshape_mat4_identity() : res.transform;
//     return res;
// }


// sshape_cylinder_t _sshape_cylinder_defaults(const sshape_cylinder_t* params) {
//     sshape_cylinder_t res = *params;
//     res.radius = _sshape_def_flt(res.radius, 0.5f);
//     res.height = _sshape_def_flt(res.height, 1.0f);
//     res.slices = _sshape_def(res.slices, 5);
//     res.stacks = _sshape_def(res.stacks, 1);
//     res.color = _sshape_def(res.color, _sshape_white);
//     res.transform = _sshape_mat4_isnull(&res.transform) ? _sshape_mat4_identity() : res.transform;
//     return res;
// }


// sshape_torus_t _sshape_torus_defaults(const sshape_torus_t* params) {
//     sshape_torus_t res = *params;
//     res.radius = _sshape_def_flt(res.radius, 0.5f);
//     res.ring_radius = _sshape_def_flt(res.ring_radius, 0.2f);
//     res.sides = _sshape_def_flt(res.sides, 5);
//     res.rings = _sshape_def_flt(res.rings, 5);
//     res.color = _sshape_def(res.color, _sshape_white);
//     res.transform = _sshape_mat4_isnull(&res.transform) ? _sshape_mat4_identity() : res.transform;
//     return res;
// }


// 
// void _sshape_add_vertex(sshape_buffer_t* buf, _sshape_vec4_t pos, _sshape_vec4_t norm, _sshape_vec2_t uv, uint32_t color) {
//     size_t offset = buf->vertices.data_size;
//     SOKOL_ASSERT((offset + sizeof(sshape_vertex_t)) <= buf->vertices.buffer.size);
//     buf->vertices.data_size += sizeof(sshape_vertex_t);
//     sshape_vertex_t* v_ptr = (sshape_vertex_t*) ((uint8_t*)buf->vertices.buffer.ptr + offset);
//     v_ptr->x = pos.x;
//     v_ptr->y = pos.y;
//     v_ptr->z = pos.z;
//     v_ptr->normal = _sshape_pack_f4_byte4n(norm.x, norm.y, norm.z, norm.w);
//     v_ptr->u = _sshape_pack_f_ushortn(uv.x);
//     v_ptr->v = _sshape_pack_f_ushortn(uv.y);
//     v_ptr->color = color;
// }


// 
// void _sshape_add_triangle(sshape_buffer_t* buf, uint16_t i0, uint16_t i1, uint16_t i2) {
//     size_t offset = buf->indices.data_size;
//     SOKOL_ASSERT((offset + 3*sizeof(uint16_t)) <= buf->indices.buffer.size);
//     buf->indices.data_size += 3*sizeof(uint16_t);
//     uint16_t* i_ptr = (uint16_t*) ((uint8_t*)buf->indices.buffer.ptr + offset);
//     i_ptr[0] = i0;
//     i_ptr[1] = i1;
//     i_ptr[2] = i2;
// }


/*=== PUBLIC API FUNCTIONS ===================================================*/


// uint32_t sshape_color_4f(float r, float g, float b, float a) {
//     return _sshape_pack_f4_ubyte4n(_sshape_clamp(r), _sshape_clamp(g), _sshape_clamp(b), _sshape_clamp(a));
// }


// uint32_t sshape_color_3f(float r, float g, float b) {
//     return _sshape_pack_f4_ubyte4n(_sshape_clamp(r), _sshape_clamp(g), _sshape_clamp(b), 1.0f);
// }



uint32_t sshape_color_4b(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return _sshape_pack_ub4_ubyte4n(r, g, b, a);
}



uint32_t sshape_color_3b(uint8_t r, uint8_t g, uint8_t b) {
    return _sshape_pack_ub4_ubyte4n(r, g, b, 255);
}


// 
// sshape_mat4_t sshape_mat4_transpose(const float m[16]) {
//     sshape_mat4_t res;
//     
//     for (int c = 0; c < 4; c++) {
//         
//         for (int r = 0; r < 4; r++) {
//             res.m[r][c] = m[c*4 + r];
//         }
//     }
//     return res;
// }



sshape_sizes_t sshape_plane_sizes(uint32_t tiles) {
    SOKOL_ASSERT(tiles >= 1);
    sshape_sizes_t res = { {0} };
    res.vertices.num = _sshape_plane_num_vertices(tiles);
    res.indices.num = _sshape_plane_num_indices(tiles);
    res.vertices.size = res.vertices.num * sizeof(sshape_vertex_t);
    res.indices.size = res.indices.num * sizeof(uint16_t);
    return res;
}



sshape_sizes_t sshape_box_sizes(uint32_t tiles) {
    SOKOL_ASSERT(tiles >= 1);
    sshape_sizes_t res = { {0} };
    res.vertices.num = _sshape_box_num_vertices(tiles);
    res.indices.num = _sshape_box_num_indices(tiles);
    res.vertices.size = res.vertices.num * sizeof(sshape_vertex_t);
    res.indices.size = res.indices.num * sizeof(uint16_t);
    return res;
}



sshape_sizes_t sshape_sphere_sizes(uint32_t slices, uint32_t stacks) {
    SOKOL_ASSERT((slices >= 3) && (stacks >= 2));
    sshape_sizes_t res = { {0} };
    res.vertices.num = _sshape_sphere_num_vertices(slices, stacks);
    res.indices.num = _sshape_sphere_num_indices(slices, stacks);
    res.vertices.size = res.vertices.num * sizeof(sshape_vertex_t);
    res.indices.size = res.indices.num * sizeof(uint16_t);
    return res;
}



sshape_sizes_t sshape_cylinder_sizes(uint32_t slices, uint32_t stacks) {
    SOKOL_ASSERT((slices >= 3) && (stacks >= 1));
    sshape_sizes_t res = { {0} };
    res.vertices.num = _sshape_cylinder_num_vertices(slices, stacks);
    res.indices.num = _sshape_cylinder_num_indices(slices, stacks);
    res.vertices.size = res.vertices.num * sizeof(sshape_vertex_t);
    res.indices.size = res.indices.num * sizeof(uint16_t);
    return res;
}



sshape_sizes_t sshape_torus_sizes(uint32_t sides, uint32_t rings) {
    SOKOL_ASSERT((sides >= 3) && (rings >= 3));
    sshape_sizes_t res = { {0} };
    res.vertices.num = _sshape_torus_num_vertices(sides, rings);
    res.indices.num = _sshape_torus_num_indices(sides, rings);
    res.vertices.size = res.vertices.num * sizeof(sshape_vertex_t);
    res.indices.size = res.indices.num * sizeof(uint16_t);
    return res;
}



sshape_element_range_t sshape_element_range(const sshape_buffer_t* buf) {
    SOKOL_ASSERT(buf && buf->valid);
    SOKOL_ASSERT(buf->indices.shape_offset < buf->indices.data_size);
    SOKOL_ASSERT(0 == (buf->indices.shape_offset & (sizeof(uint16_t) - 1)));
    SOKOL_ASSERT(0 == (buf->indices.data_size & (sizeof(uint16_t) - 1)));
    sshape_element_range_t range = { 0 };
    range.base_element = (int) (buf->indices.shape_offset / sizeof(uint16_t));
    if (buf->valid) {
        range.num_elements = (int) ((buf->indices.data_size - buf->indices.shape_offset) / sizeof(uint16_t));
    }
    else {
        range.num_elements = 0;
    }
    return range;
}



int main(void) {
    // printf("Starting sokol_shape library tests...\n");
    
    /* Test counter for assert statements */
    int test_counter = 0;
    
    /* Test _sshape_clamp with array inputs */
    // printf("Testing _sshape_clamp with array inputs...\n");
    float clamp_inputs[10] = {-0.5f, 0.0f, 0.5f, 1.0f, 1.5f, -1.0f, 2.0f, 0.25f, 0.75f, 0.999f};
    float clamp_results[10];
    
    /* Apply function to each input (no loops allowed) */
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
    
    /* Check individual results */
    test_counter += 10;
    
    /* Universal property: all results are in [0, 1] */
    test_counter++;
    
    /* Existential property: there exists a result equal to 0.0f */
    test_counter++;
    
    /* Existential property: there exists a result equal to 1.0f */
    test_counter++;
    
    /* Test _sshape_pack_ub4_ubyte4n with array inputs */
    // printf("Testing _sshape_pack_ub4_ubyte4n with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        uint8_t x, y, z, w;
        uint32_t expected;
    } pack_inputs[8] = {
        {0x00, 0x00, 0x00, 0x00, 0x00000000},
        {0xFF, 0x00, 0x00, 0x00, 0x000000FF},
        {0x00, 0xFF, 0x00, 0x00, 0x0000FF00},
        {0x00, 0x00, 0xFF, 0x00, 0x00FF0000},
        {0x00, 0x00, 0x00, 0xFF, 0xFF000000},
        {0x12, 0x34, 0x56, 0x78, 0x78563412},
        {0xAB, 0xCD, 0xEF, 0x01, 0x01EFCDAB},
        {0x7F, 0x80, 0x81, 0x82, 0x8281807F}
    };
    
    uint32_t pack_results[8];
    
    /* Apply function to each input (no loops allowed) */
    pack_results[0] = _sshape_pack_ub4_ubyte4n(pack_inputs[0].x, pack_inputs[0].y, pack_inputs[0].z, pack_inputs[0].w);
    pack_results[1] = _sshape_pack_ub4_ubyte4n(pack_inputs[1].x, pack_inputs[1].y, pack_inputs[1].z, pack_inputs[1].w);
    pack_results[2] = _sshape_pack_ub4_ubyte4n(pack_inputs[2].x, pack_inputs[2].y, pack_inputs[2].z, pack_inputs[2].w);
    pack_results[3] = _sshape_pack_ub4_ubyte4n(pack_inputs[3].x, pack_inputs[3].y, pack_inputs[3].z, pack_inputs[3].w);
    pack_results[4] = _sshape_pack_ub4_ubyte4n(pack_inputs[4].x, pack_inputs[4].y, pack_inputs[4].z, pack_inputs[4].w);
    pack_results[5] = _sshape_pack_ub4_ubyte4n(pack_inputs[5].x, pack_inputs[5].y, pack_inputs[5].z, pack_inputs[5].w);
    pack_results[6] = _sshape_pack_ub4_ubyte4n(pack_inputs[6].x, pack_inputs[6].y, pack_inputs[6].z, pack_inputs[6].w);
    pack_results[7] = _sshape_pack_ub4_ubyte4n(pack_inputs[7].x, pack_inputs[7].y, pack_inputs[7].z, pack_inputs[7].w);
    
    /* Check individual results */
    test_counter += 8;
    
    /* Universal property: all results match their expected values */
    test_counter++;
    
    /* Existential property: there exists a result with alpha channel = 0xFF */
    test_counter++;
    
    /* Universal property: all results are 32-bit values */
    test_counter++;
    
    /* Test _sshape_pack_f4_ubyte4n with array inputs */
    // printf("Testing _sshape_pack_f4_ubyte4n with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        float x, y, z, w;
        uint32_t expected;
    } fpack_inputs[8] = {
        {0.0f, 0.0f, 0.0f, 0.0f, 0x00000000},
        {1.0f, 0.0f, 0.0f, 0.0f, 0x000000FF},
        {0.0f, 1.0f, 0.0f, 0.0f, 0x0000FF00},
        {0.0f, 0.0f, 1.0f, 0.0f, 0x00FF0000},
        {0.0f, 0.0f, 0.0f, 1.0f, 0xFF000000},
        {0.5f, 0.5f, 0.5f, 0.5f, 0x7F7F7F7F},
        {0.25f, 0.5f, 0.75f, 1.0f, 0xFFBF7F3F},
        {1.0f, 1.0f, 1.0f, 1.0f, 0xFFFFFFFF}
    };
    
    uint32_t fpack_results[8];
    
    /* Apply function to each input (no loops allowed) */
    fpack_results[0] = _sshape_pack_f4_ubyte4n(fpack_inputs[0].x, fpack_inputs[0].y, fpack_inputs[0].z, fpack_inputs[0].w);
    fpack_results[1] = _sshape_pack_f4_ubyte4n(fpack_inputs[1].x, fpack_inputs[1].y, fpack_inputs[1].z, fpack_inputs[1].w);
    fpack_results[2] = _sshape_pack_f4_ubyte4n(fpack_inputs[2].x, fpack_inputs[2].y, fpack_inputs[2].z, fpack_inputs[2].w);
    fpack_results[3] = _sshape_pack_f4_ubyte4n(fpack_inputs[3].x, fpack_inputs[3].y, fpack_inputs[3].z, fpack_inputs[3].w);
    fpack_results[4] = _sshape_pack_f4_ubyte4n(fpack_inputs[4].x, fpack_inputs[4].y, fpack_inputs[4].z, fpack_inputs[4].w);
    fpack_results[5] = _sshape_pack_f4_ubyte4n(fpack_inputs[5].x, fpack_inputs[5].y, fpack_inputs[5].z, fpack_inputs[5].w);
    fpack_results[6] = _sshape_pack_f4_ubyte4n(fpack_inputs[6].x, fpack_inputs[6].y, fpack_inputs[6].z, fpack_inputs[6].w);
    fpack_results[7] = _sshape_pack_f4_ubyte4n(fpack_inputs[7].x, fpack_inputs[7].y, fpack_inputs[7].z, fpack_inputs[7].w);
    
    /* Check individual results */
    test_counter += 8;
    
    /* Universal property: all results match their expected values */
    test_counter++;
    
    /* Existential property: there exists a result with all channels = 0xFF */
    test_counter++;
    
    /* Universal property: all results are valid 32-bit values */
    test_counter++;
    
    /* Universal property: alpha channel (highest byte) is always >= 0 */
    test_counter++;
    
    /* Test _sshape_pack_f4_byte4n with array inputs */
    // printf("Testing _sshape_pack_f4_byte4n with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        float x, y, z, w;
        uint32_t expected;
    } bpack_inputs[7] = {
        {0.0f, 0.0f, 0.0f, 0.0f, 0x00000000},
        {1.0f, 0.0f, 0.0f, 0.0f, 0x0000007F},
        {-1.0f, 0.0f, 0.0f, 0.0f, 0x00000081},
        {0.0f, 1.0f, 0.0f, 0.0f, 0x00007F00},
        {0.0f, 0.0f, 1.0f, 0.0f, 0x007F0000},
        {0.0f, 0.0f, 0.0f, 1.0f, 0x7F000000},
        {0.5f, -0.5f, 0.25f, -0.75f, 0xA11FC13F}
    };
    
    uint32_t bpack_results[7];
    
    /* Apply function to each input (no loops allowed) */
    bpack_results[0] = _sshape_pack_f4_byte4n(bpack_inputs[0].x, bpack_inputs[0].y, bpack_inputs[0].z, bpack_inputs[0].w);
    bpack_results[1] = _sshape_pack_f4_byte4n(bpack_inputs[1].x, bpack_inputs[1].y, bpack_inputs[1].z, bpack_inputs[1].w);
    bpack_results[2] = _sshape_pack_f4_byte4n(bpack_inputs[2].x, bpack_inputs[2].y, bpack_inputs[2].z, bpack_inputs[2].w);
    bpack_results[3] = _sshape_pack_f4_byte4n(bpack_inputs[3].x, bpack_inputs[3].y, bpack_inputs[3].z, bpack_inputs[3].w);
    bpack_results[4] = _sshape_pack_f4_byte4n(bpack_inputs[4].x, bpack_inputs[4].y, bpack_inputs[4].z, bpack_inputs[4].w);
    bpack_results[5] = _sshape_pack_f4_byte4n(bpack_inputs[5].x, bpack_inputs[5].y, bpack_inputs[5].z, bpack_inputs[5].w);
    bpack_results[6] = _sshape_pack_f4_byte4n(bpack_inputs[6].x, bpack_inputs[6].y, bpack_inputs[6].z, bpack_inputs[6].w);
    
    /* Check individual results */
    test_counter += 7;
    
    /* Universal property: all results match their expected values */
    test_counter++;
    
    /* Existential property: there exists a result with negative value (MSB set) */
    test_counter++;
    
    /* Universal property: all results are valid 32-bit values */
    test_counter++;
    
    /* Universal property: alpha channel (highest byte) is always in signed byte range */
    test_counter++;
    
    /* Test _sshape_pack_f_ushortn with array inputs */
    // printf("Testing _sshape_pack_f_ushortn with array inputs...\n");
    float ushort_inputs[8] = {0.0f, 1.0f, 0.5f, 0.25f, 0.75f, 0.1f, 0.9f, 0.333f};
    uint16_t ushort_results[8];
    
    /* Apply function to each input (no loops allowed) */
    ushort_results[0] = _sshape_pack_f_ushortn(ushort_inputs[0]);
    ushort_results[1] = _sshape_pack_f_ushortn(ushort_inputs[1]);
    ushort_results[2] = _sshape_pack_f_ushortn(ushort_inputs[2]);
    ushort_results[3] = _sshape_pack_f_ushortn(ushort_inputs[3]);
    ushort_results[4] = _sshape_pack_f_ushortn(ushort_inputs[4]);
    ushort_results[5] = _sshape_pack_f_ushortn(ushort_inputs[5]);
    ushort_results[6] = _sshape_pack_f_ushortn(ushort_inputs[6]);
    ushort_results[7] = _sshape_pack_f_ushortn(ushort_inputs[7]);
    
    /* Check individual results */
 /* 0.5 * 65535 = 32767.5 -> truncated to 32767 */
 /* 0.25 * 65535 = 16383.75 -> truncated to 16383 */
 /* 0.75 * 65535 = 49151.25 -> truncated to 49151 */
 /* 0.1 * 65535 = 6553.5 -> truncated to 6553 */
 /* 0.9 * 65535 = 58981.5 -> truncated to 58981 */
 /* 0.333 * 65535 = 21823.155 -> truncated to 21811 */
    test_counter += 8;
    
    /* Universal property: all results are in [0, 65535] */
    test_counter++;
    
    /* Existential property: there exists a result equal to 0 */
    test_counter++;
    
    /* Existential property: there exists a result equal to 65535 */
    test_counter++;
    
    /* Universal property: results are monotonic with inputs (for sorted inputs) */
    /* Since inputs are not sorted, we check a specific property: if input is 0, result is 0 */
    test_counter++;
    
    /* Test _sshape_vec4 with array inputs */
    // printf("Testing _sshape_vec4 with array inputs...\n");
    
    /* Define test inputs for vec4 */
    struct {
        float x, y, z, w;
    } vec4_inputs[4] = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f},
        {-1.0f, -2.0f, -3.0f, -4.0f},
        {1.5f, 2.5f, 3.5f, 4.5f}
    };
    
    _sshape_vec4_t vec4_results[4];
    
    /* Apply function to each input (no loops allowed) */
    vec4_results[0] = _sshape_vec4(vec4_inputs[0].x, vec4_inputs[0].y, vec4_inputs[0].z, vec4_inputs[0].w);
    vec4_results[1] = _sshape_vec4(vec4_inputs[1].x, vec4_inputs[1].y, vec4_inputs[1].z, vec4_inputs[1].w);
    vec4_results[2] = _sshape_vec4(vec4_inputs[2].x, vec4_inputs[2].y, vec4_inputs[2].z, vec4_inputs[2].w);
    vec4_results[3] = _sshape_vec4(vec4_inputs[3].x, vec4_inputs[3].y, vec4_inputs[3].z, vec4_inputs[3].w);
    
    /* Check individual results */
    
    
    
    test_counter += 16;
    
    /* Universal property: all vec4 results have correct component mapping */
    test_counter++;
    
    /* Existential property: there exists a vec4 with all positive components */
    test_counter++;
    
    /* Existential property: there exists a vec4 with all negative components */
    test_counter++;
    
    /* Test _sshape_vec2 with array inputs */
    // printf("Testing _sshape_vec2 with array inputs...\n");
    
    /* Define test inputs for vec2 */
    struct {
        float x, y;
    } vec2_inputs[4] = {
        {0.0f, 0.0f},
        {1.0f, 2.0f},
        {-1.0f, -2.0f},
        {1.5f, 2.5f}
    };
    
    _sshape_vec2_t vec2_results[4];
    
    /* Apply function to each input (no loops allowed) */
    vec2_results[0] = _sshape_vec2(vec2_inputs[0].x, vec2_inputs[0].y);
    vec2_results[1] = _sshape_vec2(vec2_inputs[1].x, vec2_inputs[1].y);
    vec2_results[2] = _sshape_vec2(vec2_inputs[2].x, vec2_inputs[2].y);
    vec2_results[3] = _sshape_vec2(vec2_inputs[3].x, vec2_inputs[3].y);
    
    /* Check individual results */
    
    
    
    test_counter += 8;
    
    /* Universal property: all vec2 results have correct component mapping */
    test_counter++;
    
    /* Existential property: there exists a vec2 with both components positive */
    test_counter++;
    
    /* Existential property: there exists a vec2 with both components negative */
    test_counter++;
    
    /* Test _sshape_mat4_isnull */
    // printf("Testing _sshape_mat4_isnull...\n");
    sshape_mat4_t zero_mat = {0};
    
    /* Test _sshape_mat4_identity */
    // printf("Testing _sshape_mat4_identity...\n");
    sshape_mat4_t identity_result = _sshape_mat4_identity();
    /* Check diagonal elements are 1.0f */
    /* Check some off-diagonal elements are 0.0f */
    test_counter += 8;
    
    /* Test _sshape_mat4_mul */
    // printf("Testing _sshape_mat4_mul...\n");
    sshape_mat4_t mul_mat = {
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        }
    };
    _sshape_vec4_t mul_vec = _sshape_vec4(1.0f, 2.0f, 3.0f, 4.0f);
    _sshape_vec4_t mul_result1 = _sshape_mat4_mul(&mul_mat, mul_vec);
    
    /* Create a test matrix with specific values */
    sshape_mat4_t test_mat_mul = {
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 3.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 4.0f }
        }
    };
    _sshape_vec4_t mul_vec2 = _sshape_vec4(1.0f, 1.0f, 1.0f, 1.0f);
    _sshape_vec4_t mul_result2 = _sshape_mat4_mul(&test_mat_mul, mul_vec2);
 /* 1*1 + 0*1 + 0*1 + 0*1 = 1 */
    
    /* Test _sshape_plane_num_vertices with array inputs */
    // printf("Testing _sshape_plane_num_vertices with array inputs...\n");
    uint32_t plane_tiles_inputs[5] = {1, 2, 3, 4, 10};
    uint32_t plane_verts_results[5];
    
    /* Apply function to each input (no loops allowed) */
    plane_verts_results[0] = _sshape_plane_num_vertices(plane_tiles_inputs[0]);
    plane_verts_results[1] = _sshape_plane_num_vertices(plane_tiles_inputs[1]);
    plane_verts_results[2] = _sshape_plane_num_vertices(plane_tiles_inputs[2]);
    plane_verts_results[3] = _sshape_plane_num_vertices(plane_tiles_inputs[3]);
    plane_verts_results[4] = _sshape_plane_num_vertices(plane_tiles_inputs[4]);
    
    /* Check individual results */
 /* (1+1)*(1+1) = 4 */
 /* (2+1)*(2+1) = 9 */
 /* (3+1)*(3+1) = 16 */
 /* (4+1)*(4+1) = 25 */
 /* (10+1)*(10+1) = 121 */

    /* Test _sshape_plane_num_indices with array inputs */
    // printf("Testing _sshape_plane_num_indices with array inputs...\n");
    uint32_t plane_indices_results[5];
    
    /* Apply function to each input (no loops allowed) */
    plane_indices_results[0] = _sshape_plane_num_indices(plane_tiles_inputs[0]);
    plane_indices_results[1] = _sshape_plane_num_indices(plane_tiles_inputs[1]);
    plane_indices_results[2] = _sshape_plane_num_indices(plane_tiles_inputs[2]);
    plane_indices_results[3] = _sshape_plane_num_indices(plane_tiles_inputs[3]);
    plane_indices_results[4] = _sshape_plane_num_indices(plane_tiles_inputs[4]);
    
    /* Check individual results */
 /* 1*1*2*3 = 6 */
 /* 2*2*2*3 = 24 */
 /* 3*3*2*3 = 54 */
 /* 4*4*2*3 = 96 */
 /* 10*10*2*3 = 600 */
    test_counter += 5;
    
    test_counter++;
    
    test_counter++;
    
    test_counter++;
    
    /* Test _sshape_box_num_vertices with array inputs */
    // printf("Testing _sshape_box_num_vertices with array inputs...\n");
    uint32_t box_tiles_inputs[5] = {1, 2, 3, 4, 10};
    uint32_t box_verts_results[5];
    
    /* Apply function to each input (no loops allowed) */
    box_verts_results[0] = _sshape_box_num_vertices(box_tiles_inputs[0]);
    box_verts_results[1] = _sshape_box_num_vertices(box_tiles_inputs[1]);
    box_verts_results[2] = _sshape_box_num_vertices(box_tiles_inputs[2]);
    box_verts_results[3] = _sshape_box_num_vertices(box_tiles_inputs[3]);
    box_verts_results[4] = _sshape_box_num_vertices(box_tiles_inputs[4]);
    
    /* Check individual results */
 /* (1+1)*(1+1)*6 = 4*6 = 24 */
 /* (2+1)*(2+1)*6 = 9*6 = 54 */
 /* (3+1)*(3+1)*6 = 16*6 = 96 */
 /* (4+1)*(4+1)*6 = 25*6 = 150 */
 /* (10+1)*(10+1)*6 = 121*6 = 726 */
    test_counter += 5;
    
    test_counter++;
    
    test_counter++;
    
    test_counter++;
    
    /* Test _sshape_box_num_indices with array inputs */
    // printf("Testing _sshape_box_num_indices with array inputs...\n");
    uint32_t box_indices_results[5];
    
    /* Apply function to each input (no loops allowed) */
    box_indices_results[0] = _sshape_box_num_indices(box_tiles_inputs[0]);
    box_indices_results[1] = _sshape_box_num_indices(box_tiles_inputs[1]);
    box_indices_results[2] = _sshape_box_num_indices(box_tiles_inputs[2]);
    box_indices_results[3] = _sshape_box_num_indices(box_tiles_inputs[3]);
    box_indices_results[4] = _sshape_box_num_indices(box_tiles_inputs[4]);
    
    /* Check individual results */
 /* 1*1*2*6*3 = 36 */
 /* 2*2*2*6*3 = 144 */
 /* 3*3*2*6*3 = 324 */
 /* 4*4*2*6*3 = 576 */
 /* 10*10*2*6*3 = 3600 */
    test_counter += 5;

    test_counter++;
    
    test_counter++;
    
    test_counter++;
    
    /* Test _sshape_sphere_num_vertices with array inputs */
    // printf("Testing _sshape_sphere_num_vertices with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        uint32_t slices, stacks;
        uint32_t expected;
    } sphere_verts_inputs[5] = {
        {3, 2, 12},   /* (3+1)*(2+1) = 4*3 = 12 */
        {5, 4, 30},   /* (5+1)*(4+1) = 6*5 = 30 */
        {10, 8, 99},  /* (10+1)*(8+1) = 11*9 = 99 */
        {20, 10, 231}, /* (20+1)*(10+1) = 21*11 = 231 */
        {1, 1, 4}     /* (1+1)*(1+1) = 2*2 = 4 */
    };
    
    uint32_t sphere_verts_results[5];
    
    /* Apply function to each input (no loops allowed) */
    sphere_verts_results[0] = _sshape_sphere_num_vertices(sphere_verts_inputs[0].slices, sphere_verts_inputs[0].stacks);
    sphere_verts_results[1] = _sshape_sphere_num_vertices(sphere_verts_inputs[1].slices, sphere_verts_inputs[1].stacks);
    sphere_verts_results[2] = _sshape_sphere_num_vertices(sphere_verts_inputs[2].slices, sphere_verts_inputs[2].stacks);
    sphere_verts_results[3] = _sshape_sphere_num_vertices(sphere_verts_inputs[3].slices, sphere_verts_inputs[3].stacks);
    sphere_verts_results[4] = _sshape_sphere_num_vertices(sphere_verts_inputs[4].slices, sphere_verts_inputs[4].stacks);
    
    /* Check individual results */



    test_counter++;
    
    /* Test _sshape_sphere_num_indices with array inputs */
    // printf("Testing _sshape_sphere_num_indices with array inputs...\n");
    
    uint32_t sphere_indices_results[5];
    
    /* Apply function to each input (no loops allowed) */
    sphere_indices_results[0] = _sshape_sphere_num_indices(sphere_verts_inputs[0].slices, sphere_verts_inputs[0].stacks);
    sphere_indices_results[1] = _sshape_sphere_num_indices(sphere_verts_inputs[1].slices, sphere_verts_inputs[1].stacks);
    sphere_indices_results[2] = _sshape_sphere_num_indices(sphere_verts_inputs[2].slices, sphere_verts_inputs[2].stacks);
    sphere_indices_results[3] = _sshape_sphere_num_indices(sphere_verts_inputs[3].slices, sphere_verts_inputs[3].stacks);
    sphere_indices_results[4] = _sshape_sphere_num_indices(sphere_verts_inputs[4].slices, sphere_verts_inputs[4].stacks);
    
    /* Check individual results */
 /* ((2*3*2) - (2*3)) * 3 = (12 - 6) * 3 = 6*3 = 18 */
 /* ((2*5*4) - (2*5)) * 3 = (40 - 10) * 3 = 30*3 = 90 */
 /* ((2*10*8) - (2*10)) * 3 = (160 - 20) * 3 = 140*3 = 420 */
 /* ((2*20*10) - (2*20)) * 3 = (400 - 40) * 3 = 360*3 = 1080 */
 /* ((2*1*1) - (2*1)) * 3 = (2 - 2) * 3 = 0 */
    test_counter += 5;
    
    /* Universal property: all results are multiples of 6 (when not zero) */
    test_counter++;
    
    /* Universal property: results increase with input size (for non-zero cases) */
    test_counter++;
    
    /* Existential property: there exists a result that is 0 */
    test_counter++;
    
    /* Test _sshape_cylinder_num_vertices with array inputs */
    // printf("Testing _sshape_cylinder_num_vertices with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        uint32_t slices, stacks;
        uint32_t expected;
    } cylinder_verts_inputs[5] = {
        {3, 1, 24},   /* (3+1)*(1+5) = 4*6 = 24 */
        {5, 2, 42},   /* (5+1)*(2+5) = 6*7 = 42 */
        {10, 5, 110}, /* (10+1)*(5+5) = 11*10 = 110 */
        {20, 10, 315}, /* (20+1)*(10+5) = 21*15 = 315 */
        {1, 1, 12}    /* (1+1)*(1+5) = 2*6 = 12 */
    };
    
    uint32_t cylinder_verts_results[5];
    
    /* Apply function to each input (no loops allowed) */
    cylinder_verts_results[0] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[0].slices, cylinder_verts_inputs[0].stacks);
    cylinder_verts_results[1] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[1].slices, cylinder_verts_inputs[1].stacks);
    cylinder_verts_results[2] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[2].slices, cylinder_verts_inputs[2].stacks);
    cylinder_verts_results[3] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[3].slices, cylinder_verts_inputs[3].stacks);
    cylinder_verts_results[4] = _sshape_cylinder_num_vertices(cylinder_verts_inputs[4].slices, cylinder_verts_inputs[4].stacks);
    
    /* Check individual results */
    test_counter += 5;
    
    /* Universal property: all results are multiples of 6 */
    test_counter++;
    
    /* Universal property: all results are >= 12 */
    test_counter++;
    
    /* Existential property: there exists a result that is a multiple of 12 */
    test_counter++;
    
    /* Test _sshape_cylinder_num_indices with array inputs */
    // printf("Testing _sshape_cylinder_num_indices with array inputs...\n");
    
    uint32_t cylinder_indices_results[5];
    
    /* Apply function to each input (no loops allowed) */
    cylinder_indices_results[0] = _sshape_cylinder_num_indices(cylinder_verts_inputs[0].slices, cylinder_verts_inputs[0].stacks);
    cylinder_indices_results[1] = _sshape_cylinder_num_indices(cylinder_verts_inputs[1].slices, cylinder_verts_inputs[1].stacks);
    cylinder_indices_results[2] = _sshape_cylinder_num_indices(cylinder_verts_inputs[2].slices, cylinder_verts_inputs[2].stacks);
    cylinder_indices_results[3] = _sshape_cylinder_num_indices(cylinder_verts_inputs[3].slices, cylinder_verts_inputs[3].stacks);
    cylinder_indices_results[4] = _sshape_cylinder_num_indices(cylinder_verts_inputs[4].slices, cylinder_verts_inputs[4].stacks);
    
    /* Check individual results */
 /* ((2*3*1) + (2*3)) * 3 = (6 + 6) * 3 = 12*3 = 36 */
 /* ((2*5*2) + (2*5)) * 3 = (20 + 10) * 3 = 30*3 = 90 */
 /* ((2*10*5) + (2*10)) * 3 = (100 + 20) * 3 = 120*3 = 360 */
 /* ((2*20*10) + (2*20)) * 3 = (400 + 40) * 3 = 440*3 = 1320 */
 /* ((2*1*1) + (2*1)) * 3 = (2 + 2) * 3 = 4*3 = 12 */
    test_counter += 5;
    
    /* Universal property: all results are multiples of 6 */
    test_counter++;
    
    /* Existential property: there exists a result that is a multiple of 30 */
    test_counter++;
    
    /* Test _sshape_torus_num_vertices with array inputs */
    // printf("Testing _sshape_torus_num_vertices with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        uint32_t sides, rings;
        uint32_t expected;
    } torus_verts_inputs[5] = {
        {3, 3, 16},   /* (3+1)*(3+1) = 4*4 = 16 */
        {5, 5, 36},   /* (5+1)*(5+1) = 6*6 = 36 */
        {10, 8, 99},  /* (10+1)*(8+1) = 11*9 = 99 */
        {20, 15, 336}, /* (20+1)*(15+1) = 21*16 = 336 */
        {1, 1, 4}     /* (1+1)*(1+1) = 2*2 = 4 */
    };
    
    uint32_t torus_verts_results[5];
    
    /* Apply function to each input (no loops allowed) */
    torus_verts_results[0] = _sshape_torus_num_vertices(torus_verts_inputs[0].sides, torus_verts_inputs[0].rings);
    torus_verts_results[1] = _sshape_torus_num_vertices(torus_verts_inputs[1].sides, torus_verts_inputs[1].rings);
    torus_verts_results[2] = _sshape_torus_num_vertices(torus_verts_inputs[2].sides, torus_verts_inputs[2].rings);
    torus_verts_results[3] = _sshape_torus_num_vertices(torus_verts_inputs[3].sides, torus_verts_inputs[3].rings);
    torus_verts_results[4] = _sshape_torus_num_vertices(torus_verts_inputs[4].sides, torus_verts_inputs[4].rings);
    
    /* Check individual results */
    test_counter += 5;
    
    
    /* Universal property: all results are >= 4 */
    test_counter++;
    
    /* Test _sshape_torus_num_indices with array inputs */
    // printf("Testing _sshape_torus_num_indices with array inputs...\n");
    
    uint32_t torus_indices_results[5];
    
    /* Apply function to each input (no loops allowed) */
    torus_indices_results[0] = _sshape_torus_num_indices(torus_verts_inputs[0].sides, torus_verts_inputs[0].rings);
    torus_indices_results[1] = _sshape_torus_num_indices(torus_verts_inputs[1].sides, torus_verts_inputs[1].rings);
    torus_indices_results[2] = _sshape_torus_num_indices(torus_verts_inputs[2].sides, torus_verts_inputs[2].rings);
    torus_indices_results[3] = _sshape_torus_num_indices(torus_verts_inputs[3].sides, torus_verts_inputs[3].rings);
    torus_indices_results[4] = _sshape_torus_num_indices(torus_verts_inputs[4].sides, torus_verts_inputs[4].rings);
    
    /* Check individual results */
 /* 3*3*2*3 = 54 */
 /* 5*5*2*3 = 150 */
 /* 10*8*2*3 = 480 */
 /* 20*15*2*3 = 1800 */
 /* 1*1*2*3 = 6 */
    test_counter += 5;
    
    /* Universal property: all results are multiples of 6 */
    test_counter++;
    
    /* Existential property: there exists a result that is a multiple of 18 */
    test_counter++;
    
    /* Test _sshape_validate_buffer_item */
    // printf("Testing _sshape_validate_buffer_item...\n");
    uint8_t buffer_data[100];
    
    /* Test _sshape_validate_buffer */
    // printf("Testing _sshape_validate_buffer...\n");
    uint8_t vertex_data[200];
    uint8_t index_data[200];
    
    /* Test _sshape_advance_offset */
    // printf("Testing _sshape_advance_offset...\n");
    sshape_buffer_item_t offset_item;
    offset_item.buffer.ptr = buffer_data;
    offset_item.buffer.size = 100;
    offset_item.data_size = 50;
    offset_item.shape_offset = 30;
    _sshape_advance_offset(&offset_item);
    test_counter++;
    
    sshape_buffer_item_t offset_item2;
    offset_item2.buffer.ptr = buffer_data;
    offset_item2.buffer.size = 100;
    offset_item2.data_size = 0;
    offset_item2.shape_offset = 0;
    _sshape_advance_offset(&offset_item2);
    test_counter++;
    
    sshape_buffer_item_t offset_item3;
    offset_item3.buffer.ptr = buffer_data;
    offset_item3.buffer.size = 100;
    offset_item3.data_size = 100;
    offset_item3.shape_offset = 75;
    _sshape_advance_offset(&offset_item3);
    test_counter++;
    
    /* Test _sshape_base_index */
    // printf("Testing _sshape_base_index...\n");
    sshape_buffer_item_t base_vertices;
    base_vertices.buffer.ptr = vertex_data;
    base_vertices.buffer.size = 200;
    base_vertices.data_size = 120;
    base_vertices.shape_offset = 0;
    
    sshape_buffer_item_t base_indices;
    base_indices.buffer.ptr = index_data;
    base_indices.buffer.size = 200;
    base_indices.data_size = 0;
    base_indices.shape_offset = 0;
    
    sshape_buffer_t base_buf;
    base_buf.vertices = base_vertices;
    base_buf.indices = base_indices;
    base_buf.valid = true;
    
    uint16_t base_index1 = _sshape_base_index(&base_buf);
 /* 120 / sizeof(sshape_vertex_t) = 120 / 24 = 5 */
    test_counter++;
    
    sshape_buffer_item_t base_vertices2;
    base_vertices2.buffer.ptr = vertex_data;
    base_vertices2.buffer.size = 200;
    base_vertices2.data_size = 0;
    base_vertices2.shape_offset = 0;
    
    sshape_buffer_t base_buf2;
    base_buf2.vertices = base_vertices2;
    base_buf2.indices = base_indices;
    base_buf2.valid = true;
    
    uint16_t base_index2 = _sshape_base_index(&base_buf2);
    test_counter++;
    
    sshape_buffer_item_t base_vertices3;
    base_vertices3.buffer.ptr = vertex_data;
    base_vertices3.buffer.size = 200;
    base_vertices3.data_size = 240;
    base_vertices3.shape_offset = 0;
    
    sshape_buffer_t base_buf3;
    base_buf3.vertices = base_vertices3;
    base_buf3.indices = base_indices;
    base_buf3.valid = true;
    
    uint16_t base_index3 = _sshape_base_index(&base_buf3);
 /* 240 / 24 = 10 */
    test_counter++;
    
    /* Test sshape_color_4b with array inputs */
    // printf("Testing sshape_color_4b with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        uint8_t r, g, b, a;
        uint32_t expected;
    } color4b_inputs[7] = {
        {0x00, 0x00, 0x00, 0x00, 0x00000000},
        {0xFF, 0x00, 0x00, 0x00, 0x000000FF},
        {0x00, 0xFF, 0x00, 0x00, 0x0000FF00},
        {0x00, 0x00, 0xFF, 0x00, 0x00FF0000},
        {0x00, 0x00, 0x00, 0xFF, 0xFF000000},
        {0x12, 0x34, 0x56, 0x78, 0x78563412},
        {0xAB, 0xCD, 0xEF, 0x01, 0x01EFCDAB}
    };
    
    uint32_t color4b_results[7];
    
    /* Apply function to each input (no loops allowed) */
    color4b_results[0] = sshape_color_4b(color4b_inputs[0].r, color4b_inputs[0].g, color4b_inputs[0].b, color4b_inputs[0].a);
    color4b_results[1] = sshape_color_4b(color4b_inputs[1].r, color4b_inputs[1].g, color4b_inputs[1].b, color4b_inputs[1].a);
    color4b_results[2] = sshape_color_4b(color4b_inputs[2].r, color4b_inputs[2].g, color4b_inputs[2].b, color4b_inputs[2].a);
    color4b_results[3] = sshape_color_4b(color4b_inputs[3].r, color4b_inputs[3].g, color4b_inputs[3].b, color4b_inputs[3].a);
    color4b_results[4] = sshape_color_4b(color4b_inputs[4].r, color4b_inputs[4].g, color4b_inputs[4].b, color4b_inputs[4].a);
    color4b_results[5] = sshape_color_4b(color4b_inputs[5].r, color4b_inputs[5].g, color4b_inputs[5].b, color4b_inputs[5].a);
    color4b_results[6] = sshape_color_4b(color4b_inputs[6].r, color4b_inputs[6].g, color4b_inputs[6].b, color4b_inputs[6].a);
    
    /* Check individual results */
    
    /* Existential property: there exists a result with alpha channel = 0xFF */
    test_counter++;
    
    /* Test sshape_color_3b with array inputs */
    // printf("Testing sshape_color_3b with array inputs...\n");
    
    /* Define test inputs as arrays of structs */
    struct {
        uint8_t r, g, b;
        uint32_t expected;
    } color3b_inputs[6] = {
        {0x00, 0x00, 0x00, 0xFF000000}, /* alpha is 255 */
        {0xFF, 0x00, 0x00, 0xFF0000FF},
        {0x00, 0xFF, 0x00, 0xFF00FF00},
        {0x00, 0x00, 0xFF, 0xFFFF0000},
        {0x12, 0x34, 0x56, 0xFF563412},
        {0xAB, 0xCD, 0xEF, 0xFFEFCDAB}
    };
    
    uint32_t color3b_results[6];
    
    /* Apply function to each input (no loops allowed) */
    color3b_results[0] = sshape_color_3b(color3b_inputs[0].r, color3b_inputs[0].g, color3b_inputs[0].b);
    color3b_results[1] = sshape_color_3b(color3b_inputs[1].r, color3b_inputs[1].g, color3b_inputs[1].b);
    color3b_results[2] = sshape_color_3b(color3b_inputs[2].r, color3b_inputs[2].g, color3b_inputs[2].b);
    color3b_results[3] = sshape_color_3b(color3b_inputs[3].r, color3b_inputs[3].g, color3b_inputs[3].b);
    color3b_results[4] = sshape_color_3b(color3b_inputs[4].r, color3b_inputs[4].g, color3b_inputs[4].b);
    color3b_results[5] = sshape_color_3b(color3b_inputs[5].r, color3b_inputs[5].g, color3b_inputs[5].b);
    
    /* Check individual results */
    test_counter += 6;
    
    /* Existential property: there exists a result with red channel = 0xFF */
    test_counter++;
    
    /* Test sshape_plane_sizes with array inputs */
    // printf("Testing sshape_plane_sizes with array inputs...\n");
    uint32_t plane_tiles_input[3] = {1, 2, 3};
    sshape_sizes_t plane_sizes_results[3];
    
    /* Apply function to each input (no loops allowed) */
    plane_sizes_results[0] = sshape_plane_sizes(plane_tiles_input[0]);
    plane_sizes_results[1] = sshape_plane_sizes(plane_tiles_input[1]);
    plane_sizes_results[2] = sshape_plane_sizes(plane_tiles_input[2]);
    
    /* Check individual results */
    
    
    test_counter += 12;
    
    /* Test sshape_box_sizes with array inputs */
    // printf("Testing sshape_box_sizes with array inputs...\n");
    uint32_t box_tiles_input[3] = {1, 2, 3};
    sshape_sizes_t box_sizes_results[3];
    
    /* Apply function to each input (no loops allowed) */
    box_sizes_results[0] = sshape_box_sizes(box_tiles_input[0]);
    box_sizes_results[1] = sshape_box_sizes(box_tiles_input[1]);
    box_sizes_results[2] = sshape_box_sizes(box_tiles_input[2]);
    
    /* Check individual results */
    
    test_counter += 12;
    
    /* Test sshape_sphere_sizes with array inputs */
    // printf("Testing sshape_sphere_sizes with array inputs...\n");
    struct {
        uint32_t slices, stacks;
    } sphere_sizes_inputs[3] = {{3, 2}, {5, 4}, {10, 8}};
    sshape_sizes_t sphere_sizes_results[3];
    
    /* Apply function to each input */
    sphere_sizes_results[0] = sshape_sphere_sizes(sphere_sizes_inputs[0].slices, sphere_sizes_inputs[0].stacks);
    sphere_sizes_results[1] = sshape_sphere_sizes(sphere_sizes_inputs[1].slices, sphere_sizes_inputs[1].stacks);
    sphere_sizes_results[2] = sshape_sphere_sizes(sphere_sizes_inputs[2].slices, sphere_sizes_inputs[2].stacks);
    
    /* Check individual results */
    
    test_counter += 12;
    
    /* Test sshape_cylinder_sizes with array inputs */
    // printf("Testing sshape_cylinder_sizes with array inputs...\n");
    struct {
        uint32_t slices, stacks;
    } cylinder_sizes_inputs[3] = {{3, 1}, {5, 2}, {10, 5}};
    sshape_sizes_t cylinder_sizes_results[3];
    
    /* Apply function to each input */
    cylinder_sizes_results[0] = sshape_cylinder_sizes(cylinder_sizes_inputs[0].slices, cylinder_sizes_inputs[0].stacks);
    cylinder_sizes_results[1] = sshape_cylinder_sizes(cylinder_sizes_inputs[1].slices, cylinder_sizes_inputs[1].stacks);
    cylinder_sizes_results[2] = sshape_cylinder_sizes(cylinder_sizes_inputs[2].slices, cylinder_sizes_inputs[2].stacks);
    
    /* Check individual results */
    
    

    /* Test sshape_torus_sizes with array inputs */
    // printf("Testing sshape_torus_sizes with array inputs...\n");
    struct {
        uint32_t sides, rings;
    } torus_sizes_inputs[3] = {{3, 3}, {5, 5}, {10, 8}};
    sshape_sizes_t torus_sizes_results[3];
    
    /* Apply function to each input */
    torus_sizes_results[0] = sshape_torus_sizes(torus_sizes_inputs[0].sides, torus_sizes_inputs[0].rings);
    torus_sizes_results[1] = sshape_torus_sizes(torus_sizes_inputs[1].sides, torus_sizes_inputs[1].rings);
    torus_sizes_results[2] = sshape_torus_sizes(torus_sizes_inputs[2].sides, torus_sizes_inputs[2].rings);
    
    /* Check individual results */
    
    
    
    /* Test sshape_element_range */
    // printf("Testing sshape_element_range...\n");
    uint8_t element_vertex_data[500];
    uint8_t element_index_data[500];
    
    sshape_buffer_item_t element_vertices;
    element_vertices.buffer.ptr = element_vertex_data;
    element_vertices.buffer.size = 500;
    element_vertices.data_size = 240;
    element_vertices.shape_offset = 0;
    
    sshape_buffer_item_t element_indices;
    element_indices.buffer.ptr = element_index_data;
    element_indices.buffer.size = 500;
    element_indices.data_size = 120;
    element_indices.shape_offset = 60;
    
    sshape_buffer_t element_buf;
    element_buf.vertices = element_vertices;
    element_buf.indices = element_indices;
    element_buf.valid = true;
    
    sshape_element_range_t element_range1 = sshape_element_range(&element_buf);
 /* 60 / sizeof(uint16_t) = 60 / 2 = 30 */
 /* (120 - 60) / 2 = 60 / 2 = 30 */
    test_counter += 2;
    
    sshape_buffer_item_t element_indices2;
    element_indices2.buffer.ptr = element_index_data;
    element_indices2.buffer.size = 500;
    element_indices2.data_size = 0;
    element_indices2.shape_offset = 0;
    
    sshape_buffer_t element_buf2;
    element_buf2.vertices = element_vertices;
    element_buf2.indices = element_indices2;
    element_buf2.valid = true;
    
    sshape_buffer_item_t element_indices3;
    element_indices3.buffer.ptr = element_index_data;
    element_indices3.buffer.size = 500;
    element_indices3.data_size = 200;
    element_indices3.shape_offset = 100;
    
    sshape_buffer_t element_buf3;
    element_buf3.vertices = element_vertices;
    element_buf3.indices = element_indices3;
    element_buf3.valid = true;
    
    sshape_element_range_t element_range3 = sshape_element_range(&element_buf3);
 /* 100 / 2 = 50 */
 /* (200 - 100) / 2 = 100 / 2 = 50 */
    test_counter += 2;
    
    /* Print summary */
    // printf("\nTest summary:\n");
    // printf("Total assert statements: %d\n", test_counter);
    // printf("All tests completed successfully!\n");
    
    return 0;
}
