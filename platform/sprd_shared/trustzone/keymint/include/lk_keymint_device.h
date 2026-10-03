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

#ifndef LK_KEYMINT_DEVICE_H_
#define LK_KEYMINT_DEVICE_H_

#include <interface/keymint.h>
#include <ipc/lk_keymint_ipc.h>
#include <lk_keymint_messages.h>
#include <secureboot/sec_common.h>

/**
 * Send 'boot patch level' value to KM server.
 * The 'boot patch level' is stored in the vbmeta image.Everytime the device
 * is powered on, the image is parsed by AVB in the bootloader phase, and
 * the AVB provides an interface for KM to obatin 'boot patch level'.
 */
void km_config_boot_patchlevel(uint32_t boot_patchlevel);


#endif /* LK_KEYMINT_DEVICE_H_ */