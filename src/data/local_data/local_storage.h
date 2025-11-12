#ifndef Token_Storage_H
#define Token_Storage_H

#include <Preferences.h>

void saveToken(const String &fetchedToken);
String readToken();
void clearToken();

String readFirmwareVersion();
void saveFirmwareVersion(const String &newVersion);
void clearFirmwareVersion();

String readId();
void saveId(const String &paymentId);
void clearId();

#endif