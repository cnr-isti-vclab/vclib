# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(TARGET vclib-3rd-qt OR TARGET vclib::vclib-3rd-qt)
    set(EXAMPLE_REQUIRED_TARGETS vclib-3rd-qt)
    set(EXAMPLE_EXTRA_DEFINITIONS VCLIB_RENDER_EXAMPLES_WITH_QT)
else()
    set(EXAMPLE_REQUIRED_TARGETS vclib-3rd-glfw)
    set(EXAMPLE_EXTRA_DEFINITIONS VCLIB_RENDER_EXAMPLES_WITH_GLFW)
endif()

list(APPEND EXAMPLE_REQUIRED_TARGETS vclib-3rd-embree)
