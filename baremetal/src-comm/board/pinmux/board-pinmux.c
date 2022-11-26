
#include "common.h"
#include "platform.h"
#include "board-pinmux.h"

void board_pinmux_init(void) {

	//pinmux uart0
	*(volatile uint32_t *)(0x500000f4) = 0x00000220;
	*(volatile uint32_t *)(0x500000f8) = 0x00000220;
	*(volatile uint32_t *)(0x500000fc) = 0x00000220;
	*(volatile uint32_t *)(0x50000100) = 0x00000220;

	//pinmux uart3
	*(volatile uint32_t *)(0x50000120) = 0x00000320;
	*(volatile uint32_t *)(0x50000124) = 0x00000320;
	*(volatile uint32_t *)(0x50000128) = 0x00000320;
	*(volatile uint32_t *)(0x50000130) = 0x00000320;
}
