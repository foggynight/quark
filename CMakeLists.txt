# Copyright (C) 2020-2021 Robert Coffey
# Released under the GPLv2 license

cmake_minimum_required(VERSION 3.16.0)
project(QUARK_PROJECT CXX)

include_directories(inc dep/rtb/inc)

add_executable(QUARK src/main.cpp)

find_package(GLEW REQUIRED)
target_link_libraries(QUARK GLEW::GLEW)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(QUARK glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(QUARK OpenGL::GL)
