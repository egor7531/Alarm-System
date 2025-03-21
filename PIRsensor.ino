const int PIR_PIN = 4;  // Пин, к которому подключен OUT датчика
const int LED_BUILTIN = 2;

void setup() {
  Serial.begin(115200);  // Инициализация последовательного соединения
  pinMode(PIR_PIN, INPUT);  // Настройка пина как вход
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Система запущена. Ожидание движения...");
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);  // Чтение состояния датчика

  if (motionDetected == HIGH)
  {
    Serial.println("Движение обнаружено!");  // Вывод сообщения в монитор порта
    digitalWrite(LED_BUILTIN, HIGH); // Включаем светодиод
  } 
  else 
  {
    Serial.println("Движение отсутствует.");
    digitalWrite(LED_BUILTIN, LOW);  // Выключаем светодиод
  }

  delay(1000);  // Задержка для стабилизации чтения
}