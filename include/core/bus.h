/*
 * Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef BUS_H
#define BUS_H

#include <ctype.h>

#include "core/cpu6502.h"
#include "core/memory.h"
#include "core/acia.h"

struct Bus
{
  struct Memory *ram;
  struct Memory *rom;
  struct CPU6502 *cpu;

  struct ACIA *acia;
};

struct Bus* bus_create();
int bus_destroy(struct Bus** bus);

int bus_reset(struct Bus* bus);
int bus_clock(struct Bus* bus);
int bus_finished(struct Bus* bus);

#endif /* BUS_H */
