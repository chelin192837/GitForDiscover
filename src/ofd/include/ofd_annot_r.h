#ifndef _OFD_ANNOT_R_H_
#define _OFD_ANNOT_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @brief 获得页面注释。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	index				文页面索引。	
 * @return 成功返回页面注释句柄，失败返回NULL。
 *				
 */
OFD_PAGEANNOTS		OFD_Document_GetPageAnnots(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 导出页面注释。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	start_page			起始文页面索引。	
 * @param[in]	end_page			结束文页面索引。	
 * @param[in]	filepath			注释文件保存位置。	
 * @return 成功0。
 *				
 */
int OFD_Document_ExportPageAnnots(OFD_DOCUMENT hDocument, int start_page, int end_page, OFD_LPCWSTR filepath);

/**
 * @brief 计算子注释个数。
 *
 * @param[in]	hPageAnnots	页面注释句柄。		
 * @return 成功返回注释个数。
 *				
 */
int		OFD_PageAnnots_Count(OFD_PAGEANNOTS hPageAnnots);

/**
 * @brief 获得注释。
 *
 * @param[in]	hPageAnnots	页面注释句柄。		
 * @param[in]	index		注释索引。		
 * @return 成功返回子节点。
 *				
 */
OFD_ANNOT		OFD_PageAnnots_Get(OFD_PAGEANNOTS hPageAnnots, int index);

/**
 * @brief 获得注释ID。
 *
 * @param[in] hAnnot		注释句柄。
 * @return ID。
 *				
 */
int		OFD_Annot_GetID(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释索引。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 索引。
 *				
 */
int		OFD_Annot_GetIndex(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释创建者。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[out] data			创建者。
 * @return 无。
 *				
 */
void		OFD_Annot_GetCreator(OFD_ANNOT hAnnot, OFD_WSTR* data);

/**
 * @brief 获得注释最近一次修改的时间。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[out] year			年。
 * @param[out] month		月。
 * @param[out] day			日。
 * @param[out] hour			时。
 * @param[out] minute		分。
 * @param[out] second		秒。
 * @param[out] millisecond	毫秒。
 * @return 无。
 *				
 */
void		OFD_Annot_GetLastModDate(OFD_ANNOT hAnnot, int* year, OFD_BYTE* month, OFD_BYTE* day, 
	OFD_BYTE* hour, OFD_BYTE* minute, OFD_BYTE* second, OFD_WORD* millisecond);

/**
 * @brief 获得注释是否显示。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 是否显示。
 *				
 */
OFD_BOOL		OFD_Annot_GetInvisible(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释是否随页面一起打印。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 是否随页面一起打印。
 *				
 */
OFD_BOOL		OFD_Annot_GetPrintable(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释的Remark信息是否随页面缩放而同步缩放。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 是否随页面缩放而同步缩放。
 *				
 */
OFD_BOOL		OFD_Annot_GetNoZoom(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释的Remark信息是否随页面旋转而同步旋转。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 是否随页面旋转而同步旋转。
 *				
 */
OFD_BOOL		OFD_Annot_GetNoRotate(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释的Remark信息是否不能被用户更改。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 是否不能被用户更改。
 *				
 */
OFD_BOOL		OFD_Annot_GetReadOnly(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释说明内容。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[out] data			说明内容。
 * @return 无。
 *				
 */
void	OFD_Annot_GetRemark(OFD_ANNOT hAnnot, OFD_WSTR* data);

/**
 * @brief 获得注释矩形外接框。
 *
 * @param[in] hAnnot		注释句柄。
 * @return 矩形外接框值。
 *				
 */
OFD_RECT	OFD_Annot_GetBoundary(OFD_ANNOT hAnnot);

/**
 * @brief 获得注释子类型。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[out] data			子类型。
 * @return 无。
 *				
 */
void	OFD_Annot_GetSubType(OFD_ANNOT hAnnot, OFD_WSTR* data);

/**
 * @brief 获取注释外观流中图元对象的个数。
 *
 * @param[in] hAnnot		注释句柄。	
 * @return 成功返回图元对象的个数，失败返回-1。
 *				
 */
int		OFD_Annot_CountAppearanceObj(OFD_ANNOT hAnnot);

/**
 * @brief 获取注释外观流中图元对象。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] index			指定的图元对象索引号（由0开始编号）。
 * @return 成功返回图元对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Annot_GetAppearanceObj(OFD_ANNOT hAnnot, int index);

/**
 * @brief 获得Link注释动作类型。
 *
 * @param[in]	hAnnot		注释句柄。
 * @return 返回Link注释动作类型。
 *				
 */
int		OFD_Annot_LinkGetActionType(OFD_ANNOT hAnnot);

/**
 * @brief 获得Link注释跳转位置。
 *
 * @param[in]	hAnnot			注释句柄。
 * @param[out]	iPage			返回跳转位置页号（由0开始编号）。
 * @param[out]	destPoint		返回跳转目标位置。
 * @return 无。
 *				
 */
void		OFD_Annot_LinkGetDest(OFD_ANNOT hAnnot, int* iPage, OFD_POINT* destPoint);

/**
 * @brief 获得Link注释URI地址。
 *
 * @param[in]	hAnnot			注释句柄。
 * @param[in]	data			返回URI地址。	
 * @return 无。
 *				
 */
void		OFD_Annot_LinkGetURI(OFD_ANNOT hAnnot, OFD_WSTR* data);

/**
 * @brief 获取注释中动作数组。
 *
 * @param[in] hAnnot		注释句柄。
 * @param[in] index			指定的动作对象索引号（由0开始编号）。
 * @return 成功返回动作对象的句柄，失败返回NULL。
 *
 */
OFD_ACTIONS			OFD_Annot_GetActions(OFD_ANNOT hAnnot,int index);

/**
 * @brief 导出文档注释。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	filepath			注释文件保存位置。	
 * @return 成功0。
 *				
 */
int OFD_Document_ExportAnnots(OFD_DOCUMENT hDocument, OFD_LPCWSTR filepath);

#ifdef __cplusplus
}
#endif

#endif //_OFD_ANNOT_R_H_
