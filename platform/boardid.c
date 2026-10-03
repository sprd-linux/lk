/*
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
#include <lk/debug.h>
#include <lk/reg.h>
#include <target.h>
#include <asm/arch/pinmap.h>

#ifdef CONFIG_MATCH_DTBO_BY_BOARDID
/*
 *	demo: BoardID:	41  A  L
 *			--  -  -
 *		      4h10  |   \
 *			   Area  HWLevel
 *
 * Area:	gpio130 gpio131 gpio132
 *	areaA	0	0	0
 *	areaB	0	0	1
 *	...
 *
 * HWLevel:	gpio133 gpio112 gpio176
 *	EVB	1	1	0
 *	EVT	1	0	0
 *	DVT	0	1	0
 *	PVT	0	0	0
 *	MP	0	0	1
 */

/* GPIOs used for encoding BoardID */
unsigned int gpio_table[CONFIG_BOARDID_ENCODING_GPIO_NUMS] = {130, 131, 132, 133, 112, 176};

/* function select addr */
unsigned int pinaf_addr_table[CONFIG_BOARDID_ENCODING_GPIO_NUMS] = {
	REG_PIN_IIS1DI, REG_PIN_IIS1DO, REG_PIN_IIS1CLK,
	REG_PIN_IIS1LRCK, REG_PIN_IIS3DI, REG_PIN_BT_RFCTL3
};

/* drive strength addr */
unsigned int pinds_addr_table[CONFIG_BOARDID_ENCODING_GPIO_NUMS] = {
	REG_MISC_PIN_IIS1DI, REG_MISC_PIN_IIS1DO, REG_MISC_PIN_IIS1CLK,
	REG_MISC_PIN_IIS1LRCK, REG_MISC_PIN_IIS3DI, REG_MISC_PIN_BT_RFCTL3
};

char boardid_hwlevel_mapping[8][5] = {
	"PVT",
	"MP",
	"DVT",
	"",
	"EVT",
	"",
	"EVB",
	""
};

int gpio_value_table_pullup[CONFIG_BOARDID_ENCODING_GPIO_NUMS] __attribute__((section(".data"))) = { 0 };
int gpio_value_table_pulldown[CONFIG_BOARDID_ENCODING_GPIO_NUMS] __attribute__((section(".data"))) = { 0 };

int target_get_boardid() {
	int boardid, area, hwlevel;
	int *gpio_value_table = gpio_value_table_pulldown;
	area = (gpio_value_table[0]<<2) + (gpio_value_table[1]<<1) + gpio_value_table[2];
	hwlevel = (gpio_value_table[3]<<2) + (gpio_value_table[4]<<1) + gpio_value_table[5];
	boardid = CONFIG_BOARDID_BASE + (area<<4) + hwlevel;
	printf("[%s] boardid:0x%x, area:0x%x, hwlevel:0x%x\n", __func__, boardid, area, hwlevel);
	return boardid;
}

char *target_get_hwlevel() {
	char *hwlevel = NULL;
	int idx = 0xF & (target_get_boardid()-CONFIG_BOARDID_BASE);
	if (idx >= 0 && idx < 8) {
		hwlevel = boardid_hwlevel_mapping[idx];
	}
	return hwlevel;
}

#endif
