/*
 * Copyright (C) 2016 Spreadtrum Communications Inc.
 */

#include "ddr_memtest_types.h"
#include "memtest_interface.h"
#include <asm/arch/common.h>
#include <sprd_common.h>
#include <rand.h>
#include <lcd_console.h>

#define TEST_NARROW_WRITES

char progress[] = "-\\|/";
#define PROGRESSOFTEN 2500
#define ONE 0x00000001L

static int compare_times = 0;
/* Function definitions. */

#define fprintf printf
//#define printf lcd_printf

union {
	unsigned char bytes[SPRD_DDR_UL_LENGTH / 8];
	ul val;
} mword8;

union {
	unsigned short u16s[SPRD_DDR_UL_LENGTH / 16];
	ul val;
} mword16;

static ulv *sr_addr;
static ulv *dst_addr;
typedef long off_tt;
off_tt phys_addrbase = 0x80000000;
int use_phys = 1;

struct memtest {
	char *name;
	int (*fp)(ulv *buffer_1, ulv *buffer_2, size_t cnt);
};

void sprd_ddr_readRepeat(ulv *pointer1, ulv *pointer2, ulv *buffer_1,
			 ulv *buffer_2, size_t cnt, int n)
{
	int i = 0;
	int ret = NULL;

	for (i = 0; i < n; i++) {
		ret = (*pointer1 == *pointer2)?0:-1;
		if (ret) {
			compare_times ++;
		}
	}
}

int sprd_ddr_compare_regions(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	int r = 0;
	size_t i,j;
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	off_tt phys_addr;
	pointer1 = buffer_1;
	pointer2 = buffer_2;

	for (i = 0; i < cnt; i++, pointer1++, pointer2++) {
		if (*pointer1 != *pointer2) {
			//phys_addr = sr_addr + (i * sizeof(ul));
			fprintf("FAILURE: 0x%lx != 0x%lx at physical address 0x%lx and 0x%lx.\n",
				(ul) *pointer1, (ul) *pointer2, (ul) pointer1, (ul) pointer2);
			lcd_printf("FAILURE: 0x%lx != 0x%lx at physical address 0x%lx and 0x%lx.\n",
				   (ul) *pointer1, (ul) *pointer2, (ul) pointer1, (ul) pointer2);
			return -1;
		}
	}

	return r;
}

int sprd_ddr_test_stuck_address(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	unsigned int j;
	size_t i;
	off_tt phys_addr;

	for (j = 0; j < 16; j++) {
		pointer1 = (ulv *) buffer_1;
		for (i = 0; i < cnt; i++) {
			*pointer1 = ((j + i) % 2) == 0 ? (ul) pointer1 : ~((ul) pointer1);
			pointer1++;
		}
		pointer1 = (ulv *) buffer_1;
		for (i = 0; i < cnt; i++, pointer1++) {
			if (*pointer1 != (((j + i) % 2) == 0 ? (ul) pointer1 : ~((ul) pointer1))) {
				//phys_addr = sr_addr + (i * sizeof(ul));
				fprintf("FAILURE: possible bad address line at physical address 0x%lx.\n", (ul) pointer1);
				lcd_printf("FAILURE: possible bad address line at physical address 0x%lx.\n", (ul) pointer1);
				return -1;
			}
		}
	}

	return 0;
}

int sprd_ddr_test_random_value(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;

	for (i = 0; i < cnt; i++) {
		*pointer1++ = *pointer2++ = sprd_ddr_rand_ul();
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_xor_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;
	ul val = sprd_ddr_rand_ul();

	for (i = 0; i < cnt; i++) {
		*pointer1++ ^= val;
		*pointer2++ ^= val;
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_sub_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;
	ul val = sprd_ddr_rand_ul();

	for (i = 0; i < cnt; i++) {
		*pointer1++ -= val;
		*pointer2++ -= val;
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_mul_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;
	ul val = sprd_ddr_rand_ul();

	for (i = 0; i < cnt; i++) {
		*pointer1++ *= val;
		*pointer2++ *= val;
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_div_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;
	ul val = sprd_ddr_rand_ul();

	for (i = 0; i < cnt; i++) {
		if (!val)
			val++;

		*pointer1++ /= val;
		*pointer2++ /= val;
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_or_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;
	ul val = sprd_ddr_rand_ul();

	for (i = 0; i < cnt; i++) {
		*pointer1++ |= val;
		*pointer2++ |= val;
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_and_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;
	ul val = sprd_ddr_rand_ul();

	for (i = 0; i < cnt; i++) {
		*pointer1++ &= val;
		*pointer2++ &= val;
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_seqinc_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	size_t i;
	ul val = sprd_ddr_rand_ul();

	for (i = 0; i < cnt; i++) {
		*pointer1++ = *pointer2++ = (i + val);
	}

	return sprd_ddr_compare_regions(buffer_1, buffer_2, cnt);
}

int sprd_ddr_test_solidbits_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	unsigned int j;
	ul val;
	size_t i;

	for (j = 0; j < 32; j++) {
		val = (j % 2) == 0 ? SPRD_DDR_UL_ONEBITS : 0;
		pointer1 = (ulv *) buffer_1;
		pointer2 = (ulv *) buffer_2;
		for (i = 0; i < cnt; i++) {
			*pointer1++ = *pointer2++ = (i % 2) == 0 ? val : ~val;
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}

int sprd_ddr_test_checkerboard_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	unsigned int j;
	ul val;
	size_t i;

	for (j = 0; j < 32; j++) {
		val = (j % 2) == 0 ? SPRD_DDR_CHK1 : SPRD_DDR_CHK2;
		pointer1 = (ulv *) buffer_1;
		pointer2 = (ulv *) buffer_2;
		for (i = 0; i < cnt; i++) {
			*pointer1++ = *pointer2++ = (i % 2) == 0 ? val : ~val;
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}

int sprd_ddr_test_blockseq_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	unsigned int j;
	size_t i;

	for (j = 0; j < 256; j++) {
		pointer1 = (ulv *) buffer_1;
		pointer2 = (ulv *) buffer_2;
		for (i = 0; i < cnt; i++) {
			*pointer1++ = *pointer2++ = (ul) SPRD_DDR_UL_BYTE(j);
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}

int sprd_ddr_test_walkbits0_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	unsigned int j;
	size_t i;
	for (j = 0; j < SPRD_DDR_UL_LENGTH * 2; j++) {
		pointer1 = (ulv *) buffer_1;
		pointer2 = (ulv *) buffer_2;
		for (i = 0; i < cnt; i++) {
			if (j < SPRD_DDR_UL_LENGTH) { /* Walk it up. */
				*pointer1++ = *pointer2++ = ONE << j;
			} else { /* Walk it back down. */
				*pointer1++ = *pointer2++ = ONE << (SPRD_DDR_UL_LENGTH * 2 - j - 1);
			}
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}

int sprd_ddr_test_walkbits1_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	unsigned int j;
	size_t i;

	for (j = 0; j < SPRD_DDR_UL_LENGTH * 2; j++) {
		pointer1 = (ulv *) buffer_1;
		pointer2 = (ulv *) buffer_2;
		for (i = 0; i < cnt; i++) {
			if (j < SPRD_DDR_UL_LENGTH) { /* Walk it up. */
				*pointer1++ = *pointer2++ = SPRD_DDR_UL_ONEBITS ^ (ONE << j);
			} else { /* Walk it back down. */
				*pointer1++ = *pointer2++ = SPRD_DDR_UL_ONEBITS ^ (ONE << (SPRD_DDR_UL_LENGTH * 2 - j - 1));
			}
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}

int sprd_ddr_test_bitspread_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	unsigned int j;
	size_t i;

	for (j = 0; j < SPRD_DDR_UL_LENGTH * 2; j++) {
		pointer1 = (ulv *) buffer_1;
		pointer2 = (ulv *) buffer_2;
		for (i = 0; i < cnt; i++) {
			if (j < SPRD_DDR_UL_LENGTH) { /* Walk it up. */
				*pointer1++ = *pointer2++ = (i % 2 == 0)
					? (ONE << j) | (ONE << (j + 2))
					: SPRD_DDR_UL_ONEBITS ^ ((ONE << j)
							| (ONE << (j + 2)));
			} else { /* Walk it back down. */
				*pointer1++ = *pointer2++ = (i % 2 == 0)
					? ((ONE << (SPRD_DDR_UL_LENGTH * 2 - 1 - j))
					   | (ONE << (SPRD_DDR_UL_LENGTH * 2 + 1 - j)))
					: SPRD_DDR_UL_ONEBITS ^ (ONE << (SPRD_DDR_UL_LENGTH * 2 - 1 - j)
						| (ONE << (SPRD_DDR_UL_LENGTH * 2 + 1 - j)));
			}
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}

int sprd_ddr_test_bitflip_comparison(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	unsigned int j, k;
	ul val;
	size_t i;

	for (k = 0; k < SPRD_DDR_UL_LENGTH; k++) {
		val = ONE << k;
		for (j = 0; j < 8; j++) {
			val = ~val;
			pointer1 = (ulv *) buffer_1;
			pointer2 = (ulv *) buffer_2;
			for (i = 0; i < cnt; i++) {
				*pointer1++ = *pointer2++ = (i % 2) == 0 ? val : ~val;
			}
			if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
				return -1;
			}
		}
	}

	return 0;
}

void sprd_ddr_switch_for_tiger(ulv *p)
{
	char *r0;
	char *r1;
	char new_r0[4],new_r1[4];
	ulv *p_r = p;
	int i;

#define copy_byte(__x,__y) do{\
    __y = __x;}while(0)

/*Change method:
SW_original:
Word data:  0x77665544
Address 0x4:    7   6   5   4
Byte data:  0x77    0x66    0x55    0x44

Word data:  0x33221100
Address 0x0:    3   2   1   0
Byte data:  0x33    0x22    0x11    0x00

    |
    |
    V

SW_Swap:
Word data:  0x33772266
Address 0x4:    7   6   5   4
Byte data:  0x33    0x77    0x22    0x66

Word data:  0x11550044
Addrees 0x0:    3   2   1   0
Byte data:  0x11    0x55    0x00    0x44

r0_0 -> new_r0_1
r0_1 -> new_r0_3
r0_2 -> new_r1_1
r0_3 -> new_r1_3
r1_0 -> new_r0_0
r1_1 -> new_r0_2
r1_2 -> new_r1_0
r1_3 -> new_r1_2*/

	/*switch p,p-1*/
	r1 = (char *)(p_r--);
	r0 = (char *)p_r;
	/*printf("r1=0x%08lx,*r1=0x%08lx\n",r1,*(ulv*)r1);
	printf("r0=0x%08lx,*r0=0x%08lx\n",r0,*(ulv*)r0);*/
	copy_byte(r0[0],new_r0[1]);
	copy_byte(r0[1],new_r0[3]);
	copy_byte(r0[2],new_r1[1]);
	copy_byte(r0[3],new_r1[3]);
	copy_byte(r1[0],new_r0[0]);
	copy_byte(r1[1],new_r0[2]);
	copy_byte(r1[2],new_r1[0]);
	copy_byte(r1[3],new_r1[2]);
	for(i = 0; i < 4; i++) {
		r0[i] = new_r0[i]; /*write back*/
		r1[i] = new_r1[i];
	}
	/*printf("r1=0x%08lx,*r1=0x%08lx\n",r1,*(ulv*)r1);
	printf("r0=0x%08lx,*r0=0x%08lx\n",r0,*(ulv*)r0);*/
}

int sprd_ddr_test_bitflip_comparison_tiger(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	/*ulv *temp;*/
	unsigned int j, k;
	ul val;
	size_t i;

	//printf("           ");
	if (sizeof(ulv) != 4) {
		printf("This test only support for 32bit Architecture");
		return -1;
	}

	for (k = 0; k < SPRD_DDR_UL_LENGTH; k++) {
		val = ONE << k;
		for (j = 0; j < 8; j++) {
			//printf("\b\b\b\b\b\b\b\b\b\b\b");
			val = ~val;
			//printf("setting %3u", k * 8 + j);
			pointer1 = (ulv *) buffer_1;
			pointer2 = (ulv *) buffer_2;
			for (i = 0; i < cnt; i++) {
				*pointer1++ = *pointer2++ = (i % 2) == 0 ? val : ~val;
				if ((i > 0) && ((i % 2) == 1)) {/*switch for tiger*/
					/* *temp = 0x33221100;
					*pointer1 = 0x77665544;
					//printf("\nbefore switch: 0x%8X 0x%8X\n",*(ulv *)temp,*(ulv *)pointer1);
					//printf("\nbefore switch pointer1 i=%d : 0x%08lX 0x%08lX\n",i,*(ulv *)(pointer1-1),*(ulv *)pointer1);
					//printf("\nbefore switch pointer2 i=%d : 0x%08lX 0x%08lX\n",i,*(ulv *)(pointer2-1),*(ulv *)pointer2);*/
					sprd_ddr_switch_for_tiger(pointer1-1);
					sprd_ddr_switch_for_tiger(pointer2-1);
					/*printf("\nafter switch pointer1 i=%d : 0x%08lX 0x%08lX\n",i,*(ulv *)(pointer1-1),*(ulv *)pointer1);
					printf("\nafter switch pointer2 i=%d : 0x%08lX 0x%08lX\n",i,*(ulv *)(pointer2-1),*(ulv *)pointer2);
				} else {
					temp = pointer1;*/
				}
			}
			//printf("\b\b\b\b\b\b\b\b\b\b\b");
			//printf("testing %3u", k * 8 + j);
			if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
				return -1;
			}
		}
	}

	//printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}


#ifdef TEST_NARROW_WRITES
int sprd_ddr_test_8bit_wide_random(ulv* buffer_1, ulv* buffer_2, size_t cnt)
{
	u8v *pointer1, *t;
	ulv *pointer2;
	int attempt;
	unsigned int b, j = 0;
	size_t i;

	for (attempt = 0; attempt < 2;  attempt++) {
		if (attempt & 1) {
			pointer1 = (u8v *) buffer_1;
			pointer2 = buffer_2;
		} else {
			pointer1 = (u8v *) buffer_2;
			pointer2 = buffer_1;
		}

		for (i = 0; i < cnt; i++) {
			t = mword8.bytes;
			*pointer2++ = mword8.val = sprd_ddr_rand_ul();
			for (b=0; b < SPRD_DDR_UL_LENGTH/8; b++) {
				*pointer1++ = *t++;
			}
			if (!(i % PROGRESSOFTEN)) {
			}
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}

int sprd_ddr_test_16bit_wide_random(ulv* buffer_1, ulv* buffer_2, size_t cnt)
{
	u16v *pointer1, *t;
	ulv *pointer2;
	int attempt;
	unsigned int b, j = 0;
	size_t i;

	for (attempt = 0; attempt < 2; attempt++) {
		if (attempt & 1) {
			pointer1 = (u16v *) buffer_1;
			pointer2 = buffer_2;
		} else {
			pointer1 = (u16v *) buffer_2;
			pointer2 = buffer_1;
		}

		for (i = 0; i < cnt; i++) {
			t = mword16.u16s;
			*pointer2++ = mword16.val = sprd_ddr_rand_ul();
			for (b = 0; b < SPRD_DDR_UL_LENGTH/16; b++) {
				*pointer1++ = *t++;
			}
			if (!(i % PROGRESSOFTEN)) {
			}
		}
		if (sprd_ddr_compare_regions(buffer_1, buffer_2, cnt)) {
			return -1;
		}
	}

	return 0;
}
#endif

int sprd_ddr_compare_regions_tag(ulv *buffer_1, ulv *buffer_2, size_t cnt)
{
	int r = 0;
	size_t i,j;
	ulv *pointer1 = buffer_1;
	ulv *pointer2 = buffer_2;
	off_tt phys_addr;

	for (j = 0; j < 20; j++) {
		pointer1 = buffer_1;
		pointer2 = buffer_2;

		for (i = 0; i < cnt; i++, pointer1++, pointer2++) {
			if (*pointer1 != *pointer2) {
#undef DEBUG_COMPARE
#ifdef DEBUG_COMPARE
				printfTag("0x%08x: 0x%08lx | 0x%08lx\n",i,*pointer1,*pointer2);
#else
				if (use_phys) {
					phys_addr = phys_addrbase + (i * sizeof(ul));
					fprintf("Compare times: 0x%lx.\n", (j+1));
					fprintf("FAILURE: 0x%lx != 0x%lx at physical address 0x%lx.\n",
						(ul) *pointer1, (ul) *pointer2, phys_addr);
				} else {
					fprintf("Compare times: 0x%lx.\n", (j+1));
					fprintf("FAILURE: 0x%lx != 0x%lx at offset 0x%lx.\n",
						(ul) *pointer1, (ul) *pointer2,
						(ul) (i * sizeof(ul)));
				}

				sprd_ddr_readRepeat(pointer1, pointer2, buffer_1, buffer_2, cnt, 20);

				/* printf("Skipping to next test..."); */
				if (compare_times == 0) {
					r = -1;
				}else {
					r = 0; /* complete the last comparison.*/
				}
#endif
			}
		}

		if (compare_times == 0) {
			j = 20; /* do not need compare 20 times; */
		}
	}

	return r;
}

void sprd_ddr_readRepeat_tag(ulv *pointer1, ulv *pointer2, ulv *buffer_1,
			     ulv *buffer_2, size_t cnt, int n_repeat)
{
	int i = 0;
	char *pchar = NULL;

	for(i = 0; i < n_repeat; i++)
	{
		pchar = (*pointer1 == *pointer2)?"=":"!";
		fprintf("ReadRepeat(%d): 0x%lx %s= 0x%lx \n",
			i, (ul) *pointer1, pchar, (ul) *pointer2);
	}
}

int sprd_ddr_compare_regions_const(ulv *buffer_1, size_t cnt, int value)
{
	ulv *pointer1 = buffer_1;
	size_t i;
	int res = 0;
	for (i = 0; i < cnt; i++, pointer1++)
	{
		if (*pointer1 != value)
		{
			res = -1;
		}
	}

	return res;
}

struct memtest tests[] = {
	{ "stuck address", sprd_ddr_test_stuck_address},
	{ "Random Value", sprd_ddr_test_random_value },
	{ "Compare XOR", sprd_ddr_test_xor_comparison },
	{ "Compare SUB", sprd_ddr_test_sub_comparison },
	{ "Compare MUL", sprd_ddr_test_mul_comparison },
	{ "Compare DIV", sprd_ddr_test_div_comparison },
	{ "Compare OR", sprd_ddr_test_or_comparison },
	{ "Compare AND", sprd_ddr_test_and_comparison },
	{ "Compare seqinc", sprd_ddr_test_seqinc_comparison },
	{ "Compare solidbits", sprd_ddr_test_solidbits_comparison },
	{ "Compare checkerboard", sprd_ddr_test_checkerboard_comparison },
	{ "Compare blockseq", sprd_ddr_test_blockseq_comparison },
	{ "Compare walkbits0", sprd_ddr_test_walkbits0_comparison },
	{ "Compare walkbits1", sprd_ddr_test_walkbits1_comparison },
	{ "Compare bitspread", sprd_ddr_test_bitspread_comparison },
	{ "Compare bitflip", sprd_ddr_test_bitflip_comparison },
#ifdef TEST_NARROW_WRITES
	{ "Compare 8bit wide random", sprd_ddr_test_8bit_wide_random },
	{ "Compare 16bit wide random", sprd_ddr_test_16bit_wide_random },
#endif
};

int ddr_mem_test(ulong dram_base, ulong test_len, ulong is_loop)
{
	volatile ulong loop = is_loop;
	ul length, halflen, cnt;
	int regval;

	halflen = test_len / 2;
	sr_addr = (ulv *)dram_base;
	dst_addr = (ulv *) (dram_base + halflen);
	cnt = halflen / sizeof(ul);

	/*test_len div 2 for source and dest*/
	printf("dram_base 0x%lx, source address 0x%lx, dest address 0x%lx,test length 0x%lx, cnt:0x%lx\n",
		dram_base, (ul) sr_addr, (ul) dst_addr, test_len, cnt);
	lcd_printf("dram_base 0x%lx, source address 0x%lx, dest address 0x%lx,test length 0x%lx, cnt:0x%lx\n",
		dram_base, (ul) sr_addr, (ul) dst_addr, test_len, cnt);

	length = sizeof(tests)/sizeof(struct memtest);
	for (regval=0; regval < length; regval++) {
		lcd_printf("%s test\n",tests[regval].name);
		printf("%s test\n",tests[regval].name);
		if (!tests[regval].fp(dst_addr, sr_addr, cnt)) {
			lcd_printf("%s test pass\n",tests[regval].name);
			printf("%s test pass\n",tests[regval].name);
		} else {
			lcd_printf("%s test failed\n",tests[regval].name);
			printf("%s test failed\n",tests[regval].name);
			while (loop);
			return -1;
		}
	}

	return 0;

}

