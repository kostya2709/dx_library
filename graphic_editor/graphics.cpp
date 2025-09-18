// #include "sort_analyzer.hpp"
#include "../dx_library/Graphics/dx_graphics.hpp"
#include "../dx_library/Engine/dx_open_gl_engine.hpp"

using Engine_t = Engine <OpenGL_Engine>;

dx_vector <dx_abstract_object*> main_array;

static void error_callback(int error, const char* description)
{
    fputs (description, stderr);
}

static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose (window, GL_TRUE);
}

static void handle_mouse_move (GLFWwindow* window, double mouse_x, double mouse_y) noexcept
{
    dx_point mouse_point (mouse_x, mouse_y);
    int width = 0;
    int height = 0;
    glfwGetWindowSize (window, &width, &height);
    dx_point new_point = dx_convert_coordinates (mouse_point, width, height);

    for (size_t i = 0; i < main_array.size(); ++i)
        if (main_array [i]->contains_point (dx_point (new_point.x, new_point.y)))
            main_array [i]->mouse_move (mouse_point, 0);
}


void create_window (void)
{
    GLFWwindow* window;
    glfwSetErrorCallback (error_callback);
    
    if (!glfwInit())
        exit (EXIT_FAILURE);

    const int window_width = 1300;
    const int window_height = 1000;

    window = glfwCreateWindow (window_width, window_height, "Analyzer", NULL /*glfwGetPrimaryMonitor()*/, NULL);
    
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent (window);
    glfwSetKeyCallback (window, key_callback);
    glfwSetCursorPosCallback (window, handle_mouse_move);
    
    // point_array <int> points (11);
    // for (int i = 1; i <= 10; ++i)
    //     points.array_size [i] = 10 * i;

    dx_vector <std::pair <double, double>> points (10);
    for (int i = 0; i < 10; ++i)
    {
        points[i].first = i;
        points[i].second = i * i;
    }

    dx_colour red;

    dx_line line (dx_point (0, 0), dx_point (1, 1), dx_colour (255, 0, 0), 5);
    main_array.push_back (&line);

    dx_abstract_field button (dx_point (0, 0), dx_point (-1, -1), colour::AQUA, 1);
    button.set_colour (colour::RED);
    main_array.push_back (&button);

    while (!glfwWindowShouldClose (window))
    {
        // float ratio;
        // int width, height;
        // glfwGetFramebufferSize(window, &width, &height);
        // // ratio = width / (float) height;
        // glViewport(0, 0, width, height);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        line.draw();
        button.draw();
		// Engine_t::method.draw_graphic (dx_point (0.9, 0.1), dx_point (0.1, 0.9), assign);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow (window);
    glfwTerminate();
    exit (EXIT_SUCCESS);
}
