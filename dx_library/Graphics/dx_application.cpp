
#include "dx_application.hpp"
#include "../Engine/dx_using_engine.hpp"

char* dx_application::window_name = const_cast <char*> ("Window");
dx_object_container dx_application::windows;
bool dx_application::is_maximized = false;
dx_mouse_info dx_application::mouse;


void dx_application::add (dx_abstract_object& new_object) noexcept
{
    dx_application::windows.push_back (new_object);
}

void dx_application::remove (dx_abstract_object& expired_object) noexcept
{
    dx_application::windows.remove (expired_object);
}

void dx_application::set_window_size (int _window_width_, int _window_height_)
{    
    is_maximized = false;
    Engine_t::method.set_window_size (dx_point (_window_width_, _window_height_));
}

void dx_application::set_window_name (char* _window_name_)
{
    if (_window_name_ == NULL)
        throw std::runtime_error ("Error! Invalid pointer to the name of the window!");

    window_name = _window_name_;
}

void dx_application::maximize_window (void) noexcept
{
    dx_application::is_maximized = true;
}

void dx_application::set_window_colour (dx_colour _colour_)
{
    Engine_t::method.set_window_colour (_colour_);
    windows.set_colour (_colour_);
    windows.flush = true;
}

void dx_application::run (void)
{
    windows.set_coordinates (dx_point (0, 0), Engine_t::method.window_size);
    windows.flush = 1;

    if (dx_application::is_maximized)
        Engine_t::method.create_window_fullscreen (window_name);
    else
        Engine_t::method.create_window (window_name);

    mouse.current_position = Engine_t::method.get_mouse_pos();
    Engine_t::method.set_icon ("../images/system/dragon_x.png");

    while (Engine_t::method.is_opened())
    {
        windows.draw();
        
        Engine_t::method.update_while_running();
        handle_events ();
        
        //Engine_t::method.close_window();
    }

    Engine_t::method.terminate();
}


void dx_application::handle_events (void) noexcept
{
    if (mouse.is_pressed && mouse.clicked_object->handle_pressed)                           /// If object should do something while it is pressed
    {
        if (!mouse.clicked_object->contains_point (mouse.current_position))
            ;
        else
            mouse.clicked_object->mouse_down (mouse.current_position, MOUSE_BUTTON::LEFT);
    }

    while (Engine_t::method.event_queue_size())
    {
        Event event = Engine_t::method.get_event();

        switch (event.tag)
        {
            case Event::MOUSE_MOVE:
            
                mouse.current_position = event.object_mouse_move.mouse_pos;
                dx_application::handle_mouse_move();

            break;

            case Event::MOUSE_RELEASED:
                    if (mouse.clicked_object->contains_point (mouse.current_position))
                        mouse.clicked_object->operate (mouse.current_position);

                    mouse.clicked_object = NULL;
                    mouse.is_pressed = false;
                    mouse.is_clicked_draggable = false;
            break;

            case Event::MOUSE_PRESSED:

                if (dx_abstract_object* found = windows.contains_point (event.object_mouse_pressed.mouse_pos))
                {
                    mouse.is_clicked_draggable = found->mouse_down (mouse.current_position, event.object_mouse_pressed.mouse_num);

                    if (found != (dx_abstract_object*)(&windows))
                    {
                        //found->leave_container();
                        //windows.push_back (*found);
                    }

                    mouse.clicked_object = found;
                    mouse.is_pressed = 1;
                }
            break;

            default:

            break;
        }
    }
}


void dx_application::handle_mouse_move (void)
{
    if (mouse.is_clicked_draggable)
    {
        dynamic_cast <dx_draggable_object*> (mouse.clicked_object)->move (mouse.current_position);
        return;
    }
    if (dx_abstract_object* found = windows.contains_point (mouse.current_position))
    {
        if (found != mouse.hovered_object)
        {
            if (mouse.hovered_object)
                mouse.hovered_object->stop_mouse_move();
            
            mouse.hovered_object = found;
            found->start_mouse_move (mouse.current_position);
        }
        found->mouse_move (mouse.current_position, mouse.is_pressed);
    }
}
