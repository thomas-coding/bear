
#include "t-mbedtls.h"
#include "common.h"
#include "crypto_test_data.h"
#include "mbedtls/build_info.h"
#include "mbedtls/md5.h"
#include "mbedtls/platform.h"

void mbedtls_hash_md5_test(void)
{
	int i, ret;
	unsigned char digest[16];
	char str[] = "Hello, world!";

	vs_printf( "\n  MD5('%s') = ", str );

	if ( ( ret = mbedtls_md5( (unsigned char *)str, 13, digest ) ) != 0 ) {
		vs_printf( "md5 fail\n" );
		return;
	}

	for ( i = 0; i < 16; i++ )
		vs_printf( "%02x", digest[i] );

	vs_printf( "\n\n" );
}

void mbedtls_hash_test(void)
{
	mbedtls_hash_md5_test();
}

void mbedtls_test(void)
{
	vs_printf("mbedtls test enter\n");
	mbedtls_hash_test();

}