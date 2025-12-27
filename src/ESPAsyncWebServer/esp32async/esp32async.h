#ifndef _ESP32ASYNC_H_
#define _ESP32ASYNC_H_
#include <Arduino.h>
#include <config.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
AsyncWebServer server(80);
#define MAC_FILE "/master.mac"
extern bool configSaved ;

bool readMasterMAC(uint8_t *mac);
void startWiFiAndWeb();
void saveMasterMAC(String macStr);

#endif // _ESP32ASYNC_H_
