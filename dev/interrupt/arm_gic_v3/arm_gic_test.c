/*just test gic timer feature.*/
#include <dev/interrupt/arm_gic.h>
#include <platform/timer.h>
#include <kernel/timer.h>
#include <kernel/thread.h>
#include <platform/interrupts.h>

static enum handler_return oneshot_timer_test(timer_t *timer, lk_time_t now, void *arg)
{
	printf("oneshot_timer_test:current system clock now:%d\n",now);
	return INT_NO_RESCHEDULE;
}

timer_t timer_test;
void gic_oneshot_timer_test(void)
{
	thread_t *gict = get_current_thread();
	thread_set_real_time(gict);
	dprintf(INFO,"====== gic test in function: %s, in line: %d\n",__FUNCTION__, __LINE__);

	__asm__ volatile("b .");

	timer_initialize(&timer_test);
	timer_set_oneshot(&timer_test, 10, oneshot_timer_test, NULL);
}

