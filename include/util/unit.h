#ifndef UINT_H
#define UNIT_H

#include <stdio.h>
#include "util/log.h"

static int unit_good = 0;
static int unit_failed = 0;
static int unit_counter = 0;

static char* unit_test_module = NULL;

#define UNIT_TEST_INIT(testmodule) \
  do \
  { \
    unit_test_module = testmodule; \
    log_unit("*************************************************************************************************"); \
    log_unit("Initialize Test Module: %s", unit_test_module); \
    log_unit("*************************************************************************************************"); \
  }while (0)

#define UNIT_TEST_ERG() \
  do \
  { \
    log_unit("*************************************************************************************************"); \
    log_unit("Testmodule %s: %d Tests %d OK %d Failed", unit_test_module, unit_counter, unit_good, unit_failed); \
    log_unit("*************************************************************************************************"); \
  }while (0)

#define ASSERT(message, test) \
  do \
  { \
    if(!(test)) \
    { \
      log_error(message); \
      return -1; \
    } \
  }while (0)

#define RUN_TEST(test, description) \
  do \
  { \
    int erg = 0; \
    log_unit("-------------------------------------------------------------------------------------------------"); \
    log_unit("Test %04d: %s", unit_counter, description); \
    log_unit("-------------------------------------------------------------------------------------------------"); \
    erg = test(); \
    if(erg!=0) \
    { \
      log_error("TEST FAILED"); \
      unit_failed++; \
    } \
    else \
    { \
      log_unit("TEST PASSED"); \
      unit_good++; \
    } \
    unit_counter ++; \
  } while (0)

#endif /* UNIT_H */
