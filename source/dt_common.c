// dt_common.c

#include <assert.h>
#include "../include/Delta/dt_common.h"
#include "../private/dt_build_private.h"

uint8_t
dt_Color_get_red(
	dt_Color const color
) {
	return (uint8_t)(color >> 24);
}

void
dt_Color_set_red(
	dt_Color* const color,
	uint8_t const red
) {
	*color = (*color & ~DT_COLOR_RED_MASK) | (uint32_t)red << 24;
}

uint8_t
dt_Color_get_green(
	dt_Color const color
) {
	return (uint8_t)(color & DT_COLOR_GREEN_MASK >> 16);
}

void
dt_Color_set_green(
	dt_Color* const color,
	uint8_t const green
) {
	*color = (*color & ~DT_COLOR_GREEN_MASK) | (uint32_t)green << 16;
}

uint8_t
dt_Color_get_blue(
	dt_Color const color
) {
	return (uint8_t)(color & DT_COLOR_BLUE_MASK >> 8);
}

void
dt_Color_set_blue(
	dt_Color* const color,
	uint8_t const blue
) {
	*color = (*color & ~DT_COLOR_BLUE_MASK) | (uint32_t)blue << 8;
}

uint8_t
dt_Color_get_alpha(
	dt_Color const color
) {
	return (uint8_t)(color & DT_COLOR_ALPHA_MASK);
}

void
dt_Color_set_alpha(
	dt_Color* const color,
	uint8_t const alpha
) {
	*color = (*color & ~DT_COLOR_ALPHA_MASK) | (uint32_t)alpha;
}

void*
dt_Allocator_allocate(
	struct dt_Allocator* const allocator,
	size_t const size
) {
    size_t const req_offset = allocator->offset + size;
    assert(allocator->capacity >= req_offset /*Out of space*/);

	void* const buff = (char*)allocator->buffer + allocator->offset;
	allocator->offset = req_offset;
	return buff;
}
