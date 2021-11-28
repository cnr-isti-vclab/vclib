.. _face:

The Face Class
==============

The Face Class is one of the possible :ref:`elements` of a :ref:`Mesh<mesh_documentation>`
of VCLib. It's duty is to contain a set of :ref:`components_list` or :ref:`components_optional_list`,
exposing the member functions of each component.

All the components that can be part of Faces are defined inside the ``vcl::face``
namespace, but you always can include any other component in your custom Face
definition.

When it is included in a Mesh definition, the Mesh will automatically instantiate
a :ref:`face_container`, exposing its members to the :ref:`Mesh<mesh_documentation>` class.

Duty of the Face class is also to keep the data contained in its various components
consistent.

For example, consider the following Face definition:

.. code-block:: cpp

   #include <vclib/mesh/mesh.h>

   class MyFace :
       public vcl::Face<
           vcl::face::PolygonVertexRefs<MyVertex>
           vcl::face::PolygonBitFlags,
           vcl::face::AdjacentPolygons<MyFace>>
   {};

When a Face is Polygonal and stores :ref:`adjacent_faces` (``AdjacentPolygons`` component),
normally the number of adjacent polygons is tied to the number of vertices (that
is the number of edges) of the polygon. This is automatically done by the Face
class, that when modifying in some way the number of vertices of the Face, will
update also all the number of all the components that are tied to the number of
vertices.
The automatic update is performed for polygonal faces (dynamic size of the
:ref:`vertex_references`), but the property of having components tied to the
number of vertices is valid also for static sized :ref:`vertex_references` (e.g. triangles).
In fact, when declaring a Face having, for example, ``TriangleVertexRefs``, you'll
be forced to use the component ``AdjacentTraingles<Face>`` (or ``AdjacentFaces<Face, 3>``).

Generally speaking, the size of the containers tied to the vertex number of the
face will always be the same both at compile and at run time.

Face components tied to the size of the :ref:`vertex_references` are:

* :ref:`adjacent_edges`
* :ref:`adjacent_faces`
* :ref:`wedge_colors`
* :ref:`wedge_tex_coords`

Face Class Definition
---------------------

.. doxygenclass:: vcl::Face
   :members:
   :undoc-members:

Face Components ``vcl::face``
-----------------------------

.. doxygenfile:: face_components.h

Face Optional Components ``vcl::face``
--------------------------------------

.. doxygenfile:: face_components_optional.h
