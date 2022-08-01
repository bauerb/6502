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
  int i = 0;

  log_set_level(LOG_DEBUG);

  bus = bus_create();
  init(bus);
  bus_reset(bus);

  for(i=0; i<200;i++)
  {
    bus_clock(bus);
  }

  log_info("RAM DUMP 0x0200 - 0x0220");
  memory_dump(bus->ram, 0x0200, 0x0220);

  bus_destroy(&bus);

  return 0;
}
