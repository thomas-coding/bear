
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/*
 * Use the following parameter passing structure to make console_printf
 * re-entrant.
 */
struct params_s {
	u32 len;
	u64 num_integer;
	u64 num_decimal;
	char pad_character;
	u8 do_padding;
	u8 left_flag;
	u8 hex_upper;		/* Hexadecimal data output to upper case */
};

void console_printf(const char *ctrl1, ...);

#endif /* __CONSOLE_H__ */
