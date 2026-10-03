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

#include <serializable.h>

bool __buffer_bound_check(const uint8_t* buf, const uint8_t* end, size_t len) {
    uintptr_t buf_next = (uintptr_t)buf + len;

    bool overflow_occurred = (len > UINTPTR_MAX - (uintptr_t)buf) ? true : false;
    return (!overflow_occurred) && (buf_next <= (uintptr_t)end);
}

uint8_t* km_append_to_buf(uint8_t* buf, const uint8_t* end, const void* data, size_t data_len) {
    if (__buffer_bound_check(buf, end, data_len)) {
        trusty_memcpy(buf, data, data_len);
        return buf + data_len;
    } else {
        return buf;
    }
}

bool km_copy_from_buf(const uint8_t** buf_ptr, const uint8_t* end, void* dest, size_t size) {
    if (__buffer_bound_check(*buf_ptr, end, size)) {
        trusty_memcpy(dest, *buf_ptr, size);
        *buf_ptr += size;
        return true;
    } else {
        return false;
    }
}



