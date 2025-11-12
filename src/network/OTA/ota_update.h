#ifndef OTA_Update_H
#define OTA_Update_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <data/local_data/local_storage.h>
#include <utils/constants.h>
#include <utils/dynamics.h>
#include <drivers/displays/display_driver.h>
#include <network/internet/wifi_network.h>

void checkFirmwareUpdate();
void performOTA();

#endif