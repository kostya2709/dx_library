#ifndef DX_GRAPHICS
#define DX_GRAPHICS

#include "../Primitives/dx_key_constants.hpp"
#include "../Primitives/dx_colour.hpp"
#include "../Primitives/dx_point.hpp"
#include "../Vector/dx_vector.hpp"
#include <list>
#include <memory>
#include <math.h>

using std::unique_ptr;

double radian_to_grad (double radian);

class dx_rectangle
{
public:
    dx_point left_up;
    dx_point right_down;

    dx_rectangle (dx_point left_up_, dx_point right_down_) : left_up (left_up_), right_down (right_down_) {}
    dx_rectangle (void) : left_up (0, 0), right_down (0, 0) {}
    double width()  const noexcept;
    double height() const noexcept;
};

const int DEFAULT_WIDTH = 1;
const double PRESSED_COEFFICIENT = 0.5;
const double HOVER_COEFFICIENT = 0.8;

class dx_abstract_object;

struct dx_mouse_info
{
    bool is_pressed = 0;
    bool is_clicked_draggable = 0;
    dx_point current_position;
    dx_abstract_object* hovered_object = NULL;
    dx_abstract_object* clicked_object = NULL;
};


dx_point dx_convert_coordinates (const dx_point& mouse, int window_width, int window_height) noexcept;
class dx_image;

class dx_object_event_handler
{
public:

    bool is_pressed = 0;
    bool handle_pressed = 0;

    virtual void start_mouse_move (dx_point mouse) noexcept = 0;
    virtual bool mouse_move (dx_point mouse, bool is_pressed) noexcept = 0;
    virtual void stop_mouse_move (void) noexcept = 0;

    virtual void operate (dx_point mouse) noexcept = 0;
    virtual bool mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept = 0;
    virtual ~dx_object_event_handler() {}
};

class dx_object_container;

class dx_abstract_object : public dx_object_event_handler
{
public:

    dx_object_container* external = NULL;
    dx_image* image = NULL;
    dx_colour colour;

    virtual void set_colour (dx_colour new_colour) noexcept
    {
        colour = new_colour;
    }

    virtual void start_mouse_move (dx_point mouse) noexcept override {}
    virtual bool mouse_move (dx_point mouse, bool is_pressed) noexcept override {return 0;}
    virtual void stop_mouse_move (void) noexcept override {}

    virtual void operate (dx_point mouse) noexcept override {}
    virtual bool mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept override {return 0;}
    virtual dx_abstract_object* contains_point (dx_point point) noexcept
    {
        return NULL;
    }
    
    virtual void draw (void) const noexcept {}
    virtual void move_to (dx_point offset) noexcept {};

    void leave_container (void) noexcept;
    void to_front (void) noexcept;
    void to_back (void) noexcept;

    virtual ~dx_abstract_object();
};



bool between_two_points (dx_point first, dx_point second, dx_point check) noexcept;



class dx_line : public dx_abstract_object
{
    dx_point first;
    dx_point second;
    float width = 1;

public:

    dx_line (dx_point first, dx_point second, dx_colour colour = colour::DEFAULT, float width = 1);
    virtual dx_abstract_object* contains_point (dx_point point) noexcept override;

    virtual void draw (void) const noexcept override;
    void set_width (float new_width) noexcept;
};





class dx_broken_line : dx_abstract_object
{
    dx_vector <dx_point> points;

public:
    dx_broken_line (dx_vector <dx_point>& points_new);
    virtual dx_abstract_object* contains_point (dx_point point) noexcept;

    virtual void draw (void) const noexcept override;
};




class dx_abstract_field : public dx_abstract_object                 // abstract_object + left_up + right_down
{
protected:
    dx_point left_up;
    dx_point right_down;
    bool flush = 0;

public:
    dx_abstract_field (void){}
    dx_abstract_field (dx_point point1, dx_point point2, dx_colour colour = colour::DEFAULT, bool flush = 0);

    virtual dx_abstract_object* contains_point (dx_point point) noexcept override;

    virtual bool mouse_move (dx_point mouse, bool is_pressed) noexcept override {return 0;}
    virtual void draw (void) const noexcept override;
    virtual void move_to (dx_point offset) noexcept;

    void set_image (const char* image_name, bool scale = 1);

    dx_point get_position (void) noexcept;

    void set_coordinates (dx_point _left_up_, dx_point _right_down_);
    dx_point get_left_up (void) const noexcept;
    dx_point get_right_down (void) const noexcept;
    dx_point get_size (void) const noexcept;
    dx_point get_centre (void) const noexcept;
    double width (void) const noexcept;
    double height (void) const noexcept;
};


class dx_object_container : public dx_abstract_field
{
public:

    std::list <dx_abstract_object*> container;
    
    dx_object_container (dx_point point1 = dx_point (0, 0),  
        dx_point point2 = dx_point (0, 0), dx_colour colour = colour::DEFAULT, bool flush = 0) :
            dx_abstract_field (point1, point2, colour, flush)  {}

    virtual bool mouse_move (dx_point mouse, bool is_pressed) noexcept override;
    virtual dx_abstract_object* contains_point (dx_point point) noexcept override;

    virtual void draw (void) const noexcept override;

    void push_back (dx_abstract_object& new_object) noexcept;
    void push_front (dx_abstract_object& new_object) noexcept;

    bool remove (dx_abstract_object& object) noexcept;

    virtual ~dx_object_container (){}

    friend class dx_application;
};


void set_points (dx_point& left_up, dx_point& right_down, dx_point& point1, dx_point& point2);


class dx_hoverable_object : public dx_abstract_field                 
{
protected:
    dx_colour hover_colour = colour::DEFAULT;
    dx_colour pressed_colour = colour::DEFAULT;

public:

    dx_hoverable_object (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (0, 0),
        dx_colour colour = colour::DEFAULT, bool flush = 1) noexcept;

    virtual void start_mouse_move (dx_point mouse) noexcept override;
    virtual void stop_mouse_move (void) noexcept override;

    virtual void set_colour (dx_colour new_colour) noexcept;

    void set_extra_colour (dx_colour _hover_color_, dx_colour _pressed_colour_ = colour::DEFAULT) noexcept;
};




template <typename functor>
class dx_button : public dx_hoverable_object                 // abstract_field + functor
{

public:

    dx_button (void) {}
    dx_button (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (0, 0),
        dx_colour colour = colour::DEFAULT, bool flush = 1) noexcept;

    virtual void operate (dx_point mouse) noexcept override;
    virtual bool mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept override;

    virtual ~dx_button(){}

    functor function;
};

template <typename functor>
dx_button <functor>::dx_button (dx_point point1, dx_point point2, 
    dx_colour colour, bool flush) noexcept
{
    this->colour = colour;
    hover_colour = colour * 0.8;
    pressed_colour = colour * 0.5;
    this->flush  = flush;

    set_points (left_up, right_down, point1, point2);
}


template <typename functor>
bool dx_button <functor>::mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept
{
    if (key_num != MOUSE_BUTTON::LEFT)
        return false;

    function(); 
    return false;                                                       /// is not draggable
}


template <typename functor>
void dx_button <functor>::operate (dx_point mouse) noexcept
{
    // printf ("RELEASED\n");
}



class dx_draggable_object : public dx_hoverable_object                 // abstract_field + click_position
{
protected:
    dx_point click_position = dx_point (0, 0);

public:

    dx_draggable_object (void) {}
    dx_draggable_object (dx_point point1, dx_point point2, dx_colour colour = colour::DEFAULT, bool flush = 1) :
        dx_hoverable_object (point1, point2, colour, flush) {}

    virtual void operate (dx_point mouse) noexcept override {};
    virtual bool mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept override;
    
    virtual void move (const dx_point& mouse) noexcept;


    virtual ~dx_draggable_object (){}
};


class dx_image : public dx_abstract_object
{
public:

    enum MASK
    {
        NORMAL_COLOUR,
        HOVER_COLOUR,
        PRESS_COLOUR
    };

private:

    uint8_t* pixels = NULL;

    mutable dx_rectangle where_to;
    dx_rectangle where_from;

    dx_colour colour_masks [3] = {};
    dx_colour colour_test = {};
    dx_image::MASK current_colour = NORMAL_COLOUR;

    dx_point image_size;

    dx_abstract_field* owner = NULL;
    bool scale = 1;

public:

    enum
    {
        NO_SCALING,
        SCALING
    };

    dx_image (void) {}
    explicit dx_image (const char* image_name, dx_abstract_field* owner, bool scale = SCALING);

    virtual void draw (void) const noexcept override;
    void load_from_file (const char* file_name) noexcept;
    void move_source_window (const dx_point& delta) noexcept;
    void change_source_window (dx_point left_up_, double width, double height) noexcept;
    void set_colour_mask (dx_colour hover_mask, dx_colour press_mask = dx_colour (255, 255, 255)) noexcept;
    void set_current_mask (dx_image::MASK cur_mask) noexcept;
    dx_point get_source_position (void) noexcept;
    dx_point get_size (void) noexcept;

    void set_pixel (dx_point pos, dx_colour colour) noexcept;
    void draw_circle (dx_point centre, int radius, dx_colour colour) noexcept;
    void draw_line (dx_point start, dx_point end, int width, dx_colour colour) noexcept;

    friend class dx_plugin;

    ~dx_image();
};


class dx_graphic : public dx_abstract_field
{
    dx_vector <dx_vector <dx_point>> functions;

public:

    virtual void draw (void) const noexcept override;
    virtual ~dx_graphic() {}

    dx_graphic (dx_point point1, dx_point point2, dx_colour colour = colour::DEFAULT, bool flush = 0) : dx_abstract_field (point1, point2, colour, flush) {}
};


#endif              // #define DX_GRAPHICS



