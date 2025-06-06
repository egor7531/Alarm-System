#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>
#include <BLEScan.h>

#include "Config.h"

class BLEAuth 
{
private:
  BLEScan* scanner       = nullptr;
  bool     authorized    = false;
  bool      enabled      = true;
public:
  void begin() 
  {
    BLEDevice::init("");
    scanner = BLEDevice::getScan();
    scanner->setActiveScan(true);
    scanner->setInterval(33);
    scanner->setWindow(32);
  }

  void enable(bool state) {
        enabled = state;
        if(!state) {
            // Останавливаем сканирование при выключении
            if(scanner) {
                scanner->stop();
                scanner->clearResults();
            }
        }
    }

  void update() 
  {
    if(!enabled) return;

    BLEScanResults* results = scanner->start(SCAN_TIME, false);
    authorized = false;

    for (int i = 0; i < results->getCount(); i++) 
    {
      BLEAdvertisedDevice dev = results->getDevice(i);
      String addr = dev.getAddress().toString().c_str();
      int rssi = dev.getRSSI();

      if (rssi >= RSSI_THRESHOLD && dev.haveName() && dev.getName() == BLE_NAME) 
      {
        authorized = true;
        break;
      }
    }
    scanner->clearResults();
  }

  bool isAuthorized() const 
  { 
    return authorized;    
  }
};
