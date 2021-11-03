/*!
 * \file      ral_lr1110_bsp.c
 *
 * \brief     Implements the BSP (BoardSpecificPackage) HAL functions for LR1110
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

#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

#include "ral_lr1110_bsp.h"
#include "device_management_defs.h"
#include "smtc_hal_mcu.h"
#include "modem_context.h"

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

void ral_lr1110_bsp_get_rf_switch_cfg( const void* context, lr1110_system_rfswitch_cfg_t* rf_switch_cfg )
{
    rf_switch_cfg->enable =
        LR1110_SYSTEM_RFSW0_HIGH | LR1110_SYSTEM_RFSW1_HIGH | LR1110_SYSTEM_RFSW2_HIGH | LR1110_SYSTEM_RFSW3_HIGH;
    rf_switch_cfg->standby = 0;
    rf_switch_cfg->rx      = LR1110_SYSTEM_RFSW0_HIGH;
    rf_switch_cfg->tx      = LR1110_SYSTEM_RFSW0_HIGH | LR1110_SYSTEM_RFSW1_HIGH;
    rf_switch_cfg->tx_hp   = LR1110_SYSTEM_RFSW1_HIGH;
    rf_switch_cfg->tx_hf   = 0;
    rf_switch_cfg->gnss    = LR1110_SYSTEM_RFSW2_HIGH;
    rf_switch_cfg->wifi    = LR1110_SYSTEM_RFSW3_HIGH;
}

void ral_lr1110_bsp_get_tx_cfg( const void* context, const ral_lr1110_bsp_tx_cfg_input_params_t* input_params,
                                ral_lr1110_bsp_tx_cfg_output_params_t* output_params )
{
    int8_t modem_tx_offset;

    // get modem_configured tx power offset
    if( smtc_modem_get_tx_power_offset_db( 0, &modem_tx_offset ) != SMTC_MODEM_RC_OK )
    {
        // in case rc code is not RC_OK, this function will not return the offset and we need to use no offset (in test
        // mode for example)
        modem_tx_offset = 0;
    }

    int8_t power = input_params->system_output_pwr_in_dbm + modem_tx_offset;

    // The output power must be in range [ -17 , +22 ] dBm
    if( power < -17 )
    {
        power = -17;
    }
    else if( power > 22 )
    {
        power = 22;
    }

    if( power <= 14 )
    {
        output_params->pa_cfg.pa_duty_cycle = 4;
        output_params->pa_cfg.pa_hp_sel     = 0;
        output_params->pa_cfg.pa_reg_supply = LR1110_RADIO_PA_REG_SUPPLY_VREG;
        output_params->pa_cfg.pa_sel        = LR1110_RADIO_PA_SEL_LP;

        output_params->chip_output_pwr_in_dbm_configured = power;
        output_params->chip_output_pwr_in_dbm_expected   = power;
    }
    else if( power == 15 )
    {
        output_params->pa_cfg.pa_duty_cycle = 7;
        output_params->pa_cfg.pa_hp_sel     = 0;
        output_params->pa_cfg.pa_reg_supply = LR1110_RADIO_PA_REG_SUPPLY_VREG;
        output_params->pa_cfg.pa_sel        = LR1110_RADIO_PA_SEL_LP;

        output_params->chip_output_pwr_in_dbm_configured = 14;
        output_params->chip_output_pwr_in_dbm_expected   = power;
    }
    else
    {
        output_params->pa_cfg.pa_duty_cycle = 4;
        output_params->pa_cfg.pa_hp_sel     = 7;
        output_params->pa_cfg.pa_reg_supply = LR1110_RADIO_PA_REG_SUPPLY_VBAT;
        output_params->pa_cfg.pa_sel        = LR1110_RADIO_PA_SEL_HP;

        output_params->chip_output_pwr_in_dbm_configured = power;
        output_params->chip_output_pwr_in_dbm_expected   = power;
    }

    output_params->pa_ramp_time = LR1110_RADIO_RAMP_48_US;
}

void ral_lr1110_bsp_get_reg_mode( const void* context, lr1110_system_reg_mode_t* reg_mode )
{
    // TODO: manage reg mode context saving, for the moment assume that LR1110 is in DCDC reg mode
    *reg_mode = LR1110_SYSTEM_REG_MODE_DCDC;
}

void ral_lr1110_bsp_get_xosc_cfg( const void* context, bool* tcxo_is_radio_controlled,
                                  lr1110_system_tcxo_supply_voltage_t* supply_voltage, uint32_t* startup_time_in_tick )
{
    // Radio control TCXO 1.8V and 5 ms of startup time
    *tcxo_is_radio_controlled = true;
    *supply_voltage           = LR1110_SYSTEM_TCXO_CTRL_1_8V;
    *startup_time_in_tick     = 164;  // 5ms in 30.52µs ticks
}

void ral_lr1110_bsp_get_crc_state( const void* context, bool* crc_is_activated )
{
#if defined( USE_LR1110_CRC_OVER_SPI )
    TRACE_PRINTF( "\x1B[0;32mINFO: LR1110 CRC over spi is activated\n\x1B[0m" );
    *crc_is_activated = true;
#else
    *crc_is_activated = false;
#endif
}

/* --- EOF ------------------------------------------------------------------ */
