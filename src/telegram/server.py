# server.py
from flask import Flask, request
import requests
import config
import state

app = Flask(__name__)

@app.route('/notify', methods=['POST'])
def notify():
    data = request.get_json()
    if not data:
        return "Invalid request", 400

    event = data.get('event')
    if not event:
        return "No event specified", 400

    # Обработка события от ESP32
    if event == 'open':
        message = '🔓 Замок открыт авторизованным устройством!'
        state.set_locked(False)
    elif event == 'close':
        message = '🔒 Замок закрыт!'
        state.set_locked(True)
    elif event == 'motion':
        message = '⚠️ Обнаружено движение рядом с замком!'
    else:
        message = f'⚠️ Неизвестное событие: {event}'

    # Отправка уведомления в Telegram
    url = f"https://api.telegram.org/bot{config.TELEGRAM_TOKEN}/sendMessage"
    params = {
        'chat_id': config.ADMIN_CHAT_ID,
        'text': message
    }
    try:
        requests.get(url, params=params)
    except Exception as e:
        print("Failed to send Telegram message:", e)

    return 'OK', 200

if __name__ == '__main__':
    app.run(host=config.FLASK_HOST, port=config.FLASK_PORT)
