#pragma once

#include <Arduino.h>

#include "Config.h"

class MotionSensor 
{
private:
  int  sensorPin = PIR_PIN;
  bool lastState = false;
  
public:
  void begin() 
  {
    pinMode(sensorPin, INPUT);
  }

  bool check_motion() 
  {
    bool currentState = digitalRead(sensorPin);
    if (currentState && !lastState) 
    {
      lastState = true;
      Serial.println("🚨 Движение обнаружено!");
      return true;
    }
    lastState = currentState;
    return false;
  }
};