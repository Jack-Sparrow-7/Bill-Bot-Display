
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include "ui.h"
#include <vars.h>
#include <Wire.h>

#define slave_address 0x08
String statusMessage = "";

static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;
uint16_t calData[5] = {189, 3416, 359, 3439, 1};

TFT_eSPI lcd = TFT_eSPI(); /* TFT entity */

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screenWidth * screenHeight / 13];

/* display flash */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.pushColors((uint16_t *)&color_p->full, w * h, true);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

uint16_t touchX, touchY;
/*touch read*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{

  bool touched = lcd.getTouch(&touchX, &touchY, 600);
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*set location*/
    data->point.x = touchX;
  }
}

void receiveEvent(int bytes)
{
  statusMessage = "";
  while (Wire.available() > 0)
  {
    char c = Wire.read();
    statusMessage += c;
  }
  Serial.print("Received Data: ");
  Serial.println(statusMessage);
  
  if (statusMessage == "RESET")
  {
    Serial.println("Rebooting ...");
    delay(2000);
    ESP.restart();
  }

  if (statusMessage.length() > 0)
  {
    set_var_status(statusMessage.c_str());
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial Initialized!");
  Wire.begin(slave_address);

  // Port_D
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);

  // LCD init
  lcd.begin();
  lcd.setRotation(3);
  lcd.fillScreen(TFT_BLACK);
  lcd.setTouch(calData);
  delay(100);
  // background light pin
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);

  // lvgl init
  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, screenWidth * screenHeight / 13);

  /* Initialize the display */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  lcd.fillScreen(TFT_BLACK);

  ui_init();
  set_var_status("Loading...");
  Wire.onReceive(receiveEvent);
}

void loop()
{
  lv_timer_handler();
  ui_tick();
  delay(5);
}