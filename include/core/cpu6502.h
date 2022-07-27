#ifndef CPU6502_H
#define CPU6502_H

#include <stdint.h>

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
    uint8_t SP;
    struct {
      uint8_t CARRY:1;
      uint8_t ZERO:1;
      uint8_t IRQB:1;
      uint8_t DECIMAL:1;
      uint8_t BRK:1;
      uint8_t NU:1;
      uint8_t OVERFLOW:1;
      uint8_t NEGATIVE:1;
    } PSR;
  } Reg;

  uint8_t  cycles;
  uint32_t clock_count;

  uint8_t  opcode;
  uint8_t  fetched;
  uint16_t temp;
  uint16_t addr_abs;
  uint16_t addr_rel;

  uint8_t (*read)(uint16_t address);
  void (*write)(uint16_t address, uint8_t data);
};

struct CPU6502* CPU6502_create(uint8_t (*read)(uint16_t), void (*write)(uint16_t, uint8_t));
int CPU6502_destroy(struct CPU6502 **cpu);

int CPU6502_reset(struct CPU6502 *cpu);

int CPU6502_clock(struct CPU6502 *cpu);
int CPU6502_complete(struct CPU6502 *cpu);

int CPU6502_dumpStatus(struct CPU6502 *cpu);

#endif /* CPU6502_H */
