
#ifndef __BOARD_STOP_H__
#define __BOARD_STOP_H__

#include "platform.h"

#define BLK_STOP0 (0x50065074)
#define BLK_STOP0_UART0 (17)
#define BLK_STOP0_UART1 (18)
#define BLK_STOP0_UART2 (19)
#define BLK_STOP0_UART3 (20)

void board_stop_init(void);

#endif /* __BOARD_STOP_H__ */
