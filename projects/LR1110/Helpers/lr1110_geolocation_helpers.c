/*!
 * @ingroup   apps_geolocation
 * @file      lr1110_geolocation_helpers.c
 *
 * @brief     LoRa Basics Modem LR1110 Geolocation helpers
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

#include <stddef.h>

#include "smtc_hal_dbg_trace.h"

#include "lr1110_types.h"
#include "lr1110_system.h"

#include "lr1110_geolocation_helpers.h"

bool lr1110_configure_for_scan( const void* ral_context )
{
    lr1110_status_t status;

    status = lr1110_system_cfg_lfclk( ral_context, LR1110_SYSTEM_LFCLK_XTAL, true );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Fail to config lfclk\n" );
        return false;
    }

    /* TODO: get errors before clearing them ? */
    status = lr1110_system_clear_errors( ral_context );
    if( status != LR1110_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Fail to clear error\n" );
        return false;
    }

    return true;
}