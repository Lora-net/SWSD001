/*!
 * @file      lr1110_mb1lxks_board.h
 *
 * @brief     Target board LR1110 MB1LxKS shield board board driver definition
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

#ifndef LR1110_MB1LXKS_BOARD_H
#define LR1110_MB1LXKS_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "smtc_hal.h"
#include "modem_pinout.h"
#include "lis2de12.h"
#include "leds.h"
#include "external_supply.h"
#include "usr_button.h"
#include "smtc_modem_api.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define BOARD_TX_POWER_OFFSET -2

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Initializes the radio I/Os pins context
 *
 * @param [in] context Radio abstraction
 */
// void lr1110_modem_board_init_io_context( void* context );

/*!
 * @brief Initializes the radio I/Os pins interface
 *
 * @param [in] context Radio abstraction
 */
// void lr1110_modem_board_init_io( const void* context );

/*!
 * @brief De-initializes the radio I/Os pins interface.
 *
 * @param [in] context Radio abstraction
 *
 * @remark Useful when going in MCU low power modes
 */
// void lr1110_modem_board_deinit_io( const void* context );

/*!
 * @brief De-initializes the radio I/Os pins interface for deep sleep purpose --> switch Busy and DIO in analog input.
 *
 * @param [in] context Radio abstraction
 *
 * @remark Useful when going in MCU low power modes
 */
// void lr1110_modem_board_analog_deinit_io( const void* context );

/*!
 * @brief turn on the LNA
 */
void smtc_board_lna_on( void );

/*!
 * @brief turn off the LNA
 */
void smtc_board_lna_off( void );

/*!
 * @brief Measure the dropout voltage when the board drains batteries current
 *
 * @param [in] stack_id The stack identifier
 * @param [out] drop Voltage drop measured during the TX
 * @param [out] time_recovery  time taken to the supply rail to reach Vnom after th TX shutdown
 *
 * @returns LoRa Basics Modem response code
 */
smtc_modem_return_code_t smtc_board_measure_battery_drop( uint8_t stack_id, int32_t* drop, uint32_t* time_recovery );

#ifdef __cplusplus
}
#endif

#endif  // LR1110_MB1LXKS_BOARD_H

/* --- EOF ------------------------------------------------------------------ */
