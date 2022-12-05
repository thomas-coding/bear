/*
 * Header for platform.
 *
 * Author: Zhigang.Kang <Zhigang.Kang@verisilicon.com>
 *
 * Copyright (C) 2021 VeriSilicon Microelectronics (Shanghai) Co., Ltd.
 *
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <common.h>

#define CHIP_PLL_ENABLE         1

/* Interrupt Number Definition */
typedef enum IRQn {
	/* Processor Exceptions Numbers */
	NonMaskableInt_IRQn	= -14,  /*  2 Non Maskable Interrupt */
	HardFault_IRQn		= -13,  /*  3 HardFault Interrupt */
	MemoryManagement_IRQn	= -12,  /*  4 Memory Management Interrupt */
	BusFault_IRQn		= -11,  /*  5 Bus Fault Interrupt */
	UsageFault_IRQn		= -10,  /*  6 Usage Fault Interrupt */
	SecureFault_IRQn	=  -9,  /*  7 Secure Fault Interrupt */
	SVCall_IRQn		=  -5,  /* 11 SV Call Interrupt */
	DebugMonitor_IRQn	=  -4,  /* 12 Debug Monitor Interrupt */
	PendSV_IRQn		=  -2,  /* 14 Pend SV Interrupt */
	SysTick_IRQn		=  -1,  /* 15 System Tick Interrupt */

	/* Interrupts 0 .. 480 are left out */
} IRQn_Type;

/* Configuration of Core Peripherals */
#define __CM33_REV              0x0000U /* Core revision r0p1 */
#define __SAUREGION_PRESENT     1U      /* SAU regions present */
#define __MPU_PRESENT           1U      /* MPU present */
#define __VTOR_PRESENT          1U      /* VTOR present */
#define __NVIC_PRIO_BITS        3U      /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig  0U      /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT           0U      /* FPU present */
#define __DSP_PRESENT           1U      /* DSP extension present */

#include "core_cm33.h"                  /* Processor and core peripherals */

#if CHIP_PLL_ENABLE
#define M33_CLK             (96 * Mhz)
#define UART_CLK            (24576000)
#else  /* Default Clock without PLL enabled */
#endif

#define SYSTEM_CLOCK            M33_CLK

/* Memory Map */
#define LAS_SRAM_SIZE           0x00200000
#define LAS_SRAM_S_BASE         CONFIG_SRAM_S_BASE
#define LAS_SRAM_S_SIZE         CONFIG_SRAM_S_SIZE
#define LAS_SRAM_NS_BASE        CONFIG_SRAM_NS_BASE
#define LAS_SRAM_NS_SIZE        CONFIG_SRAM_NS_SIZE

/* For testing, will set to NS */
#define PERIPH_DEV_BASE         0xe0000000
#define PERIPH_DEV_SIZE         0x20000000

/* For testing, will set to NS */
#define LPS_SRAM01_BASE         0x10000000
#define LPS_SRAM01_SIZE         0x00800000
#define LPS_SRAM01_S_SIZE       0x00700000
#define LPS_SRAM01_NS_SIZE      0x00100000

/* For testing, will set to NS */
#define LPS_SRAM23_BASE         0x10800000
#define LPS_SRAM23_SIZE         0x00800000
#define LPS_SRAM23_S_SIZE       0x00700000
#define LPS_SRAM23_NS_SIZE      0x00100000

/* Reserved for testing */
#define LAS_SRAM_TEST_BASE      CONFIG_MPU_TEST_BASE
#define LAS_SRAM_TEST_SIZE      CONFIG_MPU_TEST_SIZE

/* LAS_SRAM_MPC */
#define LAS_SRAM_MPC_BASE       0xf1660000

/* LAS_SYSCTRL */
#define LAS_SYS_CTL_BASE_ADD    0xf1640000

/* Bear memory map */
#define IOMUX_BASE 0x50000000

#define SYSREG0_BASE 0x50000c00
#define SYSREG0_BLE_SYS_STOP (SYSREG0_BASE + 0x54)

#define SYSREG2_BASE 0x50065000
#define SYSREG2_PLL0_CFG0 (SYSREG2_BASE + 0x0)
#define SYSREG2_PLL1_CFG0 (SYSREG2_BASE + 0x10)
#define SYSREG2_HOCO_CFG (SYSREG2_BASE + 0x20)
#define SYSREG2_MOCO_CFG (SYSREG2_BASE + 0x24)
#define SYSREG2_MOSC_CFG (SYSREG2_BASE + 0x2c)
#define SYSREG2_DBCLK_CFG (SYSREG2_BASE + 0x38)
#define SYSREG2_BLE_BUS_CLK_CFG (SYSREG2_BASE + 0x3c)
#define SYSREG2_USB_CLK_CFG (SYSREG2_BASE + 0x40)
#define SYSREG2_BLE_CLK_SEL (SYSREG2_BASE + 0x44)
#define SYSREG2_QSPI_CORE_CLK_SEL (SYSREG2_BASE + 0x48)
#define SYSREG2_SYS_CLK_SEL (SYSREG2_BASE + 0x4c)
#define SYSREG2_PERI_CLKA_SEL (SYSREG2_BASE + 0x50)
#define SYSREG2_PERI_CLKB_SEL (SYSREG2_BASE + 0x54)
#define SYSREG2_PERI_CLKC_SEL (SYSREG2_BASE + 0x58)
#define SYSREG2_PERI_CLKD_SEL (SYSREG2_BASE + 0x5c)
#define SYSREG2_PERI_CLKE_SEL (SYSREG2_BASE + 0x60)
#define SYSREG2_TRACE_CLK_SEL (SYSREG2_BASE + 0x64)
#define SYSREG2_CLK_OUT_SEL (SYSREG2_BASE + 0x68)
#define SYSREG2_BLK_SWR0 (SYSREG2_BASE + 0x6c)
#define SYSREG2_BLK_SWR1 (SYSREG2_BASE + 0x70)
#define SYSREG2_BLK_STOP0 (SYSREG2_BASE + 0x74)
#define SYSREG2_BLK_STOP1 (SYSREG2_BASE + 0x78)
#define SYSREG2_QSPI_INTF_CFG1 (SYSREG2_BASE + 0x98)

#endif /* __PLATFORM_H__ */
