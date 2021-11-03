/*!
 * @ingroup   apps_geolocation
 * @file      gnss_helpers.c
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation example, GNSS scan helpers
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
 * @endparblockN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * @endparblock
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */
#include <string.h>

#include "apps_utilities.h"
#include "external_supply.h"

#include "smtc_hal_dbg_trace.h"
#include "smtc_modem_api.h"
#include "smtc_hal_gpio.h"
#include "modem_pinout.h"

#include "lr1110_system.h"
#include "lr1110_gnss.h"

#include "lr1110_geolocation_helpers.h"
#include "gnss_helpers.h"

#include "gnss_almanac_old.h" /* To be used for testing with ALMANAC_TOO_OLD */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*!
 * @brief Maximal number of SVs to search for in the GNSS scan configuration
 */
#define GNSS_MAX_NB_SAT 10

/*!
 * @brief Scan mode to use for GNSS scan configuration
 */
#define GNSS_SCAN_MODE LR1110_GNSS_SCAN_MODE_3_SINGLE_SCAN_AND_5_FAST_SCANS

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Container for the detected satellites description
 */
static lr1110_gnss_detected_satellite_t gnss_results[GNSS_MAX_NB_SAT];

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Configure and start assisted scan operation
 *
 * @param[in] ral_context Chip implementation context
 * @param[in] date Current date
 */
static bool gnss_assisted_scan( const void* ral_context, lr1110_gnss_date_t date );

/*!
 * @brief Parse a result buffer to determine if it is a NAV message to be sent to the solver
 *
 * @param[in] buffer Buffer containing the result to be parsed
 * @param[in] buffer_length Length of the given result buffer
 */
static inline bool gnss_is_result_to_solver( const uint8_t* buffer, uint8_t buffer_length );

/*!
 * @brief Parse a result buffer to determine if it is a message for the host (error message)
 *
 * @param[in] buffer Buffer containing the result to be parsed
 * @param[in] buffer_length Length of the given result buffer
 */
static inline bool gnss_is_result_to_host( const uint8_t* buffer, uint8_t buffer_length );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

bool smtc_gnss_init( const void* ral_context, const lr1110_gnss_solver_assistance_position_t assistance_position )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) );

    if( lr1110_gnss_set_assistance_position( ral_context, &assistance_position ) != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to set assistance position\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
        return false;
    }

    ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );

    return true;
}

void smtc_gnss_almanac_update( const void* ral_context )
{
    lr1110_gnss_context_status_bytestream_t context_status_bytestream;
    lr1110_gnss_context_status_t            context_status;

    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) );

    if( lr1110_gnss_get_context_status( ral_context, context_status_bytestream ) != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to get gnss context status\n" );
    }
    else
    {
        // parse context bytestream to get almanac crc
        lr1110_gnss_parse_context_status_buffer( context_status_bytestream, &context_status );

        HAL_DBG_TRACE_INFO( "context_status.error_code=%d\n", context_status.error_code );

        uint32_t local_almanac_crc =
            ( full_almanac[6] << 24 ) + ( full_almanac[5] << 16 ) + ( full_almanac[4] << 8 ) + ( full_almanac[3] );

        if( context_status.global_almanac_crc != local_almanac_crc )
        {
            HAL_DBG_TRACE_INFO( "Local almanac doesn't match LR1110 almanac -> start update\n" );

            // Load almanac in flash - do it only once
            uint16_t almanac_idx = 0;
            while( almanac_idx < sizeof( full_almanac ) )
            {
                if( lr1110_gnss_almanac_update( ral_context, full_almanac + almanac_idx, 1 ) != LR1110_STATUS_OK )
                {
                    HAL_DBG_TRACE_ERROR( "Failed to update almanac\n" );
                }
                almanac_idx += 20;
            }

            if( lr1110_gnss_get_context_status( ral_context, context_status_bytestream ) != LR1110_STATUS_OK )
            {
                HAL_DBG_TRACE_ERROR( "Failed to get gnss context status\n" );
            }
            else
            {
                // parse context bytestream to get almanac crc
                lr1110_gnss_parse_context_status_buffer( context_status_bytestream, &context_status );

                // check almanac crc again to decide if the update is successful or not
                if( context_status.global_almanac_crc != local_almanac_crc )
                {
                    HAL_DBG_TRACE_ERROR( "Local almanac doesn't match LR1110 almanac -> update fails\n" );
                }
                else
                {
                    HAL_DBG_TRACE_INFO( "Almanac update succeeds\n" );
                }
            }
        }
        else
        {
            HAL_DBG_TRACE_INFO( "Local almanac matches LR1110 almanac -> no update\n" );
        }
    }

    ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
}

bool smtc_gnss_scan( const uint8_t stack_id, const void* ral_context, uint32_t date )
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

    /* Suspend modem for LR1110 access */
    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) );

    HAL_DBG_TRACE_INFO( "start GNSS scan\n" );
    status = lr1110_configure_for_scan( ral_context );
    if( status == true )
    {
        /* Enable LNA for GNSS with LR1110 Evaluation board with passive antenna */
        lna_on( );

        /* Start scan */
        status = gnss_assisted_scan( ral_context, ( lr1110_gnss_date_t ) date );
        if( status == false )
        {
            HAL_DBG_TRACE_ERROR( "gnss_assisted_scan() failed\n" );
            lna_off( );
            ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
            return false;
        }
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "Failed to configure LR1110 for GNSS scan\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );
        return false;
    }

    /* Do not resume the modem here, to be done when SMTC_MODEM_EVENT_USER_RADIO_ACCESS_GNSS_SCAN_DONE event is
     * received, through the smtc_gnss_scan_ended() function call.
     */

    return true;
}

void smtc_gnss_scan_ended( void )
{
    /* The first thing to be done, resume the modem which has been suspended by smtc_gnss_scan() */
    ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );

    /* Disable LNA which was enabled during GNSS scan */
    lna_off( );
}

bool smtc_gnss_get_results( const void* ral_context, uint8_t* nb_detected_satellites, uint8_t* res_sz, uint8_t* results,
                            bool* update_almanac )
{
    lr1110_status_t status;
    uint16_t        result_size;

    // Indicate there is no almanac update to do. Only set it to true when the LR1110 indicate almanac update must be
    // driven with the adequate host return code
    *update_almanac = false;

    ASSERT_SMTC_MODEM_RC( smtc_modem_suspend_before_user_radio_access( ) );

    // This do/while construction is a mechanism to break execution flow only in forward direction as soon as something
    // wrong is detected (and the break is called). It allows to have a single point of exit even though the sequence of
    // commands in the do clause can break in many points.
    bool success = false;
    do
    {  // 1. Use read result API to fetch the result buffer
        status = lr1110_gnss_get_result_size( ral_context, &result_size );
        if( status != LR1110_STATUS_OK )
        {
            HAL_DBG_TRACE_ERROR( "Failed to get GNSS scan result size\n" );
            success = false;
            break;
        }

        *res_sz = result_size;
        HAL_DBG_TRACE_PRINTF( "GNSS scan result size: %d\n", result_size );

        status = lr1110_gnss_read_results( ral_context, results, result_size );
        if( status != LR1110_STATUS_OK )
        {
            HAL_DBG_TRACE_ERROR( "Failed to get result\n" );
            success = false;
            break;
        }

        // 2. Check if the message read from read result API is a NAV message or not. If it is not,print the appropriate
        // error message
        if( gnss_is_result_to_solver( results, result_size ) == false )
        {
            // The result read is not destinated to solver, it is probably an error message.
            if( gnss_is_result_to_host( results, result_size ) == true )
            {
                const lr1110_gnss_message_host_status_t status_code_raw =
                    ( lr1110_gnss_message_host_status_t ) results[1];
                switch( status_code_raw )
                {
                case LR1110_GNSS_HOST_NO_TIME:
                {
                    HAL_DBG_TRACE_ERROR( "GNSS error: NO DATE\n" );
                    break;
                }
                case LR1110_GNSS_HOST_NO_SATELLITE_DETECTED:
                {
                    HAL_DBG_TRACE_ERROR( "GNSS error: NO SATELLITE\n" );
                    break;
                }
                case LR1110_GNSS_HOST_ALMANAC_IN_FLASH_TOO_OLD:
                {
                    HAL_DBG_TRACE_ERROR( "GNSS error: ALMANAC TOO OLD\n" );
                    *update_almanac = true;
                    break;
                }
                case LR1110_GNSS_HOST_NOT_ENOUGH_SV_DETECTED_TO_BUILD_A_NAV_MESSAGE:
                {
                    HAL_DBG_TRACE_ERROR( "GNSS error: NOT ENOUGH SVs TO BUILD A NAV MESSAGE\n" );
                    break;
                }
                default:
                {
                    HAL_DBG_TRACE_ERROR( "GNSS error: UNKNOWN ERROR CODE: 0x%02X\n", status_code_raw );
                }
                }
            }
            else
            {
                HAL_DBG_TRACE_ERROR(
                    "GNSS error: NAV message is neither for host nor for solver. Destination byte: 0x%02x\n",
                    results[0] );
            }
            success = false;
            break;
        }

        // 3. The result read if destinated to solver, it is a NAV message, so SVs have been possibly detected. Fetch
        // the detected SVs.
        status = lr1110_gnss_get_nb_detected_satellites( ral_context, nb_detected_satellites );
        if( status != LR1110_STATUS_OK )
        {
            HAL_DBG_TRACE_ERROR( "Failed to get number of satellites detected\n" );
            success = false;
            break;
        }
        HAL_DBG_TRACE_PRINTF( "Found %d satellites\n", *nb_detected_satellites );

        status = lr1110_gnss_get_detected_satellites( ral_context, *nb_detected_satellites, gnss_results );
        if( status != LR1110_STATUS_OK )
        {
            HAL_DBG_TRACE_ERROR( "Failed to get detected satellites\n" );
            success = false;
            break;
        }
        success = true;
    } while( 0 );

    ASSERT_SMTC_MODEM_RC( smtc_modem_resume_after_user_radio_access( ) );

    return success;
}

void smtc_gnss_display_results( uint8_t nb_sats, uint8_t* results, uint8_t results_size )
{
    /* Display satellites info */
    for( uint8_t i = 0; i < nb_sats; i++ )
    {
        HAL_DBG_TRACE_PRINTF( "ID: %d - CNR: %d\n", gnss_results[i].satellite_id, gnss_results[i].cnr );
    }

    /* Display scan results */
    HAL_DBG_TRACE_ARRAY( "RESULTS", results, results_size );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static bool gnss_assisted_scan( const void* ral_context, lr1110_gnss_date_t date )
{
    lr1110_status_t status = LR1110_STATUS_ERROR;
    uint8_t         scan_input_parameters;

    status = lr1110_system_set_dio_irq_params( ral_context, LR1110_SYSTEM_IRQ_GNSS_SCAN_DONE, LR1110_SYSTEM_IRQ_NONE );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to set GNSS scan done IRQ params\n" );
        return false;
    }

    status = lr1110_gnss_set_scan_mode( ral_context, GNSS_SCAN_MODE );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to set GNSS scan mode\n" );
        return false;
    }

    status = lr1110_gnss_set_constellations_to_use( ral_context, LR1110_GNSS_GPS_MASK + LR1110_GNSS_BEIDOU_MASK );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to set constellations\n" );
        return false;
    }

    if( GNSS_SCAN_MODE == LR1110_GNSS_SCAN_MODE_3_SINGLE_SCAN_AND_5_FAST_SCANS )
    {
        scan_input_parameters = 0; /* no doppler, no bit change */
    }
    else
    {
        scan_input_parameters = LR1110_GNSS_RESULTS_LEGACY_PSEUDO_RANGE_MASK + LR1110_GNSS_RESULTS_LEGACY_DOPPLER_MASK +
                                LR1110_GNSS_RESULTS_LEGACY_BIT_CHANGE_MASK;
    }

    status = lr1110_gnss_scan_assisted( ral_context, date, LR1110_GNSS_OPTION_DEFAULT, scan_input_parameters,
                                        GNSS_MAX_NB_SAT );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to start an assisted GNSS scan\n" );
        return false;
    }

    return true;
}

static inline bool gnss_is_result_to_solver( const uint8_t* buffer, uint8_t buffer_length )
{
    if( buffer_length >= 2 )
    {
        return buffer[0] == LR1110_GNSS_DESTINATION_SOLVER;
    }
    else
    {
        return false;
    }
}

static inline bool gnss_is_result_to_host( const uint8_t* buffer, uint8_t buffer_length )
{
    if( buffer_length >= 2 )
    {
        return buffer[0] == LR1110_GNSS_DESTINATION_HOST;
    }
    else
    {
        return false;
    }
}

/* --- EOF ------------------------------------------------------------------ */