#include "pzem.h"
#include "../oled/oled.h"
#include "config.h"
#include <Arduino.h>

// SoftwareSerial SerialPZEM(PZEM_SOFT_RX_PIN, PZEM_SOFT_TX_PIN); // RX, TX (TX không dùng)
PZEM004Tv30 pzem(Serial1, PZEM_HARD_RX_PIN, PZEM_HARD_TX_PIN);
metterData sensorData;
float cospi_slave;

void pzemInit()
{
    Serial1.begin(9600);
    sensorData.chipId = SELF_CHIP_ID;
    sensorData.sensor_type = PZEM_SENSOR_TYPE;
    sensorData.threshold = DEFAULT_THRESHOLD;
    sensorData.device_status = 0;
    Serial.println("PZEM initialized on UART1");
}

void pzemReadAndUpdate()
{
    // float v = pzem.voltage();
    // float i = pzem.current();
    // float p = pzem.power();
    // float e = pzem.energy();
    // float frequency = pzem.frequency();

    // if (isnan(v))
    //     v = -1;
    // if (isnan(i))
    //     i = -1;
    // if (isnan(p))
    //     p = -1;
    // if (isnan(e))
    //     e = -1;
    // if (isnan(frequency))
    //     frequency = -1;

    // float pf = pzem.pf();
    // if (isnan(pf))
    //     pf = 0.0;

    sensorData.volt = (uint16_t)(pzem.voltage());
    sensorData.ampe = pzem.current();
    cospi_slave = pzem.pf();
    sensorData.wat = (uint16_t)pzem.power();
    sensorData.frequency = (uint8_t)pzem.frequency();
    sensorData.kwh = pzem.energy();

    // Over power bit
    if (sensorData.wat > DEFAULT_THRESHOLD)
    {
        sensorData.device_status |= (1 << 0);
        digitalWrite(RELAY_PIN, HIGH); // Tắt tải
    }
    else
    {
        sensorData.device_status &= ~(1 << 0);
        digitalWrite(RELAY_PIN, LOW);
    }
}

void pzemDisplayHome()
{
    oledClear();

    // ================== PHẦN VÀNG TRÊN: V siêu to ==================
    // display.setTextSize(2); // Font lớn cho phần vàng
    // display.setTextColor(SSD1306_WHITE);
    // display.setCursor(10, 8); // Căn giữa phần vàng (16px cao)
    // display.print("V:");
    // display.print(sensorData.volt / 10.0, 1);
    // display.print("V");

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(2, 0);
    display.printf("V:%.1fV",(float)sensorData.volt );

#define y1 18
#define y2 29
#define y3 42

    display.setTextSize(1); // Font nhỏ để vừa màn hình
    // Dòng y1: P và I
    display.setCursor(0, y1);
    display.print("P:");
    display.print(sensorData.wat, 1);
    display.print("W");

    display.setCursor(64, y1);
    display.print("I:");
    display.print(sensorData.ampe, 2);
    display.print("A");

    // Dòng y2: F và E
    display.setCursor(0, y2);
    display.print("F:");
    display.print((float)sensorData.frequency, 0);
    display.print("Hz");

    display.setCursor(64, y2);
    display.print("E:");
    display.print(sensorData.kwh, 3);
    display.print("kWh");

    // Dòng y3: T và pF
    display.setCursor(0, y3);
    display.print("T:");
    display.print(sensorData.threshold);
    display.print("W");

    // // Dòng 56: Status - cuối cùng
    // display.setCursor(0, 56);
    // display.print("St: ");
    // for (int i = 7; i >= 0; i--)
    // {
    //     display.print((sensorData.device_status >> i) & 1);
    // }

    oledDisplay();
}

void pzemResetEnergy()
{
    pzem.resetEnergy();
}