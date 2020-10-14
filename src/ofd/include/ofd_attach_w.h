#ifndef _OFD_ATTACHMENT_W_H_
#define _OFD_ATTACHMENT_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建附件数组。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回附件数组句柄，失败返回NULL。
 *				
 */
OFD_ATTACHMENTS		OFD_Document_CreateAttachments(OFD_DOCUMENT hDocument);

/**
 * @brief 添加附件。
 *
 * @param[in]	hAttachments			附件数组句柄。	
 * @return 成功返回附件句柄，失败返回NULL。
 *				
 */
OFD_ATTACHMENT		OFD_Attachments_Add(OFD_ATTACHMENTS hAttachments);

/**
 * @brief 删除附件。
 *
 * @param[in]	hAttachments			附件数组句柄。	
 * @param[in]	index					附件序号。	
 * @return 成功返回TRUE失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Attachments_Delete(OFD_ATTACHMENTS hAttachments,int index);

/**
 * @brief 设置附件文件。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	lpwszFileName			附件文件。
 * @param[in]	lpwszName				附件名称。
 * @return 无。
 *				
 */
void		OFD_Attachment_SetFile(OFD_ATTACHMENT hAttachment, OFD_LPCWSTR lpwszFileName, OFD_LPCWSTR lpwszName = NULL);

/**
 * @brief 设置附件文件。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	lpbData					附件文件数据。	
 * @param[in]	iLen					附件文件长度。	
 * @param[in]	bTakeover				是否由内部释放。
 * @param[in]	lpwszName				附件名称。
 * @return 无。
 *				
 */
void		OFD_Attachment_SetFileFromBuf(OFD_ATTACHMENT hAttachment, OFD_LPBYTE lpbData, int iLen, OFD_BOOL bTakeover, OFD_LPCWSTR lpwszName = NULL);

/**
 * @brief 设置附件名称。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	lpwszName				名称。	
 * @return 无。
 *				
 */
void		OFD_Attachment_SetName(OFD_ATTACHMENT hAttachment, OFD_LPCWSTR lpwszName);

/**
 * @brief 设置附件格式。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	lpwszFormat				格式。	
 * @return 无。
 *				
 */
void		OFD_Attachment_SetFormat(OFD_ATTACHMENT hAttachment, OFD_LPCWSTR lpwszFormat);

/**
 * @brief 设置附件创建日期。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	lpwszCreationDate		创建日期。	
 * @return 无。
 *				
 */
void		OFD_Attachment_SetCreationDate(OFD_ATTACHMENT hAttachment, OFD_LPCWSTR lpwszCreationDate);

/**
 * @brief 设置附件修改日期。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	lpwszCreationDate		修改日期。	
 * @return 无。
 *				
 */
void		OFD_Attachment_SetModDate(OFD_ATTACHMENT hAttachment, OFD_LPCWSTR lpwszModDate);

/**
 * @brief 设置附件用途。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	lpwszUsage				用途。	
 * @return 无。
 *				
 */
void		OFD_Attachment_SetUsage(OFD_ATTACHMENT hAttachment, OFD_LPCWSTR lpwszUsage);

/**
 * @brief 设置附件可见性。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	bVisible				可见性。	
 * @return 无。
 *				
 */
void		OFD_Attachment_SetVisible(OFD_ATTACHMENT hAttachment, OFD_BOOL bVisible);


#ifdef __cplusplus
}
#endif

#endif //_OFD_ATTACHMENT_W_H_
