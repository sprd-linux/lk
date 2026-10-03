TOOL_CHAIN_ROOT=$(BSP_ROOT_DIR)/toolchain

ifeq ($(BSP_BOARD_ARCH) , arm64)
LK_ARCH := arm64
ifeq ($(BSP_LK_CLANG_COMPILE), true)
LK_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/clang/host/linux-x86/$(BSP_CLANG_VERSION)/bin/
LK_APP_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/clang/host/linux-x86/$(BSP_CLANG_VERSION)/bin/
else
LK_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/gcc/linaro-x86/aarch64/gcc-linaro-4.8/gcc-linaro-4.8-2015.06-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
LK_APP_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
endif

else ifeq ($(BSP_BOARD_ARCH), arm)
LK_ARCH := arm
ifeq ($(BSP_LK_CLANG_COMPILE), true)
LK_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/clang/host/linux-x86/$(BSP_CLANG_VERSION)/bin/
LK_APP_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/clang/host/linux-x86/$(BSP_CLANG_VERSION)/bin/
else
LK_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
LK_APP_CROSS_COMPILE := $(TOOL_CHAIN_ROOT)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
endif

else ifeq ($(BSP_BOARD_ARCH),)
$(error $(BSP_BOARD_ARCH) is not null)
endif

LK_BUILD_OPTION := \
-f makefile \
TOOLCHAIN_PREFIX=$(LK_CROSS_COMPILE) \
ARCH_$(LK_ARCH)_TOOLCHAIN_PREFIX=$(LK_CROSS_COMPILE) \
BUILDROOT=$(BSP_LK_OUT) \
DEFAULT_PROJECT=$(BSP_LK_DEFCONFIG)

#LK_HEAP_IMPLEMENTATION=miniheap

all:
	@mkdir -p $(BSP_LK_DIST)
	$(MAKE)  AUTOBOOT_FLAG=true $(LK_BUILD_OPTION)
	@mkdir -p $(BSP_LK_OUT)/.out
	@cp $(BSP_LK_OUT)/build-$(BSP_LK_DEFCONFIG)/lk.bin $(BSP_LK_OUT)/.out/lk_autopoweron.bin
	@cp $(BSP_LK_OUT)/build-$(BSP_LK_DEFCONFIG)/lk.elf $(BSP_LK_OUT)/.out/lk_autopoweron.elf
	@cp $(BSP_LK_OUT)/build-$(BSP_LK_DEFCONFIG)/lk.elf.map $(BSP_LK_OUT)/.out/lk.elf_autopoweron.map
	@cp $(BSP_LK_OUT)/build-$(BSP_LK_DEFCONFIG)/config.h $(BSP_LK_OUT)/.out/config_autopoweron.h
	$(MAKE)  $(LK_BUILD_OPTION) clean
	$(MAKE)  $(LK_BUILD_OPTION)
	@mv $(BSP_LK_OUT)/.out/* $(BSP_LK_OUT)/build-$(BSP_LK_DEFCONFIG)
	@rm -rf $(BSP_LK_OUT)/.out
