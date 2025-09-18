#include <GLFW/glfw3.h>
#include "dx_open_gl_engine.hpp"
#include <math.h>
#include <stdexcept>

std::queue <Event> OpenGL_Engine::event_queue;


dx_point OpenGL_Engine::from_standart_to_opengl_coordinates (const dx_point& point) const noexcept
{
    return dx_point (point.x / (window_size.x / 2) - 1, -point.y / (window_size.y / 2) + 1);
}

void OpenGL_Engine::set_window_size (dx_point _window_size_)
{
        if (_window_size_.x < 0 || _window_size_.y < 0)
        throw std::runtime_error ("Error! It is impossible to create a window with negative coordinates!");
    
    window_size = _window_size_;
}

dx_point OpenGL_Engine::get_window_size (void) noexcept
{
    return window_size;
}

void OpenGL_Engine::set_width (float new_width) noexcept
{
    feather_width = new_width;
    glLineWidth (new_width);
}



float OpenGL_Engine :: get_width (void) const noexcept
{
    return feather_width;
}




void OpenGL_Engine::draw_quadrangle (dx_point left_up, dx_point right_up, 
    dx_point right_down, dx_point left_down, dx_colour colour, bool flush) const noexcept
{
    left_up    = from_standart_to_opengl_coordinates (left_up);
    right_up   = from_standart_to_opengl_coordinates (right_up);
    right_down = from_standart_to_opengl_coordinates (right_down);
    left_down  = from_standart_to_opengl_coordinates (left_down);

    unsigned int shape = 0;

    if (flush)
        shape = GL_QUADS;
    else
        shape = GL_LINE_LOOP;

    glBegin (shape);
    glColor4f ((GLfloat)colour.red / 255, (GLfloat)colour.green / 255, (GLfloat)colour.blue / 255, (GLfloat)colour.alpha / 255);
    glVertex2d (left_up.x, left_up.y);
    glVertex2d (right_up.x, right_up.y);
    glVertex2d (right_down.x, right_down.y);
    glVertex2d (left_down.x, left_down.y);
    glEnd();
}




void OpenGL_Engine::draw_rect (dx_point left_up, dx_point right_down, 
    dx_colour colour, bool flush) const noexcept
{
    draw_quadrangle (left_up, dx_point (right_down.x, left_up.y), right_down, dx_point (left_up.x, right_down.y), colour, flush);
}



void OpenGL_Engine::draw_rect (dx_point left_up, double width, double height,
    dx_colour colour, bool flush) const noexcept 
{
    draw_quadrangle (left_up, dx_point (left_up.x + width, left_up.y), dx_point (left_up.x + width, left_up.y + height), dx_point (left_up.x, left_up.y + height), colour, flush);
}



void OpenGL_Engine::draw_line (dx_point point1, dx_point point2,
    dx_colour colour, float width) const noexcept
{

    point1 = from_standart_to_opengl_coordinates (point1);
    point2 = from_standart_to_opengl_coordinates (point2);

    float old_width = get_width();
    glLineWidth (width);

    glBegin (GL_LINES);
        glColor4f ((GLfloat)colour.red / 255, (GLfloat)colour.green / 255, (GLfloat)colour.blue / 255, (GLfloat)colour.alpha / 255);
        glVertex2d (point1.x, point1.y);
        glVertex2d (point2.x, point2.y);
    glEnd();

    glLineWidth (old_width);
}

void OpenGL_Engine::draw_arrow_line (dx_point point1, dx_point point2,
    dx_colour colour) const noexcept
{
    point1 = from_standart_to_opengl_coordinates (point1);
    point2 = from_standart_to_opengl_coordinates (point2);

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

    glBegin (GL_TRIANGLES);
        glColor4f ((GLfloat)colour.red / 255, (GLfloat)colour.green / 255, (GLfloat)colour.blue / 255, (GLfloat)colour.alpha / 255);
        glVertex2d (centre.x - 0.5 * vec_x, centre.y - 0.5 * vec_y);
        glVertex2d (centre.x + 0.5 * vec_x, centre.y + 0.5 * vec_y);
        glVertex2d (point2.x, point2.y);
    glEnd();
    

    glBegin (GL_LINES);
        glColor4f ((GLfloat)colour.red / 255, (GLfloat)colour.green / 255, (GLfloat)colour.blue / 255, (GLfloat)colour.alpha / 255);
        glVertex2d (point1.x, point1.y);
        glVertex2d (point2.x, point2.y);
    glEnd();
}



void OpenGL_Engine::draw_graphic (dx_point left_up, dx_point right_down, const dx_vector <dx_point>& points) const noexcept
{
    GLfloat current_line_width = get_width();
    glLineWidth (2);

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

    dx_point start = from_standart_to_opengl_coordinates (dx_point (start_x, start_y));
    dx_point min   = from_standart_to_opengl_coordinates (dx_point (min_x, min_y));

    glBegin (GL_LINE_STRIP);
        for (size_t i = 0; i < points.size(); ++i)
        {
            dx_point the_point = from_standart_to_opengl_coordinates (points[i]);
            glVertex2d ((the_point.x - min.x) * relation_x + start.x, (the_point.y - min.y) * relation_y + start.y);
        }
    glEnd();

    glLineWidth (current_line_width);

}


dx_key convert_button (int key)
{
    return (dx_key)((int)(key));                //// One should add all the buttons.
}


static void keyboard_callback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Event event;

    if (action == GLFW_PRESS)
    {
        event.tag = Event::KEY_PRESSED;
        event.object_key_pressed = key_pressed_event (convert_button (key));
    }
    else
    {
        event.tag = Event::KEY_RELEASED;
        event.object_key_released = key_released_event (key);
    }

    OpenGL_Engine::event_queue.push (event);
}



void scroll_callback (GLFWwindow* window, double offset_x, double offset_y)
{
    Event event;
    event.tag = Event::SCROLL;

    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos (window, &mouse_x, &mouse_y);

    event.object_scroll = scroll_event (offset_y, dx_point (mouse_x, mouse_y));

    OpenGL_Engine::event_queue.push (event);
}



static void mouse_move_callback (GLFWwindow* window, double mouse_x, double mouse_y) noexcept
{
    Event event;
    event.tag = Event::MOUSE_MOVE;
    event.object_mouse_move = mouse_move_event (mouse_x, mouse_y);

    OpenGL_Engine::event_queue.push (event);
}


static void mouse_button_callback (GLFWwindow* window, int button, int action, int mods)
{
    Event event;

    MOUSE_BUTTON mouse_button = MOUSE_BUTTON::LEFT;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        mouse_button = MOUSE_BUTTON::LEFT;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        mouse_button = MOUSE_BUTTON::MIDDLE;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        mouse_button = MOUSE_BUTTON::RIGHT;

    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos (window, &mouse_x, &mouse_y);

    
    if (action == GLFW_PRESS)
    {
        event.tag = Event::MOUSE_PRESSED;
        event.object_mouse_pressed.mouse_num = mouse_button;
        event.object_mouse_pressed.mouse_pos = dx_point (mouse_x, mouse_y);
    }
    else
    {
        event.tag = Event::MOUSE_RELEASED;
        event.object_mouse_released.mouse_num = mouse_button;
        event.object_mouse_released.mouse_pos = dx_point (mouse_x, mouse_y);
    }

    OpenGL_Engine::event_queue.push (event);
}


dx_point OpenGL_Engine::get_mouse_pos (void) noexcept
{
    double mouse_x = 0;
    double mouse_y = 0;

    glfwGetCursorPos (window, &mouse_x, &mouse_y);

    return dx_point (mouse_x, mouse_y);
}

static void window_initializer (GLFWwindow* window)
{
    
    glfwMakeContextCurrent (window);

    glfwSetKeyCallback          (window, keyboard_callback);
    glfwSetScrollCallback       (window, scroll_callback);
    glfwSetCursorPosCallback    (window, mouse_move_callback);
    glfwSetMouseButtonCallback  (window, mouse_button_callback);

    glClear (GL_COLOR_BUFFER_BIT);
    
/*
    dx_vector <std::pair <double, double>> points (10);
    for (int i = 0; i < 10; ++i)
    {
        points[i].first = i;
        points[i].second = i * i;
    }

    dx_colour red();

    dx_line line (dx_point (0, 0), dx_point (1, 1), dx_colour (255, 0, 0), 5);
    main_array.push_back (&line);

    dx_abstract_field button (dx_point (0, 0), dx_point (-1, -1), colour::AQUA, 1);
    button.set_colour (colour::RED);
    main_array.push_back (&button);

    int size = 100;
    dx_vector <dx_point> assign (size);
    dx_vector <dx_point> compare (size);

    for (int i = 0; i < size; ++i)
    {
        assign [i].x = 10 * i;
        compare [i].x = 10 * i;
    }

    calculations (assign, compare);

    printf ("x %g; y %g\n", compare [size - 1].x, compare [size - 1].y);
*/ 

/*
    while (!glfwWindowShouldClose (window))
    {

        line.draw();
        button.draw();
        Engine_t::method.draw_graphic (dx_point (0.9, 0.1), dx_point (0.1, 0.9), assign);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear (GL_COLOR_BUFFER_BIT);
    }

    glfwDestroyWindow (window);
    glfwTerminate();

    return;

*/
}

void OpenGL_Engine::create_window_fullscreen (const char* file_name)
{
    GLFWwindow* window;
    
    if (!glfwInit())
        throw std::runtime_error ("FATAL ERROR! Cannot initialize glfw!\n");

    const GLFWvidmode* mode = glfwGetVideoMode (glfwGetPrimaryMonitor());

    window = glfwCreateWindow (mode->width, mode->height, file_name, NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error ("FATAL ERROR! Cannot create a glfw-Window in fullscreen mode!\n");
    }

    this->window = window;
    window_initializer (window);
}


void OpenGL_Engine::create_window (const char* file_name)
{
    GLFWwindow* window;
    
    if (!glfwInit())
        throw std::runtime_error ("FATAL ERROR! Cannot initialize glfw!\n");

    const GLFWvidmode* mode = glfwGetVideoMode (glfwGetPrimaryMonitor());
    const int x_pos = (mode->width - window_size.x) / 2;
    const int y_pos = (mode->height - window_size.y) / 2;

    window = glfwCreateWindow (window_size.x, window_size.y, file_name, NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error ("FATAL ERROR! Cannot create a glfw-Window!\n");
    }

    glfwSetWindowPos (window, x_pos, y_pos);

    this->window = window;
    window_initializer (window);
}


void OpenGL_Engine::close_window (void) noexcept
{
    glfwSetWindowShouldClose (window, true);
}

bool OpenGL_Engine::is_opened (void) const noexcept
{
    return !glfwWindowShouldClose (window);
}

void OpenGL_Engine::set_window_colour (dx_colour colour) noexcept
{
    clear_colour = colour;
    glClearColor (colour.red, colour.green, colour.blue, colour.alpha);
}

void OpenGL_Engine::terminate (void) const noexcept
{
    glfwDestroyWindow (window);
    glfwTerminate();
}

void OpenGL_Engine::update_while_running (void) noexcept
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClear (GL_COLOR_BUFFER_BIT);
}

int OpenGL_Engine::event_queue_size (void) const noexcept
{
    return OpenGL_Engine::event_queue.size();
}

Event OpenGL_Engine::get_event (void) noexcept
{
    if (OpenGL_Engine::event_queue.empty())
        return Event();

    Event new_event = OpenGL_Engine::event_queue.front();
    OpenGL_Engine::event_queue.pop();
    
    return new_event;
}