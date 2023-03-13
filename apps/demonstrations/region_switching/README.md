# LoRaWAN region switching demonstration

## Description

The application will automatically starts a procedure to join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), uplinks are sent on a regular basis on port `LORAWAN_APP_PORT`. The content of the uplink is the value read out from the temperature sensor available through `smtc_modem_hal_get_temperature()`.

After each uplink, the application checks the number of consecutive uplinks without downlink thanks to `smtc_modem_lorawan_get_lost_connection_counter()`. If this value is higher or equal to `LORAWAN_MAX_LOST_CONNECTION_CNT_BEFORE_LINK_CHECK`, the application start requesting link check with `smtc_modem_lorawan_request_link_check()`. As soon as there are `LORAWAN_MAX_LINK_CHECK_KO_BEFORE_SWITCH` unanswered link checks, the application automatically leave the network and start joining a 2.4GHz network. For demonstration purpose, this behavior can be forced by pushing on the button of the dev kit board: the region switching will be done immediately.

Once a 2.4GHz network is joined, the application takes sending uplinks up again - as done previously. The difference is that the device will remain connected to this network until a properly formatted downlink is received on port `LORAWAN_APP_PORT`. When this downlink is received, the application switch back to the initial region.

## Configuration

Several parameters can be updated in `main_region_switching.h` header file:

| Constant                                            | Description                                          | Possible values | Default Value |
| --------------------------------------------------- | ---------------------------------------------------- | --------------- | ------------- |
| `LORAWAN_APP_PORT`                                  | LoRaWAN FPort used for uplink and downlink messages  | [1, 223]        | 2             |
| `LORAWAN_MAX_LOST_CONNECTION_CNT_BEFORE_LINK_CHECK` | Number of uplink without downlink before link check  | [1, 255]        | 10            |
| `LORAWAN_MAX_LINK_CHECK_KO_BEFORE_SWITCH`           | Number of unanswered link check before region switch | [1, 255]        | 4             |
| `LORAWAN_MAX_LINK_CHECK_KO_BEFORE_SWITCH`           | Number of unanswered link check before region switch | [1, 255]        | 4             |
| `APP_MODEM_REGION_SWITCHING_OPCODE`                 | Opcode of the region switching command               | [0x00, 0xFF]    | 0x70          |
| `join_eui_2g4`                                      | JoinEUI to be used when joining the 2.4GHz network   |                 |               |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Expected Behavior

Here follow the steps that shall be seen in logs to indicate the expected behavior of the application:

### Device starts and reset

 ``` shell
 INFO: Modem Initialization
 INFO: ###### ===== LoRa Basics Modem region switching demonstration ==== ######
 INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
 ```

 Following this print you shall find application and LoRaWAN parameters prints

### Device joined the network

``` shell
INFO: ###### ===== JOINED EVENT ==== ######
```

### Push button event occurred, generating uplink

``` shell
INFO: ###### ===== BUTTON PUSH ==== ######
INFO: Request uplink
```

### Send done

``` shell
INFO: ###### ===== TX DONE EVENT ==== ######
```

Shall follow a consistant number of uplinks

``` shell
INFO: Uplink count: 1
```

## Known limitations

### Join Nonce shared between networks

The value of the Join nonce used to compare to the one sent by the LNS in the join accept message is not specific to one of the network the device will join in this demonstration: it is actually the same value. Due to that implementation, the first join accept received when switching from one network to another will be rejected.
