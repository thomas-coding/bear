
#include "platform.h"
#include "board-reset.h"

void board_reset_init(void)
{
	/* reset module */
	*(volatile uint32_t *)(BLK_SWR0) &= ~((1 << BLK_SWR0_UART0_PRSTN) | (1 << BLK_SWR0_UART0_SRSTN) \
					      | (1 << BLK_SWR0_UART1_PRSTN) | (1 << BLK_SWR0_UART1_SRSTN) \
					      | (1 << BLK_SWR0_UART2_PRSTN) | (1 << BLK_SWR0_UART2_PRSTN) \
					      | (1 << BLK_SWR0_UART3_PRSTN) | (1 << BLK_SWR0_UART4_SRSTN));

	/* release module */
	*(volatile uint32_t *)(BLK_SWR0) |= (1 << BLK_SWR0_UART0_PRSTN) | (1 << BLK_SWR0_UART0_SRSTN) \
					    | (1 << BLK_SWR0_UART1_PRSTN) | (1 << BLK_SWR0_UART1_SRSTN) \
					    | (1 << BLK_SWR0_UART2_PRSTN) | (1 << BLK_SWR0_UART2_PRSTN) \
					    | (1 << BLK_SWR0_UART3_PRSTN) | (1 << BLK_SWR0_UART4_SRSTN);
}
