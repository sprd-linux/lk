#ifndef __ARCH_ARM_ASM_SHARKL5_I2C__
#define __ARCH_ARM_ASM_SHARKL5_I2C__

#include <asm/arch/sprd_reg.h>
#include <asm/arch/common.h>
#include <i2c.h>

#define SPRD_I2C_NUM		6

struct sprd_i2c I2C_BUS[SPRD_I2C_NUM] = {
	{	.bus_name = "i2c0",
		.apb_base =  (void *)REG_AP_APB_APB_EB,
		.apb_eb = BIT_AP_APB_I2C0_EB,
		.base = (void *)SPRD_I2C0_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	}, {
		.bus_name = "i2c1",
		.apb_base =  (void *)REG_AP_APB_APB_EB,
		.apb_eb = BIT_AP_APB_I2C1_EB,
		.base = (void *)SPRD_I2C1_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000
	}, {
		.bus_name = "i2c2",
		.apb_base = (void *) REG_AP_APB_APB_EB,
		.apb_eb = BIT_AP_APB_I2C2_EB,
		.base = (void *)SPRD_I2C2_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000
	}, {
		.bus_name = "i2c3",
		.apb_base =  (void *)REG_AP_APB_APB_EB,
		.apb_eb = BIT_AP_APB_I2C3_EB,
		.base = (void *)SPRD_I2C3_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000
	}, {
		.bus_name = "i2c4",
		.apb_base =  (void *)REG_AP_APB_APB_EB,
		.apb_eb = BIT_AP_APB_I2C4_EB,
		.base = (void *)SPRD_I2C4_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000
	} , {
		.bus_name = "i2c5",
		.apb_base =  (void *)REG_AON_APB_APB_EB2,
		.apb_eb = BIT_AON_APB_I2C_EB,
		.base = (void *)SPRD_I2C_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000
	},
};

#endif /*__ARCH_ARM_ASM_SHARKL5_I2C__*/
