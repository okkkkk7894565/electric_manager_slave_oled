#include <Arduino.h>
#include "config.h"
#include "oled/oled.h"
#include "menu/menu.h"
#include "pzem/pzem.h"

bool lastButtonState = HIGH;
unsigned long pressTime = 0;
bool isHolding = false;
unsigned long lastPZEMUpdate = 0;

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    oledInit();
    pzemInit();
    menuInit();
    pzemReadAndUpdate();
    pzemDisplayHome();
}

void loop() {
    bool btn = digitalRead(BUTTON_PIN);

    if (lastButtonState == HIGH && btn == LOW) {
        pressTime = millis();
        isHolding = false;
    }

    if (btn == LOW && !isHolding && millis() - pressTime >= LONG_PRESS_TIME) {
        isHolding = true;
        menuHandleButton(false, false, true);
    }

    if (lastButtonState == LOW && btn == HIGH) {
        if (!isHolding) {
            menuHandleButton(true, true, false);
        }
        isHolding = false;
    }

    lastButtonState = btn;

    if (screenState == SCREEN_HOME && millis() - lastPZEMUpdate >= PZEM_UPDATE_MS) {
        pzemReadAndUpdate();
        pzemDisplayHome();
        lastPZEMUpdate = millis();
    }
}