
#ifndef __BOARD_CLOCK_H__
#define __BOARD_CLOCK_H__

#include "platform.h"

#if 0
#define PERI_CLKA_SEL (0x50065050)
#define CLOCK_DIV_ONE (0x0)
#define CLOCK_DIV_OFFSET (3)
#define CLOCK_SEL_MOSC (0x2)
#endif

enum HOCO_CLOCK_FREQ {
	HOCO_16M,
	HOCO_24M,
	HOCO_32M,
	HOCO_48M,
};

enum PLL_REF_CLOCK {
	PLL_REF_MOCO,
	PLL_REF_HOCO,
	PLL_REF_RFOSC,
	PLL_REF_MOSC,
};

/* General clock source options. */
#define BSP_CLOCKS_SOURCE_MOCO            (0U)  /* The middle speed on chip oscillator. */
#define BSP_CLOCKS_SOURCE_HOCO            (1U)  /* The high speed on chip oscillator.*/
#define BSP_CLOCKS_SOURCE_MOSC            (2U)  /* The main oscillator.*/
#define BSP_CLOCKS_SOURCE_PLL1            (3U)  /* The PLL1.*/
#define BSP_CLOCKS_SOURCE_LPCLK           (4U)  /* Low power CLK */
#define BSP_CLOCKS_SOURCE_RFOSC           (5U)  /* The RF clock oscillator. */
#define BSP_CLOCKS_SOURCE_PLL0            (6U)  /* The PLL0. */
#define BSP_CLOCKS_SOURCE_COUNT           (7U)  /* The Source Count. */

struct PLL_CFG0 {
	union {
		__IOM uint32_t PLL_CFG0;
		struct {
			__IM uint32_t RESERVED_I  : 3;          /*!< [2..0] Reserved.                                                          */
			__IM uint32_t PLL_LOCK   : 1;           /*!< [3..3] Output. Lock detection'0.8V signal'.                               */
			__IOM uint32_t PLL_BYPASS : 1;          /*!< [4..4] PLL BYPASS Signal'0.8V signal'1: PLL bypass.                       */
			__IOM uint32_t PLL_MODE   : 2;          /*!< [6..5] Operation Mode Select00: integer mode;01: fraction mode;10:
			                                         *   spread spectrum mode;11: reserved.                                        */
			__IOM uint32_t PLL_RESETN : 1;          /*!< [7..7] Soft reset. Resets the SSC & Fraction Function When Low
			                                         *   '0.8v signal for test only'.                                              */
			__IOM uint32_t PLL_PDDP : 1;            /*!< [8..8] DP Power Down Signal. '0.8V signal'1: DP power down;0:DP
			                                         *   normal operation.                                                         */
			__IOM uint32_t PLL_PD : 1;              /*!< [9..9] PLL Power Down Signal. '0.8V signal'1: PLL power down;0:
			                                         *   normal operation.                                                         */
			__IOM uint32_t PLL_DP : 3;              /*!< [12..10] Output Divider Control Pins. Set the post divider factor
			                                         *   from 1 to 7 '0.8V signal'.                                                */
			__IOM uint32_t PLL_DN : 11;             /*!< [23..13] Feedback Divider Control Pins. Set the feedback divider
			                                         *   factor from 16 to 2048 '0.8V signal'.                                     */
			__IOM uint32_t PLL_DM : 6;              /*!< [29..24] Reference Input Divider Control Pins. Set the reference
			                                         *   divider factor from 1 to 63 '0.8V signal'.                                */
			__IOM uint32_t PLL_REFIN_SEL : 2;       /*!< [31..30] PLL reference clock select2'b00: MOCO2'b01: HOCO2'b10:
			                                         *   RFOSC2'b11: MOSC.                                                         */
		} PLL_CFG0_b;
	};
};

struct CLOCK_SEL {
	__IOM uint32_t CLK_SEL;
	struct {
		__IOM uint32_t CLK_SEL : 3;     /*!< [2..0] system clocl source select3'b000: MOCO3'b001: HOCO3'b010:
		                                 *   MOSC3'b011: PLL13'b100: LPCLK'From RTC'3'b101: RFOSC3'b110:
		                                 *   PLL0Others: Reserved.                                                     */
		__IOM uint32_t CLK_DIV : 3;     /*!< [5..3] Divide the selected source clock as system clock3'b000:
		                                 *   1/13'b001: 1/23'b010: 1/33'b011: 1/43'b100: 1/53'b101:
		                                 *   1/63'b110: 1/73'b111: 1/8.                                                */
		__IM uint32_t RESERVED_15 : 26; /*!< [31..6] Reserved.                                                         */
	} CLK_SEL_b;
};

struct BLE_BUS_CLOCK_SEL {
	union {
		__IOM uint32_t BLE_BUS_CLK_CFG;
		struct {
			__IOM uint32_t BLE_BUS_CLK_DIV : 3;     /*!< [2..0] Divide the selected source clock'the BLE 128M clock'
			                                         *   as BLE bus clock0x0: 1/10x1: 1/20x2: 1/4Others: reserved.                 */
			__IM uint32_t RESERVED_11 : 29;         /*!< [31..3] Reserved.                                                         */
		} BLE_BUS_CLK_CFG_b;
	};
};

struct USB_CLOCK_SEL {
	union {
		__IOM uint32_t USB_CLK_CFG;
		struct {
			__IOM uint32_t USBCLK_SEL : 2;  /*!< [1..0] USB PHY reference clock source select2'b00: MOSC2'b01:
			                                 *   PLL0 output2'b10: PLL1 output2'b11: Reserved.                             */
			__IOM uint32_t USBCLK_DIV : 4;  /*!< [5..2] Divide the selected source clock as USB PHY reference
			                                 *   clock, it should be 12/ 24MHz0x0: 1/10x1: 1/20x2: 1/40x3:
			                                 *   1/60x4: 1/80x5: 1/100x6: 1/120x7: 1/140x8: 1/160x9: 1/180xA:
			                                 *   1/200xB: 1/220xC: 1/240xD: 1/260xE: 1/280xF: 1/30.                        */
			__IM uint32_t RESERVED_12 : 26; /*!< [31..6] Reserved.                                                         */
		} USB_CLK_CFG_b;
	};
};

struct PERI_CLKD_CLOCK_SEL {
	union {
		__IOM uint32_t PERI_CLKD_SEL;
		struct {
			__IOM uint32_t PERICLKD_SEL : 3;        /*!< [2..0] Peripheral clock D source select3'b000: MOCO3'b001: HOCO3'b010:
			                                         *   MOSC3'b011: PLL13'b100: LPCLK'From RTC'3'b101: RFOSC3'b110:
			                                         *   PLL0Others: Reserved.                                                     */
			__IOM uint32_t PERICLKD_DIV : 4;        /*!< [6..3] Divide the selected source clock as peripheral clock
			                                         *   D4'b0000: 1/14'b0001: 1/24'b0010: 1/44'b0011: 1/84'b0100:
			                                         *   1/164'b0101: 1/324'b0110: 1/644'b0111: 1/1284'b1000: 1/256Others:
			                                         *   reserved.                                                                 */
			__IM uint32_t RESERVED_19 : 25;         /*!< [31..7] Reserved.                                                         */
		} PERI_CLKD_SEL_b;
	};
};

#define PLL0_CFG0 ((struct PLL_CFG0  *)SYSREG2_PLL0_CFG0)
#define PLL1_CFG0 ((struct PLL_CFG0  *)SYSREG2_PLL1_CFG0)

#define DBCLK_CFG ((struct CLOCK_SEL  *)SYSREG2_DBCLK_CFG)
#define BLE_BUS_CLK_CFG ((struct BLE_BUS_CLOCK_SEL  *)SYSREG2_BLE_BUS_CLK_CFG)
#define USB_CLK_CFG ((struct USB_CLOCK_SEL  *)SYSREG2_USB_CLK_CFG)
#define BLE_CLK_SEL ((struct CLOCK_SEL  *)SYSREG2_BLE_CLK_SEL)
#define QSPI_CORE_CLK_SEL ((struct CLOCK_SEL  *)SYSREG2_QSPI_CORE_CLK_SEL)
#define SYS_CLK_SEL ((struct CLOCK_SEL  *)SYSREG2_SYS_CLK_SEL)
#define PERI_CLKA_SEL ((struct CLOCK_SEL  *)SYSREG2_PERI_CLKA_SEL)
#define PERI_CLKB_SEL ((struct CLOCK_SEL  *)SYSREG2_PERI_CLKB_SEL)
#define PERI_CLKC_SEL ((struct CLOCK_SEL  *)SYSREG2_PERI_CLKC_SEL)
#define PERI_CLKD_SEL ((struct PERI_CLKD_CLOCK_SEL  *)SYSREG2_PERI_CLKD_SEL)
#define PERI_CLKE_SEL ((struct CLOCK_SEL  *)SYSREG2_PERI_CLKE_SEL)
#define TRACE_CLK_SEL ((struct CLOCK_SEL  *)SYSREG2_TRACE_CLK_SEL)
#define CLK_OUT_SEL ((struct CLOCK_SEL  *)SYSREG2_CLK_OUT_SEL)


void board_clock_init(void);

#endif /* __BOARD_CLOCK_H__ */
