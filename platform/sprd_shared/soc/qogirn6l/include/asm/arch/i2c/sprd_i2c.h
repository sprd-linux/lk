#ifndef __ARCH_ARM_ASM_QOGIRN6L_I2C__
#define __ARCH_ARM_ASM_QOGIRN6L_I2C__

#include <asm/arch/sprd_reg.h>
#include <sprd_common.h>
#include <i2c.h>

#define SPRD_I2C_NUM		10

struct sprd_i2c I2C_BUS[SPRD_I2C_NUM] = {
	{
		.bus_name = "i2c0",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C0_EB,
		.base = (void *)SPRD_I2C0_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	}, {
		.bus_name = "i2c1",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C1_EB,
		.base = (void *)SPRD_I2C1_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	}, {
		.bus_name = "i2c2",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C2_EB,
		.base = (void *)SPRD_I2C2_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	}, {
		.bus_name = "i2c3",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C3_EB,
		.base = (void *)SPRD_I2C3_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	}, {
		.bus_name = "i2c4",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C4_EB,
		.base = (void *)SPRD_I2C4_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	} , {
		.bus_name = "i2c5",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C5_EB,
		.base = (void *)SPRD_I2C5_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	} , {
		.bus_name = "i2c6",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C6_EB,
		.base = (void *)SPRD_I2C6_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	} , {
		.bus_name = "i2c7",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C7_EB,
		.base = (void *)SPRD_I2C7_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	} , {
		.bus_name = "i2c8",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C8_EB,
		.base = (void *)SPRD_I2C8_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	} , {
		.bus_name = "i2c9",
		.apb_base =  (void *)REG_AP_AHB_AHB_EB,
		.apb_eb = BIT_AP_APB_I2C9_EB,
		.base = (void *)SPRD_I2C9_PHYS,
		.freq = 100*1000,
		.src_clk = 26000000,
	} ,
};

#endif /* __ARCH_ARM_ASM_QOGIRN6L_I2C__ */
