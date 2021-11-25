.. _mesh:

Mesh Customization
==================

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

Customize an Element
--------------------

Let's for example define our customized Vertex Element, that we are going to call ``MyVertex``:

.. code-block:: cpp

   #include <vclib/mesh/mesh.h>

   class MyVertex :
       public vcl::Vertex<
           vcl::vert::BitFlags,
           vcl::vert::Coordinate3f,
           vcl::vert::Normal3f,
           vcl::vert::Color,
           vcl::vert::Scalard>
   {};

We defined a Vertex Element that has several components: `BitFlags` (flags are mandatory for each Element),
a 3D float coordinate, a 3D float normal, a color and a double-type scalar. The class MyVertex will be then
composed of all the public member functions that are provided by its components. You can find the list of
all the components that can be used to create a Vertex provided by VCLib on :ref:`this page<vertex>`, and you
can also create your own custom component class and add it in the list of components in your Vertex definition.
You can add components as many as you want in your Element definition.

Customize a Mesh
----------------

Now let's define our custom Point Cloud Mesh, that we are going to call ``MyPointCloud``:

.. code-block:: cpp

   class MyPointCloud :
       public vcl::Mesh<
           MyVertex,
           vcl::mesh::BoundingBox3d,
           vcl::mesh::TransformMatrix>
   {};

The Mesh definition will automatically recognize that ``MyVertex`` is ``vcl::Vertex``, and will
define a :ref:`vertex_container`. The other template types are not vcl Elements, therefore are
treated as Components, in the same way of the Vertex Components in ``MyVertex`` class.

Mesh Documentation
------------------

The Mesh also provides a set of member functions that need to perform operations in multiple Containers
or Components, and these member functions are listed in the :ref:`mesh_doc` page.

.. toctree::
   :maxdepth: 1

   mesh/elements
   mesh/components
   mesh/containers
