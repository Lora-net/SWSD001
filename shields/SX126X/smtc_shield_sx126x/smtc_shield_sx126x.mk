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

define SMTC_SHIELD_SX126X_SUPPORTED_BODY
Shield $(SMTC_SHIELD_SX126X) is not supported.

The supported shields are:
  * SX1261: SX1261MB1BAS / SX1261MB1CAS / SX1261MB2BAS
  * SX1262: SX1262MB1CAS / SX1262MB1CBS / SX1262MB1DAS / SX1262MB1PAS / SX1262MB2CAS
  * SX1268: SX1268MB1GAS
endef

C_INCLUDES += \
-I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/common/inc \

C_SOURCES += \
$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/common/src/smtc_shield_sx126x_ant_sw.c \
$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/common/src/smtc_shield_sx126x_led.c \

ifeq ($(SMTC_SHIELD_SX126X), SX1261MB1BAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1261MB1BAS/src/smtc_shield_sx1261mb1bas.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1261MB1BAS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1261MB1CAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1261MB1CAS/src/smtc_shield_sx1261mb1cas.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1261MB1CAS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1261MB2BAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1261MB2BAS/src/smtc_shield_sx1261mb2bas.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1261MB2BAS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1262MB1CAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1CAS/src/smtc_shield_sx1262mb1cas.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1CAS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1262MB1CBS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1CBS/src/smtc_shield_sx1262mb1cbs.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1CBS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1262MB1DAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1DAS/src/smtc_shield_sx1262mb1das.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1DAS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1262MB1PAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1PAS/src/smtc_shield_sx1262mb1pas.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB1PAS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1262MB2CAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB2CAS/src/smtc_shield_sx1262mb2cas.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1262MB2CAS/inc

else ifeq ($(SMTC_SHIELD_SX126X), SX1268MB1GAS)
C_SOURCES += $(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1268MB1GAS/src/smtc_shield_sx1268mb1gas.c
C_INCLUDES += -I$(SMTC_SHIELD_SX126X_DIR)/smtc_shield_sx126x/SX1268MB1GAS/inc

else
$(error $(SMTC_SHIELD_SX126X_SUPPORTED_BODY))
endif
