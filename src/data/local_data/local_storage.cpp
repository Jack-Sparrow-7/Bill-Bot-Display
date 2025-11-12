#include "local_storage.h"

Preferences preferences;

void saveToken(const String &fetchedToken)
{
    preferences.begin("local", false);
    preferences.putString("token", fetchedToken);
    Serial.print("Token Saved:");
    Serial.println(fetchedToken.c_str());
    preferences.end();
}

String readToken()
{
    preferences.begin("local", false);
    String token = preferences.getString("token", "");
    preferences.end();
    return token;
}

void clearToken()
{
    preferences.begin("local", false);
    preferences.remove("token");
    Serial.println("Token Cleared!");
    preferences.end();
}

String readFirmwareVersion()
{
    preferences.begin("local", false);
    String version = preferences.getString("version", "1.0.0");
    preferences.end();
    return version;
}

void saveFirmwareVersion(const String &newVersion)
{
    preferences.begin("local", false);
    preferences.putString("version", newVersion);
    Serial.print("Firmware Version Saved:");
    Serial.println(newVersion.c_str());
    preferences.end();
}

void clearFirmwareVersion()
{
    preferences.begin("local", false);
    preferences.remove("version");
    Serial.println("Firmware Version Cleared!");
    preferences.end();
}


void saveId(const String &paymentId)
{
    preferences.begin("local", false);
    preferences.putString("id", paymentId);
    Serial.print("Id Saved:");
    Serial.println(paymentId.c_str());
    preferences.end();
}

String readId()
{
    preferences.begin("local", false);
    String id = preferences.getString("id", "");
    preferences.end();
    return id;
}

void clearId()
{
    preferences.begin("local", false);
    preferences.remove("id");
    Serial.println("Id Cleared!");
    preferences.end();
}