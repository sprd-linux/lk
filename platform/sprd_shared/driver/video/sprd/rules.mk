LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS += \
	$(LOCAL_DIR)/sprd_main.c \
	$(LOCAL_DIR)/sprd_dispc.c \
	$(LOCAL_DIR)/sprd_dsi.c \
	$(LOCAL_DIR)/sprd_dphy.c \
	$(LOCAL_DIR)/sprd_panel.c \
	$(LOCAL_DIR)/dphy/mipi_dphy_api.c \
	$(LOCAL_DIR)/dsi/mipi_dsi_api.c \
	$(LOCAL_DIR)/dsi/mipi_dsi_hal.c \
	$(LOCAL_DIR)/lib/adf_format.c \
	$(LOCAL_DIR)/lcd/lcd_dummy_mipi_hd.c \
	$(LOCAL_DIR)/$(PLATFORM)/global_dispc.c \
	$(LOCAL_DIR)/$(PLATFORM)/global_dsi.c \
	$(LOCAL_DIR)/$(PLATFORM)/global_dphy.c

ifeq ($(LCD_NT35596_BOE_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_NT35596_BOE_MIPI_FHD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nt35596_boe_mipi_fhd.c
endif

ifeq ($(LCD_NT35532_TRULY_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_NT35532_TRULY_MIPI_FHD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nt35532_truly_mipi_fhd.c
endif

ifeq ($(LCD_NT35695_TRULY_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_NT35695_TRULY_MIPI_FHD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nt35695_truly_mipi_fhd.c
endif

ifeq ($(LCD_ICNL9911C_TRULY_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_ICNL9911C_TRULY_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_icnl9911c_truly_mipi_hdp.c
endif

ifeq ($(LCD_ILI9881C_TRULY_MIPI_HD),1)
GLOBAL_DEFINES += CONFIG_LCD_ILI9881C_TRULY_MIPI_HD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_ili9881c_truly_mipi_hd.c
endif

ifeq ($(LCD_TD4310_TRULY_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_TD4310_TRULY_MIPI_FHD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_td4310_truly_mipi_fhd.c
endif

ifeq ($(LCD_SSD2092_TRULY_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_SSD2092_TRULY_MIPI_FHD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_ssd2092_truly_mipi_fhd.c
endif

ifeq ($(LCD_HX83102E_XY_HSD_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_HX83102E_XY_HSD_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_hx83102e_xy_hsd_mipi_hdp.c
endif

ifeq ($(LCD_G40396_TRULY_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_G40396_TRULY_MIPI_FHD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_g40396_truly_mipi_fhd.c
endif

ifeq ($(LCD_GC7202H_GENRPRO_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_GC7202H_GENRPRO_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_gc7202h_genrpro_mipi_hdp.c
endif

ifeq ($(LCD_JD9365T_CPT_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_JD9365T_CPT_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_jd9365t_cpt_mipi_hdp.c
endif

ifeq ($(LCD_JD9365T_HLT_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_JD9365T_HLT_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_jd9365t_hlt_mipi_hdp.c
endif

ifeq ($(LCD_NL9911C_TRULY_6MASK_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_NL9911C_TRULY_6MASK_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nl9911c_truly_6mask_mipi_hdp.c
endif

ifeq ($(LCD_NL9911C_TRULY_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_NL9911C_TRULY_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nl9911c_truly_mipi_hdp.c
endif

ifeq ($(LCD_NT36525B_TXD_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_NT36525B_TXD_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nt36525b_txd_mipi_hdp.c
endif

ifeq ($(LCD_NT36525B_TM_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_NT36525B_TM_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nt36525b_tm_mipi_hdp.c
endif

ifeq ($(LCD_NT36525B_HX_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_NT36525B_HX_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nt36525b_hx_mipi_hdp.c
endif

ifeq ($(LCD_NT36672E_TRULY_MIPI_FHD_NODSC),1)
GLOBAL_DEFINES += CONFIG_LCD_NT36672E_TRULY_MIPI_FHD_NODSC
MODULE_SRCS += \
       $(LOCAL_DIR)/lcd/lcd_nt36672e_truly_mipi_fhd_nodsc.c
endif

ifeq ($(LCD_NT36672E_TRULY_MIPI_FHD_BP),1)
GLOBAL_DEFINES += CONFIG_LCD_NT36672E_TRULY_MIPI_FHD_BP
MODULE_SRCS += \
       $(LOCAL_DIR)/lcd/lcd_nt36672e_truly_mipi_fhd_bp.c
endif

ifeq ($(LCD_ILI9881C_3LANE_MIPI_HDmi),1)
GLOBAL_DEFINES += CONFIG_LCD_ILI9881C_3LANE_MIPI_HD
MODULE_SRCS += \
       $(LOCAL_DIR)/lcd/lcd_ili9881c_3lane_mipi_hd.c
endif

ifeq ($(LCD_NT36672E_TRULY_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_NT36672E_TRULY_MIPI_FHD
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_nt36672e_truly_mipi_fhd.c
endif

ifeq ($(LCD_R61350_TRULY_MIPI_HD),1)
GLOBAL_DEFINES += CONFIG_LCD_R61350_TRULY_MIPI_HD
MODULE_SRCS += \
       $(LOCAL_DIR)/lcd/lcd_r61350_truly_mipi_hd.c
endif

ifeq ($(LCD_R61350_TRULY_MIPI_HD_V2),1)
GLOBAL_DEFINES += CONFIG_LCD_R61350_TRULY_MIPI_HD_V2
MODULE_SRCS += \
       $(LOCAL_DIR)/lcd/lcd_r61350_truly_mipi_hd_v2.c
endif

ifeq ($(LCD_TD4320_TRULY_MIPI_FHD),1)
GLOBAL_DEFINES += CONFIG_LCD_TD4320_TRULY_MIPI_FHD
MODULE_SRCS += \
        $(LOCAL_DIR)/lcd/lcd_td4320_truly_mipi_fhd.c
endif

ifeq ($(PLATFORM), qogirl6)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkl5.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0_ppi.c \
    $(LOCAL_DIR)/dispc/dpu_r5p0.c
else ifeq ($(PLATFORM), qogirn6pro)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkl5.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0_ppi.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0.c \
    $(LOCAL_DIR)/dispc/dpu_r6p0.c
else ifeq ($(PLATFORM), qogirn6l)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkl5.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p1_ppi.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p1.c \
    $(LOCAL_DIR)/dispc/dpu_r6p1.c
else ifeq ($(PLATFORM), sharkl3)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkle.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0_ppi.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0.c \
    $(LOCAL_DIR)/dispc/dpu_r2p0.c
else ifeq ($(PLATFORM), pike2)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkle.c \
    $(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0_ppi.c \
    $(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0.c \
    $(LOCAL_DIR)/dispc/dpu_lite_r1p0.c
else ifeq ($(PLATFORM), sharkle)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkle.c \
    $(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0_ppi.c \
    $(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0.c \
    $(LOCAL_DIR)/dispc/dpu_lite_r1p0.c
else ifeq ($(PLATFORM), sharkl5)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkl5.c \
    $(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0_ppi.c \
    $(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0.c \
    $(LOCAL_DIR)/dispc/dpu_lite_r2p0.c
else ifeq ($(PLATFORM), sharkl5pro)
MODULE_SRCS += \
    $(LOCAL_DIR)/dphy/pll/megacores_sharkl5.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0_ppi.c \
	$(LOCAL_DIR)/dsi/core/mipi_dsi_r1p0.c \
    $(LOCAL_DIR)/dispc/dpu_r4p0.c
endif

ifeq ($(LCD_TD4160_BOE_MIPI_HDP),1)
GLOBAL_DEFINES += CONFIG_LCD_TD4160_BOE_MIPI_HDP
MODULE_SRCS += \
	$(LOCAL_DIR)/lcd/lcd_td4160_boe_mipi_hdp.c
endif

include make/module.mk
