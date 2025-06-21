#include <string.h>

#include "modelpoint.h"
#include "unity.h"

static modelpoint_t mp1;
static uint32_t mp1_storage;
static modelpoint_config_t mp1_config = {
    .name = "myModelPoint",
    .dataPtr = &mp1_storage,
    .dataSize_bytes = sizeof(uint32_t),
    .isValidOnInit = true,
    .initValPtr = &((uint32_t){5}),
};

typedef enum {
   BANANA_PEEL_GREEN,
   BANANA_PEEL_YELLOW,
   BANANA_PEEL_YELLOW_WITH_BROWN_DOTS,
   BANANA_PEEL_BROWN,
   BANANA_PEEL_MAX_COUNT
} banana_peel_t;

typedef struct {
   float fleshWeight;
   banana_peel_t peel;
} banana_t;

static modelpoint_t mp2;
static banana_t mp2_storage;
static modelpoint_config_t mp2_config = {
    .name = "bananaModelPoint", .dataPtr = &mp2_storage, .dataSize_bytes = sizeof(banana_t), .isValidOnInit = false};

void setUp(void) {
   Modelpoint_Init(&mp1, &mp1_config);

   // set a bunch of time to increment the update counter
   uint32_t dummyValue = 0;
   Modelpoint_Set(&mp1, &dummyValue);
   Modelpoint_Set(&mp1, &dummyValue);
   Modelpoint_Set(&mp1, &dummyValue);

   memset(&mp1_storage, -1, sizeof(mp1_storage));
   Modelpoint_Init(&mp1, &mp1_config);

   memset(&mp2_storage, -1, sizeof(mp2_storage));
   Modelpoint_Init(&mp2, &mp2_config);
}

void tearDown(void) {
}

void test_Init_InitialState(void) {
   // valid on init modelpoint
   TEST_ASSERT_EQUAL_STRING(mp1_config.name, Modelpoint_Name_Get(&mp1));
   TEST_ASSERT_EQUAL_UINT32(0, Modelpoint_UpdateCount_Get(&mp1));
   TEST_ASSERT_FALSE(Modelpoint_IsLocked(&mp1));
   TEST_ASSERT_TRUE(Modelpoint_IsValid(&mp1));
   uint32_t mp1Val = 0;
   Modelpoint_Get(&mp1, &mp1Val);
   TEST_ASSERT_EQUAL_UINT32(5, mp1Val);

   // invalid on init modelpoint
   TEST_ASSERT_FALSE(Modelpoint_IsValid(&mp2));
   TEST_ASSERT_EQUAL_MEMORY(((uint8_t[sizeof(banana_t)]){[0 ... sizeof(banana_t) - 1] = 0}), &mp2_storage,
                            sizeof(banana_t));
}

void test_GetName_ReturnsName(void) {
   TEST_ASSERT_EQUAL_STRING(mp1_config.name, Modelpoint_Name_Get(&mp1));
}

void test_Set_StoreInProvidedStorage(void) {
   bool result = Modelpoint_Set(&mp1, &(uint32_t){42});

   TEST_ASSERT_EQUAL_UINT32(42, mp1_storage);
   TEST_ASSERT_TRUE(result);
}

void test_Set_IncrementUpdateCounter(void) {
   Modelpoint_Set(&mp1, &(uint32_t){42});

   TEST_ASSERT_EQUAL_UINT32(1, Modelpoint_UpdateCount_Get(&mp1));
}

void test_Set_FailIfLocked(void) {
   Modelpoint_Lock(&mp1);
   bool result = Modelpoint_Set(&mp1, &(uint32_t){42});

   TEST_ASSERT_NOT_EQUAL_UINT32(42, mp1_storage);
   TEST_ASSERT_FALSE(result);

   Modelpoint_Unlock(&mp1);
   result = Modelpoint_Set(&mp1, &(uint32_t){42});

   TEST_ASSERT_EQUAL_UINT32(42, mp1_storage);
   TEST_ASSERT_TRUE(result);
}

void test_Get_FailIfNotValid(void) {
   banana_t dummyDest;
   bool result = Modelpoint_Get(&mp2, &dummyDest);
   TEST_ASSERT_FALSE(result);

   Modelpoint_Set(&mp2, &(banana_t){.fleshWeight = 100.0f, .peel = BANANA_PEEL_YELLOW_WITH_BROWN_DOTS});
   result = Modelpoint_Get(&mp2, &dummyDest);
   TEST_ASSERT_TRUE(result);
}

void test_Get_TakeFromProvidedStorage(void) {
   mp1_storage = 42;

   uint32_t result = 0;
   Modelpoint_Get(&mp1, &result);
   TEST_ASSERT_EQUAL_UINT32(42, result);
}

void test_Set_Get_CustomType(void) {
   Modelpoint_Set(&mp2, &((banana_t){.fleshWeight = 100.0f, .peel = BANANA_PEEL_YELLOW_WITH_BROWN_DOTS}));

   TEST_ASSERT_EQUAL_MEMORY(&((banana_t){.fleshWeight = 100.0f, .peel = BANANA_PEEL_YELLOW_WITH_BROWN_DOTS}),
                            &mp2_storage, sizeof(banana_t));

   banana_t result = {.fleshWeight = 0.0f, .peel = BANANA_PEEL_GREEN};
   Modelpoint_Get(&mp2, &result);
   TEST_ASSERT_EQUAL_MEMORY(&((banana_t){.fleshWeight = 100.0f, .peel = BANANA_PEEL_YELLOW_WITH_BROWN_DOTS}), &result,
                            sizeof(banana_t));
}

void test_IsLocked_ReturnLockState(void) {
   Modelpoint_Lock(&mp1);
   bool result = Modelpoint_IsLocked(&mp1);
   TEST_ASSERT_TRUE(result);

   Modelpoint_Unlock(&mp1);
   result = Modelpoint_IsLocked(&mp1);

   TEST_ASSERT_FALSE(result);
}

void test_Lock_ReturnLockStateBeforeChange(void) {
   bool result = Modelpoint_Lock(&mp1);
   TEST_ASSERT_FALSE(result);

   result = Modelpoint_Lock(&mp1);
   TEST_ASSERT_TRUE(result);
}

void test_Unlock_ReturnLockStateBeforeChange(void) {
   Modelpoint_Lock(&mp1);
   bool result = Modelpoint_Unlock(&mp1);
   TEST_ASSERT_TRUE(result);

   result = Modelpoint_Unlock(&mp1);
   TEST_ASSERT_FALSE(result);
}