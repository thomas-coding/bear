
/* Use CMSE intrinsics */
#include "arm_cmse.h"
#include "platform.h"
#include "common.h"

#include "board-mpu.h"
#include "board-ns16550.h"
#include "data-type.h"

#include "t-ns16550.h"


extern u32 __DETERMINE_ADDR;
extern void tiny_uart_console(void);

void *verify_callback[][2] = {
	{(void *)ns16550_test,		"S Uart NS16550 test"},
	{0, 0},
};

/* Secure main() */
int main(void) {
	u32 *sync_addr = &__DETERMINE_ADDR;
	//FIXME
	//secure_MPU_init();

	console_init();

	vs_printf("\nBare-metal Secure world test begin ...\n");

	vs_verify_loop();

	vs_printf("\nBare-metal Secure world test end\n");
}
