# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

option(
    VCLIB_CORE_BUILD_EXAMPLES
    "If true, the examples of VCLib-core will be built."
    OFF
)

option(
    VCLIB_EXTERNAL_BUILD_EXAMPLES
    "If true, the examples of VCLib-external will be built."
    OFF
)

option(
    VCLIB_RENDER_BUILD_EXAMPLES
    "If true, the examples of VCLib-render will be built."
    OFF
)

# examples
if(VCLIB_CORE_BUILD_EXAMPLES)
    add_subdirectory(core)
endif()

if(VCLIB_BUILD_MODULE_EXTERNAL AND VCLIB_EXTERNAL_BUILD_EXAMPLES)
    add_subdirectory(external)
endif()

if(VCLIB_BUILD_MODULE_RENDER AND VCLIB_RENDER_BUILD_EXAMPLES)
    add_subdirectory(render)
endif()
