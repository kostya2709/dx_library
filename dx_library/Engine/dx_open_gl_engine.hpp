
#ifndef DX_OPEN_GL_ENGINE
#define DX_OPEN_GL_ENGINE

#include <stdio.h>
#include <GLFW/glfw3.h>
#include "dx_abstract_engine.hpp"



class OpenGL_Engine : public Iengine
{
public:

    virtual void create_window_fullscreen (const char* file_name = "Window") override;                                                                                                    

    virtual void create_window (const char* file_name = "Window") override;
    
    virtual void close_window (void) noexcept override;

    virtual bool is_opened (void) const noexcept override;

    virtual void set_window_colour (dx_colour colour) noexcept override;

    virtual void set_window_size (dx_point _window_size_) override;

    virtual dx_point get_window_size (void) noexcept override;

    virtual void terminate (void) const noexcept;

    virtual void update_while_running (void) noexcept override;

    virtual int event_queue_size (void) const noexcept override;

    virtual Event get_event (void) noexcept override;

    virtual void draw_quadrangle (dx_point left_up, dx_point right_up, 
        dx_point right_down, dx_point left_down, dx_colour colour = colour::DEFAULT, bool flush = false) const noexcept override;
    
    virtual void draw_rect (dx_point left_up, dx_point right_down, 
        dx_colour colour = colour::DEFAULT, bool flush = false) const noexcept override;
    
    virtual void draw_rect (dx_point left_up, double width, double height,
        dx_colour colour = colour::DEFAULT, bool flush = false) const noexcept override;

    virtual void draw_line (dx_point point1, dx_point point2,
        dx_colour colour = colour::DEFAULT, float width = 1) const noexcept override;

    virtual void draw_arrow_line (dx_point point1, dx_point point2,
        dx_colour colour = colour::DEFAULT) const noexcept override;

    virtual void draw_graphic (dx_point left_up, dx_point right_down,
        const dx_vector <dx_point>& points) const noexcept override;

    virtual float get_width (void) const noexcept override;

    virtual void set_width (float new_width) noexcept override;

    virtual dx_point get_mouse_pos (void) noexcept override;

///     Image handling

    virtual uint8_t* load_from_file (const char* file_name, dx_point& image_size) override {return NULL;}
    virtual void draw_image (const uint8_t* pixels, dx_point real_size, dx_rectangle where_to, dx_rectangle where_from, bool scale, dx_colour new_colour) const noexcept override {}
    virtual void set_icon (const char* icon_name) noexcept override {}
    virtual void load_to_file (const char* file_name, const uint8_t* pixels) noexcept override {}
////

    dx_point from_standart_to_opengl_coordinates (const dx_point& point) const noexcept;



    GLFWwindow* window = NULL;

    static std::queue <Event> event_queue;

};


#endif                  // #ifndef DX_OPEN_GL_ENGINE

