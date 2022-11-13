
#ifndef __BOARD_MPU_H__
#define __BOARD_MPU_H__

#include "platform.h"

//FIXME
#define CONFIG_SRAM_S_BASE 0x00200000
#define  CONFIG_SRAM_S_SIZE 0x00040000

#define  CONFIG_SRAM_NS_BASE 0x00240000
#define  CONFIG_SRAM_NS_SIZE 0x000be000

// MPU Test Region
#define  CONFIG_MPU_TEST_BASE 0x002ff000
#define  CONFIG_MPU_TEST_SIZE 0x00001000

/* Region for Secure (Executable, Read-Write) */
#define MPU_LAS_SRAM_S_BASE	LAS_SRAM_S_BASE
#define MPU_LAS_SRAM_S_END	(LAS_SRAM_S_BASE + LAS_SRAM_S_SIZE - 1)
#define MPU_LAS_SRAM_NS_BASE	LAS_SRAM_NS_BASE
#define MPU_LAS_SRAM_NS_END	(LAS_SRAM_NS_BASE + LAS_SRAM_NS_SIZE - 1)

/* Region for Device (Not-Executable, Read-Write) */
#define MPU_PERIPH_DEV_BASE	PERIPH_DEV_BASE
#define MPU_PERIPH_DEV_END	(PERIPH_DEV_BASE + PERIPH_DEV_SIZE - 1)

/* Region for Secure (Executable, Read-Write) */
#define MPU_LPS_SRAM01_BASE	LPS_SRAM01_BASE
#define MPU_LPS_SRAM01_END	(LPS_SRAM01_BASE + LPS_SRAM01_SIZE - 1)

/* Region for Secure (Executable, Read-Write) */
#define MPU_LPS_SRAM23_BASE	LPS_SRAM23_BASE
#define MPU_LPS_SRAM23_END	(LPS_SRAM23_BASE + LPS_SRAM23_SIZE - 1)

/* Region for testing (Default Executable, Read-Write) */
#define MPU_LAS_SRAM_TEST_BASE	LAS_SRAM_TEST_BASE
#define MPU_LAS_SRAM_TEST_END	(LAS_SRAM_TEST_BASE + LAS_SRAM_TEST_SIZE - 1)

void secure_MPU_init(void);

#endif /* __BOARD_MPU_H__ */
