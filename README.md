# DX_Library
This project is a graphic library. It is designed to be highle modular, for example, two underlying engines can be used: SFML or OpenGL (OpenGL is used by default). The library allows to detect all sorts of events: mouse/key press/release, scroll and so on.

# Prerequisites
```
# Install OpenGL
sudo apt install build-essential libgl1-mesa-dev freeglut3-dev libglew-dev libglfw3-dev libglm-dev

# Install SFML
sudo apt-get install libsfml-dev
```
# Graphic Editor
The main product using this library is a MS Paint-like graphical editor. It supports different pen styles, filling, custom plugins, horizontal and vertical scroll bars etc.

## Launch
```
$ cd graphic_editor
$ make
```

## Plugin API
Plugins share common interface, so they can be easily added to the Tools palette. For example, there is a Gaussian Blur plugin. The API is described in details in plugin_api/api/api.hpp.

# Trie
The library also contains an implementation of a trie (or prefix tree) and a tool for visualization - machine_drawer. The tool creates a graphviz file which allows to visualize all trie's nodes (verteces) and connections (edges).