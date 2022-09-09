/*!
 * @file      main_time_sync.c
 *
 * @brief     LoRa Basics Modem time synchronization example
 *
 * @copyright
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
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "main_time_sync.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"
#include "smtc_modem_utilities.h"
#include "smtc_board_ralf.h"
#include "apps_utilities.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define xstr( a ) str( a )
#define str( a ) #a

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define OFFSET_BETWEEN_GPS_EPOCH_AND_UNIX_EPOCH 315964800

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
 * @brief Network joined event callback
 */
static void on_modem_network_joined( void );

/*!
 * @brief Time synchronization event callback
 */
static void on_modem_time_sync( smtc_modem_event_time_status_t status );

/*!
 * @brief Alarm event callback
 */
static void on_modem_alarm( void );

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
        .time_updated_alc_sync = on_modem_time_sync,
        .tx_done               = NULL,
        .upload_done           = NULL,
    };

    ralf_t* modem_radio = smtc_board_initialise_and_get_ralf( );

    hal_mcu_disable_irq( );

    hal_mcu_init( );
    smtc_board_init_periph( );

    apps_modem_event_init( &smtc_event_callback );

    smtc_modem_init( modem_radio, &apps_modem_event_process );

    hal_mcu_enable_irq( );

    HAL_DBG_TRACE_INFO( "===== LoRa Basics Modem time synchronization example =====\n\n" );
    apps_modem_common_display_version_information( );

    while( 1 )
    {
        /* Execute modem runtime, this function must be called again in sleep_time_ms milliseconds or sooner. */
        uint32_t sleep_time_ms = smtc_modem_run_engine( );

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
    HAL_DBG_TRACE_INFO( "  - Time synchronization service = %s\n", xstr( APP_MODEM_TIME_SYNC_SERVICE ) );
    HAL_DBG_TRACE_INFO( "  - Automatic time synchronization interval = %d s\n", APP_MODEM_TIME_SYNC_INTERVAL_IN_S );
    HAL_DBG_TRACE_INFO( "  - Manual time synchronization interval = %d s\n", APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S );

    apps_modem_common_configure_lorawan_params( stack_id );

    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );
}

void on_modem_network_joined( void )
{
    HAL_DBG_TRACE_INFO( "Checking/printing local UTC time joining network\n" );
    apps_modem_common_get_utc_time( );

    ASSERT_SMTC_MODEM_RC( smtc_modem_time_set_sync_interval_s( APP_MODEM_TIME_SYNC_INTERVAL_IN_S ) );
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_start_sync_service( stack_id, APP_MODEM_TIME_SYNC_SERVICE ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S ) );
}

void on_modem_time_sync( smtc_modem_event_time_status_t status )
{
    switch( status )
    {
    case SMTC_MODEM_EVENT_TIME_VALID:
    {
        HAL_DBG_TRACE_INFO( "Checking/printing local UTC time upon TIME reception\n" );
        apps_modem_common_get_utc_time( );
        break;
    }

    default:
    {
        break;
    }
    }
}

void on_modem_alarm( void )
{
    HAL_DBG_TRACE_INFO( "Sending synchronization request upon manual timer request\n" );
    HAL_DBG_TRACE_PRINTF( "Manual time synchronization request done. Next manual request in %d seconds. \n",
                          APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S );
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_trigger_sync_request( stack_id ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S ) );
}

/* --- EOF ------------------------------------------------------------------ */
