
#include "dx_scroll_bar.hpp"


dx_point dx_abstract_scroll_bar::STANDARD_SLIDER_SIZE = dx_point (STANDARD_SLIDER_WIDTH, STANDARD_SLIDER_WIDTH);
int dx_abstract_scroll_bar::STANDARD_SLIDER_WIDTH = 20;

class func_arrow
{
public:

    dx_abstract_scroll_bar* scroll_bar = NULL;
    double delta = 0.0005;

    void operator () (void)
    {
        scroll_bar->change_scroll_offset (delta);
    }
};



dx_abstract_scroll_bar::dx_abstract_scroll_bar (dx_point point1, dx_point point2, dx_scrollable_object* window_)
{
    flush = 1;
    colour = colour::LIGHT_BLUE;
    set_points (point1, point2, point1, point2);
    set_coordinates (point1, point2);

    this->window = window_;
}

double dx_abstract_scroll_bar::change_scroll_offset (double delta) noexcept         // adds a value to the offset
{
    update (scroll_offset + delta); 
    set_slider (scroll_offset);
    return scroll_offset;
}

void dx_abstract_scroll_bar::update_scroll_offset (double new_position)
{
    scroll_offset = new_position;
    
    if (scroll_offset > 1)
        scroll_offset = 1;
    else if (scroll_offset < 0)
        scroll_offset = 0;
}

void dx_abstract_scroll_bar::update (double new_position)
{
    update_scroll_offset (new_position);
}

void dx_horizontal_scroll_bar::update (double new_position)
{
    update_scroll_offset (new_position);

    if (this->window && this->window->picture)
    {
        dx_point start = this->window->picture->image->get_source_position();
        dx_point image_size = window->picture->image->get_size();
        dx_point box_size = window->picture->get_right_down() - window->picture->get_left_up();
        dx_point updated = dx_point ((image_size.x - box_size.x) * scroll_offset, start.y);
        window->picture->image->move_source_window (updated - start);
    }
}

void dx_vertical_scroll_bar::update (double new_position)
{
    update_scroll_offset (new_position);

    if (this->window && this->window->picture)
    {
        dx_point start = this->window->picture->image->get_source_position();
        dx_point image_size = window->picture->image->get_size();
        dx_point box_size = window->picture->get_right_down() - window->picture->get_left_up();
        dx_point updated = dx_point (start.x, (image_size.y - box_size.y) * scroll_offset);
        window->picture->image->move_source_window (updated - start);
    }
}

dx_abstract_scroll_bar::~dx_abstract_scroll_bar ()
{
    for (auto i : container)
        delete i;
}


dx_horizontal_scroll_bar::dx_horizontal_scroll_bar (dx_point point1, dx_point point2, dx_scrollable_object* window_) : 
    dx_abstract_scroll_bar (point1, point2, window_)
{
    double arrow_size = abs (point1.y - point2.y);
    dx_point arrow (arrow_size, arrow_size);
    double slider_size = 0;
    
    if (window)
    {
        double width = (window->picture->get_right_down() - window->picture->get_left_up()).x;
        slider_size = std::min ((width / window->picture->image->get_size().x), 1.0) * (abs (point1.x - point2.x) - 2 * arrow_size);
    }
    else 
        slider_size = 2 * arrow_size;

    dx_button <func_arrow>* button_up = new dx_button <func_arrow> (point1 + arrow, point1, colour::RED, 1);
    button_up->function.scroll_bar = this;
    button_up->function.delta *= (-1);
    button_up->handle_pressed = 1;
    button_up->set_image ("../images/scrollbar/arrow_left.jpg");
    button_up->image->set_colour_mask (colour::SKY_BLUE);

    dx_button <func_arrow>* button_down = new dx_button <func_arrow> (point2, point2 - arrow, colour::RED, 1);
    button_down->function.scroll_bar = this;
    button_down->handle_pressed = 1;
    button_down->set_image ("../images/scrollbar/arrow_right.jpg");
    button_down->image->set_colour_mask (colour::SKY_BLUE);

    dx_horizontal_slider* slider = new dx_horizontal_slider (dx_point (point1.x + arrow_size, point1.y), dx_point (point1.x + arrow_size + slider_size, point1.y + arrow_size), colour::SILVER);
    
    
    slider->min_edge = point1.x + arrow_size;
    slider->max_edge = point2.x - arrow_size - slider_size;
    slider->scroll_bar = this;
    slider->set_image ("../images/scrollbar/horizontal_slider.jpg");
    slider->image->set_colour_mask (colour::SKY_BLUE);

    container.push_back (slider);
    container.push_back (button_up);                // Pay attention! The order is IMPORTANT!
    container.push_back (button_down);

}

dx_horizontal_scroll_bar::dx_horizontal_scroll_bar (dx_scrollable_object* window_) : 
    dx_horizontal_scroll_bar    (dx_point (window_->get_left_up().x, window_->get_right_down().y - STANDARD_SLIDER_WIDTH), window_->get_right_down() - dx_point (STANDARD_SLIDER_WIDTH, 0), window_)    
{}

void dx_horizontal_scroll_bar::set_slider (double new_position) noexcept
{
    dx_horizontal_slider* slider = dynamic_cast <dx_horizontal_slider*> (container.front());             // get slider
    double delta = (slider->max_edge - slider->min_edge) * new_position;
    slider->move_to (dx_point (slider->min_edge + delta - slider->get_position().x, 0));
}

dx_vertical_scroll_bar::dx_vertical_scroll_bar (dx_point point1, dx_point point2, dx_scrollable_object* window_) : dx_abstract_scroll_bar (point1, point2, window_)
{
    double arrow_size = point2.x - point1.x;
    dx_point arrow (arrow_size, arrow_size);
    double slider_size = 0;

    if (window)
    {
        double height = (window->picture->get_right_down() - window->picture->get_left_up()).y;
        slider_size = std::min ((height / window->picture->image->get_size().y), 1.0) * (abs (point1.y - point2.y) - 2 * arrow_size);
    }
    else 
        slider_size = 2 * arrow_size;

    dx_button <func_arrow>* button_up = new dx_button <func_arrow> (point1, point1 + arrow, colour::RED, 1);
    button_up->function.scroll_bar = this;
    button_up->set_image ("../images/scrollbar/arrow_up.jpg");
    button_up->image->set_colour_mask (colour::SKY_BLUE);
    button_up->function.delta *= (-1);
    button_up->handle_pressed = 1;
    
    dx_button <func_arrow>* button_down = new dx_button <func_arrow> (point2 - arrow, point2, colour::RED, 1);
    button_down->function.scroll_bar = this;
    button_down->handle_pressed = 1;
    button_down->set_image ("../images/scrollbar/arrow_down.jpg");
    button_down->image->set_colour_mask (colour::SKY_BLUE);

    dx_vertical_slider* slider = new dx_vertical_slider (dx_point (point1.x, point1.y + arrow_size), dx_point (point1.x + arrow_size, point1.y + arrow_size + slider_size), colour::SILVER);
    slider->set_image ("../images/scrollbar/slider.jpg");
    slider->image->set_colour_mask (colour::SKY_BLUE);

    slider->min_edge = point1.y + arrow_size;
    slider->max_edge = point2.y - arrow_size - slider_size;
    slider->scroll_bar = this;

    container.push_back (slider);
    container.push_back (button_up);                // Pay attention! The order is IMPORTANT!
    container.push_back (button_down);
}

dx_vertical_scroll_bar::dx_vertical_scroll_bar (dx_scrollable_object* window_) : 
    dx_vertical_scroll_bar    (dx_point (window_->get_right_down().x - STANDARD_SLIDER_WIDTH, window_->get_left_up().y), window_->get_right_down() - dx_point (0, STANDARD_SLIDER_WIDTH), window_)    
{}

void dx_vertical_scroll_bar::set_slider (double new_position) noexcept
{
    dx_vertical_slider* slider = dynamic_cast <dx_vertical_slider*> (container.front());
    double delta = (slider->max_edge - slider->min_edge) * new_position;
    slider->move_to (dx_point (0, slider->min_edge + delta - slider->get_position().y));
}


void dx_vertical_slider::move (const dx_point& mouse) noexcept
{
    dx_point delta_move = mouse - click_position - left_up;

    delta_move.x = 0;

    if ((min_edge == left_up.y && delta_move.y < 0) || (max_edge == left_up.y && delta_move.y > 0))
        delta_move.y = 0;

    if (left_up.y < min_edge)
        delta_move.y = min_edge - left_up.y;
    else if (left_up.y > max_edge)
        delta_move.y = max_edge - left_up.y;

    if (scroll_bar)
        scroll_bar->update ((left_up.y - min_edge) / (max_edge - min_edge));

    move_to (delta_move);
}

void dx_horizontal_slider::move (const dx_point& mouse) noexcept
{
    dx_point delta_move = mouse - click_position - left_up;

    if ((min_edge == left_up.x && delta_move.x < 0) || (max_edge == left_up.x && delta_move.x > 0))
        delta_move.x = 0;
    
    delta_move.y = 0;
    if (left_up.x < min_edge)
        delta_move.x = min_edge - left_up.x;
    else if (left_up.x > max_edge)
        delta_move.x = max_edge - left_up.x;

    if (scroll_bar)
        scroll_bar->update ((left_up.x - min_edge) / (max_edge - min_edge));

    move_to (delta_move);
}



dx_scrollable_object::dx_scrollable_object (dx_point point1, dx_point point2, const char* image_name, dx_colour colour) :
    dx_object_container (point1, point2, colour, 1)
{

    if (!image_name)
        throw std::runtime_error ("Error! Invalid name in scrollable object!");

    picture = new dx_abstract_field (point1 + dx_abstract_scroll_bar::STANDARD_SLIDER_SIZE, point2 - dx_abstract_scroll_bar::STANDARD_SLIDER_SIZE);
    picture->get_size();
    picture->set_image (image_name, 0);
    
    dx_vertical_scroll_bar* vertical_scroll_bar = new dx_vertical_scroll_bar (this);
    dx_horizontal_scroll_bar* horizontal_scroll_bar = new dx_horizontal_scroll_bar (this);

    container.push_back (picture);
    container.push_back (vertical_scroll_bar);
    container.push_back (horizontal_scroll_bar);
}

dx_scrollable_object::dx_scrollable_object (dx_point point1, dx_point point2, dx_abstract_field* image_field, dx_colour colour) :
    dx_object_container (point1, point2, colour, 1)
{
    picture = image_field;
    
    dx_vertical_scroll_bar* vertical_scroll_bar = new dx_vertical_scroll_bar (this);
    dx_horizontal_scroll_bar* horizontal_scroll_bar = new dx_horizontal_scroll_bar (this);

    container.push_back (picture);
    container.push_back (vertical_scroll_bar);
    container.push_back (horizontal_scroll_bar);
}


dx_scrollable_object::~dx_scrollable_object()
{
    for (auto i : container)
        delete i;
}