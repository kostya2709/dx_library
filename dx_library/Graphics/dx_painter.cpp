#include "dx_painter.hpp"
#include "dx_canvas.hpp"

dx_painter::dx_painter (dx_point point1, dx_point point2) : dx_object_container (point1, point2, colour::GREY_BACKGROUND, 1)
{
    dx_point frame (0, (point2 - point1).y* 0.2);
    canvas = new dx_canvas (point1 + frame, point2 - dx_abstract_scroll_bar::STANDARD_SLIDER_SIZE, &tool_manager);
    dx_scrollable_object* area = new dx_scrollable_object (point1 + frame, point2, canvas);
    tool_manager.painter = this;

    int tool_box_size = 50; 
    int arrow_size = 25;
    int thicker = 50;
    dx_point tool_start (point1.x + (point2 - point1).x / 2, point1.y + (point2 - point1 - tool_box_size) / 2);   
    
    dx_tool_box* temp_box = new dx_tool_box (tool_start, tool_start + tool_box_size, dx_tool_manager::ERASER, &tool_manager);
    temp_box->set_image ("../images/system/eraser.png");
    push_back (*temp_box);

    temp_box = new dx_tool_box (tool_start + dx_point (0, tool_box_size + 5), tool_start + dx_point (tool_box_size, 2 * tool_box_size + 5), dx_tool_manager::PENCIL, &tool_manager);
    temp_box->set_image ("../images/system/pencil.png");
    push_back (*temp_box);

    temp_box = new dx_tool_box (tool_start + dx_point (tool_box_size + 5, tool_box_size + 5), tool_start + dx_point (2 * tool_box_size, 2 * tool_box_size + 5), 2, &tool_manager);
    temp_box->set_image ("../plugin_api/plugins/BoxBlur/icon.png");
    push_back (*temp_box);

    temp_box = new dx_tool_box (tool_start + dx_point (tool_box_size, 5), tool_start + dx_point (2 * tool_box_size, tool_box_size + 5), 3, &tool_manager);
    temp_box->set_image ("../plugin_api/plugins/ColorFilter/icon.png");
    push_back (*temp_box);

    temp_box = new dx_tool_box (tool_start + dx_point (2 * tool_box_size, 5), tool_start + dx_point (3 * tool_box_size, tool_box_size + 5), 4, &tool_manager);
    temp_box->set_image ("../plugin_api/plugins/MakeTransparent/icon.png");
    push_back (*temp_box);

    dx_button <dx_thickness_arrow>* button_up = new dx_button <dx_thickness_arrow> (point1, point1 + arrow_size, colour::MAGENTA, 1);
    button_up->function.direction = dx_thickness_arrow::UP;
    button_up->function.painter = this;
    button_up->handle_pressed = 1;
    button_up->set_image ("../images/scrollbar/arrow_up.jpg");
    button_up->image->set_colour_mask (colour::SKY_BLUE);
    push_back (*button_up);

    dx_button <dx_thickness_arrow>* button_down = new dx_button <dx_thickness_arrow> (point1 + dx_point (0, arrow_size), point1 + dx_point (0, arrow_size) + arrow_size, colour::MAGENTA, 1);
    button_down->function.direction = dx_thickness_arrow::DOWN;
    button_down->function.painter = this;
    button_down->handle_pressed = 1;
    button_down->set_image ("../images/scrollbar/arrow_down.jpg");
    button_down->image->set_colour_mask (colour::SKY_BLUE);
    push_back (*button_down);

    thickness_demonstrator = new dx_canvas (point1 + dx_point (2 * arrow_size, 0), point1 + dx_point (2 * arrow_size, 0) + thicker, &tool_manager);
    push_back (*thickness_demonstrator);

    dx_point centre = thickness_demonstrator->get_centre() - thickness_demonstrator->get_left_up();
    dx_point size = thickness_demonstrator->get_size();
    thickness_demonstrator->image->draw_circle (centre, width, colour::BLACK);

    dx_point palette_offset ((point2 - point1).x, 0);
    push_back (*(new dx_palette (point1 + dx_point((point2 - point1).x * 0.7, 0), point1 + frame + palette_offset, this)));
    push_back (*area);
}

dx_palette::dx_palette (dx_point point1, dx_point point2, dx_painter* painter_)  : 
    dx_object_container (point1, point2, colour::GREY_BACKGROUND, 1), painter (painter_)
{
    int colour_num = 30;
    int delta = 3;
    int delta_cur = 10;
    int occupied = (point2 - point1).x / 6;
    int size = ((point2 - point1).x - occupied) / (colour_num / 3) - delta;

    cur_colour = new dx_abstract_field (point1 + dx_point (0, (point2.y - point1.y - occupied - delta_cur) * 0.5), point1 + dx_point (occupied - delta_cur, (point2.y - point1.y + occupied + delta_cur) * 0.5), colour::BLACK, 1);
    push_back (*cur_colour);

    dx_colour colour_array [colour_num] = {colour::WHITE};
    colour_array [0] = colour::BLACK;
    colour_array [1] = colour::GRAY;
    colour_array [2] = colour::BURGUNDY;
    colour_array [3] = colour::RED;
    colour_array [4] = colour::CARROT;
    colour_array [5] = colour::YELLOW;
    colour_array [6] = colour::DARK_GREEN;
    colour_array [7] = colour::LIGHT_BLUE;
    colour_array [8] = colour::BLUE;
    colour_array [9] = colour::MAGENTA;

    for (int i = 10; i < colour_num; ++i)
        colour_array [i] = colour::WHITE;
    
    for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 10; ++i)
            push_back (*((dx_abstract_object*)(new dx_colour_box (this, point1 + dx_point (i * (size + delta) + occupied, j * (size + delta)), size, colour_array [i + j * colour_num / 3]))));

}

void dx_palette::set_colour (dx_colour colour) noexcept
{
    if (painter)
    {
        painter->colour = colour;
        cur_colour->set_colour (colour);
    }
}

dx_palette::~dx_palette()
{
    for (auto& i : container)
        delete i;
}

dx_colour_box::dx_colour_box (dx_palette* palette_, dx_point pos, int size, dx_colour colour_) : 
    dx_hoverable_object (pos, pos + size, colour_, 1), colour (colour_), palette (palette_)
{
}

void dx_colour_box::operate (dx_point mouse) noexcept
{
    palette->set_colour (this->colour);
}
