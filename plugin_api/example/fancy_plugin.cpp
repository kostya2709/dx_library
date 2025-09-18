/*============================================================================*/

/*
 * $ g++ -c -fPIC fancy_plugin.cpp -o fancy_plugin.o
 * $ gcc -shared fancy_plugin.o -o fancy_plugin.so
 */

#include <iostream>
#include "fancy_plugin.hpp"

/*============================================================================*/

bool MyFancyPlugin::init() {
    std::cout << "I am plugin and I am ready to use\n";
    return true;
}

bool MyFancyPlugin::deinit() {
    std::cout << "Bye-bye! (plugin deinitialized)\n";
    return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void MyFancyPlugin::start_apply(PluginAPI::Canvas canvas, 
                                PluginAPI::Position pos) {
    std::cout << "Plugin application started\n";
}

void MyFancyPlugin::stop_apply(PluginAPI::Canvas canvas, 
                               PluginAPI::Position pos) {
    std::cout << "Plugin application finished\n";
}

void MyFancyPlugin::apply(PluginAPI::Canvas canvas, 
                          PluginAPI::Position pos) {
    std::cout << "Applying...\n";
}

/*============================================================================*/

extern "C" PluginAPI::Plugin* get_plugin() {
    return PluginAPI::get_instance<MyFancyPlugin>();
}

/*============================================================================*/
