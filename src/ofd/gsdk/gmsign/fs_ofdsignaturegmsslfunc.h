#ifndef _FS_OFDSIGNATUREGMSSLFUNC_H_
#define _FS_OFDSIGNATUREGMSSLFUNC_H_

#include "../../fxlib/GmSSL/include/openssl/rsa.h"
#include "../../fxlib/GmSSL/include/openssl/evp.h"
#include "../../fxlib/GmSSL/include/openssl/objects.h"
#include "../../fxlib/GmSSL/include/openssl/x509.h"
#include "../../fxlib/GmSSL/include/openssl/ts.h"
#include "../../fxlib/GmSSL/include/openssl/safestack.h"
#include "../../fxlib/GmSSL/include/openssl/err.h"
#include "../../fxlib/GmSSL/include/openssl/pem.h"
#include "../../fxlib/GmSSL/include/openssl/ssl.h"
#include "../../fxlib/GmSSL/include/openssl/pkcs12.h"
#include "../../fxlib/GmSSL/include/openssl/rand.h"
#include "../../fxlib/GmSSL/include/openssl/pkcs7.h"
#include "../../fxlib/GmSSL/include/openssl/asn1.h"
#include "../../fxlib/GmSSL/include/openssl/sm2.h"


typedef void(*DLL_X509_free)(X509* a);
typedef void(*DLL_OPENSSL_add_all_algorithms_noconf)(void);
typedef void(*DLL_OPENSSL_add_all_algorithms_conf)(void);
typedef void(*DLL_ERR_load_ERR_strings)(void);
typedef void(*DLL_ERR_load_crypto_strings)(void);
typedef void(*DLL_EVP_PKEY_free)(EVP_PKEY *pkey);
typedef void(*DLL_sk_free)(_STACK *);
typedef void*(*DLL_X509_get_ext_d2i)(X509 *x, int nid, int *crit, int *idx);
typedef ASN1_STRING*(*DLL_ASN1_STRING_dup)(const ASN1_STRING *a);
typedef int(*DLL_ASN1_UTCTIME_cmp_time_t)(const ASN1_UTCTIME *s, time_t t);
typedef void(*DLL_ASN1_STRING_free)(ASN1_STRING *a);
typedef void(*DLL_OpenSSL_add_all_ciphers)(void);
typedef void(*DLL_OpenSSL_add_all_digests)(void);
typedef PKCS12*(*DLL_d2i_PKCS12_fp)(FILE *fp, PKCS12 **p12);
typedef int(*DLL_PKCS12_parse)(PKCS12 *p12, const char *pass, EVP_PKEY **pkey, X509 **cert, STACK_OF(X509) **ca);
typedef void(*DLL_PKCS12_free)(PKCS12 *a);
typedef void(*DLL_OBJ_obj2txt)(char *buf, int buf_len, const ASN1_OBJECT *a, int no_name);
typedef int(*DLL_OBJ_txt2nid)(const char *s);
typedef PKCS7*(*DLL_PKCS7_new)();
typedef int(*DLL_PKCS7_set_type)(PKCS7 *p7, int type);
typedef int(*DLL_PKCS7_content_new)(PKCS7 *p7, int nid);
typedef int(*DLL_PKCS7_ctrl)(PKCS7 *p7, int cmd, long larg, char *parg);
typedef PKCS7_SIGNER_INFO*(*DLL_PKCS7_add_signature)(PKCS7 *p7, X509 *x509, EVP_PKEY *pkey, const EVP_MD *dgst);
typedef ASN1_TIME*(*DLL_X509_gmtime_adj)(ASN1_TIME *s, long adj);
typedef int(*DLL_PKCS7_add_signed_attribute)(PKCS7_SIGNER_INFO *p7si, int nid, int type, void *data);
typedef int(*DLL_Setsm2sm3ID)(unsigned char *pID, int nIDlen);
typedef void(*DLL_SetX509)(X509 *p509);
typedef int(*DLL_PKCS7_add_certificate)(PKCS7 *p7, X509 *x509);
typedef int(*DLL_sk_num)(const _STACK *);
typedef void*(*DLL_sk_value)(const _STACK *, int);
typedef BIO*(*DLL_PKCS7_dataInit)(PKCS7 *p7, BIO *bio);
typedef int(*DLL_PKCS7_dataFinal)(PKCS7 *p7, BIO *bio);
typedef int(*DLL_BIO_write)(BIO *b, const void *data, int len);
typedef void(*DLL_BIO_free_all)(BIO *a);
typedef int(*DLL_i2d_PKCS7)(PKCS7 *p7, unsigned char **out);
typedef void(*DLL_PKCS7_free)(PKCS7 *p7);
typedef BIO *(*DLL_BIO_new_mem_buf)(void *buf, int len);
typedef PKCS7 *(*DLL_d2i_PKCS7_bio)(BIO *bp, PKCS7 **p7);
typedef STACK_OF(PKCS7_SIGNER_INFO) *(*DLL_PKCS7_get_signer_info)(PKCS7 *p7);
typedef int(*DLL_PKCS7_add_signed_attribute)(PKCS7_SIGNER_INFO *p7si, int nid, int type, void *data);
typedef int(*DLL_PKCS7_add_attribute)(PKCS7_SIGNER_INFO *p7si, int nid, int atrtype, void *value);
typedef ASN1_TYPE *(*DLL_PKCS7_get_attribute)(PKCS7_SIGNER_INFO *si, int nid);
typedef ASN1_TYPE *(*DLL_PKCS7_get_signed_attribute)(PKCS7_SIGNER_INFO *si, int nid);
typedef int(*DLL_PKCS7_set_signed_attributes)(PKCS7_SIGNER_INFO *p7si, STACK_OF(X509_ATTRIBUTE) *sk);
typedef int(*DLL_PKCS7_set_attributes)(PKCS7_SIGNER_INFO *p7si, STACK_OF(X509_ATTRIBUTE) *sk);
typedef PKCS7 *(*DLL_PKCS7_sign)(X509 *signcert, EVP_PKEY *pkey, STACK_OF(X509) *certs, BIO *data, int flags);
typedef int(*DLL_ASN1_STRING_length)(const ASN1_STRING *x);
typedef unsigned char *(*DLL_ASN1_STRING_data)(ASN1_STRING *x);
typedef PKCS7 *(*DLL_d2i_PKCS7)(PKCS7 **a, const unsigned char **in, long len);
typedef BIO *(*DLL_PKCS7_dataDecode)(PKCS7 *p7, EVP_PKEY *pkey, BIO *in_bio, X509 *pcert);
typedef int(*DLL_BIO_read)(BIO *b, void *data, int len);
typedef TS_TST_INFO *(*DLL_d2i_TS_TST_INFO)(TS_TST_INFO **a, const unsigned char **pp, long length);
typedef unsigned char *(*DLL_SHA1)(const unsigned char *d, size_t n, unsigned char *md);
typedef unsigned char *(*DLL_SHA256)(const unsigned char *d, size_t n, unsigned char *md);
typedef X509 *(*DLL_PKCS7_cert_from_signer_info)(PKCS7 *p7, PKCS7_SIGNER_INFO *si);
typedef ASN1_INTEGER *(*DLL_X509_get_serialNumber)(X509 *x);
typedef X509_NAME *(*DLL_X509_get_issuer_name)(X509 *a);
typedef X509_NAME *(*DLL_X509_get_subject_name)(X509 *a);
typedef X509_NAME_ENTRY *(*DLL_X509_NAME_get_entry)(X509_NAME *name, int loc);
typedef int(*DLL_X509_NAME_get_text_by_NID)(X509_NAME *name, int nid, char *buf, int len);
typedef int(*DLL_PKCS7_verify)(PKCS7 *p7, STACK_OF(X509) *certs, X509_STORE *store, BIO *indata, BIO *out, int flags);
typedef int(*DLL_BIO_free)(BIO *a);
typedef int(*DLL_SHA256_Init)(SHA256_CTX *c);
typedef int(*DLL_SHA256_Update)(SHA256_CTX *c, const void *data, size_t len);
typedef int(*DLL_SHA256_Final)(unsigned char *md, SHA256_CTX *c);
typedef void(*DLL_ASN1_INTEGER_free)(ASN1_INTEGER *asn);
typedef ASN1_VALUE *(*DLL_ASN1_item_new)(const ASN1_ITEM *it);
typedef void(*DLL_CRYPTO_free)(void *ptr);
typedef int(*DLL_RAND_bytes)(unsigned char *buf, int num);
typedef void *(*DLL_CRYPTO_malloc)(int num, const char *file, int line);
typedef const EVP_MD *(*DLL_EVP_get_digestbyname)(const char *name);
typedef TS_REQ *(*DLL_TS_REQ_new)(void);
typedef void (*DLL_TS_REQ_free)(TS_REQ *a);
typedef int (*DLL_i2d_TS_REQ)(const TS_REQ *a, unsigned char **pp);
typedef int (*DLL_TS_REQ_set_version)(TS_REQ *a, long version);
typedef void (*DLL_TS_MSG_IMPRINT_free)(TS_MSG_IMPRINT *a);
typedef int (*DLL_TS_MSG_IMPRINT_set_algo)(TS_MSG_IMPRINT *a, X509_ALGOR *alg);
typedef int (*DLL_TS_MSG_IMPRINT_set_msg)(TS_MSG_IMPRINT *a, unsigned char *d, int len);
typedef int (*DLL_TS_REQ_set_msg_imprint)(TS_REQ *a, TS_MSG_IMPRINT *msg_imprint);
typedef int (*DLL_TS_REQ_set_nonce)(TS_REQ *a, const ASN1_INTEGER *nonce);
typedef int (*DLL_TS_REQ_set_cert_req)(TS_REQ *a, int cert_req);
typedef TS_RESP *(*DLL_d2i_TS_RESP)(TS_RESP **a, const unsigned char **pp, long length);
typedef void (*DLL_TS_RESP_free)(TS_RESP *a);
typedef TS_MSG_IMPRINT *(*DLL_TS_MSG_IMPRINT_new)();
typedef X509_ALGOR *(*DLL_X509_ALGOR_new)();
typedef ASN1_OBJECT *(*DLL_OBJ_nid2obj)(int n);
typedef ASN1_TYPE *(*DLL_ASN1_TYPE_new)();
typedef void (*DLL_X509_ALGOR_free)(X509_ALGOR *algo);
typedef ASN1_INTEGER *(*DLL_ASN1_INTEGER_new)(void);
typedef const EVP_MD *(*DLL_EVP_sha1)(void);
typedef const EVP_MD *(*DLL_EVP_sm3)(void);
typedef int (*DLL_EVP_MD_type)(const EVP_MD *md);
typedef void (*DLL_SetApplinkFunc)(applinkfunctype);

enum {
	FuncId_X509_free,
	FuncId_OPENSSL_add_all_algorithms_noconf,
	FuncId_OPENSSL_add_all_algorithms_conf,
	FuncId_ERR_load_ERR_strings,
	FuncId_ERR_load_crypto_strings,
	FuncId_EVP_PKEY_free,
	FuncId_sk_free,
	FuncId_X509_get_ext_d2i,
	FuncId_ASN1_STRING_dup,
	FuncId_ASN1_UTCTIME_cmp_time_t,
	FuncId_ASN1_STRING_free,
	FuncId_OpenSSL_add_all_ciphers,
	FuncId_OpenSSL_add_all_digests,
	FuncId_d2i_PKCS12_fp,
	FuncId_PKCS12_parse,
	FuncId_PKCS12_free,
	FuncId_OBJ_obj2txt,
	FuncId_OBJ_txt2nid,
	FuncId_PKCS7_new,
	FuncId_PKCS7_set_type,
	FuncId_PKCS7_content_new,
	FuncId_PKCS7_ctrl,
	FuncId_PKCS7_add_signature,
	FuncId_X509_gmtime_adj,
	FuncId_PKCS7_add_signed_attribute,
	FuncId_Setsm2sm3ID,
	FuncId_SetX509,
	FuncId_PKCS7_add_certificate,
	FuncId_sk_num,
	FuncId_sk_value,
	FuncId_PKCS7_dataInit,
	FuncId_PKCS7_dataFinal,
	FuncId_BIO_write,
	FuncId_BIO_free_all,
	FuncId_i2d_PKCS7,
	FuncId_PKCS7_free,
	FuncId_BIO_new_mem_buf,
	FuncId_d2i_PKCS7_bio,
	FuncId_PKCS7_get_signer_info,
	FuncId_PKCS7_add_attribute,
	FuncId_PKCS7_get_attribute,
	FuncId_PKCS7_get_signed_attribute,
	FuncId_PKCS7_set_signed_attributes,
	FuncId_PKCS7_set_attributes,
	FuncId_PKCS7_sign,
	FuncId_ASN1_STRING_length,
	FuncId_ASN1_STRING_data,
	FuncId_d2i_PKCS7,
	FuncId_PKCS7_dataDecode,
	FuncId_BIO_read,
	FuncId_d2i_TS_TST_INFO,
	FuncId_SHA1,
	FuncId_SHA256,
	FuncId_PKCS7_cert_from_signer_info,
	FuncId_X509_get_serialNumber,
	FuncId_X509_get_issuer_name,
	FuncId_X509_get_subject_name,
	FuncId_X509_NAME_get_entry,
	FuncId_X509_NAME_get_text_by_NID,
	FuncId_PKCS7_verify,
	FuncId_BIO_free,
	FuncId_SHA256_Init,
	FuncId_SHA256_Update,
	FuncId_SHA256_Final,
	FuncId_ASN1_INTEGER_free,
	FuncId_ASN1_item_new,
	FuncId_CRYPTO_free,
	FuncId_RAND_bytes,
	FuncId_CRYPTO_malloc,
	FuncId_EVP_get_digestbyname,
	FuncId_TS_REQ_new,
	FuncId_TS_REQ_free,
	FuncId_i2d_TS_REQ,
	FuncId_TS_REQ_set_version,
	FuncId_TS_MSG_IMPRINT_free,
	FuncId_TS_MSG_IMPRINT_set_algo,
	FuncId_TS_MSG_IMPRINT_set_msg,
	FuncId_TS_REQ_set_msg_imprint,
	FuncId_TS_REQ_set_nonce,
	FuncId_TS_REQ_set_cert_req,
	FuncId_d2i_TS_RESP,
	FuncId_TS_RESP_free,
	FuncId_TS_MSG_IMPRINT_new,
	FuncId_X509_ALGOR_new,
	FuncId_OBJ_nid2obj,
	FuncId_ASN1_TYPE_new,
	FuncId_X509_ALGOR_free,
	FuncId_ASN1_INTEGER_new,
	FuncId_EVP_sha1,
	FuncId_EVP_sm3,
	FuncId_EVP_MD_type,
	FuncId_SetApplinkFunc
};

static FX_LPCSTR g_GmsslpFuncNames[] = {
	"X509_free",
	"OPENSSL_add_all_algorithms_noconf",
	"OPENSSL_add_all_algorithms_conf",
	"ERR_load_ERR_strings",
	"ERR_load_crypto_strings",
	"EVP_PKEY_free",
	"sk_free",
	"X509_get_ext_d2i",
	"ASN1_STRING_dup",
	"ASN1_UTCTIME_cmp_time_t",
	"ASN1_STRING_free",
	"OpenSSL_add_all_ciphers",
	"OpenSSL_add_all_digests",
	"d2i_PKCS12_fp",
	"PKCS12_parse",
	"PKCS12_free",
	"OBJ_obj2txt",
	"OBJ_txt2nid",
	"PKCS7_new",
	"PKCS7_set_type",
	"PKCS7_content_new",
	"PKCS7_ctrl",
	"PKCS7_add_signature",
	"X509_gmtime_adj",
	"PKCS7_add_signed_attribute",
	"Setsm2sm3ID",
	"SetX509",
	"PKCS7_add_certificate",
	"sk_num",
	"sk_value",
	"PKCS7_dataInit",
	"PKCS7_dataFinal",
	"BIO_write",
	"BIO_free_all",
	"i2d_PKCS7",
	"PKCS7_free",
	"BIO_new_mem_buf",
	"d2i_PKCS7_bio",
	"PKCS7_get_signer_info",
	"PKCS7_add_attribute",
	"PKCS7_get_attribute",
	"PKCS7_get_signed_attribute",
	"PKCS7_set_signed_attributes",
	"PKCS7_set_attributes",
	"PKCS7_sign",
	"ASN1_STRING_length",
	"ASN1_STRING_data",
	"d2i_PKCS7",
	"PKCS7_dataDecode",
	"BIO_read",
	"d2i_TS_TST_INFO",
	"SHA1",
	"SHA256",
	"PKCS7_cert_from_signer_info",
	"X509_get_serialNumber",
	"X509_get_issuer_name",
	"X509_get_subject_name",
	"X509_NAME_get_entry",
	"X509_NAME_get_text_by_NID",
	"PKCS7_verify",
	"BIO_free",
	"SHA256_Init",
	"SHA256_Update",
	"SHA256_Final",
	"ASN1_INTEGER_free",
	"ASN1_item_new",
	"CRYPTO_free",
	"RAND_bytes",
	"CRYPTO_malloc",
	"EVP_get_digestbyname",
	"TS_REQ_new",
	"TS_REQ_free",
	"i2d_TS_REQ",
	"TS_REQ_set_version",
	"TS_MSG_IMPRINT_free",
	"TS_MSG_IMPRINT_set_algo",
	"TS_MSG_IMPRINT_set_msg",
	"TS_REQ_set_msg_imprint",
	"TS_REQ_set_nonce",
	"TS_REQ_set_cert_req",
	"d2i_TS_RESP",
	"TS_RESP_free",
	"TS_MSG_IMPRINT_new",
	"X509_ALGOR_new",
	"OBJ_nid2obj",
	"ASN1_TYPE_new",
	"X509_ALGOR_free",
	"ASN1_INTEGER_new",
	"EVP_sha1",
	"EVP_sm3",
	"EVP_MD_type",
	"SetApplinkFunc"
};

class GmsslFunctions : public CFX_Object
{
public:
	GmsslFunctions();
	~GmsslFunctions();

	FX_BOOL				LoadAllFunctions();
	FX_BOOL				IsValid();

	void				*m_Functions[100];

private:
#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
	HMODULE				m_gmsslHandle;
#else
	void*				m_gmsslHandle;
#endif

};

extern GmsslFunctions*  g_GmsslFunctions;

#define X509_free(paramter)											CallFunction(X509_free)(paramter)
#define SSLeay_add_all_algorithms()									OpenSSL_add_all_algorithms()
#define OPENSSL_add_all_algorithms_noconf()							CallFunction(OPENSSL_add_all_algorithms_noconf)()
#define OPENSSL_add_all_algorithms_conf()							CallFunction(OPENSSL_add_all_algorithms_conf)()
#define ERR_load_ERR_strings()										CallFunction(ERR_load_ERR_strings)()
#define SHA256_Init(SHA256_CTX_c)									CallFunction(SHA256_Init)(SHA256_CTX_c)
#define EVP_PKEY_free(pkey)											CallFunction(EVP_PKEY_free)(pkey)
#define sk_free(paramter)											CallFunction(sk_free)(paramter)
#define X509_get_ext_d2i(px, nid, pcrit, pidx)						CallFunction(X509_get_ext_d2i)(px, nid, pcrit, pidx)
#define ASN1_STRING_dup(ASN1_STRING_a)								CallFunction(ASN1_STRING_dup)(ASN1_STRING_a)
#define ASN1_UTCTIME_cmp_time_t(const_ASN1_UTCTIME_s, stime)		CallFunction(ASN1_UTCTIME_cmp_time_t)(const_ASN1_UTCTIME_s, stime)
#define ASN1_STRING_free(ASN1_STRING_a)								CallFunction(ASN1_STRING_free)(ASN1_STRING_a)
#define OpenSSL_add_all_ciphers()									CallFunction(OpenSSL_add_all_ciphers)()
#define OpenSSL_add_all_digests()									CallFunction(OpenSSL_add_all_digests)()
#define d2i_PKCS12_fp(fp,pp12)										CallFunction(d2i_PKCS12_fp)(fp,pp12)
#define PKCS12_parse(p12,pass,ppkey,ppcert,ppca)					CallFunction(PKCS12_parse)(p12,pass,ppkey,ppcert,ppca)
#define PKCS12_free(pkcs12)											CallFunction(PKCS12_free)(pkcs12)
#define OBJ_obj2txt(pbuf,buf_len,ASN1_OBJEC_a,no_name)				CallFunction(OBJ_obj2txt)(pbuf,buf_len,ASN1_OBJEC_a,no_name)
#define OBJ_txt2nid(s)												CallFunction(OBJ_txt2nid)(s)
#define PKCS7_new()													CallFunction(PKCS7_new)()
#define PKCS7_set_type(p7, type)									CallFunction(PKCS7_set_type)(p7, type)
#define PKCS7_content_new(p7, nid)									CallFunction(PKCS7_content_new)(p7, nid)
#define PKCS7_ctrl(p7, cmd, arg, parg)								CallFunction(PKCS7_ctrl)(p7, cmd, arg, parg)
#define PKCS7_add_signature(p7, x509, pkey, dgst)					CallFunction(PKCS7_add_signature)(p7, x509, pkey, dgst)
#define X509_gmtime_adj(time_s, adj)								CallFunction(X509_gmtime_adj)(time_s, adj)
#define PKCS7_add_signed_attribute(p7si, nid, type,	pdata)			CallFunction(PKCS7_add_signed_attribute)(p7si, nid, type, pdata)
#define Setsm2sm3ID(pID, nIDlen)									CallFunction(Setsm2sm3ID)(pID, nIDlen)
#define SetX509(p509)												CallFunction(SetX509)(p509)
#define PKCS7_add_certificate(p7, p509)								CallFunction(PKCS7_add_certificate)(p7, p509)
#define sk_num(pstack)												CallFunction(sk_num)(pstack)
#define sk_value(pstack, num)										CallFunction(sk_value)(pstack, num)
#define PKCS7_dataInit(p7, bio)										CallFunction(PKCS7_dataInit)(p7, bio)
#define PKCS7_dataFinal(p7, bio)									CallFunction(PKCS7_dataFinal)(p7, bio)
#define BIO_write(b, data, len)										CallFunction(BIO_write)(b, data, len)
#define BIO_free_all(b)												CallFunction(BIO_free_all)(b)
#define i2d_PKCS7(p7, out)											CallFunction(i2d_PKCS7)(p7, out)
#define PKCS7_free(p7)												CallFunction(PKCS7_free)(p7)
#define BIO_new_mem_buf(buf, len)									CallFunction(BIO_new_mem_buf)(buf, len)
#define d2i_PKCS7_bio(biop, pp7)									CallFunction(d2i_PKCS7_bio)(biop, pp7)
#define PKCS7_get_signer_info(p7)									CallFunction(PKCS7_get_signer_info)(p7)
#define PKCS7_add_attribute(p7si, nid, atrtype,pvalue)				CallFunction(PKCS7_add_attribute)((p7si, nid, atrtype,pvalue)
#define PKCS7_get_attribute(si, nid)								CallFunction(PKCS7_get_attribute)(si, nid)
#define PKCS7_get_signed_attribute(si, nid)							CallFunction(PKCS7_get_signed_attribute)(si, nid)
#define PKCS7_set_signed_attributes(p7si,sk)						CallFunction(PKCS7_set_signed_attributes)(p7si,sk)
#define PKCS7_set_attributes(p7si,sk)								CallFunction(PKCS7_set_attributes)(p7si,sk)
#define PKCS7_sign(signcert, pkey, certs,data, flags)				CallFunction(PKCS7_sign)(signcert, pkey, certs,data, flags)
#define ASN1_STRING_length(const_ASN1_STRING_px)					CallFunction(ASN1_STRING_length)(const_ASN1_STRING_px)
#define ASN1_STRING_data(ASN1_STRING_px)							CallFunction(ASN1_STRING_data)(ASN1_STRING_px)
#define d2i_PKCS7(pkcs7, in, len)									CallFunction(d2i_PKCS7)(pkcs7, in, len)
#define PKCS7_dataDecode(p7, pkey, in_bio, pcert)					CallFunction(PKCS7_dataDecode)(p7, pkey, in_bio, pcert)
#define BIO_read(b, data, len)										CallFunction(BIO_read)(b, data, len)
#define d2i_TS_TST_INFO(TS_TST_INFO_a, ppdata,	nlength)			CallFunction(d2i_TS_TST_INFO)(TS_TST_INFO_a, ppdata, nlength)
#define SHA1(data, nsize, md)										CallFunction(SHA1)(data, nsize, md)
#define SHA256(data, nsize, md)										CallFunction(SHA256)(data, nsize, md)
#define PKCS7_cert_from_signer_info(p7, si)							CallFunction(PKCS7_cert_from_signer_info)(p7, si)
#define X509_get_serialNumber(x509)									CallFunction(X509_get_serialNumber)(x509)
#define X509_get_issuer_name(x509)									CallFunction(X509_get_issuer_name)(x509)
#define X509_get_subject_name(x509)									CallFunction(X509_get_subject_name)(x509)
#define X509_NAME_get_entry(name, loc)								CallFunction(X509_NAME_get_entry)(name,loc)
#define X509_NAME_get_text_by_NID(name, nid, buf, len)				CallFunction(X509_NAME_get_text_by_NID)(name, nid, buf, len)
#define PKCS7_verify(p7, certs, store, indata, out, flags)			CallFunction(PKCS7_verify)(p7, certs, store, indata, out, flags)
#define BIO_free(bio)												CallFunction(BIO_free)(bio)
#define SHA256_Update(SHA256_CTX_c, data, len)						CallFunction(SHA256_Update)(SHA256_CTX_c, data, len)
#define SHA256_Final(md, c)											CallFunction(SHA256_Final)(md, c)
#define CRYPTO_free(ptr)											CallFunction(CRYPTO_free)(ptr)
#define RAND_bytes(buf, num)										CallFunction(RAND_bytes)(buf, num)
#define CRYPTO_malloc(num, file, line)								CallFunction(CRYPTO_malloc)(num, file, line)
#define EVP_get_digestbyname(name)									CallFunction(EVP_get_digestbyname)(name)
#define TS_REQ_new()												CallFunction(TS_REQ_new)()
#define TS_REQ_free(TS_REQ_a)										CallFunction(TS_REQ_free)(TS_REQ_a)
#define i2d_TS_REQ(TS_REQ_a, pp)									CallFunction(i2d_TS_REQ)(TS_REQ_a, pp)
#define TS_REQ_set_version(TS_REQ_a, version)						CallFunction(TS_REQ_set_version)(TS_REQ_a, version)
#define TS_MSG_IMPRINT_free(TS_MSG_IMPRIN_a)						CallFunction(TS_MSG_IMPRINT_free)(TS_MSG_IMPRIN_a)
#define TS_MSG_IMPRINT_set_algo(TS_MSG_IMPRINT_a, X509_ALGOR_alg)	CallFunction(TS_MSG_IMPRINT_set_algo)(TS_MSG_IMPRINT_a, X509_ALGOR_alg)
#define TS_MSG_IMPRINT_set_msg(TS_MSG_IMPRINT_a, d, len)			CallFunction(TS_MSG_IMPRINT_set_msg)(TS_MSG_IMPRINT_a, d, len)
#define TS_REQ_set_msg_imprint(TS_REQ_a, msg_imprint)				CallFunction(TS_REQ_set_msg_imprint)(TS_REQ_a, msg_imprint)
#define TS_REQ_set_nonce(TS_REQ_a, nonce)							CallFunction(TS_REQ_set_nonce)(TS_REQ_a, nonce)
#define TS_REQ_set_cert_req(TS_REQ_a, cert_req)						CallFunction(TS_REQ_set_cert_req)(TS_REQ_a, cert_req)
#define d2i_TS_RESP(a, pp, length)									CallFunction(d2i_TS_RESP)(a, pp, length)
#define TS_RESP_free(TS_RESP_a)										CallFunction(TS_RESP_free)(TS_RESP_a)
#define ASN1_INTEGER_free(asn)										CallFunction(ASN1_INTEGER_free)(asn)
#define ASN1_item_new(item)											CallFunction(ASN1_item_new)(item)
#define TS_MSG_IMPRINT_new()										CallFunction(TS_MSG_IMPRINT_new)()
#define X509_ALGOR_new()											CallFunction(X509_ALGOR_new)()
#define OBJ_nid2obj(n)												CallFunction(OBJ_nid2obj)(n)
#define ASN1_TYPE_new()												CallFunction(ASN1_TYPE_new)()
#define X509_ALGOR_free(algo)										CallFunction(X509_ALGOR_free)(algo)
#define EVP_sha1()													CallFunction(EVP_sha1)()
#define EVP_sm3()													CallFunction(EVP_sm3)()
#define EVP_MD_type(md)												CallFunction(EVP_MD_type)(md)
#define ASN1_INTEGER_new()											CallFunction(ASN1_INTEGER_new)()
#define ERR_load_crypto_strings()									CallFunction(ERR_load_crypto_strings)()
#define SetApplinkFunc(ApplinkFun)									CallFunction(SetApplinkFunc)(ApplinkFun)


#define CallFunction(FunName) ((DLL_##FunName)(g_GmsslFunctions->m_Functions[FuncId_##FunName]))

#endif //_FS_OFDSIGNATUREGMSSLFUNC_H_

