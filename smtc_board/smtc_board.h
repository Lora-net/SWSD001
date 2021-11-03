/*!
 * @file      smtc_board.h
 *
 * @brief     Board specific package board API definition.
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
#ifndef SMTC_BOARD_H
#define SMTC_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "smtc_hal_trace.h"

#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

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

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Initializes HAL used Peripherals
 */
void smtc_board_init_periph( void );

/*!
 * @brief reinit the peripherals
 */
void smtc_board_reinit_periph( void );

/*!
 * @brief deinit the peripherals
 */
void smtc_board_deinit_periph( void );

/*!
 * @brief Turn on/off the requested LED(s)
 *
 * @param [in] led_mask Mask representing the list of the LEDs to turn on/off
 * @param [in] turn_on If true, the requested LEDs are turned on, else they are turned off
 */
void smtc_board_led_set( uint32_t led_mask, bool turn_on );

/*!
 * @brief Get the status on/off of the requested LED(s)
 *
 * @param [in] led_mask Mask representing the list of the LEDs to turn on/off
 *
 * @return LED mask state
 */
uint32_t smtc_board_led_get( uint32_t led_mask );

/*!
 * @brief Turn on/off the requested LED(s) for a given duration
 *
 * @param [in] led_mask Mask representing the list of the LEDs to turn on/off
 * @param [in] turn_on If true, the requested LEDs are turned on, else they are turned off
 * @param [in] duration_ms Duration of the pulse, in milliseconds
 */
void smtc_board_led_pulse( uint32_t led_mask, bool turn_on, uint32_t duration_ms );

/*!
 * @brief Return the  mask for TX LED
 *
 * The mask is to be used for LED control functions like smtc_board_led_set or smtc_board_led_pulse
 *
 * @return TX LED mask
 */
uint32_t smtc_board_get_led_tx_mask( void );

/*!
 * @brief Return the  mask for RX LED
 *
 * The mask is to be used for LED control functions like smtc_board_led_set or smtc_board_led_pulse
 *
 * @return RX LED mask
 */
uint32_t smtc_board_get_led_rx_mask( void );

/*!
 * @brief Return the  mask for ALL LED
 *
 * The mask is to be used for LED control functions like smtc_board_led_set or smtc_board_led_pulse
 *
 * @return ALL LED mask
 */
uint32_t smtc_board_get_led_all_mask( void );

/*!
 * @brief Select and toggle Leds
 *
 * @param [in] leds Leds MASK to use
 * @param [in] delay Blink delay
 * @param [in] nb_blink Number of blink
 */
void smtc_board_leds_blink( uint8_t leds, uint32_t delay, uint8_t nb_blink );

/*!
 * @brief notify the user is the modem is ready
 *
 * @returns Modem ready state.
 */
bool smtc_board_is_ready( void );

/*!
 * @brief set the modem is ready flag
 *
 * @param [in] ready ready state
 */
void smtc_board_set_ready( bool ready );

/*!
 * @brief return the user button state.
 *
 * @returns true if usr button has been pushed, false if not.
 */
bool smtc_board_get_usr_button( void );

/*!
 * @brief return the tx power offset of the board.
 *
 * @returns tx power offset in dB.
 */
int smtc_board_get_tx_power_offset( void );

#ifdef __cplusplus
}
#endif

#endif  // SMTC_BOARD_H

/* --- EOF ------------------------------------------------------------------ */
