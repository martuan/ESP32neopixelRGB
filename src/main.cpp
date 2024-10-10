/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-take-photo-display-web-server/
  
  IMPORTANT!!! 
   - Select Board "AI Thinker ESP32-CAM"
   - GPIO 0 must be connected to GND to upload a sketch
   - After connecting GPIO 0 to GND, press the ESP32-CAM on-board RESET button to put your board in flashing mode
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include "WiFi.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
#include "Web.h"
#include "camara.h"

int periodo = 5000;
unsigned long TiempoAhora = 0;




// Replace with your network credentials
const char *ssid = "wifi01-ei";
const char *password = "Ax32MnF1975-ReB";

//const char *ssid = "milton";
//const char *password = "paternal";

// Create AsyncWebServer object on port 80
//AsyncWebServer server(80);









void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(9, OUTPUT);//Digital Output
  pinMode(10, OUTPUT);//Digital Output
  pinMode(1, INPUT);//ADC CH0

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

  // Print ESP32 Local IP Address
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  // Turn-off the 'brownout detector'
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);


  Serial.println("WRITE_PERI_REG");
  // OV2640 camera module


  initCamara();

  urls();
    // Route for root / web page

  

  // Start server
  server.begin();

}

void loop() {
  if (takeNewPhoto) {
    capturePhotoSaveSpiffs();
    takeNewPhoto = false;
  }
  if(fotoContinua && (millis() > TiempoAhora + periodo)){
    TiempoAhora = millis();
    capturePhotoSaveSpiffs();

    //delay(5000);
  }
  delay(1);
}
