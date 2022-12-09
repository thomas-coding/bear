
#include "t-mbedtls.h"
#include "common.h"
#include "crypto_test_data.h"
#include "mbedtls/build_info.h"
#include "mbedtls/md5.h"
#include "mbedtls/platform.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"

#include "psa/crypto.h"

void mbedtls_hash_md5_part_test(void) {
	int i, ret;
	unsigned char digest[16];
	mbedtls_md5_context ctx;
	char str[] = "Hello, world!";

	vs_printf( "\n PART MD5('%s') = ", str );
    if( ( ret = mbedtls_md5_starts( &ctx ) ) != 0 )
        goto exit;

    if( ( ret = mbedtls_md5_update( &ctx, str, 6 ) ) != 0 )
        goto exit;

	memset(str, 0, 6);

    if( ( ret = mbedtls_md5_update( &ctx, str + 6, 7 ) ) != 0 )
        goto exit;

    if( ( ret = mbedtls_md5_finish( &ctx, digest ) ) != 0 )
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

    if( ( ret = mbedtls_md5( (unsigned char *) str, 13, digest ) ) != 0 )
        mbedtls_exit( MBEDTLS_EXIT_FAILURE );

    for( i = 0; i < 16; i++ )
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

    if( ( ret = mbedtls_sha1( (unsigned char *) str, 13, digest ) ) != 0 )
        mbedtls_exit( MBEDTLS_EXIT_FAILURE );

    for( i = 0; i < 20; i++ )
        mbedtls_printf( "%02x", digest[i] );

    mbedtls_printf( "\n\n" );

    mbedtls_exit( MBEDTLS_EXIT_SUCCESS );
}

psa_status_t mbedtls_psa_external_get_random(
    mbedtls_psa_external_random_context_t *context,
    uint8_t *output, size_t output_size, size_t *output_length )
{
    (void) context;

	memset( output, 0x2a, output_size );
    *output_length = output_size;
    return( PSA_SUCCESS );
}

void psa_sha256_test(void) {
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

    for(int i = 0; i < 20; i++ )
        mbedtls_printf( "%02x", actual_hash[i] );

    vs_printf("Hashed a message\n");

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
	/* test used alloc, not in baremetal*/
	//mbedtls_sha256_self_test(1);

	psa_sha256_test();
}

void mbed_exit(int status) {
	if(status != MBEDTLS_EXIT_SUCCESS) {
		vs_printf( "mbed_exit fail, block here\n" );
		while(1) {};
	}
}

void mbedtls_test(void)
{
	mbedtls_platform_set_printf((int (*)(const char *, ...))console_printf);
	mbedtls_platform_set_exit(mbed_exit);
	mbedtls_hash_test();
}
