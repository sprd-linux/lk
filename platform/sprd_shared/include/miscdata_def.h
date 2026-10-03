/*
 *MISCDATA_DEF_H - use field defined on partition of 'miscdata'
 *
 *  Copyright (C) 2021 Unisoc Communications Inc.
 *  History:
 *      Tue Feb 9 10:10:36 2021 zhenxiong.lai
 *      Description
 *
 */

#ifndef __MISCDATA_DEF_H__
#define __MISCDATA_DEF_H__

/*
 *Function:Verified Boot                                                     8K~8K+512
 *
 *Function:For uboot to read miscdata                                        9K~9K+1K
 *  Configure uboot to read loglevel                                         9K~9K+32
 *  Configure uboot to read the boot mode                                    9K+32~9K+64
 *  Store sysdump switch                                                     9K+64~9K+74
 *  Store the switch for uboot to read the selinux permission check          9k+352~9k+384
 *  Store the virtual boardid to match dtbo                                  9k+384~9k+388
 *  Configure the number of CPUs to be booted by kernel                      9k+388~9k+390
 *  Configure the authentication enable of the flag bit in calibration mode  9k+390~9k+394
 *  Write download failure information to the download flag                  9k+640~9k+672
 *  For uboot to save timestamps                                             9K+768~9K+800
 *  Record the watchdog enable menu                                          9K+800~9K+832
 *  Record the watchdog deep sleep enable menu                               9K+832~9K+864
 *  Enable uart baudrate and configurate parameters                          9K+864~9K+896
 *  Record usb_pin_mux configuration menu                                    9K+896~9K+912
 *  Record ddr_debug configuration menu                                      9K+912~9K+938
 *  Record apcpu_dvfs debug configuration menu                               9K+938~9K+942
 *
 *Function:Customer defined area                                             768K~1024K
 */
#define MISCDATA_VERIFIED_BOOT		(8 * 1024)
#define PDT_INFO_LOCK_FLAG_OFFSET 	(MISCDATA_VERIFIED_BOOT)
#define PDT_INFO_LOCK_FLAG_SECTION_SIZE	(512 - 36)

#define PDT_INFO_DMVERITY_FLAG_OFFSET	(PDT_INFO_LOCK_FLAG_OFFSET + 476)
#define PDT_INFO_DMVERITY_FLAG_SECTION_SIZE	(32 + 4)

/** UBOOT dedicated area */
#define MISCDATA_UBOOT_BASE			(9 * 1024)

/* loglevel */
#define DEBUG_INFO_OFFSET			MISCDATA_UBOOT_BASE
#define DEBUG_INFO_LEN    			(32)

/* first mode */
#define SET_FIRST_MODE_OFFSET 		(MISCDATA_UBOOT_BASE + 32)
#define SET_FIRST_MDOE_LEN 			(0x4)
#define SET_FIRST_MODE_MAGIC 		(0x53464d00)

/* sysdump */
#define MISCDATA_DUMP_DATA_START 	 (MISCDATA_UBOOT_BASE + 64)///fixme
#define FULLDUMP_PARTITION_MAGIC_LEN (10)

/*selinux flag*/
#define SELINUX_SWITCH_OFFSET (MISCDATA_UBOOT_BASE + 352)
#define SELINUX_INFO_LEN    32

/* virtual board id */
#define SET_VIRTUAL_BOARD_ID_OFFSET	(MISCDATA_UBOOT_BASE + 384)
#define SET_VIRTUAL_BOARD_ID_LEN	(0x4)
#define SET_VIRTUAL_BOARD_ID_MAGIC	(0x5AA50000)

/* core0 startup flag */
#define CORE_STARTUP_FLAG_OFFSET	(MISCDATA_UBOOT_BASE + 388)
#define CORE_STARTUP_FLAG_LEN		(0x2)

/* mode startup block flags */
#define STARTUP_BLOCK_FLAG_OFFSET	(MISCDATA_UBOOT_BASE + 390)
#define STARTUP_BLOCK_FLAG_LEN		(0x4)

/* enter download flags */
#define DL_PROCESS_OFFSET		(MISCDATA_UBOOT_BASE + 640)
#define DL_PROCESS_LEN		(0x20)

/* timestamp offset on partition miscdata */
#define SET_TIMESTAMP_OFFSET		(MISCDATA_UBOOT_BASE + 768)
#define SET_TIMESTAMP_MAGIC		(0x5445537e)


/*wdt enable flags */
#define WDTEN_DATA_OFFSET		(MISCDATA_UBOOT_BASE + 800)
#define WDTEN_DATA_LEN			(0x20)
#define WDTEN_MAGIC			(0xe551)

/*deepsleep wdt enable flags */
#define DSWDTEN_DATA_OFFSET             (MISCDATA_UBOOT_BASE + 832)
#define DSWDTEN_DATA_LEN		(0x20)

/*baudrate config flags */
#define CONS_BAUDRATE_OFFSET            (MISCDATA_UBOOT_BASE + 864)
#define CONS_BAUDRATE_LEN       (0x20)

/*usb_pin_mux config flags */
#define USBMUX_DATA_OFFSET		(MISCDATA_UBOOT_BASE + 896)
#define USBMUX_DATA_LEN			(0x10)
#define USBMUX_USB_DEFAULT		(0x0)
#define USBMUX_JTAG			(0xe)
#define USBMUX_UART			(0x4)
#define USBMUX_JTAG_APWDG		(0x1)
#define USBMUX_UNASSIGNED		(0xf)

/* ddr debug config paras*/
#define MISCDATA_DDR_DEBUG_OFFSET	(MISCDATA_UBOOT_BASE + 912)
#define MISCDATA_DDR_DEBUG_LEN		(0x1A)

/* apcpu_dvfs debug config flags */
#define DVFS_SET_OFFSET			(MISCDATA_UBOOT_BASE + 938)
#define DVFS_SET_LEN			(0x4)

/* usr base */
#define MISCDATA_USR_BASE  		(512 * 1024)

/* ADC data storage */
#define ADC_DATA_OFFSET  		(MISCDATA_USR_BASE)
#define ADC_DATA_START  		(ADC_DATA_OFFSET)

/* save the creation time of pac */
#define DATETIME_OFFSET 		(MISCDATA_USR_BASE + 5120)
#define DATETIME_LEN			(0x400)

/* hvb rollback config flags */
#ifdef SPRD_VBOOT_HVB
#define MISCDATA_CUSTOMER_OFFSET (768 * 1024)
#define HVB_ROLLBAKC_OFFSET		(MISCDATA_CUSTOMER_OFFSET + 8)
#define HVB_ROLLBAKC_LEN		(0xc0)
#endif

#endif /* __MISCDATA_DEF_H__ */

