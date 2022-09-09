/*!
 * @file      main_region_switching.c
 *
 * @brief     LoRa Basics Modem region switching demonstration
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

#include "main_region_switching.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "smtc_modem_hal.h"
#include "apps_modem_common.h"
#include "usr_button.h"
#include "apps_modem_event.h"
#include "smtc_modem_utilities.h"
#include "smtc_board_ralf.h"
#include "apps_utilities.h"
#include "smtc_modem_api_str.h"
#include "lorawan_key_config.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*!
 * @brief Stack identifier
 */
static const uint8_t stack_id = 0;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Variable to keep the last region used - useful in case of modem reset
 */
static smtc_modem_region_t region_current_cache = LORAWAN_REGION;

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
 * @brief Pushbutton event callback
 */
static void on_push_button( void );

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
 * @brief Alarm event callback
 */
static void on_modem_alarm( void );

/*!
 * @brief Tx done event callback
 *
 * @param [in] status tx done status @ref smtc_modem_event_txdone_status_t
 */
static void on_modem_tx_done( smtc_modem_event_txdone_status_t status );

/*!
 * @brief Link check event callback
 *
 * @param [in] status Status of the link check request
 * @param [in] margin Margin in dB from the LNS side of the link check request
 * @param [in] gw_cnt Number of gateways which received the link check request
 */
static void on_modem_link_status( smtc_modem_event_link_check_status_t status, uint8_t margin, uint8_t gw_cnt );

/*!
 * @brief Leave the current network and join either a sub-GHz or 2.4GHz network
 *
 * @param [in] region_target Region to be used to rejoin
 */
static void leave_and_rejoin( smtc_modem_region_t region_target );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Main application entry point.
 */
int main( void )
{
    uint32_t sleep_time_ms = 0;

    static apps_modem_event_callback_t smtc_event_callback = {
        .adr_mobile_to_static  = NULL,
        .alarm                 = on_modem_alarm,
        .almanac_update        = NULL,
        .down_data             = on_modem_down_data,
        .join_fail             = NULL,
        .joined                = on_modem_network_joined,
        .link_status           = on_modem_link_status,
        .mute                  = NULL,
        .new_link_adr          = NULL,
        .reset                 = on_modem_reset,
        .set_conf              = NULL,
        .stream_done           = NULL,
        .time_updated_alc_sync = NULL,
        .tx_done               = on_modem_tx_done,
        .upload_done           = NULL,
    };

    /* Initialise the ralf_t object corresponding to the board */
    ralf_t* modem_radio = smtc_board_initialise_and_get_ralf( );

    /* Disable IRQ to avoid unwanted behaviour during init */
    hal_mcu_disable_irq( );

    /* Init board and peripherals */
    hal_mcu_init( );
    usr_button_init( );
    smtc_board_init_periph( );

    /* Init the Lora Basics Modem event callbacks */
    apps_modem_event_init( &smtc_event_callback );

    /* Init the modem and use apps_modem_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    /* Re-enable IRQ */
    hal_mcu_enable_irq( );

    HAL_DBG_TRACE_MSG( "\n" );
    HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem region switching demonstration ==== ######\n\n" );
    apps_modem_common_display_version_information( );

    while( 1 )
    {
        // Atomically check sleep condition
        hal_mcu_disable_irq( );
        if( ( sleep_time_ms == 0 ) || ( get_usr_button_irq_state( ) == true ) )
        {
            hal_mcu_enable_irq( );

            if( get_usr_button_irq_state( ) == true )
            {
                clear_usr_button_irq_state( );

                // Button may generate work for the stack, so drop down to smtc_modem_run_engine().
                on_push_button( );
            }
            sleep_time_ms = smtc_modem_run_engine( );
        }
        else
        {
            hal_mcu_set_sleep_for_ms( sleep_time_ms );
            hal_mcu_enable_irq( );
            sleep_time_ms = 0;
        }
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
    HAL_DBG_TRACE_INFO( "Application parameters:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN uplink / downlink Fport = %d\n", LORAWAN_APP_PORT );
    HAL_DBG_TRACE_INFO( "  - Interval in second between two temperature records = %d\n",
                        APP_MODEM_TEMPERATURE_RECORD_INTERVAL );
    HAL_DBG_TRACE_INFO( "  - Maximum number of uplink without downlink before link check start = %d\n",
                        LORAWAN_MAX_LOST_CONNECTION_CNT_BEFORE_LINK_CHECK );
    HAL_DBG_TRACE_INFO( "  - Maximum number of unanswered link check before region switching = %d\n",
                        LORAWAN_MAX_LINK_CHECK_KO_BEFORE_SWITCH );
    HAL_DBG_TRACE_ARRAY( "  - JoinEUI used when connected to 2.4GHz region", join_eui_2g4, SMTC_MODEM_EUI_LENGTH );

    apps_modem_common_configure_lorawan_params( stack_id );

    if( region_current_cache == SMTC_MODEM_REGION_WW2G4 )
    {
        HAL_DBG_TRACE_INFO( "Override LoRaWAN parameters when joining a 2.4GHz network.\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_region( stack_id, SMTC_MODEM_REGION_WW2G4 ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_joineui( stack_id, join_eui_2g4 ) );
    }

    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );
}

static void on_modem_network_joined( void )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_MODEM_TEMPERATURE_RECORD_INTERVAL ) );
}

static void on_push_button( void )
{
    leave_and_rejoin( SMTC_MODEM_REGION_WW2G4 );
}

static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size )
{
    if( ( port == LORAWAN_APP_PORT ) && ( size == APP_MODEM_REGION_SWITCHING_CMD_LEN ) )
    {
        HAL_DBG_TRACE_ARRAY( "Payload", payload, size );

        if( payload[0] == APP_MODEM_REGION_SWITCHING_OPCODE )
        {
            leave_and_rejoin( ( smtc_modem_region_t ) payload[1] );
        }
    }
}

static void on_modem_link_status( smtc_modem_event_link_check_status_t status, uint8_t margin, uint8_t gw_cnt )
{
    if( status == SMTC_MODEM_EVENT_LINK_CHECK_NOT_RECEIVED )
    {
        uint16_t lost_connection_cnt = 0;
        ASSERT_SMTC_MODEM_RC( smtc_modem_lorawan_get_lost_connection_counter( stack_id, &lost_connection_cnt ) );

        if( lost_connection_cnt >=
            LORAWAN_MAX_LOST_CONNECTION_CNT_BEFORE_LINK_CHECK + LORAWAN_MAX_LINK_CHECK_KO_BEFORE_SWITCH )
        {
            leave_and_rejoin( SMTC_MODEM_REGION_WW2G4 );
        }
    }
}

void on_modem_alarm( void )
{
    const int8_t temperature = smtc_modem_hal_get_temperature( );

    ASSERT_SMTC_MODEM_RC(
        smtc_modem_request_uplink( stack_id, LORAWAN_APP_PORT, false, ( uint8_t* ) &temperature, 1 ) );
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_MODEM_TEMPERATURE_RECORD_INTERVAL ) );
}

static void on_modem_tx_done( smtc_modem_event_txdone_status_t status )
{
    if( status == SMTC_MODEM_EVENT_TXDONE_SENT )
    {
        smtc_modem_region_t region;

        ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region ) );

        if( region != SMTC_MODEM_REGION_WW2G4 )
        {
            uint16_t lost_connection_cnt = 0;

            ASSERT_SMTC_MODEM_RC( smtc_modem_lorawan_get_lost_connection_counter( stack_id, &lost_connection_cnt ) );

            if( lost_connection_cnt >= LORAWAN_MAX_LOST_CONNECTION_CNT_BEFORE_LINK_CHECK )
            {
                ASSERT_SMTC_MODEM_RC( smtc_modem_lorawan_request_link_check( stack_id ) );
            }
        }
    }
}

static void leave_and_rejoin( smtc_modem_region_t region_target )
{
    smtc_modem_status_mask_t status;

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_status( stack_id, &status ) );

    if( ( status & SMTC_MODEM_STATUS_JOINED ) != 0 )
    {
        if( ( region_target == SMTC_MODEM_REGION_WW2G4 ) || ( region_target == LORAWAN_REGION ) )
        {
            smtc_modem_region_t region_current;
            ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region_current ) );

            if( region_target != region_current )
            {
                ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_clear_timer( ) );
                ASSERT_SMTC_MODEM_RC( smtc_modem_leave_network( stack_id ) );

                // Apply common LoRaWAN parameters as defined in lorawan_key_config.h
                apps_modem_common_configure_lorawan_params( stack_id );

                if( region_target == SMTC_MODEM_REGION_WW2G4 )
                {
                    HAL_DBG_TRACE_INFO( "Override LoRaWAN parameters when joining a 2.4GHz network.\n" );
                    ASSERT_SMTC_MODEM_RC( smtc_modem_set_region( stack_id, SMTC_MODEM_REGION_WW2G4 ) );
                    ASSERT_SMTC_MODEM_RC( smtc_modem_set_joineui( stack_id, join_eui_2g4 ) );
                }

                ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );

                region_current_cache = region_target;
            }
        }
    }
}

/* --- EOF ------------------------------------------------------------------ */
