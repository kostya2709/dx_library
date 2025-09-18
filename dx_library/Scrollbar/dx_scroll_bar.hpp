#ifndef SCROLL_BAR
#define SCROLL_BAR

#include "../Graphics/dx_graphics.hpp"



class dx_scrollable_object;

class dx_abstract_scroll_bar : public dx_object_container
{
protected:

    double scroll_offset = 0;
    dx_scrollable_object* window = NULL;

public:

    static int STANDARD_SLIDER_WIDTH;
    static dx_point STANDARD_SLIDER_SIZE;
    
    dx_abstract_scroll_bar (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (0, 0), dx_scrollable_object* window = NULL);

    virtual void update (double new_position);   
    void update_scroll_offset (double new_position);          
    virtual void set_slider (double new_position) noexcept = 0;
    
    double change_scroll_offset (double delta) noexcept;
    double get_scroll_offset (void)
    {
        return scroll_offset;
    }

    virtual ~dx_abstract_scroll_bar ();
};


class dx_horizontal_scroll_bar : public dx_abstract_scroll_bar
{

public:
    
    dx_horizontal_scroll_bar (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (0, 0), dx_scrollable_object* window = NULL);
    dx_horizontal_scroll_bar (dx_scrollable_object* window);

    virtual void set_slider (double new_position) noexcept override;

    void update (double new_position) override;
};


class dx_vertical_scroll_bar : public dx_abstract_scroll_bar
{

public:
    
    dx_vertical_scroll_bar (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (0, 0), dx_scrollable_object* window = NULL);
    dx_vertical_scroll_bar (dx_scrollable_object* window);

    virtual void set_slider (double new_position) noexcept override;
    void update (double new_position) override;

    // void update (double new_position) {};              // in range 0..100
};


class dx_abstract_slider : public dx_draggable_object                 // draggable + vector_direction
{
public:

    dx_abstract_scroll_bar* scroll_bar = NULL;

    int min_edge = INT32_MIN;
    int max_edge = INT32_MAX;

    dx_abstract_slider (dx_point point1, dx_point point2, dx_colour colour = colour::DEFAULT, bool flush = 1) :
        dx_draggable_object (point1, point2, colour, flush) {} 
    
    virtual void move (const dx_point& mouse) noexcept override {}
};

class dx_vertical_slider : public dx_abstract_slider                 // draggable + vector_direction
{

public:

    dx_vertical_slider (dx_point point1, dx_point point2, dx_colour colour_ = colour::DEFAULT, bool flush_ = 1) :
        dx_abstract_slider (point1, point2, colour_, flush_) {}
    
    virtual void move (const dx_point& mouse) noexcept override;
};


class dx_horizontal_slider : public dx_abstract_slider                 // draggable + vector_direction
{

public:

    dx_horizontal_slider (dx_point point1, dx_point point2, dx_colour colour_ = colour::DEFAULT, bool flush_ = 1)  :
        dx_abstract_slider (point1, point2, colour_, flush_) {}

    virtual void move (const dx_point& mouse) noexcept override;
};


class dx_scrollable_object : public dx_object_container
{
    int frame = 0;
    dx_abstract_field* picture = NULL;

public:
    dx_scrollable_object (dx_point point1, dx_point point2, const char* image_name, dx_colour colour = colour::GREY_BACKGROUND);
    dx_scrollable_object (dx_point point1, dx_point point2, dx_abstract_field* image_field, dx_colour colour = colour::GREY_BACKGROUND);

    friend class dx_horizontal_scroll_bar;
    friend class dx_vertical_scroll_bar;

    virtual ~dx_scrollable_object();
};


#endif                  // #define SCROLL_BAR