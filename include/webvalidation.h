#include "validation.h"
#ifdef PIO_UNIT_TESTING
#include "mocks/ESPAsyncWebServer.h"
#include "mocks/jsonresponse.h"
#else
#include <ESPAsyncWebServer.h>
#include "jsonresponse.h"
#endif

class WebValidation : public Validation {
public:
    WebValidation(AsyncWebServerRequest *request, JsonVariant json, int status=400) : Validation(json), _status{status} {
        Validation::ifInvalid([this, status, request](JsonVariant json, const char *errorMessage) {
            if(_ifInvalid) {
                _ifInvalid(json, errorMessage);
            }
            JsonResponse::error(_status, errorMessage, request);
        });
    }

    virtual Validation &ifInvalid(const IfInvalid &ifInvalid) {
        _ifInvalid = ifInvalid;
        return *this;
    }

    int status() const {
        return _status;
    }
private:
    int _status = 400;
    IfInvalid _ifInvalid;
};