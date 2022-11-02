# LoRa Basics Modem LoRaWAN Class C device with multicast

## Description

The application will automatically starts a procedure to join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), a multicast group is configured, and a multicast session using that multicast group is started. At this point, the application is ready to receive multicast downlinks. Otherwise, this application is similar to `lorawan` example, and the application periodically requests uplinks each time the LoRa Basics Modem alarm event is triggered.

The content of the uplink is the value read out from the charge counter by calling `smtc_modem_get_charge()`.

The application displays data and meta-data of standard and multicast downlinks.

## Configuration

Several parameters can be updated in `main_lorawan_multicast_class_c.h` header file:

| Constant                      | Description                                                                           | Possible values                                              | Default Value |
| ----------------------------- | ------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ------------- |
| `LORAWAN_APP_PORT`            | LoRaWAN FPort used for the uplink messages                                            | [1,223]                                                      | 2             |
| `LORAWAN_CONFIRMED_MSG_ON`    | Request a confirmation from the LNS that the uplink message has been received         | {`true`,`false`}                                             | `false`       |
| `APP_TX_DUTYCYCLE`            | Delay in second between two uplinks                                                   |                                                              | 60            |
| `MULTICAST_GROUP`             | Multicast group id                                                                    | [0,3]                                                        | 0             |
| `MULTICAST_GROUP_ADDRESS`     | Multicast group address coinciding with the one used by the network server            | `uint32_t`                                                   | 0x00000000    |
| `MULTICAST_SESSION_FREQUENCY` | Multicast session frequency, coinciding with the one used by the network server       | `uint32_t`, min and max value depends on the region selected | 869525000     |
| `MULTICAST_SESSION_DATARATE`  | Multicast session datarate, coinciding with the one used by the network server        | `uint8_t`, min and max values depend on the configuration    | 4             |
| `MULTICAST_NWK_SESSION_KEY`   | Multicast network session key, coinciding with the one used by the network server     | 16 bytes c-array                                             | All zeros     |
| `MULTICAST_APP_SESSION_KEY`   | Multicast application session key, coinciding with the one used by the network server | 16 bytes c-array                                             | All zeros     |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

Multicast configuration depends on specific network server configuration and capabilities. This document describes how to configure and run this example when used with a version 3 TTN stack. It is assumed that a functional class C device `sample-app-1-class-c-dev` belonging to the TTN app `sample-app-1` has already been configured and tested, connected to TTN through the 'sample-gateway-1' gateway.

First, choose unique values for the `MULTICAST_GROUP_ADDRESS`, `MULTICAST_NWK_SESSION_KEY`, and `MULTICAST_APP_SESSION_KEY` parameters, above. Also, select appropriate `MULTICAST_SESSION_FREQUENCY` and `MULTICAST_SESSION_DATARATE` for your application.

To add multicast functionality to `sample-app-1-class-c-dev`, create a multicast dummy device named `sample-app-1-multicast-dev`. The dummy device is a device that serves as a place for the multicast parameters to be stored. When creating the dummy device `sample-app-1-multicast-dev`, instead of creating the device as an OTAA or ABP device, create the device as a multicast device. Select a unique multicast group address, network session key, and application session key. Select the multicast frequency and datarate that you will use in your multicast session. This can be done from the TTN web interface, or by using a previously-configured [TTN CLI](https://www.thethingsindustries.com/docs/getting-started/cli/) command line client. For instance, from a Linux command line:

```
$ ttn-lw-cli login

$ APP_ID="sample-app-1" 
$ DEVICE_ID="sample-app-1-multicast-dev"
$ FREQUENCY_PLAN="EU_863_870_TTN"
$ LORAWAN_VERSION="MAC_V1_0_3"
$ LORAWAN_PHY_VERSION="PHY_V1_0_3_REV_A"
$ MULTICAST_GROUP_ADDRESS="00000000"
$ MULTICAST_SESSION_DATARATE="DATA_RATE_4"
$ MULTICAST_SESSION_FREQUENCY="869525000"
$ MULTICAST_APP_SESSION_KEY="00000000000000000000000000000000"
$ MULTICAST_NWK_SESSION_KEY="00000000000000000000000000000000"

$ ttn-lw-cli end-devices create $APP_ID $DEVICE_ID \
           --frequency-plan-id $FREQUENCY_PLAN \
           --lorawan-version $LORAWAN_VERSION \
           --lorawan-phy-version $LORAWAN_PHY_VERSION \
           --session.dev-addr $MULTICAST_GROUP_ADDRESS \
           --session.keys.app-s-key.key $MULTICAST_APP_SESSION_KEY \
           --session.keys.nwk-s-key.key $MULTICAST_NWK_SESSION_KEY \
           --multicast \
           --mac-settings.rx2-data-rate-index $MULTICAST_SESSION_DATARATE \
           --mac-settings.rx2-frequency $MULTICAST_SESSION_FREQUENCY \
           --supports-class-c
```

After a multicast dummy device is created, you may wish to change multicast communication parameters. When doing this, you may need to reset the session and MAC state in order to get the network server to use the updated parameters. For instance, if you need to change the multicast session datarate it will be necessary to change the `Rx2 data rate index` field, found under `General Settings -> Network layer -> Advanced MAC settings`, and then select `Reset session and MAC state` in the web interface. From the command line, this can be done by typing:

```
$ ttn-lw-cli end-devices set 'sample-app-1' 'sample-app-1-multicast-dev' --mac-settings.rx2-data-rate-index 3
$ ttn-lw-cli end-devices reset 'sample-app-1' 'sample-app-1-multicast-dev'
```

The CLI command line help provides information about the various key/value pairs that can be set.

## Demo execution

After your multicast dummy device has been configured, compile and start this application and wait for it to join the TTN network. Then, determine the gateway_id of a reachable gateway, and schedule a downlink that specifies the proper downlink path. For example, the following JSON message can be transmitted to TTN through either the Application Server MQTT server or HTTP webhooks:

```
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


Assuming that [mosquitto](https://mosquitto.org/) has been installed on a local client machine, and that an MQTT integration has been configured on TTN, this can be done using the Linux command line (after adapting the application, dummy device, token, and server names accordingly):

```
mosquitto_pub -h eu1.cloud.thethings.network   -t "v3/sample-app-1@ttn/devices/sample-app-1-multicast-dev/down/push"   -u "sample-app-1@ttn" -P "NNSXS.MN..." -d  -m '{"downlinks":[{"class_b_c":{"gateways":[{"gateway_ids":{"gateway_id":"sample-gateway-1"}}]},"f_port":42,"frm_payload":"bXVsdGljYXN0Cg==","priority":"NORMAL"}]}'
```

## Limitation

Even though the multicast class C example can run with the region configured to `SMTC_MODEM_REGION_WW2G4`, it is not supported.
