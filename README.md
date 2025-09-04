# Visual Computing Library

[![Core - Build and run Tests](https://github.com/cnr-isti-vclab/vclib/actions/workflows/Core_BuildAndRunTests.yml/badge.svg)](https://github.com/cnr-isti-vclab/vclib/actions/workflows/Core_BuildAndRunTests.yml) [![External - Build and run Tests](https://github.com/cnr-isti-vclab/vclib/actions/workflows/External_BuildAndRunTests.yml/badge.svg)](https://github.com/cnr-isti-vclab/vclib/actions/workflows/External_BuildAndRunTests.yml) [![Render - Build and run Tests](https://github.com/cnr-isti-vclab/vclib/actions/workflows/Render_BuildAndRunTests.yml/badge.svg)](https://github.com/cnr-isti-vclab/vclib/actions/workflows/Render_BuildAndRunTests.yml) [![Bindings - Build and test Wheel](https://github.com/cnr-isti-vclab/vclib/actions/workflows/Bindings_BuildAndTestWheel.yml/badge.svg)](https://github.com/cnr-isti-vclab/vclib/actions/workflows/Bindings_BuildAndTestWheel.yml)

The Visual Computing Library is a C++20 and python modern, multi-platform Mesh and Geometry Processing library. It provides a set of of functionalities for mesh processing, rendering and visualization, and it is designed to be easily extensible customizable, and easily integrable in other projects.

All the modules can be found in the [vclib](https://github.com/cnr-isti-vclab/vclib/tree/main/vclib) directory inside the root of the repository. They are written in C++20, and can be built with CMake. The python side of the library is then generated using [pybind11](https://pybind11.readthedocs.io/en/stable/).

The library is divided in several modules, each one with a specific purpose:

  - **Core**: contains the basic core library, that in its C++ side is templated header only. It contains the basic data structures and algorithms for low level mesh geometry processing; all the other modules depend on this one.
    - **IO**: the `Core` module contains an inner module for input/output operations (loading from and saving to files) that can use 3rd party libraries. It can be configured in order to avoid 3rd party libraries and be used in header-only mode.
  - **Render**: contains the rendering library, that allows to render meshes using different rendering engines and different window managers and GUI libraries.
  - **External**: contains algorithms and data structures that use external libraries, like VCG, etc.
  - **Bindings**: contains the python bindings of the library.

## Documentation

Right now, the library is a work in progress, and therefore is a work in progress also the documentation.

A first draft of the documentation can be found [here](https://alemuntoni.github.io/vclib/). Please refer to it to learn how to install and use the library.

You can also refer to the [examples](https://github.com/cnr-isti-vclab/vclib/tree/main/examples) directory to see how the library can be used.

You can build the documentation using doxygen (tested on 1.9.1 version):

```
doxygen docs/Doxyfile
```

Be sure to call doxygen from the root directory of the repository, otherwise the paths in the Doxyfile will be wrong.

The output will be placed in the `docs/_doxygen/html` directory.

## Copyright and License

The library is developed by the [Visual Computing Lab](https://vcg.isti.cnr.it) of the [ISTI Institute](https://www.isti.cnr.it) of the [National Research Council of Italy](https://www.cnr.it).

The library is under the [MPL2 license](LICENSE).
