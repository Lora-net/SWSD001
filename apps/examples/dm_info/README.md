# LoRa Basics Modem device management information report example

## Description

The application will automatically join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), the application configures the DM service by calling:

* `smtc_modem_dm_set_fport()` to configure the LoRaWAN port used by the DM service
* `smtc_modem_dm_set_info_fields()` to configure the fields to be reported
* (opt.) `smtc_modem_dm_set_user_data()` to set the user-defined data to be reported (if selected in `smtc_modem_dm_set_info_fields()`)
* `smtc_modem_dm_set_info_interval()` to set the interval between two reports

The application also implements an asynchronous DM information fields report, based on the LoRa Basics Modem alarm functionality. Each time the alarm-related event is triggered, the application request a single DM uplink by calling `smtc_modem_dm_request_single_uplink()` with a specific list of fields to be reported.

## Configuration

Several parameters can be updated in `main_dm_info.h` header file:

| Constant                        | Description                                                             | Possible values                                                | Default Value                         | Note                   |
| ------------------------------- | ----------------------------------------------------------------------- | -------------------------------------------------------------- | ------------------------------------- | ---------------------- |
| `APP_MODEM_DM_LORAWAN_FPORT`    | LoRaWAN FPort to be used by the device management service               | [0-223]                                                        | 199                                   |                        |
| `APP_MODEM_DM_FORMAT`           | Format of the interval value defined with `APP_MODEM_DM_INTERVAL`       | Values from `smtc_modem_dm_info_interval_format_t` enumeration | SMTC_MODEM_DM_INFO_INTERVAL_IN_MINUTE |                        |
| `APP_MODEM_DM_INTERVAL`         | Interval between two automatic device management information reports    | [0-63]                                                         | 2                                     |                        |
| `APP_MODEM_DM_TRIGGER_INTERVAL` | Interval between two asynchronous device management information reports | any value that fits in `uint32_t`                              | 50                                    | Set to 0 to disable it |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

There are also two arrays the user can modify - `app_modem_dm_fields` and `app_modem_dm_fields_single` - to select the fields to be reported to LoRa Cloud.

For automatic reporting, `app_modem_dm_fields` is configured by default with the following fields:

* SMTC_MODEM_DM_FIELD_STATUS
* SMTC_MODEM_DM_FIELD_CHARGE
* SMTC_MODEM_DM_FIELD_VOLTAGE
* SMTC_MODEM_DM_FIELD_TEMPERATURE

For asynchronous reporting, `app_modem_dm_fields_single` is configured by default with the following fields:

* SMTC_MODEM_DM_FIELD_STATUS
* SMTC_MODEM_DM_FIELD_CHARGE
* SMTC_MODEM_DM_FIELD_VOLTAGE
* SMTC_MODEM_DM_FIELD_TEMPERATURE
* SMTC_MODEM_DM_FIELD_SIGNAL
* SMTC_MODEM_DM_FIELD_UP_TIME
* SMTC_MODEM_DM_FIELD_RX_TIME
* SMTC_MODEM_DM_FIELD_ADR_MODE
* SMTC_MODEM_DM_FIELD_JOIN_EUI
* SMTC_MODEM_DM_FIELD_INTERVAL
* SMTC_MODEM_DM_FIELD_REGION
* SMTC_MODEM_DM_FIELD_RST_COUNT
* SMTC_MODEM_DM_FIELD_DEV_EUI
* SMTC_MODEM_DM_FIELD_SESSION
* SMTC_MODEM_DM_FIELD_CHIP_EUI
* SMTC_MODEM_DM_FIELD_APP_STATUS

## Expected Behavior

Here follow the steps that shall be seen in the logs to indicate the expected behavior of the application:

### Device starts and resets

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem device management information example =====

INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
```

Following this print you shall find application and parameter prints

### Joined the network

At first run no time is supposed to be available

```
INFO: ###### ===== JOINED EVENT ==== ######
```

Once the device joined, it will set two DM message operations:

* One automated DM message sending with periodicity of `APP_MODEM_DM_INTERVAL` minutes (default: 2min); and
* One alarm-based DM message sending with periodicity of `APP_MODEM_DM_TRIGGER_INTERVAL` seconds (default: 50 seconds).

The content of the DM messages differs:

* for the automated one, it is defined by `app_modem_dm_fields`; and
* for the alarm-based one, it is defined by `app_modem_dm_fields_single`.

### Alarm-based DM message

The sending of these message is notified by the following message in the serial console:

```
INFO: ###### ===== ALARM EVENT ==== ######
Alarm-based DM uplink request done. Next alarm-based request in 50 seconds.
```

### Automated DM message

The sending of the automated DM message is notified with this message:

```
DM  - (9 bytes):
```

Note that this size in byte indicated may change depending on the content of `app_modem_dm_fields`.