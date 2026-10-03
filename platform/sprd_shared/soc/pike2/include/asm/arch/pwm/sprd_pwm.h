#ifndef __ARCH_ARM_ASM_PWM__
#define __ARCH_ARM_ASM_PWM__

#include <asm/arch/sprd_reg.h>
#include <asm/arch/common.h>
#include <pwm.h>

#define SPRD_PWM_NUM		4

struct sprd_pwm{
	unsigned pwm_eb;
	void * apb_base_eb;
	void * pwm_clk_base;
	void * pwm_base;
};

enum{
	EXT_32K = 0,
	RPLL_26M,
	EXT_26M,
	TWPLL_48M,
};

struct sprd_pwm sprd_pwm[SPRD_PWM_NUM] = {
	{
		.pwm_eb = BIT_AON_APB_PWM0_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB0,
		.pwm_clk_base = (void *)REG_AON_CLK_PWM0_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	}, {
		.pwm_eb = BIT_AON_APB_PWM1_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB0,
		.pwm_clk_base = (void *)REG_AON_CLK_PWM1_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	}, {
		.pwm_eb = BIT_AON_APB_PWM2_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB0,
		.pwm_clk_base = (void *)REG_AON_CLK_PWM2_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	}, {
		.pwm_eb = BIT_AON_APB_PWM2_EB,
		.apb_base_eb = (void *)REG_AON_APB_APB_EB0,
		.pwm_clk_base = (void *)REG_AON_CLK_PWM3_CFG,
		.pwm_base = (void *)CTL_BASE_PWM,
	},
};

#endif /*__ARCH_ARM_ASM_PWM__*/
