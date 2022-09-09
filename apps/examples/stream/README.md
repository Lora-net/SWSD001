# LoRa Basics Modem stream application example

## Description

The application will automatically join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), the application will configure a stream and periodically add data to the stream buffer. This periodic action is based on the LoRa Basics Modem alarm functionality. Each time the alarm-related event is triggered, the application add a chunk of data to the stream buffer.

In this example, a pangram is streamed on a loop.

## Configuration

Several parameters can be updated in `main_stream.h` header file:

| Constant                                 | Comments                                                    | Possible values                                                                          | Default Value                 |
| ---------------------------------------- | ----------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ----------------------------- |
| `APP_SMTC_MODEM_STREAM_LORAWAN_FPORT`    | LoRaWAN FPort to be used by the stream service              | [0,223], must be the same as the one configured on Application Server and LoRaCloud side | 199                           |
| `APP_SMTC_MODEM_STREAM_CIPHER_MODE`      | Encryption mode to be used by the stream service            | Value from `smtc_modem_stream_cipher_mode_t`                                             | `SMTC_MODEM_STREAM_NO_CIPHER` |
| `APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO` | Redundancy ratio to be used by the stream service           | `uint8_t`                                                                                | 110                           |
| `APP_STREAM_CHUNK_SIZE`                  | Size of a chunk to be streamed                              | [1,254]                                                                                  | 40                            |
| `APP_STREAM_CHUNK_PERIOD`                | Period between two chunks being queued in the stream buffer | `uint32_t`                                                                               | 60                            |

When compiling with *arm-none-eabi-gcc* toolchain, all these constant are configurable through command line with the `EXTRAFLAGS`.
See [README.md](../../../README.md#command-line-configuration).

The pangram to be streamed is the array named `pangram`.

## Expected Behavior

### Device starts and resets

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem stream example =====

INFO: ###### ===== BASICS MODEM RESET EVENT ==== ######
```

Following this print you shall find application and parameter prints

### Device joined the network

```
INFO: ###### ===== JOINED EVENT ==== ######
```

### Stream sending start

```
INFO: Chunk to be added is: The quick brown fox jumps over the lazy  (40 byte(s) long)
INFO: STREAM_SEND [OK]
```
You can see the actual ASCII string sent in the stream.

### Stream sending done

```
WARN: Streaming DONE
INFO: ###### ===== STREAM DONE EVENT ==== ######
INFO: Stream buffer is now empty.
```
Once done you can check its reception in the cloud service.

### Looping upon timer

After the first stream done, a timer is set for the next one to be sent.
Once the timer expired the following print should show:

```
INFO: ###### ===== ALARM EVENT ==== ######
INFO: Chunk to be added is: dog.The quick brown fox jumps over the l (40 byte(s) long)
INFO: STREAM_SEND [OK]
INFO: Next chunk will be added to stream in 60 seconds
```
