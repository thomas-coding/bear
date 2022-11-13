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

#define CHIP_PLL_ENABLE		0

/* Interrupt Number Definition */
typedef enum IRQn
{
	/* Processor Exceptions Numbers */
	NonMaskableInt_IRQn	= -14,	/*  2 Non Maskable Interrupt */
	HardFault_IRQn		= -13,	/*  3 HardFault Interrupt */
	MemoryManagement_IRQn	= -12,	/*  4 Memory Management Interrupt */
	BusFault_IRQn		= -11,	/*  5 Bus Fault Interrupt */
	UsageFault_IRQn		= -10,	/*  6 Usage Fault Interrupt */
	SecureFault_IRQn	=  -9,	/*  7 Secure Fault Interrupt */
	SVCall_IRQn		=  -5,	/* 11 SV Call Interrupt */
	DebugMonitor_IRQn	=  -4,	/* 12 Debug Monitor Interrupt */
	PendSV_IRQn		=  -2,	/* 14 Pend SV Interrupt */
	SysTick_IRQn		=  -1,	/* 15 System Tick Interrupt */

	/* Interrupts 0 .. 480 are left out */
} IRQn_Type;

/* Configuration of Core Peripherals */
#define __CM33_REV		0x0000U	/* Core revision r0p1 */
#define __SAUREGION_PRESENT	1U	/* SAU regions present */
#define __MPU_PRESENT		1U	/* MPU present */
#define __VTOR_PRESENT		1U	/* VTOR present */
#define __NVIC_PRIO_BITS	3U	/* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig	0U	/* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT		0U	/* FPU present */
#define __DSP_PRESENT		1U	/* DSP extension present */

#include "core_cm33.h"			/* Processor and core peripherals */

/* Process stack size */
#define SP_PROCESS_SIZE		0x200

#if CHIP_PLL_ENABLE
#define LAS_M33_CLK		(400 * Mhz)
#define LAS_M0P_CLK		(100 * Mhz)
#define LAS_TSC_CLK		(24576000)
#define LAS_PDM_CLK		(3072000)
#define LAS_I2S_CLK		(24576000)
#define LAS_UART_CLK		(100 * Mhz)
#define LAS_I2C_CLK		(200 * Mhz)
#define LAS_SPI_CLK		(200 * Mhz)
#define LAS_PWM_CLK		(200 * Mhz)
#define LAS_TIMER_CLK		(24576000)
#define LAS_WDT_CLK		(24576000)
#define LAS_RTC_CLK		(32768)
#define LAS_PCK_CLK		(200 * Mhz)
#else  /* Default Clock without PLL enabled */
#define LAS_M33_CLK		(24576000)
#define LAS_M0P_CLK		(24576000)
#define LAS_TSC_CLK		(24576000)
#define LAS_PDM_CLK		(24576000)
#define LAS_I2S_CLK		(24576000)
#define LAS_UART_CLK		(24576000)
#define LAS_I2C_CLK		(24576000)
#define LAS_SPI_CLK		(24576000)
#define LAS_PWM_CLK		(24576000)
#define LAS_TIMER_CLK		(24576000)
#define LAS_WDT_CLK		(24576000)
#define LAS_RTC_CLK		(32768)
#define LAS_PCK_CLK		(24576000)
#endif

#define SYSTEM_CLOCK		LAS_M33_CLK

/* Memory Map */
#define LAS_SRAM_SIZE		0x00200000
#define LAS_SRAM_S_BASE		CONFIG_SRAM_S_BASE
#define LAS_SRAM_S_SIZE		CONFIG_SRAM_S_SIZE
#define LAS_SRAM_NS_BASE	CONFIG_SRAM_NS_BASE
#define LAS_SRAM_NS_SIZE	CONFIG_SRAM_NS_SIZE

/* For testing, will set to NS */
#define PERIPH_DEV_BASE		0xe0000000
#define PERIPH_DEV_SIZE		0x20000000

/* For testing, will set to NS */
#define LPS_SRAM01_BASE		0x10000000
#define LPS_SRAM01_SIZE		0x00800000
#define LPS_SRAM01_S_SIZE	0x00700000
#define LPS_SRAM01_NS_SIZE	0x00100000

/* For testing, will set to NS */
#define LPS_SRAM23_BASE		0x10800000
#define LPS_SRAM23_SIZE		0x00800000
#define LPS_SRAM23_S_SIZE	0x00700000
#define LPS_SRAM23_NS_SIZE	0x00100000

/* Reserved for testing */
#define LAS_SRAM_TEST_BASE	CONFIG_MPU_TEST_BASE
#define LAS_SRAM_TEST_SIZE	CONFIG_MPU_TEST_SIZE

/* LAS_SRAM_MPC */
#define LAS_SRAM_MPC_BASE	0xf1660000

/* LAS_SYSCTRL */
#define LAS_SYS_CTL_BASE_ADD	0xf1640000

#endif /* __PLATFORM_H__ */
