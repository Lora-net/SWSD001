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

| Constant                                   | Description                                                           | Note                   |
| ------------------------------------------ | --------------------------------------------------------------------- | ---------------------- |
| `APP_MODEM_TIME_SYNC_SERVICE`              | Select the time synchronization service                               |                        |
| `APP_MODEM_TIME_SYNC_INTERVAL_IN_S`        | Interval in second between two automatic time synchronization request |                        |
| `APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S` | Interval in second between two manual time synchronization request    | Set to 0 to disable it |
