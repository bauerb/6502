#ifndef TOOLS_H
#define TOOLS_H

#include "core/memory.h"

int memory_loadFromFile(struct Memory* mem, uint32_t pos, char* filename, uint32_t off, uint32_t count);

#endif /* TOOLS_H */
