/*!
 * @ingroup   apps_geolocation
 * @file      main_geolocation_gnss.h
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation GNSS example with *scan group" for static geolocation.
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

/*!
 * @addtogroup apps_geolocation
 * LoRa Basics Modem LR1110 Geolocation example
 * @{
 */

#ifndef MAIN_GEOLOCATION_GNSS_H
#define MAIN_GEOLOCATION_GNSS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- Application Configuration -----------------------------------------------
 */

/*!
 * @brief Time during which a LED is turned on when a TX or RX event occurs, in [ms]
 */
#define LED_PERIOD_MS 250

/*!
 * @brief If true, then the system will not power down all peripherals
 *         when going to low power mode. This is necessary to keep the LEDs
 *         active in low power mode.
 */
#define APP_PARTIAL_SLEEP true

/**
 * @brief Assistance location for GNSS geolocation
 */
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_LAT ( 45.181454 )
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_LONG ( 5.720893 )
#define MODEM_EXAMPLE_ASSISTANCE_POSITION_TEXT "Grenoble, FRANCE"

/*
 * -----------------------------------------------------------------------------
 * --- Geolocation scan group Configuration ------------------------------------
 */

/*!
 * @brief Defines the delay before starting a new GNSS scan group, value in [s].
 */
#define GNSS_SCAN_GROUP_PERIOD 30

/**
 * @brief Defines the duration between two consecutive GNSS scans of the same scan group, value in [s].
 */
#define GNSS_SCAN_GROUP_DELAY 15

/**
 * @brief Number of GNSS scan in a scan group
 */
#define GNSS_SCAN_GROUP_SIZE 4

/*
 * -----------------------------------------------------------------------------
 * --- LoRaWAN Configuration ---------------------------------------------------
 */

/*!
 * @brief LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON false

/*!
 * @brief ADR custom list and retransmission parameters for EU868 region
 */
uint8_t adr_custom_list_eu868[16] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3 }; /* 125kHz - SF7, SF8, SF9 */
uint8_t custom_nb_trans_eu868     = 1;

/*!
 * @brief ADR custom list and retransmission parameters for CN470 region
 */
uint8_t adr_custom_list_cn470[16] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3 }; /* 125kHz - SF7, SF8, SF9 */
uint8_t custom_nb_trans_cn470     = 2;

/*!
 * @brief ADR custom list and retransmission parameters for US915 region
 */
uint8_t adr_custom_list_us915[16] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1 }; /* 125kHz - SF7, SF8, SF9 */
uint8_t custom_nb_trans_us915     = 2;

#ifdef __cplusplus
}
#endif

#endif  // MAIN_GEOLOCATION_GNSS_H

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
