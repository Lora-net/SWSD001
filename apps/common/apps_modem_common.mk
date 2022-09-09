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

CRYPTO ?= LR11XX
MIDDLEWARE ?= no

RADIO_BOARD ?= LR1110MB1DIS

ifeq ($(RADIO_BOARD), LR1110MB1DIS)
$(warning This is the default value for RADIO_BOARD - please make sure it is adapted to your target)
endif

ifeq ($(CRYPTO),SOFT)
C_DEFS += \
    -DUSER_DEFINED_JOIN_PARAMETERS
else ifeq ($(CRYPTO),LR11XX)
C_DEFS += \
    -DUSER_DEFINED_JOIN_PARAMETERS
else ifeq ($(CRYPTO),LR11XX_WITH_CREDENTIALS)
C_DEFS += \
    -DLR11XX_DEFINED_JOIN_PARAMETERS
else
    $(error Invalid crypto option, options are: SOFT | LR11XX | LR11XX_WITH_CREDENTIALS )
endif

RP_VERSION ?= RP2_103

SMTC_HAL_MAKEFILE = $(TOP_DIR)/smtc_hal/smtc_hal_target.mk
LBM_MAKEFILE = $(TOP_DIR)/lora_basics_modem/makefiles/common.mk
PRINT_MAKEFILE = $(TOP_DIR)/lora_basics_modem/makefiles/printing.mk

C_SOURCES +=  \
$(TOP_DIR)/apps/common/apps_utilities.c \
$(TOP_DIR)/apps/common/apps_modem_common.c \
$(TOP_DIR)/apps/common/apps_modem_event.c

include $(PRINT_MAKEFILE)
include $(LBM_MAKEFILE)
include $(SMTC_HAL_MAKEFILE)

C_INCLUDES +=  \
-I$(TOP_DIR)/apps/common

LBM_LIB = $(LORA_BASICS_MODEM)/build/basic_modem.a

.PHONY: $(LBM_LIB)
$(LBM_LIB): build_basic_modem
