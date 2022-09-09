/*!
 * @file      main_region_switching.h
 *
 * @brief     LoRa Basics Modem region switching demonstration
 *
 * @copyright
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

#ifndef MAIN_REGION_SWITCHING_H
#define MAIN_REGION_SWITCHING_H

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
 * @brief JoinEUI to be used when joining the 2.4GHz network
 */
static const uint8_t join_eui_2g4[SMTC_MODEM_EUI_LENGTH] = { 0x00, 0x16, 0xC0, 0x01, 0xFF, 0xFE, 0x00, 0x02 };

/*!
 * @brief LoRaWAN application port
 */
#define LORAWAN_APP_PORT_DEFAULT 2

/*!
 * @brief Number of consecutive uplinks without downlink before sending LoRaWAN link check
 */
#define LORAWAN_MAX_LOST_CONNECTION_CNT_BEFORE_LINK_CHECK 10

/*!
 * @brief Number of consecutive uplinks without downlink after sending the first LoRaWAN link check before switch region
 */
#define LORAWAN_MAX_LINK_CHECK_KO_BEFORE_SWITCH 10

/*!
 * @brief Length of the region switching command
 */
#define APP_MODEM_REGION_SWITCHING_CMD_LEN 2

/*!
 * @brief Opcode of the region switching command
 */
#define APP_MODEM_REGION_SWITCHING_OPCODE 0x70

/*!
 * @brief Default datarate
 *
 * @remark See @ref smtc_modem_adr_profile_t
 */
#define LORAWAN_DEFAULT_DATARATE_DEFAULT SMTC_MODEM_ADR_PROFILE_NETWORK_CONTROLLED

/*!
 * @brief Interval in second between two temperature measurements
 */
#define APP_MODEM_TEMPERATURE_RECORD_INTERVAL 15

#ifndef LORAWAN_APP_PORT
#define LORAWAN_APP_PORT LORAWAN_APP_PORT_DEFAULT
#endif  // LORAWAN_APP_PORT

#ifndef LORAWAN_DEFAULT_DATARATE
#define LORAWAN_DEFAULT_DATARATE LORAWAN_DEFAULT_DATARATE_DEFAULT
#endif  // LORAWAN_DEFAULT_DATARATE

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

#endif  // MAIN_REGION_SWITCHING_H

/* --- EOF ------------------------------------------------------------------ */
