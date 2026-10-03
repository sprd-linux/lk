LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
	platform/common \
	lib/fs \
	$(LOCAL_DIR)/download

MODULE_SRCS += \
	$(LOCAL_DIR)/sprdboot.c \
	$(LOCAL_DIR)/sprd_cpcmdline.c \
	$(LOCAL_DIR)/process_cp_coupling_info.c \
	$(LOCAL_DIR)/boot_parse.c \
	$(LOCAL_DIR)/sprdisk.c \
	$(LOCAL_DIR)/calibration_detect.c \
	$(LOCAL_DIR)/boot_mode.c \
	$(LOCAL_DIR)/fastboot.c \
	$(LOCAL_DIR)/recv_mode.c \
	$(LOCAL_DIR)/download.c \
	$(LOCAL_DIR)/sysdump.c \
	$(LOCAL_DIR)/minidump.c \
	$(LOCAL_DIR)/autodloader_mode.c \
	$(LOCAL_DIR)/alarm_mode.c \
	$(LOCAL_DIR)/read_miscdata_boot_flag.c

ifeq ($(WITH_SMP), 1)
MODULE_SRCS += \
	$(LOCAL_DIR)/boot_os_mp.c
else
MODULE_SRCS += \
	$(LOCAL_DIR)/boot_os.c
endif

# MODULE_CFLAGS += -finstrument-functions

include make/module.mk
