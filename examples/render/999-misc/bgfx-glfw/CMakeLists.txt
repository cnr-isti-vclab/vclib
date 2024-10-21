cmake_minimum_required(VERSION 3.24)

set(SOURCES main.cpp)

vclib_add_example(999-misc-bgfx-glfw
    VCLIB_MODULE render
    SOURCES ${SOURCES})
