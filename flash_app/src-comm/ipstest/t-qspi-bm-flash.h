
#ifndef __T_QSPI_BM_FLASH_H__
#define __T_QSPI_BM_FLASH_H__

#include "common.h"

#if 0
// ----------------------- spi ------------------------------
// config XIP register
#define XIP_REG_BASE 0x5001E000
#define QSPI_IRQ_NUM      23

#define VERSION_ID                      0x00

#define BAUD_DIV                        0x04

#define QSPI_EN                 0x08

#define DEVICE_CS                       0x0C
#define DEV_CS_DEVICE_NONE                              (0)
#define DEV_CS_DEVICE_0                         (1 << 0)
#define DEV_CS_DEVICE_1                         (1 << 1)
#define DEV_CS_DEVICE_2                         (1 << 2)
#define DEV_CS_DEVICE_3                         (1 << 3)
#define DEV_CS_DEVICE_4                         (1 << 4)
#define DEV_CS_DEVICE_5                         (1 << 5)
#define DEV_CS_DEVICE_6                         (1 << 6)
#define DEV_CS_DEVICE_7                         (1 << 7)

#define SPI_CFG0                        0x10
#define DATA_SHIFT_WIDTH_SET(value)                     ((value - 1) << 24)
#define STD_SEL_STANDARD_SPI                    (0 << 16)
#define STD_SEL_DUAL_SPI                            (1 << 16)
#define STD_SEL_QUAD_SPI                            (2 << 16)
#define STD_SEL_RESERVED                            (3 << 16)

#define TRAN_DIR_NORFLASH_READ                  (0 << 8)
#define TRAN_DIR_RX                                 (1 << 8)
#define TRAN_DIR_TX                                 (2 << 8)
#define TRAN_DIR_FULL_DUPLEX                    (3 << 8)

#define SPI_CFG1                        0x14
#define DATA_DTR_EN                                 (1 << 15)
#define ADDR_DTR_EN                                 (1 << 14)
#define CTNU_DTR_EN                                 (1 << 13)
#define INST_DTR_EN                                 (1 << 12)
//instruction��addresstransfer with standard mode, data transfer with specified mode in STD_SEL.
#define CMD_FORMAT_0                                (0 << 8)
//instruction transfer with standard mode�� and address and data transfer with specified mode in STD_SEL.
#define CMD_FORMAT_1                                (1 << 8)
//instruction��address and data transfer with specified mode in STD_SEL
#define CMD_FORMAT_2                                (2 << 8)
//reserved
#define CMD_FORMAT_RESV                             (3 << 8)

#define TRAN_FRM_DATA                               (0x01)
#define TRAN_FRM_INST                               (0x10)
#define TRAN_FRM_INST_ADDR                              (0x18)
#define TRAN_FRM_INST_DATA                              (0x11)
#define TRAN_FRM_INST_ADDR_DATA                     (0x19)
#define TRAN_FRM_INST_ADDR_DMY_DATA                     (0x1B)
#define TRAN_FRM_ADDR_CTNU_DMY_DATA                     (0x0F)
#define TRAN_FRM_ADDR_DMY_DATA                      (0x0B)
#define TRAN_FRM_INST_ADDR_CTNU_DMY_DATA    (0x1F)


#define SPI_CFG2                        0x18   //renamed CMD_NUM?
#define TX_INST_NUM_SET(value)                  ((value) << 24)
#define TX_ADDR_NUM_SET(value)                  ((value) << 16)
#define TX_CTNU_NUM_SET(value)                  ((value) << 8)
#define WAIT_NUM_SET(value)                     (value)

#define DATA_NUM                        0x1C
#define INST_REG                        0x20
#define ADDR_REG                        0x24
#define CTNU_REG                        0x28

#define DMA_CFG                     0x30
#define DMA_CFG_TX_ENABLE                       (1 << 24)
#define DMA_CFG_TX_DISABLE                      (0 << 24)
#define DMA_CFG_TX_TRIG_LEVEL_SET(value)                        ((value) << 16)

#define DMA_CFG_RX_ENABLE                       (1 << 8)
#define DMA_CFG_RX_DISABLE                      (0 << 8)
#define DMA_CFG_RX_TRIG_LEVEL_SET(value)                        (value)

#define SHIFT_PHASE                     0x38
#define RX_SHIFT_MOVED_PHASE(value)                     ((value) << 16)
#define TX_SHIFT_MOVED_PHASE(value)                     (value)

#define IO_MAPPING_EN           0x40
#define SCLK_MAPPING_ENABLE                     (1 << 24)
#define SCLK_MAPPING_DISABLE                    (0 << 24)
#define IO_MAPPING_ENABLE                       (1 << 16)
#define IO_MAPPING_DISABLE                      (0 << 16)
#define CS_MAPPING_ENABLE                       (1)
#define CS_MAPPING_DISABLE                      (0)

#define IO_MAPPING_CS           0x44
#define IO_MAPPING_SCLK         0x48
#define IO_MAPPING_DIO          0x4C

#define TX_FIFO_TRIG_LVL                0x50
#define RX_FIFO_TRIG_LVL                0x54
#define FIFO_NUM                0x5C

#define INT_STATUS              0x60
#define SEC_INT_FLAG                    (1 << 26)
#define SELECTED_AS_SLAVE_INT_FLAG                      (1 << 25)
#define MUTI_MST_DETECT_INT_FLAG                        (1 << 24)
#define TRAN_COMP_INT_FLAG                      (1 << 16)
#define TX_SHIFT_ERR_INT_FLAG                   (1 << 11)
#define TX_FIFO_OVERFLOW_INT_FLAG                       (1 << 10)
#define TX_FIFO_FULL_INT_FLAG                   (1 << 9)
#define TX_FIFO_ALMOST_EMPTY_INT_FLAG                   (1 << 8)
#define RX_SHIFT_ERR_INT_FLAG                   (1 << 3)
#define RX_FIFO_UNDERFLOW_INT_FLAG                      (1 << 2)
#define RX_FIFO_EMPTY_INT_FLAG                  (1 << 1)
#define RX_FIFO_ALMOST_FULL_INT_FLAG                    (1 << 0)

#define INT_MASK                0x64
#define SELECTED_AS_SLAVE_UNMASK                        (1 << 25)
#define MUTI_MST_DETECT_UNMASK                  (1 << 24)
#define TRAN_COMP_UNMASK                        (1 << 16)
#define TX_SHIFT_ERR_UNMASK                     (1 << 11)
#define TX_FIFO_OVERFLOW_UNMASK                 (1 << 10)
#define TX_FIFO_FULL_UNMASK                     (1 << 9)
#define TX_FIFO_ALMOST_EMPTY_UNMASK                     (1 << 8)
#define RX_SHIFT_ERR_UNMASK                     (1 << 3)
#define RX_FIFO_UNDERFLOW_UNMASK                        (1 << 2)
#define RX_FIFO_EMPTY_UNMASK                    (1 << 1)
#define RX_FIFO_ALMOST_FULL_UNMASK                      (1 << 0)

#define STATUS          0x68
#define SELECTED_AS_SLAVE_STATUS                        (1 << 25)
#define MUTI_MST_DETECT_STATUS                  (1 << 24)
#define TRAN_COMP_STATUS                        (1 << 16)
#define TX_FIFO_FULL_STATUS                     (1 << 9)
#define TX_FIFO_ALMOST_EMPTY_STATUS                     (1 << 8)
#define RX_FIFO_EMPTY_STATUS                    (1 << 1)
#define RX_FIFO_ALMOST_FULL_STATUS                      (1 << 0)

#define QSPI_STATUS             0x6C
#define MULTI_MASTER_CS_BUSY_FLAG                       (1 << 1)
#define SPI_INTERFACE_STATUS_BUSY_FLAG                  (1 << 0)

#define BITS_NUM                0x70

#define AHB_TIMEOUT             0x74
#define FLASH_CFG               0x7C
#define INT_STATUS_SEC          0x80

#define INT_MASK_SEC            0x84

#define TX_DAT          0x800
#define RX_DAT          0xC00
#define SEC_FLAG                0x200
#define SEC_FADDR_LIMITED               0x204

#define SPI_MODE_NONE      (0)
#define SPI_MODE_SINGLE    (1)
#define SPI_MODE_DUAL      (2)
#define SPI_MODE_QUAD      (3)

#define SPI_MODE_READ_VALUE      (0x27091956)

#define QSPI_SPI_TXFIFO_EMPTY               (0x1L << 8)
#define QSPI_SPI_RXFIFO_EMPTY               (0x1L << 1)

#define QSPI_SPI_CS0_ENABLE            (0x01)

#define QSPI_FIFO_DEPTH            (16)
#define QSPI_TRIG_LVL            (8)


// ------------------------ flash ----------------------------
/*enable Quad I/O*/
#define FLASH_CMD_EQIO                  0x38
/*disable Quad I/O*/
#define FLASH_CMD_DSQIO                 0xff
/*read status register(7 : 0)*/
#define FLASH_CMD_RSRL                  0x05
/*read status register(15 : 8)*/
#define FLASH_CMD_RSRH                  0x35
/*1 x I/O*/
#define FLASH_CMD_READ                  0x03
/*fast read data*/
#define FLASH_CMD_FASTREAD              0x0b
/*1In/2 Out fast read*/
#define FLASH_CMD_DREAD                 0x3b
/*2 x I/O*/
#define FLASH_CMD_2READ                 0xbb
/*1In/4 Out fast read*/
#define FLASH_CMD_QREAD                 0x6b
/*4 x I/O*/
#define FLASH_CMD_4READ                 0xeb

/*Write cmd*/
/*write Enable*/
#define FLASH_CMD_WREN                  0x06
/*write Disable*/
#define FLASH_CMD_WRDI                  0x04
/*write status register(7 : 0)*/
#define FLASH_CMD_WRSR                  0x01
/*page program*/
#define FLASH_CMD_PP                    0x02
/*quad page program*/
#define FLASH_CMD_QPP                   0x32
/*dual page program*/
#define FLASH_CMD_DPP                   0xa2

/*erase cmd*/
/*sector Erase*/
#define FLASH_CMD_SE                    0x20
/*block Erase 32kb*/
#define FLASH_CMD_BE32K                 0x52
/*block Erase 64k*/
#define FLASH_CMD_BE                    0xd8
/*chip Erase*/
#define FLASH_CMD_CE                    0xc7

/*reset cmd*/
/*reset Enable*/
#define FLASH_CMD_RSTEN                 0x66
/*Reset Memory*/
#define FLASH_CMD_RST                   0x99

/*read id cmd*/
#define FLASH_CMD_REMS                  0x90
#define FLASH_CMD_2REMS                 0x92
#define FLASH_CMD_4REMS                 0x94
#define FLASH_CMD_READID                0x9f

/*Read Status Registers*/
#define FLASH_CMD_RSRS                  0x65
/*Write Status Registers*/
#define FLASH_CMD_WSRS                  0x71



/*flash status*/
#define FLASH_STS_QE                    (1 << 9)
#define FLASH_STS_SRP1                  (1 << 8)
#define FLASH_STS_SRP0                  (1 << 7)
#define FLASH_STS_BP4                   (1 << 6)
#define FLASH_STS_BP3                   (1 << 5)
#define FLASH_STS_BP2                   (1 << 4)
#define FLASH_STS_BP1                   (1 << 3)
#define FLASH_STS_BP0                   (1 << 2)
#define FLASH_STS_WEL                   (1 << 1)
#define FLASH_STS_WIP                   (1 << 0)

#define FLASH_BLOCK_SIZE                (0x10000)
#define FLASH_SECTOR_SIZE               (0x1000)
#define FLASH_PAGE_SIZE                 (0x100)
#define FLASH_MAX_READ_SIZE             (0x10000)

#define NOR_MAX_ID_LEN                  0x6

enum read_mode {
	STA_MODE = 0,
	DIO_MODE,
	QOF_MODE,
};

enum write_mode {
	PP_MODE = 0,
	QPP_MODE
};

struct op_data {
	u32 tx_len;
	u32 rx_len;
	u32 cmd_len;
	void *dout;
	void *din;
	void *cmd_meg;
};

struct norflash_info {
	char *name;
	u8 id[NOR_MAX_ID_LEN];
	u8 id_len;
	u32 sectorsize;
	u32 n_sector;
	u32 options;
};

struct norflash_dev_com {
	u8 cmd;
	void *init;
	void *dma;
	struct norflash_info *chip;
	struct op_data op;
	enum write_mode wmode;
	enum read_mode rmode;
	void (*spi_init)(struct norflash_dev_com *nor, u8 index);
	int (*spi_transmit_one_meg)(struct norflash_dev_com *nor);
	u8 use_dma;
	u8 use_irq;
	u8 cs;
};
#endif


void qspi_bm_flash_test(void);

#endif /* __T_QSPI_BM_FLASH_H__ */
