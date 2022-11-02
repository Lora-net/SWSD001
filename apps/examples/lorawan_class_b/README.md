# LoRa Basics Modem LoRaWAN Class B example

## Description

The application will automatically starts a procedure to join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), the example will perform the requested operations to enable class B.

The application is also capable of displaying data and meta-data of a received downlink.

## Configuration

Several parameters can be updated in `main_lorawan_class_b.h` header file:

| Constant                        | Description                                                   | Possible values                                           | Default Value                      |
| ------------------------------- | ------------------------------------------------------------- | --------------------------------------------------------- | ---------------------------------- |
| `LORAWAN_CLASS_B_PING_SLOT`     | LoRaWAN Class B ping slot periodicity                         | Any value of `smtc_modem_class_b_ping_slot_periodicity_t` | `SMTC_MODEM_CLASS_B_PINGSLOT_16_S` |
| `TIME_SYNC_APP_INTERVAL_S`      | Interval between 2 time synchronization once time is acquired | `uint32_t`                                                | 86400                              |
| `TIME_SYNC_APP_INVALID_DELAY_S` | Invalid delay for time sync                                   | `uint32_t`                                                | 3 * 86400                          |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Expected Behavior

Here follow the steps that shall be seen in the logs to indicate the expected behavior of the application:

### Device starts and resets

```
INFO: Modem Initialization

INFO: ###### ===== LoRa Basics Modem LoRaWAN Class B demo application ==== ######

INFO: LoRaWAN version: 01.00.04.01
INFO: LoRa Basics Modem version: 03.01.02

INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
```
Following this print you shall find application and parameter prints

### Joined and request for class B

```
INFO: ###### ===== JOINED EVENT ==== ######
WARN: PING SLOT REQUEST
```

### Class B accepted

```
INFO: ###### ===== CLASS B PING SLOT INFO EVENT ==== ######
Class B ping slot status: SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_ANSWERED
```

### Getting absolute time from server

```
INFO: ###### ===== TIME EVENT ==== ######
Time: SMTC_MODEM_EVENT_TIME_VALID
INFO: Current UTC time: Tue 2022-04-05 16:28:43 GMT
```
With the time being the current time.

### Having Class B ready

```
INFO: ###### ===== CLASS B STATUS EVENT ==== ######
Class B status: SMTC_MODEM_EVENT_CLASS_B_READY
INFO: This function is implemented as a placeholder.
```

### Downlink

If you decide to try a downlink message to test reception, you shall see this kind of print when the downlink is effectively received in Class B
```
INFO: ###### ===== DOWNLINK EVENT ==== ######
Rx window: SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB
Rx port: 1
Rx RSSI: -22
Rx SNR: 11
INFO: Downlink received:
INFO:   - LoRaWAN Fport = 1
INFO:   - Payload size  = 9
INFO:   - RSSI          = -22 dBm
INFO:   - SNR           = 11 dB
INFO:   - Rx window = SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB
```

The info displayed in this print may vary.

However, one stays always the same to confirm the downlink is indeed a class B one, which is the Rx Windows.
This last must be equal to SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB

## Limitation

Even though the class B example can run with the region configured to `SMTC_MODEM_REGION_WW2G4`, it is not supported and has not been tested.

