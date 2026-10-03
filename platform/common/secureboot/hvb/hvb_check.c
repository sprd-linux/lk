/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <linux/fs.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "hvb_ops.h"
#include "hvb.h"
#include "hvb_sysdeps.h"
#include "hvb_util.h"

#include <libavb.h>
#include <avb_cmdline.h>
#include <avb_sha.h>

#include <sprd_common_rw.h>
#include <secureboot/sec_common.h>
#include <miscdata_def.h>

static struct hvb_ops *g_hvb_ops;


static enum hvb_io_errno valid_rvt_key(struct hvb_ops *ops,
                                          const uint8_t *pubkey,
                                          uint64_t pubkey_length,
                                          const uint8_t *pubkey_metadata,
                                          uint64_t pubkey_metadata_length,
                                          bool *out_is_trusted)
{
    enum hvb_io_errno rret = HVB_IO_OK;
    *out_is_trusted = true;
    char rvt_puk[256] = {
	    0xa2, 0xe3, 0x18, 0xdc, 0xa5, 0x0b, 0x89, 0x9e, 0xbe, 0xcd, 0xf1, 0x2d, 0x61, 0xff, 0x46, 0x7a,
            0x65, 0x20, 0x70, 0x23, 0x7e, 0x45, 0x56, 0xba, 0x66, 0xa4, 0xab, 0x7e, 0x5b, 0xfc, 0x82, 0x46,
            0x5e, 0x36, 0xfe, 0xa3, 0x1b, 0x31, 0xc7, 0xdc, 0x8b, 0x94, 0x1c, 0x21, 0x82, 0xf3, 0x3e, 0xdb,
            0x86, 0xf6, 0xba, 0x8e, 0xec, 0x80, 0xde, 0xf1, 0x98, 0x19, 0x0f, 0x90, 0x77, 0x46, 0xaa, 0x57,
            0x6e, 0xa1, 0x1a, 0xec, 0x20, 0xda, 0x85, 0x9a, 0x30, 0x18, 0xd1, 0xaa, 0xa7, 0x2d, 0x64, 0x29,
            0xc4, 0x16, 0xa2, 0xe3, 0x92, 0x29, 0xc2, 0x4e, 0xe7, 0x62, 0x43, 0xbe, 0x4e, 0x2e, 0x54, 0x6d,
            0x2f, 0x0b, 0x23, 0x56, 0x58, 0xd0, 0xe2, 0x93, 0x1f, 0xa7, 0x6b, 0x82, 0xa2, 0xd0, 0x39, 0x6c,
            0xbf, 0x26, 0x4d, 0xf6, 0xa8, 0x43, 0x20, 0xa6, 0x02, 0x1a, 0x8b, 0x5e, 0x7f, 0xe7, 0x87, 0x89,
            0xcf, 0x2d, 0xe3, 0xb4, 0x96, 0x72, 0xc1, 0x43, 0x53, 0x36, 0xfb, 0xeb, 0x39, 0x8c, 0x7d, 0xcc,
            0x1d, 0xbe, 0xfd, 0xdc, 0x2f, 0x32, 0x52, 0x1d, 0xba, 0x77, 0x7c, 0xc1, 0x31, 0xa0, 0x42, 0x2d,
            0xca, 0x43, 0x1c, 0xe1, 0xed, 0x2c, 0x04, 0xf2, 0xe1, 0xec, 0x2d, 0xc3, 0x4c, 0x5c, 0xfe, 0xfe,
            0xe6, 0x7f, 0xa8, 0x6f, 0x55, 0x28, 0xaf, 0x4a, 0x65, 0x3c, 0x7d, 0xbd, 0x59, 0xe2, 0x8e, 0x4a,
            0x71, 0xa8, 0x1c, 0x2f, 0xae, 0x4c, 0xdc, 0x5f, 0x7f, 0x11, 0x77, 0xaf, 0x67, 0xc0, 0x5e, 0x3e,
            0x68, 0xbc, 0x3a, 0xc1, 0xc0, 0x9a, 0x18, 0x70, 0x75, 0xb8, 0x89, 0x6f, 0x54, 0xb7, 0xf2, 0xb4,
            0x40, 0x6b, 0xcd, 0x8a, 0x9d, 0x1a, 0xc4, 0xec, 0xa2, 0xfa, 0x19, 0x46, 0x3d, 0x0c, 0x25, 0x5d,
            0x3e, 0x28, 0x48, 0x1c, 0xbf, 0x2d, 0xc8, 0x25, 0xc7, 0x0f, 0x6d, 0x74, 0x4b, 0x07, 0x73, 0xd1
    };
    for (size_t i = 16, j=0; i < 272; i++, j++) {
        if (memcmp(rvt_puk[j], *(pubkey + i), 1) != 0) {
            *out_is_trusted = false;
            debugf("rvt pub key failed.\n" );
            return HVB_IO_ERROR_OOM;
	}
    }

    debugf("rvt pub key succeed.\n" );
    return rret;
}

static enum hvb_io_errno hvb_read_partition(struct hvb_ops *ops,
                                            const char *partition_name,
                                            int64_t offset_from_partition,
                                            uint64_t num_bytes,
                                            void *buf,
                                            uint64_t *out_num_read)
{
    int      ret = 0;
    debugf("enter %s, to read partition_name %s.\n", __func__, partition_name);
    //debugf("enter %s, to read partition_name %s, offset_from_partition:0x%llx, num_bytes:0x%llx\n", __func__, partition_name, offset_from_partition, num_bytes);

    if (partition_name == NULL) {
        errorf("error, partition is NULL");
        return HVB_IO_ERROR_IO;
    }

    if (offset_from_partition < 0) {
        uint64_t partition_size;
        ret = get_img_partition_size(partition_name, &partition_size);
        if (0 != ret) {
            errorf("get %s size error, ret:%d \n",partition_name, ret);
            return HVB_IO_ERROR_NO_SUCH_PARTITION;
        }
        offset_from_partition = (partition_size) - (-offset_from_partition);
    }

    ret = common_raw_read_hvb(partition_name, (uint64_t)num_bytes,
                              (uint64_t)offset_from_partition, buf);
    if (0 != ret) {
        errorf("PARTITION %s read error, ret:%d \n", partition_name, ret);
        return HVB_IO_ERROR_IO;
    }
    *out_num_read = num_bytes;
    return HVB_IO_OK;
}

static enum hvb_io_errno hvb_write_rollback(struct hvb_ops *ops,
                                            uint64_t rollback_index_location,
                                            uint64_t rollback_index)
{
    int rollback_index_location_offset;
    int rollback_index_len = sizeof(rollback_index);

    if (( rollback_index_location > 24 ) || ( rollback_index_location < 0 )) {
        errorf("rollback_index_location should be greater than 0 and less than 24.\n");
        return HVB_IO_ERROR_IO;
    }

    rollback_index_location_offset = HVB_ROLLBAKC_OFFSET + rollback_index_location * rollback_index_len;
    if (0 != common_raw_write("miscdata", rollback_index_len, 0,
			    rollback_index_location_offset, &rollback_index)) {
        errorf("write hvb rollback flag fail\n");
        return HVB_IO_ERROR_IO;
    }
    debugf("write hvb rollback flag success, rollback_index:%d\n", rollback_index);

    return HVB_IO_OK;
}

static enum hvb_io_errno hvb_read_rollback(struct hvb_ops *ops,
                                           uint64_t rollback_index_location,
                                           uint64_t *out_rollback_index)
{
    int rollback_index_location_offset;
    int rollback_index_len = sizeof(*out_rollback_index);

    if (( rollback_index_location > 24 ) || ( rollback_index_location < 0 )) {
        errorf("rollback_index_location should be greater than 0 and less than 24.\n");
        return HVB_IO_ERROR_IO;
    }

    rollback_index_location_offset = HVB_ROLLBAKC_OFFSET + rollback_index_location * rollback_index_len;
    if (0 != common_raw_read_hvb("miscdata", (uint64_t)rollback_index_len,
                              (uint64_t)rollback_index_location_offset, out_rollback_index)) {
        errorf("read hvb rollback flag fail\n");
        return HVB_IO_ERROR_IO;
    }

    debugf("read hvb rollback flag success, rollback_index:%d\n", *out_rollback_index);

    return HVB_IO_OK;
}

static enum hvb_io_errno hvb_get_partiton_size(struct hvb_ops *ops,
                                               const char *partition,
                                               uint64_t *out_size_in_bytes)
{
    int ret = 0;
    char partition_name[128] = {0};
    *out_size_in_bytes = 0;

    if (partition == NULL) {
        errorf("error, partition is NULL");
        return HVB_IO_ERROR_IO;
    }

    ret = get_img_partition_size(partition, out_size_in_bytes);

    if (0 != ret) {
        errorf("get %s size error, ret:%d \n",partition, ret);
        return HVB_IO_ERROR_IO;
    }
    return HVB_IO_OK;
}

static enum hvb_io_errno HvbReadLockState(struct hvb_ops* ops,
                                          bool* lock_state)
{
    return HVB_IO_OK;
}

static enum hvb_io_errno HvbWriteToPartition(struct hvb_ops* ops,
                                             const char* partition,
                                             int64_t offset, uint64_t numBytes,
                                             const void* buf)
{
    return HVB_IO_OK;
}

static struct hvb_ops *hvb_ops_user(void)
{
    struct hvb_ops *hvb_ops;

    hvb_ops = (struct hvb_ops *)hvb_malloc(sizeof(struct hvb_ops));
    if (!hvb_ops) {
        errorf("Error malloc for hvb_ops.\n");
        return NULL;
    }

    hvb_ops->user_data = hvb_ops;
    hvb_ops->read_partition = hvb_read_partition;
    hvb_ops->valid_rvt_key = valid_rvt_key;
    hvb_ops->write_partition = HvbWriteToPartition;
    hvb_ops->read_rollback = hvb_read_rollback;
    hvb_ops->write_rollback = hvb_write_rollback;
    hvb_ops->read_lock_state = HvbReadLockState,
    hvb_ops->get_partiton_size = hvb_get_partiton_size;

    return hvb_ops;
}


int hvb_check_image(void)
{
    int ret = 0;
    enum hvb_errno sub_ret = HVB_OK;
    struct hvb_verified_data *verified_data = NULL;
    //const char *hash_ptn_list[] = {"dtbo_a", "l_agdsp_a", "pm_sys_a"};
    const char *hash_ptn_list[] = {"dtbo_a", "pm_sys_a"};

    printf("hvb main function start.\n");
    g_hvb_ops = hvb_ops_user();
    if (!g_hvb_ops) {
        errorf("failed to get hvb_ops_user.\n");
	ret = -1;
        goto out;
    }

    sub_ret = hvb_chain_verify(g_hvb_ops, "rvt_a", hash_ptn_list, &verified_data);
    if (sub_ret != HVB_OK) {
        errorf("hvb verity error.\n");
	ret = -1;
        hvb_chain_verify_data_free(verified_data);
        verified_data = NULL;
        goto out;
    }

    //print cmdlie
    /*struct hvb_cmdline_data cmdline_new;
    cmdline_new = verified_data->cmdline;
    printf("cmdlie buf %s\n", cmdline_new.buf);*/

    printf("hvb main end.\n");
out:
    if (g_hvb_ops != NULL) {
        hvb_free(g_hvb_ops);
    }

    return ret;
}
