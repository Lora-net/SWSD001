/*!
 * @file      main_dm_info.h
 *
 * @brief     LoRa Basics Modem device management information example configuration
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
 * @brief LoRaWAN fport used by the DM service
 */
#define APP_MODEM_DM_LORAWAN_FPORT_DEFAULT 199

/*!
 * @brief Application device management format
 */
#define APP_MODEM_DM_FORMAT_DEFAULT SMTC_MODEM_DM_INFO_INTERVAL_IN_MINUTE

/*!
 * @brief Application device management interval
 */
#define APP_MODEM_DM_INTERVAL_DEFAULT 2

/*!
 * @brief Application device management manual request interval in second (0 to disable it)
 */
#define APP_MODEM_DM_TRIGGER_INTERVAL_DEFAULT 50

#ifndef APP_MODEM_DM_LORAWAN_FPORT
#define APP_MODEM_DM_LORAWAN_FPORT APP_MODEM_DM_LORAWAN_FPORT_DEFAULT
#endif  // APP_MODEM_DM_LORAWAN_FPORT

#ifndef APP_MODEM_DM_FORMAT
#define APP_MODEM_DM_FORMAT APP_MODEM_DM_FORMAT_DEFAULT
#endif  // APP_MODEM_DM_FORMAT

#ifndef APP_MODEM_DM_INTERVAL
#define APP_MODEM_DM_INTERVAL APP_MODEM_DM_INTERVAL_DEFAULT
#endif  // APP_MODEM_DM_INTERVAL

#ifndef APP_MODEM_DM_TRIGGER_INTERVAL
#define APP_MODEM_DM_TRIGGER_INTERVAL APP_MODEM_DM_TRIGGER_INTERVAL_DEFAULT
#endif  // APP_MODEM_DM_TRIGGER_INTERVAL

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
