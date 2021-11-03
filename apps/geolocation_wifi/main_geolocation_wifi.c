/*!
 * @ingroup   apps_geolocation
 * @file      main_geolocation_wifi.c
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation Wi-Fi example
 *
 * @copyright
 * @parblock
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * @endparblock

 */

/*!
 * @addtogroup apps_geolocation
 * LoRa Basics Modem LR1110 Geolocation Wi-Fi example
 * @{
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "main_geolocation_wifi.h"
#include "lorawan_key_config.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_utilities.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"

#include "wifi_helpers.h"
#include "lr1110_system.h"

#include "smtc_modem_api.h"
#include "smtc_modem_test_api.h"
#include "smtc_modem_utilities.h"
#include "smtc_board_ralf.h"

#include <stdio.h>
#include <string.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define MAX_WIFI_RESULT_BUFFER_SIZE ( 255 )
#define WIFI_APP_PORT ( 195 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */
/**
 * @brief LR1110 radio firmware
 */
#define LR1110_FW_VERSION 0x0307

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Stack identifier
 */
static uint8_t stack_id = 0;

/*!
 * @brief Modem radio
 */
static ralf_t* modem_radio;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief   Send an application frame on LoRaWAN port
 *
 * @param [in] tx_frame_buffer   buffer containing the LoRaWAN buffer
 * @param [in] tx_frame_buffer_size   payload len buffer
 * @param [in] confirmed   send a confirmed or unconfirmed uplink [false : unconfirmed / true : confirmed]
 * @param [in] port   LoRaWAN application port to be used
 *
 * @returns  [true: frame could be send, false: error]
 */
static bool send_frame( const uint8_t* tx_frame_buffer, const uint8_t tx_frame_buffer_size, const bool confirmed,
                        const uint8_t port );

/*!
 * @brief Helper function that configure the ADR configuration based on the region already configured in the stack.
 *
 * Prior using this function, the region must have been set already in the stack.
 */
static void configure_adr( void );

/*!
 * @addtogroup basics_modem_evt_callback
 * LoRa Basics Modem event callbacks
 * @{
 */

/*!
 * @brief Reset event callback
 *
 * @param [in] reset_count reset counter from the modem
 */
static void on_modem_reset( uint16_t reset_count );

/*!
 * @brief Network Joined event callback
 */
static void on_modem_network_joined( void );

/*!
 * @brief Alarm event callback
 */
static void on_modem_alarm( void );

/*!
 * @brief Tx done event callback
 *
 * @param [in] status tx done status \ref event_tx_done_state_t
 */
static void on_modem_tx_done( smtc_modem_event_txdone_status_t status );

/*!
 * @brief WiFi scan done event callback
 */
static void on_modem_user_radio_access( uint32_t timestamp_ms, smtc_modem_event_user_radio_access_status_t status );

/*!
 * @}
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Main application entry point.
 */
int main( void )
{
    smtc_modem_lorawan_version_t       lorawan_version;
    smtc_modem_version_t               firmware_version;
    lr1110_system_version_t            lr1110_fw_version;
    lr1110_status_t                    status;
    static apps_modem_event_callback_t smtc_event_callback = {
        .adr_mobile_to_static  = NULL,
        .alarm                 = on_modem_alarm,
        .almanac_update        = NULL,
        .down_data             = NULL,
        .join_fail             = NULL,
        .joined                = on_modem_network_joined,
        .link_status           = NULL,
        .mute                  = NULL,
        .new_link_adr          = NULL,
        .reset                 = on_modem_reset,
        .set_conf              = NULL,
        .stream_done           = NULL,
        .time_updated_alc_sync = NULL,
        .tx_done               = on_modem_tx_done,
        .upload_done           = NULL,
        .user_radio_access     = on_modem_user_radio_access,
    };

    /* Initialise the ralf_t object corresponding to the board */
    modem_radio = smtc_board_initialise_and_get_ralf( );

    /* Disable IRQ to avoid unwanted behaviour during init */
    hal_mcu_disable_irq( );

    /* Init board and peripherals */
    hal_mcu_init( );
    smtc_board_init_periph( );

    /* Init the Lora Basics Modem event callbacks */
    apps_modem_event_init( &smtc_event_callback );

    /* Init the modem and use smtc_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    /* Re-enable IRQ */
    hal_mcu_enable_irq( );

    /* Notify user that the board is initialized */
    smtc_board_leds_blink( smtc_board_get_led_all_mask( ), 100, 2 );

    HAL_DBG_TRACE_MSG( "\n" );
    HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem Geolocation Wi-Fi example ==== ######\n\n" );

    /* LoRa Basics Modem Version */
    HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem Version ==== ######\n" );

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_lorawan_version( &lorawan_version ) );
    HAL_DBG_TRACE_INFO( "LORAWAN     : %.2x.%.2x.%.2x.%.2x\n", lorawan_version.major, lorawan_version.minor,
                        lorawan_version.patch, lorawan_version.revision );

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_modem_version( &firmware_version ) );
    HAL_DBG_TRACE_INFO( "FIRMWARE    : %.2x.%.2x.%.2x\n", firmware_version.major, firmware_version.minor,
                        firmware_version.patch );

    /* Check LR1110 Firmware version */
    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) ); /* protect from radio access conflicts */
    status = lr1110_system_get_version( modem_radio->ral.context, &lr1110_fw_version );
    ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to get LR1110 firmware version\n" );
        mcu_panic( );
    }
    if( lr1110_fw_version.fw != LR1110_FW_VERSION )
    {
        HAL_DBG_TRACE_ERROR( "Wrong LR1110 firmware version, expected 0x%04X, got 0x%04X\n", LR1110_FW_VERSION,
                             lr1110_fw_version.fw );
        mcu_panic( );
    }
    HAL_DBG_TRACE_INFO( "LR1110 FW   : 0x%04X\n", lr1110_fw_version.fw );

    /* Configure the partial low power mode */
    hal_mcu_partial_sleep_enable( APP_PARTIAL_SLEEP );

    while( 1 )
    {
        /* Execute modem runtime, this function must be called again in sleep_time_ms milliseconds or sooner. */
        uint32_t sleep_time_ms = smtc_modem_run_engine( );

        /* go in low power */
        hal_mcu_set_sleep_for_ms( sleep_time_ms );
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/*!
 * @brief LoRa Basics Modem event callbacks called by smtc_event_process function
 */

static void on_modem_reset( uint16_t reset_count )
{
    /* Basic LoRaWAN configuration */
    apps_modem_common_configure_lorawan_params( stack_id );

    /* Start the Join process */
    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );

    HAL_DBG_TRACE_INFO( "###### ===== JOINING ==== ######\n\n" );

    /* Notify user with leds */
    smtc_board_led_set( smtc_board_get_led_tx_mask( ), true );
}

static void on_modem_network_joined( void )
{
    /* Set the ADR profile once joined */
    configure_adr( );

    /* Notify user with leds */
    smtc_board_led_set( smtc_board_get_led_tx_mask( ), false );
    smtc_board_led_pulse( smtc_board_get_led_rx_mask( ), true, LED_PERIOD_MS );

    /* Start the SMTC modem alarm timer */
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_GEOLOC_SCAN_PERIOD ) );
}

static void on_modem_alarm( void )
{
    smtc_modem_status_mask_t modem_status;

    /* Schedule next alarm */
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_GEOLOC_SCAN_PERIOD ) );
    HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer : %d s\n\n", APP_GEOLOC_SCAN_PERIOD );

    /* Get the modem status and display it */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_status( stack_id, &modem_status ) );
    HAL_DBG_TRACE_PRINTF( "modem_status : 0X%08X\n", modem_status );
    modem_status_to_string( modem_status );

    /* Start WiFi scan */
    wifi_helpers_start_scan( stack_id, modem_radio->ral.context );
}

static void on_modem_tx_done( smtc_modem_event_txdone_status_t status )
{
    static uint32_t uplink_cnt = 0;

    if( status != SMTC_MODEM_EVENT_TXDONE_NOT_SENT )
    {
        HAL_DBG_TRACE_PRINTF( "Uplink frame   : %lu\n", uplink_cnt++ );

        /* Notify user with leds */
        smtc_board_led_pulse( smtc_board_get_led_tx_mask( ), true, LED_PERIOD_MS );
    }
}

static void on_modem_user_radio_access( uint32_t timestamp_ms, smtc_modem_event_user_radio_access_status_t status )
{
    if( status == SMTC_MODEM_EVENT_USER_RADIO_ACCESS_WIFI_SCAN_DONE )
    {
        HAL_DBG_TRACE_PRINTF( "Wi-Fi scan done at %d\n", timestamp_ms );

        wifi_helpers_end_scan( );

        uint8_t nb_wifi_mac = 0;
        // Wi-Fi scan completed, get and display the results
        wifi_helpers_get_results( modem_radio->ral.context, &nb_wifi_mac );
        if( nb_wifi_mac > 0 )
        {
            uint8_t wifi_result_buffer[MAX_WIFI_RESULT_BUFFER_SIZE] = { 0 };
            uint8_t wifi_buffer_size                                = 0;
            wifi_helpers_display_results( );
            wifi_helpers_prepare_result_payload( wifi_result_buffer, &wifi_buffer_size, MAX_WIFI_RESULT_BUFFER_SIZE );
            if( send_frame( wifi_result_buffer, wifi_buffer_size, LORAWAN_CONFIRMED_MSG_ON, WIFI_APP_PORT ) != true )
            {
                HAL_DBG_TRACE_ERROR( "SEND FRAME ERROR\n" );
            }
        }
    }
    else
    {
        HAL_DBG_TRACE_PRINTF( "Unexpected user radio access: type %d at %d\n", status, timestamp_ms );
    }
}

/*!
 * @brief User private function
 */

static bool send_frame( const uint8_t* tx_frame_buffer, const uint8_t tx_frame_buffer_size, bool tx_confirmed,
                        uint8_t port )
{
    smtc_modem_return_code_t modem_response_code = SMTC_MODEM_RC_OK;
    uint8_t                  tx_max_payload;
    int32_t                  duty_cycle;

    /* Sanity check:
        We expect the application parameters to be properly set to avoid:
        - exceed duty cycle
        - exceed maximum payload
        The below checks are only for developer information */

    /* Inform if duty cycle is not available */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_duty_cycle_status( &duty_cycle ) );
    if( duty_cycle < 0 )
    {
        HAL_DBG_TRACE_ERROR( "Duty Cycle: available for next uplink in %d milliseconds\n", duty_cycle );
    }

    /* Get the next tx payload size */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_next_tx_max_payload( stack_id, &tx_max_payload ) );

    /* Get the next tx payload size */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_next_tx_max_payload( stack_id, &tx_max_payload ) );
    if( tx_frame_buffer_size > tx_max_payload )
    {
        HAL_DBG_TRACE_ERROR( "payload size: exceed max payload allowed for next uplink (%d > %d bytes)\n",
                             tx_frame_buffer_size, tx_max_payload );
    }

    /* Send uplink */
    modem_response_code =
        smtc_modem_request_uplink( stack_id, port, tx_confirmed, tx_frame_buffer, tx_frame_buffer_size );
    ASSERT_SMTC_MODEM_RC( modem_response_code );

    if( modem_response_code == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_INFO( "Request uplink:\n" );
        HAL_DBG_TRACE_ARRAY( "Payload", tx_frame_buffer, tx_frame_buffer_size );
        return true;
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "Request uplink failed with modem_response_code : %d \n", modem_response_code );
        return false;
    }
}

void configure_adr( void )
{
    smtc_modem_region_t region;
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region ) );

    switch( region )
    {
    case SMTC_MODEM_REGION_EU_868:
    {
        const uint8_t adr_custom_list[16] = {
            5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3
        }; /* 125kHz - SF7, SF8, SF9 */
        const uint8_t nb_trans = 1;
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, nb_trans ) );
        break;
    }
    case SMTC_MODEM_REGION_CN_470:
    {
        const uint8_t adr_custom_list[16] = {
            5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3
        }; /* 125kHz - SF7, SF8, SF9 */
        const uint8_t nb_trans = 2;
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, nb_trans ) );
        break;
    }
    case SMTC_MODEM_REGION_US_915:
    {
        const uint8_t adr_custom_list[16] = {
            3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1
        }; /* 125kHz - SF7, SF8, SF9 */
        const uint8_t nb_trans = 21;
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, nb_trans ) );
        break;
    }
    }
}

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
