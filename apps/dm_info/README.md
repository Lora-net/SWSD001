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

| Constant                        | Description                                                             | Note                   |
| ------------------------------- | ----------------------------------------------------------------------- | ---------------------- |
| `APP_MODEM_DM_LORAWAN_FPORT`    | LoRaWAN FPort to be used by the device management service               |                        |
| `APP_MODEM_DM_INTERVAL`         | Interval between two automatic device management information reports    |                        |
| `APP_MODEM_DM_TRIGGER_INTERVAL` | Interval between two asynchronous device management information reports | Set to 0 to disable it |

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
