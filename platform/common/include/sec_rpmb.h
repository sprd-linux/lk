#ifndef SEC_RPMB_H_
#define SEC_RPMB_H_


/*
*@buf  The data read from RPMB
*@len   The buffer length must be 256
*@blk_ind rpmb reserve block number [1 - 13]
*Return value: zero is ok
*you must call uboot_set_rpmb_size() berfor using this api.
*/
int sec_rpmb_read(void *buf, int len, int blk_ind);

/*
*@buf  The data to write to RPMB
*@len   The buffer length must be 256
*@blk_ind rpmb reserve block number [1 - 13]
*Return value: zero is ok
*you must call uboot_set_rpmb_size() berfor using this api.
*/
int sec_rpmb_write(void *buf, int len, int blk_ind);

/*
*@buf  The Secure Write Protect Configuration Block  write to RPMB
*@len   The buffer length must be 256
*Return value: zero is ok
*
*/
int sec_rpmb_swp_config_write(void *buf, int len);

/*
*@buf  The Secure Write Protect Block Configuration read from RPMB
*@len   The buffer length must be 256
*Return value: zero is ok
*/
int sec_rpmb_swp_config_read(void *buf, int len);

#endif
