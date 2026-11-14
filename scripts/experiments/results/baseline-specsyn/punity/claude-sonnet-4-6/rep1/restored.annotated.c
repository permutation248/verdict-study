#include <assert.h>

#include <errno.h>

#include <stdarg.h>

#include <stddef.h>

#include <stdint.h>

#include <stdio.h>

typedef uint8_t u8;

typedef uint32_t u32;

typedef int32_t i32;

typedef float f32;

struct Bank {
	u8 *begin ;
	u8 *end ;
	u8 *it ;
	u8 *cap ;
};

typedef struct Bank Bank;

struct BankState {
	Bank state ;
	Bank *bank ;
};

typedef struct BankState BankState;

struct DequeBlock_ {
	uint8_t *begin ;
	uint8_t *end ;
	uint8_t *it ;
	struct DequeBlock_ *next ;
};

typedef struct DequeBlock_ DequeBlock;

struct Deque_ {
	DequeBlock *first ;
	DequeBlock *last ;
	DequeBlock *pool ;
	size_t count ;
	size_t block_size ;
	size_t size ;
};

typedef struct Deque_ Deque;

struct __anonstruct_2 {
	u8 r ;
	u8 g ;
	u8 b ;
	u8 a ;
};

union Color {
	struct __anonstruct_2 __anonCompField1 ;
	u32 rgba ;
};

typedef union Color Color;

struct PaletteRange {
	int begin ;
	int end ;
	int it ;
};

typedef struct PaletteRange PaletteRange;

struct Palette {
	Color colors[256] ;
	PaletteRange ranges[16] ;
	int ranges_count ;
};

typedef struct Palette Palette;

struct __anonstruct_3 {
	i32 min_x ;
	i32 min_y ;
	i32 max_x ;
	i32 max_y ;
};

struct __anonstruct_4 {
	i32 left ;
	i32 top ;
	i32 right ;
	i32 bottom ;
};

union Rect {
	struct __anonstruct_3 __anonCompField2 ;
	struct __anonstruct_4 __anonCompField3 ;
	i32 E[4] ;
};

typedef union Rect Rect;

/*@
	requires \valid(bank);
	requires \valid(&bank->it);
	requires \valid_read(&bank->begin);
	assigns bank->it;
	ensures bank->it == bank->begin;
	ensures bank->begin == \old(bank->begin);
	ensures bank->end   == \old(bank->end);
	ensures bank->cap   == \old(bank->cap);
*/
void bank_clear(Bank *bank)
{
/*@ assert rte_2: mem_access: \valid(&bank->it); */
/*@ assert rte_3: mem_access: \valid_read(&bank->begin); */
	bank->it = bank->begin;
	return;
}

/*@
	requires \valid_read(bank);
	assigns \nothing;
	ensures \result.bank == bank;
	ensures \valid_read(bank) ==> \result.state.begin == bank->begin;
	ensures \valid_read(bank) ==> \result.state.end   == bank->end;
	ensures \valid_read(bank) ==> \result.state.it    == bank->it;
	ensures \valid_read(bank) ==> \result.state.cap   == bank->cap;
*/
BankState bank_begin(Bank *bank)
{
	BankState state;
	state.bank = bank;
/*@ assert rte_1: mem_access: \valid_read(bank); */
	state.state = *bank;
	return state;
}

/*@
	requires \valid(state);
	requires \valid(state->bank);
	requires \separated(state->bank, &state->state);
	assigns *(state->bank);
	assigns state->bank->begin,state->bank->end,state->bank->it,state->bank->cap;
	ensures state->state.begin == \old(state->state.begin);
	ensures state->state.end == \old(state->state.end);
	ensures state->state.it == \old(state->state.it);
	ensures state->state.cap == \old(state->state.cap);
	ensures state->state.end   == \old(state->state.end);
	ensures state->state.it    == \old(state->state.it);
	ensures state->state.cap   == \old(state->state.cap);
	ensures \old(\valid(state->bank)) ==>(\old(state->bank))->begin == \old(state->state.begin);
	ensures \old(\valid(state->bank)) ==>(\old(state->bank))->end == \old(state->state.end);
	ensures \old(\valid(state->bank)) ==>(\old(state->bank))->it == \old(state->state.it);
	ensures \old(\valid(state->bank)) ==>(\old(state->bank))->cap == \old(state->state.cap);
*/
void bank_end(BankState *state)
{
/*@ assert rte_4: mem_access: \valid(state->bank); */
/*@ assert rte_5: mem_access: \valid_read(&state->bank); */
/*@ assert rte_6: mem_access: \valid_read(&state->state); */
	*(state->bank) = state->state;
	return;
}

/*@
	requires \valid(deque);
	assigns deque->first, deque->last, deque->pool,@         deque->count, deque->block_size, deque->size;
	ensures deque->last       == \null;
	ensures deque->first      == \null;
	ensures deque->pool       == \null;
	ensures deque->count      == 0;
	ensures deque->block_size == block_size;
	ensures deque->size       == 0;
*/
void deque_init(Deque *deque, size_t block_size)
{
	DequeBlock *tmp_0;
	DequeBlock *tmp;
	tmp_0 = (DequeBlock *)0;
/*@ assert rte_19: mem_access: \valid(&deque->last); */
	deque->last = tmp_0;
	tmp = tmp_0;
/*@ assert rte_20: mem_access: \valid(&deque->first); */
	deque->first = tmp;
/*@ assert rte_21: mem_access: \valid(&deque->pool); */
	deque->pool = tmp;
/*@ assert rte_22: mem_access: \valid(&deque->count); */
	deque->count = (size_t)0;
/*@ assert rte_23: mem_access: \valid(&deque->block_size); */
	deque->block_size = block_size;
/*@ assert rte_24: mem_access: \valid(&deque->size); */
	deque->size = (size_t)0;
	return;
}

/*@
	requires 0 <= r <= 255;
	requires 0 <= g <= 255;
	requires 0 <= b <= 255;
	requires 0 <= a <= 255;
	assigns \nothing;
	ensures \result.__anonCompField1.r == r;
	ensures \result.__anonCompField1.g == g;
	ensures \result.__anonCompField1.b == b;
	ensures \result.__anonCompField1.a == a;
*/
Color color_make(u8 r, u8 g, u8 b, u8 a)
{
	Color c;
	c.__anonCompField1.r = r;
	c.__anonCompField1.g = g;
	c.__anonCompField1.b = b;
	c.__anonCompField1.a = a;
	return c;
}

/*@
	requires \is_finite(t);
	requires 0.0f <= t <= 1.0f;
	assigns \nothing;
	ensures \result.__anonCompField1.a == from.__anonCompField1.a;
	ensures (t == 0.0f) ==>\result.__anonCompField1.r == from.__anonCompField1.r;
	ensures (t == 0.0f) ==>\result.__anonCompField1.g == from.__anonCompField1.g;
	ensures (t == 0.0f) ==>\result.__anonCompField1.b == from.__anonCompField1.b;
	ensures (t == 1.0f) ==>\result.__anonCompField1.r == to.__anonCompField1.r;
	ensures (t == 1.0f) ==>\result.__anonCompField1.g == to.__anonCompField1.g;
	ensures (t == 1.0f) ==>\result.__anonCompField1.b == to.__anonCompField1.b;
	ensures (0.0f <= t <= 1.0f) ==>\result.__anonCompField1.r ==(u8)(((f32)1 - t) * (f32)from.__anonCompField1.r +t * (f32)to.__anonCompField1.r);
	ensures (0.0f <= t <= 1.0f) ==>\result.__anonCompField1.g ==(u8)(((f32)1 - t) * (f32)from.__anonCompField1.g +t * (f32)to.__anonCompField1.g);
	ensures (0.0f <= t <= 1.0f) ==>\result.__anonCompField1.b ==(u8)(((f32)1 - t) * (f32)from.__anonCompField1.b +t * (f32)to.__anonCompField1.b);
	ensures 0 <= \result.__anonCompField1.r <= 255;
	ensures 0 <= \result.__anonCompField1.g <= 255;
	ensures 0 <= \result.__anonCompField1.b <= 255;
*/
Color color_lerp(Color from, Color to, f32 t)
{
/*@ assert rte_7: is_nan_or_infinite: \is_finite(\sub_float((f32)1, t)); */
/*@ assert rte_8: is_nan_or_infinite: \is_finite(\mul_float(\sub_float((f32)1, t), (f32)from.__anonCompField1.r)); */
/*@ assert rte_9: is_nan_or_infinite: \is_finite(\mul_float(t, (f32)to.__anonCompField1.r)); */
/*@ assert rte_10: is_nan_or_infinite: \is_finite(\add_float(\mul_float(\sub_float((f32)1, t), (f32)from.__anonCompField1.r), \mul_float(t, (f32)to.__anonCompField1.r))); */
	from.__anonCompField1.r = (u8)(((f32)1 - t) * (f32)from.__anonCompField1.r +
	t * (f32)to.__anonCompField1.r);
/*@ assert rte_11: is_nan_or_infinite: \is_finite(\sub_float((f32)1, t)); */
/*@ assert rte_12: is_nan_or_infinite: \is_finite(\mul_float(\sub_float((f32)1, t), (f32)from.__anonCompField1.g)); */
/*@ assert rte_13: is_nan_or_infinite: \is_finite(\mul_float(t, (f32)to.__anonCompField1.g)); */
/*@ assert rte_14: is_nan_or_infinite: \is_finite(\add_float(\mul_float(\sub_float((f32)1, t), (f32)from.__anonCompField1.g), \mul_float(t, (f32)to.__anonCompField1.g))); */
	from.__anonCompField1.g = (u8)(((f32)1 - t) * (f32)from.__anonCompField1.g +
	t * (f32)to.__anonCompField1.g);
/*@ assert rte_15: is_nan_or_infinite: \is_finite(\sub_float((f32)1, t)); */
/*@ assert rte_16: is_nan_or_infinite: \is_finite(\mul_float(\sub_float((f32)1, t), (f32)from.__anonCompField1.b)); */
/*@ assert rte_17: is_nan_or_infinite: \is_finite(\mul_float(t, (f32)to.__anonCompField1.b)); */
/*@ assert rte_18: is_nan_or_infinite: \is_finite(\add_float(\mul_float(\sub_float((f32)1, t), (f32)from.__anonCompField1.b), \mul_float(t, (f32)to.__anonCompField1.b))); */
	from.__anonCompField1.b = (u8)(((f32)1 - t) * (f32)from.__anonCompField1.b +
	t * (f32)to.__anonCompField1.b);
	return from;
}

/*@
	requires \valid(palette->ranges + (0..15));
	requires \valid(palette->colors + (0..255));
	requires \valid(palette);
	assigns palette->colors[0..255].rgba,palette->ranges[0..15].begin,palette->ranges[0..15].end,palette->ranges[0..15].it,palette->ranges_count;
	ensures palette->ranges[0].begin == 1;
	ensures palette->ranges[0].end   == 0xFF;
	ensures palette->ranges[0].it    == 1;
	ensures palette->ranges_count    == 1;
*/
void palette_init(Palette *palette)
{
	{
		int i = 0;
		/*@
			loop invariant 0 <= i <= 256;
			loop assigns i, palette->colors[0..255].rgba;
			loop assigns i, palette->colors[0..255];
			loop variant 256 - i;
		*/
		while (i < 256) {
		/*@ assert rte_25: index_bound: 0 <= i; */
		/*@ assert rte_26: index_bound: i < 256; */
		/*@ assert rte_27: mem_access: \valid(&palette->colors[i].rgba); */
			palette->colors[i].rgba = (u32)0;
		/*@ assert rte_28: signed_overflow: i + 1 <= 2147483647; */
			i ++;
		}
	}
	{
		int i_0 = 0;
		/*@
			loop invariant 0 <= i_0 <= 16;
			loop assigns i_0,palette->ranges[0..15].begin,palette->ranges[0..15].end,palette->ranges[0..15].it;
			loop assigns i_0, palette->ranges[0..15];
			loop variant 16 - i_0;
		*/
		while (i_0 < 16) {
		/*@ assert rte_29: index_bound: 0 <= i_0; */
		/*@ assert rte_30: index_bound: i_0 < 16; */
		/*@ assert rte_31: mem_access: \valid(&palette->ranges[i_0].begin); */
			palette->ranges[i_0].begin = 0;
		/*@ assert rte_32: index_bound: 0 <= i_0; */
		/*@ assert rte_33: index_bound: i_0 < 16; */
		/*@ assert rte_34: mem_access: \valid(&palette->ranges[i_0].end); */
			palette->ranges[i_0].end = 0;
		/*@ assert rte_35: index_bound: 0 <= i_0; */
		/*@ assert rte_36: index_bound: i_0 < 16; */
		/*@ assert rte_37: mem_access: \valid(&palette->ranges[i_0].it); */
			palette->ranges[i_0].it = 0;
		/*@ assert rte_38: signed_overflow: i_0 + 1 <= 2147483647; */
			i_0 ++;
		}
	}
/*@ assert rte_39: mem_access: \valid(&palette->ranges_count); */
	palette->ranges_count = 0;
/*@ assert rte_40: mem_access: \valid(&palette->ranges[0].begin); */
	palette->ranges[0].begin = 1;
/*@ assert rte_41: mem_access: \valid(&palette->ranges[0].end); */
	palette->ranges[0].end = 0xFF;
/*@ assert rte_42: mem_access: \valid(&palette->ranges[0].it); */
	palette->ranges[0].it = 1;
/*@ assert rte_43: mem_access: \valid(&palette->ranges_count); */
	palette->ranges_count = 1;
	return;
}

/*@
	requires INT32_MIN <= min_x <= INT32_MAX;
	requires INT32_MIN <= min_y <= INT32_MAX;
	requires INT32_MIN <= max_x <= INT32_MAX;
	requires INT32_MIN <= max_y <= INT32_MAX;
	assigns \nothing;
	ensures \result.__anonCompField2.min_x == min_x;
	ensures \result.__anonCompField2.min_y == min_y;
	ensures \result.__anonCompField2.max_x == max_x;
	ensures \result.__anonCompField2.max_y == max_y;
	ensures \result.E[0] == min_x;
	ensures \result.E[1] == min_y;
	ensures \result.E[2] == max_x;
	ensures \result.E[3] == max_y;
*/
Rect rect_make(i32 min_x, i32 min_y, i32 max_x, i32 max_y)
{
	Rect r;
	r.__anonCompField2.min_x = min_x;
	r.__anonCompField2.min_y = min_y;
	r.__anonCompField2.max_x = max_x;
	r.__anonCompField2.max_y = max_y;
	return r;
}

/*@
	requires INT32_MIN <= x <= INT32_MAX;
	requires INT32_MIN <= y <= INT32_MAX;
	requires INT32_MIN <= w <= INT32_MAX;
	requires INT32_MIN <= h <= INT32_MAX;
	requires INT32_MIN <= x + w <= INT32_MAX;
	requires INT32_MIN <= y + h <= INT32_MAX;
	assigns \nothing;
	ensures \result.__anonCompField2.min_x == x;
	ensures \result.__anonCompField2.min_y == y;
	ensures \result.__anonCompField2.max_x == x + w;
	ensures \result.__anonCompField2.max_y == y + h;
	ensures \result.E[0] == x;
	ensures \result.E[1] == y;
	ensures \result.E[2] == x + w;
	ensures \result.E[3] == y + h;
*/
Rect rect_make_size(i32 x, i32 y, i32 w, i32 h)
{
/*@ assert rte_66: signed_overflow: -2147483648 <= x + w; */
/*@ assert rte_67: signed_overflow: x + w <= 2147483647; */
/*@ assert rte_68: signed_overflow: -2147483648 <= y + h; */
/*@ assert rte_69: signed_overflow: y + h <= 2147483647; */
	Rect r = rect_make(x,y,x + w,y + h);
	return r;
}

/*@
	requires INT32_MIN <= x - (w / 2) <= INT32_MAX;
	requires INT32_MIN <= x + (w / 2) <= INT32_MAX;
	requires INT32_MIN <= y - (h / 2) <= INT32_MAX;
	requires INT32_MIN <= y + (h / 2) <= INT32_MAX;
	assigns \nothing;
	ensures \result.__anonCompField2.min_x == \at(x, Pre) - \at(w, Pre) / 2;
	ensures \result.__anonCompField2.min_y == \at(y, Pre) - \at(h, Pre) / 2;
	ensures \result.__anonCompField2.max_x == \at(x, Pre) + \at(w, Pre) / 2;
	ensures \result.__anonCompField2.max_y == \at(y, Pre) + \at(h, Pre) / 2;
	ensures \result.__anonCompField3.left   == \at(x, Pre) - \at(w, Pre) / 2;
	ensures \result.__anonCompField3.top    == \at(y, Pre) - \at(h, Pre) / 2;
	ensures \result.__anonCompField3.right  == \at(x, Pre) + \at(w, Pre) / 2;
	ensures \result.__anonCompField3.bottom == \at(y, Pre) + \at(h, Pre) / 2;
	ensures \result.E[0] == \at(x, Pre) - \at(w, Pre) / 2;
	ensures \result.E[1] == \at(y, Pre) - \at(h, Pre) / 2;
	ensures \result.E[2] == \at(x, Pre) + \at(w, Pre) / 2;
	ensures \result.E[3] == \at(y, Pre) + \at(h, Pre) / 2;
*/
Rect rect_make_centered(i32 x, i32 y, i32 w, i32 h)
{
	w /= 2;
	h /= 2;
/*@ assert rte_58: signed_overflow: -2147483648 <= x - w; */
/*@ assert rte_59: signed_overflow: x - w <= 2147483647; */
/*@ assert rte_60: signed_overflow: -2147483648 <= y - h; */
/*@ assert rte_61: signed_overflow: y - h <= 2147483647; */
/*@ assert rte_62: signed_overflow: -2147483648 <= x + w; */
/*@ assert rte_63: signed_overflow: x + w <= 2147483647; */
/*@ assert rte_64: signed_overflow: -2147483648 <= y + h; */
/*@ assert rte_65: signed_overflow: y + h <= 2147483647; */
	Rect r = rect_make(x - w,y - h,x + w,y + h);
	return r;
}

/*@
	requires INT32_MIN <= clip.__anonCompField2.max_y <= INT32_MAX;
	requires INT32_MIN <= clip.__anonCompField2.max_x <= INT32_MAX;
	requires INT32_MIN <= clip.__anonCompField2.min_y <= INT32_MAX;
	requires INT32_MIN <= clip.__anonCompField2.min_x <= INT32_MAX;
	requires INT32_MIN <= rect.__anonCompField2.max_y <= INT32_MAX;
	requires INT32_MIN <= rect.__anonCompField2.max_x <= INT32_MAX;
	requires INT32_MIN <= rect.__anonCompField2.min_y <= INT32_MAX;
	requires INT32_MIN <= rect.__anonCompField2.min_x <= INT32_MAX;
	requires clip.__anonCompField2.min_y <= clip.__anonCompField2.max_y;
	requires clip.__anonCompField2.min_x <= clip.__anonCompField2.max_x;
	assigns \nothing;
	ensures \result.__anonCompField2.min_x <= \result.__anonCompField2.max_x;
	ensures \result.__anonCompField2.min_y <= \result.__anonCompField2.max_y;
	ensures \result.__anonCompField2.min_x >= clip.__anonCompField2.min_x;
	ensures \result.__anonCompField2.min_x <= clip.__anonCompField2.max_x;
	ensures \result.__anonCompField2.min_y >= clip.__anonCompField2.min_y;
	ensures \result.__anonCompField2.min_y <= clip.__anonCompField2.max_y;
	ensures \result.__anonCompField2.max_x <= clip.__anonCompField2.max_x;
	ensures \result.__anonCompField2.max_y <= clip.__anonCompField2.max_y;
	ensures (rect.__anonCompField2.min_x >= clip.__anonCompField2.min_x &&rect.__anonCompField2.min_x <= clip.__anonCompField2.max_x) ==>\result.__anonCompField2.min_x == rect.__anonCompField2.min_x;
	ensures (rect.__anonCompField2.min_x < clip.__anonCompField2.min_x) ==>\result.__anonCompField2.min_x == clip.__anonCompField2.min_x;
	ensures (rect.__anonCompField2.min_x > clip.__anonCompField2.max_x) ==>\result.__anonCompField2.min_x == clip.__anonCompField2.max_x;
	ensures (rect.__anonCompField2.min_y >= clip.__anonCompField2.min_y &&rect.__anonCompField2.min_y <= clip.__anonCompField2.max_y) ==>\result.__anonCompField2.min_y == rect.__anonCompField2.min_y;
	ensures (rect.__anonCompField2.min_y < clip.__anonCompField2.min_y) ==>\result.__anonCompField2.min_y == clip.__anonCompField2.min_y;
	ensures (rect.__anonCompField2.min_y > clip.__anonCompField2.max_y) ==>\result.__anonCompField2.min_y == clip.__anonCompField2.max_y;
	ensures (rect.__anonCompField2.max_x < clip.__anonCompField2.min_x) ==>\result.__anonCompField2.max_x == \result.__anonCompField2.min_x;
	ensures (rect.__anonCompField2.max_x > clip.__anonCompField2.max_x &&clip.__anonCompField2.max_x >= \result.__anonCompField2.min_x) ==>\result.__anonCompField2.max_x == clip.__anonCompField2.max_x;
	ensures (rect.__anonCompField2.max_x > clip.__anonCompField2.max_x &&clip.__anonCompField2.max_x < \result.__anonCompField2.min_x) ==>\result.__anonCompField2.max_x == \result.__anonCompField2.min_x;
	ensures (rect.__anonCompField2.max_x >= clip.__anonCompField2.min_x &&rect.__anonCompField2.max_x <= clip.__anonCompField2.max_x &&rect.__anonCompField2.max_x >= \result.__anonCompField2.min_x) ==>\result.__anonCompField2.max_x == rect.__anonCompField2.max_x;
	ensures (rect.__anonCompField2.max_x >= clip.__anonCompField2.min_x &&rect.__anonCompField2.max_x <= clip.__anonCompField2.max_x &&rect.__anonCompField2.max_x < \result.__anonCompField2.min_x) ==>\result.__anonCompField2.max_x == \result.__anonCompField2.min_x;
	ensures (rect.__anonCompField2.max_y < clip.__anonCompField2.min_y) ==>\result.__anonCompField2.max_y == \result.__anonCompField2.min_y;
	ensures (rect.__anonCompField2.max_y > clip.__anonCompField2.max_y &&clip.__anonCompField2.max_y >= \result.__anonCompField2.min_y) ==>\result.__anonCompField2.max_y == clip.__anonCompField2.max_y;
	ensures (rect.__anonCompField2.max_y > clip.__anonCompField2.max_y &&clip.__anonCompField2.max_y < \result.__anonCompField2.min_y) ==>\result.__anonCompField2.max_y == \result.__anonCompField2.min_y;
	ensures (rect.__anonCompField2.max_y >= clip.__anonCompField2.min_y &&rect.__anonCompField2.max_y <= clip.__anonCompField2.max_y &&rect.__anonCompField2.max_y >= \result.__anonCompField2.min_y) ==>\result.__anonCompField2.max_y == rect.__anonCompField2.max_y;
	ensures (rect.__anonCompField2.max_y >= clip.__anonCompField2.min_y &&rect.__anonCompField2.max_y <= clip.__anonCompField2.max_y &&rect.__anonCompField2.max_y < \result.__anonCompField2.min_y) ==>\result.__anonCompField2.max_y == \result.__anonCompField2.min_y;
*/
Rect rect_clip(Rect rect, Rect clip)
{
	i32 tmp;
	i32 tmp_1;
	i32 tmp_3;
	i32 tmp_5;
	if (clip.__anonCompField2.max_x < rect.__anonCompField2.min_x) tmp = clip.__anonCompField2.max_x;
	else tmp = rect.__anonCompField2.min_x;
	;
	if (clip.__anonCompField2.min_x > tmp) rect.__anonCompField2.min_x = clip.__anonCompField2.min_x;
	else {
		i32 tmp_0;
		if (clip.__anonCompField2.max_x < rect.__anonCompField2.min_x) tmp_0 = clip.__anonCompField2.max_x;
		else tmp_0 = rect.__anonCompField2.min_x;
		rect.__anonCompField2.min_x = tmp_0;
	}
	if (clip.__anonCompField2.max_y < rect.__anonCompField2.min_y) tmp_1 = clip.__anonCompField2.max_y;
	else tmp_1 = rect.__anonCompField2.min_y;
	;
	if (clip.__anonCompField2.min_y > tmp_1) rect.__anonCompField2.min_y = clip.__anonCompField2.min_y;
	else {
		i32 tmp_2;
		if (clip.__anonCompField2.max_y < rect.__anonCompField2.min_y) tmp_2 = clip.__anonCompField2.max_y;
		else tmp_2 = rect.__anonCompField2.min_y;
		rect.__anonCompField2.min_y = tmp_2;
	}
	if (clip.__anonCompField2.max_x < rect.__anonCompField2.max_x) tmp_3 = clip.__anonCompField2.max_x;
	else tmp_3 = rect.__anonCompField2.max_x;
	;
	if (rect.__anonCompField2.min_x > tmp_3) rect.__anonCompField2.max_x = rect.__anonCompField2.min_x;
	else {
		i32 tmp_4;
		if (clip.__anonCompField2.max_x < rect.__anonCompField2.max_x) tmp_4 = clip.__anonCompField2.max_x;
		else tmp_4 = rect.__anonCompField2.max_x;
		rect.__anonCompField2.max_x = tmp_4;
	}
	if (clip.__anonCompField2.max_y < rect.__anonCompField2.max_y) tmp_5 = clip.__anonCompField2.max_y;
	else tmp_5 = rect.__anonCompField2.max_y;
	;
	if (rect.__anonCompField2.min_y > tmp_5) rect.__anonCompField2.max_y = rect.__anonCompField2.min_y;
	else {
		i32 tmp_6;
		if (clip.__anonCompField2.max_y < rect.__anonCompField2.max_y) tmp_6 = clip.__anonCompField2.max_y;
		else tmp_6 = rect.__anonCompField2.max_y;
		rect.__anonCompField2.max_y = tmp_6;
	}
	return rect;
}

/*@
	requires rect.__anonCompField2.min_x < rect.__anonCompField2.max_x;
	requires rect.__anonCompField2.min_y < rect.__anonCompField2.max_y;
	assigns \nothing;
	ensures (x >= rect.__anonCompField2.min_x &&x <  rect.__anonCompField2.max_x &&y >= rect.__anonCompField2.min_y &&y <  rect.__anonCompField2.max_y)==> \result == 1;
	ensures (x <  rect.__anonCompField2.min_x ||x >= rect.__anonCompField2.max_x ||y <  rect.__anonCompField2.min_y ||y >= rect.__anonCompField2.max_y)==> \result == 0;
*/
_Bool rect_contains_point(Rect rect, i32 x, i32 y)
{
	_Bool __retres;
	int tmp;
	if (x >= rect.__anonCompField2.min_x)
	if (x < rect.__anonCompField2.max_x)
	if (y >= rect.__anonCompField2.min_y)
	if (y < rect.__anonCompField2.max_y) tmp = 1; else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	__retres = (_Bool)(tmp != 0);
	return __retres;
}

/*@
	requires @   rect_a.__anonCompField2.min_x <= rect_a.__anonCompField2.max_x &&@   rect_a.__anonCompField2.min_y <= rect_a.__anonCompField2.max_y &&@   rect_b.__anonCompField2.min_x <= rect_b.__anonCompField2.max_x &&@   rect_b.__anonCompField2.min_y <= rect_b.__anonCompField2.max_y;
	requires @   -1073741824 <= rect_a.__anonCompField2.min_x <= 1073741824 &&@   -1073741824 <= rect_a.__anonCompField2.min_y <= 1073741824 &&@   -1073741824 <= rect_a.__anonCompField2.max_x <= 1073741824 &&@   -1073741824 <= rect_a.__anonCompField2.max_y <= 1073741824 &&@   -1073741824 <= rect_b.__anonCompField2.min_x <= 1073741824 &&@   -1073741824 <= rect_b.__anonCompField2.min_y <= 1073741824 &&@   -1073741824 <= rect_b.__anonCompField2.max_x <= 1073741824 &&@   -1073741824 <= rect_b.__anonCompField2.max_y <= 1073741824;
	assigns \nothing;
*/
_Bool rect_overlaps(Rect rect_a, Rect rect_b)
{
	_Bool __retres;
	int tmp;
/*@ assert rte_70: signed_overflow: -2147483648 <= rect_b.__anonCompField2.min_x - rect_a.__anonCompField2.max_x; */
/*@ assert rte_71: signed_overflow: rect_b.__anonCompField2.min_x - rect_a.__anonCompField2.max_x <= 2147483647; */
	if (rect_b.__anonCompField2.min_x - rect_a.__anonCompField2.max_x > 0)
	tmp = 0;
	else
/*@ assert rte_72: signed_overflow: -2147483648 <= rect_b.__anonCompField2.min_y - rect_a.__anonCompField2.max_y; */
/*@ assert rte_73: signed_overflow: rect_b.__anonCompField2.min_y - rect_a.__anonCompField2.max_y <= 2147483647; */
	if (rect_b.__anonCompField2.min_y - rect_a.__anonCompField2.max_y > 0)
	tmp = 0;
	else
/*@ assert rte_74: signed_overflow: -2147483648 <= rect_a.__anonCompField2.min_x - rect_b.__anonCompField2.max_x; */
/*@ assert rte_75: signed_overflow: rect_a.__anonCompField2.min_x - rect_b.__anonCompField2.max_x <= 2147483647; */
	if (rect_a.__anonCompField2.min_x - rect_b.__anonCompField2.max_x > 0)
	tmp = 0;
	else
/*@ assert rte_76: signed_overflow: -2147483648 <= rect_a.__anonCompField2.min_y - rect_b.__anonCompField2.max_y; */
/*@ assert rte_77: signed_overflow: rect_a.__anonCompField2.min_y - rect_b.__anonCompField2.max_y <= 2147483647; */
	if (rect_a.__anonCompField2.min_y - rect_b.__anonCompField2.max_y > 0)
	tmp = 0;
	else tmp = 1;
	__retres = (_Bool)(tmp != 0);
	return __retres;
}

/*@
	requires -2147483648 <= rect.__anonCompField2.min_y + rect.__anonCompField2.max_y <= 2147483647;
	requires -2147483648 <= rect.__anonCompField2.min_x + rect.__anonCompField2.max_x <= 2147483647;
	requires \valid(y);
	requires \valid(x);
	assigns *x, *y;
	ensures *y == (rect.__anonCompField2.min_y + rect.__anonCompField2.max_y) / 2;
	ensures (rect.__anonCompField2.min_y + rect.__anonCompField2.max_y >= 0) ==>(2 * (*y) <= rect.__anonCompField2.min_y + rect.__anonCompField2.max_y &&rect.__anonCompField2.min_y + rect.__anonCompField2.max_y <= 2 * (*y) + 1);
	ensures (rect.__anonCompField2.min_y + rect.__anonCompField2.max_y < 0) ==>(2 * (*y) >= rect.__anonCompField2.min_y + rect.__anonCompField2.max_y &&rect.__anonCompField2.min_y + rect.__anonCompField2.max_y >= 2 * (*y) - 1);
	ensures 2 * (*y) == rect.__anonCompField2.min_y + rect.__anonCompField2.max_y ||2 * (*y) == rect.__anonCompField2.min_y + rect.__anonCompField2.max_y - 1 ||2 * (*y) == rect.__anonCompField2.min_y + rect.__anonCompField2.max_y + 1;
	ensures (rect.__anonCompField2.min_y <= rect.__anonCompField2.max_y) ==>(rect.__anonCompField2.min_y <= *y <= rect.__anonCompField2.max_y);
	ensures (rect.__anonCompField2.min_y > rect.__anonCompField2.max_y) ==>(rect.__anonCompField2.max_y <= *y <= rect.__anonCompField2.min_y);
*/
void rect_center(Rect rect, i32 *x, i32 *y)
{
/*@ assert rte_44: mem_access: \valid(x); */
/*@ assert rte_45: signed_overflow: -2147483648 <= rect.__anonCompField2.min_x + rect.__anonCompField2.max_x; */
/*@ assert rte_46: signed_overflow: rect.__anonCompField2.min_x + rect.__anonCompField2.max_x <= 2147483647; */
	*x = (rect.__anonCompField2.min_x + rect.__anonCompField2.max_x) / 2;
/*@ assert rte_47: mem_access: \valid(y); */
/*@ assert rte_48: signed_overflow: -2147483648 <= rect.__anonCompField2.min_y + rect.__anonCompField2.max_y; */
/*@ assert rte_49: signed_overflow: rect.__anonCompField2.min_y + rect.__anonCompField2.max_y <= 2147483647; */
	*y = (rect.__anonCompField2.min_y + rect.__anonCompField2.max_y) / 2;
	return;
}

/*@
	requires \valid_read(rect);
	assigns \nothing;
	ensures (rect->__anonCompField2.min_x >= min_x &&rect->__anonCompField2.min_y >= min_y &&rect->__anonCompField2.max_x <= max_x &&rect->__anonCompField2.max_y <= max_y &&rect->__anonCompField2.min_x <= rect->__anonCompField2.max_x &&rect->__anonCompField2.min_y <= rect->__anonCompField2.max_y)==> \result == 1;
	ensures (rect->__anonCompField2.min_x < min_x ||rect->__anonCompField2.min_y < min_y ||rect->__anonCompField2.max_x > max_x ||rect->__anonCompField2.max_y > max_y ||rect->__anonCompField2.min_x > rect->__anonCompField2.max_x ||rect->__anonCompField2.min_y > rect->__anonCompField2.max_y)==> \result == 0;
	ensures \result == 0 || \result == 1;
*/
_Bool rect_check_limits(Rect *rect, i32 min_x, i32 min_y, i32 max_x,
i32 max_y)
{
	_Bool __retres;
	int tmp;
/*@ assert rte_50: mem_access: \valid_read(&rect->__anonCompField2.min_x); */
	if (rect->__anonCompField2.min_x >= min_x)
/*@ assert rte_51: mem_access: \valid_read(&rect->__anonCompField2.min_y); */
	if (rect->__anonCompField2.min_y >= min_y)
/*@ assert rte_52: mem_access: \valid_read(&rect->__anonCompField2.max_x); */
	if (rect->__anonCompField2.max_x <= max_x)
/*@ assert rte_53: mem_access: \valid_read(&rect->__anonCompField2.max_y); */
	if (rect->__anonCompField2.max_y <= max_y)
/*@ assert rte_54: mem_access: \valid_read(&rect->__anonCompField2.min_x); */
/*@ assert rte_55: mem_access: \valid_read(&rect->__anonCompField2.max_x); */
	if (rect->__anonCompField2.min_x <= rect->__anonCompField2.max_x)
/*@ assert rte_56: mem_access: \valid_read(&rect->__anonCompField2.min_y); */
/*@ assert rte_57: mem_access: \valid_read(&rect->__anonCompField2.max_y); */
	if (rect->__anonCompField2.min_y <= rect->__anonCompField2.max_y)
	tmp = 1;
	else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	else tmp = 0;
	__retres = (_Bool)(tmp != 0);
	return __retres;
}


