.. _vertex:

The Vertex Class
================

The Vertex Class is one of the possible :ref:`elements` of a :ref:`Mesh<mesh_documentation>`
of VCLib. It's duty is just contain a set of :ref:`components_list` or :ref:`components_optional_list`,
exposing the member functions of each component.

All the components that can be part of Vertices are defined inside the ``vcl::vert``
namespace, but you always can include any other component in your custom Vertex
definition.

When it is included in a Mesh definition, the Mesh will automatically instantiate
a :ref:`vertex_container`, exposing its members to the :ref:`Mesh<mesh_documentation>` class.

Vertex Class Definition
-----------------------

.. doxygenclass:: vcl::Vertex
   :members:
   :undoc-members:

Vertex Components ``vcl::vert``
-------------------------------

.. doxygenfile:: vertex_components.h

Vertex Optional Components ``vcl::vert``
----------------------------------------

.. doxygenfile:: vertex_components_optional.h
