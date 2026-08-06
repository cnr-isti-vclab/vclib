# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(common)

set(HEADERS
    run_render_test.h
)

add_library(vclib-render-tests-common INTERFACE)

target_include_directories(
    vclib-render-tests-common
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(vclib-render-tests-common PRIVATE ${HEADERS})
