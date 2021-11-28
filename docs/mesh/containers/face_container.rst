.. _face_container:

Face Container
==============

The Face Container class is a container of Faces that can be used in a :ref:`Mesh<mesh_documentation>` data structure.

This class adds a container (vector) of faces to the Mesh, making available the accessor members
to the faces, the face number, iterators... This class will also take care to add
enablers/disablers of the eventual :ref:`components_optional_list` of the face.

FaceContainer Documentation
---------------------------

.. doxygenclass:: vcl::mesh::FaceContainer
   :members:
   :undoc-members:
