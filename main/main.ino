#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Настройки Wi-Fi
const char* ssid = "103 камера";
const char* password = "103@lol_77@:)";

// Настройки Telegram
#define BOT_TOKEN "8048733975:AAGqUOOKgbPOi2RDQQXQhwmG6MttTO0-VhI"
#define CHAT_ID "1089727938"

// Пин датчика движения
const int pirPin = 4;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Флаг для предотвращения спама
bool motionDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi подключен!");

  // Настройка HTTPS-соединения
  client.setInsecure(); // Отключаем проверку сертификата для простоты
}

void loop() {
  int currentState = digitalRead(pirPin);

  if (currentState == HIGH && !motionDetected) {
    Serial.println("Движение обнаружено!");
    bot.sendMessage(CHAT_ID, "⚠️ Обнаружено движение!", "");
    motionDetected = true;
  } 
  else if (currentState == LOW && motionDetected) {
    motionDetected = false;
    Serial.println("Движение прекратилось");
  }

  delay(100); // Задержка для стабилизации
}