/*!
 * @file      main_stream.c
 *
 * @brief     LoRa Basics Modem Stream example implementation
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

#include <string.h>

#include "main_stream.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_utilities.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"
#include "smtc_modem_utilities.h"
#include "smtc_board_ralf.h"

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

const char pangram[] = "The quick brown fox jumps over the lazy dog.";

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
 * @brief Current position in the pangram
 */
uint8_t pangram_index = 0;

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
 * @brief Alarm event callback
 */
static void on_modem_alarm( void );

/*!
 * @brief Stream done event callback
 */
static void on_modem_stream_done( void );

/*!
 * @brief Get the next chunk from the pangram string
 *
 * @param [in] buffer  Placeholder to put the next chunk to be streamed
 * @param [in] len     Length in byte of the chunk
 */
static void get_next_chunk_from_pangram( uint8_t* buffer, uint8_t len );

/*!
 * @brief Add a chunk to the stream buffer
 *
 * @param [in] len  Length in bytes of the chunk to be streamed
 */
static void add_chunk_to_stream_buffer( uint8_t len );

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
        .stream_done           = on_modem_stream_done,
        .time_updated_alc_sync = NULL,
        .tx_done               = NULL,
        .upload_done           = NULL,
    };

    ralf_t* modem_radio = smtc_board_initialise_and_get_ralf( );

    hal_mcu_disable_irq( );

    /* Init board */
    hal_mcu_init( );
    smtc_board_init_periph( );

    apps_modem_event_init( &smtc_event_callback );

    smtc_modem_init( modem_radio, &apps_modem_event_process );

    hal_mcu_enable_irq( );

    HAL_DBG_TRACE_INFO( "===== LoRa Basics Modem stream example =====\n\n" );
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

void on_modem_reset( uint16_t reset_count )
{
    HAL_DBG_TRACE_INFO( "Application parameters:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN stream port = %d\n", APP_SMTC_MODEM_STREAM_LORAWAN_FPORT );
    HAL_DBG_TRACE_INFO( "  - Stream cipher mode  = %s\n", xstr( APP_SMTC_MODEM_STREAM_CIPHER_MODE ) );
    HAL_DBG_TRACE_INFO( "  - Chunk size          = %d\n", APP_STREAM_CHUNK_SIZE );

    apps_modem_common_configure_lorawan_params( stack_id );

    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );
}

void on_modem_network_joined( void )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_stream_init( stack_id, APP_SMTC_MODEM_STREAM_LORAWAN_FPORT,
                                                  APP_SMTC_MODEM_STREAM_CIPHER_MODE,
                                                  APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO ) );

    add_chunk_to_stream_buffer( APP_STREAM_CHUNK_SIZE );

    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_STREAM_CHUNK_PERIOD ) );
}

void on_modem_alarm( void )
{
    add_chunk_to_stream_buffer( APP_STREAM_CHUNK_SIZE );

    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_STREAM_CHUNK_PERIOD ) );

    HAL_DBG_TRACE_INFO( "Next chunk will be added to stream in %d seconds\n", APP_STREAM_CHUNK_PERIOD );
}

void on_modem_stream_done( void ) { HAL_DBG_TRACE_INFO( "Stream buffer is now empty.\n" ); }

void get_next_chunk_from_pangram( uint8_t* buffer, uint8_t len )
{
    for( int i = 0; i < len; i++ )
    {
        buffer[i] = pangram[( pangram_index + i ) % strlen( pangram )];
    }

    pangram_index = ( pangram_index + len ) % strlen( pangram );

    buffer[len] = 0;  // Done to make the buffer a null-terminated string
    HAL_DBG_TRACE_INFO( "Chunk to be added is: %s (%d byte(s) long)\n", buffer, len );
}

void add_chunk_to_stream_buffer( uint8_t len )
{
    uint16_t pending;
    uint16_t free;
    uint8_t  buffer[255];

    ASSERT_SMTC_MODEM_RC( smtc_modem_stream_status( stack_id, &pending, &free ) );

    if( free >= len )
    {
        get_next_chunk_from_pangram( buffer, len );

        ASSERT_SMTC_MODEM_RC( smtc_modem_stream_add_data( stack_id, buffer, len ) );
    }
    else
    {
        HAL_DBG_TRACE_WARNING( "Not enough free space.\n" );
        return;
    }
}

/* --- EOF ------------------------------------------------------------------ */
