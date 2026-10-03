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

#include <lk_keymint_device.h>

#define LOCAL_LOG 0

static int trusty_km_version = 2;

static void forward_command(struct km_ipc_task* ipc_task,
                            void* req_buffer, void* rsp_buffer) {
    keymaster_error_t err;
    int rc;

    rc = km_ipc_connect();
    if (rc != 0) {
        trusty_error("Initlializing Trusty Keymint client failed (%d)\n", rc);
        return;
    }

    err = lk_keymint_send(ipc_task, req_buffer, rsp_buffer);
    if (err != KM_ERROR_OK) {
        trusty_error("Cmd: %d returned error: %d\n", ipc_task->cmd, ipc_task->error);
    }

    km_ipc_disconnect();
}

static int32_t message_version(uint8_t major_ver,
                              uint8_t minor_ver,
                              uint8_t subminor_ver) {
    int32_t message_version = -1;
    switch (major_ver) {
    case 0:
        message_version = 0;
        break;
    case 1:
        switch (minor_ver) {
        case 0:
            message_version = 1;
            break;
        case 1:
            message_version = 2;
            break;
        }
        break;
    case 2:
        message_version = 3;
        break;
    }
    return message_version;
}

static void km_get_version(int32_t* version) {
    void* req = NULL;
    struct km_get_version_rsp rsp;

    struct km_ipc_task ipc_task = {
        .cmd = KM_GET_VERSION,
        .serialized_size = km_empty_serialized_size,
        .serialize = km_empty_serialize,
        .deserialize = km_get_version_deserialize,
        .error = KM_ERROR_UNKNOWN_ERROR,
    };

    forward_command(&ipc_task, req, &rsp);

    if(ipc_task.error != KM_ERROR_OK) {
        trusty_error("Failed to get TA version.");
        return;
    }

    *version = message_version(rsp.major_ver, rsp.minor_ver, rsp.subminor_ver);
    return;
}

void km_initialize(void) {
    km_ipc_init();

    int32_t version = -1;
    km_get_version(&version);
    trusty_info("Get keymint version %d\n", version);
    if (version < trusty_km_version) {
        trusty_error("keymint version mismatch. Expected %d, received %d\n",
                     trusty_km_version, version);
        return;
    }

    uint32_t boot_patchlevel = 0;
#ifdef SPRD_VBOOT_V2
    get_boot_patch_level(&boot_patchlevel);
#endif
    km_config_boot_patchlevel(boot_patchlevel);

    // km_ipc_shutdown()

    return;
}

void km_config_boot_patchlevel(uint32_t boot_patchlevel) {
    struct km_boot_patchlevel_req req;
    void* rsp = NULL;

    req.boot_patchlevel = boot_patchlevel;

    struct km_ipc_task ipc_task = {
        .cmd = KM_CONFIGURE_BOOT_PATCHLEVEL,
        .serialized_size = km_boot_patchlevel_serialized_size,
        .serialize = km_boot_patchlevel_serialize,
        .deserialize = km_empty_deserialize,
        .error = KM_ERROR_UNKNOWN_ERROR,
    };

    forward_command(&ipc_task, &req, rsp);

    return;
}