#ifndef _OFD_PAGE_R_H_
#define _OFD_PAGE_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 关闭页。
 *
 * @param[in] hPage	页句柄。		
 * @return 无。
 *				
 */
void		OFD_Page_Close(OFD_PAGE hPage);

/**
 * @brief 获取页编号。
 *
 * @param[in] hPage	页句柄。		
 * @return 返回页的编号（由0开始编号）。
 *				
 */
int		OFD_Page_GetIndex(OFD_PAGE hPage);

/**
 * @brief 获取页面区域。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] iBoxType	页面区域类型（值应为OFD_PAGE_BOXTYPE_PHYSICAL或OFD_PAGE_BOXTYPE_APPLICATION或OFD_PAGE_BOXTYPE_CONTENT或OFD_PAGE_BOXTYPE_BLEED）。
 * @return 成功返回页面区域，失败返回无效区域。
 *				
 */
OFD_RECT		OFD_Page_GetPageArea(OFD_PAGE hPage, int iBoxType);

/**
 * @brief	计算页面内容区域外框.
 * 
 * @param[in] hPage		页句柄。	
 *
 * @return 成功返回页面区域，失败返回无效区域。
*/
OFD_RECT		OFD_Page_CalcContentBBox(OFD_PAGE hPage);

/**
 * @brief 获取页面动作数组。
 *
 * @param[in] hPage		页句柄。	
 * @return 成功返回页面动作数组，失败返回NULL。
 *				
 */
OFD_ACTIONS		OFD_Page_GetActions(OFD_PAGE hPage);

/**
 * @brief 获取页中模板页数。
 *
 * @param[in] hPage		页句柄。	
 * @return 成功返回页模板页的数量，失败返回-1。
 *				
 */
int		OFD_Page_CountTemplatePages(OFD_PAGE hPage);

/**
 * @brief 获取页中层数。
 *
 * @param[in] hPage		页句柄。	
 * @return 成功返回页中层的数量，失败返回-1。
 *				
 */
int		OFD_Page_CountLayer(OFD_PAGE hPage);

/**
 * @brief 获取层。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] index		指定层的索引（由0开始编号）。	
 * @return 成功返回层的句柄，失败返回NULL。
 *				
 */
OFD_LAYER		OFD_Page_GetLayer(OFD_PAGE hPage, int index);

/**
 * @brief 获取页ID。
 *
 * @param[in] hPage		页句柄。
 * @return 成功返回页ID，失败返回-1。
 *				
 */
int		OFD_Page_GetID(OFD_PAGE hPage);

/**
 * @brief 计算渲染矩阵。
 *
 * @param[in] hPage				页句柄。
 * @param[in] deviceRect		设备矩形。
 * @param[in] iRotate			旋转。
 * @return 成功返回页ID，失败返回-1。
 *				
 */
OFD_MATRIX		OFD_Page_GetMatrix(OFD_PAGE hPage, OFD_RECT deviceRect, int iRotate);

/**
 * @brief 获取层中图元对象的个数。
 *
 * @param[in] hLayer		层句柄。	
 * @return 成功返回图元对象的个数，失败返回-1。
 *				
 */
int		OFD_Layer_CountPageObject(OFD_LAYER hLayer);

/**
 * @brief 获取图元对象。
 *
 * @param[in] hLayer	层句柄。
 * @param[in] index		指定的图元对象索引号（由0开始编号）。
 * @return 成功返回图元对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Layer_GetPageObject(OFD_LAYER hLayer, int index);

/**
 * @brief 通过名称获取该图元对象的索引。
 *
 * @param[in] hLayer	层句柄。
 * @param[in] lpwszName		图元对象名称。
 * @return 成功返回图元对象索引号，失败返回-1。
 *				
 */
int		OFD_Layer_GetPageObjectIndexByName(OFD_LAYER hLayer, OFD_LPCWSTR lpwszName);

/**
 * @brief 获取图元对象所在页面的索引。
 *
 * @param[in] hPageObject 图元对象句柄。
 * @return 成功返回页面索引
 *    
 */
int		OFD_PageObject_GetPageIndex(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象类型。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 成功返回图元对象类型（OFD_PAGEOBJECT_TYPE_IMAGE，OFD_PAGEOBJECT_TYPE_TEXT，OFD_PAGEOBJECT_TYPE_PATH等）
 *		  失败返回-1。
 *				
 */
int		OFD_PageObject_GetType(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象名称。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[out] name			返回图元对象名称。
 * @return 无。
 *				
 */
void		OFD_PageObject_GetName(OFD_PAGEOBJECT hPageObject, OFD_WSTR* name);

/**
 * @brief 获取对象ID。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 对象ID。
 *				
 */
OFD_DWORD		OFD_PageObject_GetID(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 判断图元对象是否可见。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 可见返回TRUE，不可见返回FALSE。
 *				
 */
OFD_BOOL		OFD_PageObject_IsVisible(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象线宽。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回线宽（单位毫米）。
 *				
 */
float		OFD_PageObject_GetLineWidth(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象线帽样式。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回线帽样式。
 *				
 */
int		OFD_PageObject_GetLineCap(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象结合点样式。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象结合点样式。
 *				
 */
int		OFD_PageObject_GetLineJoin(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象斜切限制值。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象斜切限制值。
 *				
 */
float		OFD_PageObject_GetMiterLimit(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象虚线重复起始值。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象虚线重复起始值。
 *				
 */
float		OFD_PageObject_GetDashOffset(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象虚线重复样式（两次调用第一次获取数据长度，第二次获取数据）。
 *
 * @param[in] hPageObject		图元对象句柄。
 * @param[out] fpDashPatterns	用于存储重复样式点数据。
 * @param[out] iCount			用于存储重复样式点数据长度（当dashPatterns为NULL时）。
 * @return 无。
 *				
 */
void		OFD_PageObject_GetDashPattern(OFD_PAGEOBJECT hPageObject, float *fpDashPatterns, int *iCount);

/**
 * @brief 获取图元对象alpha属性。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象alpha属相（0-255）。
 *				
 */
OFD_BYTE		OFD_PageObject_GetAlpha(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象填充颜色。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象填充颜色。
 *				
 */
OFD_ARGB		OFD_PageObject_GetFillColor(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象勾边颜色。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象勾边颜色。
 *				
 */
OFD_ARGB		OFD_PageObject_GetStrokeColor(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象的外接矩形。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象的外接矩形。
 *				
 */
OFD_RECT		OFD_PageObject_GetBoundary(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象CTM矩阵变换值。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象的矩阵变换值。
 *				
 */
OFD_MATRIX		OFD_PageObject_GetCTM(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图元对象动作数组句柄。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象动作数组句柄。
 *				
 */
OFD_ACTIONS		OFD_PageObject_GetActions(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 获取图像对象格式。
 *
 * @param[in] hImageObject	图像对象句柄。
 * @return 返回图像对象的格式。
 *				
 */
int		OFD_ImageObject_GetImageFormat(OFD_PAGEOBJECT hImageObject);

/**
 * @brief 获取图像对象中图像数据(两次调用，第一次调用获取数据长度，第二次调用获取数据值)。
 *
 * @param[in] hImageObject	图像对象句柄。
 * @param[out] lpbData		用于存储图像对象数据。
 * @param[out] piLen		用于存储图像对象数据长度。
 * @return 无。
 *				
 */
void		OFD_ImageObject_GetImageData(OFD_PAGEOBJECT hImageObject, OFD_LPBYTE lpbData, int *piLen);

/**
 * @brief 获取路径对象填充模式。
 *
 * @param[in] hPathObject	路径对象句柄。
 * @return 返回路径对象填充模式。
 *				
 */
int		OFD_PathObject_GetFillMode(OFD_PAGEOBJECT hPathObject);

/**
 * @brief 获取路径对象中路径元素描述序列（两次调用，第一次获取数据长度，第二次获取数据）。
 *
 * @param[in] hPathObject		路径对象句柄。
 * @param[out] lpszAbbreviteData	用于存储点列值数据。
 * @param[out] piLen				用于存储点列值数据长度。
 * @return 无。
 *				
 */
void		OFD_PathObject_GetAbbreviteData(OFD_PAGEOBJECT hPathObject, OFD_LPSTR lpszAbbreviteData, int *piLen);

/**
 * @brief 获取图形数据对象。
 *
 * @param[in] hPathObject		图形对象句柄。
 * @return 图像数据对象句柄。
 *				
 */
OFD_PATH		OFD_PathObject_GetPath(OFD_PAGEOBJECT hPathObject);

/**
 * @brief 获取文字对象的字体名称。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[out] fontName		返回字体名称。
 * @return 成功返回字体名称，失败返回NULL。
 *				
 */
void		OFD_TextObject_GetFontName(OFD_PAGEOBJECT hTextObject, OFD_WSTR* fontName);

/**
 * @brief 获取文字对象的字体大小。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @return 返回文字对象的字体大小（单位毫米）。
 *				
 */
float		OFD_TextObject_GetFontSize(OFD_PAGEOBJECT hTextObject);

/**
 * @brief 获取文字对象的水平缩放值。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @return 返回文字对象的水平缩放值。
 *				
 */
float		OFD_TextObject_GetHScale(OFD_PAGEOBJECT hTextObject);

/**
 * @brief 获取文字对象的阅读方向。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @return 返回文字对象的阅读方向。
 *				
 */
int		OFD_TextObject_GetReadDirection(OFD_PAGEOBJECT hTextObject);

/**
 * @brief 获取文字对象的字符方向。
 *
 * @param[in] hTextObject	文字对象对象句柄。
 * @return 返回文字对象的字符方向。
 *				
 */
int		OFD_TextObject_GetCharDirection(OFD_PAGEOBJECT hTextObject);

/**
 * @brief 获取文字对象的粗细度。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @return 返回文字对象的粗细度。
 *				
 */
int		OFD_TextObject_GetWeight(OFD_PAGEOBJECT hTextObject);

/**
 * @brief 判断文字对象是否斜体。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @return 返回TRUE为斜体，返回FALSE不是斜体。
 *				
 */
OFD_BOOL		OFD_TextObject_IsItalic(OFD_PAGEOBJECT hTextObject);

/**
 * @brief 获取文字对象的字符信息(两次调用，第一次获取数据长度，第二次获取数据内容)。
 *
 * @param[in] hTextObject		文字对象句柄。
 * @param[out] pCharInfos		用于存储字符信息。
 * @param[out] iCount			得到字符信息长度。
 * @return 无。
 *				
 */
void		OFD_TextObject_GetCharInfos(OFD_PAGEOBJECT hTextObject, OFD_CHARINFO* pCharInfos, int *iCount);

/**
 * @brief 获取Video对象中数据(两次调用，第一次调用获取数据长度，第二次调用获取数据值)。
 *
 * @param[in] hVideoObject	Video对象句柄。
 * @param[out] lpbData		用于存储Video对象数据。
 * @param[out] piLen		用于存储Video对象数据长度。
 * @return 无。
 *				
 */
void		OFD_VideoObject_GetVideoData(OFD_PAGEOBJECT hVideoObject, OFD_LPBYTE lpbData, int *piLen);

/**
 * @brief 获取Video资源ID。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @return 成功返回video资源ID。
 *				
 */
int		OFD_VideoObject_GetResID(OFD_PAGEOBJECT hVideoObject);

/**
 * @brief 获取Video对象类型。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[out] wType			返回Video的类型。
 * @return 无。
 *				
 */
void		OFD_VideoObject_GetType(OFD_PAGEOBJECT hVideoObject, OFD_WSTR* wType);

/**
 * @brief 获取Video对象格式。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[out] wFormat			返回Video的格式。
 * @return 无。
 *				
 */
void		OFD_VideoObject_GetFormat(OFD_PAGEOBJECT hVideoObject, OFD_WSTR* wFormat);

/**
 * @brief 获取Video对象标题。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] wTitle			返回Video的标题。
 * @return 无。
 *				
 */
void		OFD_VideoObject_GetTitle(OFD_PAGEOBJECT hVideoObject, OFD_WSTR* wTitle);

/**
 * @brief 获取Video对象是否有边框。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @return 是否有边框。
 *				
 */
OFD_BOOL		OFD_VideoObject_HasBorder(OFD_PAGEOBJECT hVideoObject);

/**
 * @brief 获取Video边框线宽。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @return 边框线宽。
 *				
 */
float		OFD_VideoObject_GetLineWidth(OFD_PAGEOBJECT hVideoObject);

/**
 * @brief 获取Video对象边框虚线重复起始值。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @return 返回图元对象虚线重复起始值。
 *				
 */
float		OFD_VideoObject_GetDashOffset(OFD_PAGEOBJECT hVideoObject);

/**
 * @brief 获取Video对象边框虚线重复样式（两次调用第一次获取数据长度，第二次获取数据）。
 *
 * @param[in] hPageObject		Video对象句柄。
 * @param[out] fpDashPatterns	用于存储重复样式点数据。
 * @param[out] iCount			用于存储重复样式点数据长度（当dashPatterns为NULL时）。
 * @return 无。
 *				
 */
void		OFD_VideoObject_GetDashPattern(OFD_PAGEOBJECT hVideoObject, float *fpDashPatterns, int *iCount);

/**
 * @brief 获取Video边框线宽。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[out] fHCRadius		返回边框横向圆角半径。
 * @param[out] fHCRadius		返回边框纵向圆角半径。
 * @return 无。
 *				
 */
void		OFD_VideoObject_GetCornerRadius(OFD_PAGEOBJECT hVideoObject, float* fHCRadius, float* fVCRadius);

/**
 * @brief 获取Video边框颜色。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @return 边框颜色。
 *				
 */
OFD_ARGB		OFD_VideoObject_GetStrokeColor(OFD_PAGEOBJECT hVideoObject);

#ifdef __cplusplus
}
#endif

#endif //_OFD_PAGE_R_H_
