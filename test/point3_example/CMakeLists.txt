# This file is part of mgplib: https://github.com/alemuntoni/mgplib
# This Source Code Form is subject to the terms of the GNU GPL 3.0

cmake_minimum_required(VERSION 3.13)
project(mgp-point3-example)

set(SOURCES
	main.cpp)

add_executable(mgp-point3-example ${SOURCES})

target_link_libraries(mgp-point3-example PUBLIC mgp)
