# LoRa Basics Modem LoRaWAN Multicast Class B example

## Description

The application will automatically starts a procedure to join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), the example will perform the requested operations to enable class B:

- Enable the time synchronization service - only `SMTC_MODEM_TIME_MAC_SYNC` is supported
- Request a ping slot update from the LNS

Once the time is updated and the ping slot update request is answered by the LNS, the example enables the class B. When the event indicating that class B is ready, the example starts a multicast session.

The example is also capable of displaying data and meta-data of a received downlink.

This example is based on the [LoRaWAN class B example](../../examples/lorawan_class_b/README.md).

## Configuration

Several parameters can be updated in `main_lorawan_multicast_class_b.h` header file:

| Constant                        | Description                                                                           | Possible values                                              | Default Value                      |
| ------------------------------- | ------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ---------------------------------- |
| `LORAWAN_CLASS_B_PING_SLOT`     | LoRaWAN Class B ping slot periodicity                                                 | Any value of `smtc_modem_class_b_ping_slot_periodicity_t`    | `SMTC_MODEM_CLASS_B_PINGSLOT_16_S` |
| `TIME_SYNC_APP_INTERVAL_S`      | Interval between 2 time synchronization once time is acquired                         | `uint32_t`                                                   | 86400                              |
| `TIME_SYNC_APP_INVALID_DELAY_S` | Invalid delay for time sync                                                           | `uint32_t`                                                   | 3 * 86400                          |
| `MULTICAST_GROUP`               | Multicast group id                                                                    | [0,3]                                                        | 0                                  |
| `MULTICAST_GROUP_ADDRESS`       | Multicast group address coinciding with the one used by the network server            | `uint32_t`                                                   | 0x00000000                         |
| `MULTICAST_SESSION_FREQUENCY`   | Multicast session frequency, coinciding with the one used by the network server       | `uint32_t`, min and max value depends on the region selected | 869525000                          |
| `MULTICAST_SESSION_DATARATE`    | Multicast session datarate, coinciding with the one used by the network server        | `uint8_t`, min and max values depend on the configuration    | 4                                  |
| `MULTICAST_NWK_SESSION_KEY`     | Multicast network session key, coinciding with the one used by the network server     | 16 bytes c-array                                             | All zeros                          |
| `MULTICAST_APP_SESSION_KEY`     | Multicast application session key, coinciding with the one used by the network server | 16 bytes c-array                                             | All zeros                          |
| `MULTICAST_SESSION_PING_SLOT`   | Multicast session ping slot, coinciding with the one used by the network server       | Any value from `smtc_modem_class_b_ping_slot_periodicity_t`  | `SMTC_MODEM_CLASS_B_PINGSLOT_32_S` |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Example with TTNv3

### Introduction

Multicast configuration depends on specific network server configuration and capabilities. This paragraph describes how to configure and run this example when used with The Things Stack version 3 (TTNv3).

It is assumed that a functional class B device `sample-app-1-class-b-dev` belonging to the application `sample-app-1` has already been configured and tested, connected to TTN LoRa Network Server through the 'sample-gateway-1' gateway.

### Prerequisites

This examples is based on the Command-Line Interface (CLI) of TTNv3 to create and configure the multicast functionality - see [instructions](https://www.thethingsindustries.com/docs/getting-started/cli/) to install it. The same operations can be done with the web interface of The Things Stack (the Console).

A message brocker that implements MQTT protocol is also required to send multicast messages to end devices. In this example, Mosquitto is used - see [instructions](https://mosquitto.org/download/) to install it.

### Create a multicast group

First, set values for the `MULTICAST_GROUP_ADDRESS`, `MULTICAST_NWK_SESSION_KEY`, and `MULTICAST_APP_SESSION_KEY` parameters, above. Also, select appropriate `MULTICAST_SESSION_FREQUENCY`, `MULTICAST_SESSION_DATARATE` and `MULTICAST_SESSION_PING_SLOT` for your example. These values will be reused to configure the multicast functionality in TTN - described in the next paragraph.

To add multicast functionality to `sample-app-1-class-b-dev` on TTN, create a multicast dummy device named `sample-app-1-multicast-dev` (the name does not matter). The dummy device is a device that serves as a place for the multicast parameters to be stored. When creating a dummy device, instead of creating the device as an OTAA or ABP device, create the device as a multicast device and configure it with the values defined above.

For instance, from a command line with a previously-configured TTN CLI:

``` bash
$ ttn-lw-cli login

$ APP_ID="sample-app-1"
$ DEVICE_ID="sample-app-1-multicast-dev"
$ FREQUENCY_PLAN="EU_863_870_TTN"
$ LORAWAN_VERSION="MAC_V1_0_3"
$ LORAWAN_PHY_VERSION="PHY_V1_0_3_REV_A"
$ MULTICAST_GROUP_ADDRESS="87E1464D"
$ MULTICAST_SESSION_DATARATE="DATA_RATE_4"
$ MULTICAST_SESSION_FREQUENCY="869525000"
$ MULTICAST_APP_SESSION_KEY="00000000000000000000000000000000"
$ MULTICAST_NWK_SESSION_KEY="00000000000000000000000000000000"
$ MULTICAST_SESSION_PING_SLOT="PING_EVERY_16S"

$ ttn-lw-cli end-devices create $APP_ID $DEVICE_ID \
           --frequency-plan-id $FREQUENCY_PLAN \
           --lorawan-version $LORAWAN_VERSION \
           --lorawan-phy-version $LORAWAN_PHY_VERSION \
           --session.dev-addr $MULTICAST_GROUP_ADDRESS \
           --session.keys.app-s-key.key $MULTICAST_APP_SESSION_KEY \
           --session.keys.nwk-s-key.key $MULTICAST_NWK_SESSION_KEY \
           --multicast \
           --mac-settings.ping-slot-periodicity $MULTICAST_SESSION_PING_SLOT \
           --mac-settings.ping-slot-data-rate-index $MULTICAST_SESSION_DATARATE \
           --mac-settings.ping-slot-frequency $MULTICAST_SESSION_FREQUENCY \
           --supports-class-b
```

This can also be done from the web interface.

### Update a multicast group

After a multicast dummy device is created, you may wish to change multicast communication parameters. When doing this, you may need to reset the session and MAC state in order to get the network server to use the updated parameters. For instance, let's imagine you need to change the multicast session datarate to index 3.

From the command line, this can be done by typing:

``` bash
$ ttn-lw-cli end-devices set 'sample-app-1' 'sample-app-1-multicast-dev' --mac-settings.ping-slot-data-rate-index 3
$ ttn-lw-cli end-devices reset 'sample-app-1' 'sample-app-1-multicast-dev'
```

In the web interface, it will be necessary to change the `Ping slot data rate index` field, found under `General Settings -> Network layer -> Advanced MAC settings`, and then select `Reset session and MAC state`.

## Send a message to a multicast group

After your multicast dummy device has been created and properly configured, compile and start this example and wait for it to join the TTN network. Then, determine the gateway_id of a reachable gateway, and schedule a downlink that specifies the proper downlink path. For example, the following JSON message can be transmitted to TTN through either the Application Server MQTT server or HTTP webhooks:

``` json
{
   "downlinks" : [
      {
         "class_b_c" : {
            "gateways" : [
               {
                  "gateway_ids" : {
                     "gateway_id" : "sample-gateway-1"
                  }
               }
            ]
         },
         "f_port" : 42,
         "frm_payload" : "bXVsdGljYXN0Cg==",
         "priority" : "NORMAL"
      }
   ]
}
```

Assuming that mosquitto has been installed on a local client machine, and that an MQTT integration has been configured on TTN (see [instructions](https://www.thethingsindustries.com/docs/integrations/mqtt/)), this can be done using the Linux command line (after adapting the application, dummy device, token, and server names accordingly):

``` bash
mosquitto_pub -h eu1.cloud.thethings.network   -t "v3/sample-app-1@ttn/devices/sample-app-1-multicast-dev/down/push"   -u "sample-app-1@ttn" -P "NNSXS.MN..." -d  -m '{"downlinks":[{"class_b_c":{"gateways":[{"gateway_ids":{"gateway_id":"sample-gateway-1"}}]},"f_port":42,"frm_payload":"bXVsdGljYXN0Cg==","priority":"NORMAL"}]}'
```

## Troubleshooting

### Multicast Class B downlinks are not received with high multicast datarate configuration

As the MCU crystal is used to count the different delays to wait when opening RX windows, MCU crystal drift translates in errors when opening RX windows.
LBM compensates the MCU crystal drift attached to the LBM by opening RX windows a bit earlier, and a bit longer.
The compensation is computed from the expected crystal error (default 10ppm, configurable with `smtc_modem_set_crystal_error_ppm`).

However in the case of multicast class B ping slots, the default value may be too low with respect to the MCU crystal used by LBM. Therefore the compensation computed is not enough to ensure the ping slots RX windows are opened when the gateway start sending the ping slot packet.

The effect is even more observable with high datarates as the preamble is shorter (in term of duration).

Therefore if when using multicast class B with high datarate ping slots are not received, it is advised to increase the crystal error estimation value with `smtc_modem_set_crystal_error_ppm` function.

Note that increasing this value will lead to an overconsumption as the ping slot windows will open earlier and last longer.
The value of crystal error estimation must then be evaluated by balancing PER on multicast class B downlinks and consumption.

## Limitation

Even though the multicast class B example can run with the region configured to `SMTC_MODEM_REGION_WW2G4`, it is not supported and has not been tested.
