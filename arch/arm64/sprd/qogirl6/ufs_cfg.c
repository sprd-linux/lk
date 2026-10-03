/*
 * Copyright (C) 2018 Spreadtrum Communications Inc.
 */

#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>
//#include <asm/io.h>
#include <sprd_ufs.h>
#include <sprd_efuse.h>

#define BITS_PER_LONG 64
#define GENMASK(h, l) \
	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
#define EFUSE_UFS_BLOCK45 45
#define UFS_TXRX_PHY_MASK GENMASK(11,0)
#define UFS_TXRX_PHY_ADDR 0x58
#define REG32(x) (*((volatile u32 *)(x)))

static void ufs_sprd_rmwl(void *base, u32 mask, u32 val, u32 reg)
{
	u32 tmp;
/*
	printf("regmap_ufs %16x&=%8x\n",(base+reg),~mask);
	printf("           %16x|=%8x\n",(base+reg),(val & mask));
*/
	tmp = REG32((base) + (reg));
	tmp &= ~mask;
	tmp |= (val & mask);
	REG32((base) + (reg)) = tmp;
}
int set_ufs_trim_from_efuse()
{
	unsigned int efuse_reg, ufs_trimbg,  ufs_rxtrim,  ufs_txtrim;

	efuse_reg = sprd_ap_efuse_read(EFUSE_UFS_BLOCK45);
	if (!efuse_reg) {
		debugf("err:read ufs efuse err\n");
		return -1;
	}
	ufs_trimbg = 0xf & (efuse_reg >> 0x0);
	ufs_rxtrim = 0xf & (efuse_reg >> 0x4);
	ufs_txtrim = 0xf & (efuse_reg >> 0x8);

	ufs_sprd_rmwl(CTL_ANLG_PHY_G1_BASE, UFS_TXRX_PHY_MASK,
			(ufs_trimbg << 8) | (ufs_txtrim << 4) | (ufs_rxtrim << 0), UFS_TXRX_PHY_ADDR);
	return 0;
}

void init_global_reg(void)
{
	debugf("init_global_reg \n ");

	CHIP_REG_OR(REG_AP_AHB_MPHY_CB_CHANNEL_0, (BIT_AP_AHB_CB_RESET |
						   BIT_AP_AHB_CB_ATTRRESET));
	udelay(100);

	CHIP_REG_AND(REG_ANLG_PHY_G1_ANALOG_MIPI_MPHY_2T2R_MPHY_APB_REG_1,
		     ~BIT_ANLG_PHY_G1_ANALOG_MIPI_MPHY_2T2R_MPHY_APB_PRESETN);
	udelay(100);

	CHIP_REG_OR(REG_ANLG_PHY_G1_ANALOG_MIPI_MPHY_2T2R_MPHY_APB_REG_1,
		    BIT_ANLG_PHY_G1_ANALOG_MIPI_MPHY_2T2R_MPHY_APB_PRESETN);
	udelay(100);

	CHIP_REG_AND(REG_AP_AHB_MPHY_CB_CHANNEL_0,
		     ~(BIT_AP_AHB_CB_RESET | BIT_AP_AHB_CB_ATTRRESET));
	udelay(100);

	CHIP_REG_AND(REG_AP_AHB_MPHY_CB_CHANNEL_0,
		     ~(BIT_AP_AHB_CB_RESET | BIT_AP_AHB_CB_ATTRRESET));

	CHIP_REG_OR(REG_AP_APB_APB_EB1, (BIT_AP_APB_UFS_EB |
					 BIT_AP_APB_UFS_REG_EB));

	CHIP_REG_OR(REG_AON_APB_APB_EB1,(BIT_AON_APB_PIN_EB |
					 BIT_AON_APB_ANA_EB));
	//remove this first keep the same with the romcode

	CHIP_REG_OR(REG_AP_AHB_UFS_CLK_CTRL,BIT_AP_AHB_CG_CFGCLK_SW);

	CHIP_REG_OR(REG_AP_APB_APB_RST,(BIT_AP_APB_UFS_REG_SOFT_RST |
					BIT_AP_APB_UFS_SOFT_RST));
	udelay(100);
	CHIP_REG_AND(REG_AP_APB_APB_RST,~(BIT_AP_APB_UFS_REG_SOFT_RST |
					  BIT_AP_APB_UFS_SOFT_RST));

	CHIP_REG_OR(REG_AP_AHB_MPHY_CB_CHANNEL_0,BIT_AP_AHB_REFCLKON);

	CHIP_REG_AND(REG_AP_AHB_UFS_LP_CTRL_1,
		     ~BIT_AP_AHB_UFS_FORCE_LP_ISOL_EN);

	CHIP_REG_OR(REG_AP_AHB_UFS_LP_CTRL_1,
		     (BIT_AP_AHB_UFS_SEL_LP_RESET_N | BIT_AP_AHB_UFS_FORCE_LP_RESET_N |
		     BIT_AP_AHB_UFS_SEL_LP_ISOL_EN | BIT_AP_AHB_UFS_SEL_LP_PWR_READY |
		     BIT_AP_AHB_UFS_FORCE_LP_PWR_READY));

	udelay(300);

	CHIP_REG_OR(0x6456c08c,BIT_15);
	CHIP_REG_OR(0x6456c88c,BIT_15);

	udelay(100);
	set_ufs_trim_from_efuse();
}

int ufshci_enable_bottom()
{
	return 0;
}

struct ufs_hba_variant_ops hba_vops = {
	.name = "qogirl6",
	.enable_bottom = ufshci_enable_bottom,
};
