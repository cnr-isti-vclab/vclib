# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-external-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

if (TARGET vclib-3rd-embree)
    add_subdirectory(010-embree)

    add_subdirectory(011-best-heightfield-orientation)
endif()

if (TARGET vclib-3rd-boost AND TARGET vclib-3rd-cgal AND TARGET vclib-3rd-libigl)
    add_subdirectory(020-igl-booleans)
endif()

if (TARGET vclib-3rd-poisson-recon)
    add_subdirectory(030-poisson-recon)
endif()
