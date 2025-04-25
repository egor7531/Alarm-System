#include <WiFi.h>

#include "Config.h"
#include "TelegramBot.h"
#include "MotionSensor.h"

TelegramNotifier notifier;
MotionSensor motionSensor;

void setup() 
{
  Serial.begin(115200);
  
  // Инициализация компонентов
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi подключен!");
  
  notifier.begin();
  motionSensor.begin(PIR_PIN);
}

void loop() 
{
  // Проверка датчика движения
  if(motionSensor.check_motion())
    notifier.send_message("⚠️ Обнаружено движение!");
  
  delay(100);
}