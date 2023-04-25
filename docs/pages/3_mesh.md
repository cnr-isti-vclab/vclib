# VCLib Meshes     {#mesh_documentation_page}

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

To explain how this customization is possible, we first need to introduce what are the main
characteristics of a VCLib Mesh:

- Components: a Component represent a characteristic of something else, and it contains data structure representing that characteristic. Examples of Components are Coordinate, BoundingBox, Color, etc. Components may be part of an Element or a Mesh; All the components are listed in the @ref components group, and they can be found under the vcl::comp namespace;
- Elements: an Element is a entity that is composed of one or more components. Examples of Elements are Vertex, Face, .... When an Element is an argument of a Mesh data structure, the Mesh automatically creates a Container of Elements;
- Containers: a Container is a collection of £lements of the same type.
