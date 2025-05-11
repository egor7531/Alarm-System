#include <WiFi.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>

#include "Config.h"
#include "BLEAuth.h"
#include "ServoLock.h"
#include "MotionSensor.h"

AsyncWebServer server(80);

BLEAuth          bleAuth;
ServoLock        lock;
MotionSensor     motionSensor;

bool prevAuth   = false;
bool deviceSent = false;

void send_to_server(String event) {
  WiFiClient client;
  HTTPClient http;  
  
  http.begin("http://172.20.10.2:5000/notify");  
  http.addHeader("Content-Type", "application/json");
  
  if (event == "open") {
    http.POST("{\"event\": \"open\"}");
  } else if (event == "close") {
    http.POST("{\"event\": \"close\"}");
  } else if (event == "motion") {
    http.POST("{\"event\": \"motion\"}");
  }

  http.end();
}

void handle_command(String action) {
  if (action == "open") lock.open();
  if (action == "close") lock.close();
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Даем время для инициализации Serial

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Попытка подключения к Wi-Fi: " + String(WIFI_SSID));

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) { // Таймаут 20 сек
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Подключено! IP");
    Serial.println("Сила сигнала: " + String(WiFi.RSSI()) + " dBm");
  } else {
    Serial.println("\n❌ Ошибка подключения! Статус: " + String(WiFi.status()));
    Serial.println("Проверьте: SSID, пароль и настройки точки доступа.");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "ESP32 Ready");
});

  
  server.on("/command", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("action")) {
      handle_command(request->getParam("action")->value());
    }
    request->send(200);
  });

  server.begin();
  bleAuth.begin();
  lock.begin();
  motionSensor.begin();
}

void loop() 
{
  bleAuth.update();
  bool currAuth = bleAuth.isAuthorized();

        if (currAuth && !prevAuth) {
            lock.open();
            Serial.println("🔓 Замок открыт");
            send_to_server("open");
        }
        // Разрешённое устройство исчезло
        else if (!currAuth && prevAuth) {
            lock.close();
            Serial.println("🔒 Замок закрыт");
            send_to_server("close");
        }

  prevAuth = currAuth;

  if(motionSensor.check_motion())
  {
     send_to_server("motion");
     Serial.println("Обнаружено движение!");
  }
  delay(1000);
}
