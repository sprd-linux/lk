#ifndef __SPRD_BOARDID_H__
#define __SPRD_BOARDID_H__

int sprd_get_bandinfo(void);
int sprd_get_crystal(void);
int sprd_get_32k(void);
int sprd_get_boardid(void);
int sprd_get_wcn_crystal(void);
int sprd_get_wifi_mode(void);
int sprd_get_data_mode(void);
char sprd_get_sim(void);
int sprd_get_power_mode(void);
int sprd_get_uob_boardid(void);
#ifdef CONFIG_WCN_BOARD_ID
int sprd_get_wcn_boardid(void);
#endif
#endif
