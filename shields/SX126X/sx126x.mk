# --- The Clear BSD License ---
# Copyright Semtech Corporation 2022. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted (subject to the limitations in the disclaimer
# below) provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Semtech corporation nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
# THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
# NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

ifneq (,$(findstring SX1261,$(RADIO_BOARD)))
RADIO = sx1261
else ifneq (,$(findstring SX1262,$(RADIO_BOARD)))
RADIO = sx1262
else ifneq (,$(findstring SX1268,$(RADIO_BOARD)))
RADIO = sx1268
else
$(error Invalid platform board, please select a supported platform board)
endif

SMTC_SHIELD_SX126X_DIR = $(TOP_DIR)/shields/SX126X
SMTC_SHIELD_SX126X = $(RADIO_BOARD)

include $(TOP_DIR)/shields/SX126X/smtc_shield_sx126x/smtc_shield_sx126x.mk

C_SOURCES +=  \
$(TOP_DIR)/shields/SX126X/radio_drivers_hal/sx126x_hal.c \
$(TOP_DIR)/shields/SX126X/common/src/ral_sx126x_bsp.c \
$(TOP_DIR)/shields/SX126X/common/src/smtc_board_sx126x.c \

C_INCLUDES +=  \
-I$(TOP_DIR)/shields/SX126X/radio_drivers_hal \
-I$(TOP_DIR)/shields/SX126X/common/inc \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/radio_drivers/sx126x_driver/src
