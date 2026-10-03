/*
 *  vectors - Generic ARM exception table code
 *
 *  Copyright (c) 2022	changmao.guo <changmao.guo@unisoc.com>
 *
 */

.globl _start_hyp

/*
 * Vectors have their own section
 */

.section ".text.boot.vectab1"

/*
 * Exception vectors as described in ARM reference manuals
 */
.align 5
_start_hyp:
	ldr     pc, _handle_hyp
	ldr     pc, _handle_hyp
	ldr     pc, _handle_hyp
	ldr     pc, _handle_hyp
	ldr     pc, _handle_hyp
	ldr     pc, _irq_hyp
	ldr     pc, _handle_hyp

/*
 * Indirect vectors table
 */
_handle_hyp:	.word handle_hyp
_irq_hyp:		.word irq_hyp
	.balignl 16,0xdeadbeef

/*
 * Interrupt handling
 */
#define S_FRAME_SIZE	72

handle_hyp:
	b	arm_hyp_exception

irq_hyp:
	sub		sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12,r14}
	bl		arm_irq
	ldmia	sp, {r0 - r12,r14}
	add		sp, sp, #S_FRAME_SIZE
	eret

