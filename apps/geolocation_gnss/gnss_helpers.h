/*!
 * @ingroup   apps_geolocation
 * @file      gnss_helpers.h
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation example, GNSS scan helpers
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

#ifndef __GNSS_HELPERS_H__
#define __GNSS_HELPERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr1110_gnss_types.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define GNSS_TLV_TAG_NAV_MULTIFRAME 0x0F

#define GNSS_RESULT_SIZE_MAX_MODE3 50 /* for 10 satellites max, no doppler, no bit change */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Initialise GNSS scan
 *
 * Handles the calls to suspend/resume the stack.
 *
 * @param [in] ral_context Chip implementation context
 * @param [in] assistance_position Assistance position to use for next GNSS scan
 */
bool smtc_gnss_init( const void* ral_context, const lr1110_gnss_solver_assistance_position_t assistance_position );

/*!
 * @brief Execute full update of the almanac
 *
 * @param [in] ral_context Chip implementation context
 *
 * Handles the calls to suspend/resume the stack.
 */
void smtc_gnss_almanac_update( const void* ral_context );

/*!
 * @brief Execute the start of GNSS scan
 *
 * Handles the calls to suspend/resume the stack.
 *
 * @param[in] stack_id The stack identifier
 * @param [in] ral_context Chip implementation context
 * @param [in] date The date to use for GNSS scan operation
 */
bool smtc_gnss_scan( const uint8_t stack_id, const void* ral_context, uint32_t date );

/*!
 * @brief Execute tear down actions when GNSS scan operation is terminated
 */
void smtc_gnss_scan_ended( void );

/*!
 * @brief Fetch the result of last GNSS scan operation
 *
 * Handles the calls to suspend/resume the stack
 *
 * @param [in] ral_context Chip implementation context
 * @param [out] nb_detected_satellites Number of SVs detected on previous GNSS scan
 * @param [out] res_sz Length of the raw result buffer returned by the radio
 * @param [out] results Pointer to a buffer that will be filled with raw result buffer. The allocation length must be
 * sufficient to receive all data
 * @param [out] update_almanac Boolean indicator that is set if a scan failed due to almanac not updated
 */
bool smtc_gnss_get_results( const void* ral_context, uint8_t* nb_detected_satellites, uint8_t* res_sz, uint8_t* results,
                            bool* update_almanac );

/*!
 * @brief Helper function that prints the result of last GNSS scan
 *
 * @param [in] nb_sats Number of SVs detected on previous GNSS scan
 * @param [in] results The raw result buffer returned on smtc_gnss_get_results call
 * @param [in] results_size The lengh of the raw result buffer
 */
void smtc_gnss_display_results( uint8_t nb_sats, uint8_t* results, uint8_t results_size );

#ifdef __cplusplus
}
#endif

#endif  // __GNSS_HELPERS_H__

/* --- EOF ------------------------------------------------------------------ */