#include <sprd_common.h>
#include <sprd_cpcmdline.h>
#include <boot_mode.h>
#include <sprd_boardid.h>
#include "boot_parse.h"

char *g_CPcmdlineBuf = NULL;
#if defined( CONFIG_KERNEL_BOOT_CP )
char CPcmdlineBuf[MAX_CP_CMDLINE_LEN];
#endif
extern int sprd_get_pcbversion(void);

static char bootconfig_mode_return[64];

static char bootconfig_chipid_return[64];

static const char *cmd_arr[] = {
	BOOT_MODE,
	CALIBRATION_MODE,
	LTE_MODE,
	AP_VERSION,
	RF_BOARD_ID,
	RF_HW_INFO,
	K32_LESS,
	AUTO_TEST,
	CRYSTAL_TYPE,
	RF_HW_ID,
#ifdef CONFIG_SC9833
	RF_CHIP_ID,
#endif
#ifdef CONFIG_PMIC_CHIP_ID
	PMIC_CHIP_ID,
#endif
	CHIPUID,
	MODEM_BOOT_METHOD,
	WCN_CLK_ID,
	PCB_VERSION,
	ANDROIDBOOT_HARDWARE,
	CPCMDLINE,
	POWER_MODE,
	SPRDBOOT_MODE,
	UOB_BOARD_ID,
#ifdef CONFIG_WCN_BOARD_ID
	WCN_BOARD_ID,
#endif
	NULL
};

static bool is_invalid_cmd(const char *cmd)
{
	int i = 0;
	while(NULL != cmd_arr[i])
	{
		if(0 == strcmp(cmd_arr[i], cmd)){
			return true;
		}
		i++;
	}
	return false;
}

static void cpcmdline_get_lte_mode(char *value)
{
#ifdef CONFIG_SUPPORT_TDLTE
	strcpy(value, "tcsfb");
#elif defined CONFIG_SUPPORT_WLTE
	strcpy(value, "fcsfb");
#elif defined CONFIG_SUPPORT_LTE
	strcpy(value, "lcsfb");
#endif
}

static void cpcmdline_get_modem_boot_method(char *value)
{
#ifdef CONFIG_DDR_BOOT
	strcpy(value, "ddrboot");
#elif defined CONFIG_EMMC_BOOT
	strcpy(value, "emmcboot");
#elif defined CONFIG_NAND_BOOT
	strcpy(value, "nandboot");
#endif
}


static void cmdline_prepare(void)
{
#if defined( CONFIG_KERNEL_BOOT_CP )
	g_CPcmdlineBuf = CPcmdlineBuf;
#else
#ifdef CONFIG_MEM_LAYOUT_DECOUPLING
extern void *parse_cpcmdline_addr(void);
	g_CPcmdlineBuf = (char*)parse_cpcmdline_addr();
#else
#ifdef CALIBRATION_FLAG_CP0
	g_CPcmdlineBuf = (char*)CALIBRATION_FLAG_CP0;
#endif
#ifdef CALIBRATION_FLAG_CP1
	g_CPcmdlineBuf = (char*)CALIBRATION_FLAG_CP1;
#endif
#endif
#endif
	if (g_CPcmdlineBuf)
		memset(g_CPcmdlineBuf, 0, MAX_CP_CMDLINE_LEN);
	debugf("g_CPcmdlineBuf = 0x%p\n" , g_CPcmdlineBuf);
}

static void cmdline_add_cp_cmdline(const char *cmd, const char* value)
{
	char *p;
	int len;
	printf("add cmd, cmd = %s, value = %s\n", cmd, value);

	if(!is_invalid_cmd(cmd))return;

	if (NULL == g_CPcmdlineBuf)
		return;

	len = strlen(g_CPcmdlineBuf);
	p = g_CPcmdlineBuf + len;

	snprintf(p, MAX_CP_CMDLINE_LEN - len, "%s=%s ", cmd, value);
	printf("cmd = %s\n" , p);
}

char *bootconfig_get_bootmode()
{
	return bootconfig_mode_return;
}

char *bootconfig_get_chipid()
{
	return bootconfig_chipid_return;
}

static void fixup_bootmode(char *value)
{
#ifndef CONFIG_BOOTCONFIG
                cmdline_add_cp_cmdline(BOOT_MODE, value);
#else
                memset(bootconfig_mode_return, 0, sizeof(bootconfig_mode_return));
                cmdline_add_cp_cmdline(SPRDBOOT_MODE, value);
                sprintf(bootconfig_mode_return, "androidboot.mode=%s\n", value);
#endif
		return;
}

static void fixup_pmic_chipid(void)
{
        char buf[30]={0};
#ifdef CONFIG_PMIC_CHIP_ID
        /* get pmic chipid */
        extern int sprd_get_pmic_chipid(void);

        unsigned int pmicid = sprd_get_pmic_chipid();
        sprintf(buf,"%x",pmicid);

#ifndef CONFIG_BOOTCONFIG
        cmdline_add_cp_cmdline(PMIC_CHIP_ID, buf);
#else
       	memset(bootconfig_chipid_return, 0, sizeof(bootconfig_chipid_return));
        sprintf(bootconfig_chipid_return,"androidboot.pmic.chipid=%s\n",buf);
#endif
#endif
        return;
}

void cp_cmdline_fixup(void)
{
	char val[30] = {0};
	int boardid = 0;
	int value_less = 0;
	char buf[30] = {0};
	int uob_boardid = 0;
	int value_datamode = 0;
	char value_simmode = 0;
	int value_wifi_gpio = 0;
	char *value;
	int wcn_boardid = 0;

	cmdline_prepare();

	// androd boot mode
	value = g_env_bootmode;
	if (NULL != value)
		fixup_bootmode(value);
	// calibration parameters
	value = get_calibration_parameter();
	if (NULL != value) {
		if (NULL != strstr(value, CALIBRATION_MODE)) {
			// "calibration=%d,%d,146 ", must skipp calibration=
			value += strlen(CALIBRATION_MODE) + 1;
			cmdline_add_cp_cmdline(CALIBRATION_MODE, value);
		} else if (NULL != strstr(value, AUTO_TEST)) {
			// "autotest=1", must skipp autotest=
			value += strlen(AUTO_TEST) + 1;
			cmdline_add_cp_cmdline(AUTO_TEST, value);
		}
	}

	// lte mode
	memset(val, '\0', sizeof(val));
	cpcmdline_get_lte_mode(val);
	if (strlen(val))
		cmdline_add_cp_cmdline(LTE_MODE, val);

	//chip uid
#ifdef CONFIG_READ_UID
	{
		unsigned int block0;
		unsigned int block1;
		get_efuse_uid_ex(&block0,&block1);
		sprintf(buf,"0x%08x,0x%08x",block0,block1);
		cmdline_add_cp_cmdline(CHIPUID,buf);
	}
#endif
/* crystal and rf band auto adaption used on some platforms, such as sharkl, pike series */
/* Using RF band adaption in each board configuration and print information in cpcmdline*/
#if (defined(CONFIG_BOARD_ID) || defined(CONFIG_BAND_DETECT))
	//rf band auto adaption
	boardid = sprd_get_bandinfo();
	sprintf(buf,"%d",boardid);
	cmdline_add_cp_cmdline(RF_BOARD_ID, buf);
#endif
#ifdef CONFIG_ANDROIDBOOT_HARDWARE
	value_wifi_gpio = sprd_get_wifi_mode();
	value_datamode = sprd_get_data_mode();
	value_simmode = sprd_get_sim();

	if( value_wifi_gpio == 1 ) {
		if( value_datamode ==1 || value_simmode == 1 ) {
			debugf("error config: value_wifi_gpio = %d, value_datamode = %d, value_simmode = %d \n",
			value_wifi_gpio, value_datamode, value_simmode);
		}
		else {
			sprintf(buf,"%s","wifionly");
			cmdline_add_cp_cmdline(ANDROIDBOOT_HARDWARE, buf);
		}
	}
	else if( value_datamode ==1 || value_simmode == 1 ) {
		if(value_datamode ==1 && value_simmode == 1)
			sprintf(buf,"%s","dataonly_singlesim");
		else
			sprintf(buf,"%s",(value_simmode?"singlesim":"dataonly"));

		cmdline_add_cp_cmdline(ANDROIDBOOT_HARDWARE, buf);
	}
#endif/* Printing the clock and crystal type in cpcmdline by clk & crystal adaption */
#ifdef CONFIG_UOB_BOARD_ID
	uob_boardid = sprd_get_uob_boardid();
	if (snprintf(buf, sizeof(buf), "%d", uob_boardid)) {
		cmdline_add_cp_cmdline(UOB_BOARD_ID,buf);
	}
	else {
		printf("%s %s line %d has error.\n uob_boardid = %d\n", __FILE__, __FUNCTION__, __LINE__, uob_boardid);
	}

#endif
#if (defined(CONFIG_BOARD_ID) || defined(CONFIG_ADIE_SC2730))
	//board id
	value_less=sprd_get_boardid();
	sprintf(buf,"%d",value_less);
	cmdline_add_cp_cmdline(RF_HW_ID, buf);

	//26m crystal auto adaption
	sprintf(buf,"%d", sprd_get_crystal());
	cmdline_add_cp_cmdline(CRYSTAL_TYPE, buf);
#endif
	//32k crystal auto adaption
#if !defined(CONFIG_BOARD_ID) && !defined(CONFIG_32K_TYPE)
	sprintf(buf,"%d", 1);   // 1 K_LESS
#else
	sprintf(buf,"%d", sprd_get_32k());
#endif
	cmdline_add_cp_cmdline(K32_LESS, buf);
#ifdef CONFIG_WCN_DETECT
	//wcn crystal auto adaption
	sprintf(buf,"%d", sprd_get_wcn_crystal());
	cmdline_add_cp_cmdline(WCN_CLK_ID,buf);
#endif
#ifdef CONFIG_PCB_VERSION
	sprintf(buf,"%d",sprd_get_pcbversion());
	cmdline_add_cp_cmdline(PCB_VERSION, buf);
#endif
/* check the power mode thought option/gpio */
#ifdef CONFIG_POWER_MODE
	snprintf(buf,sizeof(buf),"%d", sprd_get_power_mode());
	cmdline_add_cp_cmdline(POWER_MODE, buf);
#endif


#ifdef CONFIG_WCN_BOARD_ID
	wcn_boardid = sprd_get_wcn_boardid();
	sprintf(buf,"%d", wcn_boardid);
	cmdline_add_cp_cmdline(WCN_BOARD_ID,buf);
#endif

#ifdef CONFIG_SC9833
	{
		extern u8 DRV_RF_Get_Type(void);

		u8 type = DRV_RF_Get_Type();
		sprintf(buf, "%d", type);
		cmdline_add_cp_cmdline(RF_CHIP_ID, buf);
	}
#endif

	fixup_pmic_chipid();
	// modem boot method
	memset(val, '\0', sizeof(val));
	cpcmdline_get_modem_boot_method(val);
	if(strlen(val))
	{
		cmdline_add_cp_cmdline(MODEM_BOOT_METHOD, val);
	}
	cmdline_add_cp_cmdline(CPCMDLINE,"end");
	debugf("cp cmdline: %s\n", g_CPcmdlineBuf);
}

char *cp_getcmdline(void)
{
	return g_CPcmdlineBuf;
}

