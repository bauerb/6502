#include <stdio.h>

#include "util/log.h"
#include "util/unit.h"

#include "core/memory.h"

int memory_t0001()
{
  struct Memory *mem = NULL;

  log_trace("Create memory");
  mem = memory_create(32768, 1);
  ASSERT("Failed to create memory", mem!=NULL);

  log_trace("Destroy memory");
  memory_destroy(&mem);
  ASSERT("Failed to destroy memory", mem==NULL);

  return 0;
}

int main()
{
  UNIT_TEST_INIT("Memory");

  RUN_TEST(memory_t0001, "Create and destroy Memory structure");

  UNIT_TEST_ERG();

  return 0;
}
