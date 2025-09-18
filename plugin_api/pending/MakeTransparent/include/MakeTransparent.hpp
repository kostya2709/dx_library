#ifndef MAKE_TRANSPARENT
#define MAKE_TRANSPARENT

#include "../../../api/api.hpp"

namespace MakeTransparentPlugin
{
    class colour
    {
    public:
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        uint8_t alpha = 0;
    };
};

class MakeTransparent: public PluginAPI::Plugin {
public:
    bool init() override;
    bool deinit() override;

    void apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void start_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void stop_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
};

#endif                      // #endif MAKE_TRANSPARENT