#ifndef Printer_Driver_H
#define Printer_Driver_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <utils/constants.h>
#include <core/bill_bot_system.h>

extern volatile bool printerBlocked;

void setupPrinter();

void printHeader(String date, String time, String billNo);

void printItem(String name, int quantity, double price, double total);

void printFooter(double grandTotal, int itemCount, int quantityCount);

bool noPaper();

#endif