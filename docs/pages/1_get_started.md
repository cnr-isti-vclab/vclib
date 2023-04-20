Get Started     {#get_started}
===========

Introduction
------------

VCLib is a C++20 header-only library, which means that you just need to add the library (and a few dependencies) in your ``INCLUDE_PATH`` to
start to use the library. However, VCLib comes also with a ``cmake`` ready configuration, that we strongly suggest to use.
Therefore, if you use ``cmake`` for your C++ projects, you can derectly jump to the :ref:`cmake` paragraph.

Supported Compilers
-------------------

Since the C++20 standard is recent, VCLib works only with recent C++ compilers:

   * GCC: 11.2
   * Clang: 14.0
   * MSVC: 17.1

Dependencies
------------

VCLib has two main dependent header only libraries:

   * `Eigen <https://eigen.tuxfamily.org/index.php?title=Main_Page>`_
   * `MapBox Earcut <https://github.com/mapbox/earcut.hpp>`_

Moreover, VCLib uses C++17 parallel STL, which:

   * It is still not supported by CLang
   * It is supported by MSVC
   * It is supported by GCC when the `Threading Building Blocks <https://github.com/oneapi-src/oneTBB>`_ library is linked.

To have access to the parallel speedup of VCLib, we suggest to use the :ref:`cmake` that will check automatically if
this feature is available in your system.

On Ubuntu, you can download the `Threading Building Blocks` library by running on terminal:

.. code-block::

   sudo apt install libtbb-dev

Usage with ``INCLUDE_PATH``
---------------------------

To work with VCLib, you need to have Eigen accessible in your ``INCLUDE_PATH``.
MapBox Earcut is already included automatically by VCLib and you don't need to include it manually.

Eigen is also provided inside the ``external`` directory of VCLib.

Set your ``INCLUDE_PATH`` in the following way:

.. code-block::

   INCLUDE_PATH += path/to/vclib/external/eigen-3.4.0/ # or your favorite Eigen version
   INCLUDE_PATH += path/to/vclib/include/

and you are ready to go.

`Note`: only MSVC compiler will access to parallel speedup of the library. For other compilers, we suggest to use the :ref:`cmake`.

.. _cmake:

CMake Configuration
-------------------

To include VCLib in your CMake project, you'll first need to place the VCLib directory inside your project directory.
Then, in your ``CMakeLists.txt``:

.. code-block::

   add_subdirectory(vclib)

   [...]

   target_link_libraries(my_target PRIVATE vclib)

Where ``my_target`` is the target of your project.
