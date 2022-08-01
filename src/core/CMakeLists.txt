# Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
# SPDX-License-Identifier: BSD-2-Clause

file(GLOB CORE_SRC "*.c")

add_library(core STATIC ${CORE_SRC})
