//#include <common.h>
//#include <asm/io.h>

#include <linux/kernel.h>
#include <regs_adi.h>
#include "adi_hal_internal.h"
#include <asm/arch/sprd_reg.h>
#include <asm/arch/sprd_eic.h>
#include <sprd_battery.h>
#include "sprd_chg_helper.h"
#include <lk/debug.h>
#include <delay.h>

extern int charger_connected(void);
bool charge_first_power_on;
extern u32 sprd_pmic_efuse_read_bits(int bit_index, int length);

extern void power_down_devices(unsigned pd_cmd);

#define REGS_FGU_BASE			ANA_FGU_BASE
#if defined(CONFIG_ADIE_UMP518)
#define REG_FGU_START	                SCI_ADDR(REGS_FGU_BASE, 0x0000)
#define REG_FGU_CONFIG                  SCI_ADDR(REGS_FGU_BASE, 0x0004)
#define REG_FGU_ADC_CTRL1               SCI_ADDR(REGS_FGU_BASE, 0x000c)
#define REG_FGU_STATUS                  SCI_ADDR(REGS_FGU_BASE, 0x0014)
#define REG_FGU_INT_EN                  SCI_ADDR(REGS_FGU_BASE, 0x0018)
#define REG_FGU_INT_CLR                 SCI_ADDR(REGS_FGU_BASE, 0x001c)
#define REG_FGU_INT_RAW                 SCI_ADDR(REGS_FGU_BASE, 0x0020)
#define REG_FGU_VOLT_VAL                SCI_ADDR(REGS_FGU_BASE, 0x0028)
#define REG_FGU_OCV_VAL                 SCI_ADDR(REGS_FGU_BASE, 0x002c)
#define REG_FGU_POCV_VAL                SCI_ADDR(REGS_FGU_BASE, 0x0030)
#define REG_FGU_CURT_VAL_H              SCI_ADDR(REGS_FGU_BASE, 0x0034)
#define REG_FGU_CURT_VAL_L              SCI_ADDR(REGS_FGU_BASE, 0x0038)
#define REG_FGU_CURT_OFFSET_H           SCI_ADDR(REGS_FGU_BASE, 0x0084)
#define REG_FGU_CURT_OFFSET_L           SCI_ADDR(REGS_FGU_BASE, 0x0088)
#define REG_FGU_USER_AREA_SET           SCI_ADDR(REGS_FGU_BASE, 0x008c)
#define REG_FGU_USER_AREA_CLEAR         SCI_ADDR(REGS_FGU_BASE, 0x0090)
#define REG_FGU_USER_AREA_STATUS        SCI_ADDR(REGS_FGU_BASE, 0x0094)
#else
#define REG_FGU_START                   SCI_ADDR(REGS_FGU_BASE, 0x0000)
#define REG_FGU_CONFIG                  SCI_ADDR(REGS_FGU_BASE, 0x0004)
#define REG_FGU_INT_EN                  SCI_ADDR(REGS_FGU_BASE, 0x0010)
#define REG_FGU_INT_CLR                 SCI_ADDR(REGS_FGU_BASE, 0x0014)
#define REG_FGU_INT_RAW                 SCI_ADDR(REGS_FGU_BASE, 0x0018)
#define REG_FGU_VOLT_VAL                SCI_ADDR(REGS_FGU_BASE, 0x0020)
#define REG_FGU_OCV_VAL                 SCI_ADDR(REGS_FGU_BASE, 0x0024)
#define REG_FGU_POCV_VAL                SCI_ADDR(REGS_FGU_BASE, 0x0028)
#define REG_FGU_CURT_VAL                SCI_ADDR(REGS_FGU_BASE, 0x002c)
#define REG_FGU_CURT_OFFSET             SCI_ADDR(REGS_FGU_BASE, 0x0090)
#define REG_FGU_USER_AREA_SET           SCI_ADDR(REGS_FGU_BASE, 0x00A0)
#define REG_FGU_USER_AREA_CLEAR         SCI_ADDR(REGS_FGU_BASE, 0x00A4)
#define REG_FGU_USER_AREA_STATUS        SCI_ADDR(REGS_FGU_BASE, 0x00A8)
#endif

#define BIT_FGU_DISABLE_EN		( BIT(11) )
#define BIT_CLK_SEL_FGU                 ( BIT(9) )
#define BIT_VOL_READY_INT               ( BIT(6) )
#define BIT_FGU_RESET                   ( BIT(1) )
#define BIT_WRITE_ACTIVE_STS		( BIT(0) )
#define BITS_CLK_SEL_512K		0x3E0

#define BITS_POWERON_TYPE_SHIFT		12
#define BITS_POWERON_TYPE(_x_)          ( (_x_) << 12 & (0xF000))
#define BITS_RTC_AREA_SHIFT		0
#define BITS_RTC_AREA(_x_)		( (_x_) << 0 & (0xFFF) )
#define BITSINDEX(b, o, w)		((b) * (w) + (o))

#define FIRST_POWERTON			0xF
#define WDG_POWERTON			0xA

#define SPRDBAT_FGUADC_CAL_NO		0
#define SPRDBAT_FGUADC_CAL_NV		1
#define SPRDBAT_FGUADC_CAL_CHIP		2
#define VOL_READY_INT_THRESHOLD		1000
#define WAIT_WRITE_ACTIVE_RETRY_CNT	3

#define SPRD_FGU_IDEAL_RESISTANCE	20000
#define SPRD_FGU_CUR_BASIC_ADC		8192

#define mdelay(_ms)			udelay(_ms*1000)

static int fgu_nv_4200mv = 2752;
static int fgu_nv_3600mv = 2374;

struct sprdfgu_cal {
	int vol_1000mv_adc;
	int cur_1000ma_adc;
	int vol_offset;
	int cur_offset;
	int cal_type;
};

#ifdef CONFIG_DM_FGU
struct sprd_fgu_dm_data {
	unsigned int blk_index;
};

struct sprd_fgu_dm_data fgu_dm_data;

/*sc2720 sc2721 sc2730 sc2731 */
#define EFUSE_BLOCK_WIDTH		16
#define EFUSE_BIT_INDEX			48
#define EFUSE_FGU_BIT_LENGTH		9
#endif

#ifndef CALIB_RESISTANCE_MICRO_OHMS
#define CALIB_RESISTANCE_MICRO_OHMS	0
#endif

static struct sprdfgu_cal fgu_cal =
	{ 2872, 0, 0, 0, SPRDBAT_FGUADC_CAL_NO};

static u32 sprdfgu_adc2vol_mv(s64 adc)
{
	return ((adc + fgu_cal.vol_offset) * 1000)
		/ fgu_cal.vol_1000mv_adc;
}

static u32 sprdfgu_adc2cur_ma(s64 adc)
{
	return ((adc + fgu_cal.cur_offset) * 1000)
		/ fgu_cal.cur_1000ma_adc;
}

static int sprdfgu_reg_get(unsigned long reg)
{
	int old_value = sci_adi_read(reg);
	int new_value, times = 1000;

	while ((old_value != (new_value = sci_adi_read(reg))) &&
		(times > 0)) {
		old_value = new_value;
		times--;
	}
	return new_value;
}

uint32_t sprdfgu_read_vbat_vol(void)
{
	int cur_vol_raw;
	uint32_t temp;

	cur_vol_raw = sprdfgu_reg_get(REG_FGU_VOLT_VAL);
	temp = sprdfgu_adc2vol_mv(cur_vol_raw);
	return temp;
}

uint32_t sprdfgu_read_ibat_cur(void)
{
	int cur_now_raw;
	uint32_t temp;

	cur_now_raw = sprdfgu_reg_get(REG_FGU_CURT_VAL)-SPRD_FGU_CUR_BASIC_ADC;
	temp = sprdfgu_adc2cur_ma(cur_now_raw);
	return temp;
}

static void sprdfgu_rtc_reg_write(uint32_t val)
{
	sci_adi_write(REG_FGU_USER_AREA_CLEAR, BITS_RTC_AREA(~val),
		      BITS_RTC_AREA(~0));
	sci_adi_write(REG_FGU_USER_AREA_SET, BITS_RTC_AREA(val),
		      BITS_RTC_AREA(~0));
}

static uint32_t sprdfgu_rtc_reg_read(void)
{
	return (sci_adi_read(REG_FGU_USER_AREA_STATUS) & BITS_RTC_AREA(~0)) >>
	    BITS_RTC_AREA_SHIFT;
}

static void sprdfgu_poweron_type_write(uint32_t val)
{
	sci_adi_write(REG_FGU_USER_AREA_CLEAR, BITS_POWERON_TYPE(~val),
		      BITS_POWERON_TYPE(~0));
	sci_adi_write(REG_FGU_USER_AREA_SET, BITS_POWERON_TYPE(val),
		      BITS_POWERON_TYPE(~0));
}

#if defined(CONFIG_ADIE_UMP518)
static void sprdfgu_switch_512k_clk(void)
{
	int cnt = WAIT_WRITE_ACTIVE_RETRY_CNT;

	sci_adi_write(REG_FGU_CONFIG, BIT_FGU_DISABLE_EN, BIT_FGU_DISABLE_EN);

	while ((sci_adi_read(REG_FGU_STATUS) & BIT_WRITE_ACTIVE_STS) && cnt--){
		udelay(50);
	}

	if (cnt <= 0)
		dprintf(ALWAYS,"sprd_chg: error!!! failed get fgu status\n!");

	sci_adi_write(ANA_REG_GLB_RTC_CLK_EN, 0, BIT_RTC_FGU_EN);
	sci_adi_write(REG_FGU_ADC_CTRL1, 0, BITS_CLK_SEL_512K);
	sci_adi_write(ANA_REG_GLB_RTC_CLK_EN, BIT_RTC_FGU_EN, BIT_RTC_FGU_EN);
	sci_adi_write(REG_FGU_START, BIT_FGU_RESET, BIT_FGU_RESET);
	udelay(100);
	sci_adi_write(REG_FGU_CONFIG, 0, BIT_FGU_DISABLE_EN);
	/* switch the 512kHZ clk is complete, need to wait for a voltage
	 * sample period (delay_time > 250ms) before reading the voltage.
	 */
	mdelay(252);
}
#endif

static uint32_t sprdfgu_poweron_type_read(void)
{
	return (sci_adi_read(REG_FGU_USER_AREA_STATUS) & BITS_POWERON_TYPE(~0))
	    >> BITS_POWERON_TYPE_SHIFT;
}

#ifdef CONFIG_DM_FGU
static int sprdfgu_dm_cal_get(unsigned int *p_cal_data)
{
	unsigned int data = 0;

	dprintf(INFO,"sprd_chg: blk index = %d\n", fgu_dm_data.blk_index);

	data |= sprd_pmic_efuse_read(fgu_dm_data.blk_index);
	data >>= (EFUSE_BIT_INDEX & (EFUSE_BLOCK_WIDTH - 1));
	data &= BIT(EFUSE_FGU_BIT_LENGTH) - 1;

	dprintf(INFO,"sprd_chg: fgu_dm_cal_get 4.2 data data: 0x%x\n", data);
	p_cal_data[0] = (data + 6963) - 4096 - 256;

	return 0;
}
#else
static int sprdfgu_cal_get(unsigned int *p_cal_data)
{
	unsigned int data;

#if defined(CONFIG_ADIE_SC2723)
	data = sprd_pmic_efuse_read_bits(BITSINDEX(12, 0, 8), 9);
	dprintf(INFO,"sprd_chg: fgu_cal_get 4.2 data data: 0x%x\n", data);
	p_cal_data[0] = (data + 6963) - 4096 - 256;

	return 0;

#elif defined(CONFIG_ADIE_SC2731) || defined(CONFIG_ADIE_SC2721)|| defined(CONFIG_ADIE_SC2720) || defined(CONFIG_ADIE_SC2730)
	data = sprd_pmic_efuse_read_bits(BITSINDEX(3, 0, 16), 9);
	dprintf(INFO,"sprd_chg: fgu_cal_get 4.2 data data: 0x%x\n", data);
	p_cal_data[0] = (data + 6963) - 4096 - 256;

	return 0;

#elif defined(CONFIG_ADIE_UMP9620)
	data = sprd_pmic_efuse_read_bits(BITSINDEX(38, 7, 16), 9);
	dprintf(INFO,"sprd_chg: ump9620 fgu_cal_get 4.2 data data: 0x%x\n", data);
	p_cal_data[0] = (data + 6963) - 4096 - 256;

	return 0;

#elif defined(CONFIG_ADIE_UMP518)
	data = sprd_pmic_efuse_read_bits(BITSINDEX(39, 7, 16), 9);
	dprintf(INFO,"sprd_chg: ump518 fgu_cal_get 4.2 data data: %#x\n", data);
	p_cal_data[0] = (data + 6963) - 4096 - 256;

	return 0;

#else
	data = sprd_pmic_efuse_read_bits(BITSINDEX(3, 0, 16), 9);
	dprintf(INFO,"sprd_chg: fgu_cal_get 4.2 data data: 0x%x\n", data);
	p_cal_data[0] = (data + 6963) - 4096 - 256;

	return 0;
#endif
}
#endif

#ifdef CONFIG_DM_FGU
static int sprd_dm_fgu_init(void)
{
	struct udevice *devp;
	int ret;

	ret = uclass_get_device(UCLASS_FGU, 0, &devp);
	if (ret) {
		dprintf(ALWAYS,"sprd_chg: %s:failed to get dev ret = %d\n", __func__, ret);
		return ret;
	}

	return 0;
}
#endif

static void sprdfgu_cal_init(void)
{
	if (fgu_nv_3600mv == 0) {
		fgu_cal.vol_1000mv_adc =
		    DIV_ROUND_CLOSEST((fgu_nv_4200mv) * 10, 42);
		fgu_cal.vol_offset = 0;
	} else {
		fgu_cal.vol_1000mv_adc =
		    DIV_ROUND_CLOSEST((fgu_nv_4200mv - fgu_nv_3600mv) * 10, 6);
		fgu_cal.vol_offset =
		    0 - (fgu_nv_4200mv * 10 - fgu_cal.vol_1000mv_adc * 42) / 10;
	}
	fgu_cal.cur_1000ma_adc =
            DIV_ROUND_CLOSEST(fgu_cal.vol_1000mv_adc * 4 * CALIB_RESISTANCE_MICRO_OHMS,
		      SPRD_FGU_IDEAL_RESISTANCE);

	dprintf(INFO,"sprd_chg: 4200mv=%d, 3600mv=%d\n", fgu_nv_4200mv, fgu_nv_3600mv);
	dprintf(INFO,"sprd_chg: fgu_cal.vol_1000mv_adc=%d, vol_offset=%d\n",
		fgu_cal.vol_1000mv_adc, fgu_cal.vol_offset);
}

static int sprdfgu_cal_from_chip(void)
{
	unsigned int fgu_data[4] = { 0 };

#ifdef CONFIG_DM_FGU
		if (sprdfgu_dm_cal_get(fgu_data)) {
			dprintf(ALWAYS,"sprd_chg: %s:dm efuse no cal data\n", __func__);
			return 1;
		}
#else
	if (sprdfgu_cal_get(fgu_data)) {
		dprintf(INFO,"sprd_chg: efuse no cal data\n");
		return 1;
	}
#endif

	dprintf(INFO,"sprd_chg: fgu_data: 0x%x\n", fgu_data[0]);

	fgu_nv_4200mv = fgu_data[0];
	fgu_nv_3600mv = 0;
	fgu_cal.cal_type = SPRDBAT_FGUADC_CAL_CHIP;
	dprintf(INFO,"sprd_chg: sprdfgu: one point\n");
	return 0;
}

void sprdfgu_init(void)
{
	int cnt = VOL_READY_INT_THRESHOLD;
	int ret;

	sci_adi_set(ANA_REG_GLB_ARM_MODULE_EN, BIT_ANA_FGU_EN);
	/* Only sc2723 and sc2731 pmic needs to set
	 * BIT_RTC_FGUA_EN bit.
	 */
#if defined(CONFIG_ADIE_SC2723) || defined(CONFIG_ADIE_SC2731)
	sci_adi_set(ANA_REG_GLB_RTC_CLK_EN, BIT_RTC_FGU_EN | BIT_RTC_FGUA_EN);
#else
	sci_adi_set(ANA_REG_GLB_RTC_CLK_EN, BIT_RTC_FGU_EN);
#endif
	sci_adi_clr(REG_FGU_INT_EN, 0xFFFF);	//disable int after watchdog reset
	sci_adi_set(REG_FGU_INT_CLR, 0xFFFF);

#ifdef CONFIG_DM_FGU
	ret = sprd_dm_fgu_init();
	if (ret) {
		dprintf(ALWAYS,"sprd_chg: %s: failed to get dts para ret = %d", __func__, ret);
		return;
	}
#endif

	sprdfgu_cal_from_chip();
	sprdfgu_cal_init();

#if defined(CONFIG_ADIE_UMP518)
	sci_adi_write(REG_FGU_CURT_OFFSET_H, 0, ~0);
	sci_adi_write(REG_FGU_CURT_OFFSET_L, 0, ~0);	//init offset after watchdog reset
	/* 518 pmic fgu need to detect the present
	 * of battery voltage.
	 */
	while ((sprdfgu_read_vbat_vol() < 2000) && cnt--){
		mdelay(2);
	}

	if (cnt <= 0)
		dprintf(ALWAYS,"sprd_chg: error!!! ump518 fgu voltage = %dmV is not ready!",
			sprdfgu_read_vbat_vol());

	sprdfgu_switch_512k_clk();
#else
	sci_adi_write(REG_FGU_CURT_OFFSET, 0, ~0);	//init offset after watchdog reset
	/* When the symbol position BIT_VOL_READY_INT is 1,
	 * the FGU voltage can be picked up
	 */
	while (!(sci_adi_read(REG_FGU_INT_RAW) & BIT_VOL_READY_INT) && cnt--){
		mdelay(2);
	}

	if (cnt <= 0)
		dprintf(ALWAYS,"sprd_chg: error!!! fgu voltage not ready!");
#endif
}

void sprdfgu_late_init(void)
{
	uint32_t mode, value, cap;

	mode = sprdfgu_poweron_type_read();
	value = sprdfgu_rtc_reg_read();

	if ((mode == FIRST_POWERTON) || (value == 0xFFF)) {
		charge_first_power_on = true;
		sprdfgu_poweron_type_write(FIRST_POWERTON);
		mdelay(1);
		dprintf(ALWAYS,"sprd_chg: charge first poweron reset, mode= 0x%x, value= 0x%x\n", mode, value);
		sprdfgu_rtc_reg_write(0xFFF);
		return;
	}

	dprintf(ALWAYS,"sprd_chg: mode = 0x%x, value = 0x%x\n", mode, value);
}

#ifdef CONFIG_DM_FGU
/*
 *sc2721 sc2720 sc2731 sc2730: blk_index: 3
 *this must config carefully,or it will lowpower ,can't power on
 */
static int sprd_fgu_probe(struct udevice *dev)
{
	int ret;

	ret = dev_read_u32(dev, "sprd,fgu-blk-index", &fgu_dm_data.blk_index);
	if (ret) {
		dprintf(ALWAYS,"sprd_chg: %s:failed to get fgu_blk_index ret=%d\n",
		       __func__, ret);
		return ret;
	}

	return 0;
}

static const struct udevice_id sprd_fgu_ids[] = {
	{.compatible = "sprd,sc2731-fgu"},
	{.compatible = "sprd,sc2730-fgu"},
	{.compatible = "sprd,sc2721-fgu"},
	{.compatible = "sprd,sc2720-fgu"},
	{ }
};

U_BOOT_DRIVER(sprd_fgu) = {
	.name = "sprd-fgu",
	.id = UCLASS_FGU,
	.of_match = sprd_fgu_ids,
	.probe = sprd_fgu_probe,
};

UCLASS_DRIVER(fgu) = {
	.name = "fgu",
	.id = UCLASS_FGU,
};
#endif

