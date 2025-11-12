#include "bill_bot_system.h"

void setupSystem()
{
    Serial.begin(9600);

    // setupSoftReset();
    setupDisplay();

    setupQrScanner();
    setupPrinter();

    Serial.println("Serial Initialized!");
    currentVersion = readFirmwareVersion();
    // token = readToken();

    setupWifi();
    syncTime();
    checkFirmwareUpdate();
    sendLog("Firmware Version: " + currentVersion);

    checkToken();
    Serial.println("System Started!");
}

void manageSystem()
{
    handleDisplay();
    String strength = String(getWifiStrength());

    if (isWifiNotStrong())
    {
        strength = String(getWifiStrength());
        displayStatus("Weak WiFi signal!\n" + strength, 300);
        return;
    }

    if (!isWifiConnected())
    {
        displayStatus("Wifi Lost!\nReconnecting...", 300);
        return;
    }

    if (noPaper())
    {
        strength = String(getWifiStrength());
        displayStatus("Sorry,Paper not Available!\n" + strength, 300);
        return;
    }

    String olderPaymentId = readId();
    if (!olderPaymentId.isEmpty() && isWifiConnected())
    {
        displayStatus("Loading...", 100);
        Serial.println("Updating older payment id!");
        updateStatus(olderPaymentId);
        clearId();
    }

    String readedId;
    strength = String(getWifiStrength());
    displayStatus("Ready to Scan!\n" + strength, 300);

    while (readedId.isEmpty())
    {
        handleDisplay();

        // checkReset();
        if (isWifiNotStrong())
        {
            strength = String(getWifiStrength());
            displayStatus("Weak WiFi signal!\n" + strength, 300);
            continue;
        }

        else if (!isWifiConnected())
        {
            displayStatus("WiFi Lost!\nReconnecting...", 300);
            continue;
        }

        else if (noPaper())
        {
            strength = String(getWifiStrength());
            displayStatus("Sorry, Paper not Available!\n" + strength, 300);
            continue;
        }

        else
        {
            strength = String(getWifiStrength());
            displayStatus("Ready to Scan!\n" + strength, 300);
            readedId = scanOrderId();
            // readedId = Serial.readStringUntil('\n');
            delay(300);
        }
    };
    displayStatus("Qr Scanned.\nPlease wait.", 1000);
    unsigned long startTime = millis();

    readedId.trim();
    Serial.println(readedId);

    int startIndex = readedId.indexOf('$');
    int endIndex = readedId.lastIndexOf('$');

    if (isInvalidId(startIndex, endIndex))
        return;

    String paymentId = readedId.substring(startIndex + 1, endIndex);
    Serial.print("Payment ID: ");
    Serial.println(paymentId);
    unsigned long fetchingStartTime = millis();

    Serial.println("Getting Data...");
    displayStatus("Getting Order Details...", 100);

    const int maxRetries = 3;
    int attempt = 0;
    String fetchedData;

    int delayMs = 500;

    while (attempt < maxRetries)
    {
        if (!isWifiConnected())
            break;

        if (isWifiNotStrong())
            break;

        fetchedData = getData(paymentId);
        if (!fetchedData.isEmpty())
            break;

        attempt++;
        Serial.println("Retrying API call...");
        delay(delayMs);
        delayMs *= 2;
    }

    if (fetchedData.isEmpty())
    {
        Serial.println("Failed to fetch data.");
        sendLog("Fetched Data is Empty.");
        displayStatus("Server busy or Wi-Fi down.Try again later.", 1000);
        return;
    }

    int separatorIndex = fetchedData.indexOf(':');
    int responseCode = 0;
    String data = "";

    if (separatorIndex > 0)
    {
        responseCode = fetchedData.substring(0, separatorIndex).toInt();
        data = fetchedData.substring(separatorIndex + 1);
    }

    unsigned long fetchingEndTime = millis();
    unsigned long fetchingTimeTaken = calculateTimeTaken(fetchingStartTime, fetchingEndTime);

    Serial.print("Time Taken for Fetching Data: ");
    Serial.print(fetchingTimeTaken);
    Serial.println(" - seconds");

    if (responseCode < 0)
    {
        Serial.println("Failed to fetch data.Server busy or Wi-Fidown.Try again later.");
        sendLog("Negative response code - " + String(responseCode));
        displayStatus("Server busy or Wi-Fi down.Try again later.", 2000);
        return;
    }

    if (responseCode == 401)
    {
        sendLog("Token Expired: " + String(responseCode));
        setNewToken();
        return;
    }

    if (!(responseCode >= 200 && responseCode < 300))
    {
        Serial.print("Response Code: ");
        Serial.println(responseCode);
        Serial.print("Fetched Data: ");
        Serial.println(data);
        String code = String(responseCode);
        sendLog("Something went wrong: " + code);
        displayStatus("Something went wrong: " + code, 2000);
        return;
    }

    if (responseCode >= 200 && responseCode < 300)
    {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, data);

        if (error)
        {
            sendLog("Order not found! JSON Error: " + String(error.c_str()));
            Serial.print("Order not found! JSON Error: ");
            displayStatus("Order not found!", 2000);
            Serial.println(error.c_str());
            Serial.println("Fetched Data: " + data);
            return;
        }

        String billNo = doc["data"]["orderId"].as<String>();
        String dateTz = doc["data"]["dateTz"].as<String>();
        String timeStr = getTimeFromISO(dateTz);
        String dateStr = getDateFromISO(dateTz);
        String status = doc["data"]["status"].as<String>();
        String paymentStatus = doc["data"]["paymentStatus"].as<String>();

        if (billNo.isEmpty())
        {
            String code = String(responseCode);
            sendLog("Bill No Empty: " + code);
            displayStatus("Order not found!", 2000);
            Serial.println("Order not found!");
            return;
        }

        // Serial.print("BillNo: ");
        // Serial.println(billNo);
        // Serial.print("Status: ");
        // Serial.println(status);
        // Serial.print("Payment-Status: ");
        // Serial.println(paymentStatus);
        // Serial.print("Date: ");
        // Serial.println(dateStr);
        // Serial.print("Time: ");
        // Serial.println(timeStr);

        if (paymentStatus != "COMPLETED")
        {
            Serial.println("Payment Pending!");
            displayStatus("Payment Pending!", 2000);
            return;
        }

        if (status == "COMPLETED")
        {
            Serial.println("Your bill is already generated!");
            displayStatus("Your Bill is Already Printed!", 2000);
            // return;
        }

        if (status == "ORDER-PLACED" || status == "INITIATED" || status == "OPEN" || status == "COMPLETED")
        {
            Serial.println("Your Bill - " + billNo + " is Printing!");
            displayStatus("Your Bill - " + billNo + " is Printing!", 1000);
            printHeader(dateStr, timeStr, billNo);
            double quantityTotal = 0;
            int quantityCount = 0;
            int itemCount = 0;
            JsonArray orderItems = doc["data"]["order"];
            for (JsonObject item : orderItems)
            {
                printEachItem(item, quantityCount, quantityTotal, itemCount);
            }
            printFooter(quantityTotal, itemCount, quantityCount);
            // Serial.printf("itemCount: %d\ntotalQuantity: %d\ngrandTotal: %.2f\n", itemCount, quantityCount, quantityTotal);
            // resetPrinter();
            displayStatus("Thank You!\nYour Bill - " + billNo + " is Printed!", 1000);
            delay(2000);
            if (noPaper())
            {
                Serial.println("Paper not Avalailable! Not Updating!");
                sendLog("Paper Out Not Updating: " + paymentId);
                return;
            }

            if (isWifiConnected() && !isWifiNotStrong())
            {
                bool isSuccess = updateStatus(paymentId);
                if (!isSuccess)
                {
                    sendLog("Server Issue Not Updating, Saved : " + paymentId);
                    saveId(paymentId);
                }
                Serial.println("Updated Successfully!");
            }
            else
            {
                saveId(paymentId);
                sendLog("WiFi Issue Not Updating, Saved : " + paymentId);
            }

            unsigned long endTime = millis();
            unsigned long totalTimeTaken = calculateTimeTaken(startTime, endTime);
            Serial.print("Total Time taken: ");
            Serial.print(totalTimeTaken);
            Serial.println(" - seconds");
            return;
        }
    }
}
