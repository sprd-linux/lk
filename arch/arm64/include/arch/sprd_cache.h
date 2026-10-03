/*
 * Copyright (C) 2021 Spreadtrum Communications Inc.
 * AUTHOR: Changmao Guo <changmao.guo@unisoc.com>
 *
 */

#ifndef __ARCH_CACHE_H
#define __ARCH_CACHE_H

void	enable_caches(void);
void	flush_cache   (unsigned long, unsigned long);
void	flush_dcache_all(void);
void	flush_dcache_range(unsigned long start, unsigned long stop);
void	invalid_dcache_range(unsigned long start, unsigned long stop);
void	invalidate_dcache_range(unsigned long start, unsigned long stop);
void	invalidate_dcache_all(void);
void	invalidate_icache_all(void);
void	icache_disable(void);
void	dcache_disable(void);
void	invalidate_icache_all(void);
void	invalidate_dcache_all(void);
void    flush_cache(unsigned long start, unsigned long size);
void    invalid_cache(unsigned long start, unsigned long size);

#if WITH_SMP
void    secondary_cache_enable(void);
#endif

#endif

