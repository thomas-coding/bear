
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "data-type.h"

#define INTR_NUM_MAX		39
#define PRIORITY_GROUP		5

typedef void (*isr_handler_t)(u32, void *);

enum isr_type_t {
	ISR_TYPE_IRQ = 0,
	ISR_TYPE_FIQ = 1
};

/* struct israction_s - per interrupt action descriptor */
struct israction_s {
	isr_handler_t handler;		/* interrupt handler function */
	void *dev_id;			/* used to identify the device */
};

void nvic_init(void);

/*
 * The parameter "isr_type" is useless in this project, just to keep the same function format
 * as other Bare-Metal projects.
 */
void request_irq(u32 irq_num, enum isr_type_t isr_type,
	isr_handler_t cur_src_isr, void *dev_id);
void free_irq(u32 irq_num);

#endif /* __INTERRUPT_H__ */
