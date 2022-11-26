
#ifndef __BOARD_RESET_H__
#define __BOARD_RESET_H__

#include "platform.h"

#define BLK_SWR0 (0x5006506c)
#define BLK_SWR0_UART0_PRSTN (13)
#define BLK_SWR0_UART0_SRSTN (14)
#define BLK_SWR0_UART1_PRSTN (15)
#define BLK_SWR0_UART1_SRSTN (16)
#define BLK_SWR0_UART2_PRSTN (17)
#define BLK_SWR0_UART2_SRSTN (18)
#define BLK_SWR0_UART3_PRSTN (19)
#define BLK_SWR0_UART4_SRSTN (20)

void board_reset_init(void);
#endif /* __BOARD_RESET_H__ */
