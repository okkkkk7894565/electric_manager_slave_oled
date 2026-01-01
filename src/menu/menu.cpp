#include "menu.h"
#include "../oled/oled.h"
#include "../pzem/pzem.h"
#include "../../include/config.h"
#include <Arduino.h>

// Khai báo biến toàn cục (đã extern trong menu.h)
ScreenState screenState = SCREEN_HOME;

// Các biến nội bộ (static)
static MenuItem currentMenu = MENU_RESET_ENERGY;
static int clickCount = 0;
static unsigned long lastClickTime = 0;

void menuInit()
{
    menuDrawHome();
}

void menuDrawHome()
{
    // Gọi hiển thị dữ liệu PZEM ở màn hình chính
    pzemDisplayHome();
}

void menuDrawMenu()
{
    oledClear();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    oledPrint(0, 8, currentMenu == MENU_RESET_ENERGY ? "> XOA SO DIEN" : "  XOA SO DIEN");
    oledPrint(0, 24, currentMenu == MENU_RESET_WIFI ? "> XOA MasterMac" : "  XOA MasterMac");
    oledPrint(0, 40, currentMenu == MENU_EXIT ? "> THOAT" : "  THOAT");

    oledDisplay();
}

void menuDoAction()
{
    oledClear();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Handling Button ...");
    oledDisplay();
    delay(500);

    if (currentMenu == MENU_RESET_ENERGY)
    {
        display.setCursor(0, 35);
        display.println("Reset Energy...");
        oledDisplay();
        pzemResetEnergy(); // Gọi hàm reset energy của PZEM
        delay(1500);
        oledClear();
        screenState = SCREEN_HOME;
        menuDrawHome();
        return;
    }
    else if (currentMenu == MENU_RESET_WIFI)
    {
        display.setCursor(0, 35);
        display.println("Delete MAC Master...");
        oledDisplay();
        resetMasterMAC(); // Xóa MAC master trong Preferences
        oledClear();
        display.setCursor(0, 20);
        display.println("MAC Address Deleted");
        display.setCursor(0, 35);
        display.println("Restarting...");
        oledDisplay();
        delay(1500);
        oledClear();
        delay(1500);
        ESP.restart(); // Restart để mở WiFiManager config lại
    }
    else if (currentMenu == MENU_EXIT)
    {
        screenState = SCREEN_HOME;
        menuDrawHome();
        return;
    }

    // Sau khi thực hiện xong action (trừ exit), quay lại menu
    menuDrawMenu();
}

void menuHandleButton(bool pressed, bool released, bool holding)
{
    // Xử lý giữ nút lâu → thực hiện action
    if (holding && screenState == SCREEN_MENU)
    {
        menuDoAction();
        return;
    }

    // Xử lý bấm ngắn (nhả nút)
    if (released && !holding)
    {
        if (screenState == SCREEN_HOME)
        {
            // Đếm click để vào menu (3 lần bấm ngắn)
            if (millis() - lastClickTime > CLICK_TIMEOUT)
            {
                clickCount = 0;
            }
            clickCount++;
            lastClickTime = millis();

            if (clickCount >= 3)
            {
                screenState = SCREEN_MENU;
                clickCount = 0;
                currentMenu = MENU_RESET_ENERGY; // Reset vị trí menu
                menuDrawMenu();
            }
        }
        else if (screenState == SCREEN_MENU)
        {
            // Chuyển mục menu
            currentMenu = (MenuItem)((currentMenu + 1) % MENU_TOTAL);
            menuDrawMenu();
        }
    }
}