#ifndef BillBot_System_H
#define BillBot_System_H

#include <Arduino.h>
#include <network/internet/wifi_network.h>
#include <data/database/database.h>
#include <ArduinoJson.h>
#include <drivers/printers/printer_driver.h>
#include <drivers/qr scanner/qr_scanner_driver.h>
#include <utils/constants.h>
#include <data/local_data/local_storage.h>
#include <drivers/displays/display_driver.h>
#include <utils/dynamics.h>
#include <network/OTA/ota_update.h>

void setupSystem();

void manageSystem();

#endif