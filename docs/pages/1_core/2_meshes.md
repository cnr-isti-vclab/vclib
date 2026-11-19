# Built-in Meshes    {#core_meshes}

VCLib provides a set of prebuilt mesh data structures:

- vcl::TriMesh: a simple Triangle Mesh data structure, allows to store a container of Vertices, a container of Triangles and some other components;
- vcl::PolyMesh: a Polygon Mesh data structure, allows to store a container of Vertices, a container of Polygons (no constraints on the number of vertices of each polygon) and some other components;
- vcl::EdgeMesh: a Mesh data structure that allows to store a container of Vertices, a container of Edges (segments), and some other components;
- vcl::PointCloud: a Mesh data structure that stores just a container of Vertices and some other components;
- (ToDo: QuadMesh)
- (ToDo: volumetric data structures)

All these meshes store scalar values using `double` floating-point precision, but the library provides also the alternative with the `f` suffix that stores scalars using single floating point precision (vcl::TriMeshf, vcl::PolyMeshf, ...). Moreover, the meshes that have the `64` suffix (vcl::TriMesh64, vcl::TriMesh64f, ...) use `uint64_t` instead of `uint` as type of the indices of their Elements, allowing to store more than 2^32 Elements.

All the prebuilt data structures are documented in the @ref meshes group.

All these data structures are implemented by customizing the vcl::Mesh data structure, which is a highly customizable mesh data structure. 