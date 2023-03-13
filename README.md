# LoRa Basics Modem SDK

The LoRa Basics modem SDK contains several simple examples highlighting LoRa Basic Modem features.

## Examples and demonstrations

The SDK is made of two different kind of projects:

* Examples, each demonstrating a specific feature of LoRa Basics Modem
* Demonstrations, more elaborated than the examples

Here is the list of the different examples available in the SDK:

| Name                        | Description                                                             | Documentation                                               |
| --------------------------- | ----------------------------------------------------------------------- | ----------------------------------------------------------- |
| Almanac update              | Perform a partial almanac update                                        | [README](apps/examples/almanac_update/README.md)            |
| DM information              | Configure periodic DM message reports and trigger asynchronous report   | [README](apps/examples/dm_info/README.md)                   |
| Large file upload           | Transfer a file (up to 2048 byte long) to LoRa Cloud                    | [README](apps/examples/large_file_upload/README.md)         |
| LoRaWAN                     | Send data on a periodic basis                                           | [README](apps/examples/lorawan/README.md)                   |
| LoRaWAN asynchronous        | React on an asynchronous event - a button press here - and send data    | [README](apps/examples/lorawan_asynchronous/README.md)      |
| LoRaWAN Class B             | Enable Class B after joining a network                                  | [README](apps/examples/lorawan_class_b/README.md)           |
| LoRaWAN Multicast - Class B | Enable Class B after joining a network and configure multicast sessions | [README](apps/examples/lorawan_multicast_class_b/README.md) |
| LoRaWAN Multicast - Class C | Enable Class C after joining a network and configure multicast sessions | [README](apps/examples/lorawan_multicast_class_c/README.md) |
| Stream                      | Send data as a stream to LoRa Cloud                                     | [README](apps/examples/stream/README.md)                    |
| Time synchronization        | Get time from the network server or from LoRa Cloud                     | [README](apps/examples/time_sync/README.md)                 |
| TX/RX continuous            | Generate a TX/RX continuous thanks to the test mode                     | [README](apps/examples/tx_rx_continuous/README.md)          |

Here is the list of the different demonstrations available in the SDK:

| Name             | Description                                                               | Documentation                                            |
| ---------------- | ------------------------------------------------------------------------- | -------------------------------------------------------- |
| Region switching | Switch between two different networks with different switching mechanisms | [README](apps/demonstrations/region_switching/README.md) |

## Configuration

Each example has its own set of parameters - see `apps/<example>/main_<example>.h`.

There is also [a common configuration file](apps/common/lorawan_key_config.h) where LoRaWAN parameters can be set:

* DevEUI
* JoinEUI
* AppKey
* Class
* Region

## Convert LoRa Basics Modem Edge application code in LoRa Basics Modem application code

In order to help a LoRa Basics Modem Edge application code conversion to a LoRa Basics Modem application code please refer to [ModemE_to_LBM](doc/ModemE_to_LBM.md).

## Requirements

### Supported platforms

LoRa Basics Modem is platform independent and can be used with any MCU that fulfills the requirements.

This SDK is developed on the STMicroeletronics [NUCLEO-L476RG development board](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html).

Different Semtech shields can configured at compile time:

* LR1110 / LR1120 / LR1121 see [README](shields/LR11XX/smtc_shield_lr11xx/README.md)
* SX1261 / SX1262 / SX1268 [README](shields/SX126X/smtc_shield_sx126x/README.md)

### Toolchain

Examples can be compiled with either:

* [Keil MDK ARM](https://www2.keil.com/mdk5)
* [GNU Arm Embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

The projects are known to compile with GCC arm-none-eabi toolchain v10.3.1.

### LR1110 / LR1120 / LR1121 firmware

The LoRa Basics Modem library requires the following firmware:

* LR1110 firmware version 0x0307 ([available here](https://github.com/Lora-net/radio_firmware_images/tree/master/lr1110/transceiver))
* LR1120 firmware version 0x0101 ([available here](https://github.com/Lora-net/radio_firmware_images/tree/master/lr1120/transceiver))
* LR1121 firmware version 0x0101 ([available here](https://github.com/Lora-net/radio_firmware_images/tree/master/lr1121/transceiver))

To update the transceiver with the desired firmware version, please use [the updater tool application](https://github.com/Lora-net/SWTL001).

## Getting started

### Configure

Before starting to build an example, check the parameters in [the common configuration file](apps/common/lorawan_key_config.h). Make sure that the LoRaWAN configuration is consistent with what is configured in your network server and in your LoRa Cloud account.

Parameters can be seen in the [debug output](#view-debug-output).

### Build

#### Keil MDK ARM

Each example is delivered with a several Keil project files, available under `MDK-ARM` folder for each project:

| Target name                                         | Transceivers             | LoRaWAN parameters | Cryptographic operations    | Notes                              |
| --------------------------------------------------- | ------------------------ | ------------------ | --------------------------- | ---------------------------------- |
| lbm_example_<example>_LR11xx_LR_cred_crypto.uvprojx | LR1110 / LR1120 / LR1121 | LR11XX-based       | LR11xx cryptographic engine |                                    |
| lbm_example_<example>_LR11xx_LR_crypto.uvprojx      | LR1110 / LR1120 / LR1121 | User-defined       | LR11xx cryptographic engine |                                    |
| lbm_example_<example>_LR11xx_soft_crypto.uvprojx    | LR1110 / LR1120 / LR1121 | User-defined       | Software                    |                                    |
| lbm_example_<example>_SX126x_soft_crypto.uvprojx    | SX1261 / SX1262 / SX1268 | User-defined       | Software                    | Not available for `Almanac update` |

Each project has different targets ([Keil manual](https://www.keil.com/support/man/docs/uv4/uv4_ca_projtargfilegr.htm)), each one selecting a specific shield.

The procedure to compile a project is then the following:

1. Open Keil with the project file corresponding to the the project to use, the transceiver available on the shield and the cryptographic mode to use
2. Select the target corresponding to the shield
3. Click on compile button

#### GNU Arm embedded toolchain

Each example is delivered with a makefile, available under `makefile` folder. The output files of the build process are stored in the `build` folder with firmware binary file having the same name as the project with a .bin extension.

It is possible to choose the cryptographic mode with the `CRYPTO` variable:

| CRYPTO value            | Cryptographic operations    | LoRaWAN parameters | Comments                                                   |
| ----------------------- | --------------------------- | ------------------ | ---------------------------------------------------------- |
| SOFT                    | Software                    | User-defined       | Compatible with all transceivers                           |
| LR11XX                  | LR11XX cryptographic engine | User-defined       | Compatible with LR1110 / LR1120 / LR1121 transceivers only |
| LR11XX_WITH_CREDENTIALS | LR11XX cryptographic engine | LR11XX-based       | Compatible with LR1110 / LR1120 / LR1121 transceivers only |

The default value is `LR11XX`.

It is possible to choose the shield with the `RADIO_BOARD` variable. The possible values are listed in the corresponding `README` file:

* [LR11xx shields](shields/LR11XX/smtc_shield_lr11xx/README.md)
* [SX126x shields](shields/SX126X/smtc_shield_sx126x/README.md)

The default value is `LR1110MB1DIS`.

For instance, to build the project `lorawan` on the LR1120MB1DJS platform board with the software cryptographic mode, call the following commands:

```shell
$ cd $SDK_FOLDER/apps/examples/lorawan/makefile
$ make RADIO_BOARD=LR1120MB1DJS CRYPTO=SOFT
```

##### Note on re-build

When re-building an example after changing a compile-time parameter that affects LoRa Basics Modem library (like *CRYPTO*, *RADIO_BOARD* or *MIDDLEWARE*), it is important to force re-build of the LoRa Basics Modem library.

This can be achieved by cleaning prior the re-build of the example with:

```shell
$ make clean_lbm
```

##### Command line configuration

Additional configuration flags can be passed from command line to compiler with `EXTRAFLAGS` argument.
This is dedicated to define macros that can be defined like the following:

```bash
$ make EXTRAFLAGS='-D<MACRO>=<VALUE>'
```

Where `<MACRO>` is the macro name to set and `<VALUE>` is the value to set for this macro.
Not all macro can be redefined through this way. Refer to the readme of examples for the list of macro that can be defined.

Note that when using the configuration on command line, `make` cannot detect a change in configuration on next build.
Therefore `make clean` must be invoked when building after a build where configuration was provided on command line.

##### Note on Critical message printing

The code can produce critical messages being printed on serial port when critical situations occurs.
This is typically the case when setting a LoRaWAN region or LoRaWAN class fails.

The default behavior is to print the critical message and keep running.
This behavior can be changed so that the code freezes (until watchdog reset the device).
To do so, the macro `ENABLE_CRITICAL_ERROR_DEADLOOP` has to be defined, for instance by doing:

```shell
$ make EXTRAFLAGS='-DENABLE_CRITICAL_ERROR_DEADLOOP'
```

### Load

After a project is built, it can be loaded onto a device.

There are multiple ways to do it:

* Drag and drop the binary file to the USB drive listed by our OS - usually shows up as `NODE_L476RG`.
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

## Validation

The examples provided in the SDK have been tested under the following configurations:

- shields (not all shields have been used on every regions):
  - LR1110MB1DJS
  - LR1110MB1GJS
  - LR1120MB1DJS
  - LR1120MB1GJS
  - SX1261MB1BAS
  - SX1261MB1CAS
  - SX1262MB1CAS
  - SX1262MB1DAS
  - SX1268MB1GAS
- LoRaWAN Network Server: The Things Network (v3.22.0)
- Application Server: the Node-Red based one provided in [`apps/examples/application_server`](apps/examples/application_server/README.md)


| Regions | Validation executed |  Comment |
| -- | :--: | -- |
| SMTC_MODEM_REGION_AS_923_GRP1 | :white_check_mark: | |
| SMTC_MODEM_REGION_AS_923_GRP2 | :white_check_mark: | |
| SMTC_MODEM_REGION_AS_923_GRP3 | :white_check_mark: | |
| SMTC_MODEM_REGION_AU_915 | :white_check_mark: | |
| SMTC_MODEM_REGION_CN_470 | :x: | Validation has not been executed on this region |
| SMTC_MODEM_REGION_CN_470_RP_1_0 | :white_check_mark: | |
| SMTC_MODEM_REGION_EU_868 | :white_check_mark: | |
| SMTC_MODEM_REGION_IN_865 | :white_check_mark: | |
| SMTC_MODEM_REGION_KR_920 | :white_check_mark: | |
| SMTC_MODEM_REGION_RU_864 | :white_check_mark: | |
| SMTC_MODEM_REGION_US_915 | :white_check_mark: | |
| SMTC_MODEM_REGION_WW2G4 | :white_check_mark: | Validation has been executed on examples as only LoRaWAN class A is supported for this region: <br> - time_sync <br> - stream <br> - lorawan_asynchronous <br> - almanac_update <br> - dm_info <br> - lorawan (`SMTC_MODEM_CLASS_A`) <br> - large_file_upload |


