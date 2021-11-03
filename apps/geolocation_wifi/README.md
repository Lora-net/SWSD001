# Geolocation Wi-Fi example application 

## Description

This application demonstrates a Wi-Fi scan procedure.
It executes Wi-Fi scans and send it through LoRaWAN.

The example application state machine is the following:

1. The application initialises
1. On Modem Reset event the application starts the Join Procedure
1. On Join Accept event, the application :
    - configures and start a timer
1. On Timer Elapsed event, the application:
    - starts a Wi-Fi scan procedure
1. On Scan Terminated event, the application:
    - fetches the Wi-Fi scan result
    - sends it through LoRaWAN link
1. On Tx Done event, the application waits a configurable amount of time and start again the Wi-Fi scan procedure

## Configuration

The `main_geolocation_wifi.h` header file defines several constants which can be set to define the configurable parameters of the application.


| Constant                    | Comments                                                |
| --------------------------- | ------------------------------------------------------- |
| `APP_GEOLOC_SCAN_PERIOD`    | Define the duration between two consecutive Wi-Fi scans |
| `LORAWAN_CONFIRMED_MSG_ON`  | Request a confirmation message or not                   |


## Usage

### LoRaWAN Network Server / Application Server

This application needs an Application Server to run in order to perform the Wi-Fi solving.
A Node-Red application server is provided in folder *apps/geolocation_application_server*.
Refer to the readme in this folder for details about setup and usage of the application server.

#### Payload format between device and Application Server

The device sends the scan results to the Application Server through LoRaWAN uplinks on port 195. Each scan result is sent following the *Tag, Length, Value* format:

- **TAG** (1 byte) `0x0F`;
- **LENGTH** (1 byte) The length in byte of *VALUE* field;
- **VALUE** (variable length) The data content.

The *VALUE* field for this demonstration contains the Wi-Fi scan results one after the other. One Wi-Fi scan result contains the following field:

- *RSSI* (1 byte) The RSSI measured for the scan result in dBm;
- *MAC address* (6 bytes) The MAC address of the result

Altogether, the payload format of uplinks is the following

| Byte  | 0   | 1      | 2               | 3-8                    | 9               | 10-15                  | ... |
| ----- | --- | ------ | --------------- | ---------------------- | --------------- | ---------------------- | --- |
| Field | TAG | Length | *Result1*: RSSI | *Result1*: MAC address | *Result2*: RSSI | *Result2*: MAC address | ... |
