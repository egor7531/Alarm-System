#pragma once
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

class TelegramNotifier 
{
  private:
    WiFiClientSecure client;
    UniversalTelegramBot bot{BOT_TOKEN, client};

  public:
    void begin();
    void send_message(const String& msg);
};