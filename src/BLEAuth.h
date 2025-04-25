/*#pragma once
#include <NimBLEDevice.h>
#include "Config.h"
#include "TelegramBot.h"

class BLEAuthHandler {
private:
    NimBLEScan* pBLEScan;
    TelegramNotifier* notifier;
    int successfulAuthCount = 0;
    bool deviceFoundInCurrentScan = false;
    uint32_t lastAuthTime = 0;

    class MyScanCallbacks : public NimBLEScanCallbacks {
    private:
        BLEAuthHandler* parent;
    public:
        MyScanCallbacks(BLEAuthHandler* p) : parent(p) {}

        void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
            if (advertisedDevice->haveName()) {
                Serial.printf("[BLE] Обнаружено устройство: %s (RSSI: %ddB)\n", 
                             advertisedDevice->getName().c_str(), 
                             advertisedDevice->getRSSI());
            }
            
            if (advertisedDevice->haveName() && 
                advertisedDevice->getName() == TARGET_DEVICE_NAME) {
                int rssi = advertisedDevice->getRSSI();
                Serial.printf("[BLE] ✅ Найдено целевое устройство! RSSI: %ddB (Порог: %ddB)\n", 
                             rssi, RSSI_THRESHOLD);

                if (rssi > RSSI_THRESHOLD && !parent->deviceFoundInCurrentScan) {
                    parent->deviceFoundInCurrentScan = true;
                    parent->on_successful_auth();
                }
            }
        }
    };

public:
    BLEAuthHandler() : pBLEScan(nullptr), notifier(nullptr) {}

    void begin(TelegramNotifier* bot) {
        notifier = bot;
        NimBLEDevice::init("ESP32-BLE-Auth");
        NimBLEDevice::setPower(ESP_PWR_LVL_P9); // Максимальная мощность
        
        pBLEScan = NimBLEDevice::getScan();
        pBLEScan->setScanCallbacks(new MyScanCallbacks(this));
        pBLEScan->setActiveScan(true);
        pBLEScan->setInterval(67); // 67.5 ms
        pBLEScan->setWindow(33);   // 33.75 ms 
        pBLEScan->setDuplicateFilter(true);
        
        Serial.println("[BLE] Инициализация завершена");
    }

    void start_scan() {
        if (millis() - lastAuthTime < 10000) return; // Защита от частых сканирований
        
        deviceFoundInCurrentScan = false;
        if (pBLEScan) {
            Serial.println("[BLE] 🔍 Запуск сканирования...");
            pBLEScan->start(SCAN_TIME, false);
            pBLEScan->clearResults();
            Serial.println("[BLE] Сканирование завершено");
        }
    }

    void on_successful_auth() {
        successfulAuthCount++;
        lastAuthTime = millis();
        Serial.printf("[BLE] 🔑 Успешная попытка %d/%d\n", successfulAuthCount, AUTH_REQUIRED_COUNT);
        
        if (successfulAuthCount >= AUTH_REQUIRED_COUNT) {
            Serial.println("[BLE] 🔓 Аутентификация успешна!");
            if (notifier) {
                notifier->send_message(F("🔓 BLE аутентификация успешна! Доступ разрешён."));
            }
            successfulAuthCount = 0;
        }
    }
};