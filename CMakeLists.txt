# Copyright (C) 2020 Robert Coffey
# Released under the GPLv2 license

cmake_minimum_required(VERSION 3.0.0)
project(QUARK_PROJECT C)

add_executable(quark src/main.c)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(quark glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(quark OpenGL::GL)
