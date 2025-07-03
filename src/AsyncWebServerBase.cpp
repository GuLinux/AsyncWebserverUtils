#include "AsyncWebServerBase.h"
#include <ElegantOTA.h>
#include <ArduinoLog.h>

#define LOG_SCOPE "[AsyncWebServerBase] "

AsyncWebServerBase::AsyncWebServerBase(uint16_t port) : webserver{port} {
}

void AsyncWebServerBase::onJsonRequest(const char *path, ArJsonRequestHandlerFunction f, WebRequestMethodComposite method) {
    auto handler = new AsyncCallbackJsonWebHandler(path, f);
    handler->setMethod(method);
    webserver.addHandler(handler);
}

void AsyncWebServerBase::setupElegantOTA() {
    ElegantOTA.begin(&webserver);
    ElegantOTA.onStart([this](){ Log.infoln(LOG_SCOPE "OTA Started"); });
    ElegantOTA.onProgress([this](size_t current, size_t total){
        Log.infoln(LOG_SCOPE "OTA progress: %d%%(%d/%d)", int(current * 100.0 /total), current, total);
    });
    ElegantOTA.onEnd([this](bool success){ Log.infoln(LOG_SCOPE "OTA Finished, success=%d", success); });
}

void AsyncWebServerBase::setupCors() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
}

void AsyncWebServerBase::setupJsonNotFoundPage(const JsonResponseCustomiser customiser) {
    webserver.onNotFound([customiser](AsyncWebServerRequest *request){
        JsonWebResponse response(request, JsonResponse::Status::NotFound);
        response.root()["error"] = "NotFound";
        response.root()["url"] = request->url();
        if(customiser) {
            customiser(request, response);
        }
    });
}
