/*
* Copyright (C) 2021 Spreadtrum Communications Inc.
*
* Use of this source code is governed by a MIT-style
* license that can be found in the LICENSE file or at
* https://opensource.org/licenses/MIT
*/

#ifndef __ASM_ARCH_SCI_HARDWARE_H
#define __ASM_ARCH_SCI_HARDWARE_H

//#define SCI_IOMAP(x)	(SCI_IOMAP_BASE + (x))

#ifndef SCI_ADDR
#define SCI_ADDR(_b_, _o_)                              ( (u32)(_b_) + (_o_) )
#endif

#include <sprd_sizes.h>
#include "regs_adi.h"
#include "sprd_eic.h"
#include "rtc_reg_v3.h"
#include "chip_qogirl6/hardware.h"

#endif
