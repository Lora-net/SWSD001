# --- The Clear BSD License ---
# Copyright Semtech Corporation 2021. All rights reserved.
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

ifneq (,$(findstring LR1110,$(RADIO_BOARD)))
RADIO = lr1110
else ifneq (,$(findstring LR1120,$(RADIO_BOARD)))
RADIO = lr1120
else ifneq (,$(findstring LR1121,$(RADIO_BOARD)))
RADIO = lr1120 # LR1121 can be used with LoRa Basics Modem v3.2.4 if defined as LR1120
else
$(error Invalid platform board, please select a supported platform board)
endif

SMTC_SHIELD_LR11XX_DIR = $(TOP_DIR)/shields/LR11XX
SMTC_SHIELD_LR11XX = $(RADIO_BOARD)

include $(TOP_DIR)/shields/LR11XX/smtc_shield_lr11xx/smtc_shield_lr11xx.mk

C_SOURCES +=  \
$(TOP_DIR)/shields/LR11XX/radio_drivers_hal/lr11xx_hal.c \
$(TOP_DIR)/shields/LR11XX/common/src/ral_lr11xx_bsp.c \
$(TOP_DIR)/shields/LR11XX/common/src/smtc_board_lr11xx.c \

C_INCLUDES +=  \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/radio_drivers/lr11xx_driver/src \
-I$(TOP_DIR)/shields/LR11XX/common/inc \
-I$(TOP_DIR)/shields/LR11XX/radio_drivers_hal \
-I$(TOP_DIR)/shields/LR11XX/smtc_lr11xx_board \
