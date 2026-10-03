#ifndef _SECURE_EFUSE_H
#define _SECURE_EFUSE_H

//#include <common.h>

#define PROG_OK     0
#define SHA1_ERR   -1
#define AES256_ERR -2
#define SHA256_ERR -3
#define SANSA_EFUSE_PRO_ERR -4
#define SANSA_GET_LCS_ERR   -5
#define SANSA_SET_RMA_ERR   -6
#define SANSA_KCE_PRO_ERR   -7

int secure_efuse_program_native(void);
int secure_efuse_program_sansa(void);

int read_master_key(void);

int sansa_compute_socid(void *socid);
int sansa_get_lcs_cmd(void);
int sansa_set_rma_cmd(void);
int sansa_kce_program(uint32_t *kcePtr);
void sansa_calc_hbk(void);

#endif
