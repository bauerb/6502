/*
 * Copyright (c) 2022, Bernd Bauer <bernd.bauer@gmx.at>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>

#include "util/log.h"
#include "util/tools.h"
#include "core/memory.h"

static int file_exists(const char *filename);


/*----------------------------------------------------------------------------*/
int memory_loadFromFile(struct Memory* mem, uint32_t pos, char* filename, uint32_t off, uint32_t count)
{
  uint32_t size = 0;
  int i = 0;
  FILE *fp = NULL;

  if(mem==NULL)
  {
    log_error("Pointer to struct memory is NULL");
    return -1;
  }
  if(pos + count - mem->baseaddr > mem->size)
  {
    log_error("An attempt is made to write beyond the memory end.");
  }

  if((size = file_exists(filename)) == 0)
  {
    log_error("File %s does not exist", filename);
    return -1;
  }
  if(off + count > size)
  {
    log_error("An attempt is made to read beyond the file.");
    return -1;
  }

  fp = fopen(filename, "r");
  if(fp==NULL)
  {
    log_error("Could not open file %s", filename);
    return -1;
  }

  fseek(fp, off, SEEK_SET);

  for(i=0; i < count; i++)
  {
    uint8_t data = 0;
    if(fread(&data, sizeof(uint8_t), 1, fp) != 1)
    {
      log_error("Error reading file");
      fclose(fp);
      return -1;
    }
    log_set_quiet(true);
    memory_writeByte(mem, pos + i, data);
    log_set_quiet(false);
  }

  fclose(fp);

  return 0;
}

/*----------------------------------------------------------------------------*/
static int file_exists(const char *filename)
{
  struct stat buffer;
  return stat(filename, &buffer) == 0 ? buffer.st_size : 0;
}
