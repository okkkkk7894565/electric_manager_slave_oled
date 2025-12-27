#pragma once
#include "espnow.h"
#include <esp_now.h>
#include <WiFi.h>
#include "../include/config.h"
#include "pzem/pzem.h"

void espnowInit();
bool espnowSend(const void* data, uint8_t size);