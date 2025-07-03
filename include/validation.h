#ifndef APB_VALIDATION_H
#define APB_VALIDATION_H


#include <ArduinoJson.h>
#include <functional>
#include <forward_list>
#include "jsonresponse.h"

class Validation {

public:
    using IfValid=std::function<void(JsonVariant)>;
    using IfInvalid=std::function<void(JsonVariant, const char*)>;

    Validation(JsonVariant json) : _json(json) {
    }

    bool valid() const { return strlen(_errorMessage) == 0; }
    bool invalid() const { return !valid(); }
    void clear() { _errorMessage[0] = 0; }

    JsonResponse errorResponse(JsonResponse::Status statusOnError=JsonResponse::BadRequest) const {
        return JsonResponse::error(statusOnError, _errorMessage);
    }

    template<typename T> Validation &required(const char *key) {
        if(valid() && !_json[key].is<T>()) {
            sprintf(_errorMessage, "Missing required parameter: `%s`", key);
        }
        return *this;
    }

    template<typename T> Validation &required(const std::forward_list<String> &keys) {
        std::for_each(keys.begin(), keys.end(), [this](const String &key) { required<T>(key.c_str()); });
        return *this;
    }

    Validation &number(const char *key) {
        if(valid() && _json[key].is<JsonVariant>()) {
            if(!_json[key].is<float>()) sprintf(_errorMessage, "Value for `%s` is not a number", key);
        }
        return *this;
    }

    Validation &range(const char *key, const std::optional<float> &min, const std::optional<float> &max) {
        number(key);
        if(valid() && _json[key].is<float>()) {
            if(max && _json[key] > *max) sprintf(_errorMessage, "Value for `%s` greater than allowed max `%f`", key, *max);
            if(min && _json[key] < *min) sprintf(_errorMessage, "Value for `%s` lower than allowed max `%f`", key, *min);
        }
        return *this;
    }
    Validation &choice(const char *key, const std::forward_list<String> &choices) {
        if(valid() &&
            _json[key].is<const char*>() &&
            std::none_of(choices.begin(), choices.end(), [&key, this](const String &choice){ return choice == _json[key]; })) {
            String choicesMessage;
            std::for_each(choices.begin(), choices.end(), [&choicesMessage](const String &choice) {
                if(!choicesMessage.isEmpty()) choicesMessage.concat(", ");
                choicesMessage.concat(choice);
            });
            sprintf(_errorMessage, "Invalid value for `%s`. Valid choices: <%s>", key, choicesMessage.c_str());
        }
        return *this;
    }
    
    Validation &notEmpty(const char *key) {
        if(valid() && _json[key].is<const char*>() && _json[key].as<String>().isEmpty()) {
            sprintf(_errorMessage, "Parameter `%s` must not be empty", key);
        }
        return *this;
    }

    virtual Validation &ifValid(const IfValid &ifValid) {
        _ifValid = ifValid;
        return *this;
    }

    virtual Validation &ifInvalid(const IfInvalid &ifInvalid) {
        _ifInvalid = ifInvalid;
        return *this;
    }

    virtual ~Validation() {
        if(_ifValid && valid()) {
            _ifValid(_json);
        }
        if(_ifInvalid && invalid()) {
            _ifInvalid(_json, _errorMessage);
        }
    }

    JsonVariant json() {
        return _json;
    }
    const char *errorMessage() const {
        return _errorMessage;
    }
protected:
private:
    JsonVariant _json;
    IfValid _ifValid;
    IfInvalid _ifInvalid;
    char _errorMessage[512] = {0};
};

#endif
