
#include "platform.h"
#include "mpu_armv8.h"
#include "board-mpu.h"

void secure_MPU_init(void)
{
	/* Configure the MAIR index 0 for Device and 1 for Cacheable */
	ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR_DEVICE_nGnRnE);

	ARM_MPU_SetMemAttr(1UL, ARM_MPU_ATTR(
		ARM_MPU_ATTR_MEMORY_(1UL, 1UL, 1UL, 1UL),
		ARM_MPU_ATTR_MEMORY_(1UL, 1UL, 1UL, 1UL)));

	/* Region for Secure (Executable, Read-Write) */
	ARM_MPU_SetRegion(1UL,
		ARM_MPU_RBAR(MPU_LAS_SRAM_S_BASE, ARM_MPU_SH_NON,
			0UL, 1UL, 0UL),
		ARM_MPU_RLAR(MPU_LAS_SRAM_S_END, 1UL));

	/* Region for Secure (Executable, Read-Write) */
	ARM_MPU_SetRegion(2UL,
		ARM_MPU_RBAR(MPU_LAS_SRAM_NS_BASE, ARM_MPU_SH_NON,
			0UL, 1UL, 0UL),
		ARM_MPU_RLAR(MPU_LAS_SRAM_NS_END, 1UL));

	/* Region for Device (Not-Executable, Read-Write) */
	ARM_MPU_SetRegion(3UL,
		ARM_MPU_RBAR(MPU_PERIPH_DEV_BASE, ARM_MPU_SH_NON,
			0UL, 1UL, 1UL),
		ARM_MPU_RLAR(MPU_PERIPH_DEV_END, 0UL));

	/* Region for Secure (Executable, Read-Write) */
	ARM_MPU_SetRegion(4UL,
		ARM_MPU_RBAR(MPU_LPS_SRAM01_BASE, ARM_MPU_SH_NON,
			0UL, 1UL, 0UL),
		ARM_MPU_RLAR(MPU_LPS_SRAM01_END, 1UL));

	/* Region for Secure (Executable, Read-Write) */
	ARM_MPU_SetRegion(5UL,
		ARM_MPU_RBAR(MPU_LPS_SRAM23_BASE, ARM_MPU_SH_NON,
			0UL, 1UL, 0UL),
		ARM_MPU_RLAR(MPU_LPS_SRAM23_END, 1UL));

	/* Region for Secure data (Executable, Read-Write) */
	ARM_MPU_SetRegion(7UL,
		ARM_MPU_RBAR(MPU_LAS_SRAM_TEST_BASE, ARM_MPU_SH_NON,
			0UL, 1UL, 0UL),
		ARM_MPU_RLAR(MPU_LAS_SRAM_TEST_END, 1UL));

	ARM_MPU_Enable(0);
}
