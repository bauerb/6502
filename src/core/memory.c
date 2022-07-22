#include <stdlib.h>
#include <string.h>

#include "util/log.h"

#include "core/memory.h"

/*----------------------------------------------------------------------------*/
struct Memory* memory_create(uint32_t size, uint8_t readonly)
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
  mem->baseaddr = malloc(sizeof(uint8_t) * size);
  if(!mem->baseaddr)
  {
    log_error("Could not allocate memory for struct Memory");
    free(mem);
    return NULL;
  }

  memset(mem->baseaddr, 0, size);
  mem->readonly = readonly;
  mem->size = size;

  return mem;
}

/*----------------------------------------------------------------------------*/
void memory_destroy(struct Memory **memory)
{
  if(*memory)
  {
    if((*memory)->baseaddr)
    {
      free((*memory)->baseaddr);
    }
    free(*memory);
    *memory = NULL;
  }
}

/*----------------------------------------------------------------------------*/
int memory_readByte(struct Memory* memory, uint32_t addr)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
int memory_writeByte(struct Memory* memory, uint32_t addr, uint8_t data)
{
  return 0;
}
