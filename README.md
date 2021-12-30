# Visual Computing Library

The Visual Computing Library is a C++17 modern, header only, multiplatform Mesh and Geometry Processing templated library.

## Install

The library is header only, therefore to install it you just need to add the vclib/include directory in your
`INCLUDE_PATH`.
You can use the library by using the `cmake` command `add_subdirectory`, by adding the path to this repository
in your `cmake` project. CMake will create a target called `vclib`, that you can link using the `target_link_libraries` command.

## Documentation

You can build the documentation trough CMake, by enabling the option `VCLIB_BUILD_DOCS`.
To build the documentation, the following dependencies must be installed and available in your system:

- doxygen (https://www.doxygen.nl/download.html#srcbin)
- sphinx (`pip3 install sphinx`)
- breathe (`pip3 install breathe`)
- furo theme (`pip3 install furo`)

Once all the dependencies are satisfied and the `VCLIB_BUILD_DOCS` is enabled, you just need to build the
project, and the directory `docs/_build/html` will contain the html documentation.

## License

The library is under the [GPL3 license](LICENSE).
