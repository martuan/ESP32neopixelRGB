#include <BluetoothSerial.h>
#include <WiFi.h>
#include <Preferences.h>

extern const char *ssid;
extern const char *password;
extern BluetoothSerial SerialBT;
extern Preferences preferences;

void cambioDeParametros(void);
void switchCaseParametros(char, String);
void setup_wifi(void);

