# LoRa Basics Modem Tx/Rx continuous example

## Description

The application will automatically starts a TX continuous or a RX continuous according to the configuration given by the user.

## Configuration

Several parameters can be updated in `main_tx_rx_continuous.c` header file:

| Constant                | Description                                                              | Possible values                                | Default Value                |
| ----------------------- | ------------------------------------------------------------------------ | ---------------------------------------------- | ---------------------------- |
| `TX_CONTINUOUS`         | Only in modulated, the signal is sent in continuous transmission if true | {`true`, `false`}                              | `true`                       |
| `RX_CONTINUOUS`         | Only in modulated, put the chip in RX continuous mode                    | {`true`, `false`}                              | `false`                      |
| `TX_MODULATED`          | Signal is modulated if true other it's unmodulated                       | {`true`, `false`}                              | `true`                       |
| `TX_SINGLE_INTER_DELAY` | Time in ms between two single transmission                               | `uint32_t`                                     | 1000                         |
| `TX_POWER_USED`         | TX power in dBm                                                          | `int8_t`, range depends on radio configuration | 14                           |
| `TX_POWER_OFFSET`       | TX power offset in dB                                                    | `int8_t`                                       | 0                            |
| `TX_PAYLOAD_LEN`        | Length of the transmitted payload                                         | [0,255]                                        | 51                           |
| `LORAWAN_REGION_USED`   | LoRaWAN used region                                                      | Value from `smtc_modem_region_t`               | `SMTC_MODEM_REGION_EU_868`   |
| `FREQUENCY`             | Freq in Hz                                                               | `uint32_t`                                     | 868100000                    |
| `SPREADING_FACTOR_USED` | Spreading factor                                                         | Value from `smtc_modem_test_sf_t`              | `SMTC_MODEM_TEST_LORA_SF7`   |
| `BANDWIDTH_USED`        | Bandwidth                                                                | Value from `smtc_modem_test_bw_t`              | `SMTC_MODEM_TEST_BW_125_KHZ` |
| `CODING_RATE_USED`      | Coding rate                                                              | Value from `smtc_modem_test_cr_t`              | `SMTC_MODEM_TEST_CR_4_5`     |

## Expected Behavior

Here follow the steps that shall be seen in the logs to indicate the expected behavior of the application.

### Device starts and resets

```
INFO: Modem Initialization

INFO: ===== LoRa Basics Modem TX/RX continuous demo application ====
```

Following this print you shall find application and parameter prints.

### Start test mode

For instance, if in Tx:

```
INFO: TST MODE: START

TX PARAM
FREQ         : 868100000 MHz
REGION       : 1
TX POWER     : 14 dBm
TX           : MODULATED
MODULATION   : LORA
SF           : 9
CR           : 4/5
BW           : 0
LoRa Tx - Freq:868100000, Power:14, sf:7, bw:7, cr:1, length:51
```