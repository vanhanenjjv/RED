#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <ArduinoJson.h>

#include "freeMemory.h"

#define BAUD_RATE 114200

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (4)
#define PN532_MISO (12)

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uid_length;				             // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

void setup(void) {
  Serial.begin(BAUD_RATE);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (true);
  }

   nfc.SAMConfig();
}

void loop(void) {
  // this is blocking
  boolean success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uid_length);

  if (success) {
    // do something
    Serial.println("got match");
  }

  StaticJsonDocument<256> doc;
  doc["sram_usage"] = freeMemory();
  serializeJson(doc, Serial);
  Serial.println();
}
