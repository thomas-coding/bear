/*
 * Device System Header File for ARM Cortex-M33 Device
 *
 * Refer to CMSIS Device System Header File for ARMCM33 Device
 *
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYSTEM_ARMCM33_H
#define SYSTEM_ARMCM33_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	unsigned int CTRL;                     /* Offset: 0x000 (R/W) Control Register */
	unsigned int RESERVED0[3];
	unsigned int BLK_MAX;                  /* Offset: 0x010 (R/ ) Block Maximum Register */
	unsigned int BLK_CFG;                  /* Offset: 0x014 (R/ ) Block Configuration Register */
	unsigned int BLK_IDX;                  /* Offset: 0x018 (R/W) Block Index Register */
	unsigned int BLK_LUT;                  /* Offset: 0x01C (R/W) Block Lookup Tabe Register */
	unsigned int INT_STAT;                 /* Offset: 0x020 (R/ ) Interrupt Status Register */
	unsigned int INT_CLEAR;                /* Offset: 0x024 ( /W) Interrupt Clear Register */
	unsigned int INT_EN;                   /* Offset: 0x028 (R/W) Interrupt Enable Register */
	unsigned int INT_INFO1;                /* Offset: 0x02C (R/ ) Interrupt Info1 Register */
	unsigned int INT_INFO2;                /* Offset: 0x030 (R/ ) Interrupt Info2 Register */
	unsigned int INT_SET;                  /* Offset: 0x034 ( /W) Interrupt Set Register */
} MPC_TypeDef;

typedef struct
{
	unsigned int RESERVED0[4U];
	unsigned int SECRESPCFG;               /* Offset: 0x010 (R/W) Security Violation Response Configuration Register */
	unsigned int NSCCFG;                   /* Offset: 0x014 (R/W) Non Secure Callable Configuration for IDAU */
	unsigned int RESERVED1[1U];
	unsigned int SECMPCINTSTATUS;          /* Offset: 0x01C (R/ ) Secure MPC Interrupt Status */
	unsigned int SECPPCINTSTAT;            /* Offset: 0x020 (R/ ) Secure PPC Interrupt Status */
	unsigned int SECPPCINTCLR;             /* Offset: 0x024 ( /W) Secure PPC Interrupt Clear */
	unsigned int SECPPCINTEN;              /* Offset: 0x028 (R/W) Secure PPC Interrupt Enable */
	unsigned int RESERVED2[1U];
	unsigned int SECMSCINTSTAT;            /* Offset: 0x030 (R/ ) Secure MSC Interrupt Status */
	unsigned int SECMSCINTCLR;             /* Offset: 0x034 ( /W) Secure MSC Interrupt Clear */
	unsigned int SECMSCINTEN;              /* Offset: 0x038 (R/W) Secure MSC Interrupt Enable */
	unsigned int RESERVED3[1U];
	unsigned int BRGINTSTAT;               /* Offset: 0x040 (R/ ) Bridge Buffer Error Interrupt Status */
	unsigned int BRGINTCLR;                /* Offset: 0x044 ( /W) Bridge Buffer Error Interrupt Clear */
	unsigned int BRGINTEN;                 /* Offset: 0x048 (R/W) Bridge Buffer Error Interrupt Enable */
	unsigned int RESERVED4[1U];
	unsigned int AHBNSPPC[4U];             /* Offset: 0x050 (R/W) Non-Secure Access AHB slave Peripheral Protection Control */
	unsigned int AHBNSPPCEXP[4U];          /* Offset: 0x060 (R/W) Expansion Non_Secure Access AHB slave Peripheral Protection Control */
	unsigned int APBNSPPC[4U];             /* Offset: 0x070 (R/W) Non-Secure Access APB slave Peripheral Protection Control */
	unsigned int APBNSPPCEXP[4U];          /* Offset: 0x080 (R/W) Expansion Non_Secure Access APB slave Peripheral Protection Control */
	unsigned int AHBSPPPC[4U];             /* Offset: 0x090 (R/ ) Secure Unprivileged Access AHB slave Peripheral Protection Control */
	unsigned int AHBSPPPCEXP[4U];          /* Offset: 0x0A0 (R/W) Expansion Secure Unprivileged Access AHB slave Peripheral Protection Control */
	unsigned int APBSPPPC[4U];             /* Offset: 0x0B0 (R/W) Secure Unprivileged Access APB slave Peripheral Protection Control */
	unsigned int APBSPPPCEXP[4U];          /* Offset: 0x0C0 (R/W) Expansion Secure Unprivileged Access APB slave Peripheral Protection Control */
	unsigned int NSMSCEXP;                 /* Offset: 0x0D0 (R/ ) Expansion MSC Non-Secure Configuration */
} SPC_TypeDef;


/*------------------- Non-Secure Privilege Control Block -----------------------------*/
typedef struct
{
	unsigned int RESERVED0[36U];
	unsigned int AHBNSPPPC[4U];            /* Offset: 0x090 (R/W)  read-write 0x0000_0000 Non-Secure Unprivileged Access AHB slave Peripheral Protection Control */
	unsigned int AHBNSPPPCEXP[4U];         /* Offset: 0x0A0 (R/W)  read-write 0x0000_0000 Expansion 0 Non-Secure Unprivileged Access AHB slave Peripheral Protection Control */
	unsigned int APBNSPPPC[4U];            /* Offset: 0x0B0 (R/W)  read-write 0x0000_0000 Non-Secure Unprivileged Access APB slave Peripheral Protection Control */
	unsigned int APBNSPPPCEXP[4U];         /* Offset: 0x0C0 (R/W)  read-write 0x0000_0000 Expansion 0 Non-Secure Unprivileged Access APB slave Peripheral Protection Control */
} NSPC_TypeDef;

/**
  \brief Exception / Interrupt Handler Function Prototype
*/
typedef void(*VECTOR_TABLE_Type)(void);

/**
  \brief System Clock Frequency (Core Clock)
*/
extern unsigned int SystemCoreClock;

/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_ARMCM33_H */
