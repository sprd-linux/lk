LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS := \
		$(LOCAL_DIR)/gunzip.c

MODULE_DEPS := \
	lib/libc

include make/module.mk
