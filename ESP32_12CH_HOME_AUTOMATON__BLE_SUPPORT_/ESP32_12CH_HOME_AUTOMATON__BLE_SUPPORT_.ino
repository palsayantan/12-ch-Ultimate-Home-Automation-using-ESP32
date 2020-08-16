/****************************************************************************************************************************
   WiFi & BLE Control Home Automation using ESP32 and Blynk

   Required Library:
      Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
      BlynkESP32_BT_WF by Khoi Hoang https://github.com/khoih-prog/BlynkESP32_BT_WF
      Based on orignal code by Crosswalkersam https://community.blynk.cc/u/Crosswalkersam

   The Sketch is Modified by Sayantan Pal https://github.com/palsayantan/12-ch-Ultimate-Home-Automation-using-ESP32

*****************************************************************************************************************************
   Important Notes:
      This code is intended to run on the ESP32 platform!
      Check your Tools->Board setting. And select ESP32 Dev Module
      To conmpile, use Partition Scheem with large APP size, such as -- Huge APP (3MB No OTA, 1MB SPIFFS)

*****************************************************************************************************************************/

#include <BlynkSimpleEsp32_BLE_WF.h>
#include <BlynkSimpleEsp32_WF.h>
#include <BLEDevice.h>
#include <BLEServer.h>
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth_WiFi[] = "zQ5UO74N-JtjCyDlrMPp3I26qb-5gqpu";
char auth_BLE[] = "_v2aCrnJ-_uMzjuVJVxFf1HKt7Ul35pB";

char BLE_Device_Name[] = "ESP32_WiFi_BLE";

// Your WiFi credentials.
char ssid[] = "Sayantan";     // Your Network SSID
char pass[] = "SayantaN";     // Your Network PASSWARD

int RELAY[12] = {13, 12, 14, 27, 26, 25, 1, 3, 17, 16, 4, 15};          // Define Output pins connected to Relay
int SWITCH[12] = {23, 22, 21, 19, 18, 5, 33, 32, 35, 34, 39, 36};       // Define Input pins connected to Switch
int VPIN[12] = {V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12};     // Define Virtual pins assigned in the APP

bool RELAY_STATE[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
bool SWITCH_STATE[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

#define STATUS_LED   2     // Shows Network Status

BLYNK_WRITE(V1) {
  RELAY_STATE[0] = param.asInt();
  digitalWrite(RELAY[0], RELAY_STATE[0]);
}
BLYNK_WRITE(V2) {
  RELAY_STATE[1] = param.asInt();
  digitalWrite(RELAY[1], RELAY_STATE[1]);
}
BLYNK_WRITE(V3) {
  RELAY_STATE[2] = param.asInt();
  digitalWrite(RELAY[2], RELAY_STATE[2]);
}
BLYNK_WRITE(V4) {
  RELAY_STATE[3] = param.asInt();
  digitalWrite(RELAY[3], RELAY_STATE[3]);
}
BLYNK_WRITE(V5) {
  RELAY_STATE[4] = param.asInt();
  digitalWrite(RELAY[4], RELAY_STATE[4]);
}
BLYNK_WRITE(V6) {
  RELAY_STATE[5] = param.asInt();
  digitalWrite(RELAY[5], RELAY_STATE[5]);
}
BLYNK_WRITE(V7) {
  RELAY_STATE[6] = param.asInt();
  digitalWrite(RELAY[6], RELAY_STATE[6]);
}
BLYNK_WRITE(V8) {
  RELAY_STATE[7] = param.asInt();
  digitalWrite(RELAY[7], RELAY_STATE[7]);
}
BLYNK_WRITE(V9) {
  RELAY_STATE[8] = param.asInt();
  digitalWrite(RELAY[8], RELAY_STATE[8]);
}
BLYNK_WRITE(V10) {
  RELAY_STATE[9] = param.asInt();
  digitalWrite(RELAY[9], RELAY_STATE[9]);
}
BLYNK_WRITE(V11) {
  RELAY_STATE[10] = param.asInt();
  digitalWrite(RELAY[10], RELAY_STATE[10]);
}
BLYNK_WRITE(V12) {
  RELAY_STATE[11] = param.asInt();
  digitalWrite(RELAY[11], RELAY_STATE[11]);
}

//this function will synchronize Relay States when any network/Hardware interruption occurs

BLYNK_CONNECTED() {
  //Uncomment this line if you want to follow app configurations
  //Blynk.syncAll();

  //Uncomment this line if you want to follow Hardware configurations
  for (int i = 0; i < 12; i++) {
    if (Blynk.connected())
      Blynk_WF.virtualWrite(VPIN[i], RELAY_STATE[i]);
    else
      Blynk_BLE.virtualWrite(VPIN[i], RELAY_STATE[i]);
  }
}

// Checks Network connection status in every 10 seconds
void Network_Checkup() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(10);
  }
}

void AUTOMATE() {
  for (int i = 0; i < 12; i++) {
    if (digitalRead(SWITCH[i]) == LOW) {
      if (SWITCH_STATE[i] != LOW) {
        RELAY_STATE[i] = !RELAY_STATE[i];
        digitalWrite(RELAY[i], RELAY_STATE[i]);
        if (Blynk.connected())
          Blynk_WF.virtualWrite(VPIN[i], RELAY_STATE[i]);
        else
          Blynk_BLE.virtualWrite(VPIN[i], RELAY_STATE[i]);
      }
      SWITCH_STATE[i] = LOW;
    }
    else {
      SWITCH_STATE[i] = HIGH;
    }
  }
}

void setup() {
  pinMode(STATUS_LED, OUTPUT);

  for (int i = 0; i < 12; i++) {
    pinMode(SWITCH[i], INPUT);
    pinMode(RELAY[i], OUTPUT);
    digitalWrite(RELAY[i], HIGH);
  }
  //Setup BLE Connection
  Blynk_BLE.setDeviceName(BLE_Device_Name);
  Blynk_BLE.begin(auth_BLE);
  //Setup WiFi Connection
  Blynk_WF.begin(auth_WiFi, ssid, pass);
  // timer enable function
  timer.setInterval(10000L, Network_Checkup);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (Blynk.connected()) {
      digitalWrite(STATUS_LED, HIGH);
    }
  }
  else {
    digitalWrite(STATUS_LED, LOW);
  }
  
  AUTOMATE();
  Blynk_WF.run();
  Blynk_BLE.run();
  timer.run();
}
