# Visual Computing Library

The Visual Computing Library is a C++20 and python modern, multi-platform Mesh and Geometry Processing library. It provides a set of of functionalities for low level and high level mesh processing, rendering and visualization, and it is designed to be easily extensible customizable, and easily integrable in other projects.

All the modules can be found in the [vclib](https://github.com/cnr-isti-vclab/vclib/tree/main/vclib) directory inside the root of the repository. They are written in C++20, and can be built with CMake. The python side of the library is then generated using [pybind11](https://pybind11.readthedocs.io/en/stable/).

The library is divided in several modules, each one with a specific purpose:

  - [Core](pages/1_core.md): contains the basic core library, that in its C++ side is templated header only. It contains the basic data structures and algorithms for low level mesh geometry processing; all the other modules depend on this one.
  - [Render](pages/2_render.md): contains the rendering library, that allows to render meshes using different rendering engines and different window managers and GUI libraries.
  - **External**: contains algorithms and data structures that use external libraries, like VCG, etc.
  - **Processing**: contains algorithms for high level mesh and geometry processing tasks that can be used as black box actions.
  - **Bindings**: contains the python bindings of the library.

You can refer to the User Guide documentation to learn how to install and use the library.

The library is developed by the [Visual Computing Lab](https://vcg.isti.cnr.it) of the [ISTI Institute](https://www.isti.cnr.it) of the [National Research Council of Italy](https://www.cnr.it).

The library is under the [MPL2 license](https://github.com/cnr-isti-vclab/vclib/blob/main/LICENSE).
