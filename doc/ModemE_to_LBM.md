# Convert LoRa Basics Modem Edge (Modem-E) to LoRa Basics Modem (LBM)

This document describes how to switch from LoRa Basics Modem Edge (Modem-E) version 1.1.7 to LoRa Basics Modem (LBM).

For more description of each API please refer to :

LoRa Basics Modem (LBM) :
-   [smtc_modem_api.h](https://github.com/Lora-net/SWL2001/blob/master/smtc_modem_api/smtc_modem_api.h)
-   [smtc_basic_modem_lr11xx_api_extension.h](https://github.com/Lora-net/SWL2001/blob/master/smtc_modem_api/smtc_basic_modem_lr11xx_api_extension.h)
-   [smtc_modem_test_api.h](https://github.com/Lora-net/SWL2001/blob/master/smtc_modem_api/smtc_modem_test_api.h)

LoRa Basics Modem Edge (Modem-E) :
-   [lr1110_modem_lorawan.h](https://github.com/Lora-net/lr1110_modem_driver/blob/master/src/lr1110_modem_lorawan.h)
-   [lr1110_bootloader.h](https://github.com/Lora-net/lr1110_modem_driver/blob/master/src/lr1110_bootloader.h)
-   [lr1110_modem_system.h](https://github.com/Lora-net/lr1110_modem_driver/blob/master/src/lr1110_modem_system.h)
-   [lr1110_modem_wifi.h](https://github.com/Lora-net/lr1110_modem_driver/blob/master/src/lr1110_modem_wifi.h)
-   [lr1110_modem_gnss.h](https://github.com/Lora-net/lr1110_modem_driver/blob/master/src/lr1110_modem_gnss.h)
-   [lr1110_modem_helper.h](https://github.com/Lora-net/lr1110_modem_driver/blob/master/src/lr1110_modem_helper.h)

## Supported platforms

LoRa Basics Modem is platform independent and can be used with any MCU that fulfills the requirements. Refer to porting guide document available on [LoRa Developer portal](https://lora-developers.semtech.com/build/software/lora-basics/lora-basics-for-end-nodes/lora-basics-for-end-nodes-welcome/) under section `LoRa Basics(tm) Modem` in `LoRa Basics Modem Porting Guide`.

## LoRaWAN

| LoRa Basics Modem (LBM) API                        | LoRa Basics ModemEdge (Modem-E) API                      | Comment |
| -------------------------------------------------- | -------------------------------------------------------- | ------- |
| `smtc_modem_get_event`                             | `lr1110_modem_get_event`                                 |         |
| `smtc_modem_get_modem_version`                     | `lr1110_modem_get_version`                               |         |
| `smtc_modem_get_lorawan_version`                   | `lr1110_modem_get_version`                               |         |
| `smtc_modem_get_regional_params_version`           | NO API                                                   |         |
| `smtc_modem_reset`                                 | `lr1110_modem_reset`                                     |         |
| `smtc_modem_factory_reset`                         | NO API                                                   |         |
| `smtc_modem_get_charge`                            | `lr1110_modem_get_charge`                                |         |
| `smtc_modem_reset_charge`                          | `lr1110_modem_reset_charge`                              |         |
| NO API                                             | `lr1110_modem_write_user_defined_charge_counter`         |         |
| NO API                                             | `lr1110_modem_read_user_defined_charge_counter`          |         |
| NO API                                             | `lr1110_modem_select_charge_uplink`                      |         |
| `smtc_modem_get_tx_power_offset_db`                | `lr1110_modem_get_tx_power_offset`                       |         |
| `smtc_modem_set_tx_power_offset_db`                | `lr1110_modem_set_tx_power_offset`                       |         |
| `smtc_modem_time_start_sync_service`               | `lr1110_modem_set_alc_sync_mode`                         |         |
| `smtc_modem_time_stop_sync_service`                | `lr1110_modem_set_alc_sync_mode`                         |         |
| NO API                                             | `lr1110_modem_get_alc_sync_mode`                         |         |
| `smtc_modem_get_time`                              | `lr1110_modem_get_gps_time`                              |         |
| NO API                                             | `lr1110_modem_set_gps_time`                              | in LBM the only way to get the date is to receive it from the network |
| `smtc_modem_time_trigger_sync_request`             | NO API                                                   |         |
| `smtc_modem_time_set_alcsync_fport`                | `lr1110_modem_set_alc_sync_port`                         |         |
| `smtc_modem_time_get_alcsync_fport`                | `lr1110_modem_get_alc_sync_port`                         |         |
| `smtc_modem_time_set_sync_interval_s`              | NO API                                                   |         |
| `smtc_modem_time_get_sync_interval_s`              | NO API                                                   |         |
| `smtc_modem_time_set_sync_invalid_delay_s`         | NO API                                                   |         |
| `smtc_modem_time_get_sync_invalid_delay_s`         | NO API                                                   |         |
| `smtc_modem_get_status`                            | `lr1110_modem_get_status`                                |         |
| `smtc_modem_alarm_start_timer`                     | `lr1110_modem_set_alarm_timer`                           |         |
| `smtc_modem_alarm_clear_timer`                     | `lr1110_modem_set_alarm_timer`                           | By setting 0 as value |
| `smtc_modem_alarm_get_remaining_time`              | NO API                                                   |         |
| `smtc_modem_get_joineui`                           | `lr1110_modem_get_join_eui`                              |         |
| `smtc_modem_set_joineui`                           | `lr1110_modem_set_join_eui`                              |         |
| `smtc_modem_get_deveui`                            | `lr1110_modem_get_dev_eui`                               |         |
| `smtc_modem_set_deveui`                            | `lr1110_modem_set_dev_eui`                               |         |
| `smtc_modem_set_nwkkey`                            | `lr1110_modem_set_app_key`                               |         |
| `smtc_modem_get_class`                             | `lr1110_modem_get_class`                                 |         |
| `smtc_modem_set_class`                             | `lr1110_modem_set_class`                                 |         |
| `smtc_modem_multicast_set_grp_config`              | NO API                                                   |         |
| `smtc_modem_multicast_get_grp_config`              | NO API                                                   |         |
| `smtc_modem_multicast_class_c_start_session`       | NO API                                                   |         |
| `smtc_modem_multicast_class_c_get_session_status`  | NO API                                                   |         |
| `smtc_modem_multicast_class_c_stop_session`        | NO API                                                   |         |
| `smtc_modem_multicast_class_c_stop_all_sessions`   | NO API                                                   |         |
| `smtc_modem_multicast_class_b_start_session`       | NO API                                                   |         |
| `smtc_modem_multicast_class_b_get_session_status`  | NO API                                                   |         |
| `smtc_modem_multicast_class_b_stop_session`        | NO API                                                   |         |
| `smtc_modem_multicast_class_b_stop_all_sessions`   | NO API                                                   |         |
| `smtc_modem_get_region`                            | `lr1110_modem_get_region`                                |         |
| `smtc_modem_set_region`                            | `lr1110_modem_set_region`                                |         |
| NO API                                             | `lr1110_modem_list_regions`                              | Region list is built at the compilation |
| `smtc_modem_adr_get_profile`                       | `lr1110_modem_get_adr_profile`                           |         |
| `smtc_modem_adr_set_profile`                       | `lr1110_modem_set_adr_profile`                           |         |
| `smtc_modem_get_available_datarates`               | `lr1110_modem_get_available_data_rate`                   |         |
| `smtc_modem_dm_get_fport`                          | `lr1110_modem_get_dm_port`                               |         |
| `smtc_modem_dm_set_fport`                          | `lr1110_modem_set_dm_port`                               |         |
| `smtc_modem_dm_get_info_interval`                  | `lr1110_modem_get_dm_info_interval`                      |         |
| `smtc_modem_dm_set_info_interval`                  | `lr1110_modem_set_dm_info_interval`                      |         |
| `smtc_modem_dm_get_info_fields`                    | `lr1110_modem_get_dm_info_field`                         |         |
| `smtc_modem_dm_set_info_fields`                    | `lr1110_modem_set_dm_info_field`                         |         |
| `smtc_modem_dm_request_single_uplink`              | `lr1110_modem_send_dm_status`                            |         |
| `smtc_modem_dm_set_user_data`                      | `lr1110_modem_set_app_status`                            |         |
| `smtc_modem_dm_get_user_data`                      | NO API                                                   |         |
| `smtc_modem_join_network`                          | `lr1110_modem_join`                                      |         |
| `smtc_modem_leave_network`                         | `lr1110_modem_leave_network`                             |         |
| `smtc_modem_suspend_radio_communications`          | `lr1110_modem_suspend`                                   |         |
| `smtc_modem_get_next_tx_max_payload`               | `lr1110_modem_get_next_tx_max_payload`                   |         |
| `smtc_modem_request_uplink`                        | `lr1110_modem_request_tx`                                |         |
| `smtc_modem_request_emergency_uplink`              | `lr1110_modem_emergency_tx`                              |         |
| `smtc_modem_request_empty_uplink`                  | `lr1110_modem_request_tx`                                | Modem-E doesn't send fport, LBM can do it if it's selected |
| `smtc_modem_file_upload_init`                      | `lr1110_modem_upload_init`                               |         |
| `smtc_modem_file_upload_init`                      | `lr1110_modem_upload_data`                               |         |
| `smtc_modem_file_upload_start`                     | `lr1110_modem_upload_start`                              |         |
| `smtc_modem_file_upload_reset`                     | NO API                                                   |         |
| `smtc_modem_stream_init`                           | `lr1110_modem_stream_init`                               |         |
| `smtc_modem_stream_add_data`                       | `lr1110_modem_send_stream_data`                          |         |
| `smtc_modem_stream_status`                         | `lr1110_modem_stream_status`                             |         |
| NO API                                             | `lr1110_modem_set_stream_redundancy_rate`                | Redundancy rate is set directly in `smtc_modem_stream_init` |
| NO API                                             | `lr1110_modem_get_stream_redundancy_rate`                |         |
| `smtc_modem_set_certification_mode`                | `lr1110_modem_set_certification_mode`                    |         |
| `smtc_modem_get_certification_mode`                | `lr1110_modem_get_certification_mode`                    |         |
| `smtc_modem_connection_timeout_set_thresholds`     | `lr1110_modem_set_connection_timeout`                    |         |
| `smtc_modem_connection_timeout_get_thresholds`     | `lr1110_modem_get_connection_timeout`                    |         |
| `smtc_modem_connection_timeout_get_current_values` | `lr1110_modem_get_connection_timeout_status`             |         |
| `smtc_modem_lorawan_get_lost_connection_counter`   | NO API                                                   |         |
| `smtc_modem_get_duty_cycle_status`                 | `lr1110_modem_get_duty_cycle_status`                     |         |
| `smtc_modem_test_duty_cycle_app_activate`          | `lr1110_modem_activate_duty_cycle`                       |         |
| `smtc_modem_get_stack_state`                       | `lr1110_modem_get_lorawan_state`                         |         |
| `smtc_modem_set_network_type`                      | `lr1110_modem_set_network_type`                          |         |
| `smtc_modem_get_network_type`                      | `lr1110_modem_get_network_type`                          |         |
| `smtc_modem_lbt_set_parameters`                    | `lr1110_modem_activate_lbt`                              |         |
| `smtc_modem_lbt_get_parameters`                    | NO API                                                   |         |
| `smtc_modem_lbt_set_state`                         | `lr1110_modem_activate_lbt`                              |         |
| `smtc_modem_lbt_get_state`                         | NO API                                                   |         |
| `smtc_modem_set_nb_trans`                          | `lr1110_modem_set_nb_trans`                              |         |
| `smtc_modem_get_nb_trans`                          | `lr1110_modem_get_nb_trans`                              |         |
| `smtc_modem_set_crystal_error_ppm`                 | `lr1110_modem_set_crystal_error`                         |         |
| `smtc_modem_get_crystal_error_ppm`                 | `lr1110_modem_get_crystal_error`                         |         |
| `smtc_modem_lorawan_request_link_check`            | NO API                                                   |         |
| `smtc_modem_suspend_before_user_radio_access`      | NO API                                                   |         |
| `smtc_modem_resume_after_user_radio_access`        | NO API                                                   |         |
| `smtc_modem_lorawan_class_b_request_ping_slot_info`| NO API                                                   |         |
| `smtc_modem_class_b_set_ping_slot_periodicity`     | NO API                                                   |         |
| `smtc_modem_class_b_get_ping_slot_periodicity`     | NO API                                                   |         |
| `smtc_modem_get_network_frame_pending_status`      | NO API                                                   |         |
| `smtc_modem_set_adr_ack_limit_delay`               | NO API                                                   |         |
| `smtc_modem_get_adr_ack_limit_delay`               | NO API                                                   |         |
| `smtc_modem_get_pin`                               | `lr1110_modem_get_pin`                                   |         |
| `smtc_modem_get_chip_eui`                          | `lr1110_modem_get_chip_eui`                              |         |
| `smtc_modem_derive_keys`                           | `lr1110_modem_derive_keys`                               |         |
| `smtc_modem_test_start`                            | `lr1110_modem_test_mode_start`                           |         |
| `smtc_modem_test_nop`                              | `lr1110_modem_test_nop`                                  |         |
| `smtc_modem_test_tx`                               | `lr1110_modem_test_tx_single`                            |         |
| `smtc_modem_test_tx`                               | `lr1110_modem_test_tx_cont`                              |         |
| `smtc_modem_test_tx_cw`                            | `lr1110_modem_test_tx_cw`                                |         |
| `smtc_modem_test_rx_continuous`                    | `lr1110_modem_test_rx_cont`                              |         |
| `smtc_modem_test_get_nb_rx_packets`                | `lr1110_modem_test_read_packet_counter_rx_cont`          |         |
| `smtc_modem_test_rssi`                             | `lr1110_modem_test_rssi_subghz`                          |         |
| `smtc_modem_test_radio_reset`                      | `lr1110_modem_test_radio_rst`                            |         |
| `smtc_modem_test_stop`                             | `lr1110_modem_test_exit`                                 |         |
| NO API                                             | `lr1110_modem_test_tx_single_preamble`                   |         |
| `smtc_modem_test_get_rssi`                         | `lr1110_modem_test_read_rssi`                            |         |
| NO API                                             | `lr1110_modem_test_rssi_2g4`                             |         |
| NO API                                             | `lr1110_modem_test_rssi_gnss`                            |         |
| NO API                                             | `lr1110_modem_set_rf_output`                             | `ral_lr11xx_bsp_get_rf_switch_cfg` Define in ral_lr11xx_bsp.c |
| NO API                                             | `lr1110_modem_set_output_power_config`                   | `lr11xx_get_tx_cfg` Define in ral_lr11xx_bsp.c |
| NO API                                             | `lr1110_modem_get_output_power_config`                   | `lr11xx_get_tx_cfg` Define in ral_lr11xx_bsp.c |
| `smtc_modem_test_direct_radio_write`               | NO API                                                   |         |
| `smtc_modem_test_direct_radio_read`                | NO API                                                   |         |
| NO API                                             | `lr1110_modem_system_write_regmem32`                     | `lr11xx_system_write_regmem32` Direct driver API call |
| NO API                                             | `lr1110_modem_system_read_regmem32`                      | `lr11xx_system_red_regmem32` Direct driver API call |
| NO API                                             | `lr1110_modem_system_calibrate`                          | `ral_lr11xx_bsp_get_rssi_calibration_table` Define in ral_lr11xx_bsp.c |
| NO API                                             | `lr1110_modem_system_set_reg_mode`                       | `ral_lr11xx_bsp_get_reg_mode` Define in ral_lr11xx_bsp.c |
| NO API                                             | `lr1110_modem_system_set_dio_as_rf_switch`               | `ral_lr11xx_bsp_get_rf_switch_cfg` Define in ral_lr11xx_bsp.c |
| NO API                                             | `lr1110_modem_system_cfg_lfclk`                          | `lr11xx_system_cfg_lfclk` Direct driver API call |
| NO API                                             | `lr1110_modem_system_set_tcxo_mode`                      | `ral_lr11xx_bsp_get_xosc_cfg` Define in ral_lr11xx_bsp.c |
| NO API                                             | `lr1110_modem_system_reboot`                             | `lr11xx_system_reboot` Direct driver API call |
| NO API                                             | `lr1110_modem_helper_get_utc_time`                       | `apps_modem_common_get_utc_time` function in apps_modem_common.c in [SWSD001 repository](https://github.com/Lora-net/SWSD001/blob/master/apps/common/apps_modem_common.c) |
| NO API                                             | `lr1110_modem_helper_gnss_get_gnss_week_number_rollover` |         |
| NO API                                             | `lr1110_modem_helper_gnss_get_almanac_date_by_index`     |         |
| NO API                                             | `lr1110_modem_helper_gnss_get_result_destination`        |         |
| NO API                                             | `lr1110_modem_helper_gnss_get_event_type`                |         |
| NO API                                             | `lr1110_modem_helper_get_event_data`                     | `apps_modem_event_process` function in apps_modem_common.c in [SWSD001 repository](https://github.com/Lora-net/SWSD001/blob/master/apps/common/apps_modem_common.c) |


## GNSS

LoRa Basics Modem doesn't have a smtc_modem_gnss layer, it uses directly the LR11XX driver as part of LoRa Basics Modem. There are two possibilities:
-   Either call directly the driver APIs with the constraint to surround each call with `smtc_modem_suspend_before_user_radio_access` and `smtc_modem_resume_after_user_radio_access` preventing the modem to access to the radio at the same time as the user.
-   Or use the GNSS Middleware dedicated to LoRa Basics Modem which is strongly recommended. An implementation example can be found [here](https://github.com/Lora-net/SWSD004/tree/master/geolocation_middleware)

## Wi-Fi

LoRa Basics Modem doesn't have a smtc_modem_wifi layer, it uses directly the LR11XX driver as part of LoRa Basics Modem. There are two possibilities:
-   Either call directly the driver APIs with the constraint to surround each call with `smtc_modem_suspend_before_user_radio_access` and `smtc_modem_resume_after_user_radio_access` preventing the modem to access to the radio at the same time as the user.
-   Or use the Wi-Fi Middleware dedicated to LoRa Basics Modem which is strongly recommended. An implementation example can be found [here](https://github.com/Lora-net/SWSD004/tree/master/geolocation_middleware)

## Bootloader

The bootloader file from LoRa Basics Modem Edge is exactly the same as the LR11XX driver in LoRa Basics Modem. User has to call directly the driver APIs with the constraint to surround each call with `smtc_modem_suspend_before_user_radio_access` and `smtc_modem_resume_after_user_radio_access` preventing the modem to access to the radio at the same time as the user.
