# Add all core examples automatically
file(GLOB CORE_EXAMPLE_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/../core" "${CMAKE_CURRENT_SOURCE_DIR}/../core/*")
foreach(dir IN LISTS CORE_EXAMPLE_DIRS)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}/main.cpp")
            add_standalone_example(core "${dir}")
        endif()
    endif()
endforeach()
