#include "constants.h"

const String wifiName = "IQOO_Z9S";
const String wifiPassword = "12345678";

const String adminWifiName = "BILL-BOT";
const String adminWifiPassword = "admin@rit";

// Production
// const String tokenUrl = "https://api.positeasy.in/api/v1/POS/merchant/get-otpAssignedDetails?otp=";
// const String orderUrl = "https://api.positeasy.in/api/v1/POS/merchant/on-table/full-order?paymentId=";
// const String closeOrderUrl = "https://api.positeasy.in/api/v1/POS/merchant/on-table/complete-order";

// Test
const String tokenUrl = "https://api.positeasy.in/api/v1/POS/merchant/get-otpAssignedDetails?otp=";
const String orderUrl = "https://stage.api.positeasy.in/renga/test/api/v1/POS/merchant/on-table/full-order?paymentId=";
const String closeOrderUrl = "https://stage.api.positeasy.in/renga/test//api/v1/POS/merchant/on-table/complete-order";

const String versionUrl = "https://github.com/Jack-Sparrow-7/Bill-Bot-System-Display-OTA/releases/download/ota/version.txt";
const String firmwareUrl = "https://github.com/Jack-Sparrow-7/Bill-Bot-System-Display-OTA/releases/download/ota/firmware.bin";

const String logUrl = "https://xkifsadlgeuwgswmjfki.supabase.co/rest/v1/device_logs";
const String supabaseApiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InhraWZzYWRsZ2V1d2dzd21qZmtpIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NjIzMTEyMTYsImV4cCI6MjA3Nzg4NzIxNn0.0nsEAwpwSrtMWaligeDMLSa_HvMdZIl737qzWz8fA2s";

int printerRxPin = 25;
int printerTxPin = 32;
int scannerRxPin = 16;
int scannerTxPin = 17;
int resetPin = 21;
