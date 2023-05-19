# VCLib Meshes     {#mesh_documentation_page}

## Overview 
### Prebuilt Mesh data structures

VCLib provides a set of prebuilt mesh data structures:

- vcl::TriMesh: a simple Triangle Mesh data structure, allows to store a container of Vertices, a container of Triangles and some other components;
- vcl::PolyMesh: a Polygon Mesh data structure, allows to store a container of Vertices, a container of Polygons (no costraints on the number of vertices of each polygon) and some other components;
- vcl::EdgeMesh: a Mesh data structore that allows to store a container of Vertices, a container of Edges (segments), and some other components;
- vcl::PointClout: a Mesh data structure that stores just a container of Vertices and some other components;
- (ToDo: QuadMesh)
- (ToDo: volumetric data structures)

All these meshes store scalar values using `double` floating-point precision, but the library provides also the alternative with the `f` suffix that stores scalars using single floating point precision (vcl::TriMeshf, vcl::PolyMeshf, ...).

All the prebuilt data structures are documented in the @ref meshes group.

All these data structures are implemented by customizing the vcl::Mesh data structure, which is a highly customizable mesh data structure. 

### Mesh Structure

To explain how Mesh customization is possible, we first need to define the entities involved that compose a VCLib Mesh, and to do that we analyze the TriMesh example:

![trimesh](trimesh.svg "TriMesh")

In this simplified scheme of the TriMesh, there are several entities involved:

  - The overall Mesh data structure (blue);
  - Elements (green);
  - Element Containers (red);
  - Components (yellow) and the data they store internally (orange);

The overal Mesh data structure is composed of two Containers of Vertex and Face Elements, and some other Components that store some data. 
Looking the Elements in detail, we see that each Element is composed of Components: for example, a Vertex could be organized as follows:

![trimesh vertex](trimesh_vertex.svg "A Vertex Element")

In this case, the Vertex Element has for example the Coordinate, Flags, Normal and Color components, each one of them stores its data and expose it trough appropriate member functions.

To understand how these entities are organized and implemented, we suggest to take a look at the @subpage mesh_components_page, @subpage mesh_elements_page and @subpage mesh_containers_page pages.

The next sections will introduce some concpets that you should know before using the VCLib Meshes.

### Components

As you may have already noticed, components can be part of both Elements and Meshes. In fact, Components in VCLib are implemented as generic as possible, in such a way that can be used in different contexts (the main reason behind this choice is to avoid code duplication).

For example, the vcl::comp::Color component can be used by Elements, but also by the Mesh (it may be useful sometimes to assign a uniform color to the Mesh). The color component exposes the member functions `color()` and `color() const`, meaning that it will be possible to call these functions from all the Elements/Meshes that will have that component:

```cpp
AMeshType m; // A Mesh type that has the Color component

m.color() = vcl::Color::Red;

auto& v : m.vertex(0); // Get a vertex from the mesh

v.color() = vcl::Color::Green; // The Vertex Element of AMeshType has the Color component
```

When Components are part of Elements, they can actually be of two categories:

  - Horizontal (default): The data of the Component is stored inside the Component itself;
  - Vertical: The Component does not store its data, but it only exposes the member functions that allow to access it. The storage is demanded to the parent Container. When a Component is Vertical, could be:
    - Persistent: data is always available in the parent Container;
    - Optional: data can be enabled and disabled at runtime, allowing to avoid memory usage for components that are not needed in some contexts.

For example, the vcl::trimesh::Vertex class (that is the Vertex Element of the TriMesh class), has some horizontal components (Coordinate, Flags, Normal, Color, the components that are used more often), and some optional (Adjacent Faces, Adjacent Vertices, Texture Coordinates, the one that are used only on some contexts). This means that each Vertex will store internally only the horizontal components:

![trimesh vertex components](trimesh_vertex_components.svg "Vertex Components")

The Vertex container will then store both the Elements and the Vertical components (if they are optional, only the enabled ones) of the Elements. For example, assuming that we enable only the Adjacent Faces of the previous Vertex Element:

 ![vertex container](vertex_container.svg "Vertex Container with Optional Components")

 For the user, the usage of Horizontal or Vertical components is the same. The only thing that the user must check is wether a Component is Optional, and in that case, enable that component before using it. Every VCLib Container provides a list of member functions for all the Components that may be Optional for certain Components (note: these member functions are available *only when the component is actually optional*). These member functions are:

  - `m.isPer<Element><Component>Enabled()`: returns a boolean indicating whether the optional `Component` of the `Element` is enabled;
  - `m.enablePer<Element><Component>()`: enables the optional `Component` of the `Element`;
  - `m.disablePer<Element><Component>()`: disable and free the memory of the optional `Component` of the `Element`.

In the example before, to use the `AdjacentFaces` component of the `Vertex` element of a mesh `m`, you should call first:

```cpp
m.enablePerVertexAdjacentFaces();
```
