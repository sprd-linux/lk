#ifndef _FASTBOOT_H_
#define _FASTBOOT_H_
void fastboot_register(const char *prefix, void (*handle) (const char *arg, void *data, uint64_t sz));
void fastboot_publish(const char *name, const char *value);
int set_fastboot_buf_base_size(void);
int do_fastboot(void);
int wait_for_keypress(void);
void fail_and_enter_fastboot_mode(void);
#ifdef SPRD_SECBOOT
void fb_cmd_getsocid(const char *arg, void *data, uint64_t sz);
void fb_cmd_getlcs(const char *arg, void *data, uint64_t sz);
#endif
#endif
