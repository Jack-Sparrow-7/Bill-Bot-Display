#include "ota_update.h"

HTTPClient http;

void checkFirmwareUpdate()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    displayStatus("Checking for firmware Update...",300);
    String url = versionUrl + "?t=" + String(millis());
    http.begin(url);
    http.setTimeout(30000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.addHeader("Cache-Control", "no-cache");
    int responseCode = http.GET();

    if (responseCode == 200)
    {
      serverVersion = http.getString();
      serverVersion.trim();
      serverVersion.replace("\"", "");

      Serial.print("Current version: ");
      Serial.println(currentVersion);
      Serial.print("Server version: ");
      Serial.println(serverVersion);

      if (serverVersion == currentVersion)
      {
        Serial.println("No New Version Available!");
        displayStatus("No New Version Available!",2000);
      }
      else
      {
        Serial.println("New Version Available!");
        displayStatus("New Version Available!",2000);
        Serial.println("New firmware available. Initiating OTA...");
        displayStatus("New firmware available.\nInitiating OTA...",300);
        performOTA();
      }
    }
    else
    {
      Serial.print("Error: ");
      Serial.println(responseCode);
    }
    http.end();
  }
}

void performOTA()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(firmwareUrl);
    http.setTimeout(30000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int responseCode = http.GET();

    if (responseCode == 200)
    {
      int firmwareSize = http.getSize();

      if (firmwareSize > 0)
      {
        bool canBegin = Update.begin(firmwareSize);
        if (canBegin)
        {
          WiFiClient *stream = http.getStreamPtr();
          size_t written = Update.writeStream(*stream);

          if (written == firmwareSize)
          {
            Serial.println("Firmware written successfully.");
            displayStatus("Firmware updated successfully.",2000);
          }
          else
          {
            Serial.print("Written only ");
            Serial.print(written);
            Serial.print(" of ");
            Serial.print(firmwareSize);
            Serial.println(" bytes.");
          }

          if (Update.end())
          {
            Serial.println("OTA completed. Rebooting...");
            displayStatus("OTA completed. Rebooting...",2000);
            saveFirmwareVersion(serverVersion);
            delay(2000);
            ESP.restart();
          }
          else
          {
            Serial.print("Update failed. Error: ");
            Serial.println(Update.getError());
            displayStatus("Update failed.",2000);
          }
        }
        else
        {
          Serial.println("Not enough space to begin OTA.");
          displayStatus("Not enough space to begin OTA.",2000);
        }
      }
      else
      {
        Serial.println("Invalid content length.");
        displayStatus("Invalid content length.",2000);
      }
    }
    else
    {
      Serial.print("Failed to download firmware. HTTP Code: ");
      Serial.println(responseCode);
      displayStatus("Failed to download firmware.",2000);
    }
    http.end();
  }
}