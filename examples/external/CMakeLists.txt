# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-external-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

# Automatically find and add all external examples.
# For each directory, we check if it's a valid example by looking for a main.cpp file.
# If a requirements.cmake is present, we include it to read EXAMPLE_REQUIRED_TARGETS
# and ensure all required targets are available before adding the subdirectory.
file(GLOB EXTERNAL_EXAMPLE_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(dir IN LISTS EXTERNAL_EXAMPLE_DIRS)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/main.cpp")
            # Reset required targets for this iteration
            set(EXAMPLE_REQUIRED_TARGETS "")
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/requirements.cmake")
                include("${CMAKE_CURRENT_SOURCE_DIR}/${dir}/requirements.cmake")
            endif()
            
            # Verify if all the required targets (e.g., 3rd party libraries) exist
            set(REQUIREMENTS_MET TRUE)
            foreach(req IN LISTS EXAMPLE_REQUIRED_TARGETS)
                if(NOT TARGET ${req})
                    set(REQUIREMENTS_MET FALSE)
                    break()
                endif()
            endforeach()

            if(REQUIREMENTS_MET)
                add_subdirectory("${dir}")
            else()
                message(STATUS "Skipping external example ${dir} due to missing required targets")
            endif()
        endif()
    endif()
endforeach()
