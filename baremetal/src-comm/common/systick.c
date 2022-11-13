
#include "platform.h"
#include "systick.h"

#define SYSTICK_RELOAD_MAX	0xffffff

#define US_TICK			(SYSTEM_CLOCK / 1000000)
#define MS_TICK			(SYSTEM_CLOCK / 1000)
#define S_TICK			(SYSTEM_CLOCK)

static void systick_tick_delay(u32 tick, u32 time)
{
	u32 ticks = tick;
	u32 times = time;

	while (ticks > SYSTICK_RELOAD_MAX) {
		times *=2;
		ticks /= 2;
	}

	SysTick->LOAD = (u32)(ticks - 1UL);	/* set reload register */
	SysTick->VAL = 0UL;			/* Load the SysTick Counter Value */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
			SysTick_CTRL_ENABLE_Msk;	/* Enable SysTick Timer */

	while (times--) {
		while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0x0)
			;
	}

	SysTick->CTRL = 0x0;			/* Disable SysTick Timer */
}

void systick_udelay(u32 us)
{
	systick_tick_delay(US_TICK, us);
}

void systick_mdelay(u32 ms)
{
	systick_tick_delay(MS_TICK, ms);
}

void systick_sdelay(u32 s)
{
	systick_tick_delay(S_TICK, s);
}
