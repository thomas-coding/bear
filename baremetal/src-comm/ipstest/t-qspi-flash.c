
#include "t-qspi-flash.h"
#include "qspi-flash.h"
#include "common.h"

void qspi_erase_test(void)
{
	struct qspi_flash_device device;

	device.flash_base = 0x10000000;
	device.ops = &qspi_flash_ops;

	//device.ops->qspi_flash_write_en(&device);
	device.ops->qspi_flash_erase(&device, 0x10000000, 100);
	//device.ops->qspi_flash_write_dis(&device);
}

void qspi_flash_read_id_test(void)
{
	struct qspi_flash_device device;

	device.flash_base = 0x10000000;
	device.ops = &qspi_flash_ops;
	device.ops->qspi_flash_read_id(&device);
}

void qspi_flash_read_test(void)
{
	struct qspi_flash_device device;
	uint8_t buffer[16];

	device.flash_base = 0x10000000;
	device.ops = &qspi_flash_ops;
	device.ops->qspi_flash_read(&device, 0x10000000, buffer, 16U);

	for (int i = 0; i < 16; i++)
		vs_printf("read buffer[%d]:0x%x\n", i, buffer[i]);
}

void memory_hex_dump(char* start, uint8_t *buffer, uint32_t len)
{
	vs_printf("\n%s:\n", start);
	for (int i = 0; i < len; i++) {
		if (buffer[i] < 0x10)
			vs_printf("0%x ", buffer[i]);
		else
			vs_printf("%2x ", buffer[i]);
		if ((i + 1) % 16 == 0)
			vs_printf("\n");
	}
	vs_printf("\n");
}

void qspi_flash_write_test(void)
{
	struct qspi_flash_device device;
	uint8_t read_buffer[512];

	device.flash_base = 0x10000000;
	device.ops = &qspi_flash_ops;

	uint8_t write_buffer[500];
	for (int i = 0; i < 500; i++)
		write_buffer[i] = i % 256;

	/* read data */
	device.ops->qspi_flash_read(&device, 0x10000000, read_buffer, 512);
	memory_hex_dump("origin", read_buffer, 512);


	/* erase flash sector (4k) */
	device.ops->qspi_flash_erase(&device, 0x10000000, 512);


	/* read data, it should be 0xff after erase */
	device.ops->qspi_flash_read(&device, 0x10000000, read_buffer, 512U);
	memory_hex_dump("after erase", read_buffer, 512U);


	/* write data */
	device.ops->qspi_flash_write(&device, write_buffer, 0x10000000, 500);


	/* read data, compare to write buffer */
	device.ops->qspi_flash_read(&device, 0x10000000, read_buffer, 512U);
	memory_hex_dump("after write", read_buffer, 512U);
}

void qspi_xip_enter_test(void)
{
	struct qspi_flash_device device;

	device.flash_base = 0x10000000;
	device.ops = &qspi_flash_ops;

	/* enter xip mode */
	device.ops->qspi_flash_xip_enter(&device);

	/* read flash data */
	vs_printf("flash value 0x10000000: 0x%x\n", *(unsigned int *)(0x10000000));

	/* exit xip mode */
	device.ops->qspi_flash_xip_exit(&device);


}

void qspi_flash_test(void)
{

	qspi_flash_read_id_test();
	qspi_flash_read_test();
	//qspi_erase_test();
	//qspi_flash_read_test();
	qspi_flash_write_test();
	//qspi_xip_enter_test();

	//qspi_flash_read_test();

}
