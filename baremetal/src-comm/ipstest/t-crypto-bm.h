
#ifndef __T_CRYPTO_BM_H__
#define __T_CRYPTO_BM_H__

#include "common.h"

/* Register */
#define BM_REG_CRYPTO_BASE (0X50070000)

#define BM_REG_FETCH_ADDR  0x00
#define BM_REG_FETCH_LEN   0x08
#define BM_REG_FETCH_TAG   0x0C
#define BM_REG_PUSH_ADDR   0x10
#define BM_REG_PUSH_LEN    0x18
#define BM_REG_INT_EN      0x1C
#define BM_REG_INT_STATRAW 0x28
#define BM_REG_INT_STAT    0x2C
#define BM_REG_INT_STATCLR 0x30
#define BM_REG_CONFIG      0x34
#define BM_REG_CONFIG_SG   3
#define BM_REG_START       0x38
#define BM_REG_START_ALL   0x3
#define BM_REG_STATUS      0x3C

#define BM_REG_STATUS_FETCHER_BUSY_MASK 0x01
#define BM_REG_STATUS_PUSHER_BUSY_MASK 0x02
#define BM_REG_STATUS_PUSHER_WAITING_FIFO_MASK 0x20
#define BM_REG_STATUS_BUSY_MASK (BM_REG_STATUS_FETCHER_BUSY_MASK | \
				 BM_REG_STATUS_PUSHER_BUSY_MASK | \
				 BM_REG_STATUS_PUSHER_WAITING_FIFO_MASK)

/* Struct */
/** A cryptomaster DMA descriptor 16bytes */
struct bm_crypto_desc {
	char *addr;
	struct bm_crypto_desc *next;
	uint32_t sz;
	uint32_t dmatag;
};

/* HASH Config */
#define BM_HASH_HW_PAD (1 << 9)
#define BM_HASH_FINAL (1 << 10)
#define BM_CMDMA_BA413_MODE(x) ((x) | (BM_HASH_HW_PAD | BM_HASH_FINAL))

#define BM_HASH_MODE_SHA_1 (2)

/* DMA Config */
#define BM_DMA_LAST_DESCRIPTOR ((struct bm_crypto_desc *)1)
#define BM_DMA_CONST_ADDR (1 << 28)
#define BM_DMA_REALIGN (1 << 29)
#define BM_DMA_DISCARD (1 << 30)

/* Tag config */
#define BM_DMATAG_BYPASS (0)
#define BM_DMATAG_BA411 (1)
#define BM_DMATAG_BA412 (2)
#define BM_DMATAG_BA413 (3)
#define BM_DMATAG_BA417 (4)
#define BM_DMATAG_BA418 (5)
#define BM_DMATAG_AESGCM (6)
#define BM_DMATAG_AESXTS (7)
#define BM_DMATAG_BA421 (0x0A)
#define BM_DMATAG_BA419 (0x0B)
#define BM_DMATAG_BA423 (0x0D)
#define BM_DMATAG_BA422 (0x0E)
#define BM_DMATAG_BA424 (0x0F)
#define BM_DMATAG_CONFIG(offset) ((1 << 4) | (offset << 8))

//can be 0, 1 or 2
#define BM_DMATAG_DATATYPE(x) (x << 6)
#define BM_DMATAG_CFG_BA415_W  (BM_DMATAG_AESGCM | BM_DMATAG_CONFIG(0))
#define BM_DMATAG_CFG_BA415_KEY (BM_DMATAG_AESGCM | BM_DMATAG_CONFIG(0x10))
#define BM_DMATAG_CFG_BA415_IV (BM_DMATAG_AESGCM | BM_DMATAG_CONFIG(0x30))

#define BM_DMATAG_CFG_BA416_W  (BM_DMATAG_AESXTS | BM_DMATAG_CONFIG(0))
#define BM_DMATAG_CFG_BA416_IV (BM_DMATAG_AESXTS | BM_DMATAG_CONFIG(0x10))
#define BM_DMATAG_CFG_BA416_KEY1 (BM_DMATAG_AESXTS | BM_DMATAG_CONFIG(0x20))
#define BM_DMATAG_CFG_BA416_KEY2 (BM_DMATAG_AESXTS | BM_DMATAG_CONFIG(0x40))


#define BM_DMATAG_DATATYPE_HEADER (1 << 6)
#define BM_DMATAG_DATATYPE_REFERENCE (3 << 6)
#define BM_DMATAG_LAST (1 << 5)
#define BM_DMATAG_INVALID_BYTES_MASK 0x1F
#define BM_DMATAG_INVALID_BYTES_OFFSET 8
#define BM_DMATAG_IGN(sz) ((sz) << BM_DMATAG_INVALID_BYTES_OFFSET)

/* Private config */
#define INPUT_DESCRIPTION_NUMBER 10
#define OUTPUT_DESCRIPTION_NUMBER 10

struct bm_crypto_desc in_descs[INPUT_DESCRIPTION_NUMBER] = { 0 };
struct bm_crypto_desc out_descs[OUTPUT_DESCRIPTION_NUMBER] = { 0 };

/* Function */

void crypto_bm_test(void);

#endif /* __T_CRYPTO_BM_H__ */
