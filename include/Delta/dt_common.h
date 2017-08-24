// dt_common.h
#pragma once

#include <stdint.h>

#ifdef __cplusplus
#	define DT_FUNC extern "C"
#else
#	define DT_FUNC
#endif

#define DT_RESTRICT __restrict

#define DT_FORCEINLINE inline __forceinline

#define DT_UNUSED(x) (void)(x)

#define DT_LIKELY(x) (x)
#define DT_UNLIKELY(x) (x)

typedef uint8_t dt_bool_t;
typedef uint32_t dt_Color_t;
typedef struct dt_Allocator dt_Allocator;
typedef enum dt_HorizontalAlignment dt_HorizontalAlignment;
typedef enum dt_VerticalAlignment dt_VerticalAlignment;

#define DT_TRUE 1
#define DT_FALSE 0

#define DT_COLOR_BLACK 0x000000FFu
#define DT_COLOR_WHITE 0xFFFFFFFFu
#define DT_COLOR_RED 0xFF0000FFu
#define DT_COLOR_GREEN 0x00FF00FFu
#define DT_COLOR_BLUE 0x0000FFFFu
#define DT_COLOR_RED_MASK 0xFF000000u
#define DT_COLOR_GREEN_MASK 0x00FF0000u
#define DT_COLOR_BLUE_MASK 0x0000FF00u
#define DT_COLOR_ALPHA_MASK 0x000000FFu

DT_FUNC uint8_t dt_Color_get_red(
	dt_Color_t color
);

DT_FUNC void dt_Color_set_red(
	dt_Color_t* color,
	uint8_t red
);

DT_FUNC uint8_t dt_Color_get_green(
	dt_Color_t color
);

DT_FUNC void dt_Color_set_green(
	dt_Color_t* color,
	uint8_t green
);

DT_FUNC uint8_t dt_Color_get_blue(
	dt_Color_t color
);

DT_FUNC void dt_Color_set_blue(
	dt_Color_t* color,
	uint8_t blue
);

DT_FUNC uint8_t dt_Color_get_alpha(
	dt_Color_t color
);

DT_FUNC void dt_Color_set_alpha(
	dt_Color_t* color,
	uint8_t alpha
);

enum dt_HorizontalAlignment {
	DT_HALIGN_CENTER = 0,
	DT_HALIGN_LEFT = 1,
	DT_HALIGN_RIGHT = 2,
};

enum dt_VerticalAlignment {
	DT_VALIGN_CENTER = 0,
	DT_VALIGN_TOP = 1,
	DT_VALIGN_BOTTOM = 2,
};

struct dt_Allocator {
	void* buffer;
	size_t capacity;
	size_t offset;
};

DT_FUNC void* dt_Allocator_allocate(
	dt_Allocator* allocator,
	size_t size
);
