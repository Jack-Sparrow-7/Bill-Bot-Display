#ifndef DataBase_H
#define DataBase_H

#include <utils/constants.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <utils/helpers.h>
#include <data/local_data/local_storage.h>
#include <drivers/displays/display_driver.h>
#include <network/internet/wifi_network.h>

bool updateStatus(String paymentId);
String getData(String paymentId);
void setupToken();
void sendLog(
    String message);

#endif