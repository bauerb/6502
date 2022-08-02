/*
 * Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>

#include "util/log.h"
#include "util/tools.h"

#include "core/bus.h"

static void init(struct Bus* bus)
{
  log_info("Load ROM from file");
  memory_loadFromFile(bus->rom, 0xE000, "kernel.bin", 0x0000, 0x2000);

  log_info("RAM DUMP 0x0000 - 0x00ff");
  memory_dump(bus->ram, 0x0000, 0x00ff);
  log_info("RAM DUMP 0x0200 - 0x0220");
  memory_dump(bus->ram, 0x0200, 0x0220);
  log_info("ROM DUMP 0xE000 - 0xE100");
  memory_dump(bus->rom, 0xE000, 0xE100);
}

int main()
{
  struct Bus *bus = NULL;

  log_set_level(LOG_DEBUG);

  bus = bus_create();
  init(bus);
  bus_reset(bus);

  do
  {
    bus_clock(bus);
  }while(bus_finished(bus) == 0);

  log_info("RAM DUMP 0x0000 - 0x00FF");
  memory_dump(bus->ram, 0x0000, 0x00FF);

  log_info("RAM DUMP 0x0200 - 0x0220");
  memory_dump(bus->ram, 0x0200, 0x0220);

  log_info("STACK DUMP 0x0100 - 0x0200");
  memory_dump(bus->ram, 0x0100, 0x0200);

  bus_destroy(&bus);

  return 0;
}
