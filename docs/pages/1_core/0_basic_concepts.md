# Basic Concepts     {#core_basic_concepts}

## Let's start

We can first declare a new Triangle Mesh and load a ply file:

```cpp
#include <vclib/io.h>
#include <vclib/meshes.h>

vcl::TriMesh myTriMesh;

vcl::loadMesh(myTriMesh, "bunny.ply");
```

Or:

```cpp
vcl::TriMesh myTriMesh = vcl::loadMesh<vcl::TriMesh>("bunny.ply");
```

The TriMesh data structure has containers of Vertices and Faces, plus some other
info like Bounding Box, Transform Matrix, ...

You can find more info about the TriMesh class and other built-in classes in
the @ref core_meshes page and the list of them in the @ref meshes group.

We can, for example, print some statistics and update the bounding box component
of the mesh after that it has been loaded:

```cpp
#include <vclib/algorithms.h>

vcl::updateBoundingBox(myTriMesh);

std::cout << "Min bb: " << myTriMesh.boundingBox().min() << "; "
          << "Max bb: " << myTriMesh.boundingBox().max() << "\n";
std::cout << "Vertex count: " << myTriMesh.vertexCount() << "; "
          << "Face count: " << myTriMesh.faceCount() << "\n";
```

## Access Elements

We can access to the elements (Vertices, Faces, ...) of the mesh by index:

```cpp
vcl::Point3d p = myTriMesh.vertex(0).position();
vcl::Point3d p1 = myTriMesh.face(0).vertex(1).position();
```

We can also iterate over Vertices and over Faces of the mesh:

```cpp
// compute barycenter of the Mesh

vcl::Point3d bar;
bar.setZero();

for (const auto& v : myTriMesh.vertices()) {
    bar += v.position();
}
bar /= myTriMesh.vertexCount();

// print all the faces id with all vertices with y > barycenter.y()

for (const auto& f : myTriMesh.faces()) {
    bool up = true;
    for (const auto& v : f.vertices()) { // iterate over verts of f
        if (v.position().y() < bar.y())
            up = false;
    }
    if (up) {
        std::cout << f.index() << " is above the y barycenter\n";
    }
}
```

Note that the Elements returned by the Mesh (and by the vertices of a face, or,
in general, by the adjacencies of a specific element) are lightweight proxies
that are returned by value: they store the index of the Element and a reference
to the Container that contains it, and they give access to its Components. For
this reason, they can be cheaply copied, and they are always valid as long as
the Mesh that contains them is alive and is not moved. The adjacencies of an
Element may be missing (e.g. the adjacent face of a border edge): in that case,
the returned proxy is not valid, and it converts to `false`. In this small
example we assume that the mesh is consistent, meaning that this cannot happen.

If we are interested only in a Component of all the elements, we can iterate
directly over it, with the same performance of iterating over a contiguous
array:

```cpp
for (vcl::Point3d& p : myTriMesh.vertices().positions()) {
    p *= 2;
}
```

The same can be done using the views of VCLib:

```cpp
for (const vcl::Point3d& p : myTriMesh | vcl::views::vertices | vcl::views::positions) {
    std::cout << p << "\n";
}
```

## Add, set and delete Elements

We can create from scratch a mesh:

```cpp
vcl::TriMesh mesh;

mesh.addVertex(vcl::Point3d(0,0,0)); // add a vertex with pos 0,0,0
mesh.addVertices(2); // add another two vertices to the mesh
mesh.vertex(1).position() = vcl::Point3d(1,0,0);
mesh.vertex(2).position() = vcl::Point3d(0,1,0);

vcl::Point3d p0, p1, p2, p3, p4;

// add other 5 vertices to the mesh:
mesh.addVertices(p0, p1, p2, p3, p4);

// add a face
mesh.addFace(0, 1, 2);

// add another face but initialize vertices later:
uint fid = mesh.addFace(); // each add returns an id telling the first added element
mesh.face(fid).setVertices(mesh.vertex(2), mesh.vertex(1), mesh.vertex(3));
```


## Components

Each data that is stored in a mesh or in a element is called "component". For
example, the position of a vertex is a component, the normal of a face is a
component, the color of a vertex is a component, and so on. 

Depending on how the Element is defined, the components can be stored in different
ways, and they can be optional: that means that a component can be enabled or
disabled at runtime, using the required memory only when it is needed.

For example, in the TriMesh class, the Vertex and Face elements have their 
Color component optional, and disabled by default. We can enable them by calling:

```cpp
myTriMesh.enableVertexColor();
myTriMesh.enableFaceColor();
```

Accessing to disabled components is not allowed, and will cause undefined behavior.
The availability of an optional component can be checked using, for example, `myTriMesh.isVertexColorAvailable()`.
