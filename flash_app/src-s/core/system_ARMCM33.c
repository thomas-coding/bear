/*
 * Device System Source File for ARM Cortex-M33 Device.
 *
 * Refer to CMSIS Device System Source File for ARMCM33 Device.
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

#include "system_ARMCM33.h"

//TODO
#define SYSTEM_CLOCK 24000000

/* Exception / Interrupt Vector table */
extern const VECTOR_TABLE_Type __VECTOR_TABLE[496];

/* System Core Clock Variable */
unsigned int SystemCoreClock = SYSTEM_CLOCK;  /* System Core Clock Frequency */

/* System Core Clock update function */
void SystemCoreClockUpdate (void)
{
	SystemCoreClock = SYSTEM_CLOCK;
}

/* System initialization function */
void SystemInit (void)
{
	unsigned int value_sys = 0;

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
	SCB->VTOR = (unsigned int) &(__VECTOR_TABLE[0]);
#endif

#if defined (__FPU_USED) && (__FPU_USED == 1U)
	SCB->CPACR |= ((3U << 10U*2U) |		/* enable CP10 Full Access */
		       (3U << 11U*2U)  );	/* enable CP11 Full Access */
#endif

	SystemCoreClock = SYSTEM_CLOCK;
}
