
#include "platform.h"
#include "interrupt.h"

static struct israction_s intr_tab[INTR_NUM_MAX] = {NULL};

/* Reads the active registers in the NVIC and return the active interrupt ID */
static u32 nvic_active_id_get(void)
{
	u32 iabr_num = 0;
	u32 iabr_off = 0;

	for (iabr_num = 0; iabr_num < 16; iabr_num++) {
		if (NVIC->IABR[iabr_num]) {
			for (iabr_off = 0; iabr_off < 32; iabr_off++) {
				if (NVIC->IABR[iabr_num] & (1 << iabr_off))
					return (iabr_num * 32 + iabr_off);
			}
		}
	}

	return 0xffffffff;
}

__attribute__((interrupt)) void ext_intr_handler(void)
{
	struct israction_s *action = NULL;
	u32 intr_id = nvic_active_id_get();

	if (intr_id != 0xffffffff) {
		action = &intr_tab[intr_id];

		if (action->handler != NULL)
			action->handler(intr_id, action->dev_id);
	}
}

void nvic_init(void)
{
	NVIC_SetPriorityGrouping(PRIORITY_GROUP);
}

void request_irq(u32 irq_num, enum isr_type_t isr_type,
	isr_handler_t cur_src_isr, void *dev_id)
{
	NVIC_SetPriority(irq_num, NVIC_EncodePriority(PRIORITY_GROUP, 2, 1));

	/*
	 * Set interrupt vector, all external interrupt route to
	 * ext_intr_handler.
	 */
	NVIC_SetVector(irq_num, (u32)ext_intr_handler);

	intr_tab[irq_num].handler = cur_src_isr;
	intr_tab[irq_num].dev_id = dev_id;

	NVIC_EnableIRQ(irq_num);
}

void free_irq(u32 irq_num)
{
	NVIC_DisableIRQ(irq_num);
	NVIC_SetVector(irq_num, (u32)0x00000000);

	intr_tab[irq_num].handler = NULL;
	intr_tab[irq_num].dev_id = NULL;
}
