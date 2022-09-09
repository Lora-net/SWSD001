/**
 * @file      ral_sx126x_bsp.c
 *
 * @brief     Board Support Package for the SX126x-specific Radio Abstraction Layer.
 *
 * Revised BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "ral_sx126x_bsp.h"
#include "smtc_board.h"
#include "smtc_shield_sx126x_common_if.h"

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

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void ral_sx126x_bsp_get_reg_mode( const void* context, sx126x_reg_mod_t* reg_mode )
{
    *reg_mode = smtc_shield_sx126x_get_reg_mode( );
}

void ral_sx126x_bsp_get_rf_switch_cfg( const void* context, bool* dio2_is_set_as_rf_switch )
{
    *dio2_is_set_as_rf_switch = smtc_shield_sx126x_is_dio2_set_as_rf_switch( );
}

void ral_sx126x_bsp_get_tx_cfg( const void* context, const ral_sx126x_bsp_tx_cfg_input_params_t* input_params,
                                ral_sx126x_bsp_tx_cfg_output_params_t* output_params )
{
    const int8_t modem_tx_offset = smtc_board_get_tx_power_offset( );

    const smtc_shield_sx126x_pa_pwr_cfg_t* pa_pwr_cfg = smtc_shield_sx126x_get_pa_pwr_cfg(
        input_params->freq_in_hz, input_params->system_output_pwr_in_dbm + modem_tx_offset );

    output_params->chip_output_pwr_in_dbm_expected   = input_params->system_output_pwr_in_dbm + modem_tx_offset;
    output_params->chip_output_pwr_in_dbm_configured = pa_pwr_cfg->power;

    output_params->pa_cfg.device_sel    = pa_pwr_cfg->pa_config.device_sel;
    output_params->pa_cfg.hp_max        = pa_pwr_cfg->pa_config.hp_max;
    output_params->pa_cfg.pa_duty_cycle = pa_pwr_cfg->pa_config.pa_duty_cycle;
    output_params->pa_cfg.pa_lut        = pa_pwr_cfg->pa_config.pa_lut;
    output_params->pa_ramp_time         = SX126X_RAMP_40_US;
}

void ral_sx126x_bsp_get_xosc_cfg( const void* context, bool* tcxo_is_radio_controlled,
                                  sx126x_tcxo_ctrl_voltages_t* supply_voltage, uint32_t* startup_time_in_tick )
{
    smtc_shield_sx126x_get_xosc_cfg( tcxo_is_radio_controlled, supply_voltage, startup_time_in_tick );
}

void ral_sx126x_bsp_get_ocp_value( const void* context, uint8_t* ocp_in_step_of_2_5_ma )
{
    smtc_shield_sx126x_get_ocp_value( ocp_in_step_of_2_5_ma );
}

/* --- EOF ------------------------------------------------------------------ */
