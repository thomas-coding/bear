
#ifndef __BOARD_UART_H__
#define __BOARD_UART_H__

#include "data-type.h"
#include "platform.h"
#include "ns16550.h"

/* indicating how many UART in this SoC */
#define UART_IP_NUM             4

/* indicating which UART as console */
#define CONSOLE_UART_NUM        0

/* indicating how many registers for each UART */
#define UART_REG_NUM            12

/* indicating the frequency of UART */
#define UART_CLK_FREQ           LAS_UART_CLK

/* indicating the base address of UART */
#define UART0_BASE              0x50200D00
#define UART1_BASE              0x50200C00
#define UART2_BASE              0x50200B00
#define UART3_BASE              0x50200A00

/*
 * indicating the interrupt number of each UART
 * because of GIC controller reserves the first 32 interrupt number, so
 * peripheral interrupt number must add 32 based on the value in chip spec.
 */
#define UART0_INTR_NUM          34
#define UART1_INTR_NUM          35
#define UART2_INTR_NUM          36
#define UART3_INTR_NUM          37

extern struct ns16550 uart[UART_IP_NUM];
extern struct ns16550_config ns16550_config_def;
extern const struct ns16550_reg_def ns16550_reg_def_table[UART_REG_NUM];

char console_getc(void);
char console_getc_timeout(u32 cnt);
s32 console_putc(char c);
void console_puts(const char *s);
void console_init(void);
void uart_irq_handler(u32 id, void *dev);

#endif /* __BOARD_UART_H__ */
