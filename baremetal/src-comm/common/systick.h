
#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "data-type.h"

void systick_udelay(u32 us);
void systick_mdelay(u32 ms);
void systick_sdelay(u32 s);

#endif /* __SYSTICK_H__ */
