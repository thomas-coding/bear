
#include "common.h"
#include "console.h"
#include "stdarg.h"

#include "tiny_command.h"
#include "qspi-flash.h"

#define DEFAULT_BAUDRATE        UART_BAUD_115200

static unsigned long tc_strtoul(const char *cp, char **endp,
				unsigned int base);

static void *map_sysmem(unsigned long vaddr, unsigned long len);
static void unmap_sysmem(void *paddr);

extern void memory_hex_dump(char* start, uint8_t *buffer, uint32_t len);

/* Command handler */
static int do_mw(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_mr(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_md(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_cmp(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_cp(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_load(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_go(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_exit(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_help(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_flash(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_boot_app(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_download_and_go(struct tiny_cmd *cmd, int argc, char *argv[]);
static int do_jump_to_non_secure_world(struct tiny_cmd *cmd, int argc, char *argv[]);

static int boot_app(uint32_t addr);

/* Command table */
struct tiny_cmd tc_cmd_tb[] = {
	TINY_CMD("mw",										   3,											      4,										   do_mw,
		 "mw addr value [width]         - Memory Write One Unit in Width"),
	TINY_CMD("mr",										   2,											      3,										   do_mr,
		 "mr addr [width]               - Memory Read One Unit in Width"),
	TINY_CMD("md",										   3,											      3,										   do_md,
		 "md addr len                   - Memory Dump in words"),
	TINY_CMD("cp",										   4,											      4,										   do_cp,
		 "cp dst src len                - Memory copy in bytes"),
	TINY_CMD("cmp",										   4,											      4,										   do_cmp,
		 "cmp addr0 addr1 len           - Memory Compare"),
	TINY_CMD("load",									   2,											      3,										   do_load,
		 "load addr [baudrate]          - UART(kermit) Load"),
	TINY_CMD("flash",									   3,											      5,										   do_flash,
		 "flash read/erase/write flash_addr len src  or flash xip enter/exit"),
	TINY_CMD("go",										   2,											      2,										   do_go,
		 "go addr                       - Jump to run"),
	TINY_CMD("exit",									   1,											      1,										   do_exit,
		 "exit                          - Exit console"),
	TINY_CMD("boot",									   2,											      2,										   do_boot_app,
		 "boot addr                     - boot flash app from address"),
	TINY_CMD("dgo",										   4,											      4,										   do_download_and_go,
		 "dgo src dst size              - download from sram(src) to flash(dst) and run"),
	TINY_CMD("ns",										   1,											      1,										   do_jump_to_non_secure_world,
		 "ns                            - jump to non-secure world"),
	TINY_CMD("help",									   1,											      2,										   do_help,
		 "help                          - Help information"),
	/* Add new command implementation here */
};

#define TC_CMD_TB_LEN   ARRAY_SIZE(tc_cmd_tb)

void idau_config(void)
{
	/* Setting SAU_CTRL.ALLNS to 1 allows the security attribution
	 * of all addresses to be set by the IDAU in the system. */
	write_mreg32(0xE000E000UL + 0x0DD0UL, 0x2);

	/* 126k secure/ 2k nsc/ other non-secure */
	write_mreg32(SYSREG2_IDAU_CTRL, 0x1);
	write_mreg32(SYSREG2_SRAM_SEC_SIZE1, 0x7E);
	write_mreg32(SYSREG2_SRAM_SEC_SIZE2, 0x10);
}

int do_jump_to_non_secure_world(struct tiny_cmd *cmd, int argc, char *argv[])
{
	/* Config idau */
	idau_config();

	/* Set non-secure world vector base address */
	*(volatile unsigned int *)0xe002ed08 = 0x20220000;

	/* Set non-secure msp */
	asm volatile ("ldr r0, =0x20220000");
	asm volatile ("ldr r1, [r0]");
	asm volatile ("msr msp_ns, r1");

	/* Get non-secure entry */
	asm volatile ("ldr r0, =0x20220004");
	asm volatile ("ldr r1, [r0]");

	/* Bit0 set to 0, indicate enter non-secure state*/
	asm volatile ("bic r1,r1,#0x1");
	asm volatile ("bxns r1");
}

/* down load image to flash and go
 * src - sram address
 * dst - flash address
 * size - image size
 */
int do_download_and_go(struct tiny_cmd *cmd, int argc, char *argv[])
{
	u32 src = 0;
	u32 dst = 0;
	unsigned long len = 0;
	struct qspi_flash_device device;

	src = tc_strtoul(argv[1], NULL, 16);
	dst = tc_strtoul(argv[2], NULL, 16);
	len = tc_strtoul(argv[3], NULL, 16);

	device.flash_base = 0x10000000;
	device.ops = &qspi_flash_ops;

	/* Exit xip, erase flash and copy image from sram to flash*/
	device.ops->qspi_flash_xip_exit(&device);
	mdelay(100);
	device.ops->qspi_flash_erase(&device, dst, len);
	mdelay(100);
	device.ops->qspi_flash_write_le(&device, (u8 *)src, dst, len);
	mdelay(100);

	/* Enter xip and run flash app */
	device.ops->qspi_flash_xip_enter(&device);
	boot_app(dst);
}

/* This function jumps to the application image. */
static void start_app(uint32_t pc __attribute__((unused)), uint32_t sp __attribute__((unused)))
{
	__asm volatile (

		/* Set stack pointer. */
		"MSR     MSP, R1                         \n"
		"DSB                                     \n"
		"ISB                                     \n"

		/* Branch to application. */
		"BX      R0                              \n"
		);
}

int boot_app(uint32_t addr)
{
	/* The vector table is located after the header. */
	uint32_t vector_table = (uint32_t)(addr);

	uint32_t * app_image = (uint32_t *)vector_table;
	uint32_t app_sp = app_image[0];
	uint32_t app_pc = app_image[1];

	vs_printf("Vector Table: 0x%x. PC=0x%x, SP=0x%x\n", (unsigned int)vector_table, (unsigned int)app_pc,
		  (unsigned int)app_sp);

	/* Set the applications vector table. */
	SCB->VTOR = vector_table;

	__set_MSPLIM(0);

	/* Set SP and branch to PC. */
	start_app(app_pc, app_sp);
}

int do_boot_app(struct tiny_cmd *cmd, int argc, char *argv[])
{
	u32 addr;

	addr = tc_strtoul(argv[1], NULL, 16);
	boot_app(addr);
}

/* flash read/write addr len */
#define DO_FLASH_READ_BUFFER_SIZE 1024
int do_flash(struct tiny_cmd *cmd, int argc, char *argv[])
{
	u32 addr, src = 0;
	unsigned long len = 0;

	struct qspi_flash_device device;
	uint8_t read_buffer[DO_FLASH_READ_BUFFER_SIZE];

	if (strncmp(argv[1], "xip", 3)) {
		addr = tc_strtoul(argv[2], NULL, 16);
		len = tc_strtoul(argv[3], NULL, 16);
	}

	device.flash_base = 0x10000000;
	device.ops = &qspi_flash_ops;

	if (!strncmp(argv[1], "read", 4)) {
		if (len > DO_FLASH_READ_BUFFER_SIZE) {
			vs_printf("mxx read size: %d\n", DO_FLASH_READ_BUFFER_SIZE);
			return RET_CMD_EXIT;
		}
		vs_printf("flash %s addr:0x%x, len:0x%x\n", argv[1], addr, len);
		device.ops->qspi_flash_read(&device, addr, read_buffer, len);
		memory_hex_dump("do flash read", read_buffer, len);
	} else if (!strncmp(argv[1], "erase", 5)) {
		vs_printf("flash %s addr:0x%x, len:0x%x\n", argv[1], addr, len);
		device.ops->qspi_flash_erase(&device, addr, len);
	} else if (!strncmp(argv[1], "write", 5)) {
		src = tc_strtoul(argv[4], NULL, 16);
		vs_printf("flash %s addr:0x%x, len:0x%x src:0x%x\n", argv[1], addr, len, src);
		/* Use the little endian, it will change the order */
		device.ops->qspi_flash_write_le(&device, (u8 *)src, addr, len);
	} else if (!strncmp(argv[1], "xip", 3)) {
		if (!strncmp(argv[2], "enter", 5)) {
			vs_printf("flash enter xip mode\n");
			device.ops->qspi_flash_xip_enter(&device);
		} else if (!strncmp(argv[2], "exit", 4)) {
			vs_printf("flash exit xip mode\n");
			device.ops->qspi_flash_xip_exit(&device);
		} else {
			vs_printf("invalid command\n");
			return RET_CMD_EXIT;
		}

	} else {
		vs_printf("invalid command\n");
		return RET_CMD_EXIT;
	}

	vs_printf("flash %s done\n", argv[1]);

	return RET_CMD_DONE;
}



int do_cp(struct tiny_cmd *cmd, int argc, char *argv[])
{
	u32 src = 0;
	u32 dst = 0;
	unsigned long len = 0;

	dst = tc_strtoul(argv[1], NULL, 16);
	src = tc_strtoul(argv[2], NULL, 16);
	len = tc_strtoul(argv[3], NULL, 16);

	memcpy((void *)dst, (void *)src, len);

	return RET_CMD_DONE;
}

/*
 * Exit console loop and continue to execute the following flow.
 */
int do_exit(struct tiny_cmd *cmd, int argc, char *argv[])
{
	/* Notify the console loop to break. */
	tc_pr("Goodbye tiny console\n");

	return RET_CMD_EXIT;
}

/*
 * Help
 */
static int do_help(struct tiny_cmd *cmd, int argc, char *argv[])
{
	int i = 0;

	for (i = 0; i < TC_CMD_TB_LEN; i++)
		tc_pr("%s\n", tc_cmd_tb[i].usage);

	return RET_CMD_DONE;
}

/*
 * Memory Write
 * - mw addr value [width]
 */
static int do_mw(struct tiny_cmd *cmd, int argc, char *argv[])
{
	unsigned int width = 32;
	unsigned long writeval = 0;
	unsigned long addr = 0;
	void *buf = 0;

	addr = tc_strtoul(argv[1], NULL, 16);

	if (argc == 4)
		width = tc_strtoul(argv[3], NULL, 10);

	if (width != 8 && width != 16 && width != 32)
		return RET_CMD_USAGE;

	writeval = tc_strtoul(argv[2], NULL, 16);
	buf = map_sysmem(addr, 4);

	if (width == 8)
		*(volatile u8*)buf = (u8)writeval;
	else if (width == 16)
		*(volatile u16*)buf = (u16)writeval;
	else
		*(volatile u32*)buf = (u32)writeval;

	unmap_sysmem(buf);

	return RET_CMD_DONE;
}

/*
 * Memory Read
 * - mr addr [width]
 */
static int do_mr(struct tiny_cmd *cmd, int argc, char *argv[])
{
	unsigned long addr = 0, width = 32;
	void *buf;

	addr = tc_strtoul(argv[1], NULL, 16);

	if (argc == 3)
		width = tc_strtoul(argv[2], NULL, 10);

	if (width != 8 && width != 16 && width != 32)
		return RET_CMD_USAGE;

	buf = map_sysmem(addr, 4);

	if (width == 8)
		tc_pr("0x%02x @ 0x%lx\n", *(u8 *)buf, addr);
	else if (width == 16)
		tc_pr("0x%04x @ 0x%lx\n", *(u16 *)buf, addr);
	else
		tc_pr("0x%08x @ 0x%lx\n", *(u32 *)buf, addr);

	unmap_sysmem(buf);

	return RET_CMD_DONE;
}

/*
 * Memory Display
 * - md addr len
 */
static int do_md(struct tiny_cmd *cmd, int argc, char *argv[])
{
	unsigned long addr = 0, length = 1, bytes = 0;
	unsigned int *buf;
	int i = 0;

	addr = tc_strtoul(argv[1], NULL, 16);
	length = tc_strtoul(argv[2], NULL, 16);

	bytes = length << 2;
	buf = (unsigned int *)map_sysmem(addr, bytes);

	for (i = 0; i < length; i++) {
		tc_pr("0x%08x @ 0x%lx\n", *buf, (unsigned long)buf);
		buf++;
	}

	unmap_sysmem(buf);

	return RET_CMD_DONE;
}

/*
 * Kermit Download
 */
extern void kermit_load(unsigned long offset, unsigned int baudrate);
unsigned int baudrate_reconfig(unsigned int baudrate)
{
	tc_pr("## Switch baudrate to %d bps and press ESC ...\n",
	      baudrate);

	/* To avoid scrambled code */
	while (1)
		if (console_getc() == 0x1b)
			break;

	/* Todo : reconfig baudrate */
	ns16550_config_def.baud_rate = baudrate;
	ns16550_uart_init(&uart[CONSOLE_UART_NUM], &ns16550_config_def);

	return baudrate;
}

static int do_load(struct tiny_cmd *cmd, int argc, char *argv[])
{
	unsigned long addr = 0;
	unsigned int bps = DEFAULT_BAUDRATE;

	addr = tc_strtoul(argv[1], NULL, 16);

	if (argc > 2)
		bps = (unsigned int)tc_strtoul(argv[2], NULL, 10);

	/* Update bps */
	if (bps != DEFAULT_BAUDRATE)
		bps = baudrate_reconfig(bps);

	kermit_load(addr, bps);

	/* Resume bps */
	if (bps != DEFAULT_BAUDRATE)
		bps = baudrate_reconfig(DEFAULT_BAUDRATE);

	return RET_CMD_DONE;
}

/*
 * Jump and run
 */
static int do_go(struct tiny_cmd *cmd, int argc, char *argv[])
{
	unsigned long addr = 0;
	func_entry payload_entry = NULL;

	addr = tc_strtoul(argv[1], NULL, 16);

	payload_entry = (func_entry)addr;

	tc_pr("Goodbye boot console, Jump to 0x%lx to run\n", addr);

	payload_entry();
	/* TODO: invalidate icache */

	return RET_CMD_DONE;
}

/*
 * Memory compare by byte.
 */
static int do_cmp(struct tiny_cmd *cmd, int argc, char *argv[])
{
	unsigned long addr0 = 0, addr1 = 0;
	unsigned char *buf0 = NULL, *buf1 = NULL;
	unsigned int bytes = 0, i = 0;

	addr0 = tc_strtoul(argv[1], NULL, 16);
	addr1 = tc_strtoul(argv[2], NULL, 16);
	bytes = tc_strtoul(argv[3], NULL, 16);

	buf0 = (unsigned char *)map_sysmem(addr0, bytes);
	buf1 = (unsigned char *)map_sysmem(addr1, bytes);

	for (i = 0; i < bytes; i++) {
		if (*buf0 != *buf1)
			break;
		buf0++;
		buf1++;
	}

	if (i == bytes)
		tc_pr("0x%lx is same as 0x%lx within %d bytes.\n",
		      addr0, addr1, bytes);
	else
		tc_pr("Difference between 0x%lx(0x%02x) and 0x%lx(0x%02x)\n",
		      addr0 + i, *buf0, addr1 + i, *buf1);

	return RET_CMD_DONE;
}

struct tiny_cmd *find_cmd(const char *name)
{
	int i = 0;

	for (i = 0; i < TC_CMD_TB_LEN; i++) {
		if (!strncmp(name, tc_cmd_tb[i].name,
			     strlen(tc_cmd_tb[i].name)))
			break;
	}

	if (i < TC_CMD_TB_LEN)
		return &tc_cmd_tb[i];

	return NULL;
}

static const char *parse_int(const char *s, unsigned int *base)
{
	if (*base == 0) {
		if (s[0] == '0') {
			if (tc_tolower(s[1]) == 'x' && tc_isxdigit(s[2]))
				*base = 16;
			else
				*base = 8;
		} else
			*base = 10;
	}

	if (*base == 16 && s[0] == '0' && tc_tolower(s[1]) == 'x')
		s += 2;

	return s;
}

static unsigned long tc_strtoul(const char *cp, char **endp,
				unsigned int base)
{
	unsigned long result = 0;
	unsigned long value;

	cp = parse_int(cp, &base);

	while (tc_isxdigit(*cp) &&
	       (value = tc_isdigit(*cp) ? *cp - '0' : (tc_islower(*cp)
		? tc_toupper(*cp) : *cp) - 'A' + 10) < base) {
		result = result * base + value;
		cp++;
	}

	if (endp)
		*endp = (char *)cp;

	return result;
}

static void *map_sysmem(unsigned long vaddr, unsigned long len)
{
	/* Return physical address*/
	return (void *)vaddr;
}

static void unmap_sysmem(void *vaddr)
{
	/* TODO: Do some resource release is necessary */
}
