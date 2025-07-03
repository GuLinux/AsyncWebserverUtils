#ifndef APB_JSON_RESPONSE_H
#define APB_JSON_RESPONSE_H

#include <ArduinoJson.h>
#include <unordered_map>

#define JSON_CONTENT_TYPE "application/json"

class JsonResponse {
public:
    enum Status {
        Ok,
        Created,
        BadRequest,
        Unauthorized,
        NotFound,
        InternalError,
    };
    JsonResponse(Status status=Ok) : _status{status} {
        static const std::unordered_map<Status, const char*> status2string {
            {Ok, "ok"}, {Created, "created"}, {BadRequest, "bad_request"}, {Unauthorized, "unauthorised"}, {NotFound, "not_found"}, {InternalError, "internal_error"},
        };
        _json["status"] = status2string.at(status);
    }

    JsonDocument &root() {
        return _json;
    }
    JsonDocument root() const {
        return _json;
    }


    ~JsonResponse() {
    }


    static JsonResponse error(Status status, const String &errorMessage) {
        JsonResponse response(status);
        response.root()["error"] = errorMessage;
        return response;
    }

    Status status() const { return _status; }
protected:
    Status _status;
    JsonDocument _json;
};

#endif
