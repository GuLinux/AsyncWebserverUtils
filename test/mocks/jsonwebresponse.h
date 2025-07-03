#pragma once
#include "../commons.h"
#include "jsonresponse.h"
#include "ESPAsyncWebServer.h"
class JsonWebResponseMock;

class JsonWebResponse {
public:
    static JsonWebResponse error(JsonResponse::Status status, const String &errorMessage, AsyncWebServerRequest *request, int statusCode=-1); 
    static JsonWebResponseMock *mock;
};

class JsonWebResponseMock{
public:
    MOCK_METHOD(JsonWebResponse, error, (JsonResponse::Status status, const String &errorMessage, AsyncWebServerRequest *request, int statusCode));
};

JsonWebResponse JsonWebResponse::error(JsonResponse::Status status, const String &errorMessage, AsyncWebServerRequest *request, int statusCode) {
    return mock->error(status, errorMessage, request, statusCode);
}