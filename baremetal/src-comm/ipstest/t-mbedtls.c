#include <stdio.h>
#include <stdlib.h>
#include "t-mbedtls.h"
#include "common.h"
#include "crypto_test_data.h"
#include "mbedtls/build_info.h"
#include "mbedtls/md5.h"
#include "mbedtls/platform.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/rsa.h"

#include "psa/crypto.h"
#include "mbedtls/memory_buffer_alloc.h"

#include "mbedtls/pk.h"
#include "mbedtls/bignum.h"

uint8_t mbedtls_buf[20 * 1024];

void mbedtls_memory_hex_dump(char* start, uint8_t *buffer, uint32_t len)
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

void mbedtls_hash_md5_part_test(void)
{
	int i, ret;
	unsigned char digest[16];
	mbedtls_md5_context ctx;
	char str[] = "Hello, world!";

	vs_printf( "\n PART MD5('%s') = ", str );
	if ( ( ret = mbedtls_md5_starts( &ctx ) ) != 0 )
		goto exit;

	if ( ( ret = mbedtls_md5_update( &ctx, str, 6 ) ) != 0 )
		goto exit;

	memset(str, 0, 6);

	if ( ( ret = mbedtls_md5_update( &ctx, str + 6, 7 ) ) != 0 )
		goto exit;

	if ( ( ret = mbedtls_md5_finish( &ctx, digest ) ) != 0 )
		goto exit;

	for ( i = 0; i < 16; i++ )
		vs_printf( "%02x", digest[i] );

	vs_printf( "\n\n" );
exit:
	mbedtls_md5_free( &ctx );
}

void mbedtls_hash_md5_test(void)
{
	int i, ret;
	unsigned char digest[16];
	char str[] = "Hello, world!";

	mbedtls_printf( "\n  MD5('%s') = ", str );

	if ( ( ret = mbedtls_md5( (unsigned char *)str, 13, digest ) ) != 0 )
		mbedtls_exit( MBEDTLS_EXIT_FAILURE );

	for ( i = 0; i < 16; i++ )
		mbedtls_printf( "%02x", digest[i] );

	mbedtls_printf( "\n\n" );

	mbedtls_exit( MBEDTLS_EXIT_SUCCESS );
}

void mbedtls_hash_sha1_test(void)
{
	int i, ret;
	unsigned char digest[20];
	char str[] = "Hello, world!";

	mbedtls_printf( "\n  SHA1('%s') = ", str );

	if ( ( ret = mbedtls_sha1( (unsigned char *)str, 13, digest ) ) != 0 )
		mbedtls_exit( MBEDTLS_EXIT_FAILURE );

	for ( i = 0; i < 20; i++ )
		mbedtls_printf( "%02x", digest[i] );

	mbedtls_printf( "\n\n" );

	mbedtls_exit( MBEDTLS_EXIT_SUCCESS );
}

psa_status_t mbedtls_psa_external_get_random(
	mbedtls_psa_external_random_context_t *context,
	uint8_t *output, size_t output_size, size_t *output_length )
{
	(void)context;

	memset( output, 0x2a, output_size );
	*output_length = output_size;
	return PSA_SUCCESS;
}

void psa_sha256_test(void)
{
	psa_status_t status;
	psa_algorithm_t alg = PSA_ALG_SHA_256;
	psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
	unsigned char input[] = { 'a', 'b', 'c' };
	unsigned char actual_hash[PSA_HASH_MAX_SIZE];
	size_t actual_hash_len;

	vs_printf("Hash a message...\t");

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to initialize PSA Crypto\n");
		return;
	}

	/* Compute hash of message  */
	status = psa_hash_setup(&operation, alg);
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to begin hash operation\n");
		return;
	}
	status = psa_hash_update(&operation, input, sizeof(input));
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to update hash operation\n");
		return;
	}
	status = psa_hash_finish(&operation, actual_hash, sizeof(actual_hash),
				 &actual_hash_len);
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to finish hash operation\n");
		return;
	}

	for (int i = 0; i < 20; i++ )
		mbedtls_printf( "%02x", actual_hash[i] );

	vs_printf("\nHashed a message\n");

	/* Clean up hash operation context */
	psa_hash_abort(&operation);

	mbedtls_psa_crypto_free();
}

void mbedtls_hash_test(void)
{
	mbedtls_hash_md5_test();
	mbedtls_hash_md5_part_test();
	mbedtls_md5_self_test(1);

	mbedtls_hash_sha1_test();
	mbedtls_sha1_self_test(1);
	mbedtls_sha256_self_test(1);

	psa_sha256_test();
}

static int myrand( void *rng_state, unsigned char *output, size_t len )
{
	size_t use_len;
	int rnd;

	if ( rng_state != NULL )
		rng_state = NULL;

	while ( len > 0 ) {
		use_len = len;
		if ( use_len > sizeof(int) )
			use_len = sizeof(int);

		rnd = rand();
		memcpy( output, &rnd, use_len );
		output += use_len;
		len -= use_len;
	}

	return 0;
}

/* RSA 1024, with baremetal key(N/D/E) */
void mbedtls_rsa_crypt_test_bmkey(void)
{
	int keysize;
	mbedtls_rsa_context rsa;
	int i, ret;
	size_t len;
	unsigned char rsa_decrypted[28];
	unsigned char rsa_ciphertext[128];

	vs_printf( "mbedtls_rsa_crypt_test_bmkey wait...\n");

	mbedtls_rsa_init( &rsa );

	/* Add N/D/E */
	if ( ( ret = mbedtls_rsa_import_raw( &rsa, rsaes_pkcs1v15_rsa_modulus, 128, \
					     NULL, 0, NULL, 0, \
					     rsaes_pkcs1v15_rsa_priv_exponent, 128, \
					     rsaes_pkcs1v15_rsa_pub_exponent, 3 ) ) != 0 ) {
		vs_printf( "mbedtls_rsa_import_raw fail\n" );
		return;
	}

	/* Generate key and check */
	if ( mbedtls_rsa_complete( &rsa ) != 0 ||
	     mbedtls_rsa_check_pubkey( &rsa ) != 0 ) {
		vs_printf( "generate key fail\n");
		return;
	}

	/* Encrypt text */
	if ( mbedtls_rsa_pkcs1_encrypt( &rsa, myrand, NULL,
					28, rsaes_pkcs1v15_message,
					rsa_ciphertext ) != 0 ) {
		vs_printf( "rsa encrypt fail\n");
		return;
	}

	mbedtls_memory_hex_dump("chipher text", rsa_ciphertext, 128);

	/* Decrpt cipher text */
	if ( mbedtls_rsa_pkcs1_decrypt( &rsa, myrand, NULL,
					&len, rsa_ciphertext, rsa_decrypted,
					sizeof(rsa_decrypted) ) != 0 ) {
		vs_printf( "rsa decryt fail\n");
		return;
	}

	mbedtls_memory_hex_dump("decrypted text", rsa_decrypted, len);

	/* Compare output with rsaes_pkcs1v15_message */
	if (memcmp(rsa_decrypted, rsaes_pkcs1v15_message, len) != 0 )
		vs_printf("mbedtls_rsa_crypt_test_bmkey check result fail\n");

	vs_printf( "mbedtls_rsa_crypt_test_bmkey done\n");
}

/* Get key form pem, encrypt and decrypt, sign and verify */
void mbedtls_rsa_crypt_test_pem(void)
{
	int keysize;
	mbedtls_rsa_context *rsa;
	mbedtls_rsa_context *rsa_pub;
	int i, ret;
	size_t len;

	/* Private key parse from pem ------------------------------------  */
	mbedtls_pk_context key;
	mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;

	mbedtls_pk_init( &key );

	mbedtls_mpi_init( &N ); mbedtls_mpi_init( &P ); mbedtls_mpi_init( &Q );
	mbedtls_mpi_init( &D ); mbedtls_mpi_init( &E ); mbedtls_mpi_init( &DP );
	mbedtls_mpi_init( &DQ ); mbedtls_mpi_init( &QP );

	if ( ( ret = mbedtls_pk_parse_key(&key, myprivate_pem, myprivate_pem_len, NULL, 0, myrand, NULL) ) != 0 ) {
		vs_printf( "mbedtls_rsa_import_raw fail\n" );
		return;
	}

	if ( mbedtls_pk_get_type( &key ) == MBEDTLS_PK_RSA ) {
		rsa = mbedtls_pk_rsa( key );

		/* Alt: also can use mbedtls_rsa_export_raw export to buffer directly */
		if ( ( ret = mbedtls_rsa_export( rsa, &N, &P, &Q, &D, &E ) ) != 0 ||
		     ( ret = mbedtls_rsa_export_crt( rsa, &DP, &DQ, &QP ) )      != 0 ) {
			vs_printf( " failed\n  ! could not export RSA parameters\n\n" );
			return;
		}

		unsigned char e_buffer[3];
		if ( ( ret = mbedtls_mpi_write_binary(&E, e_buffer, 3) ) != 0 ) {
			vs_printf( "mbedtls_mpi_write_binary e fail, ret:%d\n", ret );
			return;
		}
		mbedtls_memory_hex_dump("E", e_buffer, 3);

		unsigned char n_buffer[128];
		if ( ( ret = mbedtls_mpi_write_binary(&N, n_buffer, 128) ) != 0 ) {
			vs_printf( "mbedtls_mpi_write_binary n fail, ret:%d\n", ret );
			return;
		}
		mbedtls_memory_hex_dump("N", n_buffer, 128);

		unsigned char d_buffer[128];
		if ( ( ret = mbedtls_mpi_write_binary(&D, d_buffer, 128) ) != 0 ) {
			vs_printf( "mbedtls_mpi_write_binary d fail, ret:%d\n", ret );
			return;
		}
		mbedtls_memory_hex_dump("D", d_buffer, 128);

		unsigned char p_buffer[64];
		if ( ( ret = mbedtls_mpi_write_binary(&P, p_buffer, 64) ) != 0 ) {
			vs_printf( "mbedtls_mpi_write_binary p fail, ret:%d\n", ret );
			return;
		}
		mbedtls_memory_hex_dump("P", p_buffer, 64);

		unsigned char q_buffer[64];
		if ( ( ret = mbedtls_mpi_write_binary(&Q, q_buffer, 64) ) != 0 ) {
			vs_printf( "mbedtls_mpi_write_binary q fail, ret:%d\n", ret );
			return;
		}
		mbedtls_memory_hex_dump("Q", q_buffer, 64);

	} else {
		vs_printf( "mbedtls_rsa_crypt_test_pem unknow key type\n");
		return;
	}

	/* Public key parse from pem ------------------------------------  */
	mbedtls_pk_context pub_key;
	mbedtls_mpi PUB_N, PUB_E;

	mbedtls_mpi_init( &PUB_N );
	mbedtls_mpi_init( &PUB_E );

	mbedtls_pk_init( &pub_key );

	if ( ( ret = mbedtls_pk_parse_public_key(&pub_key, mypublic_pem, mypublic_pem_len) ) != 0 ) {
		vs_printf( "mbedtls_pk_parse_public_key fail\n" );
		return;
	}

	if ( mbedtls_pk_get_type( &pub_key ) == MBEDTLS_PK_RSA ) {
		rsa_pub = mbedtls_pk_rsa( pub_key );

		if ( ( ret = mbedtls_rsa_check_pubkey(rsa_pub) ) != 0 ) {
			vs_printf( "mbedtls_rsa_check_pubkey fail\n" );
			return;
		}

		if ( ( ret = mbedtls_rsa_export( rsa_pub, &PUB_N, NULL, NULL, NULL, &PUB_E ) ) != 0 ) {
			vs_printf( " failed\n  ! could not export RSA parameters\n\n" );
			return;
		}

		unsigned char pub_e_buffer[3];
		if ( ( ret = mbedtls_mpi_write_binary(&PUB_E, pub_e_buffer, 3) ) != 0 ) {
			vs_printf( "mbedtls_mpi_write_binary pub e fail, ret:%d\n", ret );
			return;
		}
		mbedtls_memory_hex_dump("PUB E", pub_e_buffer, 3);

		unsigned char pub_n_buffer[128];
		if ( ( ret = mbedtls_mpi_write_binary(&PUB_N, pub_n_buffer, 128) ) != 0 ) {
			vs_printf( "mbedtls_mpi_write_binary pub n fail, ret:%d\n", ret );
			return;
		}
		mbedtls_memory_hex_dump("PUB N", pub_n_buffer, 128);


	} else {
		vs_printf( "mbedtls_rsa_crypt_test_pem unknow key type\n");
		return;
	}

	/* Use public key to encrypt ------------------------------------------------------ */
	unsigned char rsa_ciphertext[128];
	/* Encrypt text */
	if ( mbedtls_rsa_pkcs1_encrypt( rsa_pub, myrand, NULL,
					28, rsaes_pkcs1v15_message,
					rsa_ciphertext ) != 0 ) {
		vs_printf( "rsa encrypt fail\n");
		return;
	}

	mbedtls_memory_hex_dump("chipher text", rsa_ciphertext, 128);

	/* Use private key to decrypt ------------------------------------------------------ */
	unsigned char rsa_decrypted[28];
	/* Decrpt cipher text */
	if ( mbedtls_rsa_pkcs1_decrypt( rsa, myrand, NULL,
					&len, rsa_ciphertext, rsa_decrypted,
					sizeof(rsa_decrypted) ) != 0 ) {
		vs_printf( "rsa decryt fail\n");
		return;
	}

	mbedtls_memory_hex_dump("decrypted text", rsa_decrypted, len);

	/* Compare output with rsaes_pkcs1v15_message */
	if (memcmp(rsa_decrypted, rsaes_pkcs1v15_message, len) != 0 ) {
		vs_printf("mbedtls_rsa_crypt_test_bmkey check result fail\n");
		return;
	}

	/* Use private key to sign ----------------------------------------------------------- */
	uint8_t message[] = "hello word";
	unsigned char sign_buf[128];
	if ( ( ret = mbedtls_rsa_pkcs1_sign(rsa, myrand, NULL, MBEDTLS_MD_NONE,
					    10, message, sign_buf ) ) != 0 ) {
		mbedtls_printf( " failed\n  ! mbedtls_rsa_pkcs1_sign returned -0x%0x\n\n", (unsigned int)-ret );
		return;
	}
	mbedtls_memory_hex_dump("sign", sign_buf, 128);


	/* Use public key to verify ----------------------------------------------------------- */
	if ( ( ret = mbedtls_rsa_pkcs1_verify(rsa_pub, MBEDTLS_MD_NONE,
					      10, message, sign_buf ) ) != 0 ) {
		mbedtls_printf( " failed\n  ! mbedtls_rsa_pkcs1_verify returned -0x%0x\n\n", (unsigned int)-ret );
		return;
	} else
		vs_printf( "sign verify ok\n");

	vs_printf( "\nmbedtls_rsa_crypt_test_pem done\n");

}

/* Use pem private key sign message and pem public key verify */
void psa_rsa_test(const uint8_t *key, size_t key_len, const uint8_t *pub_key, size_t pub_key_len)
{
	psa_status_t status;
	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
	uint8_t hash[32] = { 0x50, 0xd8, 0x58, 0xe0, 0x98, 0x5e, 0xcc, 0x7f,
			     0x60, 0x41, 0x8a, 0xaf, 0x0c, 0xc5, 0xab, 0x58,
			     0x7f, 0x42, 0xc2, 0x57, 0x0a, 0x88, 0x40, 0x95,
			     0xa9, 0xe8, 0xcc, 0xac, 0xd0, 0xf6, 0x54, 0x5c };
	uint8_t signature[PSA_SIGNATURE_MAX_SIZE] = { 0 };
	size_t signature_length;
	psa_key_id_t key_id;

	vs_printf("Sign a message...\n");

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to initialize PSA Crypto\n");
		return;
	}

	/* Sign ---------------------------------------------------------*/
	/* Set key attributes */
	psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH);
	psa_set_key_algorithm(&attributes, PSA_ALG_RSA_PKCS1V15_SIGN_RAW);
	psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
	psa_set_key_bits(&attributes, 1024);

	/* Import the key */
	status = psa_import_key(&attributes, key, key_len, &key_id);
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to import key\n");
		return;
	}

	/* Sign message using the key */
	status = psa_sign_hash(key_id, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
			       hash, sizeof(hash),
			       signature, sizeof(signature),
			       &signature_length);
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to sign\n");
		return;
	}

	vs_printf("Signed a message\n");

	/* Verify signature ---------------------------------------------------------*/
	psa_key_id_t pub_key_id;

	/* Set key attributes */
	psa_key_attributes_t pubkey_attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_usage_flags(&pubkey_attributes, PSA_KEY_USAGE_VERIFY_HASH);
	psa_set_key_algorithm(&pubkey_attributes, PSA_ALG_RSA_PKCS1V15_SIGN_RAW);
	psa_set_key_type(&pubkey_attributes, PSA_KEY_TYPE_RSA_PUBLIC_KEY);
	psa_set_key_bits(&pubkey_attributes, 1024);

	/* Import the key */
	status = psa_import_key(&pubkey_attributes, pub_key, pub_key_len, &pub_key_id);
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to import public key\n");
		return;
	}

	status = psa_verify_hash(pub_key_id, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
				 hash, sizeof(hash),
				 signature, signature_length);
	if (status != PSA_SUCCESS) {
		vs_printf("Failed to vefiry \n");
		return;
	} else
		vs_printf("verify ok \n");

	/* Free the attributes */
	psa_reset_key_attributes(&attributes);
	psa_reset_key_attributes(&pubkey_attributes);

	/* Destroy the key */
	psa_destroy_key(key_id);
	psa_destroy_key(pub_key_id);

	mbedtls_psa_crypto_free();
}

void mbedtls_rsa_test(void)
{
	mbedtls_rsa_crypt_test_bmkey();
	mbedtls_rsa_crypt_test_pem();
	mbedtls_rsa_self_test(1);
	psa_rsa_test(myprivate_pem, myprivate_pem_len, mypublic_pem, mypublic_pem_len);
}

void mbed_exit(int status)
{
	if (status != MBEDTLS_EXIT_SUCCESS) {
		vs_printf( "mbed_exit fail, block here\n" );
		while (1) {
		}
		;
	}
}

void mbedtls_init(void)
{
	mbedtls_platform_set_printf((int (*)(const char *, ...)) console_printf);
	mbedtls_platform_set_exit(mbed_exit);
	mbedtls_memory_buffer_alloc_init( mbedtls_buf, sizeof(mbedtls_buf) );
}

void mbedtls_test(void)
{
	mbedtls_init();
	mbedtls_hash_test();
	mbedtls_rsa_test();
}
