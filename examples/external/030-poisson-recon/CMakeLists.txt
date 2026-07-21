# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

get_filename_component(EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-external-example-${EXAMPLE_NAME})

set(SOURCES main.cpp)

vclib_add_example(
    ${EXAMPLE_NAME}
    MODULE external
    SOURCES ${SOURCES}
)

set_property(
    TARGET vclib-external-example-${EXAMPLE_NAME}
    PROPERTY COMPILE_WARNING_AS_ERROR OFF
)
