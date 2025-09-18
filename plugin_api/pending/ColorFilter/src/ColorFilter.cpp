#include "../include/ColorFilter/ColorFilter.h"

bool ColorFilter::init() {
    properties[PluginAPI::TYPE::PRIMARY_COLOR].display_type =
        PluginAPI::Property::DISPLAY_TYPE::COLOR_PICKER;
    return true;
}

bool ColorFilter::deinit() { return true; }

void ColorFilter::start_apply(PluginAPI::Canvas canvas,
                              PluginAPI::Position pos) {
    for (int64_t i = 0; i < canvas.height * canvas.width * sizeof(int32_t);
         i += 4) {
        ColorFilterPlugin::Color* current_pixel =
            reinterpret_cast<ColorFilterPlugin::Color*>(canvas.pixels + i);

        ColorFilterPlugin::Color* filter_color =
            reinterpret_cast<ColorFilterPlugin::Color*>(
                &properties[PluginAPI::TYPE::PRIMARY_COLOR].int_value);

        current_pixel->r = std::min(current_pixel->r, filter_color->r);
        current_pixel->g = std::min(current_pixel->g, filter_color->g);
        current_pixel->b = std::min(current_pixel->b, filter_color->b);
        current_pixel->a = filter_color->a;
    }
}

void ColorFilter::stop_apply(PluginAPI::Canvas canvas,
                             PluginAPI::Position pos) {}

void ColorFilter::apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) {}

extern "C" PluginAPI::Plugin* get_plugin() {
    return PluginAPI::get_instance<ColorFilter>();
}
