<h1 align="center">Alarm System</h1>

## About the project
**Smart Lock** is an ESP32—based IoT device that allows you to control the door lock via a Telegram bot, automatically open it for authorized Bluetooth devices and detect suspicious activity using a motion sensor.

Specifically, our project is a box that we made using laser cutting. It opens/closes using our pinene-based mechanical lock and a toothed rack printed on a 3D printer, controlled by a Micro Servo. The lock movement occurs when the device is identified based on BLE RSSI (activation occurs within a radius of 1 meter). An IR sensor also works in parallel, which reads the activity near the device. All interaction takes place through a specially designed Bot, which receives information (about movement, closing and opening the lock), as well as commands to switch to automatic (BLE)

![Device](https://github.com/egor7531/Alarm-System/blob/main/images/picture.png)
![Lock](https://github.com/egor7531/Alarm-System/blob/main/images/lock.JPG)
