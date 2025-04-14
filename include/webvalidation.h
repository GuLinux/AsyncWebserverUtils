#include "validation.h"
#include <ESPAsyncWebServer.h>
#include "jsonresponse.h"

class WebValidation : public Validation {
public:
    WebValidation(AsyncWebServerRequest *request, JsonVariant &json, int status=400) : Validation(json), request(request), _status{status} {
    }

    ~WebValidation() {
        if(!valid()) {
            JsonResponse::error(_status, errorMessage(), request);
        }
    }

    int status() const {
        return _status;
    }
private:
    AsyncWebServerRequest *request;
    int _status = 400;
};