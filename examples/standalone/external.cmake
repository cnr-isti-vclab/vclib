# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# Automatically find and add all standalone external examples if dependencies are met.
# For each directory, we check if it's a valid example by looking for a main.cpp file.
# If a requirements.cmake is present, we include it to read EXAMPLE_REQUIRED_TARGETS
# and ensure all required targets are available before adding the standalone example.
file(
    GLOB EXTERNAL_EXAMPLE_DIRS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/../external"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/*"
)
foreach(dir IN LISTS EXTERNAL_EXAMPLE_DIRS)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../external/${dir}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../external/${dir}/main.cpp")
            # Reset required targets for this iteration
            set(EXAMPLE_REQUIRED_TARGETS "")
            if(
                EXISTS
                    "${CMAKE_CURRENT_SOURCE_DIR}/../external/${dir}/requirements.cmake"
            )
                include(
                    "${CMAKE_CURRENT_SOURCE_DIR}/../external/${dir}/requirements.cmake"
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
                add_standalone_example(external "${dir}")
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
