# Basic Concepts     {#core_basic_concepts}

## Let's start

We can first declare a new Triangle Mesh and load a ply file:

```cpp
#include <vclib/meshes.h>
#include <vclib/load_save.h>

vcl::TriMesh myTriMesh;

vcl::load(myTriMesh, "bunny.ply");
```

Or:

```cpp
vcl::TriMesh myTriMesh = vcl::load<vcl::TriMesh>("bunny.ply");
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
std::cout << "Vertex number: " << myTriMesh.vertexNumber() "; "
          << "Face number: "<< myTriMesh.faceNumber() "\n";
```

## Access Elements

We can access to the elements (Vertices, Faces, ...) of the mesh by index:

```cpp
vcl::Point3d p = myTriMesh.vertex(0).coord();
vcl::Point3d p1 = myTriMesh.face(0).vertex(1)->coord();
```

We can also iterate over Vertices and over Faces of the mesh:

```cpp
// compute barycenter of the Mesh

vcl::Point3d bar;
bar.setZero();

for (const vcl::TriMesh::Vertex& v : myTriMesh.vertices()) {
    bar += v.coord();
}
bar /= myTriMesh.vertexNumber();

// print all the faces id with all vertices with y > barycenter.y()

for (const vcl::TriMesh::Face& f : myTriMesh.faces()) {
    bool up = true;
    for (const vcl::TriMesh::Vertex* v : f.vertices()) { // iterate over verts of f
        if (v->coord().y() < bar.y())
            up = false;
    }
    if (up) {
        std::cout << f.index() << " is above the y barycenter\n";
    }
}
```

Note that iterating over the vertices of the faces of the Mesh returns the
vertices/faces/elements objects by reference, while iterating over the
vertices of the face (or, in general, adjacencies of a specific element)
returns the pointer to an element that is stored inside the same Mesh. Note
also that this pointer may be `nullptr`: in this small example we assume
that the mesh is consistent meaning that this cannot happen.


## Add, set and delete Elements

We can create from scratch a mesh:

```cpp
vcl::TriMesh mesh;

mesh.addVertex(vcl::Point3d(0,0,0)); // add a vertex with pos 0,0,0
mesh.addVertices(2); // add another two vertices to the mesh
mesh.vertex(1).coord() = vcl::Point3d(1,0,0);
mesh.vertex(2).coord() = vcl::Point3d(0,1,0);

vcl::Point3d p0, p1, p2, p3, p4;

// add other 5 vertices to the mesh:
mesh.addVertices(p0, p1, p2, p3, p4);

// add a face
mesh.addFace(0, 1, 2);

// add another face but initialize vertices later:
uint fid = mesh.addFace(); // each add returns an id telling the first added element
mesh.face(fid).setVertices(&mesh.vertex(2), &mesh.vertex(1), &mesh.vertex(3));
```


## Components

Each data that is stored in a mesh or in a element is called "component". For
example, the coordinate of a vertex is a component, the normal of a face is a
component, the color of a vertex is a component, and so on. 

Depending on how the Element is defined, the components can be stored in different
ways, and they can be optional: that means that a component can be enabled or
disabled at runtime, using the required memory only when it is needed.

For example, in the TriMesh class, the Vertex and Face elements have their 
Color component optional, and disabled by default. We can enable them by calling:

```cpp
myTriMesh.enablePerVertexColor();
myTriMesh.enablePerFaceColor();
```

Accessing to disabled components is not allowed, and will cause undefined behavior.
