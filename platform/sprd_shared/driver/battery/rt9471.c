#include "sprd_chg_helper.h"
#include <errno.h>
#include <i2c.h>
#include <chipram_env.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <lk/debug.h>


/* Register 00h */
#define RT9471_REG_00				0x00

/* Register 01h */
#define RT9471_REG_01				0x01
#define REG01_WDT_RESET_MASK			0x04
#define REG01_WDT_RESET_SHIFT			2
#define REG01_WDT_RESET				1

/* Register 0x02*/
#define RT9471_REG_02				0x02
#define REG02_CHG_CONFIG_MASK			0x01
#define REG02_CHG_CONFIG_SHIFT			0
#define REG02_CHG_DISABLE			0
#define REG02_CHG_ENABLE			1

/* Register 0x03*/
#define RT9471_REG_03				0x03

#define REG03_IINLIM_MASK 			0x3f
#define REG03_IINLIM_SHIFT 			0
#define REG03_IINLIM_LSB 			50
#define REG03_IINLIM_BASE 			50
#define REG03_IINLIM_OFFSET 		50
#define REG03_IINLIM_CURRENT_MAX 	3200
#define REG03_IINLIM_CURRENT_OFFSET 50

/* Register 0x04*/
#define RT9471_REG_04				0x04

/* Register 0x05*/
#define RT9471_REG_05				0x03
#define REG05_IPRECHG_MASK			0x0f
#define REG05_IPRECHG_SHIFT			0
#define REG05_IPRECHG_LSB			50
#define REG05_IPRECHG_CURRENT_MAX		800

/* Register 0x06*/
#define RT9471_REG_06				0x06

/* Register 0x07*/
#define RT9471_REG_07				0x07

/* Register 0x08*/
#define RT9471_REG_08					0x08
#define REG08_ICHG_MASK				0x3f
#define REG08_ICHG_SHIFT			0
#define REG08_ICHG_BASE				0
#define REG08_ICHG_LSB				50
#define REG08_ICHG_MAX				3150

/* Register 0x09*/
#define RT9471_REG_09				0x09

/* Register 0x0A */
#define RT9471_REG_0A				0x0a

/* Register 0x0B */
#define	RT9471_REG_0B				0x0b
#define	REG0B_REG_RESET_MASK			0x80
#define	REG0B_REG_RESET_SHIFT			7
#define	REG0B_REG_RESET				1

#define RT9471_REG_PART_MASK		0x08
#define RT9471_REG_PART_SHIFT		3
#define RT9471_PART_VALUE			1

#define I2C_SPEED			100000
#define SLAVE_ADDR			0x53

#define POWER_PATH_ENABLE               0
#define POWER_PATH_DISABLE              1

//extern chipram_env_t* get_chipram_env(void);

static int i2c_bus_num = SPRDCHG_I2C_BUS;

#ifdef CONFIG_DM_RT9471
struct sprd_rt9471_dm_data {
	int i2c_num;
};

static struct sprd_rt9471_dm_data dm_data = {
	.i2c_num = -1,
};

struct udevice *charger;

static int sprd_dm_rt9471_i2c_init(void)
{
	int ret;
	u8 value;

	if (dm_data.i2c_num != -1) {
		ret = i2c_get_chip_for_busnum(dm_data.i2c_num,
				SLAVE_ADDR, 1,
					&charger);
		if (ret) {
			pr_err("%s: i2c%d failed to get\n",
					__func__, dm_data.i2c_num);
			return ret;
		}
	} else {
		pr_err("%s:failed to set i2c bus num\n", __func__);
		return -EINVAL;
	}

	if (!charger) {
		pr_err("NULL pointer in %s:line%d\n", __func__, __LINE__);
		return -EINVAL;
	}

	ret = dm_i2c_set_bus_speed(dev_get_parent(charger), I2C_SPEED);
	if (ret) {
		pr_err("%s: failed to set i2c%d speed\n",
		       __func__, dm_data.i2c_num);
		return ret;
	}
	return 0;
}

static int rt9471_write_reg(int reg, u8 val)
{
	if (!charger) {
		pr_err("NULL pointer in %s:line=%d\n", __func__, __LINE__);
		return -EINVAL;
	}

	dm_i2c_reg_write(charger, reg, val);
	return 0;
}

static int rt9471_read_reg(int reg, u8 *value)
{
	u8 val;
	int ret;

	if (!charger) {
		pr_err("NULL pointer in %s:line%d\n", __func__, __LINE__);
		return -EINVAL;
	}

	ret = dm_i2c_reg_read(charger, reg);
	if (ret < 0) {
		pr_err("%s: reg(0x%x), ret(%d)\n", __func__, reg, ret);
		return ret;
	}

	*value = (u8)ret;
	debugf("###### rt9471 readreg reg = %d value = %d/%x\n", reg, ret, ret);
	return 0;
}
#else
static int rt9471_i2c_init(void)
{
	return 0;
}

static int rt9471_write_reg(u8 reg, u8 val)
{
	int ret;
	uint8_t buf[2] = {0};
	buf[0] = reg;
	buf[1] = val;
	ret = i2c_send(i2c_bus_num, SLAVE_ADDR, buf, 2);
	if (ret < 0) {
		dprintf(ALWAYS,"%s reg(0x%x), ret(%d)\n", __func__, reg, ret);
		return ret;
	}

	return 0;
}

static int rt9471_read_reg(u8 reg, u8 *value)
{
	int ret;
	uint8_t reg_addr[2] = {0};
	reg_addr[0] = reg;
	ret = i2c_read_write(i2c_bus_num, SLAVE_ADDR, reg_addr, 1, value, 1);

	if (ret < 0) {
		dprintf(ALWAYS,"%s reg(0x%x), ret(%d)\n", __func__, reg, ret);
		return ret;
	}

	dprintf(ALWAYS,"rt9471_read_reg reg = 0x%x, value = %d/0x%x\n", reg, *value, *value);

	return 0;
}
#endif

static void rt9471_set_value(u8 reg, u8 reg_bit, u8 reg_shift, u8 val)
{
	u8 tmp = 0;

	rt9471_read_reg(reg, &tmp);
	tmp = tmp & (~reg_bit) | (val << reg_shift);
	rt9471_write_reg(reg, tmp);
}

static u8 rt9471_get_value(u8 reg, u8 reg_bit, u8 reg_shift)
{
	u8 reg_value = 0;

	rt9471_read_reg(reg, &reg_value);
	reg_value = (reg_value & reg_bit) >> reg_shift;

	return reg_value;
}

static void chg_rt9471_set_chg_cur(u32 cur)
{
	u8 reg_value;

	dprintf(ALWAYS,"chg_rt9471_set_chg_cur : %d \n", cur);

	if (cur > REG08_ICHG_MAX)
		cur = REG08_ICHG_MAX;

	reg_value = cur / REG08_ICHG_LSB;

	rt9471_set_value(RT9471_REG_08, REG08_ICHG_MASK,
			  REG08_ICHG_SHIFT, reg_value);

}

static void chg_rt9471_set_limit_cur(u32 limit)
{
	u8 reg_value;

	dprintf(ALWAYS,"chg_rt9471_set_limit_cur : %d \n", limit);

	if (limit > REG03_IINLIM_CURRENT_MAX)
		limit = REG03_IINLIM_CURRENT_MAX;

	reg_value = limit / REG03_IINLIM_BASE;
	rt9471_set_value(RT9471_REG_03, REG03_IINLIM_MASK,
			  REG03_IINLIM_SHIFT, reg_value);

}

static int chg_rt9471_set_prechg(u32 ichg)
{
	u8 reg_value;

	if (ichg > REG05_IPRECHG_CURRENT_MAX)
		ichg = REG05_IPRECHG_CURRENT_MAX;

	reg_value = ichg / REG05_IPRECHG_LSB;
	rt9471_set_value(RT9471_REG_05, REG05_IPRECHG_MASK,
			  REG05_IPRECHG_SHIFT, reg_value);

	return 0;
}

static int chg_rt9471_cmd(enum sprdchg_cmd cmd, int value)
{
	switch (cmd) {
	case CHG_SET_CURRENT:
		chg_rt9471_set_chg_cur(value);
		break;
	case CHG_SET_LIMIT_CURRENT:
		chg_rt9471_set_limit_cur(value);
		break;
	case CHG_SET_PRE_CURRENT:
		chg_rt9471_set_prechg(value);
		break;
	default:
		break;
	}

	return 0;
}

static void chg_rt9471_reset(void)
{
	rt9471_set_value(RT9471_REG_0B, REG0B_REG_RESET_MASK,
			  REG0B_REG_RESET_SHIFT, REG0B_REG_RESET);
}

static void chg_rt9471_init(void)
{
	int ret;

#ifdef CONFIG_DM_RT9471
	ret = sprd_dm_rt9471_i2c_init();
#else
	ret = rt9471_i2c_init();
#endif
	if (ret) {
		dprintf(ALWAYS,"%s:rt9471 i2c init fail\n", __func__);
		return;
	}
}

static void chg_rt9471_enable_chg(int val)
{
	rt9471_set_value(RT9471_REG_02, REG02_CHG_CONFIG_MASK,
			  REG02_CHG_CONFIG_SHIFT, REG02_CHG_ENABLE);
}

static void chg_rt9471_disable_chg(int val)
{

	rt9471_set_value(RT9471_REG_02, REG02_CHG_CONFIG_MASK,
			  REG02_CHG_CONFIG_SHIFT, REG02_CHG_DISABLE);
}

static void chg_rt9471_reset_timer(void)
{
	rt9471_set_value(RT9471_REG_01, REG01_WDT_RESET_MASK,
			  REG01_WDT_RESET_SHIFT, REG01_WDT_RESET);
}

static struct sprdchg_ic_operations rt9471_op = {
	.ic_init = chg_rt9471_init,
	.chg_start = chg_rt9471_enable_chg,
	.chg_stop = chg_rt9471_disable_chg,
	.timer_callback = chg_rt9471_reset_timer,
	.chg_cmd = chg_rt9471_cmd,
};

#ifdef CONFIG_DM_RT9471
static int sprd_rt9471_probe(struct udevice *dev)
{
	u32 i2c_id;
	int ret;

	ret = dev_read_u32(dev, "sprd,rt9471-i2c-bus", &i2c_id);
	if (ret) {
		pr_err("%s:failed to get i2c-bus ret = %d!\n", __func__, ret);
		return ret;
	}
	debugf("i2c_id = %d\n", i2c_id);
	dm_data.i2c_num = i2c_id;
	return 0;
}

static int sprd_dm_rt9471_init(void)
{
	struct udevice *devp;
	int ret;

	ret = uclass_get_device(UCLASS_CHARGER, 0, &devp);
	if (ret) {
		pr_err("%s:failed to get device ret = %d", __func__, ret);
		return ret;
	}
	ret = sprd_dm_rt9471_i2c_init();
	if (ret) {
		pr_err("%s:failed to init dm i2c ret = %d", __func__, ret);
		return ret;
	}
	return 0;
}
#endif

static int rt9471_charger_get_vendor_id_part_value(void)
{
	int ret = 0;
	u8 reg_part_val = 0;

	ret = rt9471_read_reg(RT9471_REG_0B, &reg_part_val);
	if (ret < 0) {
		printf("[%s]l=%d: Failed to get vendor id, ret=%d\n",
			__FUNCTION__, __LINE__, ret);
		return ret;
	}

	reg_part_val = (reg_part_val & RT9471_REG_PART_MASK) >> RT9471_REG_PART_SHIFT;
	if (reg_part_val != RT9471_PART_VALUE) {
		printf("[%s]l=%d: The part value is 0x%x\n",
			__FUNCTION__, __LINE__, reg_part_val);
		return -EINVAL;
	}

	return ret;
}

void sprdchg_rt9471_init(void)
{
	int ret;

	printf("rt9471 init\n");
#ifdef CONFIG_DM_RT9471
	ret = sprd_dm_rt9471_init();
#else
	ret = rt9471_i2c_init();
#endif
	if (ret) {
		dprintf(ALWAYS, "rt9471 i2c init failed\n");
		return;
	}

	ret = rt9471_charger_get_vendor_id_part_value();
	if (ret) {
		printf("[%s]l=%d: rt9471 is not found, not register ops\n",
			__FUNCTION__, __LINE__);
		return ret;
	}
	printf("rt9471 register charge ops!\n");
	sprdchg_register_ops(&rt9471_op);
}

#ifdef CONFIG_DM_RT9471
static const struct udevice_id sprd_rt9471_ids[] = {
	{.compatible = "sprd,rt9471-charger"},
	{ }
};

U_BOOT_DRIVER(rt9471) = {
	.name = "sprd-rt9471",
	.id = UCLASS_CHARGER,
	.of_match = sprd_rt9471_ids,
	.probe = sprd_rt9471_probe,
};

/*
UCLASS_DRIVER(charger)= {
	.name = "charger",
	.id = UCLASS_CHARGER,
};
*/
#endif
