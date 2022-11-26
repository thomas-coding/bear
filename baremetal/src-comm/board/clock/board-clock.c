
#include "platform.h"
#include "board-clock.h"

void board_clock_init(void)
{
	/* Set clock from MOSC 24M */
	*(volatile uint32_t *)(PERI_CLKA_SEL) = (CLOCK_DIV_ONE << CLOCK_DIV_OFFSET) | CLOCK_SEL_MOSC;

}
