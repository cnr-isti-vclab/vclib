# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

get_filename_component(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-test-${TEST_NAME})

set(HEADERS
    canvas.h
    directional_light.h
    drawable_object.h
    drawers.h
    render_app.h
    viewer.h
    window_manager.h
)

set(SOURCES main.cpp)

vclib_add_test(
    ${TEST_NAME}
    MODULE render
    SOURCES ${HEADERS} ${SOURCES}
    ${HEADER_ONLY_OPTION}
)
