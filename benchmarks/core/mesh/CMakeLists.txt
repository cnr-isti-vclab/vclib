# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

get_filename_component(BENCHMARK_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-core-benchmark-${BENCHMARK_NAME})

set(SOURCES
    types.h
    load_matrices.h
    test_load.h
    test_building.h
    test_bounding_box.h
    test_normals.h
    test_traversal.h
    test_topology.h
    test_transform.h
    test_copy.h
    test_compact.h
    main.cpp
)

vclib_add_benchmark(
    ${BENCHMARK_NAME}
    SOURCES ${SOURCES}
    ${HEADER_ONLY_OPTION}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE vclib-benchmarks-common)
