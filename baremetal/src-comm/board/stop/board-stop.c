
#include "platform.h"
#include "board-stop.h"

void board_stop_init(void)
{
	/* power on module */
	*(volatile uint32_t *)(BLK_STOP0) &= ~((1 << BLK_STOP0_UART0) | (1 << BLK_STOP0_UART1) \
					       | (1 << BLK_STOP0_UART2) | (1 << BLK_STOP0_UART3));
}
