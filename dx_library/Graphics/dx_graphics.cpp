#include "../Vector/dx_vector.hpp"
#include <assert.h>
#include <math.h>
#include "dx_graphics.hpp"

#include "../Engine/dx_using_engine.hpp"

namespace point
{
    dx_point DEFAULT = dx_point (0, 0);
};

double dx_rectangle::width() const noexcept
{
    return (right_down - left_up).x;
}

double dx_rectangle::height() const noexcept
{
    return (right_down - left_up).y;
}

dx_point dx_convert_coordinates (const dx_point& mouse, int window_width, int window_height) noexcept
{
    assert (window_width);
    assert (window_width);

    int half_width = window_width / 2;
    int half_height = window_height / 2;

    return dx_point ((mouse.x - half_width) / (half_width), (-mouse.y + half_height) / (half_height));
}


double radian_to_grad (double radian)
{
    return radian * 180 / M_PI;
}

bool between_two_points (dx_point first, dx_point second, dx_point check) noexcept
{
    if (first.x > second.x)
        std::swap (first, second);
    else if (first.x == second.x)
    {
        return first.x == check.x;
    }

    const double precision = 0.1;

    return abs ((check.x - first.x) * (second.y - first.y) - (second.x - first.x) * (check.y - first.y)) < precision;
}


bool is_inside_rectangle (dx_point point, dx_point left_up, dx_point right_down)
{
    if ((point.x >= left_up.x) && (point.x <= right_down.x))
        if ((point.y >= left_up.y) && (point.y <= right_down.y))
        {
            return true;
        }
    return false;
}



dx_line::dx_line (dx_point first, dx_point second, dx_colour the_colour, float width) : 
    first (first), second (second), width (width)
{
    colour = the_colour; 
    this->draw();
}

dx_abstract_object* dx_line::contains_point (dx_point point) noexcept
{
    if  (between_two_points (first, second, point))
        return reinterpret_cast <dx_abstract_object*> (this);
    
    return NULL;
}

void dx_line::draw (void) const noexcept
{ 
    float cur_width = Engine_t::method.get_width();
    Engine_t::method.set_width (width);
    Engine_t::method.draw_line (first, second, colour);
    Engine_t::method.set_width (cur_width);
}

void dx_line::set_width (float new_width) noexcept
{
    width = new_width;
}




dx_broken_line::dx_broken_line (dx_vector <dx_point>& points_new) : points (points_new)
{
    this->draw();
}

dx_abstract_object* dx_broken_line::contains_point (dx_point point) noexcept
{
    if (points.size() <= 1)
        return NULL;

    dx_point check_point (point.x, point.y);

    for (size_t i = 0; i < points.size() - 2; ++i)
        if (between_two_points (points [i], points [i + 1], check_point))
            return reinterpret_cast <dx_abstract_object*> (this);
    
    return NULL;
}

void dx_broken_line::draw (void) const noexcept
{
    if (points.size() <= 1)
        return;

    for (size_t i = 0; i < points.size() - 2; ++i)
        Engine_t::method.draw_line (points[i], points [i + 1]);
}



void set_points (dx_point& left_up, dx_point& right_down, dx_point& point1, dx_point& point2)
{
    left_up.x = std::min (point1.x, point2.x);
    left_up.y = std::min (point1.y, point2.y);
    
    right_down.x = std::max (point1.x, point2.x);
    right_down.y = std::max (point1.y, point2.y);
}


void dx_abstract_field::set_image (const char* image_name, bool scale)
{
    image = new dx_image (image_name, this, scale);
}

dx_abstract_object::~dx_abstract_object ()
{
    delete image;
}

void dx_abstract_object::leave_container (void) noexcept
{
    if (external)
        external->remove (*this);
    external = NULL;
}

dx_abstract_field::dx_abstract_field (dx_point point1, dx_point point2, dx_colour new_colour, bool flush) : flush (flush)
{
    this->colour = new_colour;
    this->flush  = flush; 
    set_points (left_up, right_down, point1, point2);
}

void dx_abstract_field::set_coordinates (dx_point _left_up_, dx_point _right_down_)
{
    set_points (left_up, right_down, _left_up_, _right_down_);

}

void dx_abstract_field::move_to (dx_point offset) noexcept
{
    set_coordinates (left_up + offset, right_down + offset);
}

void dx_abstract_field::draw (void) const noexcept
{
    Engine_t::method.draw_rect (left_up, right_down, colour, flush);

    if (image)
        image->draw();
}

dx_point dx_abstract_field::get_left_up (void) const noexcept
{
    return left_up;
}

dx_point dx_abstract_field::get_right_down (void) const noexcept
{
    return right_down;
}
dx_point dx_abstract_field::get_size (void) const noexcept
{
    return right_down - left_up;
}

dx_point dx_abstract_field::get_centre (void) const noexcept
{
    return (right_down - left_up) / 2.0 + left_up;
}

dx_point dx_abstract_field::get_position (void) noexcept
{
    return left_up;
}

double dx_abstract_field::width (void) const noexcept
{
    return (right_down - left_up).x;
}

double dx_abstract_field::height (void) const noexcept
{
    return (right_down - left_up).y;
}

dx_abstract_object* dx_abstract_field::contains_point (dx_point point) noexcept
{
    if (is_inside_rectangle (point, left_up, right_down))
        return this;
    return NULL;
}


void dx_object_container::draw (void) const noexcept
{
    if (flush)
        Engine_t::method.draw_rect (left_up, right_down, colour, flush);

    if (image)
        image->draw();

    for (auto& iter : container)
        iter->draw();
}

bool dx_object_container::mouse_move (dx_point mouse_pos, bool is_pressed) noexcept
{
    // bool found = 0;
    // for (auto iter : container)
    //     if (iter->mouse_move (mouse_pos))
    //         found = 1;
    //     else if (iter->is_hovered)
    //         {
    //             iter->stop_mouse_move();
    //             iter->is_hovered = false;
    //         }
    
    // if (!found)
    // {
    //     if (flush)
    //         ;// do something
    //     else
    //         return false;   
    // }

    // return true;

    return 0;

}

dx_abstract_object* dx_object_container::contains_point (dx_point point) noexcept               // check from end
{
    dx_abstract_object* found = NULL;
    if (flush)
    {
        if (is_inside_rectangle (point, this->left_up, this->right_down))
        {
            for (auto iter = container.rbegin(); iter != container.rend(); ++iter)
            {
                if (0 != (found = (*iter)->contains_point (point)))
                {
                    return found;
                }
            }
            return this;
        }

        return NULL;
    }
    else
    {
        for (auto iter = container.rbegin(); iter != container.rend(); ++iter)
            if ((*iter)->contains_point (point))
                return *iter;
    }
    
    return NULL;
}

void dx_object_container::push_back (dx_abstract_object& new_object) noexcept
{
    container.push_back (&new_object);
    new_object.external = this;
}

void dx_object_container::push_front (dx_abstract_object& new_object) noexcept
{
    container.push_front (&new_object);
    new_object.external = this;
}

void dx_abstract_object::to_front (void) noexcept
{
    if (external)
    {
        external->remove (*this);
        external->push_front (*this);
    }
}

void dx_abstract_object::to_back (void) noexcept
{
    if (external)
    {
        external->remove (*this);
        external->push_back (*this);
    }
}

bool dx_object_container::remove (dx_abstract_object& object) noexcept
{
    for (auto i : container)
    {
        if (&object == i)
        {
            container.remove (&object);
            object.external = NULL;
            return 1;
        }
    }
    return 0;
}



dx_hoverable_object::dx_hoverable_object (dx_point point1, dx_point point2, 
    dx_colour colour, bool flush) noexcept
{
    this->colour = colour;
    hover_colour = colour * HOVER_COEFFICIENT;
    pressed_colour = colour * PRESSED_COEFFICIENT;
    this->flush  = flush;

    set_points (left_up, right_down, point1, point2);
}

void dx_hoverable_object::set_colour (dx_colour new_colour) noexcept
{
    colour = new_colour;
    hover_colour = colour * PRESSED_COEFFICIENT;
    pressed_colour = colour * HOVER_COEFFICIENT;
}


void dx_hoverable_object::start_mouse_move (dx_point mouse) noexcept
{
    if (image)
        image->set_current_mask (dx_image::MASK::HOVER_COLOUR);
    else
        std::swap (hover_colour, colour);
}

void dx_hoverable_object::stop_mouse_move (void) noexcept              // used when mouse went out of the button
{
    if (image)
        image->set_current_mask (dx_image::MASK::NORMAL_COLOUR);
    else
        std::swap (hover_colour, colour);
}

void dx_hoverable_object::set_extra_colour (dx_colour _hover_color_, dx_colour _pressed_colour_) noexcept
{
    hover_colour = _hover_color_;
    pressed_colour = _pressed_colour_;
}


bool dx_draggable_object::mouse_down (dx_point mouse, MOUSE_BUTTON key_num) noexcept
{
    if (key_num != MOUSE_BUTTON::LEFT)
        return false;

    click_position = mouse - this->left_up;

    return true;                                                        // is draggable
}


void dx_draggable_object::move (const dx_point& mouse) noexcept
{
    move_to (mouse - left_up - click_position);
}


dx_image::dx_image (const char* image_name, dx_abstract_field* owner, bool scale) : owner (owner), scale (scale)
{
    colour_masks [NORMAL_COLOUR] = colour::WHITE;
    colour_masks [HOVER_COLOUR] = colour::WHITE * HOVER_COEFFICIENT;
    colour_masks [PRESS_COLOUR] = colour::WHITE * PRESSED_COEFFICIENT;

    where_to.left_up = owner->get_left_up();
    where_to.right_down = owner->get_right_down();

    load_from_file (image_name);
}

void dx_image::load_from_file (const char* file_name) noexcept
{
    pixels = Engine_t::method.load_from_file (file_name, image_size);

    where_from.left_up = dx_point (0, 0);
    
    if (scale == NO_SCALING)
        where_from.right_down = where_to.right_down - where_to.left_up;
    else
        where_from.right_down = image_size;
}


void dx_image::draw (void) const noexcept
{
    where_to.left_up = owner->get_left_up();
    where_to.right_down = owner->get_right_down();
    Engine_t::method.draw_image (pixels, image_size, where_to, where_from, scale, colour_masks [current_colour]);
}

void dx_image::move_source_window (const dx_point& delta) noexcept
{
    where_from.left_up = where_from.left_up + delta;
    where_from.right_down = where_from.right_down + delta;
}

void dx_image::change_source_window (dx_point left_up_, double width, double height) noexcept
{
    where_from.left_up = left_up_;
    where_from.right_down = where_from.left_up + dx_point (width, height);
}

void dx_image::set_colour_mask (dx_colour hover_colour, dx_colour press_colour) noexcept
{
    colour_masks [dx_image::MASK::HOVER_COLOUR] = hover_colour;
    colour_masks [dx_image::MASK::PRESS_COLOUR] = press_colour;
}

void dx_image::set_current_mask (dx_image::MASK cur_mask) noexcept
{
    this->current_colour = cur_mask;
}

dx_point dx_image::get_size (void) noexcept
{
    return image_size;
}

dx_point dx_image::get_source_position (void) noexcept
{
    return this->where_from.left_up;
}

void dx_image::set_pixel (dx_point pos, dx_colour colour) noexcept
{
    if (pos.x < 0 || pos.y >= image_size.x || pos.y < 0 || pos.y > image_size.y)
        return;

    uint64_t array_pos = (int(pos.y) * this->image_size.x + int(pos.x)) * 4;
    memcpy (&pixels [array_pos], &colour, 4);
}

void dx_image::draw_circle (dx_point centre, int radius, dx_colour colour) noexcept
{
    int r2 = radius * radius;
    dx_point start = centre - dx_point (radius, radius);

    for (int y = -radius; y < radius; ++y)
    {
        for (int x = -radius; x < radius; ++x)
        {
            if (x * x + y * y <= r2)
                set_pixel (centre + dx_point (x, y), colour);
        }
    }
}

void dx_image::draw_line (dx_point start, dx_point end, int width, dx_colour colour) noexcept
{
    if (!pixels)
        return;

    if (start.y > end.y)
        std::swap (start, end);

    dx_point delta = end - start;
    double length = hypot (delta.x, delta.y);
    double cos = delta.x / length;
    double sin = delta.y / length;

    for (int i = 0; i < int(length); ++i)
        draw_circle (dx_point (i * cos + start.x, i * sin + start.y), width, colour);
}

dx_image::~dx_image()
{
    delete [] pixels;
}


void dx_graphic::draw (void) const noexcept
{
/*
    dx_point left_up = this->left_up;

    GLfloat current_line_width = Engine_t::method.get_width();
    Engine_t::method.set_width (2);

    float width = right_down.x - left_up.x;
    float height = left_up.y - right_down.y;

    const float near = 0.05;

    const float absciss_length = (1 - 2 * near) * width;
    const float ordynat_length = (1 - 2 * near) * height;

    Engine_t::method.draw_rect (left_up, right_down, dx_colour (255, 255, 255), true);
    Engine_t::method.draw_arrow_line (dx_point (left_up.x + near * width, right_down.y + near * height), dx_point (left_up.x + near * width, left_up.y - near * height), dx_colour (0, 0, 0));
    Engine_t::method.draw_arrow_line (dx_point (left_up.x + near * width, right_down.y + near * height), dx_point (right_down.x - near * width, right_down.y + near * height), dx_colour (0, 0, 0));

    double min_x = points[0].x;
    double max_x = points[points.size() - 1].x;
    double min_y = points[0].y;
    double max_y = points[0].y;

    for (size_t i = 1; i < points.size(); ++i)
    {   
        if (points[i].y < min_y)
            min_y = points[i].y;

        if (points[i].y > max_y)
            max_y = points[i].y;
    }

    const float unused_part = 1.0 / 20;
    const float available_absciss = absciss_length * (1 - 2 * unused_part);
    const float available_ordynat = ordynat_length * (1 - 2 * unused_part);

    const float relation_x = available_absciss / (max_x - min_x);
    const float relation_y = available_ordynat / (max_y - min_y);           /////// !!!!!!!!!!!

    const float start_x = unused_part * absciss_length + near * width  + left_up.x;
    const float start_y = unused_part * ordynat_length + near * height + right_down.y;

    glBegin (GL_LINE_STRIP);
        for (size_t i = 0; i < points.size(); ++i)
        {
            glVertex2d ((points[i].x - min_x) * relation_x + start_x, (points[i].y - min_y) * relation_y + start_y);
        }
    glEnd();

    Engine_t::method.set_width (current_line_width);
*/
}
