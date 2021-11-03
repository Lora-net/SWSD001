# LoRa Basics Modem SDK

The LoRa Basics modem SDK contains several simple examples highlighting LoRa Basic Modem features.

There are also two demonstrations explaining how direct access to the transceiver can be interleaved while LoRa Basics modem is running. The two demonstrations are based on the geolocation features offered by the LR1110 transceiver: Wi-Fi and GNSS scanning.

## Examples

| Name                 | Description                                                           | Documentation                                 |
| -------------------- | --------------------------------------------------------------------- | --------------------------------------------- |
| Almanac update       | Perform a partial almanac update                                      | [README](apps/almanac_update/README.md)       |
| DM information       | Configure periodic DM message reports and trigger asynchronous report | [README](apps/dm_info/README.md)              |
| Geolocation - GNSS   | Perform GNSS scans interleaved with LoRaWAN operations                | [README](apps/geolocation_gnss/README.md)     |
| Geolocation - Wi-Fi  | Perform Wi-Fi scans interleaved with LoRaWAN operations               | [README](apps/geolocation_wifi/README.md)     |
| Large file upload    | Transfer a file (up to 2048 byte long) to LoRa Cloud                  | [README](apps/large_file_upload/README.md)    |
| LoRaWAN              | Send data on a periodic basis                                         | [README](apps/lorawan/README.md)              |
| LoRaWAN asynchronous | React on an asynchronous event - a putton press here - and send data  | [README](apps/lorawan_asynchronous/README.md) |
| Stream               | Send data as a stream to LoRa Cloud                                   | [README](apps/stream/README.md)               |
| Time synchronization | Get time from the network server or from LoRa Cloud                   | [README](apps/time_sync/README.md)            |

## Configuration

Each example has its own set of parameters - see `apps/<example>/main_<example>.h`.

There is also [a common configuration file](apps/common/lorawan_key_config.h) where LoRaWAN parameters can be set:

* DevEUI
* JoinEUI
* AppKey
* Class
* Region

## Requirements

### Supported platforms

LoRa Basics Modem is platform independant and can be used with any MCU that fulfills the requirements.

This SDK is developed on the following hardware:

* ST Microeletronic [NUCLEO-L476RG development board](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html)
* Semtech [LR1110MB1LBKS](https://fr.semtech.com/products/wireless-rf/lora-edge/lr1110mb1lbks) shield and [LR1110MB1LCKS](https://fr.semtech.com/products/wireless-rf/lora-edge/lr1110mb1lcks) shield

### Toolchain

Examples can be compiled with either [Keil MDK ARM](https://www2.keil.com/mdk5) or [GNU Arm Embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm).

### Firmware

The LoRa Basics Modem library requires the LR1110 runs the transceiver firmware version 0x0307 ([available here](https://github.com/Lora-net/radio_firmware_images/tree/master/lr1110/transceiver)).

To update the transceiver with the desired firmware version, please use [the updater tool application](https://github.com/Lora-net/lr1110_updater_tool/).

## Getting started

### Configure

Before starting to build an example, check the parameters in [the common configuration file](apps/common/lorawan_key_config.h). Make sure that the LoRaWAN configuration is consistent with what is configured in your network server and in your LoRa Cloud account.

Parameters can be seen in the [debug output](#view-debug-output).

### Build

#### Keil MDK ARM

Each example is delivered with a Keil project file - see `apps/MDK-ARM/lbm_example_<example>.uvprojx`. For instance, the Keil poject file for the LoRaWAN example is available in `$SDK_FOLDER/apps/lorawan/MDK-ARM/lbm_example_lorawan.uvprojx`.

Launch Keil IDE, open the project file and compile it.

Each project has 3 targets ([Keil manual](https://www.keil.com/support/man/docs/uv4/uv4_ca_projtargfilegr.htm)), each one allowing to chose how cryptographic operations are performed and where the LoRaWAN parameters are:

| Target name                                | Cryptographic operations    | LoRaWAN parameters |
| ------------------------------------------ | --------------------------- | ------------------ |
| `<example>_crypto_sw`                      | Software                    | User-defined       |
| `<example>_crypto_lr1110`                  | LR1110 cryptographic engine | User-defined       |
| `<example>_crypto_lr1110_with_credentials` | LR1110 cryptographic engine | LR1110-based       |

#### GNU Arm embedded toolchain

Examples are built from their respective subfolder in the `apps` directory. For instance, the makefile for the LoRaWAN example is available in `$SDK_FOLDER/apps/lorawan/makefile/Makefile`.

Build settings, compile time and configuration options are specified in the project's Makefile.

To build a project, simply run make

```shell
$ cd $SDK_FOLDER/apps/lorawan/makefile
$ make
```

The output files of the build process are stored in the `build` folder with firmware binary file having the same name as the project with a .bin extension.

It is possible to choose the cryptographic mode with the CRYPTO variable:

| CRYPTO value            | Cryptographic operations    | LoRaWAN parameters |
| ----------------------- | --------------------------- | ------------------ |
| SOFT                    | Software                    | User-defined       |
| LR1110                  | LR1110 cryptographic engine | User-defined       |
| LR1110_WITH_CREDENTIALS | LR1110 cryptographic engine | LR1110-based       |

For instance, to build the project `lorawan` with software-based cryptographic operations and user-defined LoRaWAN parameters:

To build a project, simply run make

```shell
$ cd $SDK_FOLDER/apps/lorawan/makefile
$ make CRYPTO=SOFT
```

The default value for all examples is set to `LR1110`, except for geolocation examples (`geolocation_gnss` and `geolocation_wifi`) where the CRYPTO value is set to `SOFT`.

### Load

After a project is built, it can be loaded onto a device.

There are multiple ways to do it:

* Drag and drop the binary file to the USB drive liste by our OS - usually shows up as `NODE_L476RG`.
* Load it through the Keil IDE

### View debug output

On the NUCLEO-L476RG development board, the firmware prints debug information to the UART that is connected via the ST-LINK to the host computer. The configuration is 921600/8-N-1:

* On Linux, this device usually shows up as `/dev/ttyACM0`
* On Windows, the port can be obtained from the device manager

For instance, using stty on Linux:

```shell
$ stty -echo raw speed 921600 < /dev/ttyACM0 && cat /dev/ttyACM0

INFO: ###### ===== LoRa Basics Modem LoRaWAN Class A/C demo application ==== ######

INFO: LoRaWAN version: 01.00.04.01
INFO: LoRa Basics Modem version: 02.01.00
INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
Reset count : 95 
INFO: Application parameters:
INFO:   - LoRaWAN uplink Fport = 2
INFO:   - DM report interval   = 60
INFO:   - Confirmed uplink     = No
INFO: LoRaWAN parameters:
DevEUI - (8 bytes):
 00 16 C0 00 00 00 00 00
JoinEUI - (8 bytes):
 00 16 C0 01 FF FE 00 01
Class: A
Region: EU868

INFO: ###### ===== JOINED EVENT ==== ######
```
