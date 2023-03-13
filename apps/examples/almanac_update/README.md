# LoRa Basics Modem almanac update example

## Description

The application will automatically join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

The almanac update service is enabled by sending an almanac status to LoRa Cloud via a DM message. Based on this status, LoRa Cloud will determine if the almanac is up-to-date and, if not, perform an incremental update by sending a downlink. It is possible that several incremental updates are needed to perform a full update.

There are 2 ways to trigger an incremental update:

* Periodic DM message configured with `smtc_modem_dm_set_info_fields()` and `smtc_modem_dm_set_info_interval()` including the almanac status among other fields
* Single DM message requested with `smtc_modem_dm_request_single_uplink()` and including the almanac status among other fields

In this example, only `SMTC_MODEM_DM_FIELD_ALMANAC_STATUS` field is sent through DM messages for clarity. It is possible to reach the same behavior with additional fields sent at the same time.

This example offers the possibility to speed up the process by requesting a DM message each time a partial almanac update is done (thanks to `SMTC_MODEM_EVENT_ALMANAC_UPDATE` event) instead of relying on the periodic DM report. This possibility is added for demonstration purpose.

## Configuration

Several parameters can be updated in `main_almanac_update.h` header file:

| Constant                            | Description                                                                             | Possible values   | Default Value |
| ----------------------------------- | --------------------------------------------------------------------------------------- | ----------------- | ------------- |
| `APP_MODEM_DM_LORAWAN_FPORT`        | LoRaWAN FPort to be used by the device management service                               | [1-223]           | 199           |
| `APP_MODEM_DM_INTERVAL`             | Interval between two automatic device management information reports                    | [0-63]            | 30            |
| `APP_MODEM_ALMANAC_UPDATE_SPEED_UP` | Speed up the almanac update by requesting almanac status report on almanac update event | {`true`, `false`} | true          |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Expected Behavior

Here follow the steps that shall be seen in the logs to indicate the expected behavior of the application.

Note that this behavior is expected in the case where the almanac of the device are outdated.
You may consider using example *full_almanac_update* to enforce outdated almanac in the device.

### Device starts and resets

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem almanac update example =====

INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
```

Following this print you shall find application and parameter prints.

### Joined the network

```
INFO: ###### ===== JOINED EVENT ==== ######
```

After the device has joined a network, it configures a DM message for synchronization of its almanac.
This DM message will be sent every `APP_MODEM_DM_INTERVAL` minutes (by default every 60 minutes).

### Almanac update events

In case the almanac of the device needs to be updated, the response to the DM message the device sent will trigger an update of the almanac. This is notified with the following message:

```
WARN: DM_ALM_UPDATE (0x0a)
INFO: ###### ===== ALMANAC UPDATE EVENT ==== ######
Almanac update status: SMTC_MODEM_EVENT_ALMANAC_UPDATE_STATUS_REQUESTED
Request a DM uplink instead of waiting for the periodic report to share the almanac status.
```

Several DM message exchanges may be necessary.
In this case the following message will be repeated:

```
WARN: DM_ALM_UPDATE (0x0a)
INFO: ###### ===== ALMANAC UPDATE EVENT ==== ######
Almanac update status: SMTC_MODEM_EVENT_ALMANAC_UPDATE_STATUS_REQUESTED
```

Once almanac is completely updated, the following message is displayed:

```
WARN: DM_ALM_UPDATE (0x0a)
INFO: ###### ===== ALMANAC UPDATE EVENT ==== ######
Almanac update status: SMTC_MODEM_EVENT_ALMANAC_UPDATE_COMPLETED
```