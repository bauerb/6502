#include <stdio.h>

#include "util/log.h"

#include "core/bus.h"

static void test(struct Bus* bus)
{
  memory_writeByte(bus->rom, 0xFFFC, 0x00); // Set reset Vecor to 0x0200
  memory_writeByte(bus->rom, 0xFFFD, 0x02);

  memory_writeByte(bus->ram, 0x0200, 0xA9); // LDA 0x18
  memory_writeByte(bus->ram, 0x0201, 0xFF); // LDA 0x18

  log_info("RAM DUMP");
  memory_dump(bus->ram, 0x0200, 0x0220);
  log_info("ROM DUMP");
  memory_dump(bus->rom, 0xFFF0, 0xFFFF);
}

int main()
{
  struct Bus *bus = NULL;
  int i = 0;

  bus = bus_create();
  test(bus);
  bus_reset(bus);

  for(i=0; i<10;i++)
  {
    bus_clock(bus);
  }

  bus_destroy(&bus);

  return 0;
}
