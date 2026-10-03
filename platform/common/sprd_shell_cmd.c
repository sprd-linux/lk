/*
 * Copyright (C) 2013 Spreadtrum Communications Inc.
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */

#include <lk/debug.h>
#include <lk/err.h>
#include <lk/compiler.h>
#include <lk/console_cmd.h>
#include <platform.h>
#include <platform/debug.h>
#include <kernel/thread.h>
#include <stdio.h>
#if WITH_LIB_CONSOLE
#include <lib/console.h>
#endif
#include <boot_mode.h>

#ifdef CONFIG_UFS_RPMB_WRITE_PROTECT
#include <part_efi.h>
#include <sprd_common.h>
#include <sprd_sizes.h>
#include <malloc.h>
#include <sprd_ufs.h>
#include <sprd_rpmb.h>

enum command_ret_t {
	CMD_RET_SUCCESS,	/* 0 = Success */
	CMD_RET_FAILURE,	/* 1 = Failure */
	CMD_RET_USAGE = -1,	/* Failure, please report 'usage' error */
};

extern int common_raw_read(const char *part_name, uint64_t size, uint64_t offset, char *buf);
extern int common_raw_write(const char *part_name, uint64_t size,
				uint64_t updsize, uint64_t offset, char *buf);
#endif

extern void fastboot_mode(void);
extern void recovery_mode(void);

 static int cmd_fastboot(int argc, const cmd_args *argv) {
    fastboot_mode();
    return 0;
}

static int cmd_recovery(int argc, const cmd_args *argv) {
    recovery_mode();
    return 0;
}

static int cmd_normal_boot(int argc, const cmd_args *argv) {
    normal_mode();
    return 0;
}

#ifdef CONFIG_UFS_RPMB_WRITE_PROTECT
cmd_args *argv_global;
int argc_global;

int test_ufs_set_swp(int argc, const cmd_args *argv)
{
	int ret = 0;
	uint64_t bytes = 0;
	uint64_t offset = 0;
	argc_global = argc;
	argv_global = argv;
	errorf("%s:%d\n",__func__,__LINE__);
	errorf("%s:%d:%s\n",__func__,__LINE__,argv[2].str);
	bytes = (uint64_t) simple_strtoul(argv[2].str, NULL, 16);
	errorf("%s:%d\n",__func__,__LINE__);

	offset = (uint64_t)simple_strtoul(argv[1].str, NULL, 16);
	errorf("%s:%d\n",__func__,__LINE__);

	debugf("set swp entry part(%s), offset=0x%llx, bytes=0x%llx,\n", argv[0].str, offset, bytes);
//	ret = storage_write_protect_set(argv[0].str, offset, bytes);
	ret = storage_write_protect_set("boot_b", 0, 0x1000);
	if (-1 == ret)
		errorf("set swp entry fail!\n");
	errorf("%s:%d\n",__func__,__LINE__);

	return ret;
}

int test_ufs_remove_swp(int argc, const cmd_args *argv)
{
	int ret = 0;
	uint64_t bytes = 0;
	uint64_t offset = 0;
	bytes = (uint64_t) simple_strtoul(argv[2].str, NULL, 16);

	offset = (uint64_t)simple_strtoul(argv[1].str, NULL, 16);

	debugf("remove swp entry part(%s), offset=0x%llx, bytes=0x%llx,\n",
		argv[0].str, offset, bytes);
	ret = storage_write_protect_remove("boot_b", 0, 0x1000);
	if (-1 == ret)
		errorf("remove swp entry fail!\n");

	return ret;
}

int test_ufs_check_swp(int argc, const cmd_args *argv)
{
	int ret = 0;
	uint64_t bytes = 0;
	uint64_t offset = 0;
	bytes = (uint64_t) simple_strtoul(argv[2].str, NULL, 16);

	offset = (uint64_t)simple_strtoul(argv[1].str, NULL, 16);

	debugf("check swp entry part(%s), offset=0x%llx, bytes=0x%llx,\n",
		argv[0].str, offset, bytes);
	ret = storage_write_protect_check("boot_b", 0, 0x1000);
	if (0 == ret)
		debugf("This entry is protected!\n");

	if (1 == ret)
		debugf("This entry is not protected!\n");

	if (-1 == ret)
		debugf("check swp entry fail!\n");

	return 0;
}

int test_ufs_read(int argc, const cmd_args *argv)
{
	int ret = 0;
	uint64_t size = 0;
	uint64_t offset = 0;
	int i =0;
	char* buf = malloc(SZ_32K);
	if (NULL == buf) {
		errorf("Malloc for read buf fail\n");
		return CMD_RET_FAILURE;
	}
	memset(buf, 0, SZ_32K);

	size = (uint64_t) simple_strtoul(argv[1].str, NULL, 16);
	if (size > SZ_32K) {
		errorf("Read size cannot over 32KB!\n");
		goto fail;
	}
	offset = (uint64_t)simple_strtoul(argv[2].str, NULL, 16);

	debugf("read part(%s), size=0x%llx, offset=0x%llx\n", argv[0].str, size, offset);
	ret = common_raw_read("boot_b",  0x100,0, buf);
	if (-1 == ret) {
		errorf("common_raw_read fail!\n");
		goto fail;
	}

	debugf("Read result:");
	for (i = 0; i < size; i += 4) {
		if (0 == i % 16)
			printf("\n");
		printf("%08x ", *(uint32_t *)(buf + i));
	}
	printf("\n");

	free(buf);
	return 0;
fail:
	free(buf);
	return CMD_RET_FAILURE;
}

int test_ufs_write(int argc, const cmd_args *argv)
{
	int ret = 0;
	uint64_t size = 0;
	uint64_t offset = 0;
	int i =0;
	int value = 0;
	char* buf = malloc(SZ_32K);
	if (NULL == buf) {
		errorf("Malloc for write buf fail\n");
		return CMD_RET_FAILURE;
	}

	size = (uint64_t) simple_strtoul(argv[1].str, NULL, 16);
	if (size > SZ_32K) {
		errorf("Write size cannot over 32KB!\n");
		goto fail;
	}
	offset = (uint64_t)simple_strtoul(argv[2].str, NULL, 16);

	value = (int)simple_strtoul(argv[3].str, NULL, 16);
	memset(buf, 'a', size);

	debugf("write part(%s), size=0x%llx, offset=0x%llx, value=0x%x\n",
		argv[0].str, size, offset, value);
	ret = common_raw_write("boot_b", 0x100, 0, 0, buf);
	if (-1 == ret) {
		errorf("common_raw_write fail!\n");
		goto fail;
	}
	debugf("Write OK\n");

	memset(buf, 0, SZ_32K);
	free(buf);
	return 0;
fail:
	free(buf);
	return CMD_RET_FAILURE;

}
#endif

STATIC_COMMAND_START
STATIC_COMMAND("fastboot", "entry fastboot mode", &cmd_fastboot)
STATIC_COMMAND("recovery", "entry recovery mode", &cmd_recovery)
STATIC_COMMAND("boot", "entry normal boot mode", &cmd_normal_boot)

#ifdef CONFIG_UFS_RPMB_WRITE_PROTECT
STATIC_COMMAND("test_ufs_set_swp", "test_ufs_set_swp", &test_ufs_set_swp)
STATIC_COMMAND("test_ufs_remove_swp", "test_ufs_remove_swp", &test_ufs_remove_swp)
STATIC_COMMAND("test_ufs_check_swp", "test_ufs_check_swp", &test_ufs_check_swp)
STATIC_COMMAND("test_ufs_read", "test_ufs_read", &test_ufs_read)
STATIC_COMMAND("test_ufs_write", "test_ufs_write", &test_ufs_write)
/*
please using cmd:
 test_ufs_set_swp boot_b 0 100 1 11 1
 test_ufs_check_swp boot_b 0 100 1 11 1
 test_ufs_write boot_b 0 100 1 11 1
 test_ufs_read boot_b 0 100 1 11 1
 test_ufs_remove_swp boot_b 0 100 1 11 1
 test_ufs_check_swp boot_b 0 100 1 11 1
 **/
#endif

STATIC_COMMAND_END(sprd_shlle_cmd);

