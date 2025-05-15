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
unsigned long previousMillis = 0;
const long interval = 5000;

void send_to_server(String event) 
{
  WiFiClient client;
  HTTPClient http;  
  
  http.begin("http://172.20.10.2:5000/notify");  
  http.addHeader("Content-Type", "application/json");
  
  if (event == "open")
    http.POST("{\"event\": \"open\"}");
  else if (event == "close")
    http.POST("{\"event\": \"close\"}");
  else if (event == "motion")
    http.POST("{\"event\": \"motion\"}");

  http.end();
}

void handle_command(String action) 
{
  if (action == "open") lock.open();
  if (action == "close") lock.close();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Попытка подключения к Wi-Fi: " + String(WIFI_SSID));

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000)
  {
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
  
  server.on("/command", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    if(request->hasParam("action")) 
    {
        String action = request->getParam("action")->value();
        if(action == "open" || action == "close") 
        {
            handle_command(action);
            request->send(200, "text/plain", "OK");
            return;
        }
    }
    request->send(400, "text/plain", "Bad Request");
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

  if (currAuth && !prevAuth) 
  {
      lock.open();
      Serial.println("🔓 Замок открыт");
      send_to_server("open");
  }
  else if (!currAuth && prevAuth) 
  {
      lock.close();
      Serial.println("🔒 Замок закрыт");
      send_to_server("close");
  }
  prevAuth = currAuth;

  unsigned long currentMillis = millis();
  if (!lock.isOpen() && motionSensor.check_motion()) 
  {
    if (currentMillis - previousMillis >= interval) 
    {
      send_to_server("motion");
      previousMillis = currentMillis;
    }
  }

  delay(1000);
}
