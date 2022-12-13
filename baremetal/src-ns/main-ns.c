
/* Use CMSE intrinsics */
#include "arm_cmse.h"
#include "platform.h"
#include "common.h"
#include "board-mpu.h"
#include "board-ns16550.h"
#include "data-type.h"
#include "t-ns16550.h"
#include "t-qspi-bm-flash.h"
#include "t-qspi-flash.h"

#include "board-pinmux.h"
#include "board-clock.h"
#include "board-stop.h"
#include "board-reset.h"

#include "call_secure.h"

#ifdef INCLUDE_MBEDTLS
#include "t-mbedtls.h"
#endif

extern void tiny_uart_console(void);
extern void qspi_bm_flash_test(void);
extern void crypto_bm_test(void);

void *verify_callback[][2] = {
	{ (void *)tiny_uart_console,  "NS Tiny console"										      },
	{ (void *)ns16550_test,	      "NS Uart NS16550 test"									      },
	{ (void *)qspi_bm_flash_test, "NS qspi baremetal flash test"								      },
	{ (void *)qspi_flash_test,    "NS qspi flash test"									      },
	{ (void *)crypto_bm_test,     "NS crypto baremeatl test"									      },
	{ (void *)non_secure_call_secure,     "NS call secure function test"									      },
#ifdef INCLUDE_MBEDTLS
	{ (void *)mbedtls_test,	      "NS mbedtls test"										      },
#endif
	{ 0,			      0												      },
};

void board_init(void)
{
	board_stop_init();
	board_reset_init();
	board_clock_init();
	board_pinmux_init();

	console_init();
}
#include "veneers.h"
/* Secure main() */
int main(void)
{
	//FIXME
	//secure_MPU_init();
	/* Some config maybe only can access by secure */
	//board_init();
	vs_printf("\nBare-metal Non secure world test begin ...\n");

	vs_verify_loop();

	vs_printf("\nBare-metal Non Secure world test end\n");
}
