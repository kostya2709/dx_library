#define _USE_MATH_DEFINES

#include <cstdint>
#include <vector>
#include <cassert>
#include <cmath>

#include "../../../../api/api.hpp"

namespace PluginAPI {
namespace TYPE {

constexpr Type SIGMA = Type(COUNT);
constexpr Type RADIUS = Type(COUNT + 1);
constexpr Type BORDER = Type(COUNT + 2);

};  // namespace TYPE
};  // namespace PluginAPI

namespace GaussianBlurPlugin {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

const int32_t MAX_BLUR_RADIUS = 40;
const double MAX_SIGMA = 2;
};  // namespace GaussianBlurPlugin


struct Matrix {
    GaussianBlurPlugin::Color* color_array;
    int32_t width;
    int32_t height;

    GaussianBlurPlugin::Color get_pixel(int32_t x, int32_t y, bool dup_border); 
    void set_pixel(GaussianBlurPlugin::Color color, int32_t x, int32_t y); 
};

struct Kernel {
   int32_t size = 0; 
    
   std::vector<float> weights;

   Kernel(float sigma, int32_t radius);
};

class GaussianBlur : public PluginAPI::Plugin {
   public:
    bool init() override;
    bool deinit() override;

    void apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void start_apply(PluginAPI::Canvas canvas,
                     PluginAPI::Position pos) override;
    void stop_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
};

