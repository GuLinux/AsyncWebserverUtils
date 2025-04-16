#pragma once
#include "commons.h"
#include "webvalidation.h"

using ::testing::StrEq;
using ::testing::InSequence;
using ::testing::Mock;

TEST(WebValidation, itShouldBeValidByDefault) {
    AsyncWebServerRequest mockRequest;
    WebValidation validation{&mockRequest, JsonObject()};
    EXPECT_TRUE(validation.valid());
}

TEST(WebValidation, itShouldCallJsonResponseError) {
    AsyncWebServerRequest mockRequest;
    EXPECT_CALL(*JsonResponse::mock, error(400, String("Missing required parameter: `key`"), &mockRequest)).Times(1);
    WebValidation{&mockRequest, JsonObject()}
        .required<int>("key");
    Mock::VerifyAndClear(JsonResponse::mock);
}

class InvalidCallback {
public:
    MOCK_METHOD(void, call, (const char *));
};

TEST(WebValidation, itShouldCallJsonResponseErrorAfterInvalidCallback) {
    AsyncWebServerRequest mockRequest;
    InvalidCallback invalidCallback;
    {
        InSequence s;
        EXPECT_CALL(invalidCallback, call(StrEq("Missing required parameter: `key`"))).Times(1);
        EXPECT_CALL(*JsonResponse::mock, error(400, String("Missing required parameter: `key`"), &mockRequest)).Times(1);
    }
    
    WebValidation{&mockRequest, JsonObject()}
        .ifInvalid(std::bind(&InvalidCallback::call, &invalidCallback, std::placeholders::_1))
        .required<int>("key");
    Mock::VerifyAndClear(&invalidCallback);
    Mock::VerifyAndClear(JsonResponse::mock);
}

