# LoRaWAN application sending asynchronous LR-FHSS uplinks

This example only applies to the following regions: EU-868, US-915, AU-915.

## Description

The application will automatically starts a procedure to join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), uplinks can be requested by pushing on the button of the dev kit board. If the device is not joined, no uplink will be sent.

Upon reception of any LinkADRReq command from the network server, the application will request the list of available datarates from LoRa Basics Modem. If LR-FHSS datarates are available, a custom ADR profile will be programmed so that all future uplinks will be transmitted over LR-FHSS. If a LinkADRReq command is never received, or if no LR-FHSS datarates are available, the application will not change the ADR profile and LoRa uplinks will be transmitted.

The content of the uplink is the value read out from the charge counter by calling `smtc_modem_get_charge()`.

The application is also capable of displaying data and meta-data of a received downlink.

Note that for this application to work as expected for the EU-868 region, the LoRaWAN network server must first configure LR-FHSS channels using the NewChannelReq command, and then activate those channels for LR-FHSS datarates 8-9, 10-11, or 8-11 in the channel mask using the LinkADRReq command. This is not standard network server behavior, and requires a custom network server installation, as described below.

For this application to work as expected for the US-915 region, the LoRaWAN network server must activate one of the TX channels 64-71 for at least one of the LR-FHSS datarates 5-6 in the channel mask using the LinkADRReq command. This may require a custom network server installation, as described below.

For this application to work as expected for the AU-915 region, the LoRaWAN network server must activate one of the TX channels 64-71 for the LR-FHSS datarate 7 in the channel mask using the LinkADRReq command. This may require a custom network server installation, as described below.

### Known limitations

For the US-915 and AU-915 regions, this application currently must use a relatively wide Rx1 receive window. In order to do this, `smtc_modem_set_crystal_error_ppm()` is called during LoRa Basics Modem initialization.

This demo application only supports the cases where datarates 8-9, 10-11, or 8-11 are supported by the gateway.

### Prerequisites

This example assumes that a ChirpStack network server stack has been downloaded and installed. The stack can be installed on a Debian or Ubuntu machine, or on a Raspberry Pi, as described [here](https://www.chirpstack.io/project/guides/debian-ubuntu/).

You must follow the 'Connecting a gateway' and 'Connecting a device' instructions as well, defining a device supporting LoRaWAN Mac version 1.0.4, with regional parameters revision 'RP002-1.0.3', ADR algorithm 'LoRa and LR-FHSS ADR algorithm', and 'Device supports OTAA'.

## Configuration

Several parameters can be updated in `main_lorawan_asynchronous.h` header file:

| Constant                   | Description                                                                   | Possible values  | Default Value |
| -------------------------- | ----------------------------------------------------------------------------- | ---------------- | ------------- |
| `LORAWAN_APP_PORT`         | LoRaWAN FPort used for the uplink messages                                    | [1, 223]         | 2             |
| `LORAWAN_CONFIRMED_MSG_ON` | Request a confirmation from the LNS that the uplink message has been received | {`true`,`false`} | `false`       |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

## Expected Behavior for EU-868 region

Here follow the steps that shall be seen in logs to indicate the expected behavior of the application:

### Device starts and reset

 ```
 INFO: Modem Initialization
 Region = US915
 INFO: ###### ===== LoRa Basics Modem LoRaWAN LR-FHSS asynchronous demo application ==== ######
 INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
 ```

### Pushbutton pressed before join

```
INFO: ###### ===== BUTTON PUSH ==== ######
Modem status: JOINING 
INFO: Cannot send uplink until joined
```

### Device joined the network

```
INFO: ###### ===== JOINED EVENT ==== ######
```

### NewChannelReq command is received from the network server

A NewChannelReq command is received by the network server:

```
  *************************************
  * Receive a Valid downlink RX1 for Hook Id = 3
  *************************************
 Cmd new_channel_parser = 8 d0 5a 84 98
MacTxFrequency [ 8 ] = 867200000, DrMin = 8, DrMax = 9
 Cmd new_channel_parser = 9 b0 89 84 ba
MacTxFrequency [ 9 ] = 868600000, DrMin = 10, DrMax = 11
```

In this particular case, the new channel 8 is configured to use LR-FHSS datarates 8 and 9, and the new channel 9 is configured to use LR-FHSS datarates 10 and 11.

### LinkADRReq command is received from the network server

A LinkADRReq command is received by the network server:

```
  *************************************
  * Receive a Valid downlink RX1 for Hook Id = 3
  *************************************
0/1 - Cmd link_adr_parser = 53 ff 03 01
0 - MULTIPLE LINK ADR REQ , channel mask = 0x3ff , ChMAstCntl = 0x0
UnwrappedChannelMask = 0xff 3 , ChMask = 0x3ff
ch0 - dr field 0x003f
 1  1  1  1  1  1  1  1  1  1  0  0  0  0  0  0  
MacTxDataRateAdr = 5
MacTxPower = 8
MacNbTrans = 1
...
INFO: ###### ===== NEW LINK ADR EVENT ==== ######
INFO: LR-FHSS datarates 8-11 are available

```

In this particular case, the new LR-FHSS channels 8 and 9 shown above are enabled (in addition to channels 0 through 7). These two channels together permit the use of LR-FHSS datarates 8, 9, 10, and 11.

### Push button event occurred, generating an LR-FHSS uplink
```
INFO: ###### ===== BUTTON PUSH ==== ######
Modem status: JOINED 

INFO: Request uplink
 User Tx LORa on FPort 2 

  *************************************
  * Send Payload  HOOK ID = 3
  *************************************
  Tx  LR FHSS at 837151 ms: freq:868600000, DR11 (CR 2/3, BW 335938 Hz), len 17 bytes, 14 dBm, fcnt_up 4, toa = 973
```

### Send done

```
INFO: ###### ===== TX DONE EVENT ==== ######
```
Will be followed by the uplink count, which is incremented on each uplink.

```
INFO: Uplink count: 1
```

## Expected Behavior for US-915 region

Here follow the steps that shall be seen in logs to indicate the expected behavior of the application:

### Device starts and reset

 ```
 INFO: Modem Initialization
 Region = US915
 INFO: ###### ===== LoRa Basics Modem LoRaWAN LR-FHSS asynchronous demo application ==== ######
 INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
 ```

### Pushbutton pressed before join

```
INFO: ###### ===== BUTTON PUSH ==== ######
Modem status: JOINING 
INFO: Cannot send uplink until joined
```

### Device joined the network

```
INFO: ###### ===== JOINED EVENT ==== ######
```

### LinkADRReq command is received from the network server

For the US-915 region, a LinkADRReq command is received by the network server:

```
  *************************************
  * Receive a Valid downlink RX1 for Hook Id = 3
  *************************************
0/2 - Cmd link_adr_parser = 00 10 00 70
1/2 - Cmd link_adr_parser = 33 00 ff 01
0 - MULTIPLE LINK ADR REQ , channel mask = 0x10 , ChMAstCntl = 0x7
ChCtrl = 0x7, ChMask = 0x0010
unwrapped channel 125 tx mask = 0x00 00 00 00 00 00 00 00  
unwrapped channel 500 tx mask = 0x10
1 - MULTIPLE LINK ADR REQ , channel mask = 0xff00 , ChMAstCntl = 0x0
ChCtrl = 0x0, ChMask = 0xff00
unwrapped channel 125 tx mask = 0x00 ff 00 00 00 00 00 00  
unwrapped channel 500 tx mask = 0x10
Ch 125kHz
 0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  
Ch 500kHz
 0  0  0  0  1  0  0  0  
MacTxDataRateAdr = 3
MacTxPower = 8
MacNbTrans = 1
...
INFO: ###### ===== NEW LINK ADR EVENT ==== ######
INFO: LR-FHSS datarates 5-6 are available
```

In this particular case, the 500-kHz-LoRa/1523-kHz-LR-FHSS channel 68 (64 + 4) shown above is enabled (in addition to various 125 kHz LoRa channels). This channel permits the use of LR-FHSS datarates 5 and 6.

### Push button event occurred, generating an LR-FHSS uplink
```
INFO: ###### ===== BUTTON PUSH ==== ######
Modem status: JOINED 

INFO: Request uplink
  ...

  *************************************
  * Send Payload  HOOK ID = 3
  *************************************
  Tx  LR FHSS at 59482 ms: freq:909400000, DR6 (CR 2/3, BW 1523438 Hz), len 17 bytes, 14 dBm, fcnt_up 4, toa = 973
```

### Send done

```
INFO: ###### ===== TX DONE EVENT ==== ######
```
Will be followed by the uplink count, which is incremented on each uplink.

```
INFO: Uplink count: 1
```

## Expected Behavior for AU-915 region

Here follow the steps that shall be seen in logs to indicate the expected behavior of the application:

### Device starts and reset

 ```
 INFO: Modem Initialization
 Region = AU915
 INFO: ###### ===== LoRa Basics Modem LoRaWAN LR-FHSS asynchronous demo application ==== ######
 INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
 ```

### Pushbutton pressed before join

```
INFO: ###### ===== BUTTON PUSH ==== ######
Modem status: JOINING 
INFO: Cannot send uplink until joined
```

### Device joined the network

```
INFO: ###### ===== JOINED EVENT ==== ######
```

### LinkADRReq command is received from the network server

For the AU-915 region, a LinkADRReq command is received by the network server:

```
  *************************************
  * Receive a Valid downlink RX1 for Hook Id = 3
  *************************************
0/2 - Cmd link_adr_parser = 00 01 00 70
1/2 - Cmd link_adr_parser = 53 ff ff 01
0 - MULTIPLE LINK ADR REQ , channel mask = 0x1 , ChMAstCntl = 0x7
ChCtrl = 0x7, ChMask = 0x0001
unwrapped channel 125 tx mask = 0x00 00 00 00 00 00 00 00  
unwrapped channel 500 tx mask = 0x01
1 - MULTIPLE LINK ADR REQ , channel mask = 0xffff , ChMAstCntl = 0x0
ChCtrl = 0x0, ChMask = 0xffff
unwrapped channel 125 tx mask = 0xff ff 00 00 00 00 00 00  
unwrapped channel 500 tx mask = 0x01
Ch 125kHz
 1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  
Ch 500kHz
 1  0  0  0  0  0  0  0  
MacTxDataRateAdr = 5
MacTxPower = 8
MacNbTrans = 1
...
INFO: ###### ===== NEW LINK ADR EVENT ==== ######
INFO: LR-FHSS datarate1 7 is available
```

In this particular case, the 500-kHz-LoRa/1523-kHz-LR-FHSS channel 64 (64 + 0) shown above is enabled (in addition to various 125 kHz LoRa channels). This channel permits the use of LR-FHSS datarate 7.

### Push button event occurred, generating an LR-FHSS uplink
```
INFO: ###### ===== BUTTON PUSH ==== ######
Modem status: JOINED 

INFO: Request uplink
...

  *************************************
  * Send Payload  HOOK ID = 3
  *************************************
  Tx  LR FHSS at 253349 ms: freq:915900000, DR7 (CR 1/3, BW 1523438 Hz), len 17 bytes, 14 dBm, fcnt_up 4, toa = 1713
```

### Send done

```
INFO: ###### ===== TX DONE EVENT ==== ######
```
Will be followed by the uplink count, which is incremented on each uplink.

```
INFO: Uplink count: 1
```

## ChirpStack Network Server configuration for LR-FHSS operation in the EU-868 region

In order to get the network server to create the necessary LR-FHSS channels on the end device, it is necessary to modify the `/etc/chirpstack-network-server/chirpstack-network-server.toml` file.

For the purposes of this demo, assume that your gateway is properly configured for the EU-868 region, and it has the following LR-FHSS channel configuration:

```
{ "freq_hz": 867200000, "ocw": 137000 },
{ "freq_hz": 867400000, "ocw": 137000 },
{ "freq_hz": 867600000, "ocw": 137000 },
{ "freq_hz": 867800000, "ocw": 137000 },
{ "freq_hz": 868000000, "ocw": 137000 },
{ "freq_hz": 868200000, "ocw": 137000 },
{ "freq_hz": 868600000, "ocw": 336000 } 
```

According to `RP002-1.0.3 LoRaWAN Regional Parameters`, datarates 8 and 9 occupy 137 kHz, and datarates 10 and 11 occupy 336 kHz.

If you wish to configure the network server to place datarates 8 and 9 in the 137 kHz channel at 867200000 Hz, add the following lines to the `[network_server.network_settings]` section. 
```
    [[network_server.network_settings.extra_channels]]
    frequency=867200000
    min_dr=8
    max_dr=9
```

If you wish to configure the network server to place datarates 10 and 11 in the 336 kHz channel at 868600000 Hz, add the following lines to the `[network_server.network_settings]` section. 
```
    [[network_server.network_settings.extra_channels]]
    frequency=868600000
    min_dr=10
    max_dr=11
```

Make sure that name='EU868' in the '[network_server.band]' section

Once this file has been edited, restart the network server:
``` bash
$ systemctl restart chirpstack-network-server
```

## ChirpStack Network Server configuration for LR-FHSS operation in the US-915 region

According to `RP002-1.0.3 LoRaWAN Regional Parameters`, datarates 5 and 6 occupy 1523 kHz.

For the purposes of this demo, assume that your gateway is properly configured for the US-915 region, and it has the following LR-FHSS channel configuration:

```
{ "freq_hz": 909400000, "ocw": 1523000 } /* 1523kHz channel */
```

Any of the 8 LR-FHSS upstream channels described in 'RP002-1.0.3 LoRaWAN Regional Parameters' can be used (903.0 to 914.2, stepping by 1.6 MHz), and depending on your gateway, you may be able to enable more than one. Here, we have chosen the 4th LR-FHSS channel, at 909.4 MHz (903.0 + 4 * 1.6  = 909.4). This corresponds to the 500-kHz-LoRa/1523-kHz-LR-FHSS channel 68 (64 + 4).

In order to get the network server to activate the necessary LR-FHSS channels on the end device, it may be necessary to modify the `/etc/chirpstack-network-server/chirpstack-network-server.toml` file. Make sure that the 'enabled_uplink_channels' variable of section '[network_server.network_settings]' contains channel 68, and that name='US_902_928' in the '[network_server.band]' section.

If it was necessary to change this file, restart the network server:
``` bash
$ systemctl restart chirpstack-network-server
```

## ChirpStack Network Server configuration for LR-FHSS operation in the AU-915 region

According to `RP002-1.0.3 LoRaWAN Regional Parameters`, datarate 7 occupies 1523 kHz.

For the purposes of this demo, assume that your gateway is properly configured for the AU-915 region, and it has the following LR-FHSS channel configuration:

```
{ "freq_hz": 915900000, "ocw": 1523000 } /* 1523kHz channel */
```

Any of the 8 LR-FHSS upstream channels described in 'RP002-1.0.3 LoRaWAN Regional Parameters' can be used (915.9 to 927.1, stepping by 1.6 MHz), and depending on your gateway, you may be able to enable more than one. Here, we have chosen the first LR-FHSS channel, at 915.9 MHz. This corresponds to the 500-kHz-LoRa/1523-kHz-LR-FHSS channel 64.

In order to get the network server to activate the necessary LR-FHSS channels on the end device, it may be necessary to modify the `/etc/chirpstack-network-server/chirpstack-network-server.toml` file. Make sure that the 'enabled_uplink_channels' variable of section '[network_server.network_settings]' contains channel 64, and that name='AU915' in the '[network_server.band]' section.

If it was necessary to change this file, restart the network server:
``` bash
$ systemctl restart chirpstack-network-server
```
