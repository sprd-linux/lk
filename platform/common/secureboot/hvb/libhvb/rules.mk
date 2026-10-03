HVB_DIR := $(GET_LOCAL_DIR)

MODULE_SRCS += \
        $(HVB_DIR)/src/auth/hvb.c \
        $(HVB_DIR)/src/cert/hvb_cert.c \
        $(HVB_DIR)/src/crypto/hvb_hash_sha256.c \
        $(HVB_DIR)/src/crypto/hvb_rsa_verify.c \
        $(HVB_DIR)/src/crypto/hvb_rsa.c \
        $(HVB_DIR)/src/deps/hvb_sysdeps.c \
        $(HVB_DIR)/src/footer/hvb_footer.c \
        $(HVB_DIR)/src/rvt/hvb_rvt.c \
        $(HVB_DIR)/src/utils/hvb_util.c \
        $(HVB_DIR)/src/cmdline/hvb_cmdline.c
