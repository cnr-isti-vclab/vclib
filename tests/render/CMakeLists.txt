# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-render-tests)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

add_subdirectory(common)

add_subdirectory(000-static-asserts)

if(TARGET vclib-3rd-bgfx)
    add_subdirectory(001-hello-triangle-headless)
    add_subdirectory(002-mesh-surface-headless)
    add_subdirectory(003-points-headless)
    add_subdirectory(004-lines-headless)
    add_subdirectory(005-mesh-wireframe-headless)
    add_subdirectory(006-mesh-pbr-headless)
    add_subdirectory(007-info-editor-headless)
endif()
