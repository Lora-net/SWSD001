/*!
 * @file      main_dm_info.h
 *
 * @brief     LoRa Basics Modem device management information example configuration
 *
 * @copyright
 * Revised BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
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

#ifndef MAIN_DM_INFO_H
#define MAIN_DM_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include "smtc_modem_api.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*!
 * @brief If true, then the system will not power down all peripherals when going to low power mode. This is necessary
 * to keep the LEDs active in low power mode.
 */
#define APP_PARTIAL_SLEEP false

/*!
 * @brief LoRaWAN fport used by the DM service
 */
#define APP_MODEM_DM_LORAWAN_FPORT 199

/*!
 * @brief Application device management format
 */
#define APP_MODEM_DM_FORMAT SMTC_MODEM_DM_INFO_INTERVAL_IN_MINUTE

/*!
 * @brief Application device management interval
 */
#define APP_MODEM_DM_INTERVAL 2

/*!
 * @brief Application device management manual request interval in second (0 to disable it)
 */
#define APP_MODEM_DM_TRIGGER_INTERVAL 50

/*!
 * @brief Device management fields to be reported with a call to @ref smtc_modem_dm_set_info_fields
 *
 * @see @ref SMTC_MODEM_DM_INFO_DEF
 */
const uint8_t app_modem_dm_fields[] = {
    SMTC_MODEM_DM_FIELD_STATUS,
    SMTC_MODEM_DM_FIELD_CHARGE,
    SMTC_MODEM_DM_FIELD_VOLTAGE,
    SMTC_MODEM_DM_FIELD_TEMPERATURE,
};

/*!
 * @brief Device management fields to be reported with a call to @ref smtc_modem_dm_request_single_uplink
 *
 * @see @ref SMTC_MODEM_DM_INFO_DEF
 */
const uint8_t app_modem_dm_fields_single[] = {
    SMTC_MODEM_DM_FIELD_STATUS,      SMTC_MODEM_DM_FIELD_CHARGE,   SMTC_MODEM_DM_FIELD_VOLTAGE,
    SMTC_MODEM_DM_FIELD_TEMPERATURE, SMTC_MODEM_DM_FIELD_SIGNAL,   SMTC_MODEM_DM_FIELD_UP_TIME,
    SMTC_MODEM_DM_FIELD_RX_TIME,     SMTC_MODEM_DM_FIELD_ADR_MODE, SMTC_MODEM_DM_FIELD_JOIN_EUI,
    SMTC_MODEM_DM_FIELD_INTERVAL,    SMTC_MODEM_DM_FIELD_REGION,   SMTC_MODEM_DM_FIELD_RST_COUNT,
    SMTC_MODEM_DM_FIELD_DEV_EUI,     SMTC_MODEM_DM_FIELD_SESSION,  SMTC_MODEM_DM_FIELD_CHIP_EUI,
    SMTC_MODEM_DM_FIELD_APP_STATUS,
};

/*!
 * @brief Application device management user-defined data
 */
const uint8_t app_modem_dm_fields_user_data[8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif /* MAIN_DM_INFO_H */

/* --- EOF ------------------------------------------------------------------ */
