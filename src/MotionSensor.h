#pragma once

#include <Arduino.h>

class MotionSensor 
{
  private:
    int sensorPin;
    bool lastState = false;

  public:
    void begin(int pin)
    {
      sensorPin = pin;
      pinMode(sensorPin, INPUT);
    }

    bool check_motion()
    {
      bool currentState = digitalRead(sensorPin);

      if(currentState && !lastState) 
      {
        lastState = true;
        Serial.println("Движение обнаружено!");
        return true;
      }

      lastState = currentState;
      return false;
    }
};