/*
 * Copyright (C) 2021 Spreadtrum Communications Inc.
 * AUTHOR: Changmao Guo <changmao.guo@unisoc.com>
 *
 */


#ifndef __ARCH_ARM64_SPRD_MACRO_H__
#define __ARCH_ARM64_SPRD_MACRO_H__
/*
 * Register link-register.
 */
lr	.req	x30

/*
 * Exception level switch
 */
.macro	switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, 0xc
	b.eq	\el3_label
	cmp	\xreg, 0x8
	b.eq	\el2_label
	cmp	\xreg, 0x4
	b.eq	\el1_label
.endm
#endif /* __ARCH_ARM64_SPRD_MACRO_H__ */
