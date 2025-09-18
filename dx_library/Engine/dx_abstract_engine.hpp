#ifndef DX_ABSTRACT_ENGINE
#define DX_ABSTRACT_ENGINE

#include "../Graphics/dx_graphics.hpp"
#include "../Event/dx_event.hpp"
#include <queue>



class Iengine
{
public:

    virtual void create_window_fullscreen (const char* file_name = "Window") = 0;                                                                                                    

    virtual void create_window (const char* file_name = "Window") = 0;

    virtual void close_window (void) noexcept = 0;

    virtual bool is_opened (void) const noexcept = 0;

    virtual void set_window_colour (dx_colour colour) noexcept = 0;

    virtual void set_window_size (dx_point _window_size_) = 0;

    virtual dx_point get_window_size (void) noexcept = 0;

    virtual void terminate (void) const noexcept = 0;

    virtual void update_while_running (void) noexcept = 0;

    virtual int event_queue_size (void) const noexcept = 0;

    virtual Event get_event (void) noexcept = 0;

    virtual void draw_quadrangle (dx_point left_up, dx_point right_up, 
        dx_point right_down, dx_point left_down, dx_colour colour, bool flush) const noexcept = 0;
    
    virtual void draw_rect (dx_point left_up, dx_point right_down, 
        dx_colour colour, bool flush) const noexcept = 0;

    virtual void draw_rect (dx_point left_up, double width, double height,
        dx_colour colour, bool flush) const noexcept = 0;

    virtual void draw_line (dx_point point1, dx_point point2,
        dx_colour colour, float width) const noexcept = 0;

    virtual void draw_arrow_line (dx_point point1, dx_point point2,
        dx_colour colour) const noexcept = 0;

    virtual void draw_graphic (dx_point left_up, dx_point right_down,
        const dx_vector <dx_point>& points) const noexcept = 0;

    virtual float get_width (void) const noexcept = 0;

    virtual void set_width (float new_width) noexcept = 0;

    virtual dx_point get_mouse_pos (void) noexcept = 0;


///     Image handling

    virtual uint8_t* load_from_file (const char* file_name, dx_point& image_size) = 0;
    virtual void draw_image (const uint8_t* pixels, dx_point real_size, dx_rectangle where_to, dx_rectangle where_from, bool scale, dx_colour new_colour) const noexcept = 0;
    virtual void set_icon (const char* icon_name) noexcept = 0;
    virtual void load_to_file (const char* file_name, const uint8_t* pixels) noexcept = 0;
////

    float feather_width = 1;

    dx_colour clear_colour = colour::WHITE;
    dx_point window_size = dx_point (400, 400);

};

template <typename Engine_Implementation>
class Engine : public Engine_Implementation
{
public:
    static Engine_Implementation method;
};

template <typename Engine_Implementation>
Engine_Implementation Engine <Engine_Implementation>::method;

#endif                  // #ifndef DX_ABSTRACT_ENGINE