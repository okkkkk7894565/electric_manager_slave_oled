#include "esp32async.h"

bool readMasterMAC(uint8_t *mac)
{
    if (!SPIFFS.exists(MAC_FILE))
        return false;
    File file = SPIFFS.open(MAC_FILE, "r");
    if (!file)
        return false;
    String macStr = file.readStringUntil('\n');
    file.close();
    int values[6];
    if (6 == sscanf(macStr.c_str(), "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]))
    {
        for (int i = 0; i < 6; ++i)
            mac[i] = (uint8_t)values[i];
        return true;
    }
    return false;
}

void saveMasterMAC(String macStr)
{
    File file = SPIFFS.open(MAC_FILE, "w");
    if (file)
    {
        file.println(macStr);
        file.close();
        Serial.println("Đã lưu MAC: " + macStr);
    }
}

void startWiFiAndWeb()
{
    WiFi.softAP("EMS_MAC_SLAVE", "12345678");
    Serial.println("AP IP: " + WiFi.softAPIP().toString());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        const char* html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Configure Gateway MAC</title>
  <style>
    html, body {
      margin: 0; padding: 0;
      width: 100%; height: 100%;
      background: url('https://i.imgur.com/3Z3X4hv.jpg') no-repeat center center fixed;
      background-size: cover;
      display: flex;
      justify-content: center;
      align-items: center;
      font-family: 'Segoe UI', Tahoma, sans-serif;
    }
    .container {
      background: rgba(0,0,0,0.6);
      padding: 30px;
      border-radius: 8px;
      text-align: center;
      color: #fff;
      max-width: 320px;
      width: 90%;
    }
    h3 {
      margin-bottom: 20px;
      font-size: 1.2em;
    }
    input[type="text"] {
      width: 100%;
      padding: 10px;
      margin-bottom: 15px;
      border: none;
      border-radius: 4px;
      font-size: 1em;
    }
    input[type="submit"] {
      width: 100%;
      padding: 12px;
      font-size: 1em;
      color: #fff;
      background: #ff9800;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }
    input[type="submit"]:hover {
      background: #e68a00;
    }
  </style>
</head>
<body>
  <div class="container">
    <h3>Enter Gateway MAC Address</h3>
    <form action="/save" method="POST">
      <input 
        type="text" 
        name="mac" 
        placeholder="AA:BB:CC:DD:EE:FF"
        pattern="^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$" 
        required>
      <input type="submit" value="Save MAC">
    </form>
  </div>
</body>
</html>
)rawliteral";
        request->send(200, "text/html", html); });

    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request)
              {
        if (request->hasParam("mac", true)) {
            String mac = request->getParam("mac", true)->value();
            saveMasterMAC(mac);
            request->send(200, "text/plain", "MAC saved! Restarting...");
            configSaved = true;
        } else {
            request->send(400, "text/plain", "Bad Request");
        } });

    server.begin();

    // block until MAC is saved
    while (!configSaved)
    {
        delay(200);
    }

    delay(4000);
    ESP.restart();
}