/*
 * Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "util/log.h"

#include "core/memory.h"

/*----------------------------------------------------------------------------*/
struct Memory* memory_create(uint32_t size, uint32_t baseaddr, uint8_t readonly)
{
  struct Memory *mem = NULL;

  log_trace("Memory: Try to allocate struct Memory");
  mem = malloc(sizeof(struct Memory));
  if(!mem)
  {
    log_error("Could not allocate memory for struct Memory");
    return NULL;
  }

  log_trace("Memory: Try to allocate 0x%04x bytes of memory", size);
  mem->mem = malloc(sizeof(uint8_t) * size);
  if(!mem->mem)
  {
    log_error("Could not allocate memory for struct Memory");
    free(mem);
    return NULL;
  }

  log_trace("Create Memory size 0x%04x and baseaddr 0x%04x", size, baseaddr);

  memset(mem->mem, 0, size);
  mem->readonly = readonly;
  mem->size = size;
  mem->baseaddr = baseaddr;

  return mem;
}

/*----------------------------------------------------------------------------*/
void memory_destroy(struct Memory **memory)
{
  if(*memory)
  {
    if((*memory)->mem)
    {
      free((*memory)->mem);
    }
    free(*memory);
    *memory = NULL;
  }
}

/*----------------------------------------------------------------------------*/
int memory_readByte(struct Memory* mem, uint32_t addr, uint8_t *data)
{
  if(mem == NULL)
  {
    log_error("Memory is NULL");
    return -1;
  }
  if(mem->mem == NULL)
  {
    log_error("Memory not initialized");
    return -1;
  }
  if(addr < mem->baseaddr || addr > mem->baseaddr + mem->size)
  {
    log_error("Address out of memory range addr 0x%04x baseaddr 0x%04x size 0x%04x", addr, mem->baseaddr, mem->size);
    return -1;
  }

  *data = mem->mem[addr - mem->baseaddr];
  log_trace("Read memory from addr 0x%04x data 0x%02x", addr, *data);

  return 0;
}

/*----------------------------------------------------------------------------*/
int memory_writeByte(struct Memory* mem, uint32_t addr, uint8_t data)
{
  log_trace("Try to write data 0x%02x to address 0x%04x baseaddr 0x%04x size 0x%04x", data, addr, mem->baseaddr, mem->size);

  if(mem == NULL)
  {
    log_error("Memory is NULL");
    return -1;
  }
  if(mem->mem == NULL)
  {
    log_error("Memory not initialized");
    return -1;
  }
  if(addr < mem->baseaddr || addr > mem->baseaddr + mem->size)
  {
    log_error("Address out of memory range addr 0x%04x baseaddr 0x%04x size 0x%04x", addr, mem->baseaddr, mem->size);
    return -1;
  }

  mem->mem[addr - mem->baseaddr] = data;
  log_trace("Write data 0x%02x to addr 0x%04x", data, addr);

  return 0;
}

/*----------------------------------------------------------------------------*/
int memory_dump(struct Memory* mem, uint32_t start, uint32_t end)
{
  int line = 0;
  int col = 0;

  if(mem == NULL)
  {
    log_error("Memory is NULL");
    return -1;
  }
  if(mem->mem == NULL)
  {
    log_error("Memory not initialized");
    return -1;
  }
  if(start > end || start < mem->baseaddr || start > mem->baseaddr + mem->size || end  > mem->baseaddr + mem->size)
  {
    log_error("Address out of memory range start 0x%04x end 0x%04x base 0x%04x size 0x%04x", start, end, mem->baseaddr, mem->size);
    return -1;
  }

  log_dump("Hexdump from 0x%04x to 0x%04x\n", start, end);

  for(line = 0; line * 16 < (end - start); line++)
  {
    log_dump("0x%04x: ", start + (line * 16));

    for(col = 0; col < 16; col++)
    {
      log_dump("%02x ", mem->mem[start - mem->baseaddr + (line * 16) + col]);
    }

    log_dump(" | ");

    for(col = 0; col < 16; col++)
    {
      if(isprint(mem->mem[start - mem->baseaddr + (line * 16) + col]) == 0)
      {
        log_dump(".");
      }
      else
      {
        log_dump("%c", mem->mem[start - mem->baseaddr + (line * 16) + col]);
      }
    }

    log_dump(" | ");

    log_dump("\n");
  }

  return 0;
}
