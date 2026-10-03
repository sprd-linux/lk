#ifndef __ARCH_ARM_ASM_PWM__
#define __ARCH_ARM_ASM_PWM__

#include <asm/arch/sprd_reg.h>
#include <asm/arch/common.h>
#include <pwm.h>

#define SPRD_PWM_VERSION_R3P1
#define SPRD_PWM_NUM		4

struct sprd_pwm{
	unsigned pwm_eb;
	void * apb_base_eb;
	void * pwm_clk_base;
	void * pwm_base;
};

enum{
	EXT_32K = 0,
	EXT_26M,
	RCO_4M,
	RCO_25M,
	TWPLL_48M,
};

struct sprd_pwm sprd_pwm[SPRD_PWM_NUM] = {
	{
		.pwm_eb = BIT_AON_APB_AON_PWM0_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB2,
		.pwm_clk_base = (void *)REG_AON_CLK_CGM_PWM0_SEL_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	}, {
		.pwm_eb = BIT_AON_APB_AON_PWM1_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB2,
		.pwm_clk_base = (void *)REG_AON_CLK_CGM_PWM1_SEL_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	}, {
		.pwm_eb = BIT_AON_APB_AON_PWM2_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB2,
		.pwm_clk_base = (void *)REG_AON_CLK_CGM_PWM2_SEL_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	},{
		.pwm_eb = BIT_AON_APB_AON_PWM3_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB2,
		.pwm_clk_base = (void *)REG_AON_CLK_CGM_PWM3_SEL_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	},
};

#endif /*__ARCH_ARM_ASM_PWM__*/
