#ifndef __SPRD_DDR_DVFS_H
#define __SPRD_DDR_DVFS_H
u8 get_dram_info(void);
u32 get_freq_vol(u32 fn);
int dmc_freq_sel_search(u32 ddr_clk, u32* fn);
int ddr_boot_freq_voltage_set(u32 vol);
void sw_dvfs_go(u32 fn);
void ddr_paras_set_to_sp(u32 dvfs_dis, u32 retention_dis, u32* mask_mode);

#endif
