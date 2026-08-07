# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# Automatically discover and add all standalone core examples.
# We traverse the examples/core directory, check if it contains a main.cpp file,
# and if so, register it as a standalone example.
file(
    GLOB CORE_EXAMPLE_DIRS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/../core"
    "${CMAKE_CURRENT_SOURCE_DIR}/../core/*"
)
foreach(dir IN LISTS CORE_EXAMPLE_DIRS)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}/main.cpp")
            # Reset required targets for this iteration
            set(EXAMPLE_REQUIRED_TARGETS "")
            if(
                EXISTS
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}/requirements.cmake"
            )
                include(
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}/requirements.cmake"
                )
            endif()

            # Verify if all the required targets (e.g., 3rd party libraries) exist
            set(REQUIREMENTS_MET TRUE)
            set(UNMET_REQ_PRINT "")
            foreach(req IN LISTS EXAMPLE_REQUIRED_TARGETS)
                if(NOT TARGET ${req} AND NOT TARGET vclib::${req})
                    set(REQUIREMENTS_MET FALSE)
                    string(REPLACE "vclib-3rd-" "" req_print ${req})
                    list(APPEND UNMET_REQ_PRINT ${req_print})
                endif()
            endforeach()

            if(REQUIREMENTS_MET)
                add_standalone_example(core "${dir}")
            else()
                message(
                    STATUS
                    "Skipping ${dir} due to missing req: "
                    "${UNMET_REQ_PRINT}"
                )
            endif()
        endif()
    endif()
endforeach()
