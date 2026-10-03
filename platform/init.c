/*
 * Copyright (c) 2008 Travis Geiselbrecht
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
#include <lk/err.h>
#include <lk/debug.h>
#include <platform.h>
#include <lk/reg.h>
#include <kernel/vm.h>
#include <platform/uart.h>
#include <chipram_env.h>
#include <sprd_types.h>

#define UART_BASE (UART1)
#define UARTREG(reg)  (*REG32((UART_BASE) + (reg)))

/* initial memory mappings. parsed by start.S */
#if WITH_KERNEL_VM
struct mmu_initial_mapping mmu_initial_mappings[] = {
	 { .phys = 0,
	   .virt =0,
	   .size = 0x80000000UL,
	   .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
	   .name = "Peripheral" },

	 { .phys = PHYS_SDRAM_1,
	   .virt = PHYS_SDRAM_1,
	   .size = (PHYS_SDRAM_1_SIZE + 0x40000000), //only used for >2G
	   .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
	   .name = "bank-0" },

#ifdef SPRD_DUAL_DDR
	{ .phys = PHYS_SDRAM_2,
	  .virt = PHYS_SDRAM_2,
	  .size = (PHYS_SDRAM_2_SIZE),
	  .flags = 0,
	  .name = "bank-1" },
#endif

	 /* null entry to terminate the list */
	 { 0 }
};

static pmm_arena_t ram_arena = {
    .name  = "ram",
    .base  =  MEMBASE,
    .size  =  MEMSIZE,
    .flags =  PMM_ARENA_FLAG_KMAP
};
#endif

phys_size_t real_ram_size = 0x40000000;
phys_size_t get_real_ram_size(void)
{
        return real_ram_size;
}

int dram_init(void)
{
#ifdef SPRD_DDR_AUTO_DETECT
	chipram_env_t * env = CHIPRAM_ENV_LOCATION;

	if (CHIPRAM_ENV_MAGIC != env->magic) {
		errorf("Chipram magic wrong , ddr data may be broken\n");
		return 0;
	}

	real_ram_size = 0;

	if (env->cs_number == 1) {
		real_ram_size += env->cs0_size;
		dprintf(INFO, "dram cs0 size %llx\n", env->cs0_size);
	} else if(env->cs_number == 2){
		real_ram_size += env->cs0_size;
		real_ram_size += env->cs1_size;
		dprintf(INFO, "dram cs0 size %llx\n", env->cs0_size);
		dprintf(INFO, "dram cs1 size %llx\n", env->cs1_size);
	}
#else
	real_ram_size = REAL_SDRAM_SIZE;
#endif

	return 0;
}

/*
 * default implementations of these routines, if the platform code
 * chooses not to implement.
 */

__WEAK void platform_init_mmu_mappings(void) {
#if WITH_KERNEL_VM
	pmm_add_arena(&ram_arena);
#else
	dram_init();
#endif
}

#if WITH_SMP
extern void smp_boot();
extern int psci_call(ulong arg0, ulong arg1, ulong arg2, ulong arg3);
#endif
uint32_t lk_start_time;
extern int sprd_timer_init(void);
__WEAK void platform_early_init(void) {
    lk_start_time = SCI_GetTickCount();

	sprd_timer_init();
	dprintf(INFO, "booting %d cpus\n", (uint)SMP_MAX_CPUS);
}

extern void setup_chipram_env(void);
__WEAK void platform_init(void) {
    dprintf(INFO,"myqin-UARTREG(0)=%d\n",UARTREG(0));
	setup_chipram_env();
}

__WEAK void platform_quiesce(void) {
}
