
#include "platform.h"
#include "common.h"

extern void *verify_callback[][2];
static u32 verify_item_num;
static u32 pseudo_seed;

static void print_test_info(void)
{
	char c = 0;
	u8 i = 0;

	vs_printf("\n*******************************************\n");
	vs_printf("please input test module ID:\n");
	while (1) {
		if ((u32)(verify_callback[i][0] == 0))
			break;

		c = (i > 9 ? (i + 87) : (i + 48));
		vs_printf("%c -- %s\n", c, verify_callback[i][1]);
		i++;
	}

	verify_item_num = i;

	vs_printf("> ");
}

void vs_verify_loop(void)
{
	u8 c = 0;
	u8 item_id = 0;
	pfunc pfunc_callback = 0;

	while (1) {
		print_test_info();

		c = vs_getc();
		vs_putc(c);

		if (c >= '0' && c <= '9') {
			item_id = c - 48;
		} else if (c >= 'A' && c <= 'Z') {
			item_id = c - 55;
		} else if (c >= 'a' && c <= 'z') {
			item_id = c - 87;
		} else {
			vs_printf("The input is wrong, try again!\n");
			continue;
		}

		if (item_id < verify_item_num) {
			vs_printf("\n******* %s run Start *******\n",
					verify_callback[item_id][1]);

			pfunc_callback = verify_callback[item_id][0];
			if (pfunc_callback != 0)
				pfunc_callback();

			vs_printf("\n******* %s run End *******\n",
					verify_callback[item_id][1]);
		} else {
			vs_printf("There is no such verify item!\n");
		}
	}
}

u32 vs_random(void)
{
	pseudo_seed = pseudo_seed * 1103515245 + 12345;

	return((u32)(pseudo_seed / 65536) % 32768);
}

void vs_srandom(u32 seed)
{
	pseudo_seed = seed;
}

static void svc(void)
{
	__ASM volatile("svc 0x01");
}

void switch_thread_unprivileged_2_privileged(void)
{
	if ((__get_CONTROL() & 0x1) == 0x0)
		return;

	svc();
}

void switch_thread_privileged_2_unprivileged(void)
{
	u32 val = __get_CONTROL();

	if ((val & 0x1) == 0x1)
		return;

	/* Switch thread mode from privileged to unprivileged */
	val |= 0x1;
	__set_CONTROL(val);
	__ISB();
}

void SVC_Handler(void)
{
	u32 val = __get_CONTROL();

	val &= ~0x1;
	__set_CONTROL(val);
	__ISB();
}

void fault_handler(u32 *fault_args)
{
	u32 stacked_r0 = 0;
	u32 stacked_r1 = 0;
	u32 stacked_r2 = 0;
	u32 stacked_r3 = 0;
	u32 stacked_r12 = 0;
	u32 stacked_lr = 0;
	u32 stacked_pc = 0;
	u32 stacked_psr = 0;
	u32 val = 0;

	stacked_r0 = ((u32)fault_args[0]);
	stacked_r1 = ((u32)fault_args[1]);
	stacked_r2 = ((u32)fault_args[2]);
	stacked_r3 = ((u32)fault_args[3]);

	stacked_r12 = ((u32)fault_args[4]);
	stacked_lr = ((u32)fault_args[5]);
	stacked_pc = ((u32)fault_args[6]);
	stacked_psr = ((u32)fault_args[7]);

	vs_printf("    Before enter fault handler:\n");
	vs_printf("\tR0    = 0x%08x\n", stacked_r0);
	vs_printf("\tR1    = 0x%08x\n", stacked_r1);
	vs_printf("\tR2    = 0x%08x\n", stacked_r2);
	vs_printf("\tR3    = 0x%08x\n", stacked_r3);
	vs_printf("\tR12   = 0x%08x\n", stacked_r12);
	vs_printf("\tLR    = 0x%08x\n", stacked_lr);
	vs_printf("\tPC    = 0x%08x\n", stacked_pc);
	vs_printf("\tPSR   = 0x%08x\n", stacked_psr);

	vs_printf("    Fault status registers in System Control Block(SCB):\n");
	/* System Handler Control and State Register */
	vs_printf("\tSHCSR = 0x%08x\n", SCB->SHCSR);
	/* Configurable Fault Status Register */
	vs_printf("\tCFSR  = 0x%08x\n", SCB->CFSR);
	/* HardFault Status Register */
	vs_printf("\tHFSR  = 0x%08x\n", SCB->HFSR);
	/* Debug Fault Status Register */
	vs_printf("\tDFSR  = 0x%08x\n", SCB->DFSR);
	/* MemManage Fault Address Register */
	vs_printf("\tMMFAR = 0x%08x\n", SCB->MMFAR);
	/* BusFault Address Register */
	vs_printf("\tBFAR  = 0x%08x\n", SCB->BFAR);
	/* Auxiliary Fault Status Register */
	vs_printf("\tAFSR  = 0x%08x\n", SCB->AFSR);

	/* Config APB2 as secure, so LAS_SYSCTL can be access in secure world */
	val = read_mreg32(LAS_SYS_CTL_BASE_ADD + 0x14);
	if (val & (1 << 6)) {
		val &= ~(1 << 6);
		write_mreg32(LAS_SYS_CTL_BASE_ADD + 0x14, val);
	}

	vs_printf("Please run LPS-A32 Baremetal and select 'M0P/M33 system\n"
		"reset server', otherwise M33 will be halted here!!!\n");
	vs_printf("    Enter any character to software reset!\n");
	vs_getc();

	__NVIC_SystemReset();
}

void hard_fault_handler(u32 *hardfault_args)
{
	vs_printf("**************************************\n");
	vs_printf("Hard Fault handler:\n");
	fault_handler(hardfault_args);
}

void memm_fault_handler(u32 *memmfault_args)
{
	vs_printf("**************************************\n");
	vs_printf("Mem manage Fault handler:\n");
	fault_handler(memmfault_args);
}

void secure_fault_handler(u32 *securefault_args)
{
	vs_printf("**************************************\n");
	vs_printf("Secure Fault handler:\n");
	fault_handler(securefault_args);
}

/* PSP must be used in thread mode */
void fault_returnaddr_set(void)
{
	u32 frm = 0;
	u32 val = 0;

	/* only for PSP used */
	frm = __get_PSP();
	val = read_mreg32(frm + 0x18) + 0x2;
	write_mreg32(frm + 0x18, val);

	__DMB();
}

/* Get CM33 current security state: 0, Secure; 1, Non-Secure */
u32 m33_currns_get(void)
{
	u32 val = read_mreg32(LAS_SYS_CTL_BASE_ADD + 0x0c);

	if (val & 0x2)
		return 1;

	return 0;
}
