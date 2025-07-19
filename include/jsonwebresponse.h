#ifndef APB_JSON_WEB_RESPONSE_H
#define APB_JSON_WEB_RESPONSE_H

#include "jsonresponse.h"
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

#define JSON_CONTENT_TYPE "application/json"

class JsonWebResponse: public JsonResponse {
public:
    JsonWebResponse(AsyncWebServerRequest *request, Status status=Ok, int statusCode=-1)
        : JsonResponse{status}, _request{request}, _statusCode{statusCode} {
        static const std::unordered_map<Status, int> status2code {
            {Ok,200}, {Created,201}, {BadRequest,400}, {Unauthorized,401}, {NotFound, 404}, {InternalError,500},
        };
        if(statusCode <= 0) {
            _statusCode = status2code.at(status);
        }
    }

    JsonWebResponse(AsyncWebServerRequest *request, const JsonResponse &jsonResponse, int statusCode=-1) : JsonWebResponse(request, jsonResponse.status(), statusCode) {
        _json = jsonResponse.root();
    }

    ~JsonWebResponse() {
        AsyncResponseStream *response = _request->beginResponseStream(JSON_CONTENT_TYPE);
        response->setCode(_statusCode);
        serializeJson(_json, *response);
        _request->send(response);
    }


    static JsonWebResponse error(Status errorType, const String &errorMessage, AsyncWebServerRequest *request, int statusCode=-1) {
        return JsonWebResponse(request, JsonResponse::error(errorType, errorMessage), statusCode);
    }
private:
    AsyncWebServerRequest *_request;
    int _statusCode;

};

#endif
