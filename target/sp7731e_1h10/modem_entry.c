#include <sprd_common.h>
#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>
#include "cp_boot.h"

/*add macro for memset agdsp/tddsp/ldsp share memory*/
#define DSP_BOOT_IRAM_START		0x5000f000
#define TGDSP_SHARE_MEM_START	0x5000e000
#define MULTI_MODE_MEM_START	0x50010000


/*****************************************************************************/
//  Description:    Gets the current reset mode.
//  Author:         Andrew.Yang
//  Note:
/*****************************************************************************/
#ifdef CONFIG_ARM7_RAM_ACTIVE
void pmic_arm7_RAM_active(void)
{
	*((volatile u32*)REG_PMU_APB_CP_SOFT_RST) &= ~BIT_PMU_APB_CM4_SOFT_RST;
	*((volatile u32*)REG_PMU_APB_SLEEP_CTRL) &= ~ BIT_PMU_APB_CM4_FORCE_DEEP_SLEEP;   /*clear sp force sleep */
	*((volatile u32*)REG_AON_APB_CM4_SYS_SOFT_RST) &= ~BIT_AON_APB_CM4_SYS_SOFT_RST;
	msleep(100);
}
#endif

/*bug:533275  memset tg/l/agdsp share memory
root cause: In agdsp share IRAM memory ,this area exits command for agdsp,when interunpt happens
from ap or cp,the agdsp will use this area. While the area exit the dirty binary data ,ap or cp
do not clean up this area when interrupt happens at the right time,so error command will cause
unexcepted problems.*/

void memset_dsp_share_memory()
{
	memset((void *)DSP_BOOT_IRAM_START,0x0,0x1000);//4k
	memset((void *)TGDSP_SHARE_MEM_START,0x0,0x1000);//4k
	memset((void *)MULTI_MODE_MEM_START,0x0,0x800);//2k
}

void modem_entry()
{
	memset_dsp_share_memory();

#ifndef CONFIG_KERNEL_BOOT_CP
	sp_boot();
	pubcp_boot();
	debugf("boot CP1 OK\n");
#else
	extern unsigned int g_charger_mode;
	if(g_charger_mode) {
		sp_boot();
		debugf("boot sp  OK\n");
	}
#endif
}

