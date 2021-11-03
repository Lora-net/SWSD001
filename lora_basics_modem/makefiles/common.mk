#-----------------------------------------------------------------------------
# Indicates library location
#-----------------------------------------------------------------------------

C_INCLUDES +=  \
-I$(LORA_BASICS_MODEM)/smtc_modem_api/ \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/modem_config/ \
-I$(LORA_BASICS_MODEM)/smtc_modem_hal/ \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/device_management/ \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/modem_core/ \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/lr1mac/src/ \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/smtc_modem_services/headers/ \
-I$(LORA_BASICS_MODEM)/smtc_modem_core/radio_planner/src/ \


#-----------------------------------------------------------------------------
# Buid targets
#-----------------------------------------------------------------------------
.PHONY: build_basic_modem

build_basic_modem:
	$(MAKE) -C $(LORA_BASICS_MODEM) basic_modem $(MTHREAD_FLAG) RADIO=$(RADIO) MODEM_TRACE=$(MODEM_TRACE) CRYPTO=$(CRYPTO)

#-----------------------------------------------------------------------------
# Clean targets
#-----------------------------------------------------------------------------

.PHONY: clean_lbm
clean_lbm:
	$(MAKE) -C $(LORA_BASICS_MODEM) $(MTHREAD_FLAG) clean_all
