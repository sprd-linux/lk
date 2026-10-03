/*
 * Copyright (c) 2021 unisoc Inc.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
#include <lk/debug.h>
#include <target.h>
#include <adi_hal_internal.h>
#include <storage_init.h>
#include <asm/arch/pinmap.h>
#include <eic.h>
#include <i2c.h>
#include <gpio.h>
#include <serial_sprd.h>
#include <sprd_adc.h>
#include <sprd_glb.h>
#include <sprd_led.h>
#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>
#include <sprd_battery.h>

#ifdef CONFIG_SENSOR_HUB_LK
#include <sprd_sensor.h>
#include "sensor_board_info.h"
#endif
#ifdef SPRD_TRACE
#include <sprd_trace.h>
#endif

extern void board_keypad_init(void);
extern void sprd_pmu_lowpower_init(void);
extern int regulator_init(void);
extern int boardid_init(void);
extern void misc_init(void);

#define udelay(x) \
	do { \
		volatile int i; \
		int cnt = 200 * (x); \
		for (i=0; i<cnt; i++);\
	} while(0);

void enable_global_clocks(void)
{
	u32 val;

	sci_glb_set(REG_AON_APB_APB_EB0, BIT_AON_APB_GPU_EB |
		BIT_AON_APB_MM_EB);
	sci_glb_clr(REG_PMU_APB_PD_GPU_TOP_CFG, BIT_PMU_APB_PD_GPU_TOP_FORCE_SHUTDOWN);
	sci_glb_clr(REG_PMU_APB_PD_MM_TOP_CFG, BIT_PMU_APB_PD_MM_TOP_FORCE_SHUTDOWN);
	val = readl(REG_PMU_APB_PWR_STATUS3_DBG);
	udelay(1000);
	while (readl(REG_PMU_APB_PWR_STATUS3_DBG) & BIT_PMU_APB_PD_MM_TOP_STATE(~0));
	sci_glb_set(REG_MM_AHB_AHB_EB, BIT_MM_AHB_CKG_EB);
}

static void battery_init(void)
{
	sprdchg_common_cfg();
	sprdchg_fan54015_init();
	sprdbat_init();
	dprintf(INFO,"CHG init OK!\n");
}

void target_early_init(void) {
	sprd_serial_init();
#ifdef SPRD_TRACE
	trace_init();
#endif
}

void target_init(void) {
#ifndef CONFIG_FPGA
	boardid_init();
	pin_init();
	ADI_init();
	sprd_i2c_init();
	/*FPGA forbiden*/
	regulator_init();
	pmic_adc_Init();
	sprd_gpio_init();
	/*FPGA forbiden*/
	misc_init();
	sprd_eic_init();
	sprd_led_init();
	/*FPGA forbiden*/
	sprd_pmu_lowpower_init();
	enable_global_clocks();
	board_keypad_init();
#endif /* endof CONFIG_FPGA */
	board_mmc_initialize();
	battery_init();

}

#ifdef CONFIG_SENSOR_HUB_LK
int sprd_sensor_bandlist(struct sensor_phypara **sensorlist)
{
	*sensorlist = sensor_board_phylist;
	return sizeof(sensor_board_phylist)/sizeof(sensor_board_phylist[0]);
}
#endif

