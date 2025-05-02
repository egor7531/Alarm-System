#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "BLEAuth.h"
#include "ServoLock.h"
#include "TelegramBot.h"

BLEAuth        bleAuth;
ServoLock      lock;
TelegramNotifier telegram;

bool prevAuth   = false;
bool deviceSent = false;

void setup() {
  Serial.begin(115200);
  // Подключаемся к Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Wi-Fi подключён!");

  bleAuth.begin();
  lock.begin();
  telegram.begin();

  // Тестовое сообщение
  telegram.send_message("Привет от ESP32!");  
}

void loop() {
  bleAuth.update();
  bool currAuth = bleAuth.isAuthorized();

  // Появилось устройство
  if (currAuth && !prevAuth) {
    lock.open();
    Serial.println("🔓 Замок открыт");
    if (!deviceSent) {
      telegram.send_message("🔓 Замок открыт авторизованным устройством");
      deviceSent = true;
    }
  }
  // Исчезло устройство
  else if (!currAuth && prevAuth) {
    lock.close();
    Serial.println("🔒 Замок закрыт");
    deviceSent = false;
  }

  prevAuth = currAuth;
  delay(100);
}
