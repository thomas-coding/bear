
#include "t-crypto-bm.h"
#include "common.h"
#include "crypto_test_data.h"

#define DMATAG_CONFIG(offset) ((1 << 4) | (offset << 8))
#define DMATAG_BA413 (3)

/* 0-ok , -1-fail */
int hex_compare(uint8_t *src, uint8_t *dst, uint32_t len)
{
	for (int i = 0; i < len; i++) {
		if (*src == *dst) {
			src++;
			dst++;
		} else
			return -1;
	}
	return 0;
}

void bm_crpto_memory_hex_dump(char* start, uint8_t *buffer, uint32_t len)
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

void cypto_wait_hw(void)
{
	while (read_mreg32(BM_REG_CRYPTO_BASE + BM_REG_STATUS) & BM_REG_STATUS_BUSY_MASK);
}

void hash_sha1_bm_test(void)
{
	uint8_t sha1_digest_out[20] = { 0 };

	memset(in_descs, 0, INPUT_DESCRIPTION_NUMBER * sizeof(struct bm_crypto_desc));
	memset(out_descs, 0, OUTPUT_DESCRIPTION_NUMBER * sizeof(struct bm_crypto_desc));

	/*Input: config + data */
	uint32_t sha1_cfgword = BM_CMDMA_BA413_MODE(BM_HASH_MODE_SHA_1);
	in_descs[0].addr = (char *)&sha1_cfgword;
	in_descs[0].next = &in_descs[1];
	in_descs[0].sz = sizeof(sha1_cfgword) | BM_DMA_REALIGN;
	in_descs[0].dmatag = BM_DMATAG_BA413 | BM_DMATAG_CONFIG(0);

	in_descs[1].addr = sha1_msg;
	in_descs[1].next = BM_DMA_LAST_DESCRIPTOR;
	in_descs[1].sz = sizeof(sha1_msg) | BM_DMA_REALIGN;
	in_descs[1].dmatag = BM_DMATAG_BA413 | BM_DMATAG_LAST;

	/* Output: 20 bits data */
	out_descs[0].addr = sha1_digest_out;
	out_descs[0].next = BM_DMA_LAST_DESCRIPTOR;
	out_descs[0].sz = sizeof(sha1_digest_out);


	write_mreg32(BM_REG_CRYPTO_BASE + BM_REG_FETCH_ADDR, in_descs);
	write_mreg32(BM_REG_CRYPTO_BASE + BM_REG_PUSH_ADDR, out_descs);
	write_mreg32(BM_REG_CRYPTO_BASE + BM_REG_CONFIG, BM_REG_CONFIG_SG);
	write_mreg32(BM_REG_CRYPTO_BASE + BM_REG_START, BM_REG_START_ALL);

	cypto_wait_hw();
	bm_crpto_memory_hex_dump("bm crypto hash1 digest", sha1_digest_out, 20);
	int ret =  hex_compare(sha1_digest_out, sha1_digest, 20);
	if (ret == -1)
		vs_printf("hash1 fail\n");
}

void hash_bm_test(void)
{
	hash_sha1_bm_test();
}

void crypto_bm_test(void)
{
	vs_printf("crypto bm test enter\n");
	hash_bm_test();

}
