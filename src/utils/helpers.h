#ifndef Helpers_H
#define Helpers_H

#include <Arduino.h>
#include "esp_system.h"
#include <ArduinoJson.h>
#include <drivers/printers/printer_driver.h>
#include <data/local_data/local_storage.h>
#include <data/database/database.h>
#include <utils/dynamics.h>

String generateOTP();
String generateUniqueOTP(String oldOtp);
void printEachItem(JsonObject item, int &quantityCount, double &quantityTotal, int &itemCount);
String getTimeFromISO(String isoTimestamp);
String getDateFromISO(String isoTimestamp);
unsigned long calculateTimeTaken(unsigned long startTime, unsigned long endTime);
void checkToken();
void setNewToken();
bool isInvalidId(int startIndex, int endIndex);
void setupSoftReset();
// void checkReset();
String getDeviceId();
void syncTime();

#endif