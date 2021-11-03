/*!
 * @ingroup   apps_geolocation
 * @file      wifi_helpers.h
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation example, WiFi scan helpers
 *
 * @copyright
 * @parblock
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
 * @endparblock

 */

#ifndef __WIFI_HELPERS_H__
#define __WIFI_HELPERS_H__

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

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Execute the initialisation process of a Wi-Fi scan
 *
 * Handle the call to suspend the stack.
 *
 * @param[in] stack_id The stack identifier
 * @param [in] ral_context Chip implementation context
 */
void wifi_helpers_start_scan( const uint8_t stack_id, const void* ral_context );

/*!
 * @brief Fetch the results obtained during previous Wi-Fi scan
 *
 * Handle the call to suspend the stack.
 *
 * @param [in] ral_context Chip implementation context
 * @param [out] n_results The number of results obtained during previous scan operation
 */
void wifi_helpers_get_results( const void* ral_context, uint8_t* n_results );

/*!
 * @brief Helper function to print results of last Wi-Fi scan operation
 */
void wifi_helpers_display_results( void );

/*!
 * @brief Prepare the buffer to be sent to AS with Wi-Fi results
 *
 * The Wi-Fi scan must be terminated before using this command. It will dump in the provided buffer the Wi-Fi scan
 * results of previous scan, using the compliant format.
 * The maximal buffer size is provided as argument. Is the buffer is not long enough to store all the wi-fi results,
 * then nothing is written to the buffer and buffer_size is set to 0
 *
 * @param [out] buffer The buffer to fill with Wi-Fi results
 * @param [out] buffer_size The number of bytes written to buffer
 * @param [in] max_buffer_size The maximal size of the buffer
 */
void wifi_helpers_prepare_result_payload( uint8_t* buffer, uint8_t* buffer_size, uint8_t max_buffer_size );

/*!
 * @brief Tear down function for Wi-Fi scan termination actions
 *
 * This function is typically to be called when during the handling of the event of user radio access.
 */
void wifi_helpers_end_scan( void );

#ifdef __cplusplus
}
#endif

#endif  // __WIFI_HELPERS_H__

/* --- EOF ------------------------------------------------------------------ */
