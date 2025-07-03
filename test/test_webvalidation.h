#pragma once
#include "commons.h"
#include "webvalidation.h"

using ::testing::StrEq;
using ::testing::InSequence;
using ::testing::Mock;
using namespace std::placeholders;

TEST(WebValidation, itShouldBeValidByDefault) {
    AsyncWebServerRequest mockRequest;
    WebValidation validation{&mockRequest, JsonObject()};
    EXPECT_TRUE(validation.valid());
}

TEST(WebValidation, itShouldCallJsonResponseError) {
    AsyncWebServerRequest mockRequest;
    EXPECT_CALL(*JsonWebResponse::mock, error(JsonResponse::BadRequest, String("Missing required parameter: `key`"), &mockRequest, -1)).Times(1);
    WebValidation{&mockRequest, JsonObject()}
        .required<int>("key");
    Mock::VerifyAndClear(JsonWebResponse::mock);
}

class InvalidCallback {
public:
    MOCK_METHOD(void, call, (JsonVariant, const char *));
};

TEST(WebValidation, itShouldCallJsonResponseErrorAfterInvalidCallback) {
    AsyncWebServerRequest mockRequest;
    InvalidCallback invalidCallback;
    JsonVariant inputJson;
    {
        InSequence s;
        EXPECT_CALL(invalidCallback, call(inputJson, StrEq("Missing required parameter: `key`"))).Times(1);
        EXPECT_CALL(*JsonWebResponse::mock, error(JsonResponse::BadRequest, String("Missing required parameter: `key`"), &mockRequest, -1)).Times(1);
    }
    
    WebValidation{&mockRequest, inputJson}
        .ifInvalid(std::bind(&InvalidCallback::call, &invalidCallback, _1, _2))
        .required<int>("key");
    Mock::VerifyAndClear(&invalidCallback);
    Mock::VerifyAndClear(JsonWebResponse::mock);
}

