#include "database.h"

HTTPClient httpClient;

String oldOtp;
String currentOtp;

bool updateStatus(String paymentId)
{
    String path = closeOrderUrl;

    JsonDocument doc;
    doc["paymentId"] = paymentId;
    String body;
    serializeJson(doc, body);

    String authorization = "Bearer " + token;

    httpClient.begin(path);
    httpClient.setTimeout(30000);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("Authorization", authorization);

    int responseCode = httpClient.PUT(body);

    bool success = false;
    if (responseCode >= 200 && responseCode < 300)
    {
        String resp = httpClient.getString();
        success = true;
    }
    else
    {
        Serial.printf("Error in update status! Response code = %d\n", responseCode);
        sendLog("Error in update status!, response-code = " + String(responseCode));
    }

    httpClient.end();
    return success;
}

String getData(String paymentId)
{
    String data = "";
    String path = orderUrl + paymentId;
    String authorization = "Bearer " + token;

    httpClient.begin(path);
    httpClient.setTimeout(30000);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("Authorization", authorization);
    int responseCode = httpClient.GET();
    if (responseCode >= 200 && responseCode < 300)
    {
        data = httpClient.getString();
        data.trim();
    }
    else
    {
        Serial.printf("Error in get data!, response-code = %d\n", responseCode);
        sendLog("Error in get data!, response-code = " + String(responseCode));
        data = httpClient.getString();
    }
    httpClient.end();
    String result = String(responseCode) + ":" + data;
    return result;
}

void setupToken()
{
    Serial.println("Starting OTP generation...");

    const unsigned long OTP_INTERVAL = 5 * 60 * 1000UL;
    const unsigned long API_CHECK_INTERVAL = 3000UL;

    while (token.isEmpty())
    {
        currentOtp = generateUniqueOTP(oldOtp);
        Serial.print("Generated OTP: ");
        Serial.println(currentOtp);
        displayStatus("OTP: " + currentOtp + "\nValid for 5 mins.", 300);

        unsigned long otpStartTime = millis();
        unsigned long lastApiCheck = 0;

        while (millis() - otpStartTime < OTP_INTERVAL)
        {
            if (millis() - lastApiCheck >= API_CHECK_INTERVAL)
            {
                lastApiCheck = millis();

                String tokenPath = tokenUrl + currentOtp;
                httpClient.begin(tokenPath);
                httpClient.setTimeout(30000);
                httpClient.addHeader("Content-Type", "application/json");

                int responseCode = httpClient.GET();
                if (responseCode == 200)
                {
                    String data = httpClient.getString();
                    data.trim();
                    Serial.printf("API response: %s, code: %d\n", data.c_str(), responseCode);

                    JsonDocument doc;
                    DeserializationError error = deserializeJson(doc, data);
                    if (!error)
                    {
                        if (doc["data"].is<const char *>())
                        {
                            token = String((const char *)doc["data"]);
                            Serial.print("Token received: ");
                            Serial.println(token);
                            httpClient.end();
                            saveToken(token);
                            return;
                        }
                        else
                        {
                            Serial.println("'data' key not found in JSON");
                        }
                    }
                    else
                    {
                        Serial.print("JSON parse error: ");
                        Serial.println(error.c_str());
                    }
                }
                else
                {
                    Serial.printf("HTTP error: %d\n", responseCode);
                }
                httpClient.end();
            }
        }

        Serial.println("5 minutes passed. Generating new OTP...");
        displayStatus("Generating new OTP...", 2000);
    }

    Serial.println("OTP generation stopped. Proceeding to main program.");
}

void sendLog(String message)
{
    JsonDocument doc;
    doc["device_id"] = getDeviceId();
    doc["message"] = message;

    String body;
    serializeJson(doc, body);

    httpClient.begin(logUrl);
    httpClient.setTimeout(15000);
    httpClient.addHeader("apikey", supabaseApiKey);
    httpClient.addHeader("Authorization", "Bearer " + supabaseApiKey);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("Prefer", "return=minimal");

    int responseCode = httpClient.POST(body);

    if (responseCode >= 200 && responseCode < 300)
    {
        Serial.println("Successfully sent Log.");
    }
    else
    {
        Serial.printf("Error in send log! Response code = %d\n", responseCode);
    }

    httpClient.end();
}
