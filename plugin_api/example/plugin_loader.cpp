/*============================================================================*/

/*
 * $ g++ -ldl plugin_loader.cpp -o plugin_loader
 */

#include <dlfcn.h>

#include <memory>
#include <cstdio>

#include "../api/api.hpp"

/*============================================================================*/

int main(int argc, const char** argv) {

    if (argc != 2) {
        printf("usage: %s [path to library]\n",
               (argc ? argv[0] : "plugin_loader"));
        return 1;
    }

    void* handle = dlopen(argv[1], RTLD_NOW);
    if (nullptr == handle) {
        printf("%s\n", dlerror()); 
        return 2;
    }

    PluginAPI::Plugin* (*get_plugin)() = 
        reinterpret_cast<PluginAPI::Plugin* (*)()>(dlsym(handle, "get_plugin"));

    if (nullptr == get_plugin) {
        printf("%s\n", dlerror());
        dlclose(handle);
        return 3;
    }

    PluginAPI::Plugin* my_fancy_plugin = get_plugin();

    my_fancy_plugin->init();
    my_fancy_plugin->deinit();

    dlclose(handle);

    return 0;
}

/*============================================================================*/
