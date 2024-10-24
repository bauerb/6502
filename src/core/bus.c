/*
 * Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>
#include <ctype.h>

#include "util/log.h"

#include "core/bus.h"

static uint8_t bus_read(struct Bus *bus, uint16_t addr);
static void bus_write(struct Bus *bus, uint16_t addr, uint8_t data);

/*----------------------------------------------------------------------------*/
struct Bus* bus_create()
{
  struct Bus *bus = NULL;

  log_info("Create Bus");

  bus = malloc(sizeof(struct Bus));
  if(bus == NULL)
  {
    log_error("Could not allocate memory for bus");
    return NULL;
  }

  bus->ram = NULL;
  bus->rom = NULL;
  bus->cpu = NULL;
  bus->stop = 0;

  log_info("Create RAM");
  bus->ram = memory_create(0x8000, 0x0000, 0);
  log_info("Create ROM");
  bus->rom = memory_create(0x8000, 0x8000, 0);
  log_info("Create CPU");
  bus->cpu = CPU6502_create(bus, bus_read, bus_write);

  return bus;
}

/*----------------------------------------------------------------------------*/
int bus_destroy(struct Bus** bus)
{
  log_info("Destroy Bus");

  if(*bus != NULL)
  {
    if((*bus)->cpu != NULL)
    {
      log_info("Destroy CPU");
      CPU6502_destroy(&(*bus)->cpu);
    }
    if((*bus)->ram != NULL)
    {
      log_info("Destroy RAM");
      memory_destroy(&(*bus)->ram);
    }
    if((*bus)->rom != NULL)
    {
      log_info("Destroy ROM");
      memory_destroy(&(*bus)->rom);
    }
    free(*bus);
    *bus = NULL;
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
int bus_reset(struct Bus* bus)
{
  CPU6502_reset(bus->cpu);
  return 0;
}

/*----------------------------------------------------------------------------*/
int bus_clock(struct Bus* bus)
{
  do
  {
    CPU6502_clock(bus->cpu);
  }while(CPU6502_complete(bus->cpu) == 0);

  CPU6502_dumpStatus(bus->cpu);

  return 0;
}

/*----------------------------------------------------------------------------*/
uint8_t bus_read(struct Bus *bus, uint16_t addr)
{
  uint8_t data = 0;
  struct Memory *mem = NULL;

  if(addr >= 0x0000 && addr <=0x7fff)
  {
    mem = bus->ram;
  }
  else if(addr >= 0x8000 && addr <=0xffff)
  {
    mem = bus->rom;
  }

  if(memory_readByte(mem, addr, &data) != 0)
  {
    log_error("Could not read address 0x%04x from memory", addr);
    return 0;
  }

  log_trace("Read data 0x%02x from 0x%04x", data, addr);
  return data;
}

/*----------------------------------------------------------------------------*/
void bus_write(struct Bus *bus, uint16_t addr, uint8_t data)
{
  if(addr >= 0x0000 && addr <=0x7fff)
  {
    if(memory_writeByte(bus->ram, addr, data) != 0)
    {
      log_error("Could not write data 0x%02x to address 0x%04x", data, addr);
    }

    log_trace("Write data 0x%02x to 0x%04x", data, addr);
  }
  else if(addr >= 0x8000 && addr <= 0xffff)
  {
    log_error("Could not write data 0x%02x to ROM addr 0x%04x", data, addr);
  }
}

/*----------------------------------------------------------------------------*/
int bus_is_set_to_stop(struct Bus* bus)
{
  return bus->stop;
}

/*----------------------------------------------------------------------------*/
int bus_set_to_stop(struct Bus* bus)
{
  bus->stop = 1;
  return 0;
}
