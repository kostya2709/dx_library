
#ifndef DX_TEXT_ENGINE
#define DX_TEXT_ENGINE

#include <stdio.h>

#include "dx_abstract_engine.hpp"


class Text_Engine : public Iengine
{
public:

    virtual void create_window_fullscreen (const char* file_name = "Window") override {}                                                                                                  

    virtual void create_window (const char* file_name = "Window") override {}

    virtual void close_window (void) const noexcept override {}

    virtual bool window_should_close (void) const noexcept override {}

    virtual void set_window_colour (dx_colour colour) noexcept override {}

    virtual void set_window_size (dx_point _window_size_) override {}

    virtual dx_point get_window_size (void) noexcept override {}

    virtual void terminate (void) const noexcept override {}

    virtual void update_while_running (void) const noexcept override {}

    virtual int event_queue_size (void) const noexcept override {}

    virtual Event get_event (void) noexcept override {}

    virtual void draw_quadrangle (dx_point left_up, dx_point right_up, 
        dx_point right_down, dx_point left_down, dx_colour colour, bool flush) const noexcept override {}
    
    virtual void draw_rect (dx_point left_up, dx_point right_down, 
        dx_colour colour, bool flush) const noexcept override {}
    
    virtual void draw_rect (dx_point left_up, double width, double height,
        dx_colour colour, bool flush) const noexcept override {}

    virtual void draw_line (dx_point point1, dx_point point2,
        dx_colour colour, float width) const noexcept override {}

    virtual void draw_arrow_line (dx_point point1, dx_point point2,
        dx_colour colour) const noexcept override{}

    virtual void draw_graphic (dx_point left_up, dx_point right_down,
        const dx_vector <dx_point>& points) const noexcept override {}

    virtual float get_width (void) const noexcept override{}

    virtual void set_width (float new_width) noexcept override;

    virtual dx_point get_mouse_pos (void) noexcept override {}
};


void Text_Engine :: set_width (float) noexcept
{
    printf ("Imagine, there is a line!\n");
}

#endif                  // #ifndef DX_TEXT_ENGINE