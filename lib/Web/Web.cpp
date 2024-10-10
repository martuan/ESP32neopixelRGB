#include "Web.h"
//#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>
#include "defines.h"

AsyncWebServer server(80);
boolean takeNewPhoto = false;
boolean fotoContinua = false;
// Photo File Name to save in SPIFFS



void urls(void){

  
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/styles.css", "text/css");
  });
 
  server.on("/test.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/test.js", "text/javascript");
  });

  server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/login.html", "text/html");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/camara", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/camara.html", "text/html");
  });

  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    takeNewPhoto = true;
    request->send_P(200, "text/plain", "Taking Photo");
  });

  server.on("/captureCont", HTTP_GET, [](AsyncWebServerRequest * request) {
    fotoContinua = true;
    request->send_P(200, "text/plain", "Taking Continuous Photo");
  });

  server.on("/stopCaptureCont", HTTP_GET, [](AsyncWebServerRequest * request) {
    fotoContinua = false;
    request->send_P(200, "text/plain", "Taking Continuous Photo");
  });

  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
  });


  
  server.on("/ESP32", HTTP_ANY, [](AsyncWebServerRequest *request){

  /*  
    Serial.println("/ESP32");

    if(request->hasArg("fname")){
      
      Serial.println("Tiene argumentos en la solicitud");

      for(int i=0;i<2;i++){
 
      AsyncWebParameter* param = request->getParam(i);
  
      Serial.print("Param name: ");
      Serial.println(param->name());
  
      Serial.print("Param value: ");
      Serial.println(param->value());
  
      Serial.println("------");

      if(param->value() == "LED_ON"){

          Serial.println("LED_ON");
          digitalWrite(LED_BUILTIN, HIGH);

      }
      if(param->value() == "LED_OFF"){

          Serial.println("LED_OFF");
          digitalWrite(LED_BUILTIN, LOW);

      }
    }
    //String param1;

    //param1 = request->getParam().value();


    //request->send(SPIFFS, "/ESP32.html", "text/html");
*/

    

    //}

    int pin, valor;

    //request->send(SPIFFS, "/ESP32.html", "text/html");
    Serial.println("/ESP32");

    //if(request->args() > 0){//chequea que tenga al menos un argumento recibido
    if(request->hasArg("GPIO1") || request->hasArg("GPIO19")){//chequea que tenga al menos un argumento recibido
    //request->args()
      
      Serial.println("Tiene argumentos en la solicitud");

      for(int i=0;i<4;i++){
 
        AsyncWebParameter* param = request->getParam(i);
    
        Serial.print("Param name: ");
        Serial.println(param->name());
    
        Serial.print("Param value: ");
        Serial.println(param->value());
    
        Serial.println("------");

        
        Serial.printf("%s = %s\n",param->name(), param->value());

        convertirParametrosRecibidos(param->name(), param->value(), &pin, &valor);


//        valor = 1;
  //      pin = 2;


        if(request->hasArg("GPIO1")){//si es escritura

          if(valor == 1){
            Serial.println("valor = 1");
            digitalWrite(pin, HIGH);  
          }
          if(valor == 0){
            digitalWrite(pin, LOW);
            Serial.println("valor = 0");
          }

        }

        if(request->hasArg("GPIO19")){//si es lectura

          
            Serial.println("Lectura");
            digitalRead(pin);  
          

        }
        
        //digitalWrite(2, HIGH);


      }
      
    }

    request->send(SPIFFS, "/ESP32.html", "text/html");

  });


  server.on("/interactuarConESP32", HTTP_ANY, [](AsyncWebServerRequest *request){

    Serial.println("/interactuarConESP32");

    if(request->hasArg("fname")){
      
      Serial.println("Tiene argumentos en la solicitud");

      for(int i=0;i<2;i++){
 
      AsyncWebParameter* param = request->getParam(i);
  
      Serial.print("Param name: ");
      Serial.println(param->name());
  
      Serial.print("Param value: ");
      Serial.println(param->value());
  
      Serial.println("------");

      if(param->value() == "LED_ON"){

          Serial.println("LED_ON");
          digitalWrite(LED_BUILTIN, HIGH);

      }
      if(param->value() == "LED_OFF"){

          Serial.println("LED_OFF");
          digitalWrite(LED_BUILTIN, LOW);

      }
    }
    //String param1;

    //param1 = request->getParam().value();


    //request->send(SPIFFS, "/ESP32.html", "text/html");


    }

/*
    for(int i=0;i<2;i++){
 
     AsyncWebParameter* param = request->getParam(i);
 
     Serial.print("Param name: ");
     Serial.println(param->name());
 
     Serial.print("Param value: ");
     Serial.println(param->value());
 
     Serial.println("------");

     if(param->value() == "LED_ON"){

        Serial.println("LED_ON");
        digitalWrite(LED_BUILTIN, HIGH);

     }
     if(param->value() == "LED_OFF"){

        Serial.println("LED_OFF");
        digitalWrite(LED_BUILTIN, LOW);

     }
    }
    //String param1;

    //param1 = request->getParam().value();


    //request->send(SPIFFS, "/ESP32.html", "text/html");
    */
    request->send(SPIFFS, "/interactuarConESP32.html", "text/html");
    //request->send(SPIFFS, "/ESP32.html", "text/html");
  });

  server.on("/consultarEntradasESP32", HTTP_ANY, [](AsyncWebServerRequest *request){

    int pin = 1;
    int lecturaPin;

    Serial.println("consultarEntradasESP32");
    Serial.println("Lectura");
    //lecturaPin = digitalRead(pin);  
    lecturaPin = analogRead(pin);  
    Serial.println(lecturaPin);

    request->send(200, "text/html", (String)lecturaPin);


  });



}

  void notFound(AsyncWebServerRequest *request) { //rutina de atención para páginas solicitadas no definidas 
      //pathDirectory = ""; //resetea el directorio para una nueva navegación
    
    Serial.print("URL = ");
    Serial.println(request->url());
    if(request->url().endsWith(".ico")){
      Serial.println("se trata de favicon.ico");
    }else{
      Serial.println("URL no encontrada: no es favicon.ico");
      Serial.println("URL no encontrada, resetea pathDirectory y redirect a /");
      //pathDirectory = ""; //resetea el directorio para una nueva navegación
      request->redirect("/");                     //Toda página solicitada no definida se redirecciona a la raíz ("/")
    }
    //Serial.println("URL no encontrada");
    //request->redirect("/");                     //Toda página solicitada no definida se redirecciona a la raíz ("/")
}


void convertirParametrosRecibidos(String paramName, String paramValue, int *pin, int *valor){

  String pinName = {};
  pinName = paramName;
  pinName.remove(0, 4);

  Serial.printf("%s\n", pinName);

  //obtiene el número de pin desde un String recibido
  *pin = pinName.toInt();


  //obtiene el valor a asignar al pin desde un String recibido (true = 1 = HIGH, false = 0 = LOW)
  if(paramValue == "true"){
    *valor = 1;
  }else if(paramValue == "false"){
    *valor = 0;
  }

  Serial.printf("%i\n", *valor);





}



