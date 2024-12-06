cmake_minimum_required(VERSION 3.24)

set(SOURCES main.cpp)

vclib_add_example(999-misc-bgfx-qt
    VCLIB_MODULE render
    SOURCES ${SOURCES})
