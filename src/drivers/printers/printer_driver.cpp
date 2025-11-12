#include "printer_driver.h"

HardwareSerial printer(2);

volatile bool printerBlocked = false;

// uint8_t control_parameter_command[] = {0x1B, 0x37, 0x0B, 0x82, 0x0A};
uint8_t control_parameter_command[] = {0x1B, 0x37, 0x0B, 0xB0, 0x04};
uint8_t bold_on_command[] = {0x1B, 0x45, 0x01};
uint8_t bold_off_command[] = {0x1B, 0x45, 0x00};
uint8_t feed_one_command[] = {0x1B, 0x64, 1};
uint8_t feed_four_command[] = {0x1B, 0x64, 4};
uint8_t font_medium_command[] = {0x1D, 0x21, 1};
uint8_t fontsize_default_command[] = {0x1D, 0x21, 0};
uint8_t jusify_center_command[] = {0x1B, 0x61, 1};
uint8_t justify_left_command[] = {0x1B, 0x61, 0};
uint8_t justify_right_command[] = {0x1B, 0x61, 2};
uint8_t out_status_command[] = {0x1B, 76, 1};
uint8_t end_status_command[] = {0x1D, 0x72, 1};
uint8_t reset_command[] = {0x1B, 0x40};
uint8_t enable_asb[] = {0x1D, 0x61, 0x14};

void onPrinterData()
{
    while (printer.available())
    {
        byte asb = printer.read();

        Serial.printf("ASB Byte: 0x%02X\n", asb);

        if (asb & 0x04)
        {
            Serial.println("Paper out detected!");
            printerBlocked = true;
        }
        else if (asb == 0x00)
        {
            Serial.println("Paper OK again.");
            printerBlocked = false;
        }
    }
}

void setupPrinter()
{
    printer.begin(9600, SERIAL_8N1, printerRxPin, printerTxPin);
    delay(300);
    printer.write(control_parameter_command, sizeof(control_parameter_command));
    printer.write(enable_asb, sizeof(enable_asb));
    printer.onReceive(onPrinterData);
    printer.setRxFIFOFull(1);
}

void printHeader(String date, String time, String billNo)
{
    printer.write(jusify_center_command, sizeof(jusify_center_command));
    printer.write(bold_on_command, sizeof(bold_on_command));
    printer.write(font_medium_command, sizeof(font_medium_command));
    printer.println(F("RIT SALES CANTEEN"));
    printer.write(fontsize_default_command, sizeof(fontsize_default_command));
    printer.printf("BILL NO: %s\n", billNo);
    printer.printf("%s %s\n", date, time);
    printer.println(F("- - - - - - - - - - - - - - - -"));
    printer.println(F("ITEM    QTY    PRICE   AMOUNT"));
    printer.println(F("- - - - - - - - - - - - - - - -"));
}

void printItem(String name, int quantity, double price, double total)
{
    printer.write(justify_left_command, sizeof(justify_left_command));
    printer.println(name);
    printer.write(jusify_center_command, sizeof(jusify_center_command));
    printer.printf("       %dNO    %.2f    %.2f\n", quantity, price, total);
}

void printFooter(double grandTotal, int itemCount, int quantityCount)
{
    printer.write(justify_left_command, sizeof(justify_left_command));
    printer.println(F("- - - - - - - - - - - - - - - -"));
    printer.printf("ITEM: %d  QTY: %d\n",
                   itemCount, quantityCount);
    printer.println(F("- - - - - - - - - - - - - - - -"));
    printer.printf("GRAND TOTAL: %.2f\n", grandTotal);
    printer.write(bold_off_command, sizeof(bold_off_command));
    printer.write(feed_four_command, sizeof(feed_four_command));
}

bool isPaperNearEnd()
{
    printer.write(end_status_command, sizeof(end_status_command));

    byte status = 0x00;
    while (printer.available())
    {
        status = printer.read();
    }

    return status & 0x0C;
}

bool isPaperOut()
{
    printer.write(out_status_command, sizeof(out_status_command));

    byte status = 0x00;
    while (printer.available())
    {
        status = printer.read();
    }
    return status & 0x04;
}

bool noPaper()
{
    return printerBlocked || isPaperNearEnd() || isPaperOut();
}