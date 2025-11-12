#include "helpers.h"

String generateOTP()
{
    uint32_t r = esp_random();
    unsigned long otp = r % 1000000UL;
    char buf[7];
    snprintf(buf, sizeof(buf), "%06lu", otp);
    return String(buf);
}

String generateUniqueOTP(String oldOtp)
{
    String newOtp;
    do
    {
        newOtp = generateOTP();
    } while (newOtp == oldOtp);
    oldOtp = newOtp;
    return newOtp;
}

void printEachItem(JsonObject item, int &quantityCount, double &quantityTotal, int &itemCount)
{
    String name = item["name"];
    name.toUpperCase();
    double price = item["price"];
    price = price / 100.0;
    int quantity = item["quantity"];
    double total = price * quantity;
    printItem(name, quantity, price, total);

    quantityCount += quantity;
    quantityTotal += total;
    itemCount++;

    // Serial.printf("Name: %s | Price: %.2f INR | Qty: %d | Total: %.2f INR\n",
    //               name.c_str(), price, quantity, total);
}

String getTimeFromISO(String isoTimestamp)
{
    int tIndex = isoTimestamp.indexOf('T');
    if (tIndex != -1 && isoTimestamp.length() >= tIndex + 6)
    {
        return isoTimestamp.substring(tIndex + 1, tIndex + 6);
    }
    return "";
}

String getDateFromISO(String isoTimestamp)
{
    int tIndex = isoTimestamp.indexOf('T');
    if (tIndex != -1)
    {
        return isoTimestamp.substring(0, tIndex);
    }
    return "";
}

unsigned long calculateTimeTaken(unsigned long startTime, unsigned long endTime)
{
    unsigned long resultTime = endTime - startTime;
    resultTime = resultTime / 1000;
    return resultTime;
}

void checkToken()
{
    if (token.isEmpty())
    {
        Serial.println("No token found in storage. Requesting new one...");
        setupToken();
    }
    else
    {
        Serial.print("Loaded stored token: ");
        Serial.println(token);
    }
}

void setNewToken()
{
    token = "";
    Serial.println("Token Expired. Requesting new one...");
    displayStatus("Token Expired.\nRequesting new one...", 2000);
    setupToken();
    delay(1000);
    ESP.restart();
}

bool isInvalidId(int startIndex, int endIndex)
{
    if (startIndex == -1 || endIndex == -1 || endIndex <= startIndex)
    {
        Serial.println("Invalid Id!");
        displayStatus("Invalid ID, Try again!", 2000);
        return true;
    }
    return false;
}

void setupSoftReset()
{
    pinMode(resetPin, INPUT_PULLUP);
    Serial.println("Soft Reset setup successful.");
}

void checkReset()
{
    if (digitalRead(resetPin) == LOW)
    {
        Serial.println("Reseting...");
        displayStatus("Restarting System...", 1000);
        // resetCredentials();
        ESP.restart();
    }
}

String getDeviceId()
{
    uint64_t chipId = ESP.getEfuseMac();
    char id[17];
    snprintf(id, sizeof(id), "%012llX", chipId);
    String deviceId = String(id);
    Serial.println("Esp 32 - " + deviceId);
    return String("Esp 32 - " + deviceId);
}

void syncTime()
{
    Serial.println("Syncing NTP time for SSL...");
    displayStatus("Loading...",100);
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 10000))
    {
        Serial.println("Failed to sync time!");
    }
    else
    {
        Serial.println("Time synchronized successfully");
    }
}
