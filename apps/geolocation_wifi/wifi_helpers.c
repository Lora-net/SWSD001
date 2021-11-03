/*!
 * @ingroup   apps_geolocation
 * @file      wifi_helpers.c
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation example, Wi-Fi scan helpers
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <string.h>

#include "apps_utilities.h"
#include "smtc_hal_dbg_trace.h"
#include "smtc_modem_api.h"

#include "lr1110_wifi.h"
#include "lr1110_system.h"

#include "lr1110_geolocation_helpers.h"
#include "wifi_helpers.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define WIFI_SINGLE_MAC_RESULT_LENGTH ( 7 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define WIFI_SINGLE_BEACON_LEN 7
#define WIFI_TIMEOUT_PER_SCAN_DEFAULT ( 80 )
#define WIFI_TIMEOUT_PER_CHANNEL_DEFAULT ( 600 )
#define WIFI_MAX_RESULTS_DEFAULT 7
#define WIFI_MAX_RESULT_TOTAL 32
#define WIFI_MAX_RESULTS_PER_SCAN ( 20 )
#define GNSS_TLV_TAG_WIFI_LEGACY ( 0x08 )
#define TLV_OVERHEAD_BYTES ( 2 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief Structure representing the configuration of Wi-Fi scan
 */
typedef struct
{
    bool                           enabled;
    lr1110_wifi_channel_mask_t     channels;
    lr1110_wifi_signal_type_scan_t types;
    lr1110_wifi_mode_t             scan_mode;
    uint8_t                        max_results;
    uint32_t                       timeout_per_channel;
    uint32_t                       timeout_per_scan;
} wifi_settings_t;

/*!
 * @brief Structure representing a single scan result
 */
typedef struct
{
    lr1110_wifi_mac_address_t        mac_address;
    lr1110_wifi_channel_t            channel;
    lr1110_wifi_signal_type_result_t type;
    int8_t                           rssi;
    uint8_t                          country_code[LR1110_WIFI_STR_COUNTRY_CODE_SIZE];
} wifi_scan_single_result_t;

/*!
 * @brief Structure representing a collection of scan results
 */
typedef struct
{
    bool                      error;
    uint8_t                   nbr_results;
    wifi_scan_single_result_t results[WIFI_MAX_RESULT_TOTAL];
} wifi_scan_all_result_t;

/*!
 * @brief Structure representing the aggregation of Wi-Fi scan settings and results
 */
typedef struct
{
    wifi_settings_t        settings;
    wifi_scan_all_result_t results;
} wifi_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Static allocation of Wi-Fi configuration and result holder
 */
static wifi_t wifi;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Initialise the settings for Wi-Fi scan
 */
static void wifi_helpers_init( void );

/*!
 * @brief Start Wi-Fi scan operation
 */
static bool wifi_helpers_scan( const void* ral_context );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void wifi_helpers_start_scan( const uint8_t stack_id, const void* ral_context )
{
    bool                     status = false;
    smtc_modem_stack_state_t stack_state;

    /* Ensure that the LoRaWAN stack is IDLE before starting a scan */
    if( smtc_modem_get_stack_state( stack_id, &stack_state ) != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_get_stack_state() failed\n" );
        return false;
    }
    if( stack_state != SMTC_MODEM_STACK_STATE_IDLE )
    {
        HAL_DBG_TRACE_WARNING( "Cannot suspend modem, the LoRaWAN stack is not in IDLE state\n" );
        return false;
    }

    /* Suspend modem to access the radio */
    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) );

    HAL_DBG_TRACE_INFO( "start Wi-Fi scan\n" );

    status = lr1110_configure_for_scan( ral_context ); /* TODO: to be done only once at application startup ? */
    if( status == true )
    {
        wifi_helpers_init( );
        status = wifi_helpers_scan( ral_context );
        if( status == false )
        {
            HAL_DBG_TRACE_ERROR( "Failed to start Wi-Fi scan\n" );
            ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
            return;
        }
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "Failed to configure LR1110 for Wi-Fi scan\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
        return;
    }

    /* Do not resume the modem here, to be done when SMTC_MODEM_EVENT_USER_RADIO_ACCESS_GNSS_SCAN_DONE event is
     * received, through the wifi_helpers_end_scan() function call.
     */
}

void wifi_helpers_end_scan( void ) { ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) ); }

void wifi_helpers_get_results( const void* ral_context, uint8_t* n_results )
{
    lr1110_wifi_basic_complete_result_t wifi_results_mac_addr[WIFI_MAX_RESULTS_PER_SCAN] = { 0 };
    uint8_t                             nb_results;
    lr1110_status_t                     status;

    /* Suspend modem to access the radio */
    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) );

    status = lr1110_wifi_get_nb_results( ral_context, &nb_results );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to get Wi-Fi scan number of results\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
        return;
    }

    *n_results = ( nb_results > WIFI_MAX_RESULTS_PER_SCAN ) ? WIFI_MAX_RESULTS_PER_SCAN : nb_results;

    status = lr1110_wifi_read_basic_complete_results( ral_context, 0, *n_results, wifi_results_mac_addr );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to read Wi-Fi scan results\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
        return;
    }

    /* Resume modem */
    ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );

    /* add scan to results */
    for( uint8_t index = 0; ( index < *n_results ) && ( wifi.results.nbr_results < WIFI_MAX_RESULT_TOTAL ); index++ )
    {
        const lr1110_wifi_basic_complete_result_t* local_basic_result = &wifi_results_mac_addr[index];

        wifi.results.results[wifi.results.nbr_results].channel =
            lr1110_wifi_extract_channel_from_info_byte( local_basic_result->channel_info_byte );

        wifi.results.results[wifi.results.nbr_results].type =
            lr1110_wifi_extract_signal_type_from_data_rate_info( local_basic_result->data_rate_info_byte );

        memcpy( wifi.results.results[wifi.results.nbr_results].mac_address, local_basic_result->mac_address,
                LR1110_WIFI_MAC_ADDRESS_LENGTH );

        wifi.results.results[wifi.results.nbr_results].rssi = local_basic_result->rssi;
        wifi.results.nbr_results++;
    }
}

void wifi_helpers_display_results( void )
{
    if( wifi.results.nbr_results != 0 )
    {
        HAL_DBG_TRACE_PRINTF( "Found %d Wi-Fi MAC addresses\n", wifi.results.nbr_results );
        for( uint8_t i = 0; i < wifi.results.nbr_results; i++ )
        {
            for( uint8_t j = 0; j < 6; j++ )
            {
                HAL_DBG_TRACE_PRINTF( "%02X ", wifi.results.results[i].mac_address[j] );
            }
            HAL_DBG_TRACE_PRINTF( " -- Channel: %d", wifi.results.results[i].channel );
            HAL_DBG_TRACE_PRINTF( " -- Type: %d", wifi.results.results[i].type );
            HAL_DBG_TRACE_PRINTF( " -- RSSI: %d\n", wifi.results.results[i].rssi );
        }
        HAL_DBG_TRACE_MSG( "\n" );
    }
    else
    {
        HAL_DBG_TRACE_MSG( "No MAC address found\n" );
    }
}

void wifi_helpers_prepare_result_payload( uint8_t* buffer, uint8_t* buffer_size, uint8_t max_buffer_size )
{
    // 0. Check if the buffer can contain all results
    const uint8_t required_size = WIFI_SINGLE_MAC_RESULT_LENGTH * wifi.results.nbr_results + TLV_OVERHEAD_BYTES;
    if( max_buffer_size < required_size )
    {
        HAL_DBG_TRACE_ERROR( "The result payload is too short to send Wi-Fi results. Requires %u, but limit is %u\n",
                             required_size, max_buffer_size );
        ( *buffer_size ) = 0;
        return;
    }
    else
    {
        ( *buffer_size ) = required_size;
    }

    // 0. Prepare the TLV
    buffer[0] = GNSS_TLV_TAG_WIFI_LEGACY;
    buffer[1] = WIFI_SINGLE_MAC_RESULT_LENGTH * wifi.results.nbr_results;

    // 1. Copy results in the buffer
    for( uint8_t index_result = 0; index_result < wifi.results.nbr_results; index_result++ )
    {
        const wifi_scan_single_result_t* local_result = &wifi.results.results[index_result];
        uint8_t* local_buffer = &buffer[TLV_OVERHEAD_BYTES + index_result * WIFI_SINGLE_MAC_RESULT_LENGTH];
        local_buffer[0]       = local_result->rssi;
        memcpy( &local_buffer[1], local_result->mac_address, LR1110_WIFI_MAC_ADDRESS_LENGTH );
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void wifi_helpers_init( void )
{
    wifi.results.nbr_results = 0;

    wifi.settings.enabled             = true;
    wifi.settings.channels            = 0x3FFF;
    wifi.settings.types               = LR1110_WIFI_TYPE_SCAN_B_G_N;
    wifi.settings.scan_mode           = LR1110_WIFI_SCAN_MODE_BEACON_AND_PKT;
    wifi.settings.max_results         = WIFI_MAX_RESULTS_DEFAULT;
    wifi.settings.timeout_per_channel = WIFI_TIMEOUT_PER_CHANNEL_DEFAULT;
    wifi.settings.timeout_per_scan    = WIFI_TIMEOUT_PER_SCAN_DEFAULT;
}

static bool wifi_helpers_scan( const void* ral_context )
{
    lr1110_status_t status;

    status = lr1110_system_set_dio_irq_params( ral_context, LR1110_SYSTEM_IRQ_WIFI_SCAN_DONE, LR1110_SYSTEM_IRQ_NONE );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to set Wi-Fi IRQ params\n" );
        return false;
    }

    status = lr1110_wifi_scan_time_limit( ral_context, wifi.settings.types, wifi.settings.channels,
                                          wifi.settings.scan_mode, wifi.settings.max_results,
                                          wifi.settings.timeout_per_channel, wifi.settings.timeout_per_scan );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to start Wi-Fi scan\n" );
        return false;
    }

    return true;
}

/* --- EOF ------------------------------------------------------------------ */