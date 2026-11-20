# Elements     {#mesh_elements_page}

An Element is the basic building block of a Mesh: the vertices, the faces and the edges of a Mesh are Elements, identified by the vcl::ElemId enumeration (`ElemId::VERTEX`, `ElemId::FACE`, `ElemId::EDGE`). All the Elements of the same kind are stored in a Container of the Mesh (see @ref mesh_containers_page).

An Element is not a data structure that stores its own data: it is a lightweight *proxy* that is composed of the index of the Element and a reference to the Container that stores it. The Element exposes all the member functions of the Components that the Container stores, and delegates their data access to the Container: for example, `vertex.position()` returns a reference to the entry of the array of the positions of the Container that corresponds to the index of the vertex.

The classes that implement the Elements are templated on the type of the Container (that can be `const`): vcl::Vertex, vcl::Face and vcl::Edge. The Mesh classes define the types of their Elements (e.g. `vcl::TriMesh::Vertex` and `vcl::TriMesh::ConstVertex`), and the type traits `vcl::VertexType`, `vcl::FaceType`, ... allow to obtain them from a Mesh, a Container or another Element. Const Meshes return const Elements, that do not allow to modify the data of the Mesh.

Elements are created by the Containers of the Mesh (`mesh.addVertex()`, `mesh.addFace(0, 1, 2)`, ...), and they are accessed through the Mesh (`mesh.vertex(i)`, `mesh.face(i)`, `mesh.vertices()`, ...). A proxy is cheap to copy, and it is valid as long as the Mesh that contains it is alive and it is not moved. The member function `isValid()` (or the conversion to `bool`) tells if the proxy refers to a Container: this is used, for example, to represent missing adjacencies.

An Element type is defined by the list of its Components. Every Element requires some Components, that are checked at compile time:

  - Vertex: BitFlags, Position;
  - Face: BitFlags (vcl::comp::TriangleBitFlags or vcl::comp::PolygonBitFlags), VertexReferences;
  - Edge: BitFlags, VertexReferences (with exactly two vertices).

The list of the Components of an Element is given using the `VertexComponents`, `FaceComponents` and `EdgeComponents` types, together with the aliases of the Components of the namespace of the Element, as it is done in the definition of the vcl::TriMeshT class (see `vclib/meshes/tri_mesh.h`).

The Elements concepts (vcl::VertexConcept, vcl::FaceConcept, vcl::EdgeConcept, ...) are listed in the @ref element_concepts group.

> Adding a new kind of Element requires several new files and the registration of the Element in different headers: the script `scripts/generate_mesh_element` automates this process.
