#ifndef GULINUX_ASYNC_WEBSERVER_BASE
#define GULINUX_ASYNC_WEBSERVER_BASE

#include <ESPAsyncWebServer.h>
#include <unistd.h>
#include <AsyncJson.h>

class AsyncWebServerBase {
public:
    AsyncWebServerBase(uint16_t port = 80);
protected:
    AsyncWebServer webserver;

    void setupBase(bool useElegantOTA=true, bool allowCORS=false);

    void onJsonRequest(const char *path, ArJsonRequestHandlerFunction f, WebRequestMethodComposite = HTTP_POST | HTTP_PUT | HTTP_PATCH);
};
#endif