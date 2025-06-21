#include <string.h>

#include "dm_registry.h"
#include "dm_registry_config.h"
#include "modelpoint.h"
#include "unity.h"

void setUp(void) {
   DmRegistry_Points_InitAll();
}

void tearDown(void) {
}

void test_start_here(void) {
   // TEST_ASSERT_EQUAL_STRING("BATH_TEMPERATURE",
   //                          Modelpoint_Name_Get(dmRegistry_modelpoints[DMREGISTRY_POINT_BATH_TEMPERATURE]));
}
