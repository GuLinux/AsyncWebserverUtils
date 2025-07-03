#ifndef GULINUX_ASYNC_WEBSERVER_BASE
#define GULINUX_ASYNC_WEBSERVER_BASE

#include <ESPAsyncWebServer.h>
#include <unistd.h>
#include <AsyncJson.h>
#include "jsonwebresponse.h"
#include <functional>

class AsyncWebServerBase {
public:
    AsyncWebServerBase(uint16_t port = 80);
protected:
    using JsonResponseCustomiser = std::function<void(AsyncWebServerRequest*, JsonWebResponse&)>;
    AsyncWebServer webserver;
    void setupElegantOTA();
    void setupCors();
    
    void setupJsonNotFoundPage(const JsonResponseCustomiser customiser={});
    void onJsonRequest(const char *path, ArJsonRequestHandlerFunction f, WebRequestMethodComposite = HTTP_POST | HTTP_PUT | HTTP_PATCH);
};
#endif