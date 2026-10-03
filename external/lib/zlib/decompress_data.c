
#include <stdio.h>
#include <string.h>
#include "sprd_common.h"
#include <zlib.h>



/* decompress zlib file */
int decompress_data(void *in, int inlen, void *out, int outlen)
{
    int ret;
    int have = 0;
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return ret;
    }

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = inlen;
        if (strm.avail_in <= 0) {
            dprintf(INFO, "[%s: %d] strm.avail_in <= 0 \n",__FUNCTION__, __LINE__);
            break;
        }

        strm.next_in = (Bytef*)in;
        //dprintf(INFO, "[%s: %d] have decompressed data size = %d\n",__FUNCTION__, __LINE__, have);
        /* run inflate() on input until output buffer not full */

        do {
            //dprintf(ALWAYS, "[%s: %d] have = %d\n",__FUNCTION__, __LINE__, have);
            strm.avail_out = outlen;
            strm.next_out = (Bytef*)out;
            strm.next_out += have; //Update storage address
            //dprintf(INFO, "[%s: %d] strm.next_out = %d\n",__FUNCTION__, __LINE__, strm.next_out);
            ret = inflate(&strm, Z_NO_FLUSH);
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                dprintf(ALWAYS, "[%s: %d] Z_DATA_ERROR or Z_MEM_ERROR\n",__FUNCTION__, __LINE__);
                (void)inflateEnd(&strm);
                return ret;
            }
            have += outlen - strm.avail_out;
            /*
            dprintf(INFO, "[%s: %d] outlen = %d\n",__FUNCTION__, __LINE__, outlen);
            dprintf(INFO, "[%s: %d] strm.avail_out = %d\n",__FUNCTION__, __LINE__, strm.avail_out);
            dprintf(INFO, "[%s: %d] have = %d\n",__FUNCTION__, __LINE__, have);
            */
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);
    /* clean up and return */
    (void)inflateEnd(&strm);
    dprintf(INFO, "[%s: %d] decompress data size = %d\n",__FUNCTION__, __LINE__, have);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
    //return have;  //support return decompress data size
}
