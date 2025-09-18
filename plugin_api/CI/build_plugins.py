#!/bin/python
from glob import glob
import os
import shutil

build_cmd = 'gcc -c -fPIC '

plugins = glob("pending/*")

for plugin in plugins:
    print("Building plugin " + plugin)

    plugin_name = plugin[plugin.rfind('/') + 1:]

    icons = glob(plugin + '/icon*')
    if len(icons) == 0:
        print("No icon provided. Skipping")

    icon_path = icons[0]

    source_file = plugin + "/src/" + plugin_name + ".cpp"
    build_cmd_for_source = build_cmd + source_file
    exit_code = os.system(build_cmd_for_source)

    if exit_code:
        print("Plugin " + plugin + " build failed. Skipping...")
        continue

    print("Built object file for " + plugin)
    print("Building shared library...")
    
    object_file = plugin_name + ".o"
    shared_lib_file = plugin_name + ".so"


    build_cmd_for_shared_lib = 'gcc ' + object_file + ' -shared -o ' + shared_lib_file 
    exit_code = os.system(build_cmd_for_shared_lib)

    if exit_code:
        print("Plugin " + plugin + " shared lib generation failed. Skipping...")
        continue

    print("Deleting " + object_file)
    os.unlink(object_file)
   
    if not os.path.exists("plugins/" + plugin_name):
        os.mkdir("plugins/" + plugin_name)

    shutil.copy2(shared_lib_file, "plugins/" + plugin_name + "/")
    shutil.copy2(icon_path, "plugins/" + plugin_name + "/") 

    os.unlink(shared_lib_file)
    print("Plagin build finished")
    print("=======================\n")
