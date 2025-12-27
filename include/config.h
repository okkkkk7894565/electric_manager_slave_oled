#pragma once

/* ================== CHÂN PHẦN CỨNG - ESP32-C3 ================== */

// I2C cho OLED SSD1306 (theo yêu cầu của bạn)
#define SDA_PIN         8       // GPIO8  - SDA
#define SCL_PIN         9       // GPIO9  - SCL
#define BUTTON_PIN      4       // GPIO4  - nút bấm
#define RELAY_PIN       2       // GPIO2  - relay điều khiển tải

#define OLED_ADDR       0x3C    // Địa chỉ I2C OLED (thường 0x3C, nếu không hiển thị thử 0x3D)
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1      // Không dùng chân reset

// PZEM-004T dùng SoftwareSerial (chọn pin tự do)
#define PZEM_HARD_RX_PIN    20  // GPIO6 - nhận dữ liệu từ TX của PZEM (bạn có thể đổi thoải mái)
#define PZEM_HARD_TX_PIN    21  // Không dùng TX → có thể để bất kỳ pin nào, hoặc -1

/* ================== THỜI GIAN ================== */
#define LONG_PRESS_TIME 3000    // Giữ nút 3 giây để thực hiện action
#define CLICK_TIMEOUT   800     // Timeout reset đếm click (ms)
#define PZEM_UPDATE_MS  800    // Cập nhật dữ liệu PZEM mỗi 5 giây
#define ESPNOW_SEND_MS  5000    // Gửi gói ESP-NOW mỗi 5 giây

/* ================== THÔNG TIN THIẾT BỊ ================== */
#define SELF_CHIP_ID    (uint16_t)(ESP.getEfuseMac() >> 32)  // Lấy 16 bit cao của MAC làm ID
#define PZEM_SENSOR_TYPE 0      // Loại cảm biến PZEM theo code gốc của bạn
#define DEFAULT_THRESHOLD 1000  // Ngưỡng công suất mặc định (W)

// WiFiManager config
#define AP_NAME         "PZEM_C3_CONFIG"
#define PREFS_NAMESPACE "pzem_slave"
#define MAC_KEY         "master_mac"