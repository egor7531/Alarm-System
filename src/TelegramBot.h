#pragma once

#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include "Config.h"

class TelegramNotifier 
{
  private:
    WiFiClientSecure client;
    UniversalTelegramBot bot{BOT_TOKEN, client};

  public:
    void begin()
    {
      client.setInsecure();
    }
    void send_message(const String& msg)
    {
      Serial.println("Попытка отправить сообщение в Telegram...");
      bot.sendMessage(CHAT_ID, msg, "HTML");
    }
};