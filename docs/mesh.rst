.. _mesh:

Mesh
====

VCLib provides a very generic Mesh library that can be highly customized with Elements (Vertices, Faces...)
and Components (Bounding Box, Textures, Transform Matrix, ...), using Templates and inheritance.

Generally speaking, the Mesh class will inherit from :ref:`containers` and :ref:`components`, exposing their
public member functions. The Mesh also provides a set of member functions that need to perform operations in
multiple Containers or Components, and these member functions are listed in this page.

Depending on the Mash definition, it can inherit (and therefore expose their member functions) from the following
Element Container classes:

   * :ref:`vertex_container`
   * :ref:`face_container`

and from the following components:

   * :ref:`bounding_box`
   * Mark
   * TransformMatrix

To see the member functions that they expose the Mesh class you're using, please check their documentation.
The Mesh could also be composed of custom Element Containers/Components. Check your specific mesh definition to
see which member function are inherited and can be used.

To see how to define a custom Mesh Data structure, please take a look to this tutorial.

Mesh Documentation
------------------

.. doxygenclass:: vcl::Mesh
   :members:
