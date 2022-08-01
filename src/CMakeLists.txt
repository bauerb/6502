# Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
# SPDX-License-Identifier: BSD-2-Clause

add_subdirectory(core)
add_subdirectory(util)
add_subdirectory(tests)

add_executable(6502 main.c)
target_link_libraries(6502 core util)
