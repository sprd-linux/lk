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

#ifndef LK_KEYMINT_MESSAGES_H_
#define LK_KEYMINT_MESSAGES_H_

#include <hardware/keymint_defs.h>
#include <serializable.h>

/*********************************************
 * Get error code from keymint TA response.
 ********************************************/
bool km_rsp_error_deserialize(keymaster_error_t* err, const uint8_t** buf_ptr, const uint8_t* end);


/***********************************************
 * Some command does't need to transmit data.
 **********************************************/
size_t km_empty_serialized_size(void);
uint8_t* km_empty_serialize(void* content, uint8_t*buf, const uint8_t* end);
bool km_empty_deserialize(void* content, const uint8_t** buf_ptr, const uint8_t* end);


/************************************
 * Used by command KM_GET_VERSION.
 ***********************************/
typedef struct km_get_version_rsp {
    uint8_t major_ver;
    uint8_t minor_ver;
    uint8_t subminor_ver;
} TRUSTY_ATTR_PACKED km_get_version_rsp;

bool km_get_version_deserialize(void* content, const uint8_t** buf_ptr, const uint8_t* end);


/**************************************************
 * Used by command KM_CONFIGURE_BOOT_PATCHLEVEL.
 *************************************************/
typedef struct km_boot_patchlevel_req {
    uint32_t boot_patchlevel;
} TRUSTY_ATTR_PACKED km_boot_patchlevel_req;

size_t km_boot_patchlevel_serialized_size(void);
uint8_t* km_boot_patchlevel_serialize(void* content, uint8_t* buf,  const uint8_t* end);


#endif /* LK_KEYMINT_MESSAGES_H_ */