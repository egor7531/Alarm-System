#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include "Config.h"

class ServoLock {
private:
  Servo lockServo;
  bool isUnlocked = false;
  
public:
  void begin() {
    lockServo.attach(SERVO_PIN);
    lockServo.write(SERVO_STOP);
    isUnlocked = false;
  }

  void open() 
  {
    if (!isUnlocked) 
    {
      lockServo.write(SERVO_BACK);
      delay(SERVO_DELAY);
      lockServo.write(SERVO_STOP);
      isUnlocked = true;
    }
  }

  void close() 
  {
    if (isUnlocked) 
    {
      lockServo.write(SERVO_FORWARD);
      delay(SERVO_DELAY);
      lockServo.write(SERVO_STOP);
      isUnlocked = true;
    }
  }

  bool isOpen() const 
  {
    return isUnlocked;
  }
};
