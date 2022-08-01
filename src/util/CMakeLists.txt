# Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
# SPDX-License-Identifier: BSD-2-Clause

file(GLOB UTIL_SRC "*.c")

add_library(util STATIC ${UTIL_SRC})
