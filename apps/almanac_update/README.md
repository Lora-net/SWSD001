# LoRa Basics Modem almanac update example

## Description

The application will automatically join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

The almanac update service is enabled by sending an almanac status to LoRa Cloud via a DM message. Based on this status, LoRa Cloud will determine if the almanac is up-to-date and, if not, perform an incremental update by sending a downlink. It is possible that several incremental updates are needed to perform a full update.

There are 2 ways to trigger an incremental update:

* Periodic DM message configured with `smtc_modem_dm_set_info_fields()` and `smtc_modem_dm_set_info_interval()` including the almanac status among other fields
* Single DM message requested with `smtc_modem_dm_request_single_uplink()` and including the almanac status among other fields

In this example, only `SMTC_MODEM_DM_FIELD_ALMANAC_STATUS` field is sent through DM messages for clarity. It is possible to reach the same beahvior with additional fields sent at the same time.

This example offers the possibility to speed up the process by requesting a DM message each time a partial almanac update is done (thanks to `SMTC_MODEM_EVENT_ALMANAC_UPDATE` event) instead of relying on the periodic DM report. This possibility is added for demonstration purpose.

## Configuration

Several parameters can be updated in `main_almanac_update.h` header file:

| Constant                            | Description                                                                             |
| ----------------------------------- | --------------------------------------------------------------------------------------- |
| `APP_DM_LORAWAN_FPORT`              | LoRaWAN FPort to be used by the device management service                               |
| `APP_MODEM_DM_INTERVAL`             | Interval between two automatic device management information reports                    |
| `APP_MODEM_ALMANAC_UPDATE_SPEED_UP` | Speed up the almanac update by requesting almanac status report on almanac update event |
