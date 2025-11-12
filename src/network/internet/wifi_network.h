#ifndef Wifi_Network_H
#define Wifi_Network_H
#include<Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <utils/constants.h>
#include <drivers/displays/display_driver.h>

void setupWifi();
bool isWifiConnected();
bool isWifiNotStrong();
long getWifiStrength();
void resetCredentials();

#endif