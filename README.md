# Visual Computing Library

[![BuildAndRunTests](https://github.com/cnr-isti-vclab/vclib/actions/workflows/BuildAndRunTests.yml/badge.svg)](https://github.com/cnr-isti-vclab/vclib/actions/workflows/BuildAndRunTests.yml)

The Visual Computing Library is a C++20 modern, header only, multiplatform Mesh and Geometry Processing templated library.

## Disclaimer

I designed and developed this library mostly for personal use and to study new features of C++17 and C++20.
Most of the features of this library are poorly tested: I maintain the library during my free time.

If you appreciate this work, or you think it could be improved, let me know by opening a thread in the discussion tab!
Looking forward for your feedback!

## Install

The library is header only, therefore to install it you just need to add the vclib/include directory in your
`INCLUDE_PATH`.
You can use the library by using the `cmake` command `add_subdirectory`, by adding the path to this repository
in your `cmake` project. CMake will create a target called `vclib`, that you can link using the `target_link_libraries` command.

## Compiler support

VCLib requires recent compiler versions in order to build the newly features of the C++20 language, that have been recently integrated in the major compilers.

VCLib currently builds with the follwing compilers:
  - GCC 11.3 (default in Ubuntu 22.04)
  - CLang 16 (default in XCode 15.0 on MacOS)
  - MSVC 19.36 (last version in Visual Studio 2022)

There is no guarantee that old compiler versions are capable to build the library.

If you use CMake, the minimum required version is 3.24.

## C++20

VCLib is designed to work with the most recent features of C++20:
  - Concepts
  - Ranges and Views

In the future, when CMake will add stable support to C++20 modules, the library will be refactored and will use only modules,
avoiding whenever is possible the usage of `#include`.

## Documentation

Right now, the library is a work in progress, and therefore is a work in progress also the documentation.

The best way to learn how the library works is to look at the code in the [tests](test), which is built at every commit and therefore it is guaranteed to work.

A first draft of the (not up-to-date) documentation of the library can be found [here](https://alemuntoni.github.io/vclib/).

You can build the documentation using doxygen (tested on 1.9.1 version):

```
cd docs
doxygen Doxyfile
```

The output will be placed in the `docs/_doxygen/html` directory.

## License

The library is under the [GPL3 license](LICENSE).
