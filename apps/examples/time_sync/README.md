# LoRa Basics Modem time synchronization example

## Description

The application will automatically join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), the configured time synchronization service is initialized by calling `smtc_modem_time_set_sync_interval_s()` and launched by calling `smtc_modem_time_start_sync_service()`. Then, time synchronization requests are sent on a regular basis by this service. The service can be stopped anytime by calling `smtc_modem_time_stop_sync_service()`.

The application also implements an asynchronous time synchronization request, based on the LoRa Basics Modem alarm functionality. Each time the alarm-related event is triggered, the application sends a time synchronization request by calling `smtc_modem_time_trigger_sync_request()`.

For more information:

* MAC command: refer to the LoRaWAN specification v1.0.4
* ALC sync: refer to the [LoRa Developer portal](https://lora-developers.semtech.com/resources/tools/lora-edge-asset-management/?url=rst/Modem/alc_sync.html).

## Configuration

Several parameters can be updated in `main_time_sync.h` header file:

| Constant                                   | Description                                                           | Possible values                           | Default Value              | Note                   |
| ------------------------------------------ | --------------------------------------------------------------------- | ----------------------------------------- | -------------------------- | ---------------------- |
| `APP_MODEM_TIME_SYNC_SERVICE`              | Select the time synchronization service                               | Value in `smtc_modem_time_sync_service_t` | `SMTC_MODEM_TIME_MAC_SYNC` |                        |
| `APP_MODEM_TIME_SYNC_INTERVAL_IN_S`        | Interval in second between two automatic time synchronization request | `uint32_t`                                | 900                        |                        |
| `APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S` | Interval in second between two manual time synchronization request    | `uint32_t`                                | 130                        | Set to 0 to disable it |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Expected Behavior

Here follow the steps that shall be seen in the logs to indicate the expected behavior of the application:

### Device starts and resets

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem time synchronization example =====

INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
```

Following this print you shall find application and parameter prints

### Joined the network

At first run no time is supposed to be available

```
INFO: ###### ===== JOINED EVENT ==== ######
INFO: Checking/printing local UTC time joining network
WARN: No time available.
```

### Absolute time reception

```
INFO: ###### ===== TIME EVENT ==== ######
Time: SMTC_MODEM_EVENT_TIME_VALID
INFO: Checking/printing local UTC time upon TIME reception
INFO: Current UTC time: Tue 2022-04-05 20:27:49 GMT
```

### Repeating synchronization request upon timer

If `APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S` is set to any other value than 0 an alarm event is supposed to be seen in the logs as follows:

```
INFO: ###### ===== ALARM EVENT ==== ######
INFO: Sending synchronization request upon manual timer request
Manual time synchronization request done. Next manual request in 130 seconds.
```
