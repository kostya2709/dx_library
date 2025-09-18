#include "../include/MakeTransparent.hpp"

bool MakeTransparent::init() {
    properties[PluginAPI::TYPE::THICKNESS].display_type = 
        PluginAPI::Property::DISPLAY_TYPE::SLIDER;
    return true;
}

bool MakeTransparent::deinit() {
    return true;
}

void MakeTransparent::start_apply (PluginAPI::Canvas canvas, PluginAPI::Position pos) 
{
    
    int width = canvas.width;
    int height = canvas.height;
    
    MakeTransparentPlugin::colour* transparent_colour = 
        reinterpret_cast <MakeTransparentPlugin::colour*> (&properties[PluginAPI::TYPE::PRIMARY_COLOR].int_value);

    for (int64_t i = 0; i < height * width * sizeof (uint32_t); i += 4) 
    {
        MakeTransparentPlugin::colour* current_pixel = reinterpret_cast <MakeTransparentPlugin::colour*>(canvas.pixels + i);

        if ((current_pixel->red == transparent_colour->red) && (current_pixel->green == transparent_colour->green)
            && (transparent_colour->blue == transparent_colour->blue))

        current_pixel->alpha = 0;
    }
}


void MakeTransparent::apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) {}
void MakeTransparent::stop_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) {}

extern "C" PluginAPI::Plugin* get_plugin() {
    return PluginAPI::get_instance<MakeTransparent>();
}
