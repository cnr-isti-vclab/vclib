.. _face_container:

Face Container
==============

The Face Container class is a container of Faces that can be used in a Mesh data structure.

This class adds a container (vector) of faces to the Mesh, making available the accessors members
to the faces, the face number, iterators... This class will also take care to add
enablers/disablers of the eventual optional components of the face.

FaceContainer Documentation
---------------------------

.. doxygenclass:: vcl::mesh::FaceContainer
   :members:
