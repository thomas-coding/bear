
#ifndef __VENEERS_H__
#define __VENEERS_H__

#include "data-type.h"

__attribute__((cmse_nonsecure_entry)) void nsc_function();

#endif /* #ifndef __VENEERS_H__ */
