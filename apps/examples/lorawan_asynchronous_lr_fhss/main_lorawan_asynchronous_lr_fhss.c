/*!
 * @file      main_lorawan_asynchronous_lr_fhss.c
 *
 * @brief     LoRaWAN application sending asynchronous LR-FHSS uplinks
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

#include "main_lorawan_asynchronous_lr_fhss.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_modem_common.h"
#include "usr_button.h"
#include "apps_modem_event.h"
#include "smtc_modem_utilities.h"
#include "smtc_board_ralf.h"
#include "apps_utilities.h"
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
 * @brief LR-FHSS workaround currently needed for US-915 and AU-915 regions
 */
#define LORAWAN_LR_FHSS_WORKAROUND_CRYSTAL_ERROR ( 14000 )

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
 * @brief User application data
 */
static uint8_t app_data_buffer[LORAWAN_APP_DATA_MAX_SIZE];

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief   Send an application frame on LoRaWAN port defined by LORAWAN_APP_PORT
 *
 * @param [in] buffer     Buffer containing the LoRaWAN buffer
 * @param [in] length     Payload length
 * @param [in] confirmed  Send a confirmed or unconfirmed uplink [false : unconfirmed / true : confirmed]
 */
static void send_frame( const uint8_t* buffer, const uint8_t length, const bool confirmed );

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
 * @brief New Link ADR event callback
 */
static void on_new_link_adr( void );
static void on_new_link_adr_eu_868( uint16_t datarate_mask );
static void on_new_link_adr_us_915( uint16_t datarate_mask );
static void on_new_link_adr_au_915( uint16_t datarate_mask );

/*!
 * @brief Pushbutton event callback
 */
static void on_pushbutton( void );

/*!
 * @brief Tx done event callback
 *
 * @param [in] status tx done status \ref event_tx_done_state_t
 */
static void on_modem_tx_done( smtc_modem_event_txdone_status_t status );

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
        .down_data             = on_modem_down_data,
        .join_fail             = NULL,
        .joined                = on_modem_network_joined,
        .link_status           = NULL,
        .mute                  = NULL,
        .new_link_adr          = on_new_link_adr,
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
    HAL_DBG_TRACE_INFO(
        "###### ===== LoRa Basics Modem LoRaWAN LR-FHSS asynchronous demo application ==== ######\n\n" );
    apps_modem_common_display_version_information( );

    uint32_t sleep_time_ms = 0;
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
                on_pushbutton( );
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
    HAL_DBG_TRACE_INFO( "  - LoRaWAN uplink Fport = %d\n", LORAWAN_APP_PORT );
    HAL_DBG_TRACE_INFO( "  - Confirmed uplink     = %s\n", ( LORAWAN_CONFIRMED_MSG_ON == true ) ? "Yes" : "No" );

    apps_modem_common_configure_lorawan_params( stack_id );

    smtc_modem_region_t region;
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region ) );

    uint32_t crystal_error;
    smtc_modem_get_crystal_error_ppm( &crystal_error );
    HAL_DBG_TRACE_INFO( "Previous crystal error setting: %d\n", crystal_error );

    if( region == SMTC_MODEM_REGION_US_915 )
    {
        crystal_error = LORAWAN_LR_FHSS_WORKAROUND_CRYSTAL_ERROR;
    }
    else if( region == SMTC_MODEM_REGION_AU_915 )
    {
        crystal_error = LORAWAN_LR_FHSS_WORKAROUND_CRYSTAL_ERROR;
    }

    HAL_DBG_TRACE_INFO( "Crystal error set to: %d\n", crystal_error );
    smtc_modem_set_crystal_error_ppm( crystal_error );

    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );
}

static void on_modem_network_joined( void )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, LORAWAN_DEFAULT_DATARATE, adr_custom_list ) );
}

static void on_new_link_adr( void )
{
    uint16_t datarate_mask = 0;
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_available_datarates( stack_id, &datarate_mask ) );

    smtc_modem_region_t region;
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_region( stack_id, &region ) );

    if( region == SMTC_MODEM_REGION_EU_868 )
    {
        on_new_link_adr_eu_868( datarate_mask );
    }
    else if( region == SMTC_MODEM_REGION_US_915 )
    {
        on_new_link_adr_us_915( datarate_mask );
    }
    else if( region == SMTC_MODEM_REGION_AU_915 )
    {
        on_new_link_adr_au_915( datarate_mask );
    }
    else
    {
        HAL_DBG_TRACE_WARNING( "Unsupported region\n" );
    }
}

static void on_new_link_adr_eu_868( uint16_t datarate_mask )
{
    // Get mask for rates 8 to 11
    datarate_mask = ( datarate_mask >> 8 ) & 0x0F;

    // For simplicity, this demo application only supports the cases where datarates 8-9, 10-11, or 8-11 are supported
    // by the gateway.
    if( datarate_mask == 0xF )
    {
        HAL_DBG_TRACE_INFO( "LR-FHSS datarates 8-11 are available\n" );
        uint8_t custom_datarate[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = { 8,  8,  8,  8,  9,  9,  9,  9,
                                                                       10, 10, 10, 10, 11, 11, 11, 11 };
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_datarate ) );
    }
    else if( datarate_mask == 0xC )
    {
        HAL_DBG_TRACE_INFO( "LR-FHSS datarates 10-11 are available\n" );
        uint8_t custom_datarate[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = { 10, 10, 10, 10, 10, 10, 10, 10,
                                                                       11, 11, 11, 11, 11, 11, 11, 11 };
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_datarate ) );
    }
    else if( datarate_mask == 0x3 )
    {
        HAL_DBG_TRACE_INFO( "LR-FHSS datarates 8-9 are available\n" );
        uint8_t custom_datarate[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = { 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9 };
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_datarate ) );
    }
    else if( datarate_mask == 0 )
    {
        HAL_DBG_TRACE_WARNING( "No LR-FHSS datarates are available\n" );
    }
    else
    {
        HAL_DBG_TRACE_WARNING( "Unsupported LR-FHSS datarates: 0x%x\n", datarate_mask );
    }
}

static void on_new_link_adr_us_915( uint16_t datarate_mask )
{
    // Get mask for rates 5 and 6
    datarate_mask = ( datarate_mask >> 5 ) & 0x03;

    if( datarate_mask == 0x3 )
    {
        HAL_DBG_TRACE_INFO( "LR-FHSS datarates 5-6 are available\n" );
        uint8_t custom_datarate[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = { 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6 };
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_datarate ) );
    }
    else if( datarate_mask == 0x2 )
    {
        HAL_DBG_TRACE_INFO( "LR-FHSS datarate 6 is available\n" );
        uint8_t custom_datarate[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 };
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_datarate ) );
    }
    else if( datarate_mask == 0x1 )
    {
        HAL_DBG_TRACE_INFO( "LR-FHSS datarate 5 is available\n" );
        uint8_t custom_datarate[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_datarate ) );
    }
    else
    {
        HAL_DBG_TRACE_WARNING( "No LR-FHSS datarates are available\n" );
    }
}

static void on_new_link_adr_au_915( uint16_t datarate_mask )
{
    // Get mask for rate 7
    datarate_mask = ( datarate_mask >> 7 ) & 0x01;

    if( datarate_mask == 0x01 )
    {
        HAL_DBG_TRACE_INFO( "LR-FHSS datarate 7 is available\n" );
        uint8_t custom_datarate[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 };
        ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_datarate ) );
    }
    else
    {
        HAL_DBG_TRACE_WARNING( "No LR-FHSS datarates are available\n" );
    }
}

static void on_pushbutton( void )
{
    smtc_modem_status_mask_t modem_status;
    uint32_t                 charge        = 0;
    uint8_t                  app_data_size = 0;

    /* Show user button push*/
    HAL_DBG_TRACE_INFO( "###### ===== BUTTON PUSH ==== ######\n" );

    /* Get the modem status and display it */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_status( stack_id, &modem_status ) );
    modem_status_to_string( modem_status );

    if( ( modem_status & SMTC_MODEM_STATUS_JOINED ) == 0 )
    {
        HAL_DBG_TRACE_INFO( "Cannot send uplink until joined\n" );
        return;
    }

    /* Get the modem charge counter */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_charge( &charge ) );

    app_data_buffer[app_data_size++] = ( uint8_t ) ( charge );
    app_data_buffer[app_data_size++] = ( uint8_t ) ( charge >> 8 );
    app_data_buffer[app_data_size++] = ( uint8_t ) ( charge >> 16 );
    app_data_buffer[app_data_size++] = ( uint8_t ) ( charge >> 24 );

    send_frame( app_data_buffer, app_data_size, LORAWAN_CONFIRMED_MSG_ON );
}

static void on_modem_tx_done( smtc_modem_event_txdone_status_t status )
{
    static uint32_t uplink_count = 0;

    if( status == SMTC_MODEM_EVENT_TXDONE_NOT_SENT )
    {
        HAL_DBG_TRACE_WARNING( "Uplink was not sent\n" );
    }
    else
    {
        HAL_DBG_TRACE_INFO( "Uplink count: %d\n", uplink_count );
        ++uplink_count;
    }
}

static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size )
{
    HAL_DBG_TRACE_INFO( "Downlink received:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN Fport = %d\n", port );
    HAL_DBG_TRACE_INFO( "  - Payload size  = %d\n", size );
    HAL_DBG_TRACE_INFO( "  - RSSI          = %d dBm\n", rssi - 64 );
    HAL_DBG_TRACE_INFO( "  - SNR           = %d dB\n", snr >> 2 );

    switch( rx_window )
    {
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX1:
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX2:
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RXC:
    {
        HAL_DBG_TRACE_INFO( "  - Rx window     = %s\n", smtc_modem_event_downdata_window_to_str( rx_window ) );
        break;
    }
    default:
    {
        HAL_DBG_TRACE_ERROR( "Unexpected Rx window %s\n", smtc_modem_event_downdata_window_to_str( rx_window ) );
        break;
    }
    }

    if( size != 0 )
    {
        HAL_DBG_TRACE_ARRAY( "Payload", payload, size );
    }
}

static void send_frame( const uint8_t* buffer, const uint8_t length, bool tx_confirmed )
{
    uint8_t tx_max_payload;
    int32_t duty_cycle;

    /* Check if duty cycle is available */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_duty_cycle_status( &duty_cycle ) );
    if( duty_cycle < 0 )
    {
        HAL_DBG_TRACE_WARNING( "Duty-cycle limitation - next possible uplink in %d ms \n\n", duty_cycle );
        return;
    }

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_next_tx_max_payload( stack_id, &tx_max_payload ) );
    if( length > tx_max_payload )
    {
        HAL_DBG_TRACE_WARNING( "Not enough space in buffer - send empty uplink to flush MAC commands \n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_request_empty_uplink( stack_id, true, LORAWAN_APP_PORT, tx_confirmed ) );
    }
    else
    {
        HAL_DBG_TRACE_INFO( "Request uplink\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_request_uplink( stack_id, LORAWAN_APP_PORT, tx_confirmed, buffer, length ) );
    }
}

/* --- EOF ------------------------------------------------------------------ */
