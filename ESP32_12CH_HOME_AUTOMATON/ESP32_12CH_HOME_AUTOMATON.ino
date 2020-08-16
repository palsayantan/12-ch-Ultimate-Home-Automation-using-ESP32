// Automatic And Manual Control Home Automation using ESP32 and Blynk

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "*****************************";

// Your WiFi credentials.
char ssid[] = "SSID";     // Your Network SSID
char pass[] = "PASSWORD";     // Your Network PASSWARD

int RELAY[12] = {13, 12, 14, 27, 26, 25, 1, 3, 17, 16, 4, 15};          // Define Output pins connected to Relay
int SWITCH[12] = {23, 22, 21, 19, 18, 5, 33, 32, 35, 34, 39, 36};       // Define Input pins connected to Switch
int VPIN[12] = {V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12};     // Define Virtual pins assigned in the APP
//[note: you need to invese the Output logic (1 _- 0) and change it to Switch Mode]

bool RELAY_STATE[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
bool SWITCH_STATE[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int STATUS_LED = 2;     //Shows Network Status

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
    Blynk.virtualWrite(VPIN[i], RELAY_STATE[i]);
  }
}

// Checks Network connection status in every 5 seconds
void Network_Checkup() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(10);
  }
}

void ONLINE() {
  if (Blynk.connected()) {
    for (int i = 0; i < 12; i++) {
      if (digitalRead(SWITCH[i]) == LOW) {
        if (SWITCH_STATE[i] != LOW) {
          RELAY_STATE[i] = !RELAY_STATE[i];
          digitalWrite(RELAY[i], RELAY_STATE[i]);
          Blynk.virtualWrite(VPIN[i], RELAY_STATE[i]);
        }
        SWITCH_STATE[i] = LOW;
      }
      else {
        SWITCH_STATE[i] = HIGH;
      }
    }
  }
}

void OFFLINE() {
  for (int i = 0; i < 12; i++) {
    if (digitalRead(SWITCH[i]) == LOW) {
      if (SWITCH_STATE[i] != LOW) {
        RELAY_STATE[i] = !RELAY_STATE[i];
        digitalWrite(RELAY[i], RELAY_STATE[i]);
      }
      SWITCH_STATE[i] = LOW;
    }
    else {
      SWITCH_STATE[i] = HIGH;
    }
  }
}

void setup()
{
  pinMode(STATUS_LED, OUTPUT);

  for (int i = 0; i < 12; i++) {
    pinMode(SWITCH[i], INPUT);
    pinMode(RELAY[i], OUTPUT);
    digitalWrite(RELAY[i], HIGH);
  }

  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  timer.setInterval(5000L, Network_Checkup);  // timer enable function
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(STATUS_LED, HIGH);
    Blynk.run();
    ONLINE();
  }
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(STATUS_LED, LOW);
    OFFLINE();
  }
  timer.run();
}
