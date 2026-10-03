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

#ifndef __DEV_INTERRUPT_ARM_GICV3_H
#define __DEV_INTERRUPT_ARM_GICV3_H

#include <sys/types.h>

void arm_gic_init(void);

enum {
	/* Ignore cpu_mask and forward interrupt to all CPUs other than the current cpu */
	ARM_GIC_SGI_FLAG_TARGET_FILTER_NOT_SENDER = 0x1,
	/* Ignore cpu_mask and forward interrupt to current CPU only */
	ARM_GIC_SGI_FLAG_TARGET_FILTER_SENDER = 0x2,
	ARM_GIC_SGI_FLAG_TARGET_FILTER_MASK = 0x3,

	/* Only forward the interrupt to CPUs that has the interrupt configured as group 1 (non-secure) */
	ARM_GIC_SGI_FLAG_NS = 0x4,
};

status_t arm_gic_sgi(u_int irq, u_int flags, u_int cpu_mask);

#if WITH_LIB_SM
void arm_gic_terminal (void);
#endif

#endif

