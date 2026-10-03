#include <lk/debug.h>
#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>
#include <adi_hal_internal.h>
#include <sprd_regulator.h>
#include <sprd_boardid.h>

static int power_on_voltage_init(void)
{
	regulator_set_voltage("vddrf1v25",1250);

	/*check dcdc_cpu1 spuuly mode */
	if(sprd_get_power_mode() != 1) { /*cpu1 power supply by pmic */
		regulator_set_voltage("vddgpu",1000);
	}

	return 0;
}

int regulator_init(void)
{
	power_on_voltage_init();
	return 0;
}
