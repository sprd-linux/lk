/*
* Copyright (c) 2016, Spreadtrum Communications.
*
* The above copyright notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef KEYMINT_SERIALIZABLE_H_
#define KEYMINT_SERIALIZABLE_H_

#include <trusty/sysdeps.h>
#include <sys/types.h>

/**
 * Performs an overflow-checked bounds check.
 * Returns true if |buf| + |len| is less than |end|.
 */
bool __buffer_bound_check(const uint8_t* buf, const uint8_t* end, size_t len);


/*******************************************************
 * Utility functions for writing Serialize() methods.
 ******************************************************/

/**
 * Appends |data_len| bytes at |data| to |buf|.
 * Performs no bounds checking, assumes sufficient memory allocated at |buf|.
 * Returns |buf| + |data_len|.
 */
uint8_t* km_append_to_buf(uint8_t* buf, const uint8_t* end, const void* data, size_t data_len);


/**
 * Appends |val| to |buf|. Performs no bounds checking.
 * Returns |buf| + sizeof(uint32_t).
 */
inline uint8_t* km_append_uint32_to_buf(uint8_t* buf, const uint8_t* end, uint32_t val) {
    return km_append_to_buf(buf, end, &val, sizeof(val));
}


/*********************************************************
 * Utility functions for writing Deserialize() methods.
 *********************************************************/

/**
 * Copy |size| bytes from |*buf_ptr| into |dest|.
 * If there are fewer than |size| bytes to read, returns false.
 * Advances |*buf_ptr| to the next byte to be read.
 */
bool km_copy_from_buf(const uint8_t** buf_ptr, const uint8_t* end, void* dest, size_t size);

/**
 * Copies a |value| convertible from uint32_t from |*buf_ptr|.
 * Returns false if there are less than four bytes remaining in |*buf_ptr|.
 * Advances |*buf_ptr| to the next byte to be read.
 */
inline bool km_copy_uint32_from_buf(const uint8_t** buf_ptr, const uint8_t* end, uint32_t* value) {
    if (!km_copy_from_buf(buf_ptr, end, value, sizeof(uint32_t))) return false;
    return true;
}


#endif /* KEYMINT_SERIALIZABLE_H_ */