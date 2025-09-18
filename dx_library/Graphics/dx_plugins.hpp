#ifndef DX_PLUGINS
#define DX_PLUGINS


#include "dx_tools.hpp"
#include <dlfcn.h>
#include "../../plugin_api/api/api.hpp"

class dx_plugin : public dx_abstract_tool
{
    
    PluginAPI::Plugin* plugin_ptr = NULL;
    void* handle = NULL;

public:
    dx_plugin (dx_tool_manager* tool_manager, const char* plugin_name);
    void perform (dx_point position) noexcept override;
    ~dx_plugin ();
    
};

#endif                      // #ifndef DX_PLUGINS