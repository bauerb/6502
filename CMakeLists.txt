# Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 2.8.9)
project(6502)

include_directories(include)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

add_subdirectory(src)
