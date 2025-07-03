#include "validation.h"
#ifdef PIO_UNIT_TESTING
#include "mocks/ESPAsyncWebServer.h"
#include "mocks/jsonwebresponse.h"
#else
#include <ESPAsyncWebServer.h>
#include "jsonwebresponse.h"
#endif

class WebValidation : public Validation {
public:
    WebValidation(AsyncWebServerRequest *request, JsonVariant json, JsonResponse::Status statusOnError=JsonResponse::BadRequest) : Validation(json), _statusOnError{statusOnError} {
        Validation::ifInvalid([this, request](JsonVariant json, const char *errorMessage) {
            if(_ifInvalid) {
                _ifInvalid(json, errorMessage);
            }
            JsonWebResponse::error(_statusOnError, errorMessage, request);
        });
    }

    virtual Validation &ifInvalid(const IfInvalid &ifInvalid) {
        _ifInvalid = ifInvalid;
        return *this;
    }

    int status() const {
        return _statusOnError;
    }
private:
    JsonResponse::Status _statusOnError;
    IfInvalid _ifInvalid;
};