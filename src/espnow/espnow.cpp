#include "espnow.h"

// Địa chỉ MAC của Master - THAY BẰNG MAC THỰC TẾ CỦA MASTER
// uint8_t masterMac[6] = {0xE4, 0x65, 0xB8, 0x49, 0xCD, 0x88}; // ←←←← THAY Ở ĐÂY

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("[ESPNOW] Gửi đến Master ");
    for (int i = 0; i < 6; i++)
    {
        Serial.printf("%02X", mac_addr[i]);
        if (i < 5)
            Serial.print(":");
    }
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? " → Thành công" : " → Thất bại");
}

void espnowInit()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("[ESPNOW] Init failed");
        return;
    }

    esp_now_register_send_cb(OnDataSent);

    // Thêm peer unicast (Master)
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, masterMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("[ESPNOW] Add peer failed");
        return;
    }

    Serial.print("[ESPNOW] Send to Master MAC: ");
    for (int i = 0; i < 6; i++)
    {
        Serial.printf("%02X", masterMac[i]);
        if (i < 5)
            Serial.print(":");
    }
    Serial.println();
}

bool espnowSend(const void *data, uint8_t size)
{
    esp_err_t result = esp_now_send(masterMac, (uint8_t *)data, size);
    return (result == ESP_OK);
}