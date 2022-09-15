# Visual Computing Library

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

## Documentation

You can find the documentation of the library [here](https://alemuntoni.github.io/vclib/).

You can build the documentation trough CMake, by enabling the option `VCLIB_BUILD_DOCS`.
To build the documentation, the following dependencies must be installed and available in your system:

- doxygen (https://www.doxygen.nl/download.html#srcbin)
- sphinx (`pip3 install sphinx`)
- breathe (`pip3 install breathe`)
- readthedocs (`pip3 install sphinx_rtd_theme` and `pip3 install readthedocs-sphinx-ext`)

Once all the dependencies are satisfied and the `VCLIB_BUILD_DOCS` is enabled, you just need to build the
project, and the directory `docs/_build/html` will contain the html documentation.

## License

The library is under the [GPL3 license](LICENSE).
