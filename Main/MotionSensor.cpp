#include "MotionSensor.h"
#include <Arduino.h>

void MotionSensor::begin(int pin) 
{
  sensorPin = pin;
  pinMode(sensorPin, INPUT);
}

bool MotionSensor::check_motion() 
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