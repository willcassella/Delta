// serializer.cpp

#include <string.h>
#include <pugixml.hpp>
#include "../include/Delta/template.h"
#include "../include/Delta/common.h"

void
dt_load_Element(
    pugi::xml_node node,
    dt_template_Element* out_element
);

static bool
dt_load_bool(
    pugi::xml_attribute const attribute,
    char const* const property_name,
    dt_template_Property* const io_property
) {
    if (!strcmp(attribute.name(), property_name))
    {
        *io_property = dt_template_Property_bool(attribute.as_bool());
        return true;
    }

    return false;
}

static bool
dt_load_i32(
    pugi::xml_attribute const attribute,
    char const* const property_name,
    dt_template_Property* const io_property)
{
    if (!strcmp(attribute.name(), property_name))
    {
        *io_property = dt_template_Property_i32(attribute.as_int());
        return true;
    }

    return false;
}

static bool
dt_load_f32(
    pugi::xml_attribute const attribute,
    char const* const property_name,
    dt_template_Property* const io_property)
{
    if (!strcmp(attribute.name(), property_name))
    {
        *io_property = dt_template_Property_f32(attribute.as_float());
        return true;
    }

    return false;
}

static bool
dt_load_color(
    pugi::xml_attribute const attribute,
    char const* const property_name,
    dt_template_Property* const io_property)
{
    if (!strcmp(attribute.name(), property_name))
    {
        if (!strcmp(attribute.value(), "red"))
        {
            *io_property = dt_template_Property_color(DT_COLOR_RED);
        }
        else if (!strcmp(attribute.value(), "green"))
        {
            *io_property = dt_template_Property_color(DT_COLOR_GREEN);
        }
        else if (!strcmp(attribute.value(), "blue"))
        {
            *io_property = dt_template_Property_color(DT_COLOR_BLUE);
        }
        else if (!strcmp(attribute.value(), "white"))
        {
            *io_property = dt_template_Property_color(DT_COLOR_WHITE);
        }
        else if (!strcmp(attribute.value(), "black"))
        {
            *io_property = dt_template_Property_color(DT_COLOR_BLACK);
        }
        else
        {
            *io_property = dt_template_Property_color(attribute.as_uint(DT_COLOR_BLACK));
        }

        return true;
    }

    return false;
}

static bool
dt_load_halign(
    pugi::xml_attribute const attribute,
    dt_template_Property* const io_property,
    char const* const property_name = "h_align"
) {
    if (!strcmp(attribute.name(), property_name))
    {
        if (!strcmp(attribute.value(), "center"))
        {
            *io_property = dt_template_Property_i32(DT_HALIGN_CENTER);
        }
        else if (!strcmp(attribute.value(), "left"))
        {
            *io_property = dt_template_Property_i32(DT_HALIGN_LEFT);
        }
        else if (!strcmp(attribute.value(), "right"))
        {
            *io_property = dt_template_Property_i32(DT_HALIGN_RIGHT);
        }

        return true;
    }

    return false;
}

static bool
dt_load_valign(
    pugi::xml_attribute const attribute,
    dt_template_Property* const io_property,
    char const* const property_name = "v_align"
) {
    if (!strcmp(attribute.name(), property_name))
    {
        if (!strcmp(attribute.value(), "center"))
        {
            *io_property = dt_template_Property_i32(DT_VALIGN_CENTER);
        }
        else if (!strcmp(attribute.value(), "top"))
        {
            *io_property = dt_template_Property_i32(DT_VALIGN_TOP);
        }
        else if (!strcmp(attribute.value(), "bottom"))
        {
            *io_property = dt_template_Property_i32(DT_VALIGN_BOTTOM);
        }

        return true;
    }

    return false;
}

static bool
dt_load_bounds(
    pugi::xml_attribute const attribute,
    dt_template_Property* DT_RESTRICT const min_width,
    dt_template_Property* DT_RESTRICT const max_width,
    dt_template_Property* DT_RESTRICT const min_height,
    dt_template_Property* DT_RESTRICT const max_height
) {
    if (!strcmp(attribute.name(), "min_width"))
    {
        *min_width = dt_template_Property_f32(attribute.as_float());
        return true;
    }
    if (!strcmp(attribute.name(), "max_width"))
    {
        *max_width = dt_template_Property_f32(attribute.as_float());
        return true;
    }
    if (!strcmp(attribute.name(), "width"))
    {
        float const value = attribute.as_float();
        *min_width = dt_template_Property_f32(value);
        *max_width = dt_template_Property_f32(value);
        return true;
    }
    if (!strcmp(attribute.name(), "min_height"))
    {
        *min_height = dt_template_Property_f32(attribute.as_float());
        return true;
    }
    if (!strcmp(attribute.name(), "max_height"))
    {
        *max_height = dt_template_Property_f32(attribute.as_float());
        return true;
    }
    if (!strcmp(attribute.name(), "height"))
    {
        float const value = attribute.as_float();
        *max_height = dt_template_Property_f32(value);
        *max_height = dt_template_Property_f32(value);
        return true;
    }

    return false;
}

static void
dt_load_BlockBorder(
    pugi::xml_node node,
    dt_template_BlockBorder* io_block_border
) {
    for (auto const attribute : node.attributes())
    {
        dt_load_bounds(attribute, &io_block_border->min_width, &io_block_border->max_width, &io_block_border->min_height, &io_block_border->max_height);
        if (dt_load_f32(attribute, "bdr_left", &io_block_border->border_thickness.left)) continue;
        if (dt_load_f32(attribute, "bdr_right", &io_block_border->border_thickness.right)) continue;
        if (dt_load_f32(attribute, "bdr_top", &io_block_border->border_thickness.top)) continue;
        if (dt_load_f32(attribute, "bdr_bottom", &io_block_border->border_thickness.bottom)) continue;
        if (dt_load_color(attribute, "bg_color", &io_block_border->background_color)) continue;
        if (dt_load_color(attribute, "bdr_color", &io_block_border->border_color)) continue;
    }

    if (node.first_child())
    {
        dt_load_Element(node.first_child(), &io_block_border->template_element);
    }
}

static void
dt_load_Box(
    pugi::xml_node const node,
    dt_template_Box* const io_box
) {
    for (auto const attribute : node.attributes())
    {
        if (dt_load_bounds(attribute, &io_box->min_width, &io_box->max_width, &io_box->min_height, &io_box->max_height)) continue;
    }

    // Load children of the node
    dt_template_BoxChild const** child_ptr = &io_box->first_child;
    for (auto const child_node : node.children())
    {
        auto* const child = new dt_template_BoxChild;
        dt_template_BoxChild_init(child, child_ptr);
        child_ptr = &child->next_child;
        dt_load_Element(child_node, &child->template_element);

        for (auto const attribute : child_node.attributes())
        {
            if (dt_load_f32(attribute, "mgn_left", &child->margin.left)) continue;
            if (dt_load_f32(attribute, "mgn_right", &child->margin.right)) continue;
            if (dt_load_f32(attribute, "mgn_top", &child->margin.top)) continue;
            if (dt_load_f32(attribute, "mgn_bottom", &child->margin.bottom)) continue;
            if (dt_load_halign(attribute, &child->horizontal_alignment)) continue;
            if (dt_load_valign(attribute, &child->horizontal_alignment)) continue;
        }
    }
}

static void
dt_load_Stack(
    pugi::xml_node const node,
    dt_template_Stack* const io_stack
) {
    for (auto const attribute : node.attributes())
    {
        if (dt_load_bounds(attribute, &io_stack->min_width, &io_stack->max_width, &io_stack->min_height, &io_stack->max_height)) continue;
        if (dt_load_bool(attribute, "last_child_fill", &io_stack->last_child_fill)) continue;

        if (!strcmp(attribute.name(), "stack_dir"))
        {
            if (!strcmp(attribute.value(), "left_to_right"))
            {
                io_stack->stack_direction = dt_template_Property_i32(DT_STACK_LEFT_TO_RIGHT);
            }
            else if (!strcmp(attribute.value(), "right_to_left"))
            {
                io_stack->stack_direction = dt_template_Property_i32(DT_STACK_RIGHT_TO_LEFT);
            }
            else if (!strcmp(attribute.value(), "top_to_bottom"))
            {
                io_stack->stack_direction = dt_template_Property_i32(DT_STACK_TOP_TO_BOTTOM);
            }
            else if (!strcmp(attribute.value(), "bottom_to_top"))
            {
                io_stack->stack_direction = dt_template_Property_i32(DT_STACK_BOTTOM_TO_TOP);
            }

            continue;
        }
    }

    // Load children of the node
    dt_template_StackChild const** child_ptr = &io_stack->first_child;
    for (auto const child_node : node.children())
    {
        auto* const child = new dt_template_StackChild;
        dt_template_StackChild_init(child, child_ptr);
        child_ptr = &child->next_child;
        dt_load_Element(child_node, &child->template_element);

        for (auto const attribute : child_node.attributes())
        {
            if (dt_load_f32(attribute, "mgn_left", &child->margin.left)) continue;
            if (dt_load_f32(attribute, "mgn_right", &child->margin.right)) continue;
            if (dt_load_f32(attribute, "mgn_top", &child->margin.top)) continue;
            if (dt_load_f32(attribute, "mgn_bottom", &child->margin.bottom)) continue;

            dt_load_valign(attribute, &child->cross_alignment);
            dt_load_halign(attribute, &child->cross_alignment);
        }
    }
}

static void
dt_load_Element(
    pugi::xml_node node,
    dt_template_Element* out_element
) {
    if (!strcmp(node.name(), "block_border"))
    {
        auto* const block_border = new dt_template_BlockBorder;
        dt_template_BlockBorder_init(block_border, out_element);
        dt_load_BlockBorder(node, block_border);
    }
    else if (!strcmp(node.name(), "box"))
    {
        auto* const box = new dt_template_Box;
        dt_template_Box_init(box, out_element);
        dt_load_Box(node, box);
    }
    else if (!strcmp(node.name(), "stack"))
    {
        auto* const stack = new dt_template_Stack;
        dt_template_Stack_init(stack, out_element);
        dt_load_Stack(node, stack);
    }
}

DT_FUNC dt_template_Window const*
dt_load_ui(
    char const* const path
) {
    auto* const window = new dt_template_Window;
    dt_template_Window_init(window);

    pugi::xml_document doc;
    doc.load_file(path);
    auto node = doc.child("test");
    dt_load_Box(doc, &window->content_box);

    return window;
}
