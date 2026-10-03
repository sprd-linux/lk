/*
 * Copyright (C) 2021 Spreadtrum Communications Inc.
 * AUTHOR: Changmao Guo <changmao.guo@unisoc.com>
 *
 */

#ifndef __ARCH_CACHE_H
#define __ARCH_CACHE_H

void	icache_disable(void);
void	dcache_disable(void);
void	enable_caches(void);
void	flush_cache   (unsigned long, unsigned long);
void	flush_dcache_all(void);
void	flush_dcache_range(unsigned long start, unsigned long stop);
void	invalidate_dcache_range(unsigned long start, unsigned long stop);
void	invalidate_dcache_all(void);
void	invalidate_icache_all(void);
void	v7_inval_tlb(void);
void	mmu_page_table_flush(unsigned long start, unsigned long stop);
#endif

