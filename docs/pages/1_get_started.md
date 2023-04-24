# Get Started     {#get_started_page}

## Introduction

VCLib is a header-only C++20 library, which means that you only need to add the library (and a few dependencies) to your `INCLUDE_PATH` to start using it. However, VCLib also comes with a `cmake`-ready configuration that we strongly recommend using. Therefore, if you use `cmake` for your C++ projects, you can directly jump to the [CMake paragraph](#cmake_config).

## Supported Compilers

As the C++20 standard is new, VCLib is designed to work exclusively with recent C++ compilers::

   * GCC: 11.2
   * Clang: 14.0
   * MSVC: 17.1

## Dependencies

To access the basic functionalities of VCLib, you don't need to install any dependencies. The libraries that VCLib depends on are header-only and are bundled in the repository. These libraries are as follows:

   * [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
   * [MapBox Earcut](https://github.com/mapbox/earcut.hpp)
   * [STB](https://github.com/nothings/stb)

However, at the time of writing, some functionalities of standard C++17 require attention when used on certain platforms. Specifically, VCLib uses C++17 parallel STL, which:

   * On Apple systems using CLang, it can be used thanks to [pstld](https://github.com/mikekazakov/pstld) (header only, bundled in VCLib, no additional action required);
   * It is supported by MSVC without requiring additional action;
   * It is supported by GCC + libstdc++ when the [Threading Building Blocks](https://github.com/oneapi-src/oneTBB) library is linked (linking is required);
   * It is not supported by CLang + libc++ (except for Apple systems, thanks to `pstld`);

To access the parallel speedup of VCLib, we suggest using [CMake](#cmake_config) which will automatically check whether this feature is available on your system.

If you are using Ubuntu, you can download the `Threading Building Blocks` library by running the following command in the terminal:

```bash
sudo apt install libtbb-dev
```

## CMake Configuration {#cmake_config}

To include VCLib in your CMake project, first copy the VCLib directory into your project directory. Then, in your `CMakeLists.txt`:

```cmake
add_subdirectory(vclib)

[...]

target_link_libraries(my_target PRIVATE vclib)
```

Where `my_target` is the target of your project.
## Usage with INCLUDE_PATH

In order to work with VCLib, you must ensure that Eigen is accessible in your `INCLUDE_PATH`.
MapBox Earcut, STB, and (if you're using Apple Clang) pstld are already included automatically by VCLib, so you don't need to include them manually. Eigen is also provided within the `external` directory of VCLib.

To set your `INCLUDE_PATH` correctly, follow these steps:

```cmake
INCLUDE_PATH += path/to/vclib/external/eigen-3.4.0/ # or your favorite Eigen version
INCLUDE_PATH += path/to/vclib/include/
```

and you are ready to go.

**Note**: Using this configuration, only MSVC compiler will access to parallel speedup of the library. For other compilers, we suggest to use the [CMake configuration](#cmake_config).


