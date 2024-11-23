#include "AsyncWebServerBase.h"
#include <ElegantOTA.h>
#include <ArduinoLog.h>

#define LOG_SCOPE "[AsyncWebServerBase] "

AsyncWebServerBase::AsyncWebServerBase(uint16_t port) : webserver{port} {
}

void AsyncWebServerBase::setupBase(bool useElegantOTA, bool allowCORS) {
    if(allowCORS) {
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    }
    ElegantOTA.begin(&webserver);
    ElegantOTA.onStart([this](){ Log.infoln(LOG_SCOPE "OTA Started"); });
    ElegantOTA.onProgress([this](size_t current, size_t total){
        Log.infoln(LOG_SCOPE "OTA progress: %d%%(%d/%d)", int(current * 100.0 /total), current, total);
    });
    ElegantOTA.onEnd([this](bool success){ Log.infoln(LOG_SCOPE "OTA Finished, success=%d", success); });
    Log.traceln(LOG_SCOPE "ElegantOTA setup");

}

void AsyncWebServerBase::onJsonRequest(const char *path, ArJsonRequestHandlerFunction f, WebRequestMethodComposite method) {
    auto handler = new AsyncCallbackJsonWebHandler(path, f);
    handler->setMethod(method);
    webserver.addHandler(handler);
}