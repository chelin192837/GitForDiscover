#ifndef _OFD_EI_H_
#define _OFD_EI_H_

#include "../include/ofd_base_r.h"

/**发票标引节点句柄*/
OFD_DEFINEHANDLE(OFD_TTNODE);

class IFX_FileRead;
class IFX_FileWrite;
class IFX_FileStream;

void OFD_SetEISDK();

void OFD_AddFontFileMap(OFD_LPCWSTR fontName, OFD_LPCWSTR fontFile);

void OFD_Package_SetSaveFormat(OFD_PACKAGE hPackage, int format);
//void OFD_Package_SetEmbedFont(OFD_PACKAGE hPackage, int flag);
OFD_BOOL OFD_Package_SaveToFileWrite(OFD_PACKAGE hPackage, IFX_FileWrite* pFileWrite);
OFD_BOOL OFD_Package_OptimizerToFileWrite(OFD_PACKAGE hPackage, IFX_FileWrite* pFileWrite);
OFD_BOOL OFD_Package_SaveAsPDF(OFD_PACKAGE hPackage, OFD_LPCWSTR lpwszFileName);

OFD_PAGE OFD_Document_AddDetailPage(OFD_DOCUMENT hDocument, int nIndex);
void OFD_Document_GetFormData(OFD_DOCUMENT hDocument, int page, OFD_LPBYTE lpbData, int *piLen);
void OFD_Document_SetFormData(OFD_DOCUMENT hDocument, int page, OFD_LPCWSTR lpwszFileName);
void OFD_Document_SetFormData(OFD_DOCUMENT hDocument, int page, IFX_FileStream* pStream);
int OFD_Document_FormAndData_Parse(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszDataFileName);
int OFD_Document_FormAndData_Assemble(OFD_DOCUMENT hDocument, int bNodeUnique);
int OFD_Document_FormAndData_AssembleSeal(OFD_DOCUMENT hDocument, void* hSeal, IFX_FileRead* pFileRead, IFX_FileWrite* pFileWrite);
int OFD_Document_FormAndData_AssembleSeal(OFD_DOCUMENT hDocument, void* hSeal, OFD_PACKAGE hPackage, IFX_FileWrite* pFileWrite);
void OFD_Document_FormAndData_Release(OFD_DOCUMENT hDocument);
void OFD_Document_InitTTNode(OFD_DOCUMENT hDocument, OFD_LPCWSTR type, OFD_LPCWSTR tagNameSpace, OFD_LPCWSTR tagName, OFD_LPCWSTR tagFileExt, int bElementUnique);
void OFD_Document_BeginTTNode(OFD_DOCUMENT hDocument, OFD_LPCWSTR path);
OFD_BOOL OFD_TTNode_InsertMetaData(OFD_DOCUMENT hDocument, OFD_LPCWSTR pwData, OFD_LPCWSTR pwType, int nIndex = -1);
void OFD_Document_EndTTNode(OFD_DOCUMENT hDocument, OFD_LPCWSTR path);
void OFD_Document_ClearTTNode(OFD_DOCUMENT hDocument);
OFD_TTNODE OFD_TTNode_GetRoot(OFD_DOCUMENT hDocument, OFD_LPCWSTR type);
int OFD_TTNode_Count(OFD_TTNODE hNode);
OFD_TTNODE OFD_TTNode_Get(OFD_TTNODE hNode, int index);
OFD_BOOL OFD_TTNode_IsGroup(OFD_TTNODE hNode);
void OFD_TTNode_GetTagKey(OFD_TTNODE hNode, OFD_WSTR* key);
void OFD_TTNode_GetText(OFD_TTNODE hNode, OFD_WSTR* text);
int OFD_TTNode_CountObj(OFD_TTNODE hNode);
OFD_PAGEOBJECT OFD_TTNode_GetPageObj(OFD_TTNODE hNode, int index);
OFD_BOOL OFD_TTNode_InsertObject(OFD_DOCUMENT hDocument, int nPageIndex, OFD_RECT rect);

void OFD_TextObject_Layout_SetText(OFD_PAGEOBJECT textObj, OFD_LPCWSTR text);
/**
 * @brief 设置Text起始位置相对于boundary的偏移量。（富文本排版）
 *
 * @param[in] textObj	text对象句柄。
 * @param[in] x			x轴偏移量。
 * @param[in] y			y轴偏移量。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_SetTextStartOffset(OFD_PAGEOBJECT textObj, float x, float y);
void OFD_TextObject_Layout_SetHAlignment(OFD_PAGEOBJECT textObj, int iAlign);
void OFD_TextObject_Layout_SetVAlignment(OFD_PAGEOBJECT textObj, int iAlign);
void OFD_TextObject_Layout_SetJustifiedX(OFD_PAGEOBJECT textObj, OFD_BOOL bSet);
void OFD_TextObject_Layout_SetJustifiedY(OFD_PAGEOBJECT textObj, OFD_BOOL bSet);
void OFD_TextObject_Layout_SetMultiline(OFD_PAGEOBJECT textObj, OFD_BOOL bSet);
void OFD_TextObject_Layout_SetFontName(OFD_PAGEOBJECT textObj, OFD_LPCWSTR fontName);
/**
 * @brief 设置备选字体。（生僻字）
 *
 * @param[in] textObj		text对象句柄。
 * @param[in] subfontName	备选字体名称。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_SetSubFontName(OFD_PAGEOBJECT textObj, OFD_LPCWSTR subfontName);
void OFD_TextObject_Layout_SetFontSize(OFD_PAGEOBJECT textObj, float fontSize);

/**
 * @brief 设置文字对象的粗细度属性。
 *
 * @param[in] textObj		text对象句柄。
 * @param[in] weight		设定的粗细度值(可选取值为100,200,300,400,500,600,700,800,900,默认值为400,取值为其它值时按照默认值处理)。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_SetWeight(OFD_PAGEOBJECT textObj, int weight);

/**
 * @brief 设置文字对象的斜体属性。
 *
 * @param[in] textObj		text对象句柄。
 * @param[in] bItalic		设置为TRUE为斜体，FALSE为非斜体。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_SetItalic(OFD_PAGEOBJECT textObj, OFD_BOOL bItalic);

/**
 * @brief 添加文字对象的下划线属性。
 *
 * @param[in] textObj		text对象句柄。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_AddUnderLine(OFD_PAGEOBJECT textObj);

/**
 * @brief 设置文字对象的最大字体大小。（富文本排版）
 *
 * @param[in] textObj		text对象句柄。
 * @param[in] fontSize		字体大小。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_SetRTFFontSize(OFD_PAGEOBJECT textObj, float fontSize);
void OFD_TextObject_Layout_SetLineCharNum(OFD_PAGEOBJECT textObj, int num);
void OFD_TextObject_Layout_SetLineBitNum(OFD_PAGEOBJECT textObj, int bitNum);
void OFD_TextObject_Layout_SetLineSpace(OFD_PAGEOBJECT textObj, float lineSpacing);
void OFD_TextObject_Layout_SetLineBreak(OFD_PAGEOBJECT textObj, OFD_LPCWSTR lpwszBreak);
void OFD_TextObject_Layout_SetRotate(OFD_PAGEOBJECT textObj, int iRotate);
void OFD_TextObject_Layout_SetAutoFontSize(OFD_PAGEOBJECT textObj, OFD_BOOL bset);
void OFD_TextObject_Layout_SetVerticalType(OFD_PAGEOBJECT textObj, OFD_BOOL bset);
void OFD_TextObject_Layout_SetVerticalOrder(OFD_PAGEOBJECT textObj, int iorder);
void OFD_TextObject_Layout_SetDeflateUnit(OFD_PAGEOBJECT textObj, float fdeflateUnit);
void OFD_TextObject_Layout_CalcLineBox(OFD_LPCWSTR pwszStr, OFD_LPCWSTR fontName, float fontSize, OFD_RECT& r);
void OFD_TextObject_Layout_CalcMultiLineBox(OFD_LPCWSTR pwszStr, OFD_LPCWSTR fontName, float fontSize, float lineWidth, OFD_RECT& r);
void OFD_TextObject_Layout_Update(OFD_PAGEOBJECT textObj);

/**
 * @brief 获取Text结束位置相对于boundary的偏移量。（富文本排版）
 *
 * @param[in] textObj	text对象句柄。
 * @param[out] x		x轴偏移量。
 * @param[out] y		y轴偏移量。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_GetTextEndOffset(OFD_PAGEOBJECT textObj, float& x, float& y);
/**
 * @brief 获取Text对象是否存在代理对。（生僻字）
 *
 * @param[in] textObj		text对象句柄。
 * @return 成功返回1，失败返回0。
 *				
 */
OFD_BOOL OFD_TextObject_Layout_GetSurrogatePairFlag(OFD_PAGEOBJECT textObj);
/**
 * @brief 拷贝公共及代理对信息至目标对象。（生僻字）
 *
 * @param[in/out] dst_textObj		目标text对象句柄。
 * @param[in] src_textObj			源text对象句柄。
 * @return 成功返回1，失败返回0。
 *				
 */
OFD_BOOL OFD_TextObject_Layout_CopySurrogatePairInfo(OFD_PAGEOBJECT dst_textObj, OFD_PAGEOBJECT src_textObj);
/**
 * @brief 代理对布局。（生僻字）
 *
 * @param[in] textObj				text对象句柄。
 * @return 无。
 *				
 */
void OFD_TextObject_Layout_SurrogatePair_Update(OFD_PAGEOBJECT textObj);
/**
 * @brief 获取层。
 *
 * @param[in] textObj				text对象句柄。
 * @return 成功返回层的句柄，失败返回NULL。
 *				
 */
OFD_LAYER OFD_TextObject_GetLayer(OFD_PAGEOBJECT textObj);

void OFD_ImageObject_SetImageFromBuf2(OFD_PAGEOBJECT hImageObject, OFD_LPBYTE lpbDataBuf, int iLen, int imageFormat, OFD_BOOL bTakeover);

/**
 * @brief 页面信息屏蔽（不可恢复）
 *
 * @param[in] hPage		页句柄。
 * @param[in] rect		矩形区域。
 * @param[in] shape		0 矩形，1 圆形。
 * @param[in] mode		0 涂黑，1 雾化，2 马赛克。
 * @param[in] horzBlur	高斯模糊。
 * @param[in] vertBlur	高斯迷糊。
 * @param[in] v			马赛克向量长度。
 * @return 无。
 *				
 */
void OFD_Page_InfoCover2(OFD_PAGE hPage, OFD_RECT rect, int shape, int mode, int horzBlur, int vertBlur, int v);

#endif //_OFD_EI_H_
