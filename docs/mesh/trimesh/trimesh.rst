.. _trimesh:

=======
TriMesh
=======

The TriMesh class is a pre-defined VCL Mesh data structure for triangle Meshes.

The data structure is a Mesh composed of:

   * a Vertex Container
   * a Face Container
   * a Bounding Box Component
   * a Mark Component
   * a Transform Matrix Component

You can look for the :ref:`trimesh_vertex` and the :ref:`trimesh_face` pages to see how Vertices
and Faces are defined in the TriMesh class.

Listed below there are the Mesh, VertexContainer and FaceContainer member functions,
that are directly inherited by the TriMesh class.

Note that containers have provide a set of member functions that are enabled only if their elements
have a particular Optional Component. Check in the trimesh::Vertex and the trimesh::Face pages if a
particular component is optional and therefore if these member function are enabled into the TriMesh class.

TriMesh Documentation Pages
---------------------------

.. toctree::
   :maxdepth: 1

   ../mesh_doc
   ../containers/vertex_container
   ../containers/face_container
