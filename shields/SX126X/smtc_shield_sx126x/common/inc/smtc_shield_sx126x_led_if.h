/*!
 * \file      smtc_shield_sx126x_led_if.h
 *
 * \brief     Interface for functions driving LEDs
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
#ifndef SMTC_SHIELD_SX126X_LED_IF_H
#define SMTC_SHIELD_SX126X_LED_IF_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include "sx126x.h"

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

/**
 * @brief Initialize the GPIO driving the TX LED
 */
void smtc_shield_sx126x_init_led_tx( void );

/**
 * @brief Initialize the GPIO driving the RX LED
 */
void smtc_shield_sx126x_init_led_rx( void );

/**
 * @brief De-initialize the GPIO driving the TX LED
 */
void smtc_shield_sx126x_deinit_led_tx( void );

/**
 * @brief De-initialize the GPIO driving the RX LED
 */
void smtc_shield_sx126x_deinit_led_rx( void );

/**
 * @brief Configure the GPIO to switch on the TX LED
 */
void smtc_shield_sx126x_set_led_tx( void );

/**
 * @brief Configure the GPIO to switch off the TX LED
 */
void smtc_shield_sx126x_reset_led_tx( void );

/**
 * @brief Configure the GPIO to switch on the RX LED
 */
void smtc_shield_sx126x_set_led_rx( void );

/**
 * @brief Configure the GPIO to switch off the RX LED
 */
void smtc_shield_sx126x_reset_led_rx( void );

#ifdef __cplusplus
}
#endif

#endif  // SMTC_SHIELD_SX126X_LED_IF_H
