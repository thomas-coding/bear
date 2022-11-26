
#ifndef __BOARD_CLOCK_H__
#define __BOARD_CLOCK_H__

#include "platform.h"

#define PERI_CLKA_SEL (0x50065050)
#define CLOCK_DIV_ONE (0x0)
#define CLOCK_DIV_OFFSET (3)
#define CLOCK_SEL_MOSC (0x2)

void board_clock_init(void);

#endif /* __BOARD_CLOCK_H__ */
