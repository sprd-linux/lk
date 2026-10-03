/*
 *  sprd_glb.c - Support register operation interface
 *
 *  Copyright (C) 2021 Unisoc Communications Inc.
 *  History:
 *      2021/7/29 porter.xu@unisoc.com
 *      Add register operation interface
 */

#include <sprd_common.h>
#include <errno.h>
#include <asm/arch/common.h>
#include <asm/types.h>

u32 sci_glb_read(u32 reg, u32 msk)
{
	return readl(reg) & msk;
}

int sci_glb_write(u32 reg, u32 val, u32 msk)
{
	writel((readl(reg) & ~msk) | val, reg);
	return 0;
}

int sci_glb_set(u32 reg, u32 bit)
{
	writel(readl(reg) | bit, reg);
	return 0;
}

int sci_glb_clr(u32 reg, u32 bit)
{

	writel((readl(reg) & ~bit), reg);
	return 0;
}
