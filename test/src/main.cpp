#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

/* ===== MAC ESP8266 NHẬN ===== */
uint8_t receiverMac[] = {0xC8, 0xC9, 0xA3, 0x34, 0xF4, 0x40};

/* ===== STRUCT DATA ===== */
typedef struct __attribute__((packed))
{
    uint16_t chipId;
    uint8_t  sensor_type;
    uint16_t volt;
    float    ampe;
    uint16_t wat;
    uint8_t  frequency;
    float    kwh;
    byte     device_status;
    uint16_t threshold;
} metterData;

metterData data;

/* ===== THỜI GIAN GỬI ===== */
unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 3000; // 3 giây

/* ===== CALLBACK ===== */
void onSent(const uint8_t *mac, esp_now_send_status_t status) {
  Serial.print("ESP-NOW send: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.print("ESP32-C3 MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMac, 6);
  peer.channel = 0;
  peer.encrypt = false;

  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Add peer failed");
    return;
  }

  randomSeed(millis());
}

void loop() {
  if (millis() - lastSend >= SEND_INTERVAL) {
    lastSend = millis();

    /* ===== RANDOM DATA ===== */
    data.chipId = 0xC3C3;
    data.sensor_type = random(0, 3);
    data.volt = random(210, 241);
    data.ampe = random(5, 100) / 10.0;
    data.wat  = data.volt * data.ampe;
    data.frequency = 50;
    data.kwh += data.wat / 3600000.0;
    data.device_status = random(0, 2);
    data.threshold = random(1000, 3000);

    esp_now_send(receiverMac, (uint8_t*)&data, sizeof(data));

    Serial.println("Da gui 1 ban tin (3s)");
  }
}
