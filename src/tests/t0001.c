#include <stdio.h>

#include "util/log.h"
#include "util/unit.h"

#include "core/memory.h"

/**
 * Create Memory structure with 32k size and destroy it
 */
int memory_t0001()
{
  struct Memory *mem = NULL;

  log_unit("Create memory");
  mem = memory_create(0x8000, 0x00, 1);
  ASSERT("Failed to create memory", mem!=NULL);

  log_unit("Destroy memory");
  memory_destroy(&mem);
  ASSERT("Failed to destroy memory", mem==NULL);

  return 0;
}

/**
 * Create Memory structure with 32k size and different baseaddr and destroy it
 */
int memory_t0002()
{
  struct Memory *mem = NULL;

  log_unit("Create memory");
  mem = memory_create(0x8000, 0x8000, 1);
  ASSERT("Failed to create memory", mem!=NULL);
  ASSERT("Wrong baseaddress", mem->baseaddr==0x8000);

  log_unit("Destroy memory");
  memory_destroy(&mem);
  ASSERT("Failed to destroy memory", mem==NULL);

  return 0;
}

/**
 * Create Memory structure with 32k size and write and read from it
 */
int memory_t0003()
{
  struct Memory *mem = NULL;
  int ret = 0;
  uint8_t data = 0;

  mem = memory_create(0x8000, 0x8000, 1);
  ASSERT("Failed to create memory", mem!=NULL);

  log_unit("Write 0xff to address 0x0100");
  ret = memory_writeByte(mem, 0x0100, 0xff);
  log_unit("Read from address 0x0100");
  ret = memory_readByte(mem, 0x0100, &data);

  ASSERT("Read from memory delivers wrong value", data==0xff);

  memory_destroy(&mem);
  ASSERT("Failed to destroy memory", mem==NULL);

  return ret;
}

/**
 * Read from Memory outside address range
 */
int memory_t0004()
{
  struct Memory *mem = NULL;
  int ret = 0;
  uint8_t data = 0;

  mem = memory_create(0x8000, 0x0000, 1);
  ASSERT("Failed to create memory", mem!=NULL);

  log_unit("Write to outside memory");
  ret = memory_writeByte(mem, 0x8100, 0xff);
  ASSERT("Write to outside adress range", ret==1);

  log_unit("Read from outside memory");
  ret = memory_readByte(mem, 0x8100, &data);
  ASSERT("Read from outside adress range", ret==1);

  memory_destroy(&mem);
  ASSERT("Failed to destroy memory", mem==NULL);

  return ret;
}

int main()
{
  UNIT_TEST_INIT("Memory");

  RUN_TEST(memory_t0001, "Create and destroy Memory structure");
  RUN_TEST(memory_t0002, "Create Memory with abseaddr 0x8000");
  RUN_TEST(memory_t0003, "Write and read from memory");
  RUN_TEST(memory_t0004, "Read and write from meory outside address range");

  UNIT_TEST_ERG();

  return 0;
}
