
#include "common.h"
#include "veneers.h"

#include "arm_cmse.h"
__attribute__((cmse_nonsecure_entry)) void nsc_function(){
    vs_printf("call from non-secure, this is secure world!\n");
}

