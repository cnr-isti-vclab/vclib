cmake_minimum_required(VERSION 3.24)

set(HEADERS my_dialog.h)

set(SOURCES main.cpp)

vclib_render_add_example(999-misc-none SOURCES ${HEADERS} ${SOURCES})
