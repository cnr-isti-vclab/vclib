# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(EXAMPLE_NAME 06-viewer-imgui-qt)
project(vclib-render-example-${EXAMPLE_NAME})

set(SOURCES
    main.cpp)

vclib_add_example(${EXAMPLE_NAME}
    MODULE render
    SOURCES ${SOURCES})
