#ifndef _TRUSTZONE_OPERATE_H_
#define _TRUSTZONE_OPERATE_H_

int uboot_set_root_of_trust(unsigned long start_addr, uint32_t lenth);
int uboot_config_os_version(unsigned long start_addr, uint32_t lenth);
int uboot_verify_product_sn_signature(unsigned long start_addr, uint32_t lenth);
int uboot_vboot_verify_img(unsigned long start_addr, uint32_t lenth);
int uboot_verify_img(unsigned long start_addr, uint32_t lenth);
int uboot_update_swVersion(uint64_t start_addr, uint64_t lenth);
int uboot_vboot_set_ver(unsigned long start_addr, uint32_t lenth);
int uboot_get_hbk(uint64_t start_addr, uint64_t lenth);
int uboot_write_hbk(uint64_t start_addr, uint64_t lenth);

#endif
