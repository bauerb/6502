#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

struct Memory
{
  uint8_t *baseaddr;
  uint32_t size;
  uint8_t readonly;
};

struct Memory* memory_create(uint32_t size, uint8_t readonly);
void memory_destroy(struct Memory **memory);

int memory_readByte(struct Memory* memory, uint32_t addr);
int memory_writeByte(struct Memory* memory, uint32_t addr, uint8_t data);

#endif /* MEMORY_H */
