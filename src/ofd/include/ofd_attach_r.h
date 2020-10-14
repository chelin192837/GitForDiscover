#ifndef _OFD_ATTACHMENT_R_H_
#define _OFD_ATTACHMENT_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获得附件数组。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回附件数组句柄，失败返回NULL。
 *				
 */
OFD_ATTACHMENTS		OFD_Document_GetAttachments(OFD_DOCUMENT hDocument);

/**
 * @brief  获取附件个数。
 *
 * @param[in]	hAttachments			附件数组句柄。	
 * @return 成功附件个数。
 *				
 */
int		OFD_Attachments_Count(OFD_ATTACHMENTS hAttachments);

/**
 * @brief 获取附件。
 *
 * @param[in]	hAttachments			附件数组句柄。	
 * @param[in]	index					附件索引。	
 * @return 附件句柄。
 *				
 */
OFD_ATTACHMENT		OFD_Attachments_Get(OFD_ATTACHMENTS hAttachments, int index);

/**
 * @brief 导出附件。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[out]	lpszFile				保存文件名。
 * @return 附件句柄。
 *				
 */
void		OFD_Attachment_ExtractFile(OFD_ATTACHMENT hAttachment, OFD_LPCWSTR lpszFile);

/**
 * @brief 导出附件。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[out]	pBuf					附件流。
 * @param[out]	size					附件大小。
 * @return 附件句柄。
 *				
 */
void		OFD_Attachment_ExtractData(OFD_ATTACHMENT hAttachment, OFD_BYTE* pBuf, int* size);

/**
 * @brief 获得附件名称。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[out]	data					返回附件名称。	
 * @return 无。
 *				
 */
void		OFD_Attachment_GetName(OFD_ATTACHMENT hAttachment, OFD_WSTR* data);

/**
 * @brief 获得附件格式。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[out]	data					返回附件格式。	
 * @return 无。
 *				
 */
void		OFD_Attachment_GetFormat(OFD_ATTACHMENT hAttachment, OFD_WSTR* data);

/**
 * @brief 获得附件创建日期。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[out]	data					获得附件创建日期。	
 * @return 无。
 *				
 */
void		OFD_Attachment_GetCreationDate(OFD_ATTACHMENT hAttachment, OFD_WSTR* data);

/**
 * @brief 获得附件修改日期。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[out]	data					获得附件修改日期。	
 * @return 无。
 *				
 */
void		OFD_Attachment_GetModDate(OFD_ATTACHMENT hAttachment, OFD_WSTR* data);

/**
 * @brief 获得附件用途。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @param[in]	data					获得附件用途。	
 * @return 无。
 *				
 */
void		OFD_Attachment_GetUsage(OFD_ATTACHMENT hAttachment, OFD_WSTR* data);

/**
 * @brief 获得附件可见性。
 *
 * @param[in]	hAttachment				附件句柄。	
 * @return 返回附件可见性。
 *				
 */
OFD_BOOL		OFD_Attachment_GetVisible(OFD_ATTACHMENT hAttachment);

#ifdef __cplusplus
}
#endif

#endif //_OFD_ATTACHMENT_R_H_
