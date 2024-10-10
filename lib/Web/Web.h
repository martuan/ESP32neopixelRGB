#ifndef Web_h
#define Web_h

#include "ESPAsyncWebServer.h"


extern AsyncWebServer server;
extern boolean takeNewPhoto;
extern boolean fotoContinua;

void urls(void);
void notFound(AsyncWebServerRequest);
void convertirParametrosRecibidos(String, String, int*, int*);



#endif