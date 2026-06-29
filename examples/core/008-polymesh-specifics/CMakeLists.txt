# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

get_filename_component(EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-example-${EXAMPLE_NAME})

set(SOURCES
    polymesh_specifics.h
    main.cpp)

vclib_add_example(
    ${EXAMPLE_NAME}
    SOURCES ${SOURCES}
    ${HEADER_ONLY_OPTION})
