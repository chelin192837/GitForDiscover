#ifndef _OFD_PAGE_W_H_
#define _OFD_PAGE_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置页面区域。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] iBoxType	要设置的页面区域类型（值应为OFD_PAGE_BOXTYPE_PHYSICAL或OFD_PAGE_BOXTYPE_APPLICATION或OFD_PAGE_BOXTYPE_CONTENT或OFD_PAGE_BOXTYPE_BLEED）。
 * @param[in] pageArea	页面区域值,默认为A4（210*297）。
 * @return 无。
 *				
 */
void		OFD_Page_SetPageArea(OFD_PAGE hPage, int iBoxType, OFD_RECT pageArea);

/**
 * @brief 设置页面旋转。
 *
 * @param[in] hPage		页句柄。
 * @param[in] nRotate	旋转角度。
 * @return 无。
 *				
 */
void		OFD_Page_SetRotate(OFD_PAGE hPage, int nRotate);

/**
 * @brief 新建一个层，并追加至页中。
 *
 * @param[in] hPage		页句柄。		
 * @return 成功返回层句柄，失败翻译NULL。
 *				
 */
OFD_LAYER		OFD_Page_AddLayer(OFD_PAGE hPage);

/**
 * @brief 新建一个页面块，并追加至页中。
 *
 * @param[in] hPage		页句柄。		
 * @return 成功返回页面块句柄，失败翻译NULL。
 *				
 */
OFD_BLOCKOBJECT		OFD_Page_AddPageBlock(OFD_PAGE hPage);

/**
 * @brief 新建一个层，并插入页中指定位置。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] index		层索引号（由0开始编号）。	
 * @return 成功返回层句柄，失败返回NULL。
 *				
 */
OFD_LAYER		OFD_Page_InsertLayer(OFD_PAGE hPage, int index);

/**
 * @brief 新建一个页面块，并插入页中指定位置。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] index		层索引号（由0开始编号）。	
 * @return 成功返回页面块句柄，失败返回NULL。
 *				
 */
OFD_BLOCKOBJECT	OFD_Page_InsertPageBlock(OFD_PAGE hPage, int nIndex);

/**
 * @brief 删除层。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] index		删除层的索引号（由0开始编号）。	
 * @return 无。
 *				
 */
void		OFD_Page_RemoveLayer(OFD_PAGE hPage, int index);

/**
 * @brief 删除页面块。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] index		删除页面块的索引号（由0开始编号）。	
 * @return 无。
 *				
 */
void		OFD_Page_RemovePageBlock(OFD_PAGE hPage, int index);

/**
 * @brief 设置页面的模板属性和呈现顺序。
 *
 * @param[in] hPage				页句柄。
 * @param[in] iTemplateID		模板页ID。
 * @param[in] iTemplateID		模板页的呈现顺序，0：背景层（默认）1：正文层 2：前景层。
 * @return 无。
 *				
 */
void		OFD_Page_SetTemplatePage(OFD_PAGE hPage, int iTemplateID, int iZorder = 0);

/**
 * @brief 设置模板页的名称。
 *
 * @param[in] hPage				模板页句柄。
 * @param[in] lpwszName			模板页名称。
 * @return 无。
 *				
 */
void		OFD_TemplatePage_SetName(OFD_PAGE hPage, OFD_LPCWSTR lpwszName);

/**
 * @brief 新建一个块对象，并追加至指定页面块中。
 *
 * @param[in] hBlock		页面块句柄。
 * @return 成功返回path对象的句柄，失败返回NULL。
 *				
 */
OFD_BLOCKOBJECT		OFD_Block_AddBlockObject(OFD_BLOCKOBJECT hBlock);

/**
 * @brief 新建一个path对象，并追加至指定层中。
 *
 * @param[in] hLayer		层句柄。
 * @return 成功返回path对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Layer_AddPathObject(OFD_LAYER hLayer);

/**
 * @brief 新建一个path对象，并追加至指定页面块中。
 *
 * @param[in] hBlock		页面块句柄。
 * @return 成功返回path对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Block_AddPathObject(OFD_BLOCKOBJECT hBlock);

/**
 * @brief 新建一个Image对象，并追加至指定层中。
 *
 * @param[in] hLayer		层句柄。
 * @return 成功返回Image对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Layer_AddImageObject(OFD_LAYER hLayer);

/**
 * @brief 新建一个Image对象，并追加至指定页面块中。
 *
 * @param[in] hBlock		页面块句柄。
 * @return 成功返回Image对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Block_AddImageObject(OFD_BLOCKOBJECT hBlock);

/**
 * @brief 新建一个Text对象，并追加至指定层中。
 *
 * @param[in] hLayer		层句柄。
 * @return 成功返回Text对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Layer_AddTextObject(OFD_LAYER hLayer);

/**
 * @brief 新建一个Text对象，并追加至指定页面块中。
 *
 * @param[in] hBlock		页面块句柄。
 * @return 成功返回Text对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Block_AddTextObject(OFD_BLOCKOBJECT hBlock);

/**
 * @brief 新建一个Video对象，并追加至指定层中。
 *
 * @param[in] hLayer		层句柄。
 * @return 成功返回Video对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Layer_AddVideoObject(OFD_LAYER hLayer);

/**
 * @brief 新建一个Video对象，并追加至指定页面块中。
 *
 * @param[in] hBlock		页面块句柄。
 * @return 成功返回Video对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Block_AddVideoObject(OFD_BLOCKOBJECT hBlock);

/**
 * @brief 新建一个Composite对象，并追加至指定层中。
 *
 * @param[in] hLayer		层句柄。
 * @return 成功返回Composite对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Layer_AddCompositeObject(OFD_LAYER hLayer);

/**
 * @brief 新建一个Composite对象，并追加至指定页面块中。
 *
 * @param[in] hBlock		页面块句柄。
 * @return 成功返回Composite对象的句柄，失败返回NULL。
 *				
 */
OFD_PAGEOBJECT		OFD_Block_AddCompositeObject(OFD_BLOCKOBJECT hBlock);

/**
 * @brief 设置层的类型。
 *
 * @param[in] hLayer		层句柄。
 * @param[in] iZorderType	层指定的类型（默认为OFD_LAYER_ZORDER_BACKGROUND）
 * @return 无。
 *				
 */
void		OFD_Layer_SetZorderType(OFD_LAYER hLayer, int iZorderType);

/**
 * @brief 删除指定的图元对象。
 *
 * @param[in] hLayer		层句柄。
 * @param[in] index			图元对象的索引号（由0开始编号）。
 * @return 无。
 *				
 */
void		OFD_Layer_RemovePageObject(OFD_LAYER hLayer, int index);

/**
 * @brief 删除所有图元对象。
 *
 * @param[in] hLayer		层句柄。
 * @return 无。
 *				
 */
void		OFD_Layer_RemoveAllPageObjects(OFD_LAYER hLayer);

/**
 * @brief 设置图元对象名称属性。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] lpwszName		名称字符串。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetName(OFD_PAGEOBJECT hPageObject, OFD_LPCWSTR lpwszName);

/**
 * @brief 设置图元对象可见性属性。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] bVisible		可见性属性值（设置为TRUE可见，FALSE不可见）。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetVisible(OFD_PAGEOBJECT hPageObject, OFD_BOOL bVisible);

/**
 * @brief 设置图元对象线宽。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] fLineWidth		线宽值（单位毫米）。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetLineWidth(OFD_PAGEOBJECT hPageObject, float fLineWidth);

/**
 * @brief 设置图元对象线帽样式。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] linecap		线帽样式值（默认为OFD_LINE_CAP_BUTT）。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetLineCap(OFD_PAGEOBJECT hPageObject, int linecap);

/**
 * @brief 设置图元对象结合点样式。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] linejoin		结合点样式值（默认为OFD_LINE_JOIN_MITER）。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetLineJoin(OFD_PAGEOBJECT hPageObject, int linejoin);

/**
 * @brief 设置图元对象斜切限制值。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] fMiterLimit	斜切限制值。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetMiterLimit(OFD_PAGEOBJECT hPageObject, float fMiterLimit);

/**
 * @brief 设置图元对象虚线重复样式。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] fDashOffset	图元对象虚线重复起始值
 * @param[in] pfDashPatters	重复样式点数据。
 * @param[in] iCount		重复样式点数据长度。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetDashPattern(OFD_PAGEOBJECT hPageObject, float fDashOffset, float* pfDashPatters, int iCount);

/**
 * @brief 设置图元对象alpha属性。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] alpha			alpha值（0-255）。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetAlpha(OFD_PAGEOBJECT hPageObject, OFD_BYTE alpha);

/**
 * @brief 设置图元对象填充颜色。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] fillColor		颜色值。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetFillColor(OFD_PAGEOBJECT hPageObject, OFD_ARGB fillColor);

/**
 * @brief 设置图元对象底纹填充单元。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] pPattern		底纹填充单元。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetFillPattern(OFD_PAGEOBJECT hPageObject, OFD_PATTERN* pPattern);

/**
 * @brief 设置图元对象轴向渐变填充属性。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] axialShd		渐变填充值。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetFillAxialShd(OFD_PAGEOBJECT hPageObject, OFD_AXIALSHD* axialShd);

/**
 * @brief 设置图元对象径向渐变填充属性。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] radialShd		渐变填充值。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetFillRadialShd(OFD_PAGEOBJECT hPageObject, OFD_RADIALSHD* radialShd);

/**
 * @brief 设置图元对象勾边颜色。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] strokeColor	颜色值。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetStrokeColor(OFD_PAGEOBJECT hPageObject, OFD_ARGB strokeColor);

/**
 * @brief 设置图元对象矩形外接框。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] boundary		矩形外接框值。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetBoundary(OFD_PAGEOBJECT hPageObject, OFD_RECT boundary);

/**
 * @brief 设置图元对象矩阵变换值。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @param[in] matrix			矩阵变换值。
 * @return 无。
 *				
 */
void		OFD_PageObject_SetCTM(OFD_PAGEOBJECT hPageObject, OFD_MATRIX matrix);

/**
 * @brief 设置图元对象动作数组句柄。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 返回图元对象动作数组句柄。
 *				
 */
OFD_ACTIONS	OFD_PageObject_CreateActions(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 创建一个裁剪区句柄。
 *
 * @param[in] hPageObject	图元对象句柄。
 * @return 裁剪区句柄。
 *				
 */
OFD_CLIPREGION		OFD_PageObject_CreateClipRegion(OFD_PAGEOBJECT hPageObject);

/**
 * @brief 添加一个裁剪句柄。
 *
 * @param[in] hClipRegion	裁剪区句柄。
 * @return 裁剪句柄。
 *				
 */
OFD_CLIP		OFD_ClipRegion_AddClip(OFD_CLIPREGION hClipRegion);

/**
 * @brief 添加一个裁剪区域句柄。
 *
 * @param[in] hClip	裁剪区句柄。
 * @return 裁剪区域句柄。
 *				
 */
OFD_CLIPAREA		OFD_Clip_AddArea(OFD_CLIP hClip);

/**
 * @brief 添加图元对象句柄。
 *
 * @param[in] hClipArea	裁剪区句柄。
 * @return 图元对象句柄。
 *				
 */
OFD_PAGEOBJECT		OFD_ClipArea_AddPathObject(OFD_CLIPAREA hClipArea);

/**
 * @brief 通过文件设置图像对象中的图像。
 *
 * @param[in] hImageObject		图像对象句柄。
 * @param[in] lpwszFilename		指定的图像文件名称。
 * @return 图像资源ID。
 *				
 */
OFD_DWORD		OFD_ImageObject_SetImageFromFile(OFD_PAGEOBJECT hImageObject, OFD_LPCWSTR lpwszFilename);

/**
 * @brief 通过内存流设置图像对象中的图像。
 *
 * @param[in] hImageObject	图像对象句柄。
 * @param[in] lpbDataBuf		图像数据指针。
 * @param[in] iLen			图像数据长度。
 * @param[in] imageFormat	图像格式（取值范围为OFD_IMAGE_FORMAT_BMP，OFD_IMAGE_FORMAT_JPG，OFD_IMAGE_FORMAT_PNG，OFD_IMAGE_FORMAT_TIF）
 * @return 图像资源ID。
 *				
 */
OFD_DWORD		OFD_ImageObject_SetImageFromBuf(OFD_PAGEOBJECT hImageObject, OFD_LPBYTE lpbDataBuf, int iLen, int imageFormat);

/**
 * @brief 设置图像对象的资源ID。
 *
 * @param[in] hImageObject	图像对象句柄。
 * @param[in] resID			图像资源ID。
 * @return 无。
 *				
 */
void OFD_ImageObject_SetImageRes(OFD_PAGEOBJECT hImageObject, OFD_DWORD resID);

/**
 * @brief 设置路径对象勾边属性。
 *
 * @param[in] hPathObject	路径对象句柄。
 * @param[in] bStroke		设定TRUE为勾边，FALSE为不勾边。
 * @return 无。
 *				
 */
void		OFD_PathObject_SetStroke(OFD_PAGEOBJECT hPathObject, OFD_BOOL bStroke);

/**
 * @brief 设置路径对象填充属性。
 *
 * @param[in] hPathObject	路径对象句柄。
 * @param[in] bFill			设定TRUE为填充，FALSE为不填充。
 * @return 无。
 *				
 */
void		OFD_PathObject_SetFill(OFD_PAGEOBJECT hPathObject, OFD_BOOL bFill);

/**
 * @brief 设置路径对象填充模式（默认为OFD_PATHFILL_NONZERO）。
 *
 * @param[in] hPathObject	路径对象句柄。
 * @param[in] fillmode		填充模式值。
 * @return 无。
 *				
 */
void		OFD_PathObject_SetFillMode(OFD_PAGEOBJECT hPathObject, int fillmode);

/**
 * @brief 设置路径对象中路径元素描述序列。
 *
 * @param[in] hPathObject		路径对象句柄。
 * @param[in] lpszAbbreviteData	路径元素描述序列（序列中点列坐标应为相对于boundary原点的相对坐标）。
 * @param[in] iLen				路径元素描述序列长度。
 * @return 无。
 *				
 */
void		OFD_PathObject_SetAbbreviteData(OFD_PAGEOBJECT hPathObject, OFD_LPCSTR lpszAbbreviteData, int iLen);

/**
 * @brief 图形对象设置数据。
 *
 * @param[in] hPathObject		图形对象句柄。
 * @param[in] hPath				图形数据对象句柄。
 * @return 无。
 *				
 */
void		OFD_PathObject_SetPath(OFD_PAGEOBJECT hPathObject, OFD_PATH hPath);

/**
 * @brief 设置文字对象的字体对象。
 *
 * @param[in] hTextObject		文字对象句柄。
 * @param[in] lpwszName			字体句柄。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetFont(OFD_PAGEOBJECT hTextObject, OFD_FONT font);

/**
 * @brief 设置文字对象的字体名称。
 *
 * @param[in] hTextObject		文字对象句柄。
 * @param[in] lpwszName			字体名称。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetFontName(OFD_PAGEOBJECT hTextObject, OFD_LPCWSTR lpwszName);

/**
 * @brief 设置文字对象的字体大小。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] fFontSize		字体大小值。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetFontSize(OFD_PAGEOBJECT hTextObject, float fFontSize);

/**
 * @brief 设置文字对象是否勾边。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] bStroke		设置为TRUE勾边，FALSE不勾边。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetStroke(OFD_PAGEOBJECT hTextObject, OFD_BOOL bStroke);

/**
 * @brief 设置文字对象是否填充。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] bFill			设置为TRUE填充，FALSE不填充。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetFill(OFD_PAGEOBJECT hTextObject, OFD_BOOL bFill);

/**
 * @brief 设置文字对象的水平缩放属性。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] hScale		设定的水平缩放值。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetHScale(OFD_PAGEOBJECT hTextObject, float hScale);

/**
 * @brief 设置文字对象的阅读方向属性。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] readDirection	设定的阅读方向值。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetReadDirection(OFD_PAGEOBJECT hTextObject, int readDirection);

/**
 * @brief 设置文字对象的字符方向属性。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] charDirection	设定的字符方向值。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetCharDirection(OFD_PAGEOBJECT hTextObject, int charDirection);

/**
 * @brief 设置文字对象的粗细度属性。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] weight		设定的粗细度值(可选取值为100,200,300,400,500,600,700,800,900,默认值为400,取值为其它值时按照默认值处理)。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetWeight(OFD_PAGEOBJECT hTextObject, int weight);

/**
 * @brief 设置文字对象的斜体属性。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] bItalic		设置为TRUE为斜体，FALSE为非斜体。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetItalic(OFD_PAGEOBJECT hTextObject, OFD_BOOL bItalic);

/**
 * @brief 添加文字对象的下划线属性。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @return 无。
 *				
 */
void		OFD_TextObject_AddUnderline(OFD_PAGEOBJECT hTextObject);

/**
 * @brief 设置文字对象的字符信息。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] pCharInfos	字符信息数组指针。
 * @param[in] hTextObject	字符信息数组长度。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetCharInfos(OFD_PAGEOBJECT hTextObject, OFD_CHARINFO* pCharInfos, int count);

/**
 * @brief 设置文字对象的字符信息。
 *
 * @param[in] hTextObject	文字对象句柄。
 * @param[in] lpwszText		文本内容
 * @param[in] fX			文本内容X坐标起始位置
 * @param[in] fY			文本内容Y坐标起始位置
 * @param[in] nLable		0 计算文本内容宽度是否大于BOUNDARY的宽度， 1不计算
 * @return FALSE 失败，TRUE 成功。
 *				
 */
OFD_BOOL	OFD_TextObject_SetText(OFD_PAGEOBJECT hTextObject, OFD_LPCWSTR lpwszText, float fX, float fY, int nLable = 0);

/**
 * @brief 设置文字对象的字符信息(文字块)。
 *
 * @param[in] hTextObject		文字对象句柄。
 * @param[in] pTextPieceInfos	文字块信息指针。
 * @return 无。
 *				
 */
void		OFD_TextObject_SetTextPieceInfos(OFD_PAGEOBJECT hTextObject, OFD_TEXTPIECEINFO* pTextPieceInfos);

/**
 * @brief 替换文字对象的字符信息(文字块)。
 *
 * @param[in] hTextObject		文字对象句柄。
 * @param[in] pInfo				文字对象内容。
 * @return 无。
 *				
 */
void		OFD_TextObject_ReplaceTextPieceInfos(OFD_PAGEOBJECT hTextObject, OFD_CHARINFO* pCharInfos, int count);

/**
 * @brief 根据指定的图像文件创建OFD_Pattern(应调用OFD_Pattern_Release()释放)。
 *
 * @param[in] lpwszImageFile;	//指定单元图像文件名称
 * @param[in] fWidth;			//底纹单元绘制区域的宽度(单位毫米)
 * @param[in] fHeight;			//底纹单元绘制区域的高度(单位毫米)
 * @param[in] fXStep;			//X方向底纹单元间距(默认为fWidth,单位毫米)
 * @param[in] fYStep;			//Y方向底纹单元间距(默认为fHeight,单位毫米)
 * @param[in] pMatrix;			//底纹单元变换矩阵（默认为单位矩阵(1,0,0,1,0,0)）
 * @return 成功返回OFD_Pattern指针，失败返回NULL。
 *				
 */
OFD_PATTERN*		OFD_Pattern_MakePatternFromFile(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszImageFile, float fWidth, float fHeight,\
		float fXStep, float fYStep, OFD_MATRIX* pMatrix);

/**
 * @brief 根据图像数据创建OFD_Pattern(应调用OFD_Pattern_Release()释放)。
 *
 * @param[in] pImageBuf;		//指定单元图像数据
 * @param[in] iImageBufLen;		//图像数据长度
 * @param[in] iImageFormat;		//图像数据类型(只为OFD_IMAGE_FORMAT_BMP，OFD_IMAGE_FORMAT_JPG，OFD_IMAGE_FORMAT_PNG，OFD_IMAGE_FORMAT_TIF）
 * @param[in] fWidth;			//底纹单元绘制区域的宽度(单位毫米)
 * @param[in] fHeight;			//底纹单元绘制区域的高度(单位毫米)
 * @param[in] fXStep;			//X方向底纹单元间距(默认为fWidth,单位毫米)
 * @param[in] fYStep;			//Y方向底纹单元间距(默认为fHeight,单位毫米)]
 * @param[in] pMatrix;			//底纹单元变换矩阵（默认为单位矩阵(1,0,0,1,0,0)）
 * @return 成功返回OFD_Pattern指针，失败返回NULL。
 *				
 */
OFD_PATTERN*		OFD_Pattern_MakePatternFromBuf(OFD_DOCUMENT hDocument, OFD_LPCBYTE pImageBuf, int iImageBufLen, int iImageFormat, float fWidth, float fHeight,\
		float fXStep, float fYStep, OFD_MATRIX* pMatrix);

/**
 * @brief 释放OFD_Pattern指针。
 *
 * @param[in] pPattern;			//指向OFD_Pattern类型的指针。
 * @return 无。
 *				
 */
void		OFD_Pattern_Release(OFD_DOCUMENT hDocument, OFD_PATTERN* pPattern);

/**
 * @brief 通过文件设置Video对象中的图像。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] lpwszFilename		指定的Video文件名称。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetVideoFromFile(OFD_PAGEOBJECT hVideoObject, OFD_LPCWSTR lpwszFilename);

/**
 * @brief 设置Video对象类型。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] lpwszType			指定Video的类型。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetType(OFD_PAGEOBJECT hVideoObject, OFD_LPCWSTR lpwszType);

/**
 * @brief 设置Video对象格式。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] lpwszFormat		指定Video的格式。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetFormat(OFD_PAGEOBJECT hVideoObject, OFD_LPCWSTR lpwszFormat);

/**
 * @brief 设置Video对象标题。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] lpwszTitle		指定Video的标题。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetTitle(OFD_PAGEOBJECT hVideoObject, OFD_LPCWSTR lpwszTitle);

/**
 * @brief 创建Video对象边框。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @return 无。
 *				
 */
void		OFD_VideoObject_CreateBorder(OFD_PAGEOBJECT hVideoObject);

/**
 * @brief 设置Video边框线宽。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] fLineWidth		指定边框线宽。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetLineWidth(OFD_PAGEOBJECT hVideoObject, float fLineWidth);

/**
 * @brief 设置Video边框虚线重复样式。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] fDashOffset		虚线重复起始值
 * @param[in] pfDashPatters		重复样式点数据。
 * @param[in] iCount			重复样式点数据长度。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetDashPattern(OFD_PAGEOBJECT hVideoObject, float fDashOffset, float* pfDashPatters, int iCount);

/**
 * @brief 设置Video边框线宽。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] fHCRadius			指定边框横向圆角半径。
 * @param[in] fHCRadius			指定边框纵向圆角半径。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetCornerRadius(OFD_PAGEOBJECT hVideoObject, float fHCRadius, float fVCRadius);

/**
 * @brief 设置Video边框颜色。
 *
 * @param[in] hVideoObject		Video对象句柄。
 * @param[in] strokeColor		颜色值。
 * @return 无。
 *				
 */
void		OFD_VideoObject_SetStrokeColor(OFD_PAGEOBJECT hVideoObject, OFD_ARGB strokeColor);

/**
 * @brief 添加复合单元。
 *
 * @param[in] hCompositeObject	Composite对象句柄。
 * @return 复合单元对象句柄。
 *				
 */
OFD_COMPOSITEUNIT		OFD_CompositeObject_AddUnit(OFD_PAGEOBJECT hCompositeObject);

/**
 * @brief 添加图层。
 *
 * @param[in] hCompositeObject	Composite对象句柄。
 * @return 图层对象句柄。
 *				
 */
OFD_LAYER		OFD_CompositeUnite_AddLayer(OFD_COMPOSITEUNIT hCompositeUnit);

#ifdef __cplusplus
}
#endif

#endif //_OFD_PAGE_W_H_
