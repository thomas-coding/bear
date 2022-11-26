/*
 * Contains the test code for NS16550 UART IP.
 *
 * Author: Jianheng.Zhang <zhang.jianheng@verisilicon.com>
 *         Zhigang.Kang <kang.zhigang@verisilicon.com>
 *
 * Copyright (C) 2021 VeriSilicon Microelectronics (Shanghai) Co., Ltd.
 *
 */

#include "common.h"
#include "ns16550.h"
#include "board-ns16550.h"
#include "t-ns16550.h"

//#define NS16550_REG_DEFAULT_TEST

#define TDLEN           16

struct uart_test_item {
	char *test_name;
	void (*test_func)(struct ns16550 *ns_uart);
};

struct uart_test_table {
	u32 id;
	char *name;
	struct uart_test_item *uart_test_item;
};

static void ns16550_test_irq_polling(struct ns16550 *ns_uart);
static void ns16550_test_baudrate(struct ns16550 *ns_uart);
static void ns16550_test_word_size(struct ns16550 *ns_uart);
static void ns16550_test_parity(struct ns16550 *ns_uart);
static void ns16550_test_stopbit(struct ns16550 *ns_uart);
static void ns16550_test_flowctrl(struct ns16550 *ns_uart);
void ns16550_test_loopback(struct ns16550 *ns_uart);

struct uart_test_item uart0_test_items[] = {
	{ "loop back",	  ns16550_test_loopback	   },
	{ "uart mode",	  ns16550_test_irq_polling },
	{ "baudrate",	  ns16550_test_baudrate	   },
	{ "word size",	  ns16550_test_word_size   },
	{ "parity",	  ns16550_test_parity	   },
	{ "stop bit",	  ns16550_test_stopbit	   },
	{ "flow control", ns16550_test_flowctrl	   },
	{ NULL }
};

struct uart_test_item uart1_test_items[] = {
	{ "loop back", ns16550_test_loopback	},
	{ "uart mode", ns16550_test_irq_polling },
	{ "baudrate",  ns16550_test_baudrate	},
	{ "word size", ns16550_test_word_size	},
	{ "parity",    ns16550_test_parity	},
	{ "stop bit",  ns16550_test_stopbit	},
	{ NULL }
};

struct uart_test_table uart_test_tb[] = {
	{ 0, "uart0", uart0_test_items },
	{ 1, "uart1", uart1_test_items },
	{ 0, NULL }
};

char data[TDLEN] = { 'A', 'B', 'C', 'D', 'E',  'F', 'G', 'H', 'I', 'J',
		     'K', 'L', 'M', 'N', '\n', '\r' };

/*
 * Use for user's reply, y or n.
 * Waiting for user to input with an end of ENTER key.
 * If it's 'y', return 1; else if 'n', return 0; others 2.
 */
u32 wait_for_reply(struct ns16550 *ns_uart)
{
	u8 i = 0;
	char ch = 0;
	char answer[20] = { 0 };

	for (i = 0; i < 20; i++)
		answer[i] = '\0';

	i = 0;

	while (1) {
		if (ns16550_uart_getc(ns_uart, &ch) == 0) {
			if (ch == '\n' || ch == '\r') {
				answer[i] = '\0';
				break;
			}

			if (ch == '\b') {
				if (i > 0) {
					i--;
					ns16550_uart_putc(ns_uart, ch);
				}
			} else {
				answer[i++] = ch;
				ns16550_uart_putc(ns_uart, ch);
			}
		}
	}

	ns16550_uart_putc(ns_uart, '\n');
	ns16550_uart_putc(ns_uart, '\r');

	if ((i == 1) && (answer[0] == 'y'))
		return 1;
	else if ((i == 1) && (answer[0] == 'n'))
		return 0;

	return 2;
}

/*
 * ns16550_test_irq_polling: Tests query and interrupt mode of UART
 */
static void ns16550_test_irq_polling(struct ns16550 *ns_uart)
{
	u32 i = 0;
	u32 get = 0;
	u32 mode = 0;
	struct ns16550_config config = { 0 };
	u8 intr_enable[] = { 1, 0 };
	char * const uart_mode_str_tb[] = { "interrupt", "query" };

	memcpy(&config, &ns16550_config_def, sizeof(config));
	ns16550_uart_init(ns_uart, &config);
	ns16550_uart_puts(ns_uart, "\n\t- - - Testing uart mode...\n");

	for (mode = 0; mode < sizeof(intr_enable); mode++) {
		ns16550_uart_puts(ns_uart, "\tUart mode is ");
		ns16550_uart_puts(ns_uart, uart_mode_str_tb[mode]);
		ns16550_uart_puts(ns_uart, "\n");

		mdelay(5);

		if (intr_enable[mode])
			request_irq(ns_uart->irq_num, ISR_TYPE_IRQ,
				    uart_irq_handler, ns_uart);
		else
			free_irq(ns_uart->irq_num);

		config.intr_enable = intr_enable[mode];
		ns16550_uart_init(ns_uart, &config);

		ns16550_uart_puts(ns_uart, "Output is---\n\t");
		for (i = 0; i < TDLEN; i++)
			ns16550_uart_putc(ns_uart, data[i]);

		ns16550_uart_puts(ns_uart, "- - - [y/n] ");
		while (1) {
			get = wait_for_reply(ns_uart);

			if ((get != 1) && (get != 0))
				ns16550_uart_puts(ns_uart,
						  "\n\tPlease enter 'y' or 'n'   ");
			else
				break;
		}

		if (get == 1)
			ns16550_uart_puts(ns_uart, "\t- - -PASS\n");
		else
			ns16550_uart_puts(ns_uart, "\t- - -FAILURE\n");
	}
}

/*
 * ns16550_test_baudrate: Tests baudrate of UARTs.
 */
static void ns16550_test_baudrate(struct ns16550 *ns_uart)
{
	u32 i = 0;
	u32 baudrate = 0;
	u32 get = 0;
	struct ns16550_config config = { 0 };
	u32 bard_tb[] = { UART_BAUD_9600, UART_BAUD_38400, UART_BAUD_57600,
			  UART_BAUD_DIV1, UART_BAUD_115200 };
	char * const bard_str_tb[] = { "9600",		"38400", "57600",
				       "1500000(DIV1)", "115200" };

	memcpy(&config, &ns16550_config_def, sizeof(config));
	ns16550_uart_init(ns_uart, &config);
	ns16550_uart_puts(ns_uart, "\n\t- - - Test uart baudrate.\n");

	for (baudrate = 0; baudrate < sizeof(bard_tb) / 4; baudrate++) {
		ns16550_uart_puts(ns_uart, "\tBaudrate is ");
		ns16550_uart_puts(ns_uart, bard_str_tb[baudrate]);
		ns16550_uart_puts(ns_uart, "? [y] ");

		mdelay(10);

		config.baud_rate = bard_tb[baudrate];
		ns16550_uart_init(ns_uart, &config);

		while (wait_for_reply(ns_uart) != 1)
			ns16550_uart_puts(ns_uart, "\n\tPlease enter 'y'   ");

		ns16550_uart_puts(ns_uart, " :Output is ---\n\t\t");
		for (i = 0; i < TDLEN; i++)
			ns16550_uart_putc(ns_uart, data[i]);

		ns16550_uart_puts(ns_uart, "- - -[y/n] ");
		while (1) {
			get = wait_for_reply(ns_uart);

			if ((get != 1) && (get != 0))
				ns16550_uart_puts(ns_uart,
						  "\n\tPlease enter 'y' or 'n'   ");
			else
				break;
		}

		if (get == 1)
			ns16550_uart_puts(ns_uart, "\t- - -PASS\n");
		else
			ns16550_uart_puts(ns_uart, "\t- - -FAILURE\n");
	}
}

/*
 * ns16550_test_parity: Tests number of data bits per character(5-8)
 */
static void ns16550_test_word_size(struct ns16550 *ns_uart)
{
	char c = 0;
	u8 wordsize = 0;
	u8 first = 2;
	u32 i = 0;
	u32 get = 0;
	struct ns16550_config config = { 0 };
	u8 word_len[] = { UART_LCR_WORD_LEN5, UART_LCR_WORD_LEN6,
			  UART_LCR_WORD_LEN7, UART_LCR_WORD_LEN8 };
	char * const word_len_str_tb[] = { "5", "6", "7", "8" };

	memcpy(&config, &ns16550_config_def, sizeof(config));
	ns16550_uart_init(ns_uart, &config);
	ns16550_uart_puts(ns_uart, "\n\t- - - Test word size.\n");

	if (first < 2) {
		ns16550_uart_puts(ns_uart, "\nNote: If test 5/6bits, please ");
		ns16550_uart_puts(ns_uart, "use 'vs-uart-test' to ");
		ns16550_uart_puts(ns_uart, "continue the test.\n");
		ns16550_uart_puts(ns_uart, "run './vs-uart-test ");
		ns16550_uart_puts(ns_uart, "/dev/ttyUSBx 5 1 0x**'\n");
		ns16550_uart_puts(ns_uart, "or\n");
		ns16550_uart_puts(ns_uart, "run './vs-uart-test ");
		ns16550_uart_puts(ns_uart, "/dev/ttyUSBx 6 1 0x**'\n");
	}

	for (wordsize = first; wordsize < sizeof(word_len); wordsize++) {
		if (wordsize == 0 || wordsize == 1) {
			mdelay(10);

			config.word_size = word_len[wordsize];
			ns16550_uart_init(ns_uart, &config);

			ns16550_uart_getc(ns_uart, &c);
			for (i = 0; i < 8; i++)
				ns16550_uart_putc(ns_uart, c);

			if (wordsize == 1) {
				ns16550_uart_putc(ns_uart, 0x1a);
				ns16550_uart_putc(ns_uart, 0x1b);
				ns16550_uart_putc(ns_uart, 0x1c);
				ns16550_uart_putc(ns_uart, 0x1d);
			}
		} else {
			if (wordsize == 3 || first == 2) {
				ns16550_uart_puts(ns_uart, "\tWord size is ");
				ns16550_uart_puts(ns_uart,
						  word_len_str_tb[wordsize]);
				ns16550_uart_puts(ns_uart, "? [y] ");
			}

			mdelay(10);

			config.word_size = word_len[wordsize];
			ns16550_uart_init(ns_uart, &config);

			while (wait_for_reply(ns_uart) != 1)
				ns16550_uart_puts(ns_uart,
						  "\n\tPlease enter 'y' ");

			ns16550_uart_puts(ns_uart, " :Output is ---\n\t\t");
			for (i = 0; i < TDLEN; i++)
				ns16550_uart_putc(ns_uart, data[i]);

			ns16550_uart_puts(ns_uart, "- - -[y/n] ");
			while (1) {
				get = wait_for_reply(ns_uart);

				if ((get != 1) && (get != 0))
					ns16550_uart_puts(ns_uart,
							  "\n\tPlease enter 'y' or 'n' ");
				else
					break;
			}

			if (get == 1)
				ns16550_uart_puts(ns_uart, "\t- - -PASS\n");
			else
				ns16550_uart_puts(ns_uart, "\t- - -FAILURE\n");
		}
	}
}

/*
 * ns16550_test_parity: Tests Parity of uarts
 */
static void ns16550_test_parity(struct ns16550 *ns_uart)
{
	u8 parity = 0;
	u32 i = 0;
	u32 get = 0;
	struct ns16550_config config = { 0 };
	u8 parity_tb[] = { UART_PARITY_ODD, UART_PARITY_EVEN, UART_PARITY_NONE };
	char * const parity_str_tb[] = { "ODD", "EVEN", "NONE" };

	memcpy(&config, &ns16550_config_def, sizeof(config));
	ns16550_uart_init(ns_uart, &config);
	ns16550_uart_puts(ns_uart, "\n\t- - - Test uart parity.\n");

	for (parity = 0; parity < sizeof(parity_tb); parity++) {
		ns16550_uart_puts(ns_uart, "\tParity is ");
		ns16550_uart_puts(ns_uart, parity_str_tb[parity]);
		ns16550_uart_puts(ns_uart, "? [y] ");

		mdelay(10);

		config.parity = parity_tb[parity];
		ns16550_uart_init(ns_uart, &config);

		while (wait_for_reply(ns_uart) != 1)
			ns16550_uart_puts(ns_uart, "\n\tPlease enter 'y'   ");

		ns16550_uart_puts(ns_uart, " :Output is ---\n\t\t");
		for (i = 0; i < TDLEN; i++)
			ns16550_uart_putc(ns_uart, data[i]);

		ns16550_uart_puts(ns_uart, "- - -[y/n] ");
		while (1) {
			get = wait_for_reply(ns_uart);

			if ((get != 1) && (get != 0))
				ns16550_uart_puts(ns_uart,
						  "\n\tPlease enter 'y' or 'n'   ");
			else
				break;
		}

		if (get == 1)
			ns16550_uart_puts(ns_uart, "\t- - -PASS\n");
		else
			ns16550_uart_puts(ns_uart, "\t- - -FAILURE\n");
	}

}

/*
 * ns16550_test_stopbit: Tests Stop Bit of UARTs.
 */
static void ns16550_test_stopbit(struct ns16550 *ns_uart)
{
	char c = 0;
	u8 stopbit = 0;
	u8 first = 1;
	u32 i = 0;
	u32 get = 0;
	struct ns16550_config config = { 0 };
	u8 stop_cfg[] = { UART_LCR_STOP_2BIT, UART_LCR_STOP_2BIT,
			  UART_LCR_STOP_1BIT };
	char * const stop_str_tb[] = { "1.5 bits", "2 bits", "1 bit" };

	memcpy(&config, &ns16550_config_def, sizeof(config));
	ns16550_uart_init(ns_uart, &config);
	ns16550_uart_puts(ns_uart, "\n- - - Test Stop Bit.\n");

	if (first < 1) {
		ns16550_uart_puts(ns_uart, "\nNote: If test 1.5 stop bits, ");
		ns16550_uart_puts(ns_uart, "please use 'vs-uart-test");
		ns16550_uart_puts(ns_uart, "'to continue the test.\n");
		ns16550_uart_puts(ns_uart, "run './vs-uart-test ");
		ns16550_uart_puts(ns_uart, "/dev/ttyUSBx 5 2 0x**'\n");
	}

	for (stopbit = first; stopbit < sizeof(stop_cfg); stopbit++) {
		if (stopbit == 0) {
			mdelay(10);

			config.word_size = UART_LCR_WORD_LEN5;
			config.stop_bit = stop_cfg[stopbit];
			ns16550_uart_init(ns_uart, &config);

			ns16550_uart_getc(ns_uart, &c);
			for (i = 0; i < 8; i++)
				ns16550_uart_putc(ns_uart, c);

			ns16550_uart_putc(ns_uart, 0x1a);
			ns16550_uart_putc(ns_uart, 0x1b);
			ns16550_uart_putc(ns_uart, 0x1c);
			ns16550_uart_putc(ns_uart, 0x1d);
		} else {
			if (stopbit == 2 || first == 1) {
				ns16550_uart_puts(ns_uart, "\tstopbit is ");
				ns16550_uart_puts(ns_uart,
						  stop_str_tb[stopbit]);
				ns16550_uart_puts(ns_uart, "? [y] ");
			}
			mdelay(10);

			config.word_size = UART_LCR_WORD_LEN8;
			config.stop_bit = stop_cfg[stopbit];
			ns16550_uart_init(ns_uart, &config);

			while (wait_for_reply(ns_uart) != 1)
				ns16550_uart_puts(ns_uart,
						  "\n\tPlease enter 'y' ");

			ns16550_uart_puts(ns_uart, " :Output is ---\n\t\t");
			for (i = 0; i < TDLEN; i++)
				ns16550_uart_putc(ns_uart, data[i]);

			ns16550_uart_puts(ns_uart, "- - -[y/n] ");
			while (1) {
				get = wait_for_reply(ns_uart);

				if ((get != 1) && (get != 0))
					ns16550_uart_puts(ns_uart,
							  "\n\tPlease enter 'y' or 'n' ");
				else
					break;
			}

			if (get == 1)
				ns16550_uart_puts(ns_uart, "\t- - -PASS\n");
			else
				ns16550_uart_puts(ns_uart, "\t- - -FAILURE\n");
		}
	}
}

/*
 * ns16550_test_flowctrl: Tests Flow Control of UARTs.
 */
static void ns16550_test_flowctrl(struct ns16550 *ns_uart)
{
	u8 flowctrl = 0;
	u32 i = 0;
	u32 get = 0;
	struct ns16550_config config = { 0 };
	u8 fc_test[] = { 1, 0 };
	char * const fc_str_tb[] = { "enabled", "disabled" };

	memcpy(&config, &ns16550_config_def, sizeof(config));
	ns16550_uart_init(ns_uart, &config);
	ns16550_uart_puts(ns_uart, "\n\t- - - Test Flow Control.\n");

	for (flowctrl = 0; flowctrl < sizeof(fc_test); flowctrl++) {
		ns16550_uart_puts(ns_uart, "\tflowctrl is ");
		ns16550_uart_puts(ns_uart, fc_str_tb[flowctrl]);
		ns16550_uart_puts(ns_uart, "? [y] ");

		mdelay(10);

		config.flow_ctrl_enable = fc_test[flowctrl];
		/*
		 * If FIFOs are not implemented, Flow Control can't be selected
		 */
		config.fifo_enable = config.flow_ctrl_enable;
		ns16550_uart_init(ns_uart, &config);

		while (wait_for_reply(ns_uart) != 1)
			ns16550_uart_puts(ns_uart, "\n\tPlease enter 'y'   ");

		ns16550_uart_puts(ns_uart, " :Output is ---\n\t\t");
		for (i = 0; i < TDLEN; i++)
			ns16550_uart_putc(ns_uart, data[i]);

		ns16550_uart_puts(ns_uart, "- - -[y/n] ");
		while (1) {
			get = wait_for_reply(ns_uart);

			if ((get != 1) && (get != 0))
				ns16550_uart_puts(ns_uart,
						  "\n\tPlease enter 'y' or 'n'   ");
			else
				break;
		}

		if (get == 1)
			ns16550_uart_puts(ns_uart, "\t- - -PASS\n");
		else
			ns16550_uart_puts(ns_uart, "\t- - -FAILURE\n");
	}
}

/*
 * ns16550_test_loopback: Tests LoopBack Bit of UARTs.
 */
void ns16550_test_loopback(struct ns16550 *ns_uart)
{
	char sch = 0, rch = 0;
	u8 i = 0;
	u32 ret = 0;
	struct ns16550_config config = { 0 };

	//vs_printf("\n\t- - - Test loop back.\n");

	mdelay(10);

	memcpy(&config, &ns16550_config_def, sizeof(config));
	config.loop_back = 1;
	ns16550_uart_init(ns_uart, &config);

	sch = 'A';
	for (i = 0; i < 20; i++) {
		ns16550_uart_putc(ns_uart, sch);
		ns16550_uart_getc(ns_uart, &rch);

		if (sch != rch)
			ret = 1;

		sch++;
	}

	mdelay(10);

	memcpy(&config, &ns16550_config_def, sizeof(config));
	ns16550_uart_init(ns_uart, &config);

	if (ret)
		vs_printf("\t- - -FAILURE\n");
	else
		vs_printf("\t- - -PASS\n");
}

#ifdef NS16550_REG_DEFAULT_TEST
/*
 * ns16550_test_reg_default: Tests register default value of UARTs.
 */
static void ns16550_test_reg_default(struct ns16550 *ns_uart, u32 ns_id)
{
	u32 i = 0;
	u32 ret = 0;
	u32 val = 0;
	struct ns16550_reg_def comp[UART_REG_NUM];

	vs_printf("\n\t- - - Test register default value.\n");

	mdelay(10);
	/* UARTs reset */
	val = read_mreg32(LAS_SW_RST);
	val |= LAS_UART_SW_RESET;
	write_mreg32(LAS_SW_RST, val);
	val &= ~LAS_UART_SW_RESET;
	write_mreg32(LAS_SW_RST, val);

	memset(comp, 0x00, sizeof(comp));

	ret = ns16550_register_default(ns_uart, ns16550_reg_def_table,
				       comp, UART_REG_NUM);

	if (ns_id == CONSOLE_UART_NUM) {
		mdelay(10);
		ns16550_uart_init(&uart[CONSOLE_UART_NUM], &ns16550_config_def);
	}

	if (ret == 0) {
		vs_printf("\tUART_%d - - - PASS\n", ns_id);
		return;
	}

	vs_printf("\tUART_%d - - - FAIL\n", ns_id);
	vs_printf("\t\tOff\tDef\t\tRead\n");
	for (i = 0; i < UART_REG_NUM; i++) {
		if (comp[i].off == 0xffffffff) {
			vs_printf("\t\t0x%02x\t%08x\t%08x\n",
				  ns16550_reg_def_table[i].off,
				  ns16550_reg_def_table[i].def,
				  comp[i].def);
		}
	}
}
#endif

void ns16550_test(void)
{
	u32 get = 0;
	struct uart_test_table *uart_test = uart_test_tb;
	struct uart_test_item *uart_test_item = NULL;

	vs_printf("Testing uart...\n");
	vs_printf("Default configure:\n\t Baudrate --- 115200\n"
		  "\t Parity --- NONE\n"
		  "\t Wordsize --- 8\n");

	while (uart_test->name != NULL) {
		vs_printf("Testing %s...\n", uart_test->name);

#ifdef NS16550_REG_DEFAULT_TEST
		ns16550_test_reg_default(&uart[uart_test->id], uart_test->id);
#endif
		if (uart_test->id == 0) {
			vs_printf("Connect  UART_to_USB board and Alius\n");
			vs_printf("J1 PIN8 to J566 PIN8(TX),\n");
			vs_printf("J1 PIN7 to J566 PIN6(RX)\n");
			vs_printf("J1 PIN5 to J566 PIN9(GND)\n");
			vs_printf("J1 PIN4 to J566 PIN4(CTS)\n");
			vs_printf("J1 PIN3 to J566 PIN2(RTS)\n");
		}

		vs_printf(" - - - UART%d ready?[y] ", uart_test->id);
		while (1) {
			get = wait_for_reply(&uart[CONSOLE_UART_NUM]);
			if ((get != 1))
				vs_printf("\nPlease enter 'y'	 ");
			else
				break;
		}

		uart_test_item = uart_test->uart_test_item;
		while (uart_test_item->test_name != NULL) {
			vs_printf("\n \tStart Uart Test: %s\n",
				  uart_test_item->test_name);
			mdelay(100);
			if (uart_test_item->test_func)
				uart_test_item->test_func(&uart[uart_test->id]);

			uart_test_item++;
		}

		uart_test++;
	}
}
