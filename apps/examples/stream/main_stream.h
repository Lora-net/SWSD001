/*!
 * @file      main_stream.h
 *
 * @brief     LoRa Basics Modem streaming application configuration
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

#ifndef MAIN_STREAM_H
#define MAIN_STREAM_H

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
 * @brief LoRaWAN fport used by the stream service
 */
#define APP_SMTC_MODEM_STREAM_LORAWAN_FPORT_DEFAULT 199

/*!
 * @brief Streaming encryption mode
 */
#define APP_SMTC_MODEM_STREAM_CIPHER_MODE_DEFAULT SMTC_MODEM_STREAM_NO_CIPHER

/*!
 * @brief Streaming redundancy ratio
 */
#define APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO_DEFAULT 110

/*!
 * @brief Size in byte of a chunk added to the stream
 *
 * @remark This value has to be in the range [1:254]
 */
#define APP_STREAM_CHUNK_SIZE_DEFAULT 40

/*!
 * @brief Period in second between two chunks being added to the stream buffer
 */
#define APP_STREAM_CHUNK_PERIOD_DEFAULT 60

#ifndef APP_SMTC_MODEM_STREAM_LORAWAN_FPORT
#define APP_SMTC_MODEM_STREAM_LORAWAN_FPORT APP_SMTC_MODEM_STREAM_LORAWAN_FPORT_DEFAULT
#endif  // APP_SMTC_MODEM_STREAM_LORAWAN_FPORT

#ifndef APP_SMTC_MODEM_STREAM_CIPHER_MODE
#define APP_SMTC_MODEM_STREAM_CIPHER_MODE APP_SMTC_MODEM_STREAM_CIPHER_MODE_DEFAULT
#endif  // APP_SMTC_MODEM_STREAM_CIPHER_MODE

#ifndef APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO
#define APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO_DEFAULT
#endif  // APP_SMTC_MODEM_STREAM_REDUNDANCY_RATIO

#ifndef APP_STREAM_CHUNK_SIZE
#define APP_STREAM_CHUNK_SIZE APP_STREAM_CHUNK_SIZE_DEFAULT
#endif  // APP_STREAM_CHUNK_SIZE

#ifndef APP_STREAM_CHUNK_PERIOD
#define APP_STREAM_CHUNK_PERIOD APP_STREAM_CHUNK_PERIOD_DEFAULT
#endif  // APP_STREAM_CHUNK_PERIOD

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

#endif /* MAIN_STREAM_H */

/* --- EOF ------------------------------------------------------------------ */
