#pragma once

class MotionSensor 
{
  private:
    int sensorPin;
    bool lastState = false;

  public:
    void begin(int pin);
    bool check_motion();
};