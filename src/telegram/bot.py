import requests
import telebot
import config
import state
from urllib3.exceptions import NewConnectionError, MaxRetryError
from requests.exceptions import RequestException

bot = telebot.TeleBot(config.TELEGRAM_TOKEN)
bot.remove_webhook()

def is_admin(message):
    return message.chat.id == config.ADMIN_CHAT_ID

def check_esp32_connection():
    """Проверяет доступность ESP32 через отдельный эндпоинт /ping"""
    try:
        response = requests.get(
            f"http://{config.ESP32_IP}/ping",  # Используем отдельный эндпоинт
            timeout=2
        )
        return response.status_code == 200 and response.text.strip() == "pong"
    except Exception as e:
        print(f"Connection check failed: {str(e)}")
        return False

@bot.message_handler(commands=['start', 'help'])
def send_welcome(message):
    if not is_admin(message):
        return
    
    esp32_online = check_esp32_connection()
    status = "🟢 онлайн" if esp32_online else "🔴 офлайн"
    mode = "автоматический" if state.get_mode() == 'auto' else "ручной"
    lock_status = "открыт" if not state.is_locked() else "закрыт"
    
    bot.send_message(
        message.chat.id,
        f"🔑 *Управление умным замком*\n\n"
        f"• Состояние ESP32: {status}\n"
        f"• Режим работы: {mode}\n"
        f"• Состояние замка: {lock_status}\n\n"
        "*Доступные команды:*\n"
        "/auto - Включить автоматический режим\n"
        "/manual - Включить ручной режим\n"
        "/open - Открыть замок (ручной режим)\n"
        "/close - Закрыть замок (ручной режим)\n"
        "/status - Показать текущее состояние",
        parse_mode='Markdown'
    )

@bot.message_handler(commands=['status'])
def status_handler(message):
    if not is_admin(message):
        return
    
    esp32_online = check_esp32_connection()
    status = "🟢 онлайн" if esp32_online else "🔴 офлайн"
    mode = "автоматический" if state.get_mode() == 'auto' else "ручной"
    lock_status = "открыт" if not state.is_locked() else "закрыт"
    
    bot.send_message(
        message.chat.id,
        f"📊 *Текущее состояние*\n\n"
        f"• ESP32: {status}\n"
        f"• Режим: {mode}\n"
        f"• Замок: {lock_status}",
        parse_mode='Markdown'
    )

@bot.message_handler(commands=['auto'])
def auto_handler(message):
    if not is_admin(message):
        return
    
    state.set_mode('auto')

    try:
        requests.get(f"http://{config.ESP32_IP}/command", 
                    params={'action': 'enableble', 'value': '1'},
                    timeout=3)
    except Exception as e:
        print(f"Ошибка включения BLE: {e}")

    bot.send_message(
        message.chat.id,
        "🤖 *Автоматический режим включен*\n\n"
        "Замок теперь управляется автоматически через BLE устройство. "
        "Команды /open и /close недоступны в этом режиме.",
        parse_mode='Markdown'
    )

@bot.message_handler(commands=['manual'])
def manual_handler(message):
    if not is_admin(message):
        return
    
    state.set_mode('manual')

    try:
        requests.get(f"http://{config.ESP32_IP}/command",
                    params={'action': 'enableble', 'value': '0'},
                    timeout=3)
    except Exception as e:
        print(f"Ошибка выключения BLE: {e}")

    bot.send_message(
        message.chat.id,
        "🛠 *Ручной режим включен*\n\n"
        "Теперь вы можете управлять замком вручную с помощью команд "
        "/open и /close.",
        parse_mode='Markdown'
    )

@bot.message_handler(commands=['open', 'close'])
def handle_lock_command(message):
    if not is_admin(message):
        return
    
    # Проверка режима
    if state.get_mode() == 'auto':
        bot.send_message(
            message.chat.id,
            "⚠️ *Команда недоступна*\n\n"
            "Сначала переключитесь в ручной режим с помощью /manual",
            parse_mode='Markdown'
        )
        return
    
    command = message.text[1:]
    action = "open" if command == "open" else "close"
    
    # Проверка текущего состояния
    if (action == "open" and not state.is_locked()) or (action == "close" and state.is_locked()):
        status = "уже открыт" if action == "open" else "уже закрыт"
        bot.send_message(message.chat.id, f"🔒 Замок {status}.")
        return
    
    # Проверка связи с ESP32
    if not check_esp32_connection():
        bot.send_message(message.chat.id, "⚠️ ESP32 недоступен! Проверьте подключение.")
        return
    
    # Отправка команды
    try:
        response = requests.get(
            f"http://{config.ESP32_IP}/command",
            params={'action': action},
            timeout=5
        )
        
        if response.status_code == 200:
            state.set_locked(action == "close")
            emoji = "🔓" if action == "open" else "🔒"
            bot.send_message(
                message.chat.id,
                f"{emoji} *Замок {'открыт' if action == 'open' else 'закрыт'}!*",
                parse_mode='Markdown'
            )
        else:
            bot.send_message(
                message.chat.id,
                f"⚠️ *Ошибка:* ESP32 вернул статус {response.status_code}",
                parse_mode='Markdown'
            )
            
    except RequestException as e:
        error_msg = "ESP32 не отвечает. Проверьте питание и сеть." if "Connection refused" in str(e) else str(e)
        bot.send_message(
            message.chat.id,
            f"⚠️ *Ошибка подключения:* {error_msg}",
            parse_mode='Markdown'
        )

if __name__ == '__main__':
    print("Telegram-бот запущен...")
    try:
        bot.polling(none_stop=True, interval=3, timeout=30)
    except Exception as e:
        print(f"Фатальная ошибка бота: {e}")