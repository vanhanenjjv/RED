#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <ArduinoJson.h>

#define BAUD_RATE 114200

#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

bool nfc_enabled = true;

void setup(void) {
  Serial.begin(BAUD_RATE);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if (!versiondata) {
    Serial.println("Didn't find PN53x board");
    while (1); // halt
  }
  
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.');              Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0x00);

  nfc.SAMConfig();
}

void loop(void) {
  if (nfc_enabled) {
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
    uint8_t uid_length;				               // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uid_length)) {
      StaticJsonDocument<256> doc;

      String uid_str;

      for (int i = 0; i < uid_length; ++i) 
        uid_str.concat(uid[i]);

      doc["type"] = "CARD";
      doc["uid"]  = uid_str;

      serializeJson(doc, Serial);
      Serial.println();
    }
  }

  while (Serial.available()) {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, Serial);

    int type = doc["type"].as<int>();

    switch (type) {
       case 0: {
        nfc_enabled = false;
        break;
      }
      case 1: {
        nfc_enabled = true;
        break;
      }
    }
  }
}
