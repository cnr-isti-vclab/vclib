# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-render)

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/render/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/vclib/render/*.cpp")

file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "assets/*")

if (TARGET vclib-3rd-qt)
    file (GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS "assets/*.qrc")
endif()

# if render engine is bgfx - include sources that use bgfx
if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    file(GLOB_RECURSE HEADERS_BGFX CONFIGURE_DEPENDS
        "include/vclib/bgfx/*.h")
    list(APPEND HEADERS ${HEADERS_BGFX})

    file(GLOB_RECURSE SOURCES_BGFX CONFIGURE_DEPENDS
        "src/vclib/bgfx/*.cpp")
    list(APPEND SOURCES ${SOURCES_BGFX})

    if (APPLE)
        file(GLOB_RECURSE SOURCES_MM CONFIGURE_DEPENDS
            "src/vclib/bgfx/*.mm")
        list(APPEND SOURCES ${SOURCES_MM})
    endif()

    vclib_generate_all_shaders_from_configs(
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vclib"
    )

    file(GLOB_RECURSE SHADERS_BGFX CONFIGURE_DEPENDS
        "shaders/vclib/bgfx/*.sh" "shaders/vclib/bgfx/*.sc"
        "${CMAKE_CURRENT_BINARY_DIR}/shaders/vclib/bgfx/*.sh" 
        "${CMAKE_CURRENT_BINARY_DIR}/shaders/vclib/bgfx/*.sc")
    list(APPEND SHADERS ${SHADERS_BGFX})

    if (TARGET vclib-3rd-imgui)
        file(GLOB_RECURSE HEADERS_BGFX_IMGUI CONFIGURE_DEPENDS
            "include/vclib/bgfx_imgui/*.h")
        list(APPEND HEADERS ${HEADERS_BGFX_IMGUI})

        file(GLOB_RECURSE SOURCES_BGFX_IMGUI CONFIGURE_DEPENDS
            "src/vclib/bgfx_imgui/*.cpp")
        list(APPEND SOURCES ${SOURCES_BGFX_IMGUI})

        file(GLOB_RECURSE SHADERS_BGFX_IMGUI CONFIGURE_DEPENDS
            "shaders/vclib/bgfx_imgui/*.sh" "shaders/vclib/bgfx_imgui/*.sc")
        list(APPEND SHADERS ${SHADERS_BGFX_IMGUI})
    endif()
endif()

# if render engine is opengl2 - include sources that use opengl2
if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    file(GLOB_RECURSE HEADERS_OPENGL2 CONFIGURE_DEPENDS
        "include/vclib/opengl2/*.h")
    list(APPEND HEADERS ${HEADERS_OPENGL2})

    file(GLOB_RECURSE SOURCES_OPENGL2 CONFIGURE_DEPENDS
        "src/vclib/opengl2/*.cpp")
    list(APPEND SOURCES ${SOURCES_OPENGL2})
endif()

if (TARGET vclib-3rd-glfw)
    file(GLOB_RECURSE HEADERS_GLFW CONFIGURE_DEPENDS
        "include/vclib/glfw/*.h")
    list(APPEND HEADERS ${HEADERS_GLFW})

    file(GLOB_RECURSE SOURCES_GLFW CONFIGURE_DEPENDS "src/vclib/glfw/*.cpp")
    list(APPEND SOURCES ${SOURCES_GLFW})

    if (TARGET vclib-3rd-imgui)
        file(GLOB_RECURSE HEADERS_GLFW_IMGUI CONFIGURE_DEPENDS
            "include/vclib/glfw_imgui/*.h")
        list(APPEND HEADERS ${HEADERS_GLFW_IMGUI})

        file(GLOB_RECURSE SOURCES_GLFW_IMGUI CONFIGURE_DEPENDS
            "src/vclib/glfw_imgui/*.cpp")
        list(APPEND SOURCES ${SOURCES_GLFW_IMGUI})
    endif()
endif()

if (TARGET vclib-3rd-imgui)
    file(GLOB_RECURSE HEADERS_IMGUI CONFIGURE_DEPENDS
        "include/vclib/imgui/*.h")
    list(APPEND HEADERS ${HEADERS_IMGUI})
endif()

if (TARGET vclib-3rd-qt)
    file(GLOB_RECURSE HEADERS_QT CONFIGURE_DEPENDS "include/vclib/qt/*.h")
    list(APPEND HEADERS ${HEADERS_QT})

    file(GLOB_RECURSE SOURCES_QT CONFIGURE_DEPENDS "src/vclib/qt/*.cpp")
    list(APPEND SOURCES ${SOURCES_QT})

    file(GLOB_RECURSE FORMS_QT CONFIGURE_DEPENDS "src/vclib/qt/*.ui")
    list(APPEND FORMS ${FORMS_QT})

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})

    if (TARGET vclib-3rd-imgui)
        file(GLOB_RECURSE HEADERS_QT_IMGUI CONFIGURE_DEPENDS
            "include/vclib/qt_imgui/*.h")
        list(APPEND HEADERS ${HEADERS_QT_IMGUI})

        file(GLOB_RECURSE SOURCES_QT_IMGUI CONFIGURE_DEPENDS
            "src/vclib/qt_imgui/*.cpp")
        list(APPEND SOURCES ${SOURCES_QT_IMGUI})
    endif()
endif()

add_library(${PROJECT_NAME}
    ${HEADERS} ${SOURCES} ${SHADERS} ${FORMS} ${RESOURCES})
add_library(vclib::render ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${VCLIB_RENDER_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_definitions(${PROJECT_NAME} INTERFACE VCLIB_RENDER_MODULE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_INCLUDE_DIR ${VCLIB_RENDER_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_SHADER_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_BACKEND_BGFX)



    vclib_build_shaders_to_headers(${SHADERS})
    vclib_build_assets_to_headers(${ASSETS})

    # Add embedded programs from config files
    vclib_add_embedded_programs_from_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/embedded_vf_programs.config"
        VERT_FRAG
    )
    vclib_add_embedded_programs_from_file(
        "${CMAKE_CURRENT_BINARY_DIR}/shaders/embedded_vf_combo_programs.config"
        VERT_FRAG
    )
    vclib_add_embedded_programs_from_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/embedded_c_programs.config"
        COMPUTE
    )
    
    # Generate the combined headers
    vclib_generate_embedded_programs_headers()
    
    if (VCLIB_GENERATED_SHADER_CONFIGS)
        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Shaders" FILES ${VCLIB_GENERATED_SHADER_CONFIGS})
        target_sources(${PROJECT_NAME} PRIVATE ${VCLIB_GENERATED_SHADER_CONFIGS})
    endif()
endif()

if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_BACKEND_OPENGL2)
endif()

if (TARGET vclib-3rd-qt)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTORCC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOUIC ON)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${VCLIB_RENDER_3RDPARTY_LIBRARIES} vclib::core)

# Install
if(VCLIB_INSTALL_MODULE_RENDER)
    install(DIRECTORY ${VCLIB_RENDER_INCLUDE_DIR}/vclib
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/vclib
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)

    install(TARGETS ${PROJECT_NAME}
        EXPORT vclibTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
