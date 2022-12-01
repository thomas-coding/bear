
#include "t-qspi-bm-flash.h"
#include "common.h"

void qspi_test_init()
{

	//config xip: disable xip and disable cache
	u32 value = 0;
	value = read_mreg32(0x50065000 + 0x98);
	//vsi_printf("value = %x\n", value);
	value |= 0x04;
	value &= 0xfffffffe;
	//vsi_printf("value = %x\n", value);

	write_mreg32(0x50065000 + 0x98, value); // SYSREG2:6.30.8.2.	QSPI Interface Configure Register 1, QSPI_INTF_CFG1
}

void qspi_enable()
{
	write_mreg32(0x5001E000 + 0x0c, 0x01);
	write_mreg32(0x5001E000 + 0x08, 0x01);//QSPI_EN;
}

void qspi_disable()
{
	write_mreg32(0x5001E000 + 0x0c, 0x00);  //DEVICE_CS
	write_mreg32(0x5001E000 + 0x08, 0x00);  //QSPI_EN;
}

void qspi_spi_read_single_mode_config(unsigned int src_flash_addr, unsigned int len)
{
	///single mode
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000000);
	write_mreg32(0x5001E000 + ( 0x14 ), 0x19);
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08180000);
	//write_mreg32(0x5001E000 + ( 0x04 ), 0x8);
	write_mreg32(0x5001E000 + ( 0x04 ), 512);
	write_mreg32(0x5001E000 + ( 0x1c ), len);
	write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG
	write_mreg32(0x5001E000 + ( 0x20 ), 0x03);
}

void qspi_spi_receive(unsigned char *pRxData, unsigned int len)
{
	unsigned char *data1 = pRxData;
	unsigned int i = 0;

	//status: TX_FIFO_NUM + RX_FIFO_NUM
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001)); //QSPI_STATUS, until spi idle or disable.

	while (len > 0) {
		while ((read_mreg32(0x5001E000 + 0x05C) & 0xFF) == 0x00); //FIFO_NUM, until have data in RX FIFO.
		data1[i] = read_mreg32(0x5001E000 + RX_DAT);
		vs_printf("data1 = 0x%x\n\n", data1[i]);
		len--;
		i++;
	}
}

#define QSPI_READ_TEST_LEN     4
void qspi_read_flash_test()
{
	unsigned char data[QSPI_READ_TEST_LEN] = { 0 };

	//qspi_spi_read_single_mode_config(0x32000, 4);
	qspi_spi_read_single_mode_config(0x0, 4);

	qspi_enable();

	qspi_spi_receive(data, QSPI_READ_TEST_LEN);

	qspi_disable();
}

void qspi_spi_read_flash_id()
{
	u32 status_l = 0;
	u32 rx_fifo_num = 0;
	u32 len = 5;
	vs_printf("read flash id\n");
	write_mreg32(0x5001E000 + ( 0x04 ), 512);
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000000);        //QSPI_CFG0:
	write_mreg32(0x5001E000 + ( 0x14 ), 0x11);              //QSPI_CFG1
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08000000);
	//write_mreg32(0x5001E000 + ( 0x18 ), 0x0);
	write_mreg32(0x5001E000 + ( 0x1c ), 5);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_READID); //INST_REG
	//write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG
	qspi_enable();

	//rx_fifo_num = (read_mreg32(0x5001E000+0x05C)&0xFF);
	while (len--) {
		while ((rx_fifo_num = (read_mreg32(0x5001E000 + 0x05C) & 0xFF)) == 0x00); //FIFO_NUM, until have data in RX FIFO.
		//while ((read_mreg32(0x5001E000+0x06C)&0x00000001));//QSPI_STATUS, until spi idle or disable.

		//data1[i] = read_mreg32(0x5001E000+0x0C0);
		status_l = read_mreg32(0x5001E000 + RX_DAT);
		vs_printf("status_l = 0x%x, rx_fifo_num = %d\n\n", status_l, rx_fifo_num);
	}

	qspi_disable();
}

void Flash_Checkbusy()
{
	u32 status_l;
	u32 rx_fifo_num;
	while (1) {
		status_l = 0;
		rx_fifo_num = 0;

		write_mreg32(0x5001E000 + ( 0x10 ), 0x1f000000);        //QSPI_CFG0:
		write_mreg32(0x5001E000 + ( 0x14 ), 0x11);              //QSPI_CFG1
		write_mreg32(0x5001E000 + ( 0x1c ), 0x01);
		write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_RSRL);    //INST_REG
		//write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG
		qspi_enable();

		//rx_fifo_num = (read_mreg32(0x5001E000+0x05C)&0xFF);
		while ((rx_fifo_num = (read_mreg32(0x5001E000 + 0x05C) & 0xFF)) == 0x00); //FIFO_NUM, until have data in RX FIFO.
		while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));

		//data1[i] = read_mreg32(0x5001E000+0x0C0);

		while (rx_fifo_num--) {
			status_l = read_mreg32(0x5001E000 + RX_DAT);
			//console_printf("status_l = 0x%x, rx_fifo_num = %d\n\n", status_l, rx_fifo_num);

			if ((status_l & 0x00000001) == 0) {
				qspi_disable();
				//console_printf("Checkbusy PASS!\n");
				return;
			}
		}

		qspi_disable();
	}
}

void Flash_Enable_Write()
{
	write_mreg32(0x5001E000 + ( 0x10 ), 0x1f000200);        //QSPI_CFG0:
	write_mreg32(0x5001E000 + ( 0x14 ), 0x10);              //QSPI_CFG1
	write_mreg32(0x5001E000 + ( 0x1c ), 0x01);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_WREN);    //INST_REG
	//write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG
	qspi_enable();
	udelay(10);
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));
	qspi_disable();
}

void Flash_Disable_Write()
{
	write_mreg32(0x5001E000 + ( 0x10 ), 0x1f000200);        //QSPI_CFG0:
	write_mreg32(0x5001E000 + ( 0x14 ), 0x10);              //QSPI_CFG1
	write_mreg32(0x5001E000 + ( 0x1c ), 0x01);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_WRDI);    //INST_REG
	//write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG
	qspi_enable();
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));
	qspi_disable();

}

void Flash_Disable_Protect()
{
	//write_mreg32(0x5001E000 + ( 0x10 ), 0x1f000200); //QSPI_CFG0:
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000200);        //QSPI_CFG0:

	write_mreg32(0x5001E000 + ( 0x14 ), 0x11);              //QSPI_CFG1
	write_mreg32(0x5001E000 + ( 0x1c ), 0x01);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_WRSR);    //INST_REG
	//write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG

	qspi_enable();
	write_mreg32(0x5001E000 + ( TX_DAT ), 0x00);

	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));
	qspi_disable();

}


void qspi_spi_init()
{
	write_mreg32(0x5001E000 + ( 0x04 ), 0x20);              //BAUD_DIV
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08180000);        //CMD_NUM
	//vs_printf("cmd num = 0x%x\n", read_mreg32(0x5001E000 + ( 0x18 )));
	vs_printf("BADU_DIV = 0x%x\n", read_mreg32(0x5001E000 + ( 0x04 )));

	//write_mreg32(0x5001E000 + ( 0x50 ), 0x01);///TX_FIFO_TRIG_LVL
	//write_mreg32(0x5001E000 + ( 0x54 ), 0x01);///RX_FIFO_TRIG_LVL

	//write_mreg32(0x5001E000+0x0c, 0x00);//DEVICE_CS
	//write_mreg32(0x5001E000+0x08, 0x00);//QSPI_EN;
}

void flash_write_status_register()
{
	Flash_Enable_Write();
	//write_mreg32(0x5001E000 + ( 0x10 ), 0x1f000200); //QSPI_CFG0:
	write_mreg32(0x5001E000 + ( 0x04 ), 0x20);
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000200);        //QSPI_CFG0: 8 bit, tx
	write_mreg32(0x5001E000 + ( 0x14 ), 0x11);              //QSPI_CFG1
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08000000);        //instruction len 8 bit
	write_mreg32(0x5001E000 + ( 0x1c ), 0x01);              //data number 1
	write_mreg32(0x5001E000 + ( 0x20 ), 0x31/*FLASH_CMD_WRSR*/);    //INST_REG
	//write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG

	qspi_enable();
	//udelay(10);
	write_mreg32(0x5001E000 + ( TX_DAT ), 0x2);

	udelay(10);
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001)); //qspi idle
	qspi_disable();

}


void qspi_spi_read_flash_status0(void)
{
	u32 status_l = 0;
	u32 rx_fifo_num = 0;
	mdelay(10);
	//flash_write_status_register();
	write_mreg32(0x5001E000 + ( 0x04 ), 512);               //512
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000000);        //QSPI_CFG0:
	write_mreg32(0x5001E000 + ( 0x14 ), 0x11);              //QSPI_CFG1
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08000000);
	//write_mreg32(0x5001E000 + ( 0x18 ), 0x0);
	write_mreg32(0x5001E000 + ( 0x1c ), 1);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_RSRL); //INST_REG
	//write_mreg32(0x5001E000 + ( 0x24 ), src_flash_addr);//ADDR_REG
	qspi_enable();

	mdelay(10);

	//rx_fifo_num = (read_mreg32(0x5001E000+0x05C)&0xFF);
	while ((rx_fifo_num = (read_mreg32(0x5001E000 + 0x05C) & 0xFF)) == 0x00);       //FIFO_NUM, until have data in RX FIFO.
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));                         //QSPI_STATUS, until spi idle or disable.

	status_l = read_mreg32(0x5001E000 + RX_DAT);
	vs_printf("333 status = 0x%x, rx_fifo_num = %d\n\n", status_l, rx_fifo_num);

	qspi_disable();
}


void qspi_spi_erase_sector(unsigned int addr)
{
	//write_mreg32(0x5001E000 + ( 0x18 ), 0x08180000);//CMD_NUM
	//write_mreg32(0x5001E000 + ( 0x04 ), 0x20);//BAUD_DIV
	//Flash_Enable_Write();	//enable flash write
	//Flash_Disable_Protect(); //disable all block protect
	//Flash_Checkbusy();		//enable flash write
	//qspi_spi_read_flash_status0();
	Flash_Enable_Write();
	//qspi_spi_read_flash_status0();
	 #if 1
	write_mreg32(0x5001E000 + ( 0x04 ), 0x20);              //BAUD_DIV
	write_mreg32(0x5001E000 + ( 0x10 ), 0x1f000200);        //QSPI_CFG0:
	write_mreg32(0x5001E000 + ( 0x14 ), 0x18);              //QSPI_CFG1
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08180000);        //CMD_NUM
	write_mreg32(0x5001E000 + ( 0x1c ), 0x04);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_SE);      //INST_REG
	write_mreg32(0x5001E000 + ( 0x24 ), addr);              //ADDR_REG
	qspi_enable();
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));
	qspi_disable();
	 #endif
	Flash_Checkbusy();
}


void qspi_write_flash_test(void)
{
	//qspi_spi_init();
	//Flash_Enable_Write();    //enable flash write
	//Flash_Disable_Protect(); //disable all block protect
	//Flash_Checkbusy();	   //enable flash write
	Flash_Enable_Write();

	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000200);        //QSPI_CFG0:tx, data shift 8bit
	write_mreg32(0x5001E000 + ( 0x14 ), 0x19);              //QSPI_CFG1: Instruction+address+data
	write_mreg32(0x5001E000 + ( 0x1c ), 4);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_PP);      //INST_REG
	write_mreg32(0x5001E000 + ( 0x24 ), 0x1);               //ADDR_REG


	write_mreg32(0x5001E000 + 0x08, 0x01);  //QSPI_EN for write data to fifo
	write_mreg32(0x5001E000 + 0x0c, 0x01);  //cs en for start transaction
	//mdelay(1000);
	//qspi_enable();
	write_mreg32(0x5001E000 + TX_DAT, 0x11);
	write_mreg32(0x5001E000 + TX_DAT, 0x22);
	write_mreg32(0x5001E000 + TX_DAT, 0x33);
	write_mreg32(0x5001E000 + TX_DAT, 0x44);

	vs_printf("tx fifo number:0x%x\n", read_mreg32(0x5001E000 + ( 0x5c )) >> 16);

	//mdelay(100);
	//qspi_enable();
	//write_mreg32(0x5001E000 +0x0c, 0x01);//cs en for start transsaction

	udelay(10);

	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));

	qspi_disable();

	Flash_Checkbusy();

}

void quad_qspi_write_flash_test(void)
{

	Flash_Enable_Write();

	write_mreg32(0x5001E000 + ( 0x04 ), 8);                         //BAUD_DIV
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000200 | (0x2 << 16));  //QSPI_CFG0:tx, data shift 8bit
	write_mreg32(0x5001E000 + ( 0x14 ), 0x19);                      //QSPI_CFG1: Instruction+address+data
	write_mreg32(0x5001E000 + ( 0x1c ), 4);
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_QPP);             //INST_REG
	write_mreg32(0x5001E000 + ( 0x24 ), 0x0);                       //ADDR_REG


	write_mreg32(0x5001E000 + 0x08, 0x01);  //QSPI_EN for write data to fifo
	write_mreg32(0x5001E000 + 0x0c, 0x01);  //cs en for start transaction

	write_mreg32(0x5001E000 + TX_DAT, 0x11);
	write_mreg32(0x5001E000 + TX_DAT, 0x22);
	write_mreg32(0x5001E000 + TX_DAT, 0x33);
	write_mreg32(0x5001E000 + TX_DAT, 0x44);

	vs_printf("tx fifo number:0x%x\n", read_mreg32(0x5001E000 + ( 0x5c )) >> 16);

	udelay(10);

	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001));

	qspi_disable();

	Flash_Checkbusy();

}

uint32_t count = 0;
void qspi_spi_erase_sector2()
{
	vs_printf("666 erase sector 2\n");
	/* First cs is de-assert, qspi is idle */
	write_mreg32(0x5001E000 + 0x0c, 0x00);  //DEVICE_CS
	write_mreg32(0x5001E000 + 0x08, 0x00);  //QSPI_EN;

	/* config enable write command */
	write_mreg32(0x5001E000 + ( 0x04 ), 512);               //BAUD_DIV
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000200);        //QSPI_CFG0: tx, data shift 8bit
	write_mreg32(0x5001E000 + ( 0x14 ), 0x10);              //QSPI_CFG1: frame: instruction
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08000000);        //CMD_NUM:instruction 8 bit,address 0 bit
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_WREN);    //INST_REG

	/* enable cs and start qspi for send command to flash*/
	write_mreg32(0x5001E000 + 0x0c, 0x01);  //DEVICE_CS
	write_mreg32(0x5001E000 + 0x08, 0x01);  //QSPI_EN;

	udelay(10);
	/* wait comand send ok */
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001)); //QSPI_STATUS, until spi idle or disable.

	/* disable qspi transfer */
	write_mreg32(0x5001E000 + 0x0c, 0x00);  //DEVICE_CS
	write_mreg32(0x5001E000 + 0x08, 0x0);   //QSPI_EN;


	//mdelay(100);


	write_mreg32(0x5001E000 + ( 0x04 ), 512);               //BAUD_DIV
	write_mreg32(0x5001E000 + ( 0x10 ), 0x07000200);        //QSPI_CFG0: tx, data shift 8bit
	write_mreg32(0x5001E000 + ( 0x14 ), 0x18);              //QSPI_CFG1: Instruction+address
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08180000);        //CMD_NUM: inst 8bit,addr 24bit
	write_mreg32(0x5001E000 + ( 0x20 ), FLASH_CMD_SE);      //INST_REG
	write_mreg32(0x5001E000 + ( 0x24 ), 0x0);               //ADDR_REG

	write_mreg32(0x5001E000 + 0x0c, 0x01);
	write_mreg32(0x5001E000 + 0x08, 0x01);//QSPI_EN;

	udelay(10);
	while ((read_mreg32(0x5001E000 + 0x06C) & 0x00000001)) {
	}
	;
	write_mreg32(0x5001E000 + 0x0c, 0x00);  //DEVICE_CS
	write_mreg32(0x5001E000 + 0x08, 0x00);  //QSPI_EN;

	Flash_Checkbusy();
	//mdelay(100);

}


void xip_enter(){
	unsigned int value = 0; 

	write_mreg32(0x5001E000+0x10, 0x1f020000);//QSPI_CFG0: quad spi, 32bit data shift
	write_mreg32(0x5001E000+0x14, 0x11f); //QSPI_CFG1:  ins+address+continue+dummy+data
	write_mreg32(0x5001E000+0x18, 0x08180804); //CMD_NUM: ins 8, add 24, continue 8, dummy 4
	write_mreg32(0x5001E000+0x04, 0x8); //BAUD_DIV
	write_mreg32(0x5001E000+0x1c, 0x01); //DATA_NUM: 1, 32 bits
	write_mreg32(0x5001E000+0x20, 0xEB); //INST_REG
	write_mreg32(0x5001E000+0x24, 0x00000000);//ADDR_REG
	write_mreg32(0x5001E000+0x28, 0x20); ////CTNU_REG
		
	write_mreg32(0x5001E000+0x0c, 0x01);//DEVICE_CS	
	write_mreg32(0x5001E000+0x08, 0x01);//QSPI_EN;


	while ((read_mreg32(0x5001E000+0x05C)&0xFF) == 0x00);//FIFO_NUM, until have data in RX FIFO.
	while ((read_mreg32(0x5001E000+0x06C)&0x00000001));//QSPI_STATUS, until spi idle or disable.
	value = read_mreg32(0x5001E000+RX_DAT);//
	vs_printf("data1 = 0x%x\n\n", value);

	mdelay(1);

	write_mreg32(0x5001E000+0x0c, 0x00);//DEVICE_CS
	write_mreg32(0x5001E000+0x08, 0x00);//QSPI_EN;

	mdelay(1);

	/* change to continue mode */
	write_mreg32(0x5001E000+0x14, 0x10f); //QSPI_CFG1:  address+continue+dummy+data


	value = read_mreg32(0x50065000+0x98); 
	value |= 0x01;
	vs_printf("value = %x\n", value);
	write_mreg32(0x50065000+0x98, value); 

	asm volatile("dsb");

	vs_printf("flash value 0x10000000: 0x%x\n", *(unsigned int *)(0x10000000));

}

void xip_exit()
{

	unsigned int value = 0; 
	value = read_mreg32(0x50065000+0x98); 
	value &= (~0x01U);
	vs_printf("value = %x\n", value);
	write_mreg32(0x50065000+0x98, value); 

    /* Reset tran_fmt to any non XIP mode, otherwise hard fault will be triggered when accessing QSPI_EN */
	write_mreg32(0x5001E000+0x14, 0x10); 

    /* Reset to 8 bits mode */
	write_mreg32(0x5001E000+0x10, 0x08000000);

    /* Reset QSPI to normal status */
	write_mreg32(0x5001E000+0x0c, 0x01);//DEVICE_CS	
	write_mreg32(0x5001E000+0x08, 0x01);//QSPI_EN;
	udelay(10);
	write_mreg32(0x5001E000+0x0c, 0x00);//DEVICE_CS
	write_mreg32(0x5001E000+0x08, 0x00);//QSPI_EN;
}

void qspi_bm_flash_test(void)
{

	qspi_test_init();
	qspi_spi_read_flash_id();
	Flash_Enable_Write();                                   //enable flash write
	Flash_Disable_Protect();                                //disable all block protect
	write_mreg32(0x5001E000 + ( 0x04 ), 0x20);              //BAUD_DIV
	write_mreg32(0x5001E000 + ( 0x18 ), 0x08180000);        //CMD_NUM
	qspi_spi_erase_sector2();
	qspi_read_flash_test();
	quad_qspi_write_flash_test();
	qspi_read_flash_test();
	xip_enter();
	xip_exit();
}
