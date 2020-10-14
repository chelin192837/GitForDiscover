/*
 * Generated by asn1c-0.9.27 (http://lionet.info/asn1c)
 * From ASN.1 module "ElectronicStamp"
 * 	found in "htfoxitSign.asn1"
 * 	`asn1c -S skeletons -fskeletons-copy`
 */

#ifndef	_SESeal_H_
#define	_SESeal_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SES_SealInfo.h"
#include "SES_SignInfo.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SESeal */
typedef struct SESeal {
	SES_SealInfo_t	 esealInfo;
	SES_SignInfo_t	 signInfo;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SESeal_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SESeal;

#ifdef __cplusplus
}
#endif

#endif	/* _SESeal_H_ */
#include <asn_internal.h>
