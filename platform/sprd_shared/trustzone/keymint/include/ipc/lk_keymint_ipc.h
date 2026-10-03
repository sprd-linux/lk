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


#ifndef LK_KEYMINT_IPC_H
#define LK_KEYMINT_IPC_H

#include <trusty/trusty_dev.h>
#include <trusty/trusty_ipc.h>
#include <trusty/util.h>
#include <ipc/keymint_ipc.h>
#include <lk_keymint_messages.h>

#define KEYMINT_RECV_BUF_SIZE 4 * KM_IPC_SIZE
#define KEYMINT_SEND_BUF_SIZE \
          (2 * KM_IPC_SIZE - sizeof(struct keymaster_message) - 16 /* tipc header */)

// Initialize trusty device and trusty ipc device.
void km_ipc_init(void);

// Establish IPC connection between KM client and KM server.
int km_ipc_connect(void);

// Close the IPC connection between KM client and KM server.
void km_ipc_disconnect(void);

// Release the resources of trusty device and trusty ipc device.
void km_ipc_shutdown(void);

// Send and receive ipc data.
int lk_keymint_call(uint32_t cmd,
                    void* req_buffer, uint32_t req_size,
                    uint8_t* rsp_buffer, uint32_t* rsp_size);

// Process ipc data.
keymaster_error_t lk_keymint_send(struct km_ipc_task* ipc_task,
                                  void* req_buffer, void* rsp_buffer);


#endif /* LK_KEYMINT_IPC_H */