# LoRa Basics Modem stream application example

## Description

The application will automatically join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), the application will configure a stream and periodically add data to the stream buffer. This periodic action is based on the LoRa Basics Modem alarm functionality. Each time the alarm-related event is triggered, the application add a chunk of data to the stream buffer.

In this example, a pangram is streamed on a loop.

## Configuration

Several parameters can be updated in `main_stream.h` header file:

| Constant                                 | Comments                                                    |
| ---------------------------------------- | ----------------------------------------------------------- |
| `APP_SMTC_MODEM_STREAM_LORAWAN_FPORT`    | LoRaWAN FPort to be used by the stream service              |
| `APP_SMTC_MODEM_STREAM_CIPHER_MODE`      | Encryption mode to be used by the stream service            |
| `APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO` | Redundancy ratio to be used by the stream service           |
| `APP_STREAM_CHUNK_SIZE`                  | Size of a chunk to be streamed                              |
| `APP_STREAM_CHUNK_PERIOD`                | Period between two chunks being queued in the stream buffer |

The pangram to be streamed is the array named `pangram`.
