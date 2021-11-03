# LoRa Basics Modem common content

## Description

In order to bring some clarity, several generic operations (i.e. operations that are used no matter the LoRa Basics Modem functionality being demonstrated in an example) are implemented in this common folder.

## LoRaWAN configuration

In this SDK, the LoRaWAN configuration is handled by the function `apps_modem_common_configure_lorawan_params()`.

The join parameters (DevEUI, JoinEUI and AppKey) can be taken from different places:

* if `USER_DEFINED_JOIN_PARAMETERS` is defined: the join parameters are taken from the `lorawan_key_config.h` header file
* if `LR1110_DEFINED_JOIN_PARAMETERS` is defined: the join parameters are taken from the LR1110 and the application key is derived according to the algorithm described in LoRa Cloud.

In the user-defined case, the configuration has to be updated in `lorawan_key_config.h` header file:

| Constant             | Description             | Note                                                     |
| -------------------- | ----------------------- | -------------------------------------------------------- |
| `LORAWAN_DEVICE_EUI` | LoRaWAN device EUI      | Used if `USER_DEFINED_JOIN_PARAMETERS` is defined        |
| `LORAWAN_JOIN_EUI`   | LoRaWAN join EUI        | Used if `USER_DEFINED_JOIN_PARAMETERS` is defined        |
| `LORAWAN_APP_KEY`    | LoRaWAN application key | Used if `USER_DEFINED_JOIN_PARAMETERS` is defined        |
| `LORAWAN_REGION`     | LoRaWAN region          | Only regions EU868, US915 and CN470 (RP-1) are supported |
| `LORAWAN_CLASS`      | LoRaWAN class           | Only classes A and C are supported                       |

## LoRa Basics Modem event management

When LoRa Basics Modem is initialized, a callback is given as parameter to `smtc_modem_init()` so the application can be informed of events. In a final application, it is  up to the user to implement this function.

In this SDK, this function is pre-defined to bring consistency - it is `apps_modem_event_process()` implemented in [this file](./apps_modem_event.c)). Each example needs to implement the functions defined in `apps_modem_event_callback_t` that are useful to it - others have to be set to `NULL`.
