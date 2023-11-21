# External Library Support

This folder contains a set of VCLib functions and classes that can be used only if you have the corresponding external library available on your system.

The functionalities are then accessible under a specific namespace for each library.

A list of the supported libraries and their namespaces is provided below:

| Library(s) | Namespace | Notes |
|:-------:|:---------:|:-----:|
| [OpenGL2](https://registry.khronos.org/OpenGL-Refpages/gl2.1/) | `vcl::gl2` | |
| [Qt6](https://www.qt.io/) | `vcl::qt` | |
| [QGLViewer](http://libqglviewer.com/) | `vcl::qgl` | This library requires Qt6 |
| [VCG](http://vcg.isti.cnr.it/vcglib/) | `vcl::vc` | |
