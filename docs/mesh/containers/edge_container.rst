.. _edge_container:

Edge Container
==============

The Edge Container class is a container of Edges that can be used in a :ref:`Mesh<mesh_documentation>` data structure.

This class adds a container (vector) of faces to the Mesh, making available the accessor members
to the edges, the edge number, iterators... This class will also take care to add
enablers/disablers of the eventual :ref:`components_optional_list` of the edge.

EdgeContainer Documentation
---------------------------

.. doxygenclass:: vcl::mesh::EdgeContainer
   :members:
   :undoc-members:
