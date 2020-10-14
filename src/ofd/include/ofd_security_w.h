#ifndef _OFD_SECURITY_W_H_
#define _OFD_SECURITY_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 自定义加密。
 *
 * @param[in]	hDocument			文档句柄。
 * @param[in]	handle				安全对象句柄句柄。
 * @param[in]	ownerKeyField		所有者密码。
 * @param[in]	userKeyField		用户密码。
 * @return 成功返回0，失败返回-1。
 *	
 */
int	OFD_Security_CustomEncrypt(OFD_DOCUMENT hDocument, OFD_SECURITYHANDLE handle, 
	OFD_LPCSTR ownerKeyField, OFD_LPCSTR userKeyField);

/**
 * @brief 删除自定义文档密码。
 *
 * @param[in]	hDocument			文档句柄。
 * @param[in]	ownerKeyField		所有者密码。
 * @return 成功返回0，失败返回-1。
 *	
 */
int	OFD_Security_RemoveCustomEncryption(OFD_DOCUMENT hDocument, OFD_LPCSTR ownerKeyField);


#ifdef __cplusplus
}
#endif

#endif  //#define _OFD_SECURITY_W_H_
