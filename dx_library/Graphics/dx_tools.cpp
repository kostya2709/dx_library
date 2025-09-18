
#include "dx_painter.hpp"
#include "dx_plugins.hpp"

void dx_pencil::perform (dx_point position) noexcept
{
    dx_colour colour = this->tool_manager->painter->colour; 
    dx_point picture_pos = this->tool_manager->painter->canvas->get_left_up();
    picture_pos = picture_pos - this->tool_manager->painter->canvas->image->get_source_position();

    dx_point* last = &this->tool_manager->painter->canvas->last_point;
    uint8_t width = this->tool_manager->painter->width;

    if (!this->tool_manager->painter->canvas->last_point)
        this->tool_manager->painter->canvas->image->draw_circle (position - picture_pos, width, colour);
    else
        this->tool_manager->painter->canvas->image->draw_line (position - picture_pos, *last - picture_pos, width, colour);
    *last = position;
}

void dx_eraser::perform (dx_point position) noexcept
{
    dx_colour colour = this->tool_manager->painter->colour; 
    dx_point picture_pos = this->tool_manager->painter->canvas->get_left_up();
    picture_pos = picture_pos - this->tool_manager->painter->canvas->image->get_source_position();

    dx_point* last = &this->tool_manager->painter->canvas->last_point;
    uint8_t width = this->tool_manager->painter->width;

    if (!this->tool_manager->painter->canvas->last_point)
        this->tool_manager->painter->canvas->image->draw_circle (position - picture_pos, width, colour::WHITE);
    else
        this->tool_manager->painter->canvas->image->draw_line (position - picture_pos, *last - picture_pos, width, colour::WHITE);
    *last = position;
}

dx_tool_manager::dx_tool_manager (void)
{
    tools.push_back (new dx_pencil (this));
    tools.push_back (new dx_eraser (this));
    tools.push_back (new dx_plugin (this, "../plugin_api/plugins/BoxBlur/BoxBlur.so"));
    tools.push_back (new dx_plugin (this, "../plugin_api/plugins/ColorFilter/ColorFilter.so"));
    tools.push_back (new dx_plugin (this, "../plugin_api/plugins/MakeTransparent/MakeTransparent.so"));
}

void dx_tool_manager::perform (dx_point position) noexcept
{
    tools [active_tool]->perform (position);
}

dx_tool_box::dx_tool_box (dx_point point1, dx_point point2, int tool_num_, dx_tool_manager* tool_manager_)
 : dx_hoverable_object (point1, point2, colour::LIGHT_BLUE, 1), tool_num (tool_num_), tool_manager (tool_manager_)
{

}

void dx_tool_box::operate (dx_point mouse) noexcept
{
    tool_manager->active_tool = tool_num;
    // dprintf(2, "Active Tool: %d\n", tool_num);
}
