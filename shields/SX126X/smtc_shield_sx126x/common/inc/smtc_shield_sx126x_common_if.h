/*!
 * \file      smtc_shield_sx126x_common_if.h
 *
 * \brief     Interface for functions common to all SX126x-based shields
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
#ifndef SMTC_SHIELD_SX126X_COMMON_IF_H
#define SMTC_SHIELD_SX126X_COMMON_IF_H

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

/**
 * @brief Power amplifier and output power configurations structure definition
 */
typedef struct smtc_shield_sx126x_pa_pwr_cfg_s
{
    int8_t                 power;
    sx126x_pa_cfg_params_t pa_config;
} smtc_shield_sx126x_pa_pwr_cfg_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/**
 * @brief Get power amplifier and output power configurations for a given expected output power
 *
 * @param [in] rf_freq_in_hz RF frequency in Hz
 * @param [in] expected_output_pwr_in_dbm Expected output power in dBm
 *
 * @returns A pointer to a structure holding the configuration to be applied to the chip
 */
const smtc_shield_sx126x_pa_pwr_cfg_t* smtc_shield_sx126x_get_pa_pwr_cfg( const uint32_t rf_freq_in_hz,
                                                                          const int8_t   expected_output_pwr_in_dbm );

/**
 * @brief Get the information about DIO2 driving a RF switch or not
 *
 * @returns A boolean giving the information if DIO2 drives a RF switch or not
 */
bool smtc_shield_sx126x_is_dio2_set_as_rf_switch( void );

/**
 * @brief Get the regulator mode used
 *
 * @returns The regulator mode
 */
sx126x_reg_mod_t smtc_shield_sx126x_get_reg_mode( void );

/**
 * @brief Get the XOSC configuration
 *
 * @remark If no TCXO is present, this function should set tcxo_is_radio_controlled to false, and return.
 *
 * @param [out] tcxo_is_radio_controlled Let the caller know if there is a radio-controlled TCXO
 * @param [out] supply_voltage TCXO supply voltage parameter
 * @param [out] startup_time_in_tick TCXO setup time in clock tick
 */
void smtc_shield_sx126x_get_xosc_cfg( bool* tcxo_is_radio_controlled, sx126x_tcxo_ctrl_voltages_t* supply_voltage,
                                      uint32_t* startup_time_in_tick );

/**
 * @brief Get the OCP (Over Current Protection) value
 *
 * @param [out] ocp_in_step_of_2_5_ma OCP value given in steps of 2.5 mA
 */
void smtc_shield_sx126x_get_ocp_value( uint8_t* ocp_in_step_of_2_5_ma );

/**
 * @brief Initialize components available on the shield
 */
void smtc_shield_sx126x_init( void );

/**
 * @brief De-initialize components available on the shield
 */
void smtc_shield_sx126x_deinit( void );

/**
 * @brief Configure the shield before entering in RX mode
 */
void smtc_shield_sx126x_handle_pre_rx( void );

/**
 * @brief Configure the shield after leaving RX mode
 */
void smtc_shield_sx126x_handle_post_rx( void );

/**
 * @brief Configure the shield before entering in TX mode
 */
void smtc_shield_sx126x_handle_pre_tx( void );

/**
 * @brief Configure the shield after leaving TX mode
 */
void smtc_shield_sx126x_handle_post_tx( void );

#ifdef __cplusplus
}
#endif

#endif  // SMTC_SHIELD_SX126X_COMMON_IF_H
