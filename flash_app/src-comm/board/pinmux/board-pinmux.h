
#ifndef __BOARD_PINMUX_H__
#define __BOARD_PINMUX_H__

#include "platform.h"

/** IO pins. */
enum bsp_io_pin_t {
	BSP_IO_00_PIN_00,       ///< JTAG_DO
	BSP_IO_00_PIN_01,       ///< JTAG_MS
	BSP_IO_00_PIN_02,       ///< JTAG_CK
	BSP_IO_00_PIN_03,       ///< JTAG_DI
	BSP_IO_00_PIN_04,       ///< CACREF
	BSP_IO_00_PIN_05,       ///< IRQ0
	BSP_IO_00_PIN_06,       ///< IRQ1
	BSP_IO_00_PIN_07,       ///< IRQ2
	BSP_IO_00_PIN_08,       ///< IRQ3
	BSP_IO_00_PIN_09,       ///< IRQ4
	BSP_IO_00_PIN_10,       ///< IRQ5
	BSP_IO_00_PIN_11,       ///< IRQ6
	BSP_IO_00_PIN_12,       ///< IRQ7
	BSP_IO_00_PIN_13,       ///< IRQ8
	BSP_IO_00_PIN_14,       ///< IRQ9
	BSP_IO_00_PIN_15,       ///< IRQ10
	BSP_IO_00_PIN_16,       ///< IRQ11
	BSP_IO_00_PIN_17,       ///< IRQ_DS0
	BSP_IO_00_PIN_18,       ///< IRQ_DS1
	BSP_IO_00_PIN_19,       ///< IRQ_DS2
	BSP_IO_00_PIN_20,       ///< IRQ_DS3
	BSP_IO_00_PIN_21,       ///< IRQ_DS4
	BSP_IO_00_PIN_22,       ///< IRQ_DS5
	BSP_IO_00_PIN_23,       ///< IRQ_DS6
	BSP_IO_00_PIN_24,       ///< IRQ_DS7
	BSP_IO_00_PIN_25,       ///< IRQ_DS8
	BSP_IO_00_PIN_26,       ///< IRQ_DS9
	BSP_IO_00_PIN_27,       ///< IRQ_DS10
	BSP_IO_00_PIN_28,       ///< IRQ_DS11
	BSP_IO_00_PIN_29,       ///< IRQ_DS12
	BSP_IO_00_PIN_30,       ///< IRQ_DS13
	BSP_IO_00_PIN_31,       ///< IRQ_DS14

	BSP_IO_01_PIN_00,       ///< IRQ_DS15
	BSP_IO_01_PIN_01,       ///< NMI
	BSP_IO_01_PIN_02,       ///< SPI_CS0
	BSP_IO_01_PIN_03,       ///< SPI_MOSI0
	BSP_IO_01_PIN_04,       ///< SPI_MISO0
	BSP_IO_01_PIN_05,       ///< SPI_CLK0
	BSP_IO_01_PIN_06,       ///< SPI_CS1
	BSP_IO_01_PIN_07,       ///< SPI_MOSI1
	BSP_IO_01_PIN_08,       ///< SPI_MISO1
	BSP_IO_01_PIN_09,       ///< SPI_CLK1
	BSP_IO_01_PIN_10,       ///< SPI_CS2
	BSP_IO_01_PIN_11,       ///< SPI_MOSI2
	BSP_IO_01_PIN_12,       ///< SPI_MISO2
	BSP_IO_01_PIN_13,       ///< SPI_CLK
	BSP_IO_01_PIN_14,       ///< SPI_CS3
	BSP_IO_01_PIN_15,       ///< SPI_MOSI3
	BSP_IO_01_PIN_16,       ///< SPI_MISO3
	BSP_IO_01_PIN_17,       ///< SPI_CLK3
	BSP_IO_01_PIN_18,       ///< PWM_OUT0
	BSP_IO_01_PIN_19,       ///< PWM_OUT1
	BSP_IO_01_PIN_20,       ///< PWM_OUT2
	BSP_IO_01_PIN_21,       ///< PWM_OUT3
	BSP_IO_01_PIN_22,       ///< PWM_OUT4
	BSP_IO_01_PIN_23,       ///< PWM_OUT5
	BSP_IO_01_PIN_24,       ///< PWM_OUT6
	BSP_IO_01_PIN_25,       ///< I2S_BCLK
	BSP_IO_01_PIN_26,       ///< I2S_LRCK
	BSP_IO_01_PIN_27,       ///< I2S_DI
	BSP_IO_01_PIN_28,       ///< I2S_DO
	BSP_IO_01_PIN_29,       ///< UART_TXD0
	BSP_IO_01_PIN_30,       ///< UART_RXD0
	BSP_IO_01_PIN_31,       ///< UART_CTS0

	BSP_IO_02_PIN_00,       ///< UART_RTS0
	BSP_IO_02_PIN_01,       ///< UART_TXD1
	BSP_IO_02_PIN_02,       ///< UART_RXD1
	BSP_IO_02_PIN_03,       ///< UART_CTS1
	BSP_IO_02_PIN_04,       ///< UART_RTS1
	BSP_IO_02_PIN_05,       ///< UART_TXD2
	BSP_IO_02_PIN_06,       ///< UART_RXD2
	BSP_IO_02_PIN_07,       ///< UART_CTS2
	BSP_IO_02_PIN_08,       ///< UART_RTS2
	BSP_IO_02_PIN_09,       ///< UART_TXD3
	BSP_IO_02_PIN_10,       ///< UART_RXD3
	BSP_IO_02_PIN_11,       ///< UART_CTS3
	BSP_IO_02_PIN_12,       ///< UART_RTS3
	BSP_IO_02_PIN_13,       ///< UART_TXD_BLE
	BSP_IO_02_PIN_14,       ///< UART_RXD_BLE
	BSP_IO_02_PIN_15,       ///< UART_CTS_BLE
	BSP_IO_02_PIN_16,       ///< UART_RTS_BLE
	BSP_IO_02_PIN_17,       ///< ADC_EXTTRIG
};

struct IOMUX_REG {
	union {
		__IOM uint32_t PAD_CTRL[82];                       /*!< (@ 0x00000000) pad jtag_do mux control                                    */

		struct {
			__IOM uint32_t SYS_IOMUXC_PAD_CTL_DS : 2;       /*!< [1..0] pad drive strength. drive strength of pad jtag_do:00:
			                                                 * load 2pf;01: load 4pf;10: load 8pf;11: load 16pf.                         */
			__IOM uint32_t SYS_IOMUXC_SW_INPUT_ON : 1;      /*!< [2..2] pad input force on. set this bit to 1 to force pad jtag_do
			                                                 * input enable                                                              */
			__IOM uint32_t SYS_IOMUXC_PAD_CTL_ST : 1;       /*!< [3..3] pad schmitt trigger enable. high active to enable schmitt
			                                                 * trigger of pad jtag_do                                                    */
			__IOM uint32_t SYS_IOMUXC_PAD_CTL_PD : 1;       /*!< [4..4] pad pull down enable. high active to enable pull down
			                                                 * of pad jtag_do                                                            */
			__IOM uint32_t SYS_IOMUXC_PAD_CTL_PU : 1;       /*!< [5..5] pad pull up enable. high active to enable pull up of
			                                                 * pad jtag_do                                                               */
			__IOM uint32_t SYS_IOMUXC_PAD_CTL_OD : 1;       /*!< [6..6] pad open drain enable. high active to enable open drain
			                                                 * of pad jtag_do                                                            */
			__IOM uint32_t SYS_IOMUXC_SW_MUX : 5;           /*!< [11..7] pad mux select. select the pin mux mode of pad jtag_do     */
			__IOM uint32_t SYS_IOMUXC_IOKEEP : 1;           /*!< [12..12] IO keep enable. high active to enable the IO keep function
			                                                 * of pad jtag_do. It is used to keep the IO states when chip
			                                                 * enter or leave deep software standby mode.                                */
			__IM uint32_t RESERVED_I : 19;                  /*!< [31..13] reserved.                                                        */
		} PAD_CTRL_b[82];
	};
};

#define IOMUX ((struct IOMUX_REG  *)IOMUX_BASE)

void board_pinmux_init(void);

#endif /* __BOARD_PINMUX_H__ */
