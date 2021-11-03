# Geolocation GNSS example application

## 1. Description

This application demonstrates a GNSS scan procedure with multiframe *scan group*
protocol for static geolocation.
It executes several GNSS scans in a raw (called a *scan group*) and sends all
the results one after the other with a LoRaWAN uplink.
The application server will use the multiframe API of the geolocation solver
with all the received NAV messages of the same group.
The application waits for a configurable amount of time before starting the next
scan group.

The example application state machine is the following:

```
JOIN
--> ON_JOINED
    |
    +--> Start ALC Sync
    +--> Set ALARM 30s

--> ON_ALARM
    |
    +--> Start GNSS scan (or restart ALC Sync if not synced yet)

--> ON_SCAN_DONE
    |
    +--> Get results (if ALMANAC TOO OLD -> DM request ALMANAC -> Set ALARM)
    +--> Push scan results to scan group queue
    +--> IF scan group queue NOT full
            |
            +--> Set ALARM for next scan (--> ON_ALARM)
    +--> ELSE
            |
            +--> Send 1st scan uplink

--> ON_TX_DONE
    |
    +--> IF scan group queue NOT empty
            |
            +--> Send next scan uplink
    +--> ELSE
            |
            +--> NEW scan group
            +--> Set ALARM for next scan group (--> ON_ALARM)
```

## 2. Configuration

## 2.1. LoRaWAN related configuration

The `apps/common/lorawan_key_config.h` header file defines several constants to
configure the LoRaWAN parameters (profile, region, keys).

| Constant              | Comments |
| --------------------- | -------- |
| `LORAWAN_REGION` | Selects the regulatory region |
| `LORAWAN_CLASS` | Selects the LoRaWAN class to use |

Supported values for `LORAWAN_REGION`:

* `SMTC_MODEM_REGION_EU_868 (default)`
* `SMTC_MODEM_REGION_US_915`
* `SMTC_MODEM_REGION_CN_470_RP_1_0`

Supported values for `LORAWAN_CLASS`:

* `SMTC_MODEM_CLASS_A (default)`
* `SMTC_MODEM_CLASS_C`

## 2.2. GNSS demonstration related configuration

The `main_geolocation_gnss.h` header file defines several constants to configure
geolocation parameters.

The following constants MUST be updated before compiling depending on the
location of the test. It is very important in order to get a good localization
accuracy.

The assistance position must be within a 150 km range of the actual location.
It is currently hard-coded in the example, but in future, the network will able
to help on this.

| Constant              | Comments |
| --------------------- | -------- |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT` | The latitude to use for GNSS Assisted scan (decimal degree) |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG` | The longitude to use for GNSS Assisted scan (decimal degree)  |
| `MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT` | A text representation of the assistance position, for information printing only |

The following constants do not need modification and the provided values are
known to provide expected behavior:

| Constant              | Comments |
| --------------------- | -------- |
| `GNSS_SCAN_GROUP_PERIOD` | Defines the duration between the end of a scan group and the start of next group |
| `GNSS_SCAN_GROUP_DELAY` | The duration between the end of GNSS scan and the start of the next GNSS scan in the same scan group |
| `GNSS_SCAN_GROUP_SIZE` | The number of scan to execute in the same scan group |

Some LoRaWAN parameters specific to this application are also available:

| Constant              | Comments |
| --------------------- | -------- |
| `LORAWAN_CONFIRMED_MSG_ON` | Request a confirmation message or not |

## 3. Build

Before building be sure to set the GNSS assistance position as described in the
previous section of this readme.

The demo can be built through GNU make command by doing the following:

```shell
# Navigate to the build folder
$ cd apps/geolocation_gnss/makefile

# Execute the make call
$ make -j
```

By default, the demonstration is compiled to use the EUIs and Application key
defined in the file *apps/common/lorawan_key_config.h*.

## 4. Usage

### 4.1. Serial console

The application requires no user intervention after the static configuration
option have been set.

Information messages are displayed on the serial console, starting with the
DevEUI, AppEUI/JoinEUI and PIN that you might need to register your device with
the LoRa Cloud Device Join service.

### 4.2. LoRaWAN Network Server / Application Server

This application needs an Application Server to run in order to perform the GNSS
solving.
A Node-Red application server is provided in folder *apps/geolocation_application_server*.
Refer to the readme in this folder for details about setup and usage of the
application server.

#### 4.3 Payload format between device and Application Server

The device sends the scan results to the Application Server through LoRaWAN
uplinks on port 195. Each scan result is sent following the *Tag, Length, Value*
format:

- **TAG** (1 byte) `0x0F`;
- **LENGTH** (1 byte) The length in byte of *VALUE* field;
- **VALUE** (variable length) The data content.

The *VALUE* field for this demonstration is composed of the following fields:

- *scan group token* (8 bits) A token allowing to group together scan results of the same scan group;
- *NAV index* (5 bits) A decrementing counter allowing to identify a scan result in its scan group;
- *Detected SV* (3 bits) Value indicating the number of satellites (Space Vehicules) detected in the corresponding scan result. This field is encoded as the actual number of satellites detected minus 3. So that the field's value is in the range [0, 7] which means that the actual number of satellites is in the range [3, 10];
- *nav message* (variable length) The actual NAV message obtained from the LR1110.

All together, the payload format of uplinks is the following

| Byte | 0 | 1 | 2 | 3 | 4 ... |
| ---- | - | - | - | - | - |
| Field | TAG | Length | scan group token | NAV index + Detected SV | NAV message |

#### 4.4 Full almanac update for GNSS geolocation

By default, this application does not perform a full almanac update at startup.
It rather set the ALMANAC_STATUS field as a periodic DM message to the DAS for
regular update.

But it is possible to force a full almanac update, for testing purpose or to
optimize geolocation performances from the start.

In order to perform this full almanac update, it is required to get an almanac
image from the LoRa Cloud DAS.

For this a python script is provided with this application:

```
$ python3 get_full_almanac.py
```

Before running this python script, it needs to be updated with a valid token to
be set in the *YOUR_TOKEN* variable.

Once executed you should get, printed on the console, a C binary array to be
copy/pasted into a C header file and included by the *gnss_helpers.c* file.
Please refer to the *gnss_almanac_old.h* file provided as an example and used to
test the ALMANAC_TOO_OLD sequence of the application.

Once you have a header file with the almanac image included in *gnss_helpers.c*,
you'll need to set the *GNSS_FULL_ALMANAC_UPDATE* flag to 1 in the
*main_geolocation_gnss.c* file.
