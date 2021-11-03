/*!
 * @file      lr1110_mb1lxks_board.c
 *
 * @brief     Target board LR1110 MB1LxKS shield board board driver implementation
 *
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

#include <stdlib.h>

#include "smtc_board.h"
#include "lorawan_key_config.h"
#include "smtc_board_ralf.h"
#include "lr1110_mb1lxks_board.h"
#include "ralf_lr1110.h"
#include "modem_pinout.h"
#include "lr1110_hal_context.h"

#include "smtc_hal.h"
#include "modem_pinout.h"
#include "smtc_modem_api_lr1110_system.h"
#include "smtc_modem_test_api.h"
#include "lr1110_types.h"
#include "smtc_modem_hal_dbg_trace.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief LR1110 EVK LED context
 */
typedef struct
{
    timer_event_t led_timer;         /*!< @brief Pulse timer */
    bool          timer_initialized; /*!< @brief True if the pulse timer has been initialized, false otherwise */
} smtc_board_led_ctx_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief smtc board ready flag
 */
static bool smtc_board_ready = false;

/*!
 * @brief LR1110 EVK LED context array
 */
static smtc_board_led_ctx_t smtc_board_leds[LR1110_EVK_LED_COUNT] = { { .timer_initialized = false },
                                                                      { .timer_initialized = false },
                                                                      { .timer_initialized = false } };

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Pulse timer timeout callback
 *
 * @param context Context used to retrieve the index of the relevant LED.
 */
static void on_led_timer_event( void* context );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void smtc_board_init_periph( void )
{
    /* Init TX & RX Leds */
    leds_init( );

    /* External supplies */
    external_supply_init( LNA_SUPPLY_MASK );

    /* LIS2DE12 accelerometer */
    accelerometer_init( INT_1 );

    /* USR button */
    usr_button_init( );
}

void smtc_board_reinit_periph( void )
{
    leds_init( );

    /* External supplies */
    external_supply_init( LNA_SUPPLY_MASK );
}

void smtc_board_deinit_periph( void )
{
    leds_deinit( );

    /* Disable external supply */
    external_supply_deinit( LNA_SUPPLY_MASK );
}

bool smtc_board_get_usr_button( void )
{
    if( get_usr_button_irq_state( ) == true )
    {
        clear_usr_button_irq_state( );

        return true;
    }
    else
    {
        return false;
    }
}

void smtc_board_lna_on( void ) { lna_on( ); }

void smtc_board_lna_off( void ) { lna_off( ); }

bool smtc_board_is_ready( void ) { return smtc_board_ready; }

void smtc_board_set_ready( bool ready ) { smtc_board_ready = ready; }

void smtc_board_led_set( uint32_t led_mask, bool turn_on )
{
    /* If a pulse timer is running on one of the requested LEDs, it
     *  must be stopped to avoid conflicting with the requested LED state. */
    lr1110_evk_led_t led = LR1110_EVK_LED_TX;
    for( led = LR1110_EVK_LED_TX; led < LR1110_EVK_LED_COUNT; led++ )
    {
        if( led_mask & ( 1 << led ) )
        {
            if( ( smtc_board_leds[led].timer_initialized ) && ( timer_is_started( &smtc_board_leds[led].led_timer ) ) )
            {
                timer_stop( &smtc_board_leds[led].led_timer );
            }
        }
    }
    if( turn_on )
    {
        leds_on( led_mask );
    }
    else
    {
        leds_off( led_mask );
    }
}

uint32_t smtc_board_led_get( uint32_t led_mask ) { return leds_get_state( led_mask ); }

void smtc_board_led_pulse( uint32_t led_mask, bool turn_on, uint32_t duration_ms )
{
    lr1110_evk_led_t led = LR1110_EVK_LED_TX;
    for( led = LR1110_EVK_LED_TX; led < LR1110_EVK_LED_COUNT; led++ )
    {
        if( led_mask & ( 1 << led ) )
        {
            if( smtc_board_leds[led].timer_initialized )
            {
                if( timer_is_started( &smtc_board_leds[led].led_timer ) )
                {
                    timer_stop( &smtc_board_leds[led].led_timer );
                }
            }
            else
            {
                timer_init( &smtc_board_leds[led].led_timer, on_led_timer_event );
                timer_set_context( &smtc_board_leds[led].led_timer, ( void* ) led );
                smtc_board_leds[led].timer_initialized = true;
            }
            timer_set_value( &smtc_board_leds[led].led_timer, duration_ms );
            timer_start( &smtc_board_leds[led].led_timer );
        }
    }
    if( turn_on )
    {
        leds_on( led_mask );
    }
    else
    {
        leds_off( led_mask );
    }
}

uint32_t smtc_board_get_led_tx_mask( void ) { return LED_TX_MASK; }

uint32_t smtc_board_get_led_rx_mask( void ) { return LED_RX_MASK; }

uint32_t smtc_board_get_led_all_mask( void ) { return LED_ALL_MASK; }

void smtc_board_leds_blink( uint8_t leds, uint32_t delay, uint8_t nb_blink )
{
    leds_blink( leds, delay, nb_blink, true );
}

ralf_t* smtc_board_initialise_and_get_ralf( void )
{
    static lr1110_hal_context_t context = {
        .nss    = SMTC_RADIO_NSS,
        .busy   = SMTC_RADIO_BUSY,
        .reset  = SMTC_RADIO_NRST,
        .spi_id = HAL_RADIO_SPI_ID,
    };
    static ralf_t local_ralf = { 0 };
    local_ralf               = ( ralf_t ) RALF_LR1110_INSTANTIATE( &context );
    return &local_ralf;
}

int smtc_board_get_tx_power_offset( void )
{
    return BOARD_TX_POWER_OFFSET;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

void on_led_timer_event( void* context )
{
    lr1110_evk_led_t led      = ( lr1110_evk_led_t ) context;
    uint32_t         led_mask = 1 << led;
    leds_toggle( led_mask );
    timer_stop( &smtc_board_leds[led].led_timer );
}

/* --- EOF ------------------------------------------------------------------ */
