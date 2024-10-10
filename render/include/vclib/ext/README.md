# External Libraries

This folder contains a set of VCLib functions and classes that can be used only if you have the corresponding external library available on your system.

The functionalities are then accessible under a specific namespace for each library.

A list of the supported libraries, their paths and their namespaces is provided below:

| Library(s) | Namespace | Path | Notes |
|:----------:|:---------:|:----:|:-----:|
| [GLFW](https://www.glfw.org/) | `vcl::glfw` | `ext/glfw` | |
| [OpenGL 2.1](https://registry.khronos.org/OpenGL-Refpages/gl2.1/) | `vcl::gl2` | `ext/opengl2` | |
| [Qt 6](https://www.qt.io/) | `vcl::qt` | `ext/qt` | |
| [QGLViewer](http://libqglviewer.com/) | `vcl::qgl` | `ext/qglviewer` | This library requires Qt6 |
