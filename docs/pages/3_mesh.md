# VCLib Meshes     {#mesh_documentation_page}

## Prebuilt Mesh data structures

VCLib provides a set of prebuilt mesh data structures:

- vcl::TriMesh: a simple Triangle Mesh data structure, allows to store a container of Vertices, a container of Triangles and some other components;
- vcl::PolyMesh: a Polygon Mesh data structure, allows to store a container of Vertices, a container of Polygons (no costraints on the number of vertices of each polygon) and some other components;
- vcl::EdgeMesh: a Mesh data structore that allows to store a container of Vertices, a container of Edges (segments), and some other components;
- vcl::PointClout: a Mesh data structure that stores just a container of Vertices and some other components;
- vcl::Dcel: a Doubly-Connected Edge List mesh, that allows to stores containers of Vertices, Faces and Half Edges, allowing practical and efficient topology navigation;
- (ToDo: QuadMesh)
- (ToDo: volumetric data structures)

All these meshes store scalar values using `double` floating-point precision, but the library provides also the alternative with the `f` suffix that stores scalars using single floating point precision (vcl::TriMeshf, vcl::PolyMeshf, ...).

All the prebuilt data structures are documented in the @ref meshes group.

All these data structures are implemented by customizing the vcl::Mesh data structure, which is a highly customizable mesh data structure. 

To explain how this customization is possible, we first need to define the entities involved that compose a VCLib Mesh.

## Component

A Component is a C++ class that stores some data and exposes a list of member functions that allow to manipulate it. An example of Component is the  vcl::comp::Color class: this class stores a vcl::Color, and exposes some member functions that allows to access that color: vcl::comp::Color::color().

Components are combined with each other to form more complex structures, which provide all the member functions of the components they are composed of. These structures can be Elements or Meshes.

Components are listed in the @ref components group, and they are defined under the vcl::comp namespace.

## Element

An Element is a entity that is composed of one or more components. Its purpose is to expose all the member functions of the Components they composed of. An example of Element is the vcl::Vertex class: this class is composed of a collection of Components (coordinate, color, normal, ...), and exposes all the member functions of these components. 

For example, the vcl::comp::Color class has the vcl::comp::Color::color() member function that allows to access the data it stores (the color itself). A Vertex element Class that has the Color component, will allow to access that color with the same member function of the Color component.

## Container 

a Container is a collection of Elements of the same type.

## Mesh

A VCLib Mesh is a class is composed of a collection of classes that are Element Containers or Components.
