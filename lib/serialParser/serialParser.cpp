#include <serialParser.h>
//#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
Preferences preferences;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


const char *ssid = "wifi01-ei";
const char *password = "Ax32MnF1975-ReB";

void cambioDeParametros(void){

  char charParamID = ' ';
  String valorParam = "";
  int inChar = 0;
  String inString = "";
    
  
  //**** Chequeo por Serie o Bluetooth ***************
  while (Serial.available() > 0 || SerialBT.available() > 0) {

    if(Serial.available() > 0){
      inChar = Serial.read();
    }else if(SerialBT.available() > 0){
      inChar = SerialBT.read();
    }
    

    if(inChar != '\n'){
      Serial.print((char)inChar);

      inString += (char)inChar;//encola los caracteres recibidos

    }else{//si llegó el caracter de terminación
      
      Serial.print("Input string: ");
      Serial.println(inString);
      Serial.print("string length: ");
      Serial.println(inString.length());


      //obtiene el identificador
      charParamID = inString.charAt(0);
      
      Serial.println(charParamID);
      
      //obtiene el valor
      for(int i = 1; i < inString.length(); i++){
        valorParam += inString.charAt(i);
      }

      Serial.println(valorParam);

      //evalua el identificador y los parámetros enviados
      switchCaseParametros(charParamID, valorParam);
      
      //borra el contenido y lo prepara para recibir uno nuevo
      inString = "";
    
    }
  }

}

void switchCaseParametros(char charParamID, String valorParam){

  int inChar = 0;
  int index = 0;
  int valorParamLength = 0;
  int endIndex = 0;
  int modoDebug = 0;
  int consultarLecturas = 0;
  int correccionActivada = 0;
  uint8_t numSensor = 0;
  uint16_t direccion = 0;
  int scanActivado = 0;
  byte oldAddress = 0;
  byte newAddress = 0;
  int analizarLecturasCantidad = 0;
  int intercambioSensores = 0;
  int color = 0;
  String nombreSensor = "";
  String ssidStr = {};
  String passStr = {};
  
  //valorParam = 
  valorParam.replace(0x0A,'\0');//Se filtra el caracter LF
  valorParam.replace(0x0D,'\0');//Se filtra el caracter CR

  switch(charParamID){
 
    case 'W':

        Serial.println("Wifi: ");
        valorParamLength = strlen(valorParam.c_str());
        endIndex = valorParamLength;
        index = valorParam.indexOf(' ');
        ssidStr = valorParam.substring(0, index);
        ssid = ssidStr.c_str();
        Serial.println(ssid);
        //password = valorParam.substring(index + 1, endIndex - 1);
        passStr = valorParam.substring(index + 1, endIndex);
        password = passStr.c_str();
        Serial.println(password);

        //guarda config wifi en EEPROM
        preferences.begin("credentials", false);
        preferences.putString("ssid", ssid); 
        preferences.putString("password", password);

        Serial.println("Network Credentials Saved using Preferences");
        SerialBT.println("WifiConfigOK");
        setup_wifi();

    break;
    case 'I':
        Serial.println(WiFi.localIP());
        SerialBT.println(WiFi.localIP());
    break;
 
    default:
      Serial.println("Parámetro incorrecto");
    break;

  }  
}

void setup_wifi(void){

    int cuenta = 0;

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && cuenta < 10) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
        cuenta++;
    }
    
    if(cuenta == 10){
        Serial.println("No se pudo conectar a Wifi");
    }else{
        // Print ESP32 Local IP Address
        Serial.print("IP Address: http://");
        Serial.println(WiFi.localIP());
    }
}