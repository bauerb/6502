#include <stdio.h>

#include "util/log.h"

#include "core/cpu6502.h"
#include "core/memory.h"

struct Memory *ram = NULL;
struct Memory *rom = NULL;
struct CPU6502 *cpu = NULL;

static uint8_t read(uint16_t addr)
{
  uint8_t data = 0;
  struct Memory *mem = NULL;

  if(addr >= 0x0000 && addr <=0x7fff)
  {
    mem = ram;
  }
  else if(addr >= 0x8000 && addr <=0xffff)
  {
    mem = rom;
  }

  if(memory_readByte(mem, addr, &data) != 0)
  {
    log_error("Could not read address 0x%04x from memory", addr);
    return 0;
  }

  log_info("Read data 0x%02x from 0x%04x", data, addr);
  return data;
}

static void write(uint16_t addr, uint8_t data)
{
  if(addr >= 0x0000 && addr <=0x7fff)
  {
    if(memory_writeByte(ram, addr, data) != 0)
    {
      log_error("Could not write data 0x%02x to address 0x%04x", data, addr);
    }

    log_info("Write data 0x%02x to 0x%04x", data, addr);
  }
  else if(addr >= 0x8000 && addr <= 0xffff)
  {
    log_error("Could not write data 0x%02x to ROM addr 0x%04x", data, addr);
  }
}

static void init()
{
  log_info("Create Memory");
  ram = memory_create(0x8000, 0x0000, 0);
  rom = memory_create(0x8000, 0x8000, 0);

  memory_writeByte(rom, 0xFFFC, 0x00); // Set reset Vecor to 0x0200
  memory_writeByte(rom, 0xFFFD, 0x02);

  memory_writeByte(ram, 0x0200, 0xA9); // LDA 0x18
  memory_writeByte(ram, 0x0201, 0xFF); // LDA 0x18

  log_info("RAM DUMP");
  memory_dump(ram, 0x0200, 0x0220);
  log_info("ROM DUMP");
  memory_dump(rom, 0xFFF0, 0xFFFF);

  log_info("Create CPU");
  cpu = CPU6502_create(read, write);
}

static void destroy()
{
  log_info("Destroy Memory");
  memory_destroy(&ram);
  memory_destroy(&rom);
  log_info("Destroy CPU");
  CPU6502_destroy(&cpu);
}

int main()
{
  int i = 0;
  init();

  for(i=0; i<10;i++)
  {
    do
    {
      CPU6502_clock(cpu);
    }while(CPU6502_complete(cpu) == 0);

    CPU6502_dumpStatus(cpu);
  }

  destroy();

  return 0;
}
