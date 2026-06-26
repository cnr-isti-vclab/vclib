cmake_minimum_required(VERSION 3.24)
project(vclib-standalone-examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(VCLIB_USE_FETCHCONTENT "If true, VCLib is downloaded via FetchContent instead of using find_package" OFF)

if (VCLIB_USE_FETCHCONTENT)
    message(STATUS "VCLib Standalone: Using FetchContent")
    include(FetchContent)
    
    # We use the local repository root for testing local changes
    set(VCLIB_LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    
    set(VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS ON CACHE STRING "" FORCE)
    
    # We configure CMake to build only the core module to keep things fast during our tests
    set(VCLIB_BUILD_MODULE_EXTERNAL OFF CACHE STRING "" FORCE)
    set(VCLIB_BUILD_MODULE_RENDER OFF CACHE STRING "" FORCE)
    set(VCLIB_BUILD_MODULE_BINDINGS OFF CACHE STRING "" FORCE)

    FetchContent_Declare(
        vclib
        SOURCE_DIR ${VCLIB_LOCAL_DIR}
    )
    FetchContent_MakeAvailable(vclib)
else()
    message(STATUS "VCLib Standalone: Using find_package")
    find_package(vclib REQUIRED)
endif()

enable_testing()

# Define data paths macros (mirroring cmake/examples_and_tests.cmake)
set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../assets")

add_library(vclib-standalone-examples-common INTERFACE)
target_compile_definitions(vclib-standalone-examples-common INTERFACE
    VCLIB_ASSETS_PATH="${VCLIB_ASSETS_PATH}"
    VCLIB_EXAMPLE_MESHES_PATH="${VCLIB_ASSETS_PATH}/example_meshes"
    VCLIB_CORE_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/core"
    VCLIB_EXTERNAL_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/external"
)

if (NOT VCLIB_USE_FETCHCONTENT)
    # As requested by the user, add the embedded Eigen version when using find_package
    target_include_directories(vclib-standalone-examples-common INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/eigen-5.0.1"
    )
endif()

# Function to add a standalone example linking to the original source code
function(add_standalone_example module name)
    set(TARGET_NAME "standalone-${module}-${name}")
    set(SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../${module}/${name}/main.cpp")
    
    if (NOT EXISTS "${SOURCE_FILE}")
        message(FATAL_ERROR "Source file not found: ${SOURCE_FILE}")
    endif()

    add_executable(${TARGET_NAME} "${SOURCE_FILE}")
    
    # Link to vclib and the common definitions
    target_link_libraries(${TARGET_NAME} PRIVATE vclib::vclib vclib-standalone-examples-common)
    
    # Add the example directory to include path in case it uses local headers
    target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../${module}/${name}")
    
    # Register the test for CTest
    add_test(NAME run-${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()

# Add a few manual examples to test for now
add_standalone_example(core 000-mesh-io)
add_standalone_example(core 001-mesh-basics)
add_standalone_example(core 002-mesh-iteration)
