#include <stdint.h>
#include <stdlib.h>

#include "util/log.h"

#include "core/acia.h"

/*----------------------------------------------------------------------------*/
struct ACIA* acia_create(uint16_t baseaddr)
{
  struct ACIA* acia = NULL;

  acia = malloc(sizeof(struct ACIA));
  if(acia == NULL)
  {
    log_error("Could not create struct ACIA");
    return NULL;
  }

  acia->data_reg = 0;
  acia->status_reg = 0;
  acia->ctrl_reg = 0;
  acia->command_reg = 0;

  acia->baseaddr = baseaddr;

  return acia;
}

/*----------------------------------------------------------------------------*/
int acia_destroy(struct ACIA **acia)
{
  if(*acia != NULL)
  {
    free(*acia);
    *acia = NULL;
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
int acia_read(struct ACIA* acia, uint16_t addr, uint8_t *data)
{
  if(addr - acia->baseaddr == 0)
  {
    *data = acia->data_reg;
    return 0;
  }
  else if(addr - acia->baseaddr == 1)
  {
    *data = acia->status_reg;
    return 0;
  }
  else if(addr - acia->baseaddr == 2)
  {
    *data = acia->ctrl_reg;
    return 0;
  }
  else if(addr - acia->baseaddr == 3)
  {
    *data = acia->command_reg;
    return 0;
  }

  log_error("ACIA: Invalid address");
  return 1;
}

/*----------------------------------------------------------------------------*/
int acia_write(struct ACIA* acia, uint16_t addr, uint8_t data)
{
  if(addr - acia->baseaddr == 0)
  {
    acia->data_reg = data;
    putchar(acia->data_reg);
    fflush(stdout);
    return 0;
  }
  else if(addr - acia->baseaddr == 1)
  {
    acia->status_reg = data;
    return 0;
  }
  else if(addr - acia->baseaddr == 2)
  {
    acia->ctrl_reg = data;
    return 0;
  }
  else if(addr - acia->baseaddr == 3)
  {
    acia->command_reg = data;
    return 0;
  }

  log_error("ACIA: Invalid address");
  return 1;
}
