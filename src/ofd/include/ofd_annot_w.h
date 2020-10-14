#ifndef _OFD_ANNOT_W_H_
#define _OFD_ANNOT_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建页面注释。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 返回页面注释句柄。
 *				
 */
OFD_PAGEANNOTS		OFD_Document_CreatePageAnnots(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 导入页面注释。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	filepath			注释文件位置。	
 * @return 成功0。
 *				
 */
int OFD_Document_ImportPageAnnots(OFD_DOCUMENT hDocument, OFD_LPCWSTR filepath);

/**
 * @brief 创建注释并添加到页面注释中。
 *
 * @param[in] hPageAnnots	页面注释句柄。
 * @param[in] eType			注释类型。
 * @return 创建的注释。
 *				
 */
OFD_ANNOT	OFD_PageAnnots_Add(OFD_PAGEANNOTS hPageAnnots, int eType);

/**
 * @brief 页面已有注释移动到新的位置。
 *
 * @param[in] hPageAnnots	页面注释句柄。
 * @param[in] nIndex		页面注释索引。
 * @return 移动的注释。
 *				
 */
OFD_ANNOT	OFD_PageAnnots_Move(OFD_PAGEANNOTS hPageAnnots, int nIndex);

/**
 * @brief 删除页面注释。
 *
 * @param[in]	pageAnnots			页面注释句柄。	
 * @param[in]	index				索引（从0开始）
 * @return 成功返回true,失败返回false。
 *				
 */
OFD_BOOL OFD_PageAnnots_RemoveAnnot(OFD_PAGEANNOTS hPageAnnots,int index);

/**
 * @brief 设置注释创建者。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] lpwszCreator	创建者。
 * @return 无。
 *				
 */
void	OFD_Annot_SetCreator(OFD_ANNOT hAnnot, OFD_LPCWSTR lpwszCreator);

/**
 * @brief 设置注释最近一次修改的时间。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] year			年。
 * @param[in] month			月。
 * @param[in] day			日。
 * @param[in] hour			时。
 * @param[in] minute		分。
 * @param[in] second		秒。
 * @param[in] millisecond	毫秒。
 * @return 无。
 *				
 */
void	OFD_Annot_SetLastModDate(OFD_ANNOT hAnnot, int year, OFD_BYTE month, OFD_BYTE day, OFD_BYTE hour, OFD_BYTE minute, OFD_BYTE second, OFD_WORD millisecond);

/**
 * @brief 设置注释是否显示。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] bSet			设置值。
 * @return 无。
 *				
 */
void	OFD_Annot_SetInvisible(OFD_ANNOT hAnnot, OFD_BOOL bSet);

/**
 * @brief 设置注释是否随页面一起打印。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] bSet			设置值。
 * @return 无。
 *				
 */
void	OFD_Annot_SetPrintable(OFD_ANNOT hAnnot, OFD_BOOL bSet);

/**
 * @brief 设置注释的Remark信息是否随页面缩放而同步缩放。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] bSet			设置值。
 * @return 无。
 *				
 */
void	OFD_Annot_SetNoZoom(OFD_ANNOT hAnnot, OFD_BOOL bSet);

/**
 * @brief 设置注释的Remark信息是否随页面旋转而同步旋转。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] bSet			设置值。
 * @return 无。
 *				
 */
void	OFD_Annot_SetNoRotate(OFD_ANNOT hAnnot, OFD_BOOL bSet);

/**
 * @brief 设置注释的Remark信息是否不能被用户更改。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] bSet			设置值。
 * @return 无。
 *				
 */
void	OFD_Annot_SetReadOnly(OFD_ANNOT hAnnot, OFD_BOOL bSet);

/**
 * @brief 设置注释说明内容。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] boundary		说明内容。
 * @return 无。
 *				
 */
void	OFD_Annot_SetRemark(OFD_ANNOT hAnnot, OFD_LPCWSTR lpwszRemark);

/**
 * @brief 设置注释矩形外接框。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] boundary		矩形外接框值。
 * @return 无。
 *				
 */
void	OFD_Annot_SetBoundary(OFD_ANNOT hAnnot, OFD_RECT boundary);

/**
 * @brief 设置注释子类型。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] lpwszSubType	子类型。
 * @return 无。
 *				
 */
void	OFD_Annot_SetSubType(OFD_ANNOT hAnnot, OFD_LPCWSTR lpwszSubType);

/**
 * @brief 新建一个path对象，并追加至注释外观流中。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] pageObjType	图元对象类型，OFD_PAGEOBJECT_TYPE_XXX
 * @return 成功返回图元对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Annot_AddAppearanceObj(OFD_ANNOT hAnnot, int pageObjType);

/**
 * @brief 新建一个BLOCK对象，并追加至注释外观流中。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 成功返回块对象的句柄，失败返回NULL。
 *				
 */
OFD_BLOCKOBJECT		OFD_Annot_AddAppearanceBlock(OFD_ANNOT hAnnot);

/**
 * @brief 设置Link注释动作，跳转位置。
 *
 * @param[in]	hAnnot		注释句柄。
 * @param[in]	iPage		跳转位置页号（由0开始编号）。
 * @param[in]	destPoint	跳转目标位置。
 * @return 无。
 *				
 */
void	OFD_Annot_LinkSetDest(OFD_ANNOT hAnnot, int iPage, OFD_POINT destPoint);

/**
 * @brief 设置Link注释动作，跳转URI地址。
 *
 * @param[in]	hAnnot		注释句柄。
 * @param[in]	lpwszUri		跳转位置。
 * @return 无。
 *				
 */
void	OFD_Annot_LinkSetURI(OFD_ANNOT hAnnot, OFD_LPCWSTR lpwszUri);

/**
 * @brief 创建一个注释动作数组。
 *
 * @param[in]	hAnnot		注释句柄。
 * @return 无。
 *				
 */
OFD_ACTIONS		OFD_Annot_CreateActions(OFD_ANNOT hAnnot);


/**
 * @brief 导入文档注释。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	filepath			注释文件保存位置。	
 * @return 成功0。
 *				
 */
int OFD_Document_ImportAnnots(OFD_DOCUMENT hDocument, OFD_LPCWSTR filepath);

#ifdef __cplusplus
}
#endif

#endif //_OFD_ANNOT_W_H_
