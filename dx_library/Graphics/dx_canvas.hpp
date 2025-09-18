#ifndef DX_CANVAS
#define DX_CANVAS

#include "dx_graphics.hpp"
#include "dx_tools.hpp"

class dx_canvas : public dx_abstract_field
{
private:

    enum
    {
        DEFAULT_WIDTH = 400,
        DEFAULT_HEIGHT = 300
    };


public:

    dx_tool_manager* tool_manager;
    dx_point last_point = dx_point (0, 0);

    dx_canvas (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (0, 0), dx_tool_manager* manager = NULL);

    virtual bool mouse_move (dx_point mouse, bool is_pressed) noexcept override;
    virtual bool mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept override;
    virtual void stop_mouse_move (void) noexcept override;
    virtual void operate (dx_point mouse) noexcept override;
  /*  

    virtual dx_abstract_object* contains_point (dx_point point) noexcept override;

    virtual void draw (void) const noexcept override;
    virtual void move_to (dx_point offset) noexcept;

    dx_point get_position (void) noexcept
    {
        return left_up;
    }
*/
};

#endif      // #ifndef DX_CANVAS