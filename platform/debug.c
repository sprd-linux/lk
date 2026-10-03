/*
 * Copyright (c) 2008-2015 Travis Geiselbrecht
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */

#include <lk/compiler.h>
#include <lk/debug.h>
#include <lk/trace.h>
#include <serial_sprd.h>

void platform_dputc(char c)
{
	sprd_serial_putc(c);
}

int platform_dgetc(char *c, bool wait)
{
	if (!wait) {
		if (!sprd_rxfifo_cnt()) {
			return -1;
		}
		*c = sprd_serial_getc();
		return 0;
	} else {
		*c = sprd_serial_getc();
		return 0;
	}
}

/* Default implementation of panic time getc/putc.
 * Just calls through to the underlying dputc/dgetc implementation
 * unless the platform overrides it.
 */
__WEAK void platform_pputc(char c) {
    sprd_serial_putc(c);
}

__WEAK int platform_pgetc(char *c, bool wait) {
    return platform_dgetc(c, wait);
}
