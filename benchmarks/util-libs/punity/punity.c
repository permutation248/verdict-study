/** 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015-2016 Martin Cohen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>


typedef unsigned int uint;
typedef uint8_t   u8;
typedef int8_t    i8;
typedef uint16_t  u16;
typedef int16_t   i16;
typedef uint32_t  u32;
typedef int32_t   i32;
typedef uint64_t  u64;
typedef int64_t   i64;
typedef float     f32;
typedef double    f64;
typedef u32       b32;
typedef size_t    usize;
// typedef ptrdiff_t isize;


#define ASSERT assert

#define unused(x) (void)x

#define align_to(value, N) ((value + (N-1)) & ~(N-1))
#define ceil_div(n, a) (((n) + (a-1))/(a))
#define equalf(a, b, epsilon) (fabs(b - a) <= epsilon)

#define maximum(a, b) ((a) > (b) ? (a) : (b))
#define minimum(a, b) ((a) < (b) ? (a) : (b))
#define clamp(x, a, b)  (maximum(a, minimum(x, b)))
#define array_count(a) (sizeof(a) / sizeof((a)[0]))
#define swap_t(T, a, b) do { T tmp__ = a; a = b; b = tmp__; } while (0)

#define lerp(v0, v1, t) ((1-(t))*(v0) + (t)*(v1))


#define kilobytes(n) (1024*(n))
#define megabytes(n) (1024*kilobytes(n))
#define gigabytes(n) (1024*megabytes(n))

struct Bank
{
    u8 *begin;
    u8 *end;
    u8 *it;
    u8 *cap;
};
typedef struct Bank Bank;


struct BankState
{
    Bank state;
    Bank *bank;
};
typedef struct BankState BankState;


struct DequeBlock_
{
    uint8_t *begin;
    uint8_t *end;
    uint8_t *it;
    struct DequeBlock_ *next;
};
typedef struct DequeBlock_ DequeBlock;


struct Deque_
{
    DequeBlock *first;
    DequeBlock *last;
    DequeBlock *pool;
    size_t count;
    size_t block_size;

    // Total number of bytes in deque.
    size_t size;
};
typedef struct Deque_ Deque;


//
// Graphics
//


enum
{
    Edge_Left       = 1 << 0,
    Edge_Top        = 1 << 1,
    Edge_Right      = 1 << 2,
    Edge_Bottom     = 1 << 3,
    Edge_All        = Edge_Left | Edge_Top | Edge_Right | Edge_Bottom,
};


#define PUN_COLOR_TRANSPARENT 0

union Color
{
    struct {
        u8 r, g, b, a;
    };
    u32 rgba;
};
typedef union Color Color;


struct PaletteRange
{
    int begin;
    int end;
    int it;
};
typedef struct PaletteRange PaletteRange;


#define PUN_PALETTE_RANGES_COUNT (16)


struct Palette
{
    Color colors[256];
    PaletteRange ranges[PUN_PALETTE_RANGES_COUNT];
    int ranges_count;
};
typedef struct Palette Palette;


union Rect
{
    struct {
        i32 min_x;
        i32 min_y;
        i32 max_x;
        i32 max_y;
    };
    struct {
        i32 left;
        i32 top;
        i32 right;
        i32 bottom;
    };
    i32 E[4];
};
typedef union Rect Rect;


struct Bitmap
{
    i32 width;
    i32 height;
    i32 pitch;
    u8 *pixels;
    i32 palette_range;
    i32 tile_width;
    i32 tile_height;
};
typedef struct Bitmap Bitmap;


enum {
    DrawFlags_None  = 0,
    DrawFlags_FlipH = 1 << 0,
    DrawFlags_FlipV = 1 << 1,
    DrawFlags_Mask  = 1 << 2,
};


struct Canvas
{
    Bitmap *bitmap;
    i32 translate_x;
    i32 translate_y;
    Rect clip;
    Bitmap *font;
    u32 flags;
    u8 mask;
};
typedef struct Canvas Canvas;


#define PUN_BITMAP_MASK 0
#define PUN_BITMAP_8    1
#define PUN_BITMAP_32   4


typedef struct TextAttr
{
    u8 fg;
    u8 bg;
};
typedef struct TextAttr TextAttr;


enum DrawListItemType
{
    DrawListItemType_Callback,
    DrawListItemType_Line,
    DrawListItemType_Rect,
    DrawListItemType_Frame,
    DrawListItemType_Text,
    DrawListItemType_BitmapFull,
    DrawListItemType_BitmapPartial
};
typedef enum DrawListItemType DrawListItemType;


//
// Sound
//


enum
{
    SoundFlag_Loop        = 1 << 1,
    SoundFlag_FadeOut     = 1 << 2,
    SoundFlag_FadeIn      = 1 << 3,
};


struct Sound
{
    char *name;
    u32 flags;
    f32 volume;
    i32 channels;
    u32 rate;
    i16 *samples;
    // Samples per channel.
    size_t samples_count;
    i32 sources_count;
};
typedef struct Sound Sound;



void bank_clear(Bank *bank)
{
    bank->it = bank->begin;
}


// 
// void bank_pop(Bank *stack, void *ptr)
// {
//     ASSERT(stack);
//     ASSERT(stack->begin);
//     ASSERT((u8 *)ptr >= stack->begin && (u8 *)ptr <= stack->it);
//     stack->it = ptr;
// }



BankState bank_begin(Bank *bank)
{
    BankState state;
    state.bank = bank;
    state.state = *bank;
    return state;
}



void bank_end(BankState *state)
{
    *state->bank = state->state;
}


//
// Deque
//



void deque_init(Deque *deque, size_t block_size)
{
    deque->pool = deque->first = deque->last = 0;
    deque->count = 0;
    deque->block_size = block_size;
    deque->size = 0;
}


//
// Color
//



Color color_make(u8 r, u8 g, u8 b, u8 a)
{
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}



Color color_lerp(Color from, Color to, f32 t)
{
    from.r = (u8)(lerp((f32)from.r, (f32)to.r, t));
    from.g = (u8)(lerp((f32)from.g, (f32)to.g, t));
    from.b = (u8)(lerp((f32)from.b, (f32)to.b, t));
    return from;
}


//
// Palette
//



void palette_init(Palette *palette)
{
    // memset(palette, 0, sizeof(Palette));
    
    for (int i = 0; i < 256; i++)
        palette->colors[i].rgba = 0;
    
    for (int i = 0; i < PUN_PALETTE_RANGES_COUNT; i++) {
        palette->ranges[i].begin = 0;
        palette->ranges[i].end = 0;
        palette->ranges[i].it = 0;
    }
    palette->ranges_count = 0;
    
    // Skip the 0th color, as that's reserved for transparency.
    palette->ranges[0].begin = 1;
    palette->ranges[0].end   = 0xFF;
    palette->ranges[0].it    = 1;
    palette->ranges_count = 1;
}


//
// Rectangle
//



Rect rect_make(i32 min_x, i32 min_y, i32 max_x, i32 max_y)
{
    Rect r;
    r.min_x = min_x;
    r.min_y = min_y;
    r.max_x = max_x;
    r.max_y = max_y;
    return r;
}



Rect rect_make_size(i32 x, i32 y, i32 w, i32 h)
{
    Rect r = rect_make(x, y, x + w, y + h);
    return r;
}



Rect rect_make_centered(i32 x, i32 y, i32 w, i32 h)
{
    w = w / 2; h = h / 2;
    Rect r = rect_make(x - w, y - h, x + w, y + h);
    return r;
}



Rect rect_clip(Rect rect, Rect clip)
{
    rect.min_x = maximum(clip.min_x,     minimum(clip.max_x, rect.min_x));
    rect.min_y = maximum(clip.min_y,     minimum(clip.max_y, rect.min_y));
    rect.max_x = maximum(rect.min_x,     minimum(clip.max_x, rect.max_x));
    rect.max_y = maximum(rect.min_y,     minimum(clip.max_y, rect.max_y));
    return rect;
}



bool rect_contains_point(Rect rect, i32 x, i32 y)
{
    return (x >= rect.min_x && x < rect.max_x &&
            y >= rect.min_y && y < rect.max_y);
}



bool rect_overlaps(Rect rect_a, Rect rect_b)
{
    return !(((rect_b.min_x - rect_a.max_x) > 0 || (rect_b.min_y - rect_a.max_y) > 0) ||
             ((rect_a.min_x - rect_b.max_x) > 0 || (rect_a.min_y - rect_b.max_y) > 0));
}



void rect_center(Rect rect, i32 *x, i32 *y) {
    *x = (rect.min_x + rect.max_x) / 2;
    *y = (rect.min_y + rect.max_y) / 2;
}



bool rect_check_limits(Rect *rect, i32 min_x, i32 min_y, i32 max_x, i32 max_y)
{
    return (rect->min_x >= min_x) && (rect->min_y >= min_y) &&
           (rect->max_x <= max_x) && (rect->max_y <= max_y) &&
           (rect->min_x <= rect->max_x) &&
           (rect->min_y <= rect->max_y);
}