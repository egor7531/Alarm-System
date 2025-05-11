#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>
#include <BLEScan.h>

#include "Config.h"

class BLEAuth 
{
public:
  void begin() 
  {
    BLEDevice::init("");
    scanner = BLEDevice::getScan();
    scanner->setActiveScan(true);
    scanner->setInterval(33);
    scanner->setWindow(32);
  }

  void update() {
    BLEScanResults* results = scanner->start(SCAN_TIME, false);
    authorized = false;

    for (int i = 0; i < results->getCount(); i++) {
      BLEAdvertisedDevice dev = results->getDevice(i);
      String addr = dev.getAddress().toString().c_str();
      int rssi = dev.getRSSI();

      if (rssi >= RSSI_THRESHOLD && dev.haveName() && dev.getName() == BLE_NAME) {
        authorized = true;
        /*if (!lastAuthorized) {
          Serial.print("✅ Обнаружено разрешённое устройство: ");
          Serial.println(addr);
        }*/
        break;
      }
    }
    scanner->clearResults();
  }

  bool isAuthorized() const   { return authorized;    }
  bool wasAuthorized() const  { return lastAuthorized;}
  void setLastAuthorized(bool s) { lastAuthorized = s;  }

private:
  BLEScan* scanner       = nullptr;
  bool     authorized    = false;
  bool     lastAuthorized= false;
};
