// Import of Libraries
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>
//-------------------------------------------------


// Global Settings
#define LED_PIN     14 // Defines the Data-Input Pin
#define LED_COUNT    8
#define BRIGHTNESS 75


// https://www.uuidgenerator.net/ to get a unique UUID

#define SERVICE_UUID        "fcf88392-048b-11ea-8d71-362b9e155667"
#define CONTROL_CHAR_UUID   "fcf88b58-048b-11ea-8d71-362b9e155667"
#define LED_MODE_CHAR_UUID  "93783900-051a-11ea-8d71-362b9e155667"

#define DEVICE_NAME "Name of Bubble"


// Global Objects & Variables

BLEDevice           Device;
BLEServer*          pServer;
BLEService*         pService;
BLECharacteristic*  pControlCharacteristic;
BLECharacteristic*  pLEDModeCharacteristic;
BLEAdvertising*     pAdvertising;

uint32_t            colormode;
std::string         ledMode = "rainbow";
std::string         powerMode = "2";


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


// Callbacks Classes
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServerCallback) {
      Serial.println("Client connected");
    }

    void onDisconnect(BLEServer* pServerCallback) {
      Serial.println("Client disconnected");
    }
};

class ControlCharacteristicCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pControlCharacteristic->getValue().length() > 0) {
        powerMode = pControlCharacteristic->getValue();
        Serial.println(powerMode.c_str());
      }
    }
};

class LEDModeCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pLEDModeCharacteristic->getValue().length() > 0) {
        ledMode = pLEDModeCharacteristic->getValue();
        Serial.println(ledMode.c_str());
      }
    }
};

// Method to initialize Bluetooth Low Energy
void initBLE() {

  Device.init(DEVICE_NAME); //Give your Device a custom name (Bluetooth-Name)
  pServer       = Device.createServer();
  pService      = pServer->createService(SERVICE_UUID);
  pControlCharacteristic  = pService->createCharacteristic(CONTROL_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE );
  pLEDModeCharacteristic  = pService->createCharacteristic(LED_MODE_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);


  Device.setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  pControlCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pLEDModeCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);


  pServer->setCallbacks(new ServerCallbacks());
  pControlCharacteristic->setCallbacks(new ControlCharacteristicCallbacks());
  pLEDModeCharacteristic->setCallbacks(new LEDModeCallbacks());

  pControlCharacteristic->setValue("default");
  pLEDModeCharacteristic->setValue("default");

  powerMode = pControlCharacteristic->getValue();
  ledMode = pLEDModeCharacteristic->getValue();

  pService->start();

  pAdvertising = Device.getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  Device.startAdvertising();

  Serial.print(DEVICE_NAME);
  Serial.println(" Advertised");
};

boolean check_user_request() {
  if (powerMode == "on" || powerMode == "On" || powerMode == "ON" || powerMode == "1") {
    return true;
  } else if (powerMode == "off" || powerMode == "of" || powerMode == "OFF" || powerMode == "OF" || powerMode == "0") {
    return false;
  }
}

void turn_on_leds() {

  if (ledMode == "rainbow" || ledMode == "Rainbow") {
    rainbow(5);
  } else if (ledMode == "random" || ledMode == "Random") {
    randomColor(100);
  } else if (ledMode == "red") {
    color(strip.Color(255, 0, 0));
  } else if (ledMode == "orange") {
    color(strip.Color(255, 128, 64));
  } else if (ledMode == "yellow") {
    color(strip.Color(255, 255, 0));
  } else if (ledMode == "green") {
    color(strip.Color(0, 255, 0));
  } else if (ledMode == "lime") {
    color(strip.Color(50, 250, 50));
  } else if (ledMode == "blue") {
    color(strip.Color(0, 0, 255));
  } else if (ledMode == "purple") {
    color(strip.Color(163, 73, 164));
  } else if (ledMode == "magenta") {
    color(strip.Color(226, 0, 116));
  } else { // white
    color(strip.Color(255, 255, 255));
  }
}

void turn_off_leds() {
  strip.clear();
  strip.show();
  colormode = strip.Color(0, 0, 0);
}

void randomColor(int rounds) {
  for (int i = 0; i < rounds; i++) {
    strip.setPixelColor(random(8), strip.Color(random(256), random(256), random(256)));
    strip.show();
    delay(150);
  }
  strip.clear();

  delay(2500);

}


// Rainbow
void rainbow(int rainbowLoops) {
  // Array with all Rainbow Colors in it
  uint32_t colors[7] = {strip.Color(255, 0, 0), strip.Color(255, 127, 0), strip.Color(255, 255, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255), strip.Color(75, 0, 130), strip.Color(139, 0, 255)};

  // Show the Rainbow on all LED's for 1 sec each Color
  for (int c = 0; c <= 6; c++) {
    for (int x = 0; x <= 7; x++) {
      strip.setPixelColor(x, colors[c]);
    }

    strip.show();
    delay(1000);
    strip.clear();
  }


  // Rainbow Fade - Example of AdafruitNeoPixel Library
  int fadeVal = 0, fadeMax = 100;

  for (uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops * 65536;
       firstPixelHue += 256) {

    for (int i = 0; i < strip.numPixels(); i++) {

      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
                                           255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(3);

    if (firstPixelHue < 65536) {
      if (fadeVal < fadeMax) fadeVal++;
    } else if (firstPixelHue >= ((rainbowLoops - 1) * 65536)) {
      if (fadeVal > 0) fadeVal--;
    } else {
      fadeVal = fadeMax;
    }
  }
};


// One Color on all LEDs
void color(uint32_t color) {

  for (int x = 0; x <= 7; x++) {
    strip.setPixelColor(x, color);
  }

  if (colormode != color) {
    strip.show();
  }

  colormode = color;
}


void setup() {
  // Set Baudrate to get the right output ( Serial.println(TEXT) ; Serial.print(TEXT) )
  Serial.begin(9600);

  initBLE();

  // Initialize LED Strip
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  turn_off_leds();

}

void loop() {
  if (check_user_request()) {
    turn_on_leds();
  }
  else {
    turn_off_leds();
  }
  delay(10);
}
