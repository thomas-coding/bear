
#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#include "stdbool.h"
#include "stddef.h"

typedef signed char			s8;
typedef unsigned char			u8;
typedef signed short int		s16;
typedef unsigned short int		u16;
typedef signed int			s32;
typedef unsigned int			u32;
typedef signed long long		s64;
typedef unsigned long long		u64;

typedef u32				phys_addr_t;

typedef void (*pfunc)(void);

#endif /* __DATA_TYPE_H__ */
