#ifndef ACIA_H
#define ACIA_H

#include <stdint.h>

struct ACIA
{
  uint8_t data_reg;
  uint8_t status_reg;
  uint8_t ctrl_reg;
  uint8_t command_reg;

  uint16_t baseaddr;
};

struct ACIA* acia_create(uint16_t baseaddr);
int acia_destroy(struct ACIA **acia);

int acia_read(struct ACIA* acia, uint16_t addr, uint8_t *data);
int acia_write(struct ACIA* acia, uint16_t addr, uint8_t data);

#endif /* ACIA_H */
