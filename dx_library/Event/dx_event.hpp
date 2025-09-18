#ifndef DX_EVENT
#define DX_EVENT

#include "../Primitives/dx_key_constants.hpp"

class mouse_released_event
{
public:

    mouse_released_event () = default;

    mouse_released_event (int x, int y, MOUSE_BUTTON key) : 
        mouse_pos (dx_point (x, y)), mouse_num (key){}

    dx_point mouse_pos = dx_point (0, 0);
    MOUSE_BUTTON mouse_num = MOUSE_BUTTON::LEFT;
};


class mouse_pressed_event
{
public:

    mouse_pressed_event () = default;

    mouse_pressed_event (int x, int y, MOUSE_BUTTON key) : 
        mouse_pos (dx_point (x, y)), mouse_num (key){}

    dx_point mouse_pos = dx_point (0, 0);
    MOUSE_BUTTON mouse_num = MOUSE_BUTTON::LEFT;
};




class mouse_move_event
{
public:
    mouse_move_event (int x, int y) : mouse_pos (dx_point (x, y)) {}

// private:
    dx_point mouse_pos;
};




class key_pressed_event
{
public:
    key_pressed_event (void) = default;
    explicit key_pressed_event (dx_key key) : key (key) {}
private:
    dx_key key = dx_key::A;
};


class key_released_event
{
public:
    key_released_event (void) = default;
    key_released_event (int key) : key (key) {}
private:
    int key = 0;
};



class scroll_event
{
public:

    scroll_event () = default;
    
    scroll_event (double offset, dx_point position) : 
        offset (offset), position (position) {}

    double offset;
    dx_point position;
};


class close_window_event
{

};



class Event
{
public:

    Event() {}

    enum
    {
        MOUSE_PRESSED,
        MOUSE_RELEASED,
        MOUSE_MOVE,
        
        KEY_PRESSED,
        KEY_RELEASED,

        SCROLL,

        CLOSE_WINDOW
    } tag;

    union
    {
        mouse_pressed_event     object_mouse_pressed;
        mouse_released_event    object_mouse_released;
        mouse_move_event        object_mouse_move;
        
        key_pressed_event       object_key_pressed;
        key_released_event      object_key_released;
        
        scroll_event            object_scroll;

        close_window_event      object_close_window;
    };
};

#endif              // #define DX_EVENT