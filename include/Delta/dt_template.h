// dt_template.h
#pragma once

#include "dt_layout.h"

typedef char const* dt_ViewModel_Expr;

/* ViewModel function used to evaluate boolean properties and predicates. */
typedef dt_bool(*dt_ViewModel_Eval_bool_fn)(
	void* user_data,
	dt_ViewModel_Expr expr
);

/* ViewModel function used to evaluate float properties. */
typedef float(*dt_ViewModel_Eval_f32_fn)(
	void* user_data,
	dt_ViewModel_Expr expr
);

/* ViewModel function used to evaluate int32 properties. */
typedef int32_t(*dt_ViewModel_Eval_i32_fn)(
	void* user_data,
	dt_ViewModel_Expr expr
);

typedef dt_Color(*dt_ViewModel_Eval_Color_fn)(
	void* user_data,
	dt_ViewModel_Expr expr
);

struct dt_ViewModel {
	void* user_data;
	dt_ViewModel_Eval_bool_fn eval_bool;
	dt_ViewModel_Eval_f32_fn eval_f32;
	dt_ViewModel_Eval_i32_fn eval_i32;
	dt_ViewModel_Eval_Color_fn eval_color;
};

struct dt_template_Property {
	union {
		dt_bool fixed_bool;
		float fixed_f32;
		int32_t fixed_i32;
		dt_Color fixed_color;
		dt_ViewModel_Expr expr;
	};
	dt_bool is_expr;
};

DT_FUNC struct dt_template_Property
dt_template_Property_expr(
	dt_ViewModel_Expr expr
);

DT_FUNC struct dt_template_Property
dt_template_Property_bool(
	dt_bool value
);

DT_FUNC dt_bool
dt_template_Property_get_bool(
	struct dt_template_Property property,
	struct dt_ViewModel const* view_model
);

DT_FUNC struct dt_template_Property
dt_template_Property_f32(
	float value
);

DT_FUNC float
dt_template_Property_get_f32(
	struct dt_template_Property property,
	struct dt_ViewModel const* view_model
);

DT_FUNC struct dt_template_Property
dt_template_Property_i32(
	int32_t value
);

DT_FUNC int32_t
dt_template_Property_get_i32(
	struct dt_template_Property property,
	struct dt_ViewModel const* view_model
);

DT_FUNC struct dt_template_Property
dt_template_Property_color(
	dt_Color value
);

DT_FUNC dt_Color
dt_template_Property_get_color(
	struct dt_template_Property property,
	struct dt_ViewModel const* view_model
);

struct dt_template_EdgeBox {
	struct dt_template_Property top;
	struct dt_template_Property right;
	struct dt_template_Property bottom;
	struct dt_template_Property left;
};

DT_FUNC void
dt_template_EdgeBox_init_default(
	struct dt_template_EdgeBox* edge_box
);

DT_FUNC struct dt_layout_EdgeBox
dt_template_EdgeBox_get_layout(
	struct dt_template_EdgeBox const* edge_box,
	struct dt_ViewModel const* view_model
);

struct dt_template_CornerBox {
	struct dt_template_Property top_left;
	struct dt_template_Property top_right;
	struct dt_template_Property bottom_left;
	struct dt_template_Property bottom_right;
};

DT_FUNC void
dt_template_CornerBox_init(
	struct dt_template_CornerBox* corner_box
);

DT_FUNC struct dt_layout_CornerBox
dt_template_CornerBox_get_layout(
	struct dt_template_CornerBox const* corner_box,
	struct dt_ViewModel const* view_model
);

typedef enum {
	DT_TEMPLATE_EMPTY,
	DT_TEMPLATE_BLOCK_BORDER,
	DT_TEMPLATE_BOX,
	DT_TEMPLATE_STACK,
} dt_template_ElementType;

struct dt_template_Element {
	void const* data;
	dt_template_ElementType type;
};

DT_FUNC struct dt_layout_Element
dt_template_Element_generate_layout(
	struct dt_template_Element template_element,
	struct dt_ViewModel const* view_model,
	struct dt_Allocator* layout_allocator,
	struct dt_layout_Bounds* out_bounds
);

/*
 *
 * BLOCK BORDER
 *
 */

struct dt_template_BlockBorder {
	dt_ViewModel_Expr element_predicate_expr;
	struct dt_template_Element template_element;
	struct dt_template_Property min_width;
	struct dt_template_Property max_width;
	struct dt_template_Property min_height;
	struct dt_template_Property max_height;
	struct dt_template_EdgeBox padding;
	struct dt_template_EdgeBox border_thickness;
	struct dt_template_CornerBox corner_radii;
	struct dt_template_Property background_color;
	struct dt_template_Property border_color;
};

DT_FUNC void
dt_template_BlockBorder_init(
	struct dt_template_BlockBorder* out_block_border,
	struct dt_template_Element* out_element
);

DT_FUNC struct dt_layout_BlockBorder*
dt_template_BlockBorder_generate_layout(
	struct dt_template_BlockBorder const* block_border,
	struct dt_ViewModel const* view_model,
	struct dt_Allocator* layout_allocator,
	struct dt_layout_Bounds* out_bounds
);

/*
 *
 * BOX
 *
 */

struct dt_template_Box {
	struct dt_template_BoxChild const* first_child;
	struct dt_template_Property min_width;
	struct dt_template_Property max_width;
	struct dt_template_Property min_height;
	struct dt_template_Property max_height;
};

struct dt_template_BoxChild {
	struct dt_template_BoxChild const* next_child;
	dt_ViewModel_Expr predicate_expr;
	struct dt_template_Element template_element;
	struct dt_template_EdgeBox margin;
	struct dt_template_Property horizontal_alignment;
	struct dt_template_Property vertical_alignment;
};

DT_FUNC void
dt_template_Box_init(
	struct dt_template_Box* out_box,
	struct dt_template_Element* out_element
);

DT_FUNC void
dt_template_BoxChild_init(
	struct dt_template_BoxChild* out_box_child,
	struct dt_template_BoxChild const** out_child_ptr
);

DT_FUNC void
dt_template_Box_set_width(
	struct dt_template_Box* box,
	float width
);

DT_FUNC void
dt_template_Box_set_height(
	struct dt_template_Box* box,
	float height
);

DT_FUNC struct dt_layout_Box*
dt_template_Box_generate_layout(
	struct dt_template_Box const* box,
	struct dt_ViewModel const* view_model,
	struct dt_Allocator* layout_allocator,
	struct dt_layout_Bounds* out_bounds
);

/*
 *
 * STACK
 *
 */

struct dt_template_Stack {
	struct dt_template_StackChild const* first_child;
	struct dt_template_Property min_width;
	struct dt_template_Property max_width;
	struct dt_template_Property min_height;
	struct dt_template_Property max_height;
	struct dt_template_Property stack_direction;
	struct dt_template_Property last_child_fill;
};

struct dt_template_StackChild {
	struct dt_template_StackChild const* next_child;
	dt_ViewModel_Expr predicate_expr;
	struct dt_template_Element template_element;
	struct dt_template_EdgeBox margin;
	struct dt_template_Property cross_alignment;
};

DT_FUNC void
dt_template_Stack_init(
	struct dt_template_Stack* out_stack,
	struct dt_template_Element* out_element
);

DT_FUNC void
dt_template_StackChild_init(
	struct dt_template_StackChild* out_stack_child,
	struct dt_template_StackChild const** out_child_ptr
);

DT_FUNC struct dt_layout_Stack*
dt_template_Stack_generate_layout(
	struct dt_template_Stack const* stack,
	struct dt_ViewModel const* view_model,
	struct dt_Allocator* layout_allocator,
	struct dt_layout_Bounds* out_bounds
);

/*
*
* WINDOW
*
*/

struct dt_template_Window {
	struct dt_template_Box content_box;
	struct dt_template_Property background_color;
};

DT_FUNC void dt_template_Window_init(
	struct dt_template_Window* out_window
);

DT_FUNC void dt_template_WindowChild_init(
	struct dt_template_BoxChild* out_window_child,
	struct dt_template_BoxChild const** out_child_ptr
);

DT_FUNC void dt_template_Window_generate_layout(
	struct dt_template_Window const* window,
	struct dt_ViewModel const* view_model,
	struct dt_Allocator* layout_allocator,
	struct dt_layout_Window* out_layout_window
);
