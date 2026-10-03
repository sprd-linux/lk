#ifndef __FB_SPARSE_H_
#define __FB_SPARSE_H_


#include <lk/list.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>
#include <linux/types.h>
#include <part_efi.h>
#include <sprd_sizes.h>


#define WR_BUF_MAX	 SZ_8M		/* write buffer size */
#define WR_LMAX_LIMIT	(SZ_8M)			/* write buffer max */
#define WR_UNP_CK_MAX	128
typedef struct {
	struct list_head node;
	__le16 type;						/* unprocess chunk type */
	u64 offset;							/* current offset on flush which next to be write */
	u64 chunk_len;						/* length of this chunk */
	u32 fill_val;						/* fill data if chunk type is FILL */
} wr_unp_ck_t;
typedef struct {
	u8 wr_addr[WR_BUF_MAX];				/* write buffer 8M */
	u8 wr_tmp[WR_BUF_MAX];				/* temp buffer  8M*/
	wr_unp_ck_t cks[WR_UNP_CK_MAX];		/* unprocess chunk nodes 128*/
	char pname[PARTNAME_SZ];			/* unprocessed partition name */
	u64 wr_offset;						/* offset of flash for write buffer */
	u32 wr_len;							/* length of data in write buffer */
	u32 wr_lmax;						/* max length limit of write buffer */
	int free_cnt;						/* free chunks nodes available */
} wr_sparse_mgt_t;


#ifdef CONFIG_WR_SPARSE
#define wr_dbg(fmt, args...) do {\
		dprintf(INFO, "%s(): ", __func__);\
		dprintf(INFO, fmt, ##args);\
	} while (0)
#define wr_raw_dbg(fmt, args...) //wr_dbg
#define wr_fill_dbg wr_dbg
#define wr_dontcare_dbg wr_dbg

int wr_sparse_raw(char *pname, u64 chunk_len, u64 offset, char *buf);
int wr_sparse_fill(char *pname, s16 chunk_type, u64 size, u64 offset,
    u32 fill_val);
int wr_sparse_dontcare(char *pname, u64 size, u64 offset);
int wr_sparse_flush(void);
void wr_sparse_rest(char *pname);
int wr_sparse_prepare(u8 *base_address, u64 *max_size);

#ifdef CONFIG_WRBG_SPARSE
    int wrbg_sparse_flush(char *pname);
    static int wrbg_sparse_raw(wr_sparse_mgt_t *mgt, char *pname, u64 size, u64 offset, char *buf);
    static void wrbg_sparse_rest_alt(void);
#endif

#endif

#endif /*__FB_SPARSE_H_*/

