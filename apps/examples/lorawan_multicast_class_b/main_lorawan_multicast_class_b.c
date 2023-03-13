/*!
 * @file      main_lorawan_class_b.c
 *
 * @brief     LoRa Basics Modem Class B device implementation
 *
 * @copyright
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
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
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "main_lorawan_multicast_class_b.h"
#include "lorawan_key_config.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"
#include "smtc_modem_api.h"
#include "smtc_board_ralf.h"
#include "apps_utilities.h"
#include "smtc_modem_utilities.h"
#include "smtc_modem_api_str.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*!
 * @brief Stringify constants
 */
#define xstr( a ) str( a )
#define str( a ) #a

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*!
 * @brief Multicast network session key
 */
static const uint8_t mc_nwk_skey[16] = MULTICAST_NWK_SESSION_KEY;

/*!
 * @brief Multicast application session key
 */
static const uint8_t mc_app_skey[16] = MULTICAST_APP_SESSION_KEY;

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

static bool is_ping_slot_set = false;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
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
 * @brief Downlink data event callback.
 *
 * @param [in] rssi       RSSI in signed value in dBm + 64
 * @param [in] snr        SNR signed value in 0.25 dB steps
 * @param [in] rx_window  RX window
 * @param [in] port       LoRaWAN port
 * @param [in] payload    Received buffer pointer
 * @param [in] size       Received buffer size
 */
static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size );

/*!
 * @brief Time synchronization event callback
 */
static void on_modem_time_sync( smtc_modem_event_time_status_t status );

/*!
 * @brief Class B ping slot status event callback
 */
static void on_class_b_ping_slot_info( smtc_modem_event_class_b_ping_slot_status_t status );

/*!
 * @brief Class B status event callback
 */
static void on_class_b_status( smtc_modem_event_class_b_status_t status );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Main application entry point.
 */
int main( void )
{
    static apps_modem_event_callback_t smtc_event_callback = {
        .adr_mobile_to_static   = NULL,
        .alarm                  = NULL,
        .almanac_update         = NULL,
        .down_data              = on_modem_down_data,
        .join_fail              = NULL,
        .joined                 = on_modem_network_joined,
        .link_status            = NULL,
        .mute                   = NULL,
        .new_link_adr           = NULL,
        .reset                  = on_modem_reset,
        .set_conf               = NULL,
        .stream_done            = NULL,
        .time_updated_alc_sync  = on_modem_time_sync,
        .tx_done                = NULL,
        .upload_done            = NULL,
        .class_b_ping_slot_info = on_class_b_ping_slot_info,
        .class_b_status         = on_class_b_status,
    };

    /* Initialise the ralf_t object corresponding to the board */
    ralf_t* modem_radio = smtc_board_initialise_and_get_ralf( );

    /* Disable IRQ to avoid unwanted behaviour during init */
    hal_mcu_disable_irq( );

    /* Init board and peripherals */
    hal_mcu_init( );
    smtc_board_init_periph( );

    /* Init the Lora Basics Modem event callbacks */
    apps_modem_event_init( &smtc_event_callback );

    /* Init the modem and use apps_modem_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    /* Re-enable IRQ */
    hal_mcu_enable_irq( );

    HAL_DBG_TRACE_MSG( "\n" );
    HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem LoRaWAN Multicast Class B demo application ==== ######\n\n" );
    apps_modem_common_display_version_information( );

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

static void on_modem_reset( uint16_t reset_count )
{
    HAL_DBG_TRACE_INFO( "Application parameters:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN class B ping slot = %d\n", LORAWAN_CLASS_B_PING_SLOT );

    apps_modem_common_configure_lorawan_params( stack_id );

    ASSERT_SMTC_MODEM_RC( smtc_modem_multicast_set_grp_config( stack_id, MULTICAST_GROUP, MULTICAST_GROUP_ADDRESS,
                                                               mc_nwk_skey, mc_app_skey ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );
}

static void on_modem_network_joined( void )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_class_b_set_ping_slot_periodicity( stack_id, LORAWAN_CLASS_B_PING_SLOT ) );
    ASSERT_SMTC_MODEM_RC( smtc_modem_lorawan_class_b_request_ping_slot_info( stack_id ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_time_start_sync_service( stack_id, SMTC_MODEM_TIME_MAC_SYNC ) );
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_set_sync_interval_s( TIME_SYNC_APP_INTERVAL_S ) );
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_set_sync_invalid_delay_s( TIME_SYNC_APP_INVALID_DELAY_S ) );
}

static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size )
{
    HAL_DBG_TRACE_INFO( "Downlink received:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN Fport = %u\n", port );
    HAL_DBG_TRACE_INFO( "  - Payload size  = %u\n", size );
    HAL_DBG_TRACE_INFO( "  - RSSI          = %d dBm\n", rssi - 64 );
    HAL_DBG_TRACE_INFO( "  - SNR           = %d dB\n", snr >> 2 );

    switch( rx_window )
    {
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX1:
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX2:
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB:
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXB_MC_GRP0:
    {
        HAL_DBG_TRACE_INFO( "  - Rx window     = %s\n", smtc_modem_event_downdata_window_to_str( rx_window ) );
        break;
    }
    default:
    {
        HAL_DBG_TRACE_ERROR( "Unexpected event down data window %s\n",
                             smtc_modem_event_downdata_window_to_str( rx_window ) );
        break;
    }
    }

    if( size != 0 )
    {
        HAL_DBG_TRACE_ARRAY( "Payload", payload, size );
    }
}

void on_modem_time_sync( smtc_modem_event_time_status_t status )
{
    switch( status )
    {
    case SMTC_MODEM_EVENT_TIME_VALID:
    {
        apps_modem_common_get_utc_time( );

        if( is_ping_slot_set == true )
        {
            ASSERT_SMTC_MODEM_RC( smtc_modem_set_class( stack_id, SMTC_MODEM_CLASS_B ) );
        }

        break;
    }

    default:
    {
        break;
    }
    }
}

void on_class_b_ping_slot_info( smtc_modem_event_class_b_ping_slot_status_t status )
{
    switch( status )
    {
    case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_ANSWERED:
    {
        is_ping_slot_set = true;

        uint32_t gps_time_s;
        uint32_t gps_fractional_s;

        if( smtc_modem_get_time( &gps_time_s, &gps_fractional_s ) ==
            SMTC_MODEM_RC_OK )  // Performed to check if time is available
        {
            ASSERT_SMTC_MODEM_RC( smtc_modem_set_class( stack_id, SMTC_MODEM_CLASS_B ) );
        }

        break;
    }
    case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_NOT_ANSWERED:
    {
        ASSERT_SMTC_MODEM_RC( smtc_modem_lorawan_class_b_request_ping_slot_info( stack_id ) );
        break;
    }
    }
}

void on_class_b_status( smtc_modem_event_class_b_status_t status )
{
    switch( status )
    {
    case( SMTC_MODEM_EVENT_CLASS_B_READY ):
    {
        ASSERT_SMTC_MODEM_RC(
            smtc_modem_multicast_class_b_start_session( stack_id, MULTICAST_GROUP, MULTICAST_SESSION_FREQUENCY,
                                                        MULTICAST_SESSION_DATARATE, MULTICAST_SESSION_PING_SLOT ) );
        break;
    }
    default:
    {
        break;
    }
    }
}

/* --- EOF ------------------------------------------------------------------ */
