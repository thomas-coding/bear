
#include "common.h"
#include "qspi-flash.h"


void qspi_enable_with_cs_assert()
{
	write_mreg32(QSPI_REG_BASE + DEVICE_CS, 0x01);
	write_mreg32(QSPI_REG_BASE + QSPI_EN, 0x01);
}

void qspi_disable_with_cs_deassert()
{
	/* sometimes set cs first will fail, so set en first */
	//write_mreg32(QSPI_REG_BASE + DEVICE_CS, 0x00);
	write_mreg32(QSPI_REG_BASE + QSPI_EN, 0x00);
	write_mreg32(QSPI_REG_BASE + DEVICE_CS, 0x00);
}

void qspi_check_busy(void) {
	/* wait qspi idle, wait 10us for one clock cycle status set */
	udelay(10);
	while ((read_mreg32(QSPI_REG_BASE + QSPI_STATUS) & 0x00000001));

}

void qspi_set_config(struct qspi_config *config) {
	write_mreg32(QSPI_REG_BASE + BAUD_DIV, config->divide);
	write_mreg32(QSPI_REG_BASE + SPI_CFG0, config->config0);
	write_mreg32(QSPI_REG_BASE + SPI_CFG1, config->config1);
	write_mreg32(QSPI_REG_BASE + SPI_CFG2, config->config2);
	write_mreg32(QSPI_REG_BASE + DATA_NUM, config->data_number);
	write_mreg32(QSPI_REG_BASE + INST_REG, config->instruction);
	write_mreg32(QSPI_REG_BASE + ADDR_REG, config->address);
	write_mreg32(QSPI_REG_BASE + CTNU_REG, config->ctun);

}

void flash_wait_complete(void);
uint32_t internel_flash_read(uint8_t *buffer, uint32_t len) {
	uint32_t count = 0;

	qspi_enable_with_cs_assert();

	//vs_printf("internel_flash_read\n");
	while (len--) {
		/* Check FIFO_NUM, until have data in RX FIFO */
		while ((read_mreg32(QSPI_REG_BASE + FIFO_NUM) & 0xFF) == 0x00);
		buffer[count] = read_mreg32(QSPI_REG_BASE + RX_DAT);
		count++;
	}

	qspi_disable_with_cs_deassert();
	flash_wait_complete();
	return count;
}

struct qspi_config flash_read_id_config = {
	.divide = QSPI_DFAULT_DIV,
	.config0 = DATA_SHIFT_WIDTH_SET(8) | \
				STD_SEL_STANDARD_SPI | TRAN_DIR_NORFLASH_READ,
	.config1 = CMD_FORMAT_0 | TRAN_FRM_INST_DATA,
	.config2 = TX_INST_NUM_SET(8),
	.data_number = 5,
	.instruction = FLASH_CMD_READID,
};

void qspi_flash_read_id(struct qspi_flash_device *dev) {
	//vs_printf("qspi flash read id\n");
	uint8_t id[5];

	qspi_set_config(&flash_read_id_config);
	internel_flash_read(id, 5);
	for(int i = 0; i < 5; i++)
		vs_printf("flash id[%d]: 0x%x\n", i, id[i]);
}

struct qspi_config flash_read_config = {
	.divide = 0x20,//QSPI_DFAULT_DIV,
	.config0 = DATA_SHIFT_WIDTH_SET(8) | \
				STD_SEL_STANDARD_SPI | TRAN_DIR_NORFLASH_READ,
	.config1 = CMD_FORMAT_0 | TRAN_FRM_INST_ADDR_DATA,
	.config2 = TX_INST_NUM_SET(8) | TX_ADDR_NUM_SET(24),
	.instruction = FLASH_CMD_READ,
};

void flash_read(struct qspi_flash_device *dev, u32 src_addr,
				u8 *dest_buff, u32 read_len)
{
	uint32_t flash_offset, once_read_len, dest_offset;

	flash_offset = src_addr - dev->flash_base;
	dest_offset = 0;
	while(read_len) {
		if(read_len > TRANSATION_MAX_BYTES)
			once_read_len = TRANSATION_MAX_BYTES;
		else
			once_read_len = read_len;
	
		/* read 256 bytes */
		flash_read_config.address = flash_offset;
		flash_read_config.data_number = once_read_len;
		qspi_set_config(&flash_read_config);
		internel_flash_read(dest_buff + dest_offset, once_read_len);

		/* update position */
		read_len -= once_read_len;
		flash_offset += once_read_len;
		dest_offset += once_read_len;
	}

}

struct qspi_config flash_write_enable_config = {
	.divide = QSPI_DFAULT_DIV,
	.config0 = DATA_SHIFT_WIDTH_SET(8) | \
				STD_SEL_STANDARD_SPI | TRAN_DIR_TX,
	.config1 = CMD_FORMAT_0 | TRAN_FRM_INST,
	.config2 = TX_INST_NUM_SET(8),
	.instruction = FLASH_CMD_WREN,
};

void flash_write_enable(struct qspi_flash_device *dev) {
	qspi_set_config(&flash_write_enable_config);
	//vs_printf("flash_write_enable\n");
	qspi_enable_with_cs_assert();
	qspi_check_busy();
	qspi_disable_with_cs_deassert();

}

struct qspi_config flash_write_disable_config = {
	.divide = QSPI_DFAULT_DIV,
	.config0 = DATA_SHIFT_WIDTH_SET(8) | \
				STD_SEL_STANDARD_SPI | TRAN_DIR_TX,
	.config1 = CMD_FORMAT_0 | TRAN_FRM_INST,
	.config2 = TX_INST_NUM_SET(8),
	.instruction = FLASH_CMD_WRDI,
};

void flash_write_disable(struct qspi_flash_device *dev) {
	qspi_set_config(&flash_write_disable_config);

	qspi_enable_with_cs_assert();
	qspi_check_busy();
	qspi_disable_with_cs_deassert();
}

struct qspi_config flash_read_status_register_config = {
	.divide = QSPI_DFAULT_DIV,
	.config0 = DATA_SHIFT_WIDTH_SET(8) | \
				STD_SEL_STANDARD_SPI | TRAN_DIR_NORFLASH_READ,
	.config1 = CMD_FORMAT_0 | TRAN_FRM_INST_DATA,
	.config2 = TX_INST_NUM_SET(8),
	.data_number = 1,
};

/* index = 1 --> status register 1*/
uint32_t flash_read_status_register(int index) {
	uint32_t status;

	if(index == 1)
		flash_read_status_register_config.instruction = FLASH_CMD_RSRL;
	else if(index == 2)
		flash_read_status_register_config.instruction = FLASH_CMD_RSRH;
	else
		return 0;

	qspi_set_config(&flash_read_status_register_config);

	qspi_enable_with_cs_assert();


	/* Check FIFO_NUM, until have data in RX FIFO */
	while ((read_mreg32(QSPI_REG_BASE + FIFO_NUM) & 0xFF) == 0x00);
	status = read_mreg32(QSPI_REG_BASE + RX_DAT) & 0x1U;

	qspi_disable_with_cs_deassert();

	return status;

}

void flash_wait_complete(void) {
	uint32_t status;
    do
    {
        status = flash_read_status_register(1);
    } while (status & 0x00000001);
}

struct qspi_config flash_erase_4k_config = {
	.divide = QSPI_DFAULT_DIV,
	.config0 = DATA_SHIFT_WIDTH_SET(8) | \
				STD_SEL_STANDARD_SPI | TRAN_DIR_TX,
	.config1 = CMD_FORMAT_0 | TRAN_FRM_INST_ADDR,
	.config2 = TX_INST_NUM_SET(8) | TX_ADDR_NUM_SET(24),
	.instruction = FLASH_CMD_SE,
};

/* Notice: now only use 4k sector erase */
u32 flash_erase(struct qspi_flash_device *dev, u32 addr, u32 len) {
	u32 n_sector = 0;
	u32 erase_len = 0;
	uint32_t flash_offset;

	if (len % (4 * 1024))
		n_sector = len / 4096 + 1;
	else
		n_sector = len / 4096;

	while(n_sector) {
		flash_offset = (addr - dev->flash_base) & 0xFFFFF000;

		/* Erase one sector(4k) */
		flash_erase_4k_config.address = flash_offset;
		flash_write_enable(dev);
		qspi_set_config(&flash_erase_4k_config);
	
		qspi_enable_with_cs_assert();
		qspi_check_busy();
		qspi_disable_with_cs_deassert();

		udelay(10);

		flash_wait_complete();

		flash_write_disable(dev);

		n_sector--;
	}
}

uint32_t internel_flash_write(uint8_t *buffer, uint32_t len) {
	uint32_t count = 0;
	

	qspi_enable_with_cs_assert();

    /* Write data to QSPI. Polling method, each time write QSPI_USOD_FIFO_DEPTH */
    for (uint32_t i = 0; i < len; i++)
    {
        while (((read_mreg32(QSPI_REG_BASE + FIFO_NUM) >> 16) & 0x1FF) == QSPI_USOD_FIFO_DEPTH)
        {
            ;/* FIFO_NUM, wait for TX FIFO availeble. */
        }

        write_mreg32(QSPI_REG_BASE + TX_DAT, buffer[i]);
    }

	/* wait qspi transation complete */
	udelay(10);
	qspi_disable_with_cs_deassert();

	/* wait flash internel write complete */
	udelay(10);
	flash_wait_complete();

	return count;
}

struct qspi_config flash_write_config = {
	.divide = QSPI_DFAULT_DIV,
	.config0 = DATA_SHIFT_WIDTH_SET(8) | \
				STD_SEL_STANDARD_SPI | TRAN_DIR_TX,
	.config1 = CMD_FORMAT_0 | TRAN_FRM_INST_ADDR_DATA,
	.config2 = TX_INST_NUM_SET(8) | TX_ADDR_NUM_SET(24),
	.instruction = FLASH_CMD_PP,
};

void flash_write(struct qspi_flash_device *dev, u8 *src_buff,
				u32 dest_addr, u32 write_len)
{
	uint32_t n_write, flash_offset, once_write_len, src_offset;

	flash_offset = dest_addr - dev->flash_base;
	src_offset = 0;
	while(write_len) {

		if(write_len > TRANSATION_MAX_BYTES)
			once_write_len = TRANSATION_MAX_BYTES;
		else
			once_write_len = write_len;

		/* write bytes */
		flash_write_config.address = flash_offset;
		flash_write_enable(dev);
		qspi_set_config(&flash_write_config);
		internel_flash_write(src_buff + src_offset, once_write_len);
		flash_write_disable(dev);

		/* update position */
		write_len -= once_write_len;
		flash_offset += once_write_len;
		src_offset += once_write_len;

	}	

}

struct qspi_flash_operation qspi_flash_ops = {
	.qspi_flash_erase = (u32 (*)(void *, u32, u32))flash_erase,
	.qspi_flash_read = (void (*)(void *, u32, u8 *, u32))flash_read,
	.qspi_flash_write = (void (*)(void *, u8 *, u32, u32))flash_write,
	.qspi_flash_read_id = (void (*)(void *))qspi_flash_read_id,
	//.qspi_flash_check_busy = (void (*)(void *))flash_checkbusy,
	.qspi_flash_write_en = (void (*)(void *))flash_write_enable,
	.qspi_flash_write_dis = (void (*)(void *))flash_write_disable,
};
