# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

get_filename_component(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-core-test-${TEST_NAME})

set(HEADERS types.h)

set(SOURCES
    add_elements.cpp
    adjacencies.cpp
    append.cpp
    clean_algorithms.cpp
    concepts.cpp
    const_correctness.cpp
    container_proxy.cpp
    create_algorithms.cpp
    custom_components.cpp
    delete_algorithms.cpp
    deletion_compaction.cpp
    edges.cpp
    elem_algorithms.cpp
    import_from.cpp
    lifetime.cpp
    mesh_info.cpp
    mesh_pos.cpp
    navigation.cpp
    optional_components.cpp
    presets.cpp
    serialization.cpp
    shuffle.cpp
    stat_quality.cpp
    update_color.cpp
    update_normals.cpp
    update_quality.cpp
    update_selection.cpp
    vertex_references.cpp
    views.cpp
    wedge.cpp
)

vclib_add_test(
    ${TEST_NAME}
    SOURCES ${HEADERS} ${SOURCES}
    ${HEADER_ONLY_OPTION}
)
