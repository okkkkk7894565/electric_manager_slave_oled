#include <Arduino.h>
#include "../include/config.h"
#include "oled/oled.h"
#include "menu/menu.h"
#include "pzem/pzem.h"
#include "espnow/espnow.h" // ← Thêm

bool lastButtonState = HIGH;
unsigned long pressTime = 0;
bool isHolding = false;
unsigned long lastPzemUpdate = 0;
unsigned long lastEspnowSend = 0; // Thời gian gửi ESP-NOW

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    { // Đợi USB CDC sẵn sàng - RẤT QUAN TRỌNG!
        delay(10);
    }
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    oledInit();
    pzemInit();
    espnowInit(); // ← Khởi tạo ESP-NOW
    menuInit();
    pzemReadAndUpdate();
    pzemDisplayHome();
}

void loop()
{
    bool btn = digitalRead(BUTTON_PIN);

    // Xử lý nút bấm (giữ nguyên)
    if (lastButtonState == HIGH && btn == LOW)
    {
        pressTime = millis();
        isHolding = false;
    }
    if (btn == LOW && !isHolding && millis() - pressTime >= LONG_PRESS_TIME)
    {
        isHolding = true;
        menuHandleButton(false, false, true);
    }
    if (lastButtonState == LOW && btn == HIGH)
    {
        if (!isHolding)
        {
            menuHandleButton(false, true, false);
        }
        isHolding = false;
    }
    lastButtonState = btn;

    // Cập nhật PZEM và hiển thị HOME
    if (screenState == SCREEN_HOME && millis() - lastPzemUpdate >= PZEM_UPDATE_MS)
    {
        pzemReadAndUpdate();
        pzemDisplayHome();
        lastPzemUpdate = millis();
    }

    // Gửi dữ liệu qua ESP-NOW định kỳ
    if (millis() - lastEspnowSend >= ESPNOW_SEND_INTERVAL)
    {
        if (espnowSend(&sensorData, sizeof(sensorData)))
        {
            Serial.println("[MAIN] ESP-NOW gửi thành công đến Master");
        }
        else
        {
            Serial.println("[MAIN] ESP-NOW gửi thất bại");
        }
        lastEspnowSend = millis();
    }
}