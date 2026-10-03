/*
 * Copyright (C) 2016 Spreadtrum Communications Inc.
 */

#ifndef _DDR_MEMTEST_INTERFACE_H_
#define _DDR_MEMTEST_INTERFACE_H_

#include "ddr_memtest_types.h"

int ddr_mem_test(ulong dram_base, ulong test_len, ulong is_loop);

#endif
