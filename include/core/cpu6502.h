/*
 * Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CPU6502_H
#define CPU6502_H

#include <stdint.h>

#include "core/bus.h"

struct CPU6502
{
  struct {
    uint8_t A;
    uint8_t Y;
    uint8_t X;
    union {
      struct
      {
        uint8_t PCL;
        uint8_t PCH;
      };
      uint16_t PC;
    };
    uint16_t PC_old;
    uint8_t SP;
    union {
      struct {
        uint8_t CARRY:1;
        uint8_t ZERO:1;
        uint8_t IRQB:1;
        uint8_t DECIMAL:1;
        uint8_t BRK:1;
        uint8_t NU:1;
        uint8_t OVERFLOW:1;
        uint8_t NEGATIVE:1;
      };
      uint8_t PSR;
    };
  } Reg;

  uint8_t  cycles;
  uint32_t clock_count;

  uint8_t  opcode;
  uint8_t  fetched;
  uint16_t addr_abs;
  uint16_t addr_rel;

  struct Bus* bus;

  uint8_t (*read)(struct Bus *bus, uint16_t address);
  void (*write)(struct Bus *bus, uint16_t address, uint8_t data);
};

struct CPU6502* CPU6502_create(struct Bus* bus, uint8_t (*read)(struct Bus*, uint16_t), void (*write)(struct Bus*, uint16_t, uint8_t));
int CPU6502_destroy(struct CPU6502 **cpu);

int CPU6502_reset(struct CPU6502 *cpu);

int CPU6502_clock(struct CPU6502 *cpu);
int CPU6502_complete(struct CPU6502 *cpu);

int CPU6502_dumpStatus(struct CPU6502 *cpu);

#endif /* CPU6502_H */
