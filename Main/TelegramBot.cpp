#include "Config.h"
#include "TelegramBot.h"

void TelegramNotifier::begin() 
{
  client.setInsecure(); // Отключаем проверку SSL
}

void TelegramNotifier::send_message(const String& msg) 
{
  Serial.println("Попытка отправить сообщение в Telegram...");
  bot.sendMessage(CHAT_ID, msg, "HTML");
}