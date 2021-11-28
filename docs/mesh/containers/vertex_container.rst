.. _vertex_container:

Vertex Container
================

The Vertex Container class is a container of Vertices that can be used in a :ref:`Mesh<mesh_documentation>` data structure.

This class adds a container (vector) of vertices to the Mesh, making available the accessor members
to the vertices, the vertex number, iterators... This class will also take care to add
enablers/disablers of the eventual :ref:`components_optional_list` of the vertex.

VertexContainer Documentation
-----------------------------

.. doxygenclass:: vcl::mesh::VertexContainer
   :members:
   :undoc-members:
