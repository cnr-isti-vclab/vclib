set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_library(vclib-examples-common INTERFACE)

target_compile_definitions(vclib-examples-common INTERFACE
    VCLIB_ASSETS_PATH="${VCLIB_ASSETS_PATH}")

target_compile_definitions(vclib-examples-common INTERFACE
    VCLIB_EXAMPLE_MESHES_PATH="${VCLIB_ASSETS_PATH}/example_meshes")

target_compile_definitions(vclib-examples-common INTERFACE
    VCLIB_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results")
