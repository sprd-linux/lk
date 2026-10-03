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

#include <lk_keymint_messages.h>

bool km_rsp_error_deserialize(keymaster_error_t* err, const uint8_t** buf_ptr, const uint8_t* end) {
    if(!err || !buf_ptr || !end) {
        return false;
    }

    uint32_t error;
    if (!km_copy_uint32_from_buf(buf_ptr, end, &error)) {
        return false;
    }

    *err = (keymaster_error_t)error;
    return true;
}

size_t km_empty_serialized_size(void) {
    return 0;
}

uint8_t* km_empty_serialize(void* content, uint8_t* buf, const uint8_t* end) {
    return buf;
}

size_t km_empty_deserialized_size(void) {
    return 0;
}

bool km_empty_deserialize(void* content, const uint8_t** buf_ptr, const uint8_t* end) {
    return true;
}

bool km_get_version_deserialize(void* content, const uint8_t** buf_ptr, const uint8_t* end) {
    if(!content || !buf_ptr || !end) {
        return false;
    }

    struct km_get_version_rsp* data = (struct km_get_version_rsp*)content;
    const uint8_t* tmp = *buf_ptr;
    data->major_ver = *tmp++;
    data->minor_ver = *tmp++;
    data->subminor_ver = *tmp++;
    *buf_ptr = tmp;

    content = (void*)data;

    return true;
}

size_t km_boot_patchlevel_serialized_size(void) {
    struct km_boot_patchlevel_req data;
    size_t size = 0;
    size = sizeof(data.boot_patchlevel);

    return size;
}

uint8_t* km_boot_patchlevel_serialize(void* content, uint8_t* buf, const uint8_t* end) {
    struct km_boot_patchlevel_req* data = (struct km_boot_patchlevel_req*)content;

    return km_append_uint32_to_buf(buf, end, data->boot_patchlevel);
}






