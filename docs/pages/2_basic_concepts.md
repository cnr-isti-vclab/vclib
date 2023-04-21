# Basic Concepts     {#basic_concepts_page}

There are a small set of headers that will bring all the library functionalitied that need to be exposed to the user:

* `#include<vclib/types.h>`: includes all the basic types used by the library. It is generally not required to include it since almost all the other library files include this header;
* `#include<vclib/space.h>`: includes all the spatial (but also non-spatial) data structures of the library, with the exception of meshes. These data structures are used by the library algorithms but they are meant to be used also by the user. Examples of data structures included by this header are Points (2D, 3D, N-Dimensional), Boxes, Triangles and Polygons, Planes, Spheres, Segments, but also complex structures like spatial regular Grids, KD-Trees, Graphs;
* `#include<vclib/meshes.h>`: includes a set of predefined Mesh data structures, such as Triangle Meshes, Polygonal Meshes, Point Clouds, [Doubly-Connected Edge List](https://en.wikipedia.org/wiki/Doubly_connected_edge_list) Meshes. Along with the predefined meshes, all the definitions that allow to create a custom Mesh data structure are included using this header;
* `#include<vclib/load_save.h>`: includes all the functions to load or save meshes into the formats supported by the library;
* `#include<vclib/concepts.h>`: includes all the c++20 [concepts](https://en.cppreference.com/w/cpp/language/constraints) of the library. They define all the constraints that a template type must satisfy to be treated, for example, as Mesh, Triangle Mesh, Point by a function (e.g. an algorithm) or another class.
* `#include<vclib/algorithms.h>`: includes all the algorithms implemented by the library. Algorithms in VCLib are simple functions that take arguments constrained by concepts. Most of the algorithms work with Meshes, but also some other utility algorithms that work with other entities are included by this headr (e.g. distance or intersection functions);
* `#include<vclib/views.h>`: includes all the c++20 [Range Views](https://en.cppreference.com/w/cpp/ranges) of the library. They allow to iterate easily along all the elements and components contained by the Meshes;
* `#include<vclib/miscellaneous.h>`: includes a list of miscellanous classes, functions or utility structures (e.g. comparators, loggers, timers, ...);

Additional headers that are Work in Progress:

* `#include<vclib/image.h>`: Data structures to manage images;

# Let's start

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

We can, for example, print some statistics and update the bounding box component
of the mesh after has been loaded:

```cpp
#include <vclib/algorithms.h>

vcl::updateBoundingBox(myTriMesh);

std::cout << "Min bb: " << myTriMesh.boundingBox().min() << "; "
          << "Max bb: " << myTriMesh.boundingBox().max() << "\n";
std::cout << "Vertex number: " << myTriMesh.vertexNumber() "; "
          << "Face number: "<< myTriMesh.faceNumber() "\n";
```

We can iterate over Vertices and over Faces of the mesh:

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
        std::cout << myTriMesh.index(f) << " is above the y barycenter\n";
    }
}
```

Note that iterating over the vertices of the faces of the Mesh returns the
vertices/faces/elements objects by reference, while iterating over the
vertices of the face (or, in general, adjacencies of a specific element)
returns the pointer to an element that is stored inside the same Mesh. Note
also that this pointer may be `nullptr`: in this small example we assume
that the mesh is consistent meaning that this cannot happen.


# Add, set and delete Elements

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