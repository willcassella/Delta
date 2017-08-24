// dt_template.c

#include <math.h>
#include <assert.h>
#include <string.h>
#include "../include/Delta/dt_template.h"
#include "../private/dt_build_private.h"

dt_template_Property dt_template_Property_expr(
	dt_ViewModel_Expr_t const expr
) {
	dt_template_Property out;
	out.expr = expr;
	out.is_expr = DT_TRUE;
	return out;
}

dt_template_Property dt_template_Property_bool(
	dt_bool_t const value
) {
	dt_template_Property out;
	out.fixed_bool = value;
	out.is_expr = DT_FALSE;
	return out;
}

dt_bool_t dt_template_Property_get_bool(
	dt_template_Property const property,
	dt_ViewModel const* const view_model
) {
	return property.is_expr ? view_model->eval_bool(view_model->user_data, property.expr) : property.fixed_bool;
}

dt_template_Property dt_template_Property_f32(
	float const value
) {
	dt_template_Property out;
	out.fixed_f32 = value;
	out.is_expr = DT_FALSE;
	return out;
}

float dt_template_Property_get_f32(
	dt_template_Property const property,
	dt_ViewModel const* const view_model
) {
	return property.is_expr ? view_model->eval_f32(view_model->user_data, property.expr) : property.fixed_f32;
}

dt_template_Property dt_template_Property_i32(
	int32_t const value
) {
	dt_template_Property out;
	out.fixed_i32 = value;
	out.is_expr = DT_FALSE;
	return out;
}

int32_t dt_template_Property_get_i32(
	dt_template_Property const property,
	dt_ViewModel const* const view_model
) {
	return property.is_expr ? view_model->eval_i32(view_model->user_data, property.expr) : property.fixed_i32;
}

dt_template_Property dt_template_Property_color(
	dt_Color_t const value
) {
	dt_template_Property out;
	out.fixed_color = value;
	out.is_expr = DT_FALSE;
	return out;
}

dt_Color_t dt_template_Property_get_color(
	dt_template_Property const property,
	dt_ViewModel const* const view_model
) {
	return property.is_expr ? view_model->eval_color(view_model->user_data, property.expr) : property.fixed_color;
}

void dt_template_EdgeBox_init_default(
	dt_template_EdgeBox* const edge_box
) {
	memset(edge_box, 0, sizeof(dt_template_EdgeBox));
}

dt_layout_EdgeBox dt_template_EdgeBox_get_layout(
	dt_template_EdgeBox const* const edge_box,
	dt_ViewModel const* const view_model
) {
	dt_layout_EdgeBox out;
	out.top = dt_template_Property_get_f32(edge_box->top, view_model);
	out.right = dt_template_Property_get_f32(edge_box->right, view_model);
	out.bottom = dt_template_Property_get_f32(edge_box->bottom, view_model);
	out.left = dt_template_Property_get_f32(edge_box->left, view_model);
	return out;
}

void dt_template_CornerBox_init(
	dt_template_CornerBox* const corner_box
) {
	memset(corner_box, 0, sizeof(dt_template_CornerBox));
}

dt_layout_CornerBox dt_template_CornerBox_get_layout(
	dt_template_CornerBox const* const corner_box,
	dt_ViewModel const* const view_model
) {
	dt_layout_CornerBox out;
	out.top_left = dt_template_Property_get_f32(corner_box->top_left, view_model);
	out.top_right = dt_template_Property_get_f32(corner_box->top_right, view_model);
	out.bottom_left = dt_template_Property_get_f32(corner_box->bottom_left, view_model);
	out.bottom_right = dt_template_Property_get_f32(corner_box->bottom_right, view_model);
	return out;
}

dt_layout_Element dt_template_Element_generate_layout(
	dt_template_Element const template_element,
	dt_ViewModel const* view_model,
	dt_Allocator* const layout_allocator,
	dt_layout_Bounds* const out_bounds
) {
	dt_layout_Element element;
	element.data = NULL;
	element.type = DT_LAYOUT_EMPTY;

	switch (template_element.type)
	{
	case DT_TEMPLATE_EMPTY:
		break;

	case DT_TEMPLATE_BLOCK_BORDER:
		element.data = dt_template_BlockBorder_generate_layout(template_element.data, view_model, layout_allocator, out_bounds);
		element.type = DT_LAYOUT_BLOCK_BORDER;
		break;

	case DT_TEMPLATE_BOX:
		element.data = dt_template_Box_generate_layout(template_element.data, view_model, layout_allocator, out_bounds);
		element.type = DT_LAYOUT_BOX;
		break;

	case DT_TEMPLATE_STACK:
		element.data = dt_template_Stack_generate_layout(template_element.data, view_model, layout_allocator, out_bounds);
		element.type = DT_LAYOUT_STACK;
		break;

	default:
		assert(DT_FALSE);
		break;
	}

	return element;
}

/*
 *
 * BLOCK BORDER
 *
 */

void dt_template_BlockBorder_init(
	dt_template_BlockBorder* const out_block_border,
	dt_template_Element* const out_template_element
) {
	memset(out_block_border, 0, sizeof(dt_template_BlockBorder));
	out_block_border->max_width = dt_template_Property_f32(INFINITY);
	out_block_border->max_height = dt_template_Property_f32(INFINITY);

	out_template_element->data = out_block_border;
	out_template_element->type = DT_TEMPLATE_BLOCK_BORDER;
}

dt_layout_BlockBorder* dt_template_BlockBorder_generate_layout(
	dt_template_BlockBorder const* const block_border,
	dt_ViewModel const* const view_model,
	dt_Allocator* const layout_allocator,
	dt_layout_Bounds* const out_bounds
) {
	dt_layout_BlockBorder* const out = dt_Allocator_allocate(layout_allocator, sizeof(dt_layout_BlockBorder));

	// Get actual values for template properties
	out->padding = dt_template_EdgeBox_get_layout(&block_border->padding, view_model);
	out->border_thickness = dt_template_EdgeBox_get_layout(&block_border->border_thickness, view_model);
	out->corner_radii = dt_template_CornerBox_get_layout(&block_border->corner_radii, view_model);
	out->background_color = dt_template_Property_get_color(block_border->background_color, view_model);
	out->border_color = dt_template_Property_get_color(block_border->border_color, view_model);

	// Generate child
	if (!block_border->element_predicate_expr || view_model->eval_bool(view_model->user_data, block_border->element_predicate_expr))
	{
		out->layout_element = dt_template_Element_generate_layout(
			block_border->template_element,
			view_model,
			layout_allocator,
			&out->element_bounds);
	}

	// Arrange content
	out_bounds->min_width = dt_template_Property_get_f32(block_border->min_width, view_model);
	out_bounds->max_width = dt_template_Property_get_f32(block_border->max_width, view_model);
	out_bounds->min_height = dt_template_Property_get_f32(block_border->min_height, view_model);
	out_bounds->max_height = dt_template_Property_get_f32(block_border->max_height, view_model);
	dt_layout_BlockBorder_arrange_content(out, out_bounds);
	dt_layout_Bounds_validate(out_bounds);

	return out;
}

/*
 *
 * BOX
 *
 */

void dt_template_Box_init(
	dt_template_Box* const out_box,
	dt_template_Element* const out_template_element
) {
	memset(out_box, 0, sizeof(dt_template_Box));
	out_box->max_width = dt_template_Property_f32(INFINITY);
	out_box->max_height = dt_template_Property_f32(INFINITY);

	out_template_element->data = out_box;
	out_template_element->type = DT_TEMPLATE_BOX;
}

void dt_template_BoxChild_init(
	dt_template_BoxChild* const out_box_child,
	dt_template_BoxChild const** const child_ptr
) {
	memset(out_box_child, 0, sizeof(dt_template_BoxChild));
	*child_ptr = out_box_child;
}

void dt_template_Box_set_width(
	dt_template_Box* const box,
	float const width
) {
	box->min_width = dt_template_Property_f32(width);
	box->max_width = dt_template_Property_f32(width);
}

void dt_template_Box_set_height(
	dt_template_Box* const box,
	float const height
) {
	box->min_height = dt_template_Property_f32(height);
	box->max_height = dt_template_Property_f32(height);
}

dt_layout_Box* dt_template_Box_generate_layout(
	dt_template_Box const* const box,
	dt_ViewModel const* const view_model,
	dt_Allocator* const layout_allocator,
	dt_layout_Bounds* const out_bounds
) {
	dt_layout_Box* const out = dt_Allocator_allocate(layout_allocator, sizeof(dt_layout_Box));

	// Generate children
	dt_layout_BoxChild** next_child_ptr = &out->first_child;
	for (dt_template_BoxChild const* child = box->first_child; child != NULL; child = child->next_child)
	{
		// If the child has a predicate and it fails, skip the child
		if (child->predicate_expr && !view_model->eval_bool(view_model->user_data, child->predicate_expr))
		{
			continue;
		}

		// Generate layout child
		dt_layout_BoxChild* const layout_child = dt_Allocator_allocate(layout_allocator, sizeof(dt_layout_BoxChild));
		layout_child->margin = dt_template_EdgeBox_get_layout(&child->margin, view_model);
		layout_child->horizontal_alignment = dt_template_Property_get_i32(child->horizontal_alignment, view_model);
		layout_child->vertical_alignment = dt_template_Property_get_i32(child->vertical_alignment, view_model);
		layout_child->element = dt_template_Element_generate_layout(child->template_element, view_model, layout_allocator, &layout_child->bounds);

		*next_child_ptr = layout_child;
		next_child_ptr = &layout_child->next_child;
	}
	*next_child_ptr = NULL;

	// Arrange content
	out_bounds->min_width = dt_template_Property_get_f32(box->min_width, view_model);
	out_bounds->max_width = dt_template_Property_get_f32(box->max_width, view_model);
	out_bounds->min_height = dt_template_Property_get_f32(box->min_height, view_model);
	out_bounds->max_height = dt_template_Property_get_f32(box->max_height, view_model);
	dt_layout_Box_arrange_content(out, out_bounds);
	dt_layout_Bounds_validate(out_bounds);

	return out;
}

/*
 *
 * STACK
 *
 */

void dt_template_Stack_init(
	dt_template_Stack* const out_stack,
	dt_template_Element* const out_element
) {
	memset(out_stack, 0, sizeof(dt_template_Stack));
	out_stack->max_width = dt_template_Property_f32(INFINITY);
	out_stack->max_height = dt_template_Property_f32(INFINITY);

	out_element->data = out_stack;
	out_element->type = DT_TEMPLATE_STACK;
}

void dt_template_StackChild_init(
	dt_template_StackChild* const out_stack_child,
	dt_template_StackChild const** const out_child_ptr
) {
	memset(out_stack_child, 0, sizeof(dt_template_StackChild));
	*out_child_ptr = out_stack_child;
}

dt_layout_Stack* dt_template_Stack_generate_layout(
	dt_template_Stack const* stack,
	dt_ViewModel const* view_model,
	dt_Allocator* layout_allocator,
	dt_layout_Bounds* out_bounds
) {
	dt_layout_Stack* const out = dt_Allocator_allocate(layout_allocator, sizeof(dt_template_Stack));
	out->stack_direction = dt_template_Property_get_i32(stack->stack_direction, view_model);
	out->last_child_fill = dt_template_Property_get_bool(stack->last_child_fill, view_model);

	dt_layout_StackChild** next_child_ptr = &out->first_child;
	for (dt_template_StackChild const* child = stack->first_child; child != NULL; child = child->next_child)
	{
		// Evaluate child's predicate expression if it exists
		if (child->predicate_expr && !view_model->eval_bool(view_model->user_data, child->predicate_expr))
		{
			continue;
		}

		// Generate layout child
		dt_layout_StackChild* const layout_child = dt_Allocator_allocate(layout_allocator, sizeof(dt_layout_StackChild));
		layout_child->margin = dt_template_EdgeBox_get_layout(&child->margin, view_model);
		layout_child->cross_alignment = dt_template_Property_get_i32(child->cross_alignment, view_model);
		layout_child->element = dt_template_Element_generate_layout(child->template_element, view_model, layout_allocator, &layout_child->bounds);

		*next_child_ptr = layout_child;
		next_child_ptr = &layout_child->next_child;
	}
	*next_child_ptr = NULL;

	// Arrange content
	out_bounds->min_width = dt_template_Property_get_f32(stack->min_width, view_model);
	out_bounds->max_width = dt_template_Property_get_f32(stack->max_width, view_model);
	out_bounds->min_height = dt_template_Property_get_f32(stack->min_height, view_model);
	out_bounds->max_height = dt_template_Property_get_f32(stack->max_height, view_model);
	dt_layout_Stack_arrange_content(out, out_bounds);
	dt_layout_Bounds_validate(out_bounds);

	return out;
}

/*
 *
 * WINDOW
 *
 */

void dt_template_Window_init(
	dt_template_Window* template_window
) {
	dt_template_Element content_element;
	dt_template_Box_init(&template_window->content_box, &content_element);
	template_window->background_color = dt_template_Property_color(DT_COLOR_WHITE);
}

void dt_template_WindowChild_init(
	dt_template_WindowChild* const out_window_child,
	dt_template_WindowChild const** const out_child_ptr
) {
	dt_template_BoxChild_init(out_window_child, out_child_ptr);
}

void dt_template_Window_generate_layout(
	dt_template_Window const* const window,
	dt_ViewModel const* const view_model,
	dt_Allocator* const layout_allocator,
	dt_layout_Window* const out_layout_window
) {
	out_layout_window->content = dt_template_Box_generate_layout(&window->content_box, view_model, layout_allocator, &out_layout_window->content_bounds);
	out_layout_window->background_color = dt_template_Property_get_color(window->background_color, view_model);
}

