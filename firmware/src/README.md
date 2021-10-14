# Eddy S1 firmware
## Overview
This firmware allows you to easly transform an MCU, like ESP8266 or ESP32, into a 1-channel smart switch.
## Configuration
The library adds the `eddy` section to the device configuration:
```javascript
{
  "relay1": {
    "id": "0",              // The relay ID
    "grp_id": 0,            // The relay gorup ID
    "inching_timeout": 0.0, // The inching timeout, in seconds; default is 0 (inching disabled)
    "inching_lock": false,  // Lock the state waiting for the inching timeout expiration
  },
  "sw1": {
    "id": "0",              // The siwtch ID"
    "mode": 3               // Siwtch mode: 0(DASH_BUTTON), 1(DETACHED), 2(TOGGLE_ON_PUSH), 3(TOGGLE_ON_EDGE)
  }
}
```
## To Dd
*No improvememts scheduled.*