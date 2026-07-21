# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

get_filename_component(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-test-${TEST_NAME})

# vcg generates a lot of warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

set(SOURCES mesh.h main.cpp)

vclib_add_test(
    ${TEST_NAME}
    MODULE external
    SOURCES ${SOURCES}
    ${HEADER_ONLY_OPTION}
)

get_property(VCG_INCLUDE_DIRS TARGET vclib-3rd-vcg PROPERTY VCG_INCLUDE_DIRS)

# add plylib.cpp source from vcg
target_sources(
    vclib-external-test-${TEST_NAME}
    PRIVATE ${VCG_INCLUDE_DIRS}/wrap/ply/plylib.cpp
)
