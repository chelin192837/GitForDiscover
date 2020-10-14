// author: gm
// date: 20170809
#ifndef  _OPENSSL_COMPAT_LAYER_H_
#define _OPENSSL_COMPAT_LAYER_H_

EVP_MD_CTX *EVP_MD_CTX_new(void);

void EVP_MD_CTX_free(EVP_MD_CTX *ctx);


#endif
