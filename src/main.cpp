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

#include <Adafruit_NeoPixel.h>
#include <defines.h>
#include "Web.h"
#include <SPIFFS.h>
#include <serialParser.h>



void leerValoresRGB(void);
void cambiarColoresRGB(void);
void accionarRuletaRGB(void);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, pinNeopixel, NEO_GRB + NEO_KHZ800);

// Replace with your network credentials
//const char *ssid = "wifi01-ei";
//const char *password = "Ax32MnF1975-ReB";

//int r,g,b = 0;
int cuenta = 0;
char flagCambiarColoresRGB = 0;
int rAnt = 0;
int gAnt = 0;
int bAnt = 0;
int tol = 10;//tolerancia para variación de los potenciómetros (medición por ADC)


void setup() {

  Serial.begin(115200);
  SerialBT.begin("ESP32-RGB");

  preferences.begin("credentials", false);
  preferences.getString(ssid); 
  preferences.getString(password);

  Serial.println("Network Credentials Saved using Preferences");

  setup_wifi();

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }


  
  pixels.begin(); 

  server.begin();

  urls();

	//pixels.setPixelColor(i, 255, 255, 255); // Brillo moderado en rojo
  accionarRuletaRGB();

}

void loop() {


  if(cambiarColoresForm == false){
    leerValoresRGB();
  }
  

  if(flagCambiarColoresRGB == 1){//si se modificaron los valores leídos
    //Serial.println("cambiarColoresRGB");
    cambiarColoresRGB();
    flagCambiarColoresRGB = 0;
  }
  

  cambioDeParametros();//chequea si hay cambio de parámetros a través de puerto serie o Bluetooth

  if(encenderRuleta == true){

    Serial.println("encenderRuleta");
    accionarRuletaRGB();
    encenderRuleta = false;

  }
  if(cambiarColoresForm == true){

    Serial.println("cambiarColoresRGB");
    cambiarColoresRGB();
    //cambiarColoresForm = false;
    
  }
  
}

void leerValoresRGB(void){

  int valRed = analogRead(pinPoteR);
  r = map(valRed, 0, 4095, 0, 255);
  int valGreen = analogRead(pinPoteG);
  g = map(valGreen, 0, 4095, 0, 255);
  int valBlue = analogRead(pinPoteB);
  b = map(valBlue, 0, 4095, 0, 255);

  //Si los valores son distintos a los tomados anteriormente (con margen de tolerancia), los muestra por serie y cambia los colores
  if((r >= rAnt + tol || r <= rAnt - tol) || (g >= gAnt + tol || g <= gAnt - tol) || (b >= bAnt + tol || b <= bAnt - tol)){

    Serial.printf("R = %i\n", r);
    Serial.printf("G = %i\n", g);
    Serial.printf("B = %i\n", b);

    //guarda nuevos valores
    rAnt = r;
    gAnt = g;
    bAnt = b;

    flagCambiarColoresRGB = 1;

  }
  

}

void accionarRuletaRGB(void){

  int pasos = 0;
  int min = 24;
  int max = 24 * 5;
  int demoraFinal = 1000;
  int demoraInicial = 25;
  int demora = 25;
  int vueltas = 0;
  float vueltasFloat = 0;
  int saltos = NUMPIXELS;
  int p = 1;
  float remanente = 0;
  float remanentePorcentaje = 0;
  int ultimoPixel = 0;



  pasos = random(min, max);
  vueltas = (int)pasos/NUMPIXELS;
  vueltasFloat = (float)pasos/(float)NUMPIXELS;

  remanente = vueltasFloat - (float)vueltas;
  remanentePorcentaje = remanente * 100;


  Serial.print("pasos = ");
  Serial.println(pasos);
  Serial.print("vueltas = ");
  Serial.println(vueltas);
  Serial.print("vueltasFloat = ");
  Serial.println(vueltasFloat);
  
  demora = demoraFinal/pasos;


  if(remanente > 0){
    vueltas = vueltas + 1;
  }


  //for(int i=0;i<NUMPIXELS;i++){//afecta a los 8 pixels
  for(int j = 0; j < vueltas; j++){
    
    if(j == (vueltas - 1)){//si está en la última vuelta
      saltos = (int)((float)NUMPIXELS * remanente);
      //Serial.println(saltos);

    }else{
      saltos = NUMPIXELS;
    }
    Serial.print("saltos = ");
    Serial.println(saltos);

    for(int i=0;i<saltos;i++){//afecta a los 8 pixels
      pixels.setPixelColor(i, 255, 0, 0); // Brillo moderado en rojo
      pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB  	  
      //demora = demoraFinal/(i+1);
      
      Serial.print("demora = ");
      Serial.println(demora);
      delay(demora);
      //demora = demora + (p * 1);//va agregando cada vez más demora. Se desacelera.
      demora = demora + 5;//va agregando cada vez más demora. Se desacelera.
      
      p++;


      pixels.setPixelColor(i, 0, 0, 0); // Apaga el LED
      ultimoPixel = i;
      pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB  	  

    }
  }

  pixels.setPixelColor(ultimoPixel, 255, 0, 0); // Deja encendido el último pixel para visualizarlo por un cierto tiempo
  pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB  	  

  delay(3000);

}

void cambiarColoresRGB(void){

  for(int i=0;i<NUMPIXELS;i++){//afecta a los 8 pixels
    pixels.setPixelColor(i, r, g, b); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    
  }

}

