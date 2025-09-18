#include "dx_sfml_engine.hpp"
#include <math.h>
#include <stdexcept>


sf::Vector2f sfml_vector (const dx_point& point)
{
    return sf::Vector2f (point.x, point.y);
}

sf::Vector2i sfml_vector_i (const dx_point& point)
{
    return sf::Vector2i (point.x, point.y);
}

dx_point from_sfml_vector (const sf::Vector2u& point)
{
    return dx_point (point.x, point.y);
}

sf::Color sfml_color (const dx_colour& colour)
{
    return sf::Color (colour.red, colour.green, colour.blue, colour.alpha);
}


std::queue <Event> SFML_Engine::event_queue;
dx_point SFML_Engine::window_size = dx_point (400, 400);
    



void SFML_Engine::set_window_size (dx_point _window_size_)
{
        if (_window_size_.x < 0 || _window_size_.y < 0)
        throw std::runtime_error ("Error! It is impossible to create a window with negative coordinates!");
    
    window_size = _window_size_;
}

dx_point SFML_Engine::get_window_size (void) noexcept
{
    return window_size;
}

void SFML_Engine::set_width (float new_width) noexcept
{
    feather_width = new_width;
    // glLineWidth (new_width);
}



float SFML_Engine :: get_width (void) const noexcept
{
    return feather_width;
}




void SFML_Engine::draw_quadrangle (dx_point left_up, dx_point right_up, 
    dx_point right_down, dx_point left_down, dx_colour colour, bool flush) const noexcept
{
    sf::ConvexShape object (4);
    object.setOutlineThickness (0);
    object.setOutlineColor (sfml_color (colour));

    if (flush)
        object.setFillColor (sfml_color (colour));
    else
        object.setFillColor (sf::Color::Transparent);

    object.setPoint (0, sfml_vector (left_up));
    object.setPoint (1, sfml_vector (right_up));
    object.setPoint (2, sfml_vector (right_down));
    object.setPoint (3, sfml_vector (left_down));

    if (window)
        window->draw (object);
}




void SFML_Engine::draw_rect (dx_point left_up, dx_point right_down, 
    dx_colour colour, bool flush) const noexcept
{
    draw_quadrangle (left_up, dx_point (right_down.x, left_up.y), right_down, dx_point (left_up.x, right_down.y), colour, flush);
}



void SFML_Engine::draw_rect (dx_point left_up, double width, double height,
    dx_colour colour, bool flush) const noexcept 
{
    draw_quadrangle (left_up, dx_point (left_up.x + width, left_up.y), dx_point (left_up.x + width, left_up.y + height), dx_point (left_up.x, left_up.y + height), colour, flush);
}

void sort_points (dx_point& point1, dx_point& point2)
{
    if (point1.x > point2.x)
        std::swap (point1, point2);
    else if (point1.x == point2.x)
    {
        if (point1.y > point2.y)
            std::swap (point1, point2);
    } 
}

void SFML_Engine::draw_line (dx_point point1, dx_point point2,
    dx_colour colour, float width) const noexcept
{
    sort_points (point1, point2);

    double delta_x = point2.x - point1.x;
    double delta_y = point2.y - point1.y;

    if (width <= 0)
        width = feather_width;

    sf::RectangleShape line (sf::Vector2f (hypot (delta_x, delta_y), width));

    line.setPosition (point1.x, point1.y);
    line.setFillColor (sfml_color (colour));

    line.rotate (radian_to_grad (atan (delta_y / delta_x)));

    if (window)
        window->draw (line);
}

void SFML_Engine::draw_arrow_line (dx_point point1, dx_point point2,
    dx_colour colour) const noexcept
{
    const double delta_x = point2.x - point1.x;
    const double delta_y = point2.y - point1.y;

    const double line_length = sqrt (delta_x * delta_x + delta_y * delta_y);
    const double divide = 1.0 / 20;
    const double cross_length = divide * line_length;
    const double side_length = 2 * cross_length / sqrt (3);

    const double precision = 0.00000001;
    double tangens = 0;
    
    double vec_y = 0;
    double vec_x = 0;

    if (abs (delta_x) < precision)
    {
        vec_x = side_length;
        vec_y = 0;
    }
    else
    {
        tangens = -delta_y / delta_x;
        vec_y = side_length / sqrt (tangens * tangens + 1);
        vec_x = tangens * vec_y;
    }


    const dx_point centre (point2.x - divide * delta_x, point2.y - divide * delta_y);


    sf::ConvexShape triangle (3);
    triangle.setFillColor (sfml_color (colour));
    
    triangle.setPoint (0, sf::Vector2f (centre.x - 0.5 * vec_x, centre.y - 0.5 * vec_y));
    triangle.setPoint (1, sf::Vector2f (centre.x + 0.5 * vec_x, centre.y + 0.5 * vec_y));
    triangle.setPoint (2, sfml_vector (point2));

    if (window)
        window->draw (triangle);


    sf::ConvexShape line (2);
    line.setOutlineThickness (feather_width);
    line.setOutlineColor (sfml_color (colour));
    
    line.setPoint (0, sfml_vector (point1));
    line.setPoint (1, sfml_vector (point2));

    if (window)
        window->draw (line);
}



void SFML_Engine::draw_graphic (dx_point left_up, dx_point right_down, const dx_vector <dx_point>& points) const noexcept
{
    if (left_up.x > right_down.x)
        std::swap (left_up,right_down);

    float width = right_down.x - left_up.x;
    float height = left_up.y - right_down.y;

    const float near = 0.05;

    const float absciss_length = (1 - 2 * near) * width;
    const float ordynat_length = (1 - 2 * near) * height;

    draw_rect (left_up, right_down, dx_colour (255, 255, 255), true);
    draw_arrow_line (dx_point (left_up.x + near * width, right_down.y + near * height), dx_point (left_up.x + near * width, left_up.y - near * height), dx_colour (0, 0, 0));
    draw_arrow_line (dx_point (left_up.x + near * width, right_down.y + near * height), dx_point (right_down.x - near * width, right_down.y + near * height), dx_colour (0, 0, 0));

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

    dx_point start = dx_point (start_x, start_y);
    dx_point min   = dx_point (min_x, min_y);

    sf::VertexArray graphic (sf::LinesStrip, points.size() - 1);
    for (size_t i = 0; i < points.size(); ++i)
    {
        graphic [0].color = sfml_color (colour::BLACK);
        graphic [0].position = sf::Vector2f ((points[i].x - min.x) * relation_x + start.x, (points[i].y - min.y) * relation_y + start.y);
    }
}

dx_key convert_button (sf::Keyboard::Key key)
{
    return (dx_key)((int)(key));
}


static void keyboard_callback (dx_key key, int action)
{
    Event event;

    if (action == BUTTON_STATE::PRESSED)
    {
        event.tag = Event::KEY_PRESSED;
        event.object_key_pressed = key_pressed_event (key);
    }
    else
    {
        event.tag = Event::KEY_RELEASED;
        event.object_key_released = key_released_event (key);
    }

    SFML_Engine::event_queue.push (event);
}



void scroll_callback (sf::Event::MouseWheelScrollEvent sf_event)
{
    Event event;
    event.tag = Event::SCROLL;

    event.object_scroll = scroll_event (sf_event.delta, dx_point (sf_event.x, sf_event.y));

    SFML_Engine::event_queue.push (event);
}



static void mouse_move_callback (sf::Event::MouseMoveEvent sf_event) noexcept
{
    Event event;
    event.tag = Event::MOUSE_MOVE;
    event.object_mouse_move = mouse_move_event (sf_event.x, sf_event.y);

    SFML_Engine::event_queue.push (event);
}


static void mouse_button_callback (sf::Event::MouseButtonEvent sf_event, BUTTON_STATE state)
{
    Event event;

    MOUSE_BUTTON mouse_button = MOUSE_BUTTON::LEFT;
    
    if (sf_event.button == sf::Mouse::Button::Left)
        mouse_button = MOUSE_BUTTON::LEFT;
    else if (sf_event.button == sf::Mouse::Button::Middle)
        mouse_button = MOUSE_BUTTON::MIDDLE;
    else if (sf_event.button == sf::Mouse::Button::Right)
        mouse_button = MOUSE_BUTTON::RIGHT;

    
    if (state == BUTTON_STATE::PRESSED)
    {
        event.tag = Event::MOUSE_PRESSED;
        event.object_mouse_pressed.mouse_num = mouse_button;
        event.object_mouse_pressed.mouse_pos = dx_point (sf_event.x, sf_event.y);
    }
    else
    {
        event.tag = Event::MOUSE_RELEASED;
        event.object_mouse_released.mouse_num = mouse_button;
        event.object_mouse_released.mouse_pos = dx_point (sf_event.x, sf_event.y);
    }

    SFML_Engine::event_queue.push (event);
}


void SFML_Engine::poll_events (void) noexcept
{
    sf::Event event;

    while (window->pollEvent (event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                keyboard_callback (convert_button (event.key.code), BUTTON_STATE::PRESSED);
            break;

            case sf::Event::KeyReleased:
                keyboard_callback (convert_button (event.key.code), BUTTON_STATE::RELEASED);
            break;

            case sf::Event::MouseButtonPressed:
                mouse_button_callback (event.mouseButton, BUTTON_STATE::PRESSED);
            break;

            case sf::Event::MouseButtonReleased:
                mouse_button_callback (event.mouseButton, BUTTON_STATE::RELEASED);
            break;

            case sf::Event::MouseMoved:
                mouse_move_callback (event.mouseMove);
            break;

            case sf::Event::MouseWheelScrolled:
                scroll_callback (event.mouseWheelScroll);
            break;            
            
            case sf::Event::Closed:
                close_window ();
            break;

            default:

            break;
        }
    }
}




dx_point SFML_Engine::get_mouse_pos (void) noexcept
{
    sf::Vector2i pos = sf::Mouse::getPosition();

    return dx_point (pos.x, pos.y);
}




void SFML_Engine::create_window_fullscreen (const char* file_name)
{
    this->window = new sf::RenderWindow (sf::VideoMode(800, 600), file_name, sf::Style::Fullscreen);
}


void SFML_Engine::create_window (const char* file_name)
{
    this->window = new sf::RenderWindow (sf::VideoMode(window_size.x, window_size.y), file_name);
}


void SFML_Engine::close_window (void) noexcept
{
    window_should_be_closed = 1;
}

bool SFML_Engine::is_opened (void) const noexcept
{
    return !window_should_be_closed;
}

void SFML_Engine::set_window_colour (dx_colour colour) noexcept
{
    clear_colour = colour;
}

void SFML_Engine::terminate (void) const noexcept
{
    window->close();
}

void SFML_Engine::update_while_running (void) noexcept
{
    poll_events();
    window->display();
    window->clear (sfml_color (clear_colour));
}

int SFML_Engine::event_queue_size (void) const noexcept
{
    return SFML_Engine::event_queue.size();
}


Event SFML_Engine::get_event (void) noexcept
{
    if (SFML_Engine::event_queue.empty())
        return Event();

    Event new_event = SFML_Engine::event_queue.front();
    SFML_Engine::event_queue.pop();
    
    return new_event;
}

///     Image handling

uint8_t* SFML_Engine::load_from_file (const char* file_name, dx_point& image_size)
{    
    sf::Image image;
    if (!image.loadFromFile (file_name))
        throw std::runtime_error ("Error! Cannot load the file \"" + std::string (file_name) + "\"!\n");
    
    image_size = from_sfml_vector (image.getSize());

    const uint64_t file_size = int (image_size.x) * int (image_size.y) * 4;
    uint8_t* storage = new uint8_t [file_size];
    memcpy (storage, image.getPixelsPtr(), file_size);

    return storage;
}

void SFML_Engine::draw_image (const uint8_t* pixels, dx_point real_size, dx_rectangle where_to, dx_rectangle where_from, bool scale, dx_colour new_colour) const noexcept
{
    sf::Texture texture;
    texture.create (real_size.x, real_size.y);
    texture.update (pixels, real_size.x, real_size.y, 0, 0);

    sf::Sprite sprite (texture);
    sprite.setPosition (sfml_vector (where_to.left_up));

    sprite.setTextureRect (sf::IntRect (sfml_vector_i (where_from.left_up), sfml_vector_i (where_from.right_down - where_from.left_up)));

    sprite.setColor (sfml_color (new_colour));

    if (scale)
    {
        dx_point delta_to   = where_to.right_down - where_to.left_up;
        dx_point delta_from = where_from.right_down - where_from.left_up;
        sprite.setScale (delta_to.x / delta_from.x, delta_to.y / delta_from.y);
    }
    // printf ("%g %g\n", where_from.right_down.x, where_from.right_down.y);

    if (window)
        window->draw (sprite);
}

void SFML_Engine::set_icon (const char* icon_name) noexcept
{
    sf::Image icon;
    icon.loadFromFile (icon_name);
    window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void SFML_Engine::load_to_file (const char* file_name, const uint8_t* pixels) noexcept
{
    
}