#include "../include/GaussianBlur/GaussianBlur.h"

#include <bits/stdint-uintn.h>

#include <cstdio>

/*~~~~~~~~~~~~~~~~~~~~~~~ Matrix ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Matrix::set_pixel(GaussianBlurPlugin::Color color, int32_t x, int32_t y) {
    color_array[y * width + x] = color;
}

GaussianBlurPlugin::Color Matrix::get_pixel(int32_t x, int32_t y,
                                            bool dup_border) {
    if (!dup_border) {
        GaussianBlurPlugin::Color black = {0, 0, 0, 0};
        if (x >= width || x < 0) return black;
        if (y >= height || y < 0) return black;
    }

    int32_t adjusted_x = x >= 0 ? x : 0;
    adjusted_x = x < width ? x : width - 1;

    int32_t adjusted_y = y >= 0 ? y : 0;
    adjusted_y = y < height ? y : height - 1;

    return color_array[y * width + x];
}

/*~~~~~~~~~~~~~~~~~~~~ Kernel ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Kernel::Kernel(float sigma, int32_t radius) : size(radius) {
    weights.assign((2 * radius - 1) * (2 * radius - 1), 0);

    assert(sigma != 0);
    assert(radius != 0);

    assert(!std::isnan(sigma));

    double sum = 0;

    float fraction = 1.f / static_cast<float>(2 * M_PI * sigma * sigma);

    for (int x = -radius + 1; x < radius; ++x) {
        for (int y = -radius + 1; y < radius; ++y) {
            float power = -static_cast<float>(x * x + y * y) /
                          static_cast<float>(2 * sigma * sigma);

            float value = fraction * pow(M_E, power);

            assert(!std::isnan(value));

            weights[(y + radius - 1) * (2 * radius - 1) + x + radius - 1] =
                value;
            sum += value;
        }
    }

    for (auto& weight : weights) {
        weight /= sum;
        assert(!std::isnan(weight));
    }
}

bool GaussianBlur::init() {
    properties[PluginAPI::TYPE::SIGMA].display_type =
        PluginAPI::Property::DISPLAY_TYPE::SLIDER;
    properties[PluginAPI::TYPE::SIGMA].label = "Sigma";

    properties[PluginAPI::TYPE::RADIUS].display_type =
        PluginAPI::Property::DISPLAY_TYPE::SLIDER;
    properties[PluginAPI::TYPE::RADIUS].label = "Radius";

    properties[PluginAPI::TYPE::BORDER].display_type =
        PluginAPI::Property::DISPLAY_TYPE::CHECKBOX;
    properties[PluginAPI::TYPE::BORDER].label = "Duplicate borders";

    return true;
}

bool GaussianBlur::deinit() { return true; }

void GaussianBlur::start_apply(PluginAPI::Canvas canvas,
                               PluginAPI::Position pos) {
    float sigma = properties[PluginAPI::TYPE::SIGMA].double_value *
                  GaussianBlurPlugin::MAX_SIGMA;
    int32_t radius = properties[PluginAPI::TYPE::RADIUS].double_value *
                     GaussianBlurPlugin::MAX_BLUR_RADIUS;
    bool dup_border = properties[PluginAPI::TYPE::BORDER].int_value;

    auto pixel_array =
        reinterpret_cast<GaussianBlurPlugin::Color*>(canvas.pixels);
    auto output_array = reinterpret_cast<GaussianBlurPlugin::Color*>(
        new uint8_t[4 * canvas.height * canvas.width]());

    Matrix input_canvas = {pixel_array, static_cast<int32_t>(canvas.width),
                           static_cast<int32_t>(canvas.height)};
    Matrix output_canvas = {output_array, static_cast<int32_t>(canvas.width),
                            static_cast<int32_t>(canvas.height)};

    Kernel blur_kernel(sigma, radius);

    for (int32_t x = 0; x < canvas.width; ++x) {
        for (int32_t y = 0; y < canvas.height; ++y) {
            float r = 0;
            float g = 0;
            float b = 0;
            float a = 0;

            for (int32_t i = -radius + 1; i < radius; ++i) {
                for (int32_t j = -radius + 1; j < radius; ++j) {
                    auto pixel =
                        input_canvas.get_pixel(x + i, y + j, dup_border);

                    int weight_idx =
                        (radius - 1 + j) * (2 * radius - 1) + (radius - 1 + i);

                    r += static_cast<float>(pixel.r) *
                         blur_kernel.weights[weight_idx];
                    g += static_cast<float>(pixel.g) *
                         blur_kernel.weights[weight_idx];
                    b += static_cast<float>(pixel.b) *
                         blur_kernel.weights[weight_idx];
                    a += static_cast<float>(pixel.a) *
                         blur_kernel.weights[weight_idx];
                }
            }

            GaussianBlurPlugin::Color blurred_pixel = {
                static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                static_cast<uint8_t>(b), static_cast<uint8_t>(a)};

            output_canvas.set_pixel(blurred_pixel, x, y);
        }
    }

    uint8_t* output_pixels = reinterpret_cast<uint8_t*>(output_array);

    for (int64_t i = 0; i < 4 * canvas.height * canvas.width; ++i) {
        canvas.pixels[i] = output_pixels[i];
    }

    delete[] output_pixels;
}

void GaussianBlur::stop_apply(PluginAPI::Canvas canvas,
                              PluginAPI::Position pos) {}

void GaussianBlur::apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) {}

extern "C" PluginAPI::Plugin* get_plugin() {
    return PluginAPI::get_instance<GaussianBlur>();
}
