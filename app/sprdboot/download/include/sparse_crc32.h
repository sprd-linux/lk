/*
 * <sparse_crc32.h>
 * Copyright (C) 2021 Unisoc Communications Inc.
 * History:
 * 	<2021> <sparse_crc32.h>
 *	Verify function for sparse image download, included by sparse_crc32.c.
 */

u32 sparse_crc32(u32 crc, const void *buf, size_t size);
