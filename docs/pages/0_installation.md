# Installation

The library can be used both as a C++ library and as a Python library.

> [!WARNING]
> **License Disclaimer:** While VCLib is licensed under the MPL2 license, please note that when using the `External` module or the `IO` module of `Core`, you are implicitly accepting the licenses of the underlying external third-party libraries, which may differ from MPL2.

## C++

VCLib requires a compiler that supports C++20 and **CMake >= 3.24**.

### Supported Compilers

VCLib uses modern C++20 features. The supported compilers are:
- **GCC 13.3** (default in Ubuntu 24.04) or later
- **Apple Clang 16** (Xcode 16.2 on macOS) / **Clang 17** or later
- **MSVC 19.36** (Visual Studio 2022) or later

### Requirements & System Dependencies

VCLib bundles some core dependencies (like Eigen, MapBox Earcut, STB). For other dependencies, the requirements vary depending on your Operating System and the modules you want to use.

**Windows Users**: You do not need to install any system dependencies manually. CMake will automatically download and build everything that is needed (including heavy libraries like CGAL and Embree).

**Linux and macOS Users**: VCLib relies on the system package manager (e.g., `apt` on Ubuntu/Debian, `brew` on macOS) to provide some libraries. Below is the exhaustive list of system dependencies:

#### Core Module
- **Linux Only (Recommended)**: `libtbb-dev` (Ubuntu/Debian)
  *VCLib uses C++17 Parallel STL for performance. Since `libstdc++` requires TBB for parallel STL, installing it is highly recommended on Linux. On macOS and Windows, parallel STL is supported without TBB. If TBB is not found, VCLib will fallback to a thread-pool implementation ([poolSTL](https://github.com/alugowski/poolSTL)).*

#### Render Module
- **Linux Only (Mandatory)**: `freeglut3-dev libgl1-mesa-dev libxi-dev libxinerama-dev libxcursor-dev libxrandr-dev` (Ubuntu/Debian)
  *Required for windowing and BGFX rendering. On macOS and Windows, the necessary windowing frameworks are already provided natively by the OS.*

#### External Module
If you enable the External module, you must install the following dependencies on both Linux and macOS:
- **For CGAL and libigl (Mandatory)**:
  - Ubuntu/Debian: `libgmp-dev libmpfr-dev`
  - macOS: `gmp mpfr`
- **For Embree (Mandatory)**:
  - Ubuntu/Debian: `libembree-dev`
  - macOS: `embree`

### CMake Integration

By default, the library is built with the **Core** module only. You can enable the other modules by setting the `VCLIB_BUILD_MODULE_<module>` CMake variable to `ON` *before* finding or adding VCLib. You can also disable the automatic download of dependencies setting `VCLIB_ALLOW_<dependency>` to `OFF`.

There are three main ways to include VCLib in your CMake project.

#### 1. Using `find_package` (Pre-compiled Archive)

You can download a pre-compiled archive of VCLib from the [GitHub Releases](https://github.com/cnr-isti-vclab/vclib/releases) page. We provide archives for both the `core` version (only the Core module) and the `all` version (Core, Render, and External modules).

Once downloaded and extracted, you can use `find_package`:

```cmake
cmake_minimum_required(VERSION 3.24)
project(my_project LANGUAGES CXX)

# enable any of the vclib modules you require here:
set(VCLIB_BUILD_MODULE_EXTERNAL OFF)
set(VCLIB_BUILD_MODULE_RENDER OFF)

find_package(vclib REQUIRED)

add_executable(my_project main.cpp)
target_link_libraries(my_project PRIVATE vclib::vclib)
```

*Note: If you extracted the archive in a non-standard location, you will need to pass `-Dvclib_DIR=/path/to/extracted/vclib/lib/cmake/vclib` when configuring your project.*

#### 2. Using `FetchContent` (Build from Source)

If you prefer to download and build the library at configuration time without dealing with archives, you can use `FetchContent`:

```cmake
cmake_minimum_required(VERSION 3.24)
project(my_project LANGUAGES CXX)

include(FetchContent)

# enable any of the vclib modules you require here:
set(VCLIB_BUILD_MODULE_EXTERNAL OFF)
set(VCLIB_BUILD_MODULE_RENDER OFF)

FetchContent_Declare(
    vclib
    GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
    GIT_TAG        origin/main) # or a specific tag

FetchContent_MakeAvailable(vclib)

add_executable(my_project main.cpp)
target_link_libraries(my_project PRIVATE vclib::vclib)
```

#### 3. Using `add_subdirectory`

Alternatively, you can clone the repository directly into your project directory (or add it as a git submodule) and include it:

```bash
cd /path/to/your/project
git clone --recursive https://github.com/cnr-isti-vclab/vclib
```

```cmake
# enable any of the vclib modules you require here:
set(VCLIB_BUILD_MODULE_EXTERNAL OFF)
set(VCLIB_BUILD_MODULE_RENDER OFF)

add_subdirectory(vclib)

add_executable(my_target main.cpp)
target_link_libraries(my_target PRIVATE vclib::vclib)
```

### Header-only Usage (Core Module)

If you only need the **Core** module, you can also use VCLib as a header-only library without involving CMake targets.

You just need to add the VCLib `include` directory and the bundled Eigen directory to your project's include path. MapBox Earcut, STB, and poolSTL are included automatically.

```cmake
target_include_directories(my_target PRIVATE 
    path/to/vclib/external/eigen-3.4.0/ # or your favorite Eigen version
    path/to/vclib/include/
)
```

## Python

WORK IN PROGRESS