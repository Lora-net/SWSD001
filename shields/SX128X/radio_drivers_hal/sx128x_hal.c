/**
 * @file      sx128x_hal.c
 *
 * @brief     Implements the SX128x radio HAL functions
 *
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

#include <stdint.h>
#include "sx128x_hal.h"
#include "sx128x_hal_context.h"
#include "smtc_hal_gpio.h"
#include "smtc_hal_spi.h"
#include "smtc_hal_mcu.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

typedef enum
{
    RADIO_SLEEP,
    RADIO_AWAKE
} radio_mode_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static volatile radio_mode_t radio_mode = RADIO_AWAKE;
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/**
 * @brief Wait until radio busy pin returns to 0
 */
static void sx128x_hal_wait_on_busy( const hal_gpio_pin_names_t busy_pin );

static void sx128x_hal_check_device_ready( const sx128x_hal_context_t* sx128x_context );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

sx128x_hal_status_t sx128x_hal_write( const void* context, const uint8_t* command, const uint16_t command_length,
                                      const uint8_t* data, const uint16_t data_length )

{
    // Get context
    const sx128x_hal_context_t* sx128x_context = ( const sx128x_hal_context_t* ) context;

    // Wait for readiness
    sx128x_hal_check_device_ready( sx128x_context );

    // Put NSS low to start spi transaction
    hal_gpio_set_value( sx128x_context->nss, HAL_GPIO_RESET );

    // Send write command
    for( uint16_t i = 0; i < command_length; i++ )
    {
        hal_spi_in_out( sx128x_context->spi_id, command[i] );
    }

    // Send data (write payload)
    for( uint16_t i = 0; i < data_length; i++ )
    {
        hal_spi_in_out( sx128x_context->spi_id, data[i] );
    }

    // Put NSS high as the spi transaction is finished
    hal_gpio_set_value( sx128x_context->nss, HAL_GPIO_SET );

    // Check whether the command is a sleep command to keep the state up to date
    if( ( command_length == 2 ) && ( command[0] == 0x84 ) )
    {
        radio_mode = RADIO_SLEEP;
    }

    return SX128X_HAL_STATUS_OK;
}

sx128x_hal_status_t sx128x_hal_read( const void* context, const uint8_t* command, const uint16_t command_length,
                                     uint8_t* data, const uint16_t data_length )
{
    const sx128x_hal_context_t* sx128x_context = ( const sx128x_hal_context_t* ) context;

    // Wait for readiness
    sx128x_hal_check_device_ready( sx128x_context );

    // Put NSS low to start spi transaction
    hal_gpio_set_value( sx128x_context->nss, HAL_GPIO_RESET );

    // Send read command
    for( uint16_t i = 0; i < command_length; i++ )
    {
        hal_spi_in_out( sx128x_context->spi_id, command[i] );
    }

    // Get read response bytes
    for( uint16_t i = 0; i < data_length; i++ )
    {
        data[i] = hal_spi_in_out( sx128x_context->spi_id, SX128X_NOP );
    }

    // Finish SPI transaction
    hal_gpio_set_value( sx128x_context->nss, HAL_GPIO_SET );

    return SX128X_HAL_STATUS_OK;
}

sx128x_hal_status_t sx128x_hal_reset( const void* context )
{
    const sx128x_hal_context_t* sx128x_context = ( const sx128x_hal_context_t* ) context;
    hal_gpio_set_value( sx128x_context->reset, HAL_GPIO_RESET );
    hal_mcu_wait_us( 2000 );
    hal_gpio_set_value( sx128x_context->reset, HAL_GPIO_SET );

    // Reset wakes up radio
    radio_mode = RADIO_AWAKE;

    return SX128X_HAL_STATUS_OK;
}
sx128x_hal_status_t sx128x_hal_wakeup( const void* context )
{
    const sx128x_hal_context_t* sx128x_context = ( const sx128x_hal_context_t* ) context;

    sx128x_hal_check_device_ready( sx128x_context );
    return SX128X_HAL_STATUS_OK;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */
void sx128x_hal_wait_on_busy( const hal_gpio_pin_names_t busy_pin )
{
    while( hal_gpio_get_value( busy_pin ) == HAL_GPIO_SET )
    {
    };
}

void sx128x_hal_check_device_ready( const sx128x_hal_context_t* sx128x_context )
{
    if( radio_mode != RADIO_SLEEP )
    {
        sx128x_hal_wait_on_busy( sx128x_context->busy );
    }
    else
    {
        // Busy is HIGH in sleep mode, wake-up the device with a small glitch on NSS
        hal_gpio_set_value( sx128x_context->nss, 0 );
        sx128x_hal_wait_on_busy( sx128x_context->busy );
        hal_gpio_set_value( sx128x_context->nss, 1 );
        radio_mode = RADIO_AWAKE;
    }
}

/* --- EOF ------------------------------------------------------------------ */
