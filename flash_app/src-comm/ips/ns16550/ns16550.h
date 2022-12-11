
#ifndef __NS16550_H__
#define __NS16550_H__

#include "data-type.h"

/* IP owner */
#define NS_IP_OWNER_VS  0x00            /* VeriSilicion */
#define NS_IP_OWNER_SNPS        0x01    /* Synopsys */

/* NS16550 specification */
#define UART_RBR                0x000   /* Receiver Buffer Register */
#define UART_THR                0x000   /* Transmitter Holding Register */
#define UART_IER                0x004   /* Interrupt Enable Register */
#define UART_IIR                0x008   /* Interrupt Identification Reg */
#define UART_FCR                0x008   /* FIFO Control Register */
#define UART_LCR                0x00c   /* Line Control Register */
#define UART_MCR                0x010   /* Modem Control Register */
#define UART_LSR                0x014   /* Line Status Register */
#define UART_MSR                0x018   /* Modem Status Register */
#define UART_SCR                0x01c   /* Scratch Register */
#define UART_DLL                0x000   /* Divisor Latch LSB */
#define UART_DLM                0x004   /* Divisor Latch MSB */

/* SNPS: Software Reset Register */
#define UART_SRR                0x088

/* UART_LCR bit define */
#define UART_LCR_WORD_LEN5      (0 << 0)
#define UART_LCR_WORD_LEN6      (1 << 0)
#define UART_LCR_WORD_LEN7      (2 << 0)
#define UART_LCR_WORD_LEN8      (3 << 0)
#define UART_LCR_STOP_1BIT      (0 << 2)
#define UART_LCR_STOP_2BIT      (1 << 2)
#define UART_LCR_PE             (1 << 3)
#define UART_LCR_EPS            (1 << 4)
#define UART_LCR_SP             (1 << 5)
#define UART_LCR_BC             (1 << 6)
#define UART_LCR_DLAB           (1 << 7)

/* UART_IER bit define */
#define UART_IER_ERDI           (1 << 0)
#define UART_IER_ETI            (1 << 1)
#define UART_IER_ERLSI          (1 << 2)
#define UART_IER_EMSI           (1 << 3)

/* UART interrupt ID */
#define UART_INTR_ID_RLSI       0x06
#define UART_INTR_ID_RDI        0x04
#define UART_INTR_ID_TIMEOUT    0x0c
#define UART_INTR_ID_TI         0x02

/* UART trigger level */
#define UART_RX_TRIGLVL0        0x00    /* 1 char in the fifo */
#define UART_RX_TRIGLVL1        0x01    /* fifo 1/4 full */
#define UART_RX_TRIGLVL2        0x02    /* fifo 1/2 full */
#define UART_RX_TRIGLVL3        0x03    /* fifo 1/8 less full */
#define UART_TX_TRIGLVL0        0x00    /* fifo empty */
#define UART_TX_TRIGLVL1        0x01    /* 2 chars in the fifo */
#define UART_TX_TRIGLVL2        0x02    /* fifo 1/4 full */
#define UART_TX_TRIGLVL3        0x03    /* fifo 1/2 full */

/* UART baud rate setting */
#define UART_BAUD_115200        115200
#define UART_BAUD_57600         57600
#define UART_BAUD_38400         38400
#define UART_BAUD_19200         19200
#define UART_BAUD_9600          9600
#define UART_BAUD_6M            6000000
#define UART_BAUD_2M            2000000
#define UART_BAUD_1M            1000000
#define UART_BAUD_DIV1          1500000
#define UART_BAUD_DIV32         3125000

/* UART Parity */
#define UART_PARITY_NONE        (0 << 3)
#define UART_PARITY_ODD         (1 << 3)
#define UART_PARITY_EVEN        (3 << 3)

struct ns16550_config {
	u32 baud_rate;
	u8 word_size;
	u8 parity;
	u8 stop_bit;
	u8 rx_trig_lvl;
	u8 tx_trig_lvl;
	u8 dma_mode;
	u8 intr_enable;
	u8 fifo_enable;
	u8 flow_ctrl_enable;
	u8 loop_back;
};

struct ns16550_priv {
	u32 intr_tflag;
	u32 intr_rflag;
	char irq_rx;
	u8 parity_en;
};

struct ns16550 {
	u32 ip_owner;
	void *base;
	u32 clk_freq;
	u8 reg_shift;
	u32 time_out;
	u8 en_irq_mode;
	u32 irq_num;
	struct ns16550_priv priv_data;
};

struct ns16550_reg_def {
	u32 off;
	u32 def;
};

s32 ns16550_uart_getc(struct ns16550 *ns_uart, char *c);
s32 ns16550_uart_getc_timeout(struct ns16550 *ns_uart, char *c, u32 timeout);
s32 ns16550_uart_putc(struct ns16550 *ns_uart, char c);
void ns16550_uart_puts(struct ns16550 *ns_uart, const char *s);
s32 ns16550_uart_init(struct ns16550 *ns_uart,
		      struct ns16550_config *config);
void ns16550_isr_handle(struct ns16550 *ns_uart);
s32 ns16550_register_default(struct ns16550 *ns_uart,
			     const struct ns16550_reg_def *table,
			     struct ns16550_reg_def *comp, u32 num);

#endif /* #ifndef __NS16550_H__ */
