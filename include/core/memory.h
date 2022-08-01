/*
 * Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

struct Memory
{
  uint8_t *mem;
  uint32_t size;
  uint32_t baseaddr;
  uint8_t readonly;
};

struct Memory* memory_create(uint32_t size, uint32_t baseaddr, uint8_t readonly);
void memory_destroy(struct Memory **memory);

int memory_readByte(struct Memory* mem, uint32_t addr, uint8_t *data);
int memory_writeByte(struct Memory* mem, uint32_t addr, uint8_t data);

int memory_dump(struct Memory* mem, uint32_t start, uint32_t end);

#endif /* MEMORY_H */
