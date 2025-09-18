#include "../include/BoxBlur.hpp"

bool BoxBlur::init() {
    properties[PluginAPI::TYPE::THICKNESS].display_type = 
        PluginAPI::Property::DISPLAY_TYPE::SLIDER;
    return true;
}

bool BoxBlur::deinit() {
    return true;
}

void BoxBlur::start_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) {
    int width = canvas.width;
    int height = canvas.height;
    int radius = properties[PluginAPI::TYPE::THICKNESS].int_value / 2;

    auto canvasPixels = reinterpret_cast<uint32_t*> (canvas.pixels);
    uint32_t* oldPixels = new uint32_t[height * width];

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            oldPixels[y * width + x] =  canvasPixels[y * width + x];
        }
    }

    for (int pos = 0; pos < height * width; ++pos) {

        uint32_t colorSum[4] = {};

        for(int y = pos / width - radius; y <= pos / width + radius; ++y) {
            for (int x = pos % width - radius; x <= pos % width + radius; ++x) {
                int colorX;
                int colorY;

                if (y < 0)
                    colorY = 0;
                else if (y >= height)
                    colorY = height - 1;
                else 
                    colorY = y;

                if (x < 0)
                    colorX = 0;
                else if (x >= width)
                    colorX = width - 1;
                else 
                    colorX = x;
                
                uint32_t color = oldPixels[colorY * width + colorX];

                colorSum[3] += (color >> 24) & 0xff;
                colorSum[2] += (color >> 16) & 0xff;
                colorSum[1] += (color >>  8) & 0xff;
                colorSum[0] +=  color        & 0xff;
            }
        }

        for (auto& sum: colorSum) {
            sum /= (radius * 2 + 1) * (radius * 2 + 1);
        }

        for (int i = 0; i < 4; ++i) {
            canvas.pixels[pos * 4 + i] = static_cast<uint8_t> (colorSum[i]);
        }
    }

    delete[] oldPixels;
}


void BoxBlur::apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) {}
void BoxBlur::stop_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) {}

extern "C" PluginAPI::Plugin* get_plugin() {
    return PluginAPI::get_instance<BoxBlur>();
}
