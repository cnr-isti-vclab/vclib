# Mesh Documentation     {#mesh_documentation_page}

VCLib provides a very generic Mesh class that can be highly customized.

To explain how this customization is possible, we first need to introduce what are the main
characteristics of a VCLib Mesh:

* :ref:`components`: a Component represent a characteristic of something else, and it contains
     data structure representing that characteristic. Examples of Components are Coordinate,
     BoundingBox, Color, etc. Components may be part of an :ref:`Element<elements>` or a Mesh;
* :ref:`elements`: an Element is a entity that is composed of one or more :ref:`components`.
     Examples of Elements are Vertex, Face, .... When an Element is an argument of a Mesh data
     structure, the Mesh automatically creates a :ref:`Container<containers>` of Elements;
* :ref:`containers`: a Container is a collection of :ref:`elements` of the same type.

Generally speaking, the Mesh class will inherit from :ref:`containers` of :ref:`elements` and
:ref:`components`, exposing their public member functions. :ref:`elements` are themselves composed
of a set of :ref:`components`.

Mesh Data Structures
--------------------

.. toctree::
   :maxdepth: 1

   mesh/elements
   mesh/components
   mesh/containers
   mesh/mesh_doc
   mesh/meshes
