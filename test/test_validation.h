#pragma once
#include "commons.h"
#include "validation.h"

using ::testing::StrEq;

TEST(Validation, itShouldBeValidByDefault) {
    Validation validation{JsonObject()};
    EXPECT_TRUE(validation.valid());
}

TEST(Validation, itShouldExecuteCallbackWhenValid) {
    bool called = false;
    Validation::IfValid callback = [&called](JsonVariant){ called = true; };
    {
        Validation validation{JsonObject()};
        validation.ifValid(callback);
        EXPECT_TRUE(validation.valid());
    }
    EXPECT_TRUE(called);

}

TEST(Validation, itShouldBeInvalidOnRequiredKeys) {
    Validation validation{JsonObject()};
    validation.required<int>("key");
    EXPECT_FALSE(validation.valid());
    EXPECT_THAT(validation.errorMessage(), StrEq("Missing required parameter: `key`"));
}

TEST(Validation, itShouldExecuteCallbackWhenInvalid) {
    bool called = false;
    Validation::IfInvalid callback = [&called](JsonVariant, const char *){ called = true; };
    Validation{JsonObject()}
        .required<int>("key")
        .ifInvalid(callback);
    EXPECT_TRUE(called);

}