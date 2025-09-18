#include "dx_canvas.hpp"


dx_canvas::dx_canvas (dx_point point1, dx_point point2, dx_tool_manager* manager) : 
    dx_abstract_field (point1, point2, dx_colour(), 1), tool_manager (manager)
{
    image = new dx_image("../images/system/empty_canvas.jpg", this, 0); 
}

bool dx_canvas::mouse_move (dx_point mouse, bool is_pressed) noexcept
{
    if (!is_pressed)
        return 0;

    if (tool_manager)
        tool_manager->perform (mouse);
    
    last_point = mouse;
    return 0;
}

void dx_canvas::stop_mouse_move (void) noexcept
{
    last_point = dx_point (0, 0);
}

void dx_canvas::operate (dx_point mouse) noexcept
{
    last_point = dx_point (0, 0);
}

bool dx_canvas::mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept
{
    last_point = dx_point (0, 0);
    if (tool_manager)
        tool_manager->perform (mouse);
    return false;
}
