#ifndef __SPRD_EFUSE_H_
#define __SPRD_EFUSE_H_

u32 sprd_ap_efuse_read(int blk_index);
int sprd_ap_efuse_prog(int blk_index, u32 val);
int sprd_efuse_double_prog(u32 blk, bool backup, bool lock, u32 val);
u32 sprd_efuse_double_read(int blk, bool backup);
u32 sprd_pmic_efuse_read(int blk_index);
u32 sprd_pmic_efuse_read_bits(int bit_index, int length);
void pmic_efuse_block_dump(void);
void ap_efuse_block_dump(void);
void sprd_uid_bin_info(void);
int sprd_get_chip_uid(char *buf);
int sprd_get_chip_hex_uid(char *buf);
int sprd_secure_efuse_read(u32 start_id, u32 end_id, u32 *pReadData,u32 Isdouble);
void ap_sansa_efuse_prog_power_on(void);
void ap_sansa_efuse_power_off(void);
void sansa_enable_efuse_EB(void);

#endif
