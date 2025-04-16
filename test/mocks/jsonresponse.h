#pragma once
#include "../commons.h"
#include "ESPAsyncWebServer.h"
class JsonResponseMock;

class JsonResponse {
public:
    static JsonResponse error(int statusCode, const String &errorMessage, AsyncWebServerRequest *request); 
    static JsonResponseMock *mock;
};

class JsonResponseMock{
public:
    MOCK_METHOD(JsonResponse, error, (int status, const String &errorMessage, AsyncWebServerRequest *request));
};

JsonResponse JsonResponse::error(int statusCode, const String &errorMessage, AsyncWebServerRequest *request) {
    return mock->error(statusCode, errorMessage, request);
}