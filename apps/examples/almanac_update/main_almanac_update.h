/*!
 * @file      main_almanac_update.h
 *
 * @brief     LoRa Basics Modem almanac update example
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

#ifndef MAIN_ALMANAC_UPDATE_H
#define MAIN_ALMANAC_UPDATE_H

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
 * @brief Application device management interval in minute
 */
#define APP_MODEM_DM_INTERVAL_DEFAULT 30

/*!
 * @brief Almanac update speed-up mode
 */
#define APP_MODEM_ALMANAC_UPDATE_SPEED_UP_DEFAULT true

#ifndef APP_MODEM_DM_LORAWAN_FPORT
#define APP_MODEM_DM_LORAWAN_FPORT APP_MODEM_DM_LORAWAN_FPORT_DEFAULT
#endif  // APP_MODEM_DM_LORAWAN_FPORT

#ifndef APP_MODEM_DM_INTERVAL
#define APP_MODEM_DM_INTERVAL APP_MODEM_DM_INTERVAL_DEFAULT
#endif  // APP_MODEM_DM_INTERVAL

#ifndef APP_MODEM_ALMANAC_UPDATE_SPEED_UP
#define APP_MODEM_ALMANAC_UPDATE_SPEED_UP APP_MODEM_ALMANAC_UPDATE_SPEED_UP_DEFAULT
#endif  // APP_MODEM_ALMANAC_UPDATE_SPEED_UP

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

#endif /* MAIN_ALMANAC_UPDATE_H */

/* --- EOF ------------------------------------------------------------------ */
