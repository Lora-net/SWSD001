# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.1.0] - 2023-03-05

### Added

- Support of all LR1121 shields:
  - LR1121MB1DIS
  - LR1121MB1GIS
- `HAL_DBG_TRACE_CRITICAL` with compile-time-enabled deadloop

## [2.0.1] - 2022-11-02

### Changed

- Documentation update:
  - Only LoRaWAN class A supported for WW2G4 region
  - Region CN470 not validated

## [2.0.0] - 2022-09-09

### Added

- Regional Parameters version can now be chosen in `apps/common/apps_modem_common.mk` - default value set to `RP2_103`
- Region switching demonstration
- Class B, multicast class B and multicast class C examples (region `SMTC_MODEM_REGION_WW2G4` is not supported)
- Support of all SX1261/SX1262/SX1268 and LR1110/LR1120 shields:
  - SX1261MB1BAS
  - SX1261MB1CAS
  - SX1261MB2BAS
  - SX1262MB1CAS
  - SX1262MB1CBS
  - SX1262MB1DAS
  - SX1262MB1PAS
  - SX1262MB2CAS
  - SX1268MB1GAS
  - LR1120MB1DxS
  - LR1120MB1GxS
  - LR1110MB1GxS
  - LR1110MB1DxS
- Tx/Rx continuous example
- LR-FHSS example: apps/examples/lorawan_asynchronous_lr_fhss

### Changed

- Update LoRa Basics Modem to v3.2.4
- Default DevEUI value is now FE-FF-FF-FF-FD-FF-00-00
- Rename the makefile compile variable  `TARGET_BOARD` to `MCU_BOARD` and `PLATFORM_BOARD` to `RADIO_BOARD`
- Change the almanac update example DM interval from 1 hour to 30 seconds
- Change the full almanac update python script CLI

### Fixed

- Implementation for `hal_rtc_get_time_ms()` because of early wrapping
- Implementation of `smtc_modem_hal_get_radio_tcxo_startup_delay_ms` because of hard-coded value - now taken from `smtc_board_get_tcxo_startup_time_in_ms`

### Removed

- GNSS and Wi-Fi examples (now available in [SWSD004](https://github.com/Lora-net/SWSD004))

## [1.0.0] - 2021-10-26

### Added

- Initial version
