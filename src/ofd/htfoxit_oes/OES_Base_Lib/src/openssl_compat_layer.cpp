
//OpenSSL
#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/x509v3.h>
#include <openssl/asn1.h>

#include "../include/openssl_compat_layer.h"


static void *OPENSSL_zalloc(size_t num)
 {
    void *ret = OPENSSL_malloc(num);

    if (ret != NULL)
        memset(ret, 0, num);
    return ret;
 }

EVP_MD_CTX *EVP_MD_CTX_new(void)
{
    return (EVP_MD_CTX *)OPENSSL_zalloc(sizeof(EVP_MD_CTX));
}

void EVP_MD_CTX_free(EVP_MD_CTX *ctx)
{
    EVP_MD_CTX_cleanup(ctx);
    OPENSSL_free(ctx);
}
