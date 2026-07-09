# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(common)

set(HEADERS get_drawable_mesh.h default_viewer.h)

add_library(vclib-render-examples-common INTERFACE)

target_include_directories(
    vclib-render-examples-common
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(vclib-render-examples-common PRIVATE ${HEADERS})
