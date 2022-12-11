/*
 * GNU Linker Script for Cortex-M based device.
 */

/* Non-secure sram 128K */
/* If enable mbedtls, need more code size */
CONFIG_SRAM_NS_BASE = 0x20220000;
CONFIG_SRAM_NS_SIZE = 0x20000;

/* Embedded RAM Configuration */
__RAM_BASE = CONFIG_SRAM_NS_BASE;
__RAM_SIZE = (CONFIG_SRAM_NS_SIZE);

/* Stack / Heap Configuration */
__STACK_SIZE = 0x00002000;
__PSTACK_SIZE = 0x00002000;
__HEAP_SIZE  = 0x00000C00;

MEMORY
{
	RAM (rwx) : ORIGIN = __RAM_BASE, LENGTH = __RAM_SIZE
}

ENTRY(Reset_Handler)

SECTIONS
{
	.text :
	{
		KEEP(*(.vectors))
		KEEP(*(.handler))
		*(.text*)

		KEEP(*(.init))
		KEEP(*(.fini))

		/* .ctors */
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)

		/* .dtors */
		*crtbegin.o(.dtors)
		*crtbegin?.o(.dtors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
		*(SORT(.dtors.*))
		*(.dtors)

		*(.rodata*)

		KEEP(*(.eh_frame*))
	} > RAM

	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > RAM

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > RAM
	__exidx_end = .;

	.zero.table :
	{
		. = ALIGN(4);
		__zero_table_start__ = .;
		__zero_table_end__ = .;
	} > RAM

	.data :
	{
		. = ALIGN(4);
		*(vtable)
		*(.data)
		*(.data.*)

		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

		KEEP(*(.jcr*))
		. = ALIGN(4);
		/* All data end */
	} > RAM

	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	} > RAM

	.pstack :
	{
		. = ALIGN(8);
		__PspStackLimit = .;
		. = . + __PSTACK_SIZE;
		. = ALIGN(8);
		__PspStackTop = .;
	} > RAM

	.heap :
	{
		. = ALIGN(8);
		__end__ = .;
		PROVIDE(end = .);
		. = . + __HEAP_SIZE;
		. = ALIGN(8);
		__HeapLimit = .;
	} > RAM

	.stack (ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE) :
	{
		. = ALIGN(8);
		__StackLimit = .;
		. = . + __STACK_SIZE;
		. = ALIGN(8);
		__StackTop = .;
	} > RAM
	PROVIDE(__stack = __StackTop);

	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
