/*!
 * @file      main_tx_rx_continuous.c
 *
 * @brief     LoRa Basics TX/RX continuous implementation example
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

#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"
#include "smtc_modem_test_api.h"
#include "smtc_board_ralf.h"
#include "apps_utilities.h"
#include "smtc_modem_utilities.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*!
 * @brief TX continuous
 *
 * @note only on modulated
 */
#define TX_CONTINUOUS true

/*!
 * @brief RX continuous
 */
#define RX_CONTINUOUS false

/*!
 * @brief TX continuous or modulated
 */
#define TX_MODULATED true

/*!
 * @brief Delay in ms between two TX single
 */
#define TX_SINGLE_INTER_DELAY 1000

/*!
 * @brief Tx Power used during test, in dBm.
 */
#define TX_POWER_USED 14

/*!
 * @brief Tx Power offset used during test
 */
#define TX_POWER_OFFSET 0

/*!
 * @brief Preamble size in symbol \note only on modulated
 */
#define PREAMBLE_SIZE 8

/*!
 * @brief Tx payload len \note only on modulated
 */
#define TX_PAYLOAD_LEN 51

/*!
 * @brief LoRaWAN regulatory region \ref smtc_modem_region_t
 */
#define LORAWAN_REGION_USED SMTC_MODEM_REGION_EU_868

/*!
 * @brief Frequency used during test, \note the frequency SHALL be allow by the lorawan region,
 *       set 915MHz with EU868 region will not work
 */
#define FREQUENCY 868100000

/*!
 * @brief Spreading factor for test mode \see smtc_modem_test_sf_t
 */
#define SPREADING_FACTOR_USED SMTC_MODEM_TEST_LORA_SF7

/*!
 * @brief bandwidth for test mode \see smtc_modem_test_bw_t
 */
#define BANDWIDTH_USED SMTC_MODEM_TEST_BW_125_KHZ

/*!
 * @brief Coding rate for test mode \see smtc_modem_test_cr_t
 */
#define CODING_RATE_USED SMTC_MODEM_TEST_CR_4_5

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
        .alarm                 = NULL,
        .almanac_update        = NULL,
        .down_data             = NULL,
        .join_fail             = NULL,
        .joined                = NULL,
        .link_status           = NULL,
        .mute                  = NULL,
        .new_link_adr          = NULL,
        .reset                 = NULL,
        .set_conf              = NULL,
        .stream_done           = NULL,
        .time_updated_alc_sync = NULL,
        .tx_done               = NULL,
        .upload_done           = NULL,
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
    HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem TX/RX continuous demo application ==== ######\n\n" );

    if( TX_CONTINUOUS == RX_CONTINUOUS )
    {
        HAL_DBG_TRACE_ERROR( "Select between TX Continuous or RX Continuous\n" );
    }
    apps_modem_common_display_version_information( );

    ASSERT_SMTC_MODEM_RC( smtc_modem_set_region( stack_id, LORAWAN_REGION_USED ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_set_tx_power_offset_db( stack_id, TX_POWER_OFFSET ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_test_start( ) );

    if( TX_CONTINUOUS )
    {
        HAL_DBG_TRACE_PRINTF( "TX PARAM\r\n" );
        HAL_DBG_TRACE_PRINTF( "FREQ         : %d MHz\r\n", FREQUENCY );
        HAL_DBG_TRACE_PRINTF( "REGION       : %d\r\n", LORAWAN_REGION_USED );
        HAL_DBG_TRACE_PRINTF( "TX POWER     : %d dBm\r\n", TX_POWER_USED );
        if( TX_MODULATED )
        {
            HAL_DBG_TRACE_PRINTF( "TX           : MODULATED\r\n" );
            if( SPREADING_FACTOR_USED != SMTC_MODEM_TEST_FSK )
            {
                HAL_DBG_TRACE_PRINTF( "MODULATION   : LORA\r\n" );
                HAL_DBG_TRACE_PRINTF( "SF           : %d\r\n", SPREADING_FACTOR_USED + 6 );
                HAL_DBG_TRACE_PRINTF( "CR           : 4/%d\r\n", CODING_RATE_USED + 5 );
            }
            else
            {
                HAL_DBG_TRACE_PRINTF( "MODULATION   : FSK\r\n" );
            }
            HAL_DBG_TRACE_PRINTF( "BW           : %d\r\n", BANDWIDTH_USED );
        }
        else
        {
            HAL_DBG_TRACE_PRINTF( "TX           : CONTINUOUS\r\n" );
        }

        if( TX_MODULATED )
        {
            if( TX_CONTINUOUS == true )
            {
                ASSERT_SMTC_MODEM_RC( smtc_modem_test_tx( NULL, TX_PAYLOAD_LEN, FREQUENCY, TX_POWER_USED,
                                                          SPREADING_FACTOR_USED, BANDWIDTH_USED, CODING_RATE_USED,
                                                          PREAMBLE_SIZE, true ) );
            }
            else
            {
                while( 1 )
                {
                    HAL_DBG_TRACE_PRINTF( "TX\r\n" );
                    ASSERT_SMTC_MODEM_RC( smtc_modem_test_tx( NULL, TX_PAYLOAD_LEN, FREQUENCY, TX_POWER_USED,
                                                              SPREADING_FACTOR_USED, BANDWIDTH_USED, CODING_RATE_USED,
                                                              PREAMBLE_SIZE, false ) );
                    hal_mcu_delay_ms( TX_SINGLE_INTER_DELAY );
                }
            }
        }
        else
        {
            ASSERT_SMTC_MODEM_RC( smtc_modem_test_tx_cw( FREQUENCY, TX_POWER_USED ) );
        }
    }
    else
    {
        HAL_DBG_TRACE_PRINTF( "RX PARAM\r\n" );
        HAL_DBG_TRACE_PRINTF( "FREQ         : %d MHz\r\n", FREQUENCY );
        HAL_DBG_TRACE_PRINTF( "REGION       : %d\r\n", LORAWAN_REGION_USED );

        HAL_DBG_TRACE_PRINTF( "TX           : MODULATED\r\n" );
        if( SPREADING_FACTOR_USED != SMTC_MODEM_TEST_FSK )
        {
            HAL_DBG_TRACE_PRINTF( "MODULATION   : LORA\r\n" );
            HAL_DBG_TRACE_PRINTF( "SF           : %d\r\n", SPREADING_FACTOR_USED + 6 );
            HAL_DBG_TRACE_PRINTF( "CR           : 4/%d\r\n", CODING_RATE_USED + 5 );
        }
        else
        {
            HAL_DBG_TRACE_PRINTF( "MODULATION   : FSK\r\n" );
        }
        HAL_DBG_TRACE_PRINTF( "BW           : %d\r\n", BANDWIDTH_USED );

        ASSERT_SMTC_MODEM_RC(
            smtc_modem_test_rx_continuous( FREQUENCY, SPREADING_FACTOR_USED, BANDWIDTH_USED, CODING_RATE_USED ) );
    }

    while( 1 )
    {
        /* go in low power */
        hal_mcu_set_sleep_for_ms( 10000 );
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
