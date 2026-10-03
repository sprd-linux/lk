
#include <sprd_common.h>
#include <malloc.h>
#include <zlib.h>
#include <linux/compiler.h>

#define MALLOC_ALIGNMENT	16
#define SPRD_HEAD_CRC		2
#define SPRD_EXTRA_FIELD		4
#define SPRD_ORIG_NAME		8
#define SPRD_COMMENT			0x10
#define SPRD_RESERVED		0xe0
#define SPRD_DEFLATED		8

void *sprdgzmalloc(void *x, unsigned items, unsigned size)
{
	void *p;

	size *= items;
	size = (size + MALLOC_ALIGNMENT - 1) & ~(MALLOC_ALIGNMENT - 1);

	p = malloc (size);

	return (p);
}

void sprdgzfree(void *x, void *addr, unsigned nb)
{
	free (addr);
}

/*
 * Uncompress blocks compressed with gzip file.
 */
int zunzip(void *dst, int dstlen, unsigned char *src,
			unsigned long *lenp, int stoponerr, int offset)
{
	z_stream s;
	int err = 0;
	int r;

	s.zalloc = sprdgzmalloc;
	s.zfree = sprdgzfree;

	r = inflateInit2(&s, -MAX_WBITS);
	if (r != Z_OK) {
		dprintf(INFO, "Error: inflateInit2() returned %d\n", r);
		return -1;
	}
	s.next_in = src + offset;
	s.avail_in = *lenp - offset;
	s.next_out = dst;
	s.avail_out = dstlen;
	do {
		r = inflate(&s, Z_FINISH);
		if (stoponerr == 1 && r != Z_STREAM_END &&
		    (s.avail_out == 0 || r != Z_BUF_ERROR)) {
			dprintf(INFO, "Error: inflate() returned %d\n", r);
			err = -1;
			break;
		}
		s.avail_in = *lenp - offset - (int)(s.next_out - (unsigned char*)dst);
	} while (r == Z_BUF_ERROR);
	*lenp = s.next_out - (unsigned char *) dst;
	inflateEnd(&s);

	return err;
}

int gunzip(void *dst, int dstlen, unsigned char *src, unsigned long *lenp)
{
	int i, flags;

	/* skip header */
	i = 10;
	flags = src[3];
	if (src[2] != SPRD_DEFLATED || (flags & SPRD_RESERVED) != 0) {
		dprintf(INFO, "Error: Bad gzipped data\n");
		return (-1);
	}
	if ((flags & SPRD_EXTRA_FIELD) != 0)
		i = 12 + src[10] + (src[11] << 8);
	if ((flags & SPRD_ORIG_NAME) != 0)
		while (src[i++] != 0)
			;
	if ((flags & SPRD_COMMENT) != 0)
		while (src[i++] != 0)
			;
	if ((flags & SPRD_HEAD_CRC) != 0)
		i += 2;
	if (i >= *lenp) {
		dprintf(INFO, "Error: gunzip out of data in header\n");
		return (-1);
	}

	return zunzip(dst, dstlen, src, lenp, 1, i);
}

