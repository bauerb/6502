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
  log_info("Load RAM from file");
  memory_loadFromFile(bus->ram, 0x0000, "test.bin", 0x0000, 0x8000);
  log_info("Load ROM from file");
  memory_loadFromFile(bus->rom, 0x8000, "test.bin", 0x8000, 0x8000);

  log_info("RAM DUMP 0x0200 - 0x0220");
  memory_dump(bus->ram, 0x0200, 0x0220);
  log_info("ROM DUMP 0x8000 - 0x8100");
  memory_dump(bus->rom, 0x8000, 0x8100);
  log_info("ROM DUMP 0xFFF0 - 0xFFFF");
  memory_dump(bus->rom, 0xFFF0, 0xFFFF);
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
  }while(bus_is_set_to_stop(bus) != 1);

  log_info("RAM DUMP 0x0200 - 0x0220");
  memory_dump(bus->ram, 0x0200, 0x0220);

  log_info("STACK DUMP 0x0100 - 0x0200");
  memory_dump(bus->ram, 0x0100, 0x0200);

  bus_destroy(&bus);

  return 0;
}
