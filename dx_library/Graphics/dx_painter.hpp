#ifndef DX_PAINTER
#define DX_PAINTER

#include "dx_graphics.hpp"
#include "../Scrollbar/dx_scroll_bar.hpp"
#include "dx_tools.hpp"
#include "dx_canvas.hpp"

class dx_painter : public dx_object_container
{
    dx_tool_manager tool_manager;

public:

    dx_canvas* thickness_demonstrator;
    dx_colour colour = colour::BLACK;
    dx_canvas* canvas = NULL;
    double width = 2;

    dx_painter (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (200, 200));
};


class dx_palette : public dx_object_container
{
public:
    dx_painter* painter;
    dx_abstract_field* cur_colour;

    dx_palette (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (200, 50), dx_painter* painter = NULL);
    void set_colour (dx_colour colour) noexcept;
    virtual ~dx_palette();
};




class dx_colour_box : public dx_hoverable_object
{
public:
    dx_palette* palette;
    dx_colour colour;

    dx_colour_box (dx_palette* palette, dx_point pos, int size, dx_colour colour = colour::DEFAULT);
    void operate (dx_point mouse) noexcept override;
};

class dx_thickness_arrow
{
public:

    enum
    {
        UP = 1, DOWN = -1
    };

    int direction = 1;
    double delta = 0.05;
    dx_painter* painter;

    void operator () (void)
    {
        dx_point centre = painter->thickness_demonstrator->get_centre() - painter->thickness_demonstrator->get_left_up();
        dx_point size = painter->thickness_demonstrator->get_size();
        
        painter->thickness_demonstrator->image->draw_circle (centre, 2 * size.x, colour::WHITE);

        if (direction == UP && (painter->width + delta < 50))
            painter->width += delta;
        else if ((painter->width - delta) > 1)
            painter->width -= delta;
        
        painter->thickness_demonstrator->image->draw_circle (centre, painter->width, painter->colour);
    }
};

#endif              // #ifndef DX_PAINTER