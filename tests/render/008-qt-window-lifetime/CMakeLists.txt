# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

vclib_add_test(
    008-qt-window-lifetime
    MODULE render
    SOURCES main.cpp
)

# This test requires an interactive desktop; opt in with
# VCLIB_RUN_QT_WINDOW_TESTS=1.
if(TEST 008-qt-window-lifetime)
    set_tests_properties(008-qt-window-lifetime PROPERTIES SKIP_RETURN_CODE 77)
endif()
