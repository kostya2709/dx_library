#ifndef BOXBLUR_PLUGIN_H
#define BOXBLUR_PLUGIN_H

#include "../../../api/api.hpp"

class BoxBlur: public PluginAPI::Plugin {
public:
    bool init() override;
    bool deinit() override;

    void apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void start_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void stop_apply(PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
};

#endif
