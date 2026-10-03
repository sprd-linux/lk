#ifndef __SPRD_DDR_MEMTEST_H__
#define __SPRD_DDR_MEMTEST_H__

#define DDR_MEMTEST_LENGTH (0x140000000 - 0x1000)

int ddr_memtester(ulong base_addr, ulong test_len, ulong is_loop);

#endif