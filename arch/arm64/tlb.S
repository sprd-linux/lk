/*
 * Copyright (C) 2021 Spreadtrum Communications Inc.
 * AUTHOR: Changmao Guo <changmao.guo@unisoc.com>
 *
 */

#include <arch/arm64/sprd_macro.h>
#include <lk/asm.h>

/*
 * void __arm64_invalidate_tlb_all(void)
 *
 * invalidate all tlb entries.
 */
FUNCTION(__arm64_invalidate_tlb_all)
	switch_el x9, 3f, 2f, 1f
3:	tlbi	alle3
	dsb	sy
	isb
	b	0f
2:	tlbi	alle2
	dsb	sy
	isb
	b	0f
1:	tlbi	vmalle1
	dsb	sy
	isb
0:
	ret
