#pragma once

#include <Arduino.h>
#include "../oled/oled.h"
#include "../include/config.h"
#include "esp32async/esp32async.h"

// Trạng thái màn hình (enum)
enum ScreenState {
    SCREEN_HOME,
    SCREEN_MENU
};

// Trạng thái hiện tại (biến toàn cục để main.cpp truy cập)
extern ScreenState screenState;

// Các mục menu
enum MenuItem {
    MENU_RESET_ENERGY,
    MENU_RESET_WIFI,
    MENU_EXIT,
    MENU_TOTAL
};

// API
void menuInit();
void menuDrawHome();
void menuDrawMenu();
void menuDoAction();
void menuHandleButton(bool pressed, bool released, bool holding);