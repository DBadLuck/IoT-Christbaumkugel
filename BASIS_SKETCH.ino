// Import of Libraries
#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
//-------------------------------------------------


// Global Settings
#define LED_PIN     14 // Defines the Data-Input Pin
#define LED_COUNT    8 // Amount of LEDs on a Stripe

//Generate unique UUIDs on https://www.uuidgenerator.net/ for every part of a Profile

#define SERVICE_UUID        "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
#define COMMAND_CHAR_UUID   "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"

//-------------------------------------------------

// Global Objects & Variables
BLEDevice           Device;   
BLEServer*          pServer;
BLEService*         pService;
BLECharacteristic*  pCharCommand;
BLECharacteristic*  pCharCommand2;
BLECharacteristic*  pCharCommand3;
BLEAdvertising*     pAdvertising;

Adafruit_NeoPixel stripe(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); 
//-------------------------------------------------


// The setup function is executed only once in the beginning
void setup() {
  // Set Baudrate to get the right output ( Serial.println(TEXT) ; Serial.print(TEXT) )
  Serial.begin(9600);

}

// After the setup function was executed, the loop runs until the microcontroller is resetted or the power was removed
void loop() {

}

// Server Callbacks
class ServerCallbacks : public BLEServerCallbacks {

};

// Characteristic Callbacks
class CharacteristicCallbacks: public BLECharacteristicCallbacks {

};
