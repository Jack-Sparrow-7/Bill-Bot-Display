#include "wifi_network.h"

WiFiManager wm;

void webServerCallBack()
{
    displayStatus("Connection for Saved Wi-Fi Failed.", 2000);
    displayStatus("Connect to BILL-BOT Wi-Fi for changing System's Wi-Fi.", 100);
}

void setupWifi()
{
    wm.setWiFiAutoReconnect(true);
    wm.setWebServerCallback(webServerCallBack);

    // wm.resetSettings();

    Serial.println("Connecting to Wi-Fi...");
    displayStatus("Connecting to Wi-Fi...", 300);
    bool res = wm.autoConnect(adminWifiName.c_str(), adminWifiPassword.c_str());

    if (!res)
    {
        Serial.println("Failed to connect. Restarting ESP...");
        displayStatus("Failed to connect.\nRestarting...", 2000);
        ESP.restart();
    }
    else
    {
        Serial.println("Wi-Fi Connected!");
        displayStatus("Wi-Fi Connected!", 2000);
    }
}

bool isWifiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool isWifiNotStrong()
{
    long rssi = WiFi.RSSI();
    if (rssi < -70)
    {
        return true;
    }
    return false;
}

long getWifiStrength()
{
    return WiFi.RSSI();
}

void resetCredentials()
{
    Serial.println("Credentials Erased.");
    wm.resetSettings();
}
