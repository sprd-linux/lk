LOCAL_PATH := $(my-dir)
HLOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

include $(HLOCAL_DIR)/libhvb/rules.mk

MODULE_SRCS += \
        $(HLOCAL_DIR)/hvb_check.c

#include make/module.mk
#LOCAL_SHARED_LIBRARIES += libopen \
			  libioctl \
			  libclose
