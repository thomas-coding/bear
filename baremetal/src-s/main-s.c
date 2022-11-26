
/* Use CMSE intrinsics */
#include "arm_cmse.h"
#include "platform.h"
#include "common.h"
#include "board-mpu.h"
#include "board-ns16550.h"
#include "data-type.h"
#include "t-ns16550.h"

#include "board-pinmux.h"
#include "board-clock.h"
#include "board-stop.h"
#include "board-reset.h"

extern void tiny_uart_console(void);

void *verify_callback[][2] = {
	{(void *)tiny_uart_console,	"S Tiny console"},
	{ (void *)ns16550_test, "S Uart NS16550 test"																 },
	{ 0,			0																		 },
};

void board_init(void) {
	board_stop_init();
	board_reset_init();
	board_clock_init();
	board_pinmux_init();

	console_init();
}

/* Secure main() */
int main(void)
{
	//FIXME
	//secure_MPU_init();

	vs_printf("\nBare-metal Secure world test begin ...\n");

	vs_verify_loop();

	vs_printf("\nBare-metal Secure world test end\n");
}
