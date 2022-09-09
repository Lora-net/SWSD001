/*!
 * @file      main_lorawan_class_b.h
 *
 * @brief     LoRa Basics Modem Class B device application configuration
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

#ifndef MAIN_LORAWAN_CLASS_B_H
#define MAIN_LORAWAN_CLASS_B_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "smtc_modem_api.h"

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
 * --- LoRaWAN Configuration ---------------------------------------------------
 */

/*!
 * @brief LoRaWAN class B ping slot periodicity
 */
#define LORAWAN_CLASS_B_PING_SLOT_DEFAULT SMTC_MODEM_CLASS_B_PINGSLOT_16_S

/*!
 * @brief Interval between 2 time synchronization once time is acquired
 */
#define TIME_SYNC_APP_INTERVAL_S_DEFAULT ( 86400 )  // 24h in secondes

/*!
 * @brief Invalid delay for time sync
 */
#define TIME_SYNC_APP_INVALID_DELAY_S_DEFAULT ( 3 * TIME_SYNC_APP_INTERVAL_S )

#ifndef LORAWAN_CLASS_B_PING_SLOT
#define LORAWAN_CLASS_B_PING_SLOT LORAWAN_CLASS_B_PING_SLOT_DEFAULT
#endif  // LORAWAN_CLASS_B_PING_SLOT

#ifndef TIME_SYNC_APP_INTERVAL_S
#define TIME_SYNC_APP_INTERVAL_S TIME_SYNC_APP_INTERVAL_S_DEFAULT
#endif  // TIME_SYNC_APP_INTERVAL_S

#ifndef TIME_SYNC_APP_INVALID_DELAY_S
#define TIME_SYNC_APP_INVALID_DELAY_S TIME_SYNC_APP_INVALID_DELAY_S_DEFAULT
#endif  // TIME_SYNC_APP_INVALID_DELAY_S

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

#endif  // MAIN_LORAWAN_CLASS_B_H

/* --- EOF ------------------------------------------------------------------ */
