#pragma once

#include <Adafruit_SSD1306.h>
#include "../include/config.h"

extern Adafruit_SSD1306 display;

void oledInit();
void oledClear();
void oledDisplay();
void oledPrint(int x, int y, const char* text);