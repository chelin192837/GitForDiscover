#ifndef _OFD_SECURITY_R_H_
#define _OFD_SECURITY_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _OFD_SECURITYHANDLER_
{
	/**
	 * @brief	用户定义数据。
	 *
	 * @note	调用者用于回调控制。
	 */
	void*	clientData;

	int	(*StartDecrypt)(void* clientData, int document, OFD_LPCSTR key, OFD_LPCSTR ownerKeyField, OFD_LPCSTR userKeyField);
	int	(*GetDecryptedSize)(void* clientData, int document, OFD_DWORD dataSize);
	int	(*DecryptData)(void* clientData, int document, OFD_LPBYTE encDataBuf, OFD_DWORD encDataLen, 
		OFD_LPBYTE decDataBuf, int* decDataLen);
	int	(*EndDecrypt)(void* clientData, int document);

	int	(*StartEncrypt)(void* clientData, int document);
	int	(*GetEncryptedSize)(void* clientData, int document, OFD_LPCBYTE srcBuf, OFD_DWORD srcLen);
	int	(*EncryptData)(void* clientData, int document, OFD_LPCBYTE srcBuf, OFD_DWORD srcLen, OFD_LPBYTE dstBuf);
	int	(*EndEncrypt)(void* clientData, int document);

} OFD_SECURITYHANDLER;

/**
 * @brief 注册安全对象。
 *
 * @param[in]	securityType			安全类型。
 * @param[in]	cryptoType				加密类型。
 * @param[in]	keyLen					key值长度。
 * @param[in]	handler					用户定义数据。
 * @return 成功返回安全对象句柄，失败返回NULL。
 *	
 */
OFD_SECURITYHANDLE OFD_Security_RegisterHandler(OFD_LPCSTR securityType, OFD_LPCSTR cryptoType, int keyLen, OFD_SECURITYHANDLER* handler);

/**
 * @brief 反注册安全对象。
 *
 * @param[in]	hSecurityHandler		安全对象句柄。
 
 * @return 无。
 *	
 */
void OFD_Security_UnregisterHandler(OFD_SECURITYHANDLE hSecurityHandler);

#ifdef __cplusplus
}
#endif

#endif  //#define _OFD_SECURITY_R_H_
