
#include "platform.h"
#include "board-stop.h"

void board_stop_init(void)
{
	/* power on all modules */
	write_mreg32(SYSREG0_BLE_SYS_STOP, 0x0);
	write_mreg32(SYSREG2_BLK_STOP0, 0x0);
	write_mreg32(SYSREG2_BLK_STOP1, 0x0);
}
