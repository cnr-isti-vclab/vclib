.. _basic_concepts:

Basic Concepts
==============

This is a C++17 modern and header only Mesh and Geometry Processing templated library.

The goal of this library is to provide a set of tools and algorithms in a well-structured Object Oriented paradigm exploiting the powerfullness of C++17.

Into the ``include`` directory, the library is structured as follows:

   * algorithms: contains a set of header/source files having functions that implement various geometry processing algorithms;
   * exception: contains all the exception classes used in this library, divided by caregories in header files;
   * io: contains functions and data structures to manage the input/output of the data structures supported in this library;
   * math: contains a set of headers having various math helper functions;
   * mesh: contains all the core classes that allows to define the various Mesh data structures of this library;
   * misc: utility functions and classes used in the rest of the library;
   * space: classes that represent basic spatial coordinates and relatives, like Point, Box, Color, TexCoords.

The library also provides a set of pre-defined Mesh data structures that can be found inside the main directory of the library, like TriMesh, PolygonMesh...

Let's start
-----------

We can first declare a new Triangle Mesh and load a ply file:

.. code-block:: cpp

   #include <vclib/trimesh.h>
   #include <vclib/io/load_ply>

   vcl::Trimesh myTriMesh;

   vcl::loadPly(m, "bunny.ply");

The TriMesh data structure has containers of Vertices and Faces, plus some other
info like Bounding Box, Transform Matrix, ...

We can, for example, print some statistics and update the bounding box component
of the mesh after has been loaded:

.. code-block:: cpp

   #include <vclib/algorithms/update/bounding_box.h>

   vcl::updateBoundingBox(myTriMesh);

   std::cout << "Min bb: " << myTriMesh.boundingBox().min() << "; "
             << "Max bb: " << myTriMesh.boundingBox().max() << "\n";
   std::cout << "Vertex number: " << myTriMesh.vertexNumber() "; "
             << "Face number: "<< myTriMesh.faceNumber() "\n";

We can iterate over Vertices and over Faces of the mesh:

.. code-block:: cpp

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
           std::cout << m.index(f) << " is above the y barycenter\n";
       }
   }

Note that iterating over the vertices of the faces of the Mesh returns the
vertices/faces/elements objects by reference, while iterating over the
vertices of the face (or, in general, adjacencies of a specific element)
returns the pointer to an element that is stored inside the same Mesh. Note
also that this pointer may be ``nullptr``: in this small example we assume
that the mesh is consistent meaning that this cannot happen.


Add, set and delete Elements
----------------------------

We can create from scratch a mesh:

.. code-block:: cpp

   vcl::Trimesh mesh;

   mesh.addVertex(vcl::Point3d(0,0,0)); // add a vertex with pos 0,0,0
   mesh.addVertices(2); // add another two vertices to the mesh
   mesh.vertex(1).coord() = vcl::Point3d(1,0,0);
   mesh.vertex(2).coord() = vcl::Point3d(0,1,0);

   vcl::Point3d p0, p1, p2, p3, p4;

   // add other 5 vertices to the mesh:
   mesh.addVertices(p0, p1, p2, p3, p4);

   // add a face
   mesh.addFace(&mesh.vertex(0), &mesh.vertex(1), &mesh.vertex(2));

   // add another face but initialize vertices later:
   unsigned int fid = mesh.addFace(); // each add returns an id telling the first added element
   mesh.face(fid).setVertices(&mesh.vertex(2), &mesh.vertex(1), &mesh.vertex(3));
