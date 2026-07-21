# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-core-tests)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

add_subdirectory(000-static-asserts)
add_subdirectory(001-trimesh-base)
add_subdirectory(002-mesh-topology)
add_subdirectory(003-mesh-conversions)
add_subdirectory(004-mesh-custom-components)
add_subdirectory(005-mesh-new-user-component)
add_subdirectory(006-load-save-mesh-obj)
add_subdirectory(007-load-save-mesh-off)
add_subdirectory(008-load-save-mesh-ply)
add_subdirectory(009-load-save-mesh-stl)
add_subdirectory(010-mesh-clean)
add_subdirectory(011-mesh-filter)
add_subdirectory(012-kd-tree)
add_subdirectory(013-mesh-update-normal)
add_subdirectory(014-polymesh-base)
add_subdirectory(015-mesh-copy-and-append)
add_subdirectory(016-mesh-crease-edges)
add_subdirectory(017-serialization)
add_subdirectory(018-polygon)
add_subdirectory(019-export-matrix)
add_subdirectory(020-import-matrix)
add_subdirectory(021-append-buffer)
add_subdirectory(022-grid-query)
add_subdirectory(024-space-core)
add_subdirectory(025-mesh-convex-hull)
add_subdirectory(026-random)

if(TARGET vclib-3rd-tinygltf)
    add_subdirectory(023-load-mesh-gltf)
endif()
