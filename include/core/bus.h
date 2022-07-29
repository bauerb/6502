#ifndef BUS_H
#define BUS_H

#include <ctype.h>

#include "core/cpu6502.h"
#include "core/memory.h"

struct Bus
{
  struct Memory *ram;
  struct Memory *rom;
  struct CPU6502 *cpu;
};

struct Bus* bus_create();
int bus_destroy(struct Bus** bus);

int bus_reset(struct Bus* bus);
int bus_clock(struct Bus* bus);

#endif /* BUS_H */
