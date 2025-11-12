#ifndef Display_Driver_H
#define Display_Driver_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include "ui/ui.h"
#include "ui/vars.h"

extern void setupDisplay();
void displayStatus(String status, int milliseconds);
void handleDisplay();

#endif