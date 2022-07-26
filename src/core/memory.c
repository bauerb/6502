#include <stdlib.h>
#include <string.h>

#include "util/log.h"

#include "core/memory.h"

/*----------------------------------------------------------------------------*/
struct Memory* memory_create(uint32_t size, uint32_t baseaddr, uint8_t readonly)
{
  struct Memory *mem = NULL;

  log_debug("Memory: Try to allocate struct Memory");
  mem = malloc(sizeof(struct Memory));
  if(!mem)
  {
    log_error("Could not allocate memory for struct Memory");
    return NULL;
  }

  log_debug("Memory: Try to allocate 0x%04x bytes of memory", size);
  mem->mem = malloc(sizeof(uint8_t) * size);
  if(!mem->mem)
  {
    log_error("Could not allocate memory for struct Memory");
    free(mem);
    return NULL;
  }

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

  *data = mem->mem[mem->baseaddr - addr];
  log_debug("Read memory from addr 0x%04x data 0x%02x", addr, *data);

  return 0;
}

/*----------------------------------------------------------------------------*/
int memory_writeByte(struct Memory* mem, uint32_t addr, uint8_t data)
{
  log_debug("Try to read from address 0x%04x baseaddr 0x%04x size 0x%04x", addr, mem->baseaddr, mem->size);

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

  mem->mem[mem->baseaddr - addr] = data;
  log_debug("Read memory from addr 0x%04x data 0x%02x", addr, data);

  return 0;
}
