#pragma once

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Config.h"

class TelegramNotifier {
public:
    void begin() {
        // ничего инициализировать не нужно
    }

    void send_message(const String& msg) {
        static unsigned long lastSend = 0;
        if (millis() - lastSend < 1000) {
            Serial.println("⏱️ Можно отправлять не чаще 1 раза в секунду");
            return;
        }
        lastSend = millis();

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("❌ Wi-Fi не подключён");
            return;
        }

        Serial.println("📡 Попытка отправить сообщение в Telegram...");

        // URL-encode текста
        String encodedMsg;
        encodedMsg.reserve(msg.length() * 3);
        for (size_t i = 0; i < msg.length(); i++) {
            char c = msg.charAt(i);
            if (isalnum(c) || c=='-'||c=='_'||c=='.'||c=='~') {
                encodedMsg += c;
            } else {
                encodedMsg += '%';
                char buf[3];
                sprintf(buf, "%02X", (uint8_t)c);
                encodedMsg += buf;
            }
        }

        String requestUrl = String("https://api.telegram.org/bot") + BOT_TOKEN
                          + "/sendMessage?chat_id=" + String(CHAT_ID)
                          + "&parse_mode=HTML&text=" + encodedMsg;

        Serial.print("🔗 URL: ");
        Serial.println(requestUrl);

        // используем WiFiClientSecure для HTTPS
        WiFiClientSecure secureClient;
        secureClient.setInsecure();

        HTTPClient http;
        http.begin(secureClient, requestUrl);
        int httpCode = http.GET();

        Serial.print("📶 HTTP-статус: ");
        Serial.println(httpCode);

        if (httpCode > 0) {
            String payload = http.getString();
            Serial.print("📨 Ответ Telegram API: ");
            Serial.println(payload);
        } else {
            Serial.print("❌ Ошибка HTTP-запроса: ");
            Serial.println(http.errorToString(httpCode).c_str());
        }
        http.end();
    }
};
