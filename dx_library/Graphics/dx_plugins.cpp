#include "dx_plugins.hpp"
#include "dx_painter.hpp"

dx_plugin::dx_plugin (dx_tool_manager* tool_manager, const char* plugin_name) : dx_abstract_tool (tool_manager)
{
    handle = dlopen (plugin_name, RTLD_NOW);
    if (nullptr == handle) {
        printf("%s\n", dlerror());
    }

    PluginAPI::Plugin* (*get_plugin)() = 
        reinterpret_cast<PluginAPI::Plugin* (*)()>(dlsym(handle, "get_plugin"));

    if (nullptr == get_plugin) {
        printf("%s\n", dlerror());
        dlclose (handle);
    }

    plugin_ptr = get_plugin();
    plugin_ptr->init();

}

void dx_plugin::perform (dx_point mouse) noexcept
{
    dx_image* image_ptr = tool_manager->painter->canvas->image;
    PluginAPI::Canvas canvas;
    canvas.pixels = image_ptr->pixels;
    canvas.width  = image_ptr->image_size.x;
    canvas.height = image_ptr->image_size.y;

    PluginAPI::Position position;
    position.x = mouse.x;
    position.y = mouse.y;
    
    plugin_ptr->properties [PluginAPI::TYPE::THICKNESS].int_value = tool_manager->painter->width;
    plugin_ptr->properties [PluginAPI::TYPE::PRIMARY_COLOR].int_value = *((int*)(&tool_manager->painter->colour));
    plugin_ptr->start_apply (canvas, position);
}

dx_plugin::~dx_plugin ()
{
    plugin_ptr->deinit();
    dlclose (handle);
}

