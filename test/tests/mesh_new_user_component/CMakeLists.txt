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

project(vclib-mesh-new-user-component-test)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-new-user-component-test ${SOURCES})

target_link_libraries(vclib-mesh-new-user-component-test PUBLIC vclib vclib-test-common)
target_include_directories(vclib-mesh-new-user-component-test
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../examples/mesh_new_user_component)

enable_testing()
add_test(NAME mesh-new-user-component-test
    COMMAND vclib-mesh-new-user-component-test)
