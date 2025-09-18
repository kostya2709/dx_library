#ifndef DX_APPLICATION
#define DX_APPLICATION

#include "dx_graphics.hpp"

class dx_application
{
public:

    enum system
    {
        relative_coordinate_system,
        absolute_coordinate_system
    };

    static void run (void);
    static void add (dx_abstract_object& new_object) noexcept;
    static void remove (dx_abstract_object& expired_object) noexcept;

    static void set_window_size (int _window_width_, int _window_height_);
    static void maximize_window (void) noexcept;
    static void set_window_name (char* _window_name_);
    static void set_window_colour (dx_colour _colour_);

    static void handle_mouse_move (void);

    static void set_coordinate_system (dx_application::system sys) noexcept;
    static dx_application::system get_coordinate_system (void) noexcept;


private:
    dx_application  (void) {}

    static void handle_events (void) noexcept;

    static char* window_name;
    static bool is_maximized;

    static dx_object_container windows;
    static dx_mouse_info mouse;
};

#endif      // #ifndef DX_APPLICATION