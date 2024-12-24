#include <SinricPro.h>
#include <SinricProSwitch.h>
#include "SinricProConfig.h"

// Relay control function for Sinric Pro
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s turned %s\n", deviceId.c_str(), state?"on":"off");
  
  // Control Relays
  if (deviceId == RELAY1_ID) {
    digitalWrite(RELAY1_PIN, state ? (RELAY_ACTIVE_LOW ? LOW : HIGH) : (RELAY_ACTIVE_LOW ? HIGH : LOW));
    return true;
  }
  if (deviceId == RELAY2_ID) {
    digitalWrite(RELAY2_PIN, state ? (RELAY_ACTIVE_LOW ? LOW : HIGH) : (RELAY_ACTIVE_LOW ? HIGH : LOW));
    return true;
  }
  if (deviceId == RELAY3_ID) {
    digitalWrite(RELAY3_PIN, state ? (RELAY_ACTIVE_LOW ? LOW : HIGH) : (RELAY_ACTIVE_LOW ? HIGH : LOW));
    return true;
  }
  if (deviceId == RELAY4_ID) {
    digitalWrite(RELAY4_PIN, state ? (RELAY_ACTIVE_LOW ? LOW : HIGH) : (RELAY_ACTIVE_LOW ? HIGH : LOW));
    return true;
  }

  
  return false;
}

void setupSinricPro() {
  // Add devices and set callbacks
  SinricProSwitch& relay1 = SinricPro[RELAY1_ID];
  SinricProSwitch& relay2 = SinricPro[RELAY2_ID];
  SinricProSwitch& relay3 = SinricPro[RELAY3_ID];
  SinricProSwitch& relay4 = SinricPro[RELAY4_ID];


  // Set callback function
  relay1.onPowerState(onPowerState);
  relay2.onPowerState(onPowerState);
  relay3.onPowerState(onPowerState);
  relay4.onPowerState(onPowerState);


  // Setup SinricPro
  SinricPro.begin(APP_KEY, APP_SECRET);
  Serial.println("SinricPro Started");
}