# Installation

The library can be used both as a C++ library and as a Python library.

## C++

In order to work easily with VCLib, you can use the CMake configuration, but you can also use the core module of the library as a header-only library.

### CMake

The minimum required version is 3.24.

CMake will take care to find and download all the required and optional dependencies for each module of the library that you want to build and use.
By default, the library is built with the `Core` module only, but you can enable the other modules by setting the `VCLIB_BUILD_MODULE_<module>` CMake variable to `ON`, before adding the VCLib directory to your project or in your CMake configuration.

There are two very easy ways to include VCLib in your project.

#### Using *add_subdirectory*

To include VCLib in your CMake project, first clone (recursively) the VCLib repository into your project directory:

```bash
cd /path/to/your/project
git clone --recursive https://github.com/cnr-isti-vclab/vclib
```

(alternatively, you can use [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)).

Then, in your `CMakeLists.txt`:

```cmake
# If you want to use other modules, set the corresponding variable to ON, e.g:
# set(VCLIB_BUILD_MODULE_RENDER ON)
add_subdirectory(vclib)

[...]
add_executable(my_target main.cpp)
target_link_libraries(my_target PRIVATE vclib::vclib)
```

Where `my_target` is the target of your project.

#### Using *FetchContent*

If you don't want to clone the repository, you can use `FetchContent` to download the library at configuration time. In your `CMakeLists.txt`:

```cmake
include(FetchContent)
```

WORK IN PROGRESS

### Usage with *INCLUDE_PATH*

You can also use the VCLib Core module just as a header-only library.

In order to work with VCLib, you must ensure that Eigen is accessible in your `INCLUDE_PATH`.
MapBox Earcut, STB, and poolSTL are already included automatically by VCLib, so you don't need to include them manually. Eigen is also provided within the `external` directory of VCLib.

To set your `INCLUDE_PATH` correctly, follow these steps:

```cmake
INCLUDE_PATH += path/to/vclib/external/eigen-3.4.0/ # or your favorite Eigen version
INCLUDE_PATH += path/to/vclib/include/
```

and you are ready to go.

### Supported Compilers

VCLib requires recent compiler versions in order to build the newly features of the C++20 language, that have been recently integrated in the major compilers:

  - GCC 13.3 (default in Ubuntu 24.04)
  - Apple CLang 16 (XCode 16.2 on MacOS) or CLang 17
  - MSVC 19.36 (Visual Studio 2022)

#### C++17 Parallel STL

The lack of support for C++17 parallel STL in some compilers is still a problem.

To allow the use of C++17 parallel STL, VCLib uses [poolSTL](https://github.com/alugowski/poolSTL) 
as a fallback. poolSTL will use the parallel STL if it is available, and otherwise, it implements 
the parallel algorithms using a thread pool.

In the case of libstdc++, the parallel STL is available only if the `Threading Building Blocks`
library is installed. If you are using libstdc++, we suggest to install TBB in 
order to bypass poolSTL and use the parallel STL.

On Ubuntu, you can install it by running the following command in the terminal:

```bash
sudo apt install libtbb-dev
```

#### Dependencies

VCLib has several dependencies that depend on the module you want to use. 
Along with TBB or poolSTL for parallelism, the Core module depends on the following libraries:

   * [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
   * [MapBox Earcut](https://github.com/mapbox/earcut.hpp)
   * [STB](https://github.com/nothings/stb)

All these libraries are bundled with VCLib, so you don't need to install them separately.

All the other dependencies can be required or optional, and they are needed by the other modules of the library.
The CMake configuration, when possible, will take care to download them for you using `FetchContent`.
You can always enable or disable the usage of a dependency by setting the corresponding CMake variable called
`VCLIB_ALLOW_<dependency>` to `ON` or `OFF`.

## Python

WORK IN PROGRESS