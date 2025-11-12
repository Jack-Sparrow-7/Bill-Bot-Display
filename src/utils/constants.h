#ifndef Constants_H
#define Constants_H

#include <Arduino.h>

extern const String wifiName;
extern const String wifiPassword;
extern const String apiKey;

extern const String serverUrl;
extern const String tokenUrl;
extern const String orderUrl;
extern const String closeOrderUrl;
extern const String versionUrl;
extern const String firmwareUrl;
extern const String logUrl;
extern const String supabaseApiKey;

extern const String adminWifiName;
extern const String adminWifiPassword;

extern int printerRxPin;
extern int printerTxPin;
extern int scannerRxPin;
extern int scannerTxPin;
extern int resetPin;

#endif