/*!
 * @ingroup   apps_geolocation
 * @file      main_geolocation_gnss.c
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation GNSS example with *scan group" for static geolocation.
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
 * LoRa Basics Modem LR1110 Geolocation GNSS example for static objects
 * @{
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "main_geolocation_gnss.h"
#include "lorawan_key_config.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_utilities.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"

#include "gnss_helpers.h"
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

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */
/**
 * @brief LR1110 radio firmware
 */
#define LR1110_FW_VERSION 0x0307

/**
 * @brief Set to 1 to force a full almanac update at application startup
 */
#define GNSS_FULL_ALMANAC_UPDATE 0

/**
 * @brief Number of bytes of the TLV overhead
 */
#define GNSS_SCAN_TLV_SIZE 2

/**
 * @brief Number of bytes of the metadata field concerning a GNSS scan
 */
#define GNSS_SCAN_METADATA_SIZE 2

/**
 * @brief LoRaWAN port use for uplinks of the GNSS scan results
 */
#define GNSS_APP_PORT 195

/**
 * @brief Maximal number of scan to attempt per scan group
 *
 * In a scan group, every scan is added to the group results if it contains enough satellites (defined with
 * GNSS_SCAN_NB_SATELLITE_MIN). A scan group will stop the scanning process when one of the following conditions is met:
 *   - at least GNSS_SCAN_GROUP_SIZE valid scans are stored; or
 *   - at least GNSS_SCAN_GROUP_ATTEMPT_MAX scans have been attempted
 */
#define GNSS_SCAN_GROUP_ATTEMPT_MAX ( GNSS_SCAN_GROUP_SIZE + 4 )

/**
 * @brief Minimal number of detected SVs in a scan result to consider the scan valid
 */
#define GNSS_SCAN_NB_SATELLITE_MIN 6

/**
 * @brief Duration in second after last ALC sync response received to consider the local clock time invalid
 *
 * Set time valid for 1 day (to be fine tuned depending on board properties)
 */
#define APP_ALC_TIMING_INVALID ( 3600 * 24 )

/**
 * @brief Interval in second between two consecutive ALC sync requests
 *
 * 3 time sync requests per day
 */
#define APP_ALC_TIMING_INTERVAL ( APP_ALC_TIMING_INVALID / 3 )

/**
 * @brief Duration in second after configuring the ALC sync to check the time has been correctly synchronized
 *
 * If the stack reports the time has been correctly synchronized before this duration is elapsed, then the demo keeps on
 * running and this additionnal check is not executed.
 */
#define APP_TIMER_CHECK_ALC_SYNC_INIT ( 30 )

/**
 * @brief Duration in second after restarting the ALC sync to check the time has been correctly synchronized
 *
 * When the application detects that clock is not synchronized anymore, it restart the ALC sync service and set this
 * duration timing to check it got re-synchronized.
 * If the stack reports the time has been correctly synchronized before this duration is elapsed, then the demo keeps on
 * running and this additionnal check is not executed.
 */
#define APP_TIMER_CHECK_ALC_SYNC_RESTART ( 10 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

typedef struct
{
    uint8_t detected_sv; /* Number of satellites detected */
    uint8_t results_size;
    uint8_t results_buffer[GNSS_SCAN_TLV_SIZE + GNSS_SCAN_METADATA_SIZE + GNSS_RESULT_SIZE_MAX_MODE3];
} gnss_scan_t;

typedef struct
{
    gnss_scan_t scan[GNSS_SCAN_GROUP_SIZE];
    uint8_t     nb_scan_valid;
    uint8_t     nb_scan_total;
    uint8_t     nb_scan_sent;
    uint8_t     token;
} gnss_scan_group_queue_t;

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

/**
 * @brief Assistance position for GNSS scan
 */
static const lr1110_gnss_solver_assistance_position_t assistance_position = {
    .latitude  = MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT,
    .longitude = MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG,
};
static const char* assistance_position_text = MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT;

/*!
 * @brief Static allocation of the scan group queue
 */
static gnss_scan_group_queue_t gnss_scan_group_queue;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Helper function that prints a queue
 */
static void gnss_scan_group_queue_print( gnss_scan_group_queue_t* queue );

/*!
 * @brief Initialise a queue with zero'ed content
 */
static void gnss_scan_group_queue_init( gnss_scan_group_queue_t* queue );

/*!
 * @brief Set all counters of queue to 0
 */
static void gnss_scan_group_queue_new( gnss_scan_group_queue_t* queue );

/*!
 * @brief Add a new scan result to a queue
 *
 * @param[in] queue The queue to update
 * @param[in] scan The scan result to push to the queue
 */
static void gnss_scan_group_queue_push( gnss_scan_group_queue_t* queue, gnss_scan_t* scan );

/*!
 * @brief Send a queue over-the-air through LoRaWAN
 *
 * @param[in] queue The queue to push
 * @param[in] lorawan_port The LoRaWAN port to use to push the queue
 *
 */
static void gnss_scan_group_queue_send( gnss_scan_group_queue_t* queue, uint8_t lorawan_port );

/*!
 * @brief Check if a queue is full
 *
 * A queue is considered full if one of the following confition is met:
 *   - The number of valid GNSS scan result is reached GNSS_SCAN_GROUP_SIZE; or
 *   - The number of GNSS scan results attempted reached GNSS_SCAN_GROUP_ATTEMPT_MAX
 *
 * @param[in] queue The queue to check
 * @retval true The queue is full
 * @retval false The queue is not full or queue is a null pointer
 */
static bool gnss_scan_group_queue_is_full( gnss_scan_group_queue_t* queue );

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
 * @brief GNSS scan done event callback
 */
static void on_modem_user_radio_access( uint32_t timestamp_ms, smtc_modem_event_user_radio_access_status_t status );

/*!
 * @brief Clock synchronisation event callback
 */
static void on_modem_clk_synch( smtc_modem_event_time_status_t time_status );

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
        .time_updated_alc_sync = on_modem_clk_synch,
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

    /* Init the modem and use apps_modem_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    /* Re-enable IRQ */
    hal_mcu_enable_irq( );

    /* Notify user that the board is initialized */
    smtc_board_leds_blink( smtc_board_get_led_all_mask( ), 100, 2 );

    HAL_DBG_TRACE_MSG( "\n" );
    HAL_DBG_TRACE_INFO(
        "###### ===== LoRa Basics Modem GNSS Geolocation example (for static objects) ==== ######\n\n" );

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
    apps_modem_common_configure_lorawan_params( stack_id );

    /* Initialize for GNSS scan */
    smtc_gnss_init( modem_radio->ral.context,
                    assistance_position );  // No need to suspend when called before smtc_modem_run_engine()
    HAL_DBG_TRACE_WARNING( "Assistance position has been set in %s\n", assistance_position_text );

    /* Initialize GNSS scan group queue */
    gnss_scan_group_queue_init( &gnss_scan_group_queue );

#if GNSS_FULL_ALMANAC_UPDATE
    HAL_DBG_TRACE_INFO( "!! Force full almanac update !!\n" );
    /* Update full almanac with local one */
    smtc_gnss_almanac_update( modem_radio->ral.context );

    /* do not set DM ALMANAC_STATUS field to test ALMANAC_TOO_OLD case */
#else
    /* Configure modem DM status for regular almanac status update */
    smtc_modem_dm_info_interval_format_t format   = SMTC_MODEM_DM_INFO_INTERVAL_IN_DAY;
    uint8_t                              interval = 1;
    ASSERT_SMTC_MODEM_RC( smtc_modem_dm_set_info_interval( format, interval ) );

    /* Active almanac update OTA - WARNING: will remove all other DM message */
    uint8_t info_field = SMTC_MODEM_DM_FIELD_ALMANAC_STATUS;
    ASSERT_SMTC_MODEM_RC( smtc_modem_dm_set_info_fields( &info_field, 1 ) );
#endif

    /* Start the Join process */
    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );

    HAL_DBG_TRACE_INFO( "###### ===== JOINING ==== ######\n\n" );

    /* Notify user with leds */
    smtc_board_led_set( smtc_board_get_led_tx_mask( ), true );
}

static void on_modem_network_joined( void )
{
    smtc_modem_region_t region;

    /* Set the ADR profile based on selected region */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region ) );
    switch( region )
    {
    case SMTC_MODEM_REGION_EU_868:
        HAL_DBG_TRACE_INFO( "Set ADR profile for EU868\n" );
        ASSERT_SMTC_MODEM_RC(
            smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list_eu868 ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, custom_nb_trans_eu868 ) );
        break;
    case SMTC_MODEM_REGION_CN_470:
    case SMTC_MODEM_REGION_CN_470_RP_1_0:
        HAL_DBG_TRACE_INFO( "Set ADR profile for CN470\n" );
        ASSERT_SMTC_MODEM_RC(
            smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list_cn470 ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, custom_nb_trans_cn470 ) );
        break;
    case SMTC_MODEM_REGION_US_915:
        HAL_DBG_TRACE_INFO( "Set ADR profile for US915\n" );
        ASSERT_SMTC_MODEM_RC(
            smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list_us915 ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_set_nb_trans( stack_id, custom_nb_trans_us915 ) );
        break;
    default:
        HAL_DBG_TRACE_ERROR( "Region not supported in this example, could not set custom ADR profile\n" );
        break;
    }

    /* Notify user with leds */
    smtc_board_led_set( smtc_board_get_led_tx_mask( ), false );
    smtc_board_led_pulse( smtc_board_get_led_rx_mask( ), true, LED_PERIOD_MS );

    /* Start time sync (ALC sync), necessary for GNSS scan:
    The interval_s indicates how often the LBM will request a time sync from the DAS.
    If no time sync downlink has been received from the DAS after the invalid_delay_s is elapsed,
    the LBM will report SMTC_MODEM_RC_NO_TIME on smtc_modem_get_time() call. */
    /* -- */
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_set_sync_interval_s( APP_ALC_TIMING_INTERVAL ) );     /* keep call order */
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_set_sync_invalid_delay_s( APP_ALC_TIMING_INVALID ) ); /* keep call order */
    /* Start the service */
    ASSERT_SMTC_MODEM_RC( smtc_modem_time_start_sync_service( stack_id, SMTC_MODEM_TIME_ALC_SYNC ) );

    /* Program an alarm to check time synced */
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_TIMER_CHECK_ALC_SYNC_INIT ) );
    HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer : %d s\n\n", APP_TIMER_CHECK_ALC_SYNC_INIT );
}

static void on_modem_alarm( void )
{
    smtc_modem_status_mask_t modem_status;
    smtc_modem_return_code_t err;
    uint32_t                 gps_time           = 0;
    uint32_t                 fractional_seconds = 0;

    /* Get the modem status and display it */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_status( stack_id, &modem_status ) );
    HAL_DBG_TRACE_PRINTF( "modem_status : 0X%08X\n", modem_status );
    modem_status_to_string( modem_status );

    /* Check if time sync is valid */
    err = smtc_modem_get_time( &gps_time, &fractional_seconds );
    if( err == SMTC_MODEM_RC_OK )
    {
        /* Start GNSS scan */
        if( smtc_gnss_scan( stack_id, modem_radio->ral.context, gps_time ) != true )
        {
            /* Modem is busy, retry later */
            ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( GNSS_SCAN_GROUP_DELAY ) );
            HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer for next scan: %d s\n\n", GNSS_SCAN_GROUP_DELAY );
        }
    }
    else if( err == SMTC_MODEM_RC_NO_TIME )
    {
        /* Restart ALC sync service (to speed-up retry) */
        ASSERT_SMTC_MODEM_RC( smtc_modem_time_stop_sync_service( stack_id ) );
        ASSERT_SMTC_MODEM_RC( smtc_modem_time_start_sync_service( stack_id, SMTC_MODEM_TIME_ALC_SYNC ) );

        /* Program an alarm to check time synced */
        ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_TIMER_CHECK_ALC_SYNC_RESTART ) );
        HAL_DBG_TRACE_INFO( "Time Sync Service restarted: smtc_modem_alarm_start_timer : %d s\n\n",
                            APP_TIMER_CHECK_ALC_SYNC_RESTART );
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "Failed to get time\n" );
    }
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

    /* Send scan uplink if any, or trigger new scan group */
    if( gnss_scan_group_queue.nb_scan_sent < gnss_scan_group_queue.nb_scan_valid )
    {
        gnss_scan_group_queue_send( &gnss_scan_group_queue, GNSS_APP_PORT );
    }
    else
    {
        /* Set next GNSS scan group */
        gnss_scan_group_queue_new( &gnss_scan_group_queue );

        ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( GNSS_SCAN_GROUP_PERIOD ) );
        HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer for next scan group : %d s\n\n", GNSS_SCAN_GROUP_PERIOD );
    }
}

static void on_modem_user_radio_access( uint32_t timestamp_ms, smtc_modem_event_user_radio_access_status_t status )
{
    bool request_almanac_update = false;

    if( status == SMTC_MODEM_EVENT_USER_RADIO_ACCESS_GNSS_SCAN_DONE )
    {
        HAL_DBG_TRACE_PRINTF( "GNSS scan done at %d\n", timestamp_ms );
        gnss_scan_t scan_results;

        /* GNSS scan completed - first thing to be done */
        smtc_gnss_scan_ended( );

        /* Get scan results from LR1110 */
        memset( &scan_results, 0, sizeof scan_results );
        smtc_gnss_get_results( modem_radio->ral.context, &scan_results.detected_sv, &scan_results.results_size,
                               &scan_results.results_buffer[GNSS_SCAN_TLV_SIZE + GNSS_SCAN_METADATA_SIZE],
                               &request_almanac_update );

        /* Trigger almanac update if needed or push scan results into the scan group queue */
        if( request_almanac_update == true )
        {
            /* TODO: should we drop the current scan group ? */

            /* Request Almanac update from DAS */
            HAL_DBG_TRACE_INFO( "Requesting ALMANAC UPDATE\n" );
            uint8_t dm_almanac_status = SMTC_MODEM_DM_FIELD_ALMANAC_STATUS;
            ASSERT_SMTC_MODEM_RC( smtc_modem_dm_request_single_uplink( &dm_almanac_status, 1 ) );

            /* Program an alarm to trigger next GNSS scan */
            ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( GNSS_SCAN_GROUP_PERIOD ) );
            HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer : %d s\n\n", GNSS_SCAN_GROUP_PERIOD );
        }
        else
        {
            smtc_gnss_display_results( scan_results.detected_sv,
                                       &scan_results.results_buffer[GNSS_SCAN_TLV_SIZE + GNSS_SCAN_METADATA_SIZE],
                                       scan_results.results_size );

            /* Push scan to the scan group */
            gnss_scan_group_queue_push( &gnss_scan_group_queue, &scan_results );

            /* Trigger next GNSS scan or send first scan results, if scan group completed */
            if( gnss_scan_group_queue_is_full( &gnss_scan_group_queue ) == false )
            {
                /* Program the timer for the next scan of the group */
                ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( GNSS_SCAN_GROUP_DELAY ) );
                HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer for next scan: %d s\n\n", GNSS_SCAN_GROUP_DELAY );
            }
            else
            {
                /* Send scan uplink if any, or trigger new scan group */
                if( gnss_scan_group_queue.nb_scan_sent < gnss_scan_group_queue.nb_scan_valid )
                {
                    gnss_scan_group_queue_send( &gnss_scan_group_queue, GNSS_APP_PORT );
                }
                else
                {
                    /* No valid scan after GNSS_SCAN_GROUP_ATTEMPT_MAX, nothing to be sent */

                    /* Set next GNSS scan group */
                    gnss_scan_group_queue_new( &gnss_scan_group_queue );

                    /* Program alarm for next scan group */
                    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( GNSS_SCAN_GROUP_PERIOD ) );
                    HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer : %d s\n\n", GNSS_SCAN_GROUP_PERIOD );
                }
            }
        }
    }
    else
    {
        HAL_DBG_TRACE_PRINTF( "User radio access: type %d at %d\n", status, timestamp_ms );
    }
}

static void on_modem_clk_synch( smtc_modem_event_time_status_t time_status )
{
    if( time_status == SMTC_MODEM_EVENT_TIME_VALID )
    {
        HAL_DBG_TRACE_WARNING( "Time sync OK\n" );
    }
}

/*!
 * @brief User private function
 */

static void gnss_scan_group_queue_init( gnss_scan_group_queue_t* queue )
{
    if( queue != NULL )
    {
        memset( queue, 0, sizeof( gnss_scan_group_queue_t ) );

        HAL_DBG_TRACE_PRINTF( "%s:\n", __FUNCTION__ );
        gnss_scan_group_queue_print( queue );
    }
}

static void gnss_scan_group_queue_new( gnss_scan_group_queue_t* queue )
{
    if( queue != NULL )
    {
        queue->nb_scan_valid = 0;
        queue->nb_scan_total = 0;
        queue->nb_scan_sent  = 0;

        queue->token += 1;

        HAL_DBG_TRACE_PRINTF( "%s:\n", __FUNCTION__ );
        gnss_scan_group_queue_print( queue );
    }
}

static bool gnss_scan_group_queue_is_full( gnss_scan_group_queue_t* queue )
{
    if( queue != NULL )
    {
        return ( ( queue->nb_scan_valid == GNSS_SCAN_GROUP_SIZE ) ||
                 ( queue->nb_scan_total == GNSS_SCAN_GROUP_ATTEMPT_MAX ) );
    }

    return false;
}

static void gnss_scan_group_queue_push( gnss_scan_group_queue_t* queue, gnss_scan_t* scan )
{
    if( ( queue != NULL ) && ( scan != NULL ) )
    {
        /* Add the scan to the queue if valid */
        if( scan->detected_sv >= GNSS_SCAN_NB_SATELLITE_MIN )
        {
            memcpy( &( queue->scan[queue->nb_scan_valid] ), scan, sizeof( gnss_scan_t ) );
            queue->nb_scan_valid += 1;
        }
        queue->nb_scan_total += 1;

        HAL_DBG_TRACE_PRINTF( "%s:\n", __FUNCTION__ );
        gnss_scan_group_queue_print( queue );
    }
}

static void gnss_scan_group_queue_send( gnss_scan_group_queue_t* queue, uint8_t lorawan_port )
{
    uint8_t index;

    if( ( queue != NULL ) && ( queue->nb_scan_valid > 0 ) )
    {
        index = queue->nb_scan_sent;

        /* set TLV header */
        queue->scan[index].results_buffer[0] = GNSS_TLV_TAG_NAV_MULTIFRAME;
        queue->scan[index].results_buffer[1] = GNSS_SCAN_METADATA_SIZE + queue->scan[index].results_size;

        /* Set scan group metadata
            | token (8b) | NAV index (5b) | Detected SV (3b) |
            - token: scan group identifier
            - NAV index: decreasing NAV index in the scan group
            - Detected SV: actual number of satellites detected minus 3. codes [3-10] satellites
        */
        queue->scan[index].results_buffer[GNSS_SCAN_TLV_SIZE + 0] = queue->token;
        queue->scan[index].results_buffer[GNSS_SCAN_TLV_SIZE + 1] =
            ( ( queue->nb_scan_valid - queue->nb_scan_sent - 1 ) << 3 ) | ( queue->scan[index].detected_sv - 3 );

        /* Send uplink */
        if( send_frame( queue->scan[index].results_buffer,
                        GNSS_SCAN_TLV_SIZE + GNSS_SCAN_METADATA_SIZE + queue->scan[index].results_size,
                        LORAWAN_CONFIRMED_MSG_ON, lorawan_port ) == true )
        {
            /* Update queue info */
            queue->nb_scan_sent += 1;
        }
        else
        {
            HAL_DBG_TRACE_ERROR( "Failed to send uplink frame\n" );
        }

        HAL_DBG_TRACE_PRINTF( "%s:\n", __FUNCTION__ );
        gnss_scan_group_queue_print( queue );
    }
}

static void gnss_scan_group_queue_print( gnss_scan_group_queue_t* queue )
{
    HAL_DBG_TRACE_PRINTF( "****************************************\n" );
    HAL_DBG_TRACE_PRINTF( "token:       0x%02X\n", queue->token );
    HAL_DBG_TRACE_PRINTF( "scan_total:  %d\n", queue->nb_scan_total );
    HAL_DBG_TRACE_PRINTF( "scan_valid:  %d\n", queue->nb_scan_valid );
    HAL_DBG_TRACE_PRINTF( "scan_sent:   %d\n", queue->nb_scan_sent );
    for( uint8_t i = 0; i < queue->nb_scan_valid; i++ )
    {
        HAL_DBG_TRACE_PRINTF( "scan[%d]: %02d %02d - ", i, queue->scan[i].detected_sv, queue->scan[i].results_size );
        for( uint8_t j = 0; j < ( GNSS_SCAN_TLV_SIZE + GNSS_SCAN_METADATA_SIZE + queue->scan[i].results_size ); j++ )
        {
            HAL_DBG_TRACE_PRINTF( "%02X ", queue->scan[i].results_buffer[j] );
        }
        HAL_DBG_TRACE_PRINTF( "\n" );
    }
    HAL_DBG_TRACE_PRINTF( "****************************************\n" );
}

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
    if( tx_frame_buffer_size > tx_max_payload )
    {
        HAL_DBG_TRACE_ERROR( "payload size: exceed max payload allowed for next uplink (%d > %d bytes)\n",
                             tx_frame_buffer_size, tx_max_payload );
    }

    /* Send uplink */
    modem_response_code =
        smtc_modem_request_uplink( stack_id, port, tx_confirmed, tx_frame_buffer, tx_frame_buffer_size );
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

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
