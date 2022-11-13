/*
 * Startup Code for Arm Cortex-M33 Device
 *
 * Refer to CMSIS-Core(M) Device Startup File for Cortex-M33 Device
 *
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
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

	.syntax unified
	.arch armv8-m.main

	.section .vectors
	.align 2
	.globl __Vectors
	.globl __Vectors_End
	.globl __Vectors_Size

	.globl hard_fault_handler
	.globl memm_fault_handler
	.globl secure_fault_handler
	.globl SVC_Handler

__Vectors:
	.long __StackTop		/*     Top of Stack */
	.long Reset_Handler		/*     Reset Handler */
	.long NMI_Handler		/* -14 NMI Handler */
	.long HardFault_Handler		/* -13 Hard Fault Handler */
	.long MemManage_Handler		/* -12 MPU Fault Handler */
	.long BusFault_Handler		/* -11 Bus Fault Handler */
	.long UsageFault_Handler	/* -10 Usage Fault Handler */
	.long SecureFault_Handler	/*  -9 Secure Fault Handler */
	.long 0				/*     Reserved */
	.long 0				/*     Reserved */
	.long 0				/*     Reserved */
	.long SVC_Handler		/*  -5 SVCall Handler */
	.long DebugMon_Handler		/*  -4 Debug Monitor Handler */
	.long 0				/*     Reserved */
	.long PendSV_Handler		/*  -2 PendSV Handler */
	.long SysTick_Handler		/*  -1 SysTick Handler */

	.space (480 * 4)		/* Interrupts 0 .. 480 are left out */

__Vectors_End:
	.equ __Vectors_Size, __Vectors_End - __Vectors
	.size __Vectors, . - __Vectors

	.thumb
	.section .text
	.align 2

	.thumb_func
	.type Reset_Handler, %function
	.globl Reset_Handler
	.fnstart
Reset_Handler:
	ldr r0, =__StackLimit
	msr msplim, r0

	/* Clear bss */
	ldr r1, =__bss_start__
	ldr r2, =__bss_end__

	movs r0, 0
.L_loop1:
	cmp r1, r2
	itt lt
	strlt r0, [r1], #4
	blt .L_loop1

	/* CMSIS System Initialization */
	bl SystemInit

	ldr r3, =__zero_table_start__
	ldr r4, =__zero_table_end__

.L_loop2:
	cmp r3, r4
	bge .L_loop2_done
	ldr r1, [r3]			/* destination address */
	ldr r2, [r3, #4]		/* word count */
	lsl r2, r2, #2			/* byte count */
	movs r0, 0

.L_loop2_0:
	subs r2, #4			/* decrement byte count */
	itt ge
	strge r0, [r1, r2]
	bge .L_loop2_0

	adds r3, #8
	b .L_loop2
.L_loop2_done:

	/* Select switch to PSP */
	mrs r0, control			/* Get control value */
	orr r0, r0, #2			/* Select swtich to PSP */
	msr control, r0
	ldr r0, =__PspStackLimit
	msr psplim, r0
	ldr r0, =__PspStackTop
	msr psp, r0

	bl main

	.fnend
	.size Reset_Handler, . - Reset_Handler

/*
 * The default macro is not used for HardFault_Handler
 * because this results in a poor debug illusion.
 */
	.thumb_func
	.type HardFault_Handler, %function
	.weak HardFault_Handler
	.fnstart
HardFault_Handler:
	tst lr, #4			/* Test EXC_RETURN bit.2 */
	ite eq				/* If equal to 0 */
	mrseq r0, msp			/* If MSP was used, read MSP */
	mrsne r0, psp			/* Otherwise, read PSP */
	ldr r1, [r0, #24]		/* Read PC from stack */
	ldr r2, =hard_fault_handler
	bx r2

	.fnend
	.size HardFault_Handler, . - HardFault_Handler

	.thumb_func
	.type MemManage_Handler, %function
	.weak MemManage_Handler
	.fnstart
MemManage_Handler:
	tst lr, #4
	ite eq
	mrseq r0, msp
	mrsne r0, psp
	ldr r1, [r0, #24]
	ldr r2, =memm_fault_handler
	bx r2

	.fnend
	.size MemManage_Handler, . - MemManage_Handler

	.thumb_func
	.type SecureFault_Handler, %function
	.weak SecureFault_Handler
	.fnstart
SecureFault_Handler:
	tst lr, #4
	ite eq
	mrseq r0, msp
	mrsne r0, psp
	ldr r1, [r0, #24]
	ldr r2, =secure_fault_handler
	bx r2

	.fnend
	.size SecureFault_Handler, . - SecureFault_Handler

	.thumb_func
	.type Default_Handler, %function
	.weak Default_Handler
	.fnstart
Default_Handler:
	b .
	.fnend
	.size Default_Handler, . - Default_Handler

/*
 * Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
	.macro Set_Default_Handler  Handler_Name
	.weak \Handler_Name
	.set \Handler_Name, Default_Handler
	.endm

/* Default exception/interrupt handler */

	Set_Default_Handler NMI_Handler
	Set_Default_Handler BusFault_Handler
	Set_Default_Handler UsageFault_Handler
	Set_Default_Handler SVC_Handler
	Set_Default_Handler DebugMon_Handler
	Set_Default_Handler PendSV_Handler
	Set_Default_Handler SysTick_Handler

	.end
