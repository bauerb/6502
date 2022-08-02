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
  bus->acia = NULL;

  log_info("Create CPU");
  bus->cpu = CPU6502_create(bus, bus_read, bus_write);
  log_info("Create RAM 0x0000 - 0x7FFF");
  bus->ram = memory_create(0x8000, 0x0000, 0);
  log_info("Create ACIA 0x8000 - 0x8002");
  bus->acia = acia_create(0x8000);
  log_info("Create ROM 0xC000 - 0XFFFF");
  bus->rom = memory_create(0xC000, 0x4000, 0);

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
    if((*bus)->acia != NULL)
    {
      log_info("Destroy ACIA");
      acia_destroy(&(*bus)->acia);
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
int bus_finished(struct Bus* bus)
{
  return bus->cpu->finished;
}

/*----------------------------------------------------------------------------*/
uint8_t bus_read(struct Bus *bus, uint16_t addr)
{
  uint8_t retval = 0;
  uint8_t data = 0;

  if(addr >= 0x0000 && addr <= 0x7fff) /* RAM */
  {
    retval = memory_readByte(bus->ram, addr, &data);
  }
  else if(addr >=0x8000 && addr <= 0x81ff) /* I/O Space */
  {
    if(addr >= 0x8000 && addr <= 0x8003) /* ACIA */
    {
      retval = acia_read(bus->acia, addr, &data);
    }
  }
  else if(addr >= 0xC000 && addr <= 0xffff) /* ROM */
  {
    retval = memory_readByte(bus->rom, addr, &data);
  }

  if(retval != 0)
  {
    log_error("Could not read address 0x%04x from memory", addr);
  }
  else
  {
    log_trace("Read data 0x%02x from 0x%04x", data, addr);
  }

  return data;
}

/*----------------------------------------------------------------------------*/
void bus_write(struct Bus *bus, uint16_t addr, uint8_t data)
{
  uint8_t retval = 0;

  if(addr >= 0x0000 && addr <=0x7fff) /* RAM */
  {
    retval = memory_writeByte(bus->ram, addr, data);
  }
  else if(addr >=0x8000 && addr <= 0x81ff) /* I/O Space */
  {
    if(addr >= 0x8000 && addr <= 0x8003) /* ACIA */
    {
      retval = acia_write(bus->acia, addr, data);
    }
  }
  else if(addr >= 0xc000 && addr <= 0xffff) /* ROM */
  {
    retval = 1;
  }

  if(retval != 0)
  {
    log_error("Could not write data 0x%02x to address 0x%04x", data, addr);
  }
  else
  {
    log_trace("Write data 0x%02x to addr 0x%04x", data, addr);
  }
}
