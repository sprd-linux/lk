#ifndef _MODEM_COMMON_H_
#define _MODEM_COMMON_H_

#include <config.h>
#include <sprd_common.h>
#include <string.h>


#define MAX_CP_CMDLINE_LEN  (200)

/*cp cmd define */

#define BOOT_MODE        "androidboot.mode"
#define CALIBRATION_MODE "calibration"
#define LTE_MODE         "ltemode"
#define AP_VERSION       "apv"
#define RF_BOARD_ID      "rfboard.id"
#define RF_HW_INFO       "hardware.version"
#define K32_LESS         "32k.less"
#define AUTO_TEST        "autotest"
#define CRYSTAL_TYPE    "crystal"
#define RF_HW_ID       "rfhw.id"
#define MODEM_BOOT_METHOD         "modemboot.method"
#define WCN_CLK_ID      "marlin.clktype"
#define ANDROIDBOOT_HARDWARE		"androidboot.product.hardware.sku"
#define CPCMDLINE       "cpcmdline"
#define PCB_VERSION	"pcb.version"
#define POWER_MODE	"power.from.extern"
#define SPRDBOOT_MODE   "sprdboot.mode"

#ifdef CONFIG_PMIC_CHIP_ID
#define PMIC_CHIP_ID        "androidboot.pmic.chipid"
#endif

#define CHIPUID          "chip.uid"
#define UOB_BOARD_ID        "uobboard.id"

#ifdef CONFIG_WCN_BOARD_ID
#define WCN_BOARD_ID        "wcnboard.id"
#endif

#define INVALID_BOARD_ID 0xFF

#ifdef CONFIG_SC9833
#define RF_CHIP_ID       "rf.type"
#endif
void cp_cmdline_fixup(void);
char *cp_getcmdline(void);

char *bootconfig_get_bootmode(void);
char *bootconfig_get_chipid(void);

#endif // _MODEM_COMMON_H_

