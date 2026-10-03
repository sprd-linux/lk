LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

GLOBAL_INCLUDES += arch/arm64/include/arch/ \
                   $(LOCAL_DIR)/include

MODULE_SRCS += \
        $(LOCAL_DIR)/sprd_crypto_special.c \
        $(LOCAL_DIR)/sec_memcpy_invert.c

ifneq (,$(filter $(PLATFORM), pike2))
GLOBAL_DEFINES += CONFIG_SOC_PIKE2
MODULE_INCLUDES += $(LOCAL_DIR)/r2p0lite/inc
include $(LOCAL_DIR)/r2p0lite/rules.mk
endif

ifneq (,$(filter $(PLATFORM), sharkle))
GLOBAL_DEFINES += CONFIG_SOC_SHARKLE
MODULE_INCLUDES += $(LOCAL_DIR)/r2p0lite/inc
include $(LOCAL_DIR)/r2p0lite/rules.mk
endif

ifneq (,$(filter $(PLATFORM), sharkl3))
GLOBAL_DEFINES += CONFIG_SOC_SHARKL3
MODULE_INCLUDES += $(LOCAL_DIR)/r2p0lite/inc
include $(LOCAL_DIR)/r2p0lite/rules.mk
endif

ifneq (,$(filter $(PLATFORM), sharkl5))
GLOBAL_DEFINES += CONFIG_SOC_SHARKL5
MODULE_INCLUDES += $(LOCAL_DIR)/r2p0lite/inc
include $(LOCAL_DIR)/r2p0lite/rules.mk
endif

ifneq (,$(filter $(PLATFORM), sharkl5pro))
GLOBAL_DEFINES += CONFIG_SOC_SHARKL5PRO
MODULE_INCLUDES += $(LOCAL_DIR)/r2p0lite/inc
include $(LOCAL_DIR)/r2p0lite/rules.mk
endif

ifneq (,$(filter $(PLATFORM), qogirl6 qogirn6pro qogirn6l))
MODULE_INCLUDES += $(LOCAL_DIR)/r5p0lite/inc
include $(LOCAL_DIR)/r5p0lite/rules.mk
endif

include make/module.mk
