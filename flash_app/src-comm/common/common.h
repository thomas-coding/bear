
#ifndef __COMMON_H__
#define __COMMON_H__

#include "data-type.h"
#include "systick.h"
#include "interrupt.h"
#include "string.h"

/* Is there a UART? */
#define UART_IS_OK
#ifdef UART_IS_OK
#include "board-ns16550.h"
#include "console.h"

extern char console_getc(void);
extern s32 console_putc(char c);
extern void console_puts(const char *s);
#endif

/* Is there a TIMER? */
/* #define TIMER_IS_OK */

#define read_mreg64(addr)               (*(volatile u64 *)(addr))
#define write_mreg64(addr, val)         \
	(*(volatile u64 *)(addr) = (u64)(val))

#define read_mreg32(addr)               (*(volatile u32 *)(addr))
#define write_mreg32(addr, val)         \
	(*(volatile u32 *)(addr) = (u32)(val))

#define read_mreg16(addr)               (*(volatile u16 *)(addr))
#define write_mreg16(addr, val)         \
	(*(volatile u16 *)(addr) = (u16)(val))

#define read_mreg8(addr)                (*(volatile u8 *)(addr))
#define write_mreg8(addr, val)          \
	(*(volatile u8 *)(addr) = (u8)(val))

#define BIT(nr)                         (1UL << (nr))

#define MIN(a, b) ({                    \
		typeof(a) _min1 = (a);          \
		typeof(b) _min2 = (b);          \
		(void)(&_min1 == &_min2);       \
		_min1 < _min2 ? _min1 : _min2; })
#define MAX(a, b) ({                    \
		typeof(a) _max1 = (a);          \
		typeof(b) _max2 = (b);          \
		(void)(&_max1 == &_max2);       \
		_max1 > _max2 ? _max1 : _max2; })

#define ARRAY_SIZE(x)                   (sizeof(x) / sizeof((x)[0]))

#ifdef UART_IS_OK
#define vs_getc()                       console_getc()
#define vs_getc_timeout(x)              console_getc_timeout(x)
#define vs_putc(x)                      console_putc(x)
#define vs_printf(fmt, arg ...)         console_printf(fmt, ## arg)
#define panic(fmt, arg ...)              do { console_printf(fmt, ## arg); \
					      while (1); } while (0)
#else
#define vs_getc()                       '0'
#define vs_getc_timeout(x)              '0'
#define vs_putc(x)                      do {} while (0)
#define vs_printf(fmt, arg ...)         do {} while (0)
#define panic(fmt, arg ...)              do { while (1); } while (0)
#endif

#ifdef TIMER_IS_OK
#define udelay(x)                       do { snps_udelay(x) } while (0)
#define mdelay(x)                       do { snps_mdelay(x) } while (0)
#define delay(x)                        do { snps_sdelay(x) } while (0)
#else
#define udelay(x)                       systick_udelay(x)
#define mdelay(x)                       systick_mdelay(x)
#define delay(x)                        systick_sdelay(x)
#endif

#define Ghz                             1000000000
#define Mhz                             1000000
#define Khz                             1000
#define hz                              1

#define HARDWARE_REGISTER_WAIT(reg, required_value)    while (read_mreg32(reg) != required_value) { /* Wait. */ }

void vs_verify_loop(void);

u32 vs_random(void);
void vs_srandom(u32 seed);

void switch_thread_unprivileged_2_privileged(void);
void switch_thread_privileged_2_unprivileged(void);

void fault_returnaddr_set();

/* Get CM33 current security state: 0, Secure; 1, Non-Secure */
u32 m33_currns_get(void);

#endif /* __COMMON_H__ */
