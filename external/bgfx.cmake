#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

set(VCLIB_BGFX_DIR ${CMAKE_CURRENT_LIST_DIR}/bgfx)

if (VCLIB_ALLOW_BUNDLED_BGFX AND EXISTS ${VCLIB_BGFX_DIR})
    message(STATUS "- bgfx - using bundled source")

    set(BGFX_BUILD_EXAMPLES OFF)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    add_subdirectory(${VCLIB_BGFX_DIR})

    add_library(vclib-external-bgfx INTERFACE)

    # there are three warnings on gcc that we need to ignore
    set_property(TARGET bgfx PROPERTY COMPILE_WARNING_AS_ERROR OFF)

    target_link_libraries(vclib-external-bgfx INTERFACE bx bgfx bimg)

    set_target_properties(vclib-external-bgfx PROPERTIES BGFX_DIR ${VCLIB_BGFX_DIR})

    list(APPEND VCLIB_RENDER_EXTERNAL_LIBRARIES vclib-external-bgfx)
elseif(VCLIB_ALLOW_BUNDLED_BGFX)
    message(STATUS "- bgfx - not found, skipping")
endif()
