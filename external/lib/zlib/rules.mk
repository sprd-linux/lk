LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS := \
		$(LOCAL_DIR)/decompress_data.c \
		$(LOCAL_DIR)/adler32.c \
		$(LOCAL_DIR)/infback.c \
		$(LOCAL_DIR)/inffast.c \
		$(LOCAL_DIR)/inflate.c \
		$(LOCAL_DIR)/inftrees.c \
		$(LOCAL_DIR)/uncompr.c \
		$(LOCAL_DIR)/zutil.c

GLOBAL_INCLUDES += $(LOCAL_DIR)/include/

#$(warning lib png rule.mk .... include $(GLOBAL_INCLUDES))

MODULE_DEPS := \
	lib/libc


include make/module.mk
