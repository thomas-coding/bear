
#include "platform.h"
#include "board-reset.h"

void board_reset_init(void)
{
	/* Reset all modules */
	//write_mreg32(SYSREG2_BLK_SWR0, 0x0);
	//write_mreg32(SYSREG2_BLK_SWR1, 0x0);

	//mdelay(10);

	/* Release all modules */
	write_mreg32(SYSREG2_BLK_SWR0, 0xffffffff);
	write_mreg32(SYSREG2_BLK_SWR1, 0xffffffff);
}
