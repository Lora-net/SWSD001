/*!
 * @file      main_time_sync.h
 *
 * @brief     LoRa Basics Modem time synchronization example
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

#ifndef MAIN_TIME_SYNC_H
#define MAIN_TIME_SYNC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*!
 * @brief Application time synchronization service - see @ref smtc_modem_time_sync_service_t
 */
#define APP_MODEM_TIME_SYNC_SERVICE_DEFAULT SMTC_MODEM_TIME_MAC_SYNC

/*!
 * @brief Application time synchronization interval in second
 */
#define APP_MODEM_TIME_SYNC_INTERVAL_IN_S_DEFAULT 900

/*!
 * @brief Application time manual synchronization interval in second (0 to disable it)
 */
#define APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S_DEFAULT 130

#ifndef APP_MODEM_TIME_SYNC_SERVICE
#define APP_MODEM_TIME_SYNC_SERVICE APP_MODEM_TIME_SYNC_SERVICE_DEFAULT
#endif  // APP_MODEM_TIME_SYNC_SERVICE

#ifndef APP_MODEM_TIME_SYNC_INTERVAL_IN_S
#define APP_MODEM_TIME_SYNC_INTERVAL_IN_S APP_MODEM_TIME_SYNC_INTERVAL_IN_S_DEFAULT
#endif  // APP_MODEM_TIME_SYNC_INTERVAL_IN_S

#ifndef APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S
#define APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S_DEFAULT
#endif  // APP_MODEM_TIME_MANUAL_SYNC_INTERVAL_IN_S

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

#endif /* MAIN_TIME_SYNC_H */

/* --- EOF ------------------------------------------------------------------ */