# Organization of the Core Module {#core_organization}

There are a small set of headers that will bring all the core module functionalities that need to be exposed to the user:

* `#include<vclib/base.h>`: includes all the basic types and data structures used by the library. It is generally not required to include it since almost all the other library files include this header;
* `#include<vclib/miscellaneous.h>`: includes a list of miscellaneous classes, functions or utility structures (e.g. comparators, loggers, timers, ...);
* `#include<vclib/space.h>`: includes all the spatial (but also non-spatial) data structures of the library, with the exception of meshes. These data structures are used by the library algorithms but they are meant to be used also by the user. Examples of data structures included by this header are Points (2D, 3D, N-Dimensional), Boxes, Triangles and Polygons, Planes, Spheres, Segments, but also complex structures like spatial regular Grids, KD-Trees, Graphs;
* `#include<vclib/meshes.h>`: includes a set of predefined Mesh data structures, such as Triangle Meshes, Polygonal Meshes, Point Clouds, [Doubly-Connected Edge List](https://en.wikipedia.org/wiki/Doubly_connected_edge_list) Meshes. Along with the predefined meshes, all the definitions that allow to create a custom Mesh data structure are included using this header;
* `#include<vclib/io.h>`: includes all the functions to load or save meshes (or other data structures) into the formats supported by the library;
* `#include<vclib/algorithms.h>`: includes all the algorithms implemented by the library. Algorithms in VCLib are simple functions that take arguments constrained by concepts. Most of the algorithms work with Meshes, but also some other utility algorithms that work with other entities are included by this header (e.g. distance or intersection functions);
