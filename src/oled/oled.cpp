#include "oled.h"
#include <Wire.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void oledInit() {
    Wire.begin(SDA_PIN, SCL_PIN);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("OLED init failed!"));
        while (1);
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    oledDisplay();
    Serial.println("OLED initialized.");
}

void oledClear() { display.clearDisplay(); }
void oledDisplay() { display.display(); }
void oledPrint(int x, int y, const char* text) {
    display.setCursor(x, y);
    display.print(text);
}