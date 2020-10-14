#ifndef _OFD_DOCUMENT_W_H_
#define _OFD_DOCUMENT_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 将源文档合并到目标文档中。
 *
 * @param[in]	hDst		目标文档句柄。	
 * @param[in]	hSrc		源文档句柄。
 * @param[in]	index		源文档索引。
 * @param[in]	insertTo	插入位置。（目标文档的页索引号）
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_Merge(OFD_DOCUMENT hDst, OFD_DOCUMENT hSrc,int index, int insertTo);

/**
 * @brief 创建文档目录。
 *
 * @param[in]	hDst		目标文档句柄。	
 * @param[in]	hSrc		源文档句柄。
 * @param[in]	index		源文档索引。
 * @param[in]	insertTo	插入位置。（目标文档的页索引号）
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_CATALOG		OFD_Catalog_Create();

/**
 * @brief 添加目录信息。（按顺序添加）
 *
 * @param[in]	hCatalog		目录句柄。
 * @param[in]	lpwszTitle		文档标题。（用于生成目录）	
 * @param[in]	pagecnt			文档页数。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Catalog_Add(OFD_CATALOG hCatalog, OFD_LPCWSTR lpwszTitle, int pageCnt);

/**
 * @brief 添加附件信息。（按顺序添加）
 *
 * @param[in]	hCatalog		目录句柄。
 * @param[in]	hAttachs		附件句柄。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Catalog_AddAttachment(OFD_CATALOG hCatalog, OFD_ATTACHMENTS hAttachs);

/**
 * @brief 设置页边距。
 *
 * @param[in]	hCatalog		目录句柄。
 * @param[in]	leftMargin		页面水平边距。
 * @param[in]	upperMargin		页面垂直边距。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Catalog_SetMargin(OFD_CATALOG hCatalog, float leftMargin, float upperMargin);

/**
 * @brief 添加目录信息。（按顺序添加）
 *
 * @param[in]	hCatalog		目录句柄。
 * @param[in]	section			目录区域。取值OFD_CATALOG_TITLE_SECTION,OFD_CATALOG_CONTENT_SECTION,
 OFD_ATTACHMENT_TITLE_SECTION,OFD_ATTACHMENT_CONTENT_SECTION之一
 * @param[in]	fontName		字体名称。
 * @param[in]	fontSize		字体大小。
 * @param[in]	lowerSpace		与下一个条目之间的间距。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Catalog_SetFormat(OFD_CATALOG hCatalog, int section, OFD_LPCWSTR fontName, float fontSize, float lowerSpace);

/**
 * @brief 释放。
 *
 * @param[in]	hCatalog		目录句柄。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
void		OFD_Catalog_Release(OFD_CATALOG hCatalog);

/**
 * @brief 添加目录信息。
 *
 * @param[in]	hDst			目标文档句柄。
 * @param[in]	hCatalog		目录句柄。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_AddCatalogs(OFD_DOCUMENT hDocument, OFD_CATALOG hCatalog);

/**
 * @brief 添加附件信息。
 *
 * @param[in]	hDst			目标文档句柄。
 * @param[in]	hCatalog		目录句柄。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_AddAttachments(OFD_DOCUMENT hDocument, OFD_CATALOG hCatalog);

/**
 * @brief 根据目录信息生成大纲。
 *
 * @param[in]	hDst			目标文档句柄。
 * @param[in]	hCatalog		目录句柄。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_AddOutlines(OFD_DOCUMENT hDocument, OFD_CATALOG hCatalog);

/**
 * @brief 新建一页，并追加至文档中。
 *
 * @param[in]	hDocument		文档句柄。	
 * @return 成功返回页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_AddPage(OFD_DOCUMENT hDocument);

/**
 * @brief 新建一页，并插入文档中的指定位置。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			页序号（由0开始编号）。	
 * @return 成功返回页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_InsertPage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 删除文档中的一页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			页序号（由0开始编号）。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_DeletePage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 移动文档中的一页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	iSrcIndex		移动前页序号（由0开始编号）。
 * @param[in]	iDstIndex		移动后页序号（由0开始编号）。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_MovePage(OFD_DOCUMENT hDocument, int iSrcIndex, int iDstIndex);

/**
 * @brief 拆分文档中的一页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			页序号（由0开始编号）。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_SplitPage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 新建一个模板页，并追加至指定文档的模板库中。
 *
 * @param[in]	hDocument		文档句柄。	
 * @return 成功返回模板页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_AddTemplatePage(OFD_DOCUMENT hDocument);

/**
 * @brief 将普通页转换成一个模板页，并追加至指定文档的模板库中。
 *
 * @param[in]	hDocument		文档句柄。
 * @param[in]	hSrcPage		普通页的页句柄。
 * @return 成功返回模板页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_AddTemplatePageFromPage(OFD_DOCUMENT hDocument, OFD_PAGE hSrcPage);

/**
 * @brief 新建一个模板页，并插入到文档中模板库中指定的位置。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			模板页序号（由0开始编号）。	
 * @return 成功返回模板页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_InsertTemplatePage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 将普通页转换成一个模板页，并插入到文档中模板库中指定的位置。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			模板页序号（由0开始编号）。	
 * @param[in]	hSrcPage		普通页的页句柄。
 * @return 成功返回模板页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_InsertTemplatePageFromPage(OFD_DOCUMENT hDocument, int index, OFD_PAGE hSrcPage);


/**
 * @brief 删除指定的模板页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			模板页序号（由0开始编号）。	
 * @return 无。
 *				
 */
void		OFD_Document_RemoveTemplatePage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 删除所有模板页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @return 无。
 *				
 */
void		OFD_Document_RemoveAllTemplatePage(OFD_DOCUMENT hDocument);

/**
 * @brief 设置元数据。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	lpwszMetadataName	元数据名称(OFD_DOCUMENT_METADATA_XXX)。
 * @param[in]	data				元数据。	
 * @return 无。
 *				
 */
void		OFD_Document_SetMetadata(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszMetadataName, OFD_LPCWSTR data);

/**
 * @brief 设置元数据关键字。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	lpwszKeyWord		元数据关键字。
 * @param[in]	index				元数据关键字索引。	
 * @return 成功返回TRUE，失败返回 -1。
 *				
 */
int			OFD_Document_SetKeyWord(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszKeyWord, int index = -1);

/**
 * @brief 删除指定元数据关键字。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	index				元数据关键字索引。
 * @return 无。
 *				
 */
void		OFD_Document_RemoveKeyWord(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 设置自定义元数据。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	lpwszDataName		自定义元数据名字。
 * @param[in]	lpwszDataValue		自定义元数据值。
 * @param[in]	index				元数据关键字索引。	
 * @return 成功返回TRUE，失败返回 -1。
 *				
 */
int			OFD_Document_SetCustomData(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszDataName, OFD_LPCWSTR lpwszDataValue, int index = -1);

/**
 * @brief 删除自定义元数据。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	index				自定义元数据索引。
 * @return 无。
 *				
 */
void		OFD_Document_RemoveCustomData(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 加密文档。
 *
 * @param[in]	hDocument		文档句柄。
 * @param[in]	encryptType		加密算法类型。
 * @param[in]	userPassword	使用者密码。
 * @param[in]	ownerPassword	拥有者密码。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_SetPassword(OFD_DOCUMENT hDocument, int encryptType, OFD_LPCSTR userPassword, OFD_LPCSTR ownerPassword);

/**
 * @brief 加密文档（证书加密）。
 *
 * @param[in]	hDocument		文档句柄。
 * @param[in]	encryptType		加密算法类型。
 * @param[in]	certPath		证书。
 * @param[in]	password		证书密码。
  * @param[in]	certType		0:FX_EVP_AES_CBC，1：FX_EVP_SM4_CBC（国密证书）。
 * @return 成功返回TRUE，失败返回FALSE。
 *	
 */
OFD_BOOL OFD_Document_CertEncrypt(OFD_DOCUMENT hDocument, int encryptType, OFD_LPCWSTR certPath, OFD_LPCSTR password, int certType = 0);

/**
 * @brief 移除密码。
 *
 * @param[in]	hDocument		文档句柄。
 * @param[in]	userPassword	使用者密码。
 * @param[in]	ownerPassword	拥有者密码。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_RemovePassword(OFD_DOCUMENT hDocument, OFD_LPCSTR ownerPassword);

/**
 * @brief 向文档中添加图像资源。
 *
 * @param[in]	hDocument		文档句柄。
 * @param[in]	lpwszFilename	指定的图像文件名称。
 * @return 图像资源ID。
 *				
 */
OFD_DWORD OFD_Document_AddImageResFromFile(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszFilename);

/**
 * @brief 向文档中添加图像资源。
 *
 * @param[in]	hDocument		文档句柄。
 * @param[in]	lpbDataBuf		图像数据指针。
 * @param[in]	iLen			图像数据长度。
 * @param[in]	imageFormat		图像格式（取值范围为OFD_IMAGE_FORMAT_BMP，OFD_IMAGE_FORMAT_JPG，OFD_IMAGE_FORMAT_PNG，OFD_IMAGE_FORMAT_TIF）
 * @return 图像资源ID。
 *				
 */
OFD_DWORD OFD_Document_AddImageResFromBuf(OFD_DOCUMENT hDocument, OFD_LPBYTE lpbDataBuf, int iLen, int imageFormat);

#ifdef __cplusplus
}
#endif

#endif  //_OFD_DOCUMENT_W_H_
