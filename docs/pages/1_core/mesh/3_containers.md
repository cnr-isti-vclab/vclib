# Containers     {#mesh_containers_page}

A Container stores all the Elements of the same kind of a Mesh (the vertices, the faces, or the edges), and it is a template argument of the vcl::Mesh class: the type of a Mesh is defined by the list of its Containers (and of its Mesh Components).

The vcl::mesh::VertexContainer, vcl::mesh::FaceContainer and vcl::mesh::EdgeContainer classes are templated on the list of the Components of the Elements they store (e.g. `vcl::VertexComponents<...>`) and on the type of the indices of the Elements (`uint` by default). They derive from vcl::mesh::ElementContainer, that stores the data using a *Structure of Arrays*: for each Component there is a `std::vector` that stores the data of that Component for all the Elements, and all the vectors have the same size. The vector of an optional Component is allocated only when the Component is enabled.

The Container provides the member functions to manage its Elements. Their names include the name of the Element (they are available from the Mesh that has the Container):

  - access: `vertex(i)`, `vertices()`, `vertexCount()`, `vertexContainerSize()`, `deletedVertexCount()`;
  - addition and removal: `addVertex(...)`, `addVertices(...)`, `resizeVertices(n)`, `reserveVertices(n)`, `clearVertices()`, `deleteVertex(i)`, `compactVertices()`;
  - management of the optional and custom components: `enableVertexColor()`, `isVertexColorAvailable()`, `addVertexCustomComponent<T>(name)`, ...

where `vertexCount()` is the number of non-deleted vertices, and `vertexContainerSize()` the number of vertices including the deleted ones.

### Access to the Components

The Container does not return only Elements (proxies): the member function `vertices()` returns a vcl::mesh::ElementContainerProxy, that is a range that iterates over the Elements, jumping the deleted ones by default, and allows to access to the arrays of the Components, using the vcl::mesh::ComponentView class:

```cpp
for (auto v : mesh.vertices()) { // Elements, they are proxies
    v.position() += vcl::Point3d(1, 0, 0);
}

for (vcl::Point3d& p : mesh.vertices().positions()) { // Component view
    p += vcl::Point3d(1, 0, 0);
}

auto positions = mesh.vertices().positions();
positions.data(); // pointer to the contiguous array of positions
```

The Component views are also available for the other Components (`normals()`, `colors()`, `qualities()`, ...), and they can be combined with the VCLib views (`mesh | vcl::views::vertices | vcl::views::positions`).

### Compaction and references

When an Element is deleted, it stays in the Container, flagged as deleted. Compacting a Container (`mesh.compact()`, or `mesh.compactVertices()`) removes the deleted Elements, and updates all the references to the Elements stored by the Components of the other Containers of the Mesh (e.g. the vertex indices of the faces). The same update can be done after any other permutation of the Elements of a Container (e.g. after a sort) using the `updateVertexReferences(newIndices)` member function.
