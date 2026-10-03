#ifndef _SDIO_REG_V0_H_
#define _SDIO_REG_V0_H_

#define EMMC	0
#define SD	1

struct sdio_base_info {
	uint32_t  dev_type;
	uint32_t  regbase;
	uint32_t  baseclk_reg;
	uint32_t  baseclk_mask;
	uint32_t  maxclk;
	uint32_t  minclk;
	uint32_t  ahb_enable_reg;
	uint32_t  ahb_enable_bit;
	uint32_t  ahb_reset_reg;
	uint32_t  ahb_reset_bit;
	uint32_t aon_clk_reg;
	uint32_t aon_clk_bit;
	const char *ldo_io;
	const char *ldo_core;
	uint32_t hs_dly;
	uint32_t hs200_dly;
};

struct sdio_base_info *get_sdcontrol_info(uint32_t device_type);
#endif
