
#include "../dx_library/dx_paint_application.hpp"

#ifndef MY_TYPE
#include "my_type.hpp"
#define MY_TYPE 1
#endif


template <>
int super_type <int>::assignment_counter = 0;

template <>
int super_type <int>::comparison_counter = 0;

class func
{
public:
    void operator () (void)
    {
        printf ("PRESSED\n");
    }
};


int main()
{
    dx_painter app (dx_point (50, 50), dx_point (1260, 700));
    dx_application::add (app);
    // dx_application::maximize_window();
    dx_application::set_window_size (1280, 720);
    dx_application::set_window_name ((char*)"DX_PAINT");
    dx_application::set_window_colour (colour::GREY_BACKGROUND);
    dx_application::run();


    //create_window();
    
    return 0;
}