#pragma once

// Motion Sensor
//inline const int PIR_PIN = 4;

// Wi-Fi
inline const char* WIFI_SSID = "mipt-ng";//"103 камера";
inline const char* WIFI_PASS = "019ej6q4";//"103@lol_77@:)";

// Telegram
inline const char* BOT_TOKEN = "8048733975:AAGqUOOKgbPOi2RDQQXQhwmG6MttTO0-VhI";
inline const char* CHAT_ID   = "1089727938";

// BLE Settings
inline const int RSSI_THRESHOLD = -65;      // RSSI threshold for ~1m distance (higher = closer signal strength)
inline const uint32_t SCAN_TIME = 1;        // BLE scan duration in seconds for each cycle
inline const char* BLE_NAME = "Egor";  // <--- replace with actual allowed device address(es)

// Servo Lock Settings
inline const int SERVO_PIN     = 13;     
inline const int SERVO_STOP    = 90;
inline const int SERVO_FORWARD = 105;   
inline const int SERVO_BACK    = 80;    
inline const int SERVO_DELAY   = 1000;    