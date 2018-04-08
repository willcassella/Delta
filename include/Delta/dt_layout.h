// dt_layout.h
#pragma once

#include "dt_common.h"

struct dt_layout_Bounds {
	float min_width;
	float max_width;
	float min_height;
	float max_height;
	float computed_width;
	float computed_height;
};

DT_FUNC void
dt_layout_Bounds_validate(
	struct dt_layout_Bounds* io_bounds
);

struct dt_layout_EdgeBox {
	float left;
	float right;
	float top;
	float bottom;
};

struct dt_layout_CornerBox {
	float top_left;
	float top_right;
	float bottom_left;
	float bottom_right;
};

struct dt_layout_ContentBox {
	float x;
	float width;
	float y;
	float height;
};

typedef enum {
	DT_LAYOUT_EMPTY,
	DT_LAYOUT_TEXT,
	DT_LAYOUT_BLOCK_BORDER,
	DT_LAYOUT_BOX,
	DT_LAYOUT_STACK,
} dt_layout_ElementType;

struct dt_layout_Element {
	void* data;
	dt_layout_ElementType type;
};

DT_FUNC void
dt_layout_Element_arrange_content(
	struct dt_layout_Element layout_element,
	struct dt_layout_Bounds* io_bounds
);

DT_FUNC void
dt_layout_Element_generate_commands(
	struct dt_layout_Element layout_element,
	struct dt_Allocator* command_allocator,
	struct dt_layout_ContentBox content_box
);

/*
 *
 * TEXT
 *
 */

struct dt_layout_Text {
	char const* text;
	size_t text_len;
};

/*
 *
 * BLOCK BORDER
 *
 */

struct dt_layout_BlockBorder {
	struct dt_layout_Element layout_element;
	struct dt_layout_Bounds element_bounds;
	struct dt_layout_EdgeBox padding;
	struct dt_layout_EdgeBox border_thickness;
	struct dt_layout_CornerBox corner_radii;
	dt_Color background_color;
	dt_Color border_color;
};

DT_FUNC void
dt_layout_BlockBorder_arrange_content(
	struct dt_layout_BlockBorder const* block_border,
	struct dt_layout_Bounds* io_bounds
);

DT_FUNC void
dt_layout_BlockBorder_generate_commands(
	struct dt_layout_BlockBorder const* block_border,
	struct dt_Allocator* command_allocator,
	struct dt_layout_ContentBox content_box
);

/*
 *
 * BOX
 *
 */

struct dt_layout_Box {
	struct dt_layout_BoxChild* first_child;
};

struct dt_layout_BoxChild {
	struct dt_layout_BoxChild* next_child;
	struct dt_layout_Element element;
	struct dt_layout_Bounds bounds;
	struct dt_layout_EdgeBox margin;
	dt_HorizontalAlignment horizontal_alignment;
	dt_VerticalAlignment vertical_alignment;
};

DT_FUNC void
dt_layout_Box_arrange_content(
	struct dt_layout_Box const* layout_box,
	struct dt_layout_Bounds* io_bounds
);

DT_FUNC void
dt_layout_Box_generate_commands(
	struct dt_layout_Box const* layout_box,
	struct dt_Allocator* command_allocator,
	struct dt_layout_ContentBox content_box
);

/*
 *
 * STACK
 *
 */

typedef enum {
	DT_STACK_LEFT_TO_RIGHT = 0,
	DT_STACK_RIGHT_TO_LEFT = 1,
	DT_STACK_TOP_TO_BOTTOM = 2,
	DT_STACK_BOTTOM_TO_TOP = 3,
} dt_layout_Stack_Direction;

struct dt_layout_Stack {
	struct dt_layout_StackChild* first_child;
	dt_layout_Stack_Direction stack_direction;
	dt_bool last_child_fill;
};

struct dt_layout_StackChild {
	struct dt_layout_StackChild* next_child;
	struct dt_layout_Element element;
	struct dt_layout_Bounds bounds;
	struct dt_layout_EdgeBox margin;
	int32_t cross_alignment;
};

DT_FUNC void
dt_layout_Stack_arrange_content(
	struct dt_layout_Stack const* layout_stack,
	struct dt_layout_Bounds* io_bounds
);

DT_FUNC void
dt_layout_Stack_generate_commands(
	struct dt_layout_Stack const* layout_stack,
	struct dt_Allocator* command_allocator,
	struct dt_layout_ContentBox content_box
);

/*
 *
 * WINDOW
 *
 */

struct dt_layout_Window {
	struct dt_layout_Box* content;
	struct dt_layout_Bounds content_bounds;
	dt_Color background_color;
};

DT_FUNC void
dt_layout_Window_arrange_content(
	struct dt_layout_Window* window
);

DT_FUNC void
dt_layout_Window_generate_commands(
	struct dt_layout_Window const* window,
	struct dt_Allocator* command_allocator,
	float width,
	float height
);

