#include <cstdint>

#include "../../../../api/api.hpp"

namespace ColorFilterPlugin {
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
};  // namespace ColorFilterPlugin

class ColorFilter : public PluginAPI::Plugin {
   public:
    bool init() override;
    bool deinit() override;

    void apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void start_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void stop_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
};

