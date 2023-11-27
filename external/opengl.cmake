#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

find_package(OpenGL QUIET)

if (VCLIB_ALLOW_SYSTEM_OPENGL)
    if (OpenGL_FOUND)
        message(STATUS "- OpenGL - using system-provided library")

        add_library(vclib-external-opengl INTERFACE)
        target_link_libraries(vclib-external-opengl INTERFACE OpenGL::GL)

        if(APPLE)
            target_compile_definitions(vclib-external-opengl INTERFACE GL_SILENCE_DEPRECATION)
        endif()

        list(APPEND VCLIB_RENDER_EXTERNAL_LIBRARIES vclib-external-opengl)
    else()
        message(STATUS "- OpenGL - not found, skipping")
    endif()
endif()
