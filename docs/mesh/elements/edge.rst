.. _edge:

The Edge Class
==============

The Edge Class is one of the possible :ref:`elements` of a :ref:`Mesh<mesh_documentation>`
of VCLib. It's duty is just contain a set of :ref:`components_list` or :ref:`components_optional_list`,
exposing the member functions of each component.

All the components that can be part of Edges are defined inside the ``vcl::edge``
namespace, but you always can include any other component in your custom Edge
definition.

When it is included in a Mesh definition, the Mesh will automatically instantiate
a :ref:`edge_container`, exposing its members to the :ref:`Mesh<mesh_documentation>` class.

Edge Class Definition
---------------------

.. doxygenclass:: vcl::Edge
   :members:
   :undoc-members:

Edge Components ``vcl::edge``
-----------------------------

.. doxygenfile:: edge_components.h

Edge Optional Components ``vcl::edge``
--------------------------------------

.. doxygenfile:: edge_components_optional.h
