
#include "platform.h"
#include "board-clock.h"

void board_clock_init(void)
{
	/* Set MOSC, enable output */
	write_mreg32(SYSREG2_MOSC_CFG, MOSC_CLOCK_DEFAULT_CONFIG);

	/* Set MOCO and wait stable */
	write_mreg32(SYSREG2_MOCO_CFG, MOCO_CLOCK_DEFAULT_CONFIG);
	udelay(10);

	/* Set HOCO (24M) and wait stable */
	write_mreg32(SYSREG2_HOCO_CFG, HOCO_CLOCK_DEFAULT_CONFIG);
	udelay(10);

	/* Set PLL0 384M */
	write_mreg32(SYSREG2_PLL0_CFG0, PLLO_DEFAULT_CONFIG);
	udelay(10);

	/* Set PLL1 96M */
	write_mreg32(SYSREG2_PLL1_CFG0, PLL1_DEFAULT_CONFIG);
	udelay(10);

	/* Sys clock (96M) : M33 ,Bus, ROM, SRAM, DMA, CRYP*/
	write_mreg32(SYSREG2_SYS_CLK_SEL, SYS_CLOCK_DEFAULT_CONFIG);

	/* BLE bus clock (96M) : BLE base band, BLE Protocol Engine*/
	write_mreg32(SYSREG2_BLE_BUS_CLK_CFG, BLE_BUS_CLOCK_DEFAULT_CONFIG);

	/* Sys clock (384M) : QSPI*/
	write_mreg32(SYSREG2_QSPI_CORE_CLK_SEL, QSPI_CORE_CLOCK_DEFAULT_CONFIG);

	/* BLE clock (384M), BLE core */
	write_mreg32(SYSREG2_BLE_CLK_SEL, BLE_CORE_CLOCK_DEFAULT_CONFIG);

	/* USB clock (24M), usb phy */
	write_mreg32(SYSREG2_USB_CLK_CFG, USB_CLOCK_DEFAULT_CONFIG);

	/* PERI A clock (24M), UART */
	write_mreg32(SYSREG2_PERI_CLKA_SEL, PERI_CLKA_CLOCK_DEFAULT_CONFIG);

	/* PERI B clock (24M), WDT */
	write_mreg32(SYSREG2_PERI_CLKB_SEL, PERI_CLKB_CLOCK_DEFAULT_CONFIG);

	/* PERI C clock (24M), ADC */
	write_mreg32(SYSREG2_PERI_CLKC_SEL, PERI_CLKC_CLOCK_DEFAULT_CONFIG);

	/* PERI D clock (24M), DAC */
	write_mreg32(SYSREG2_PERI_CLKD_SEL, PERI_CLKD_CLOCK_DEFAULT_CONFIG);

	/* PERI E clock (24M), TIMER32, PWM */
	write_mreg32(SYSREG2_PERI_CLKE_SEL, PERI_CLKE_CLOCK_DEFAULT_CONFIG);

	/* Debounce clock clock (24M), IRQ, ADC, GPIO */
	write_mreg32(SYSREG2_DBCLK_CFG, DEBOUNCE_CLOCK_DEFAULT_CONFIG);

	/* Trace clock (24M), CPU-OCD */
	write_mreg32(SYSREG2_TRACE_CLK_SEL, TRACE_CLOCK_DEFAULT_CONFIG);

	/* CLKOUT clock (12M), CLKOUT pin */
	write_mreg32(SYSREG2_CLK_OUT_SEL, CLOCK_OUT_CLOCK_DEFAULT_CONFIG);

	/* Low power clock (LPCLK) RTC, BLE, SysTick, GPIO, PMU, TSNS, VDT 32.768KHz */

}
