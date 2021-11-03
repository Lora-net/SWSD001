# LoRa Basics Modem large file upload application example

## Description

The application will automatically join a LoRaWAN network (see [configuration](../../apps/common/lorawan_key_config.h)).

Once a network is joined (i.e. when the corresponding event is triggered), the application will initialize a file transfer by calling `smtc_modem_file_upload_init()` and start it by calling `smtc_modem_file_upload_start()`.

In this example, the file is a text in ASCII.

When the transfer is over, the event `SMTC_MODEM_EVENT_UPLOADDONE` will be triggered with a status indicating if LoRa Cloud acknowledged the reception (`SMTC_MODEM_EVENT_UPLOADDONE_SUCCESSFUL`) or not (`SMTC_MODEM_EVENT_UPLOADDONE_ABORTED`).

The application can relaunch the same file transfer as soon as the current one is over.

The file upload can be stopped anytime by calling `smtc_modem_file_upload_reset()`.

## Configuration

Several parameters can be updated in `main_large_file_upload.h` header file:

| Constant                           | Description                                                    | Note                  |
| ---------------------------------- | -------------------------------------------------------------- | --------------------- |
| `APP_SMTC_MODEM_LFU_CIPHER_MODE`   | Large file upload encryption mode                              |                       |
| `APP_SMTC_MODEM_LFU_SIZE`          | Size in byte of the file to be uploaded                        | In the range [1:2048] |
| `APP_SMTC_MODEM_LFU_AVERAGE_DELAY` | Average delay in second between two uplinks                    |                       |
| `APP_SMTC_MODEM_LFU_RELAUNCH`      | Re-start a file upload session as soon as the previous is over |                       |

The text to be sent is the array named `text`.
