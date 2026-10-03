/*
 * Copyright (C) 2016 Spreadtrum Communications Inc.
 */

#ifndef _DDR_MEMTEST_TYPES_H_
#define _DDR_MEMTEST_TYPES_H_

#include <sprd_common.h>
#include <linux/types.h>
#include <asm/arch/common.h>
#include <rand.h>

typedef unsigned long ul;
typedef unsigned long long ull;
typedef unsigned long volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;

#define sprd_ddr_rand32() ((unsigned int) rand() | ( (unsigned int) rand() << 16))

#ifndef CONFIG_ARM64
	#define sprd_ddr_rand_ul() sprd_ddr_rand32()
	#define SPRD_DDR_UL_ONEBITS 0xffffffff
	#define SPRD_DDR_UL_LENGTH 32
	#define SPRD_DDR_CHK1 0x55555555
	#define SPRD_DDR_CHK2 0xaaaaaaaa
	#define SPRD_DDR_UL_BYTE(x) ((x | x << 8 | x << 16 | x << 24))
#else
	#define sprd_ddr_rand64() (((ul) sprd_ddr_rand32()) << 32 | ((ul) sprd_ddr_rand32()))
	#define sprd_ddr_rand_ul() sprd_ddr_rand64()
	#define SPRD_DDR_UL_ONEBITS 0xffffffffffffffffUL
	#define SPRD_DDR_UL_LENGTH 64
	#define SPRD_DDR_CHK1 0x5555555555555555
	#define SPRD_DDR_CHK2 0xaaaaaaaaaaaaaaaa
	#define SPRD_DDR_UL_BYTE(x) (((ul)x | (ul)x<<8 | (ul)x<<16 | (ul)x<<24 | (ul)x<<32 | (ul)x<<40 | (ul)x<<48 | (ul)x<<56))
#endif

struct test {
	char *name;
	int (*fp)(ulv *bufa, ulv *bufb, size_t count);
};

typedef struct refresh_t_ {
	ulv *bufa;
	ulv *bufb;
	size_t count;
	int time;
	int use;
	int do_mlock;
}refresh_t, *lprefresh_t;

#define MODE_NORMAL 1
#define MODE_REFRESH 2

#endif
