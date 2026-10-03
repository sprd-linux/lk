/*
 * Copyright (C) 2013 Spreadtrum Communications Inc.
 *
 */


extern void write_sysdump_before_boot(int rst_mode);

extern int save_minidump(int reset_mode, char *reason);
