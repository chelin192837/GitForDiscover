#ifndef _OFD_TEXTPAGE_R_H_
#define _OFD_TEXTPAGE_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**文本页面*/
OFD_DEFINEHANDLE(OFD_TEXTPAGE);

/**文本页面选择*/
OFD_DEFINEHANDLE(OFD_TEXTSELECTION);

/**文本页面查找*/
OFD_DEFINEHANDLE(OFD_TEXTFIND);

#define OFD_TEXT_MATCH_CASE			0x00000001
#define OFD_TEXT_MATCH_WHOLEWORD	0x00000002

#define OFD_TEXT_SELTYPE_RECTBOX		0
#define OFD_TEXT_SELTYPE_PARAGRAPH		1

/**
 * @brief	Structure for character's information definition
 */
typedef struct _OFD_TEXTPAGE_CHARINFO_
{
	OFD_RECT	charBox;			    //字符外框(相对于页面位置)
	OFD_DWORD	charcode;				//文字的UNICODE编码值
	OFD_DWORD	textObjectID;			//字符所属文字对象ID
} OFD_TEXTPAGE_CHARINFO;

/**
 * @brief 创建文本页面。
 *
 * @param[in]	hPage	页句柄。		
 * @return 成功返回文本页面句柄。
 *				
 */
OFD_TEXTPAGE	OFD_TextPage_Create(OFD_PAGE hPage);

/**
 * @brief 关闭文本页面，并释放对应内存。
 *
 * @param[in]	hTextPage	文本页句柄。		
 * @return 成功返回文本页面句柄。
 *				
 */
void	OFD_TextPage_Release(OFD_TEXTPAGE hTextPage);

/**
 * @brief 计算文本页内的字符个数。
 *
 * @param[in]	hTextPage	文本页句柄。	
 * @return 成功返回字符个数，失败返回-1。
 *				
 */
int	OFD_TextPage_CountChars(OFD_TEXTPAGE hTextPage);

/**
 * @brief 获取字符的属性信息。
 *
 * @param[in]	hTextPage	文本页句柄。	
 * @param[in]	charIndex	字符序号（由0开始编号）。
 * @param[in]	info		字符的属性信息。
 * @return 是否可以获取到字符的属性信息。
 *				
 */
OFD_BOOL	OFD_TextPage_GetCharInfo(OFD_TEXTPAGE hTextPage, int charIndex, OFD_TEXTPAGE_CHARINFO* info);

/**
 * @brief 获取指定坐标的文字索引。
 *
 * @param[in]	hTextPage	文本页句柄。
 * @param[in]	x			文字X坐标。
 * @param[in]	y			文字Y坐标。
 * @param[in]	tolerance	误差。
 * @return 成功返回字符索引（从0开始），失败返回-1，未找到返回-2。
 *	
 */
int	OFD_TextPage_GetCharIndexAtPos(OFD_TEXTPAGE hTextPage, float x, float y, float tolerance);

/**
 * @brief 获取字符文本。
 *
 * @param[in]	hTextPage	文本页句柄。	
 * @param[in]	charIndex	字符序号（由0开始编号）。
 * @param[in]	cout		字符个数。
 * @param[in]	text		用来存放字符串。
 * @return 是否可以获取到字符。
 *				
 */
OFD_BOOL	OFD_TextPage_GetText(OFD_TEXTPAGE hTextPage, int charIndex, int count, OFD_WSTR* text);

/**
 * @brief 导出文本页内容。
 *
 * @param[in]	hTextPage	文本页句柄。
 * @param[in]	file		文件路径。
 * @return 无。
 *				
 */
void	OFD_TextPage_ExportToFile(OFD_TEXTPAGE hTextPage, OFD_LPCWSTR file);

/**
 * @brief 创建文本选择对象。
 *
 * @param[in]	hPage	页句柄。		
 * @return 成功返回文本选择页面句柄，失败返回NULL。
 *				
 */
OFD_TEXTSELECTION	OFD_TextPageSelect_Create(OFD_PAGE hPage);

/**
 * @brief 关闭文本选择对象，并释放对应内存。
 *
 * @param[in]	hTextSel	文本页句柄。		
 * @return 无。
 *				
 */
void	OFD_TextSelection_Release(OFD_TEXTSELECTION hTextSel);

/**
 * @brief 设置文本选择对象的类型。
 *
 * @param[in]	hTextSel	文本页句柄。
 * @param[in]	mode		文本选择对象的类型。
 * @return 无。
 *				
 */
void	OFD_TexSelect_SetMode(OFD_TEXTSELECTION hTextSel, int mode);

/**
 * @brief 设置文本选择对象的范围。
 *
 * @param[in]	hTextSel	文本页句柄。
 * @param[in]	startPoint	文本选择起点。
 * @param[in]	startPoint	文本选择终点。
 * @return 无。
 *				
 */
void	OFD_TextSelect_SetRange(OFD_TEXTSELECTION hTextSel, OFD_POINT startPoint, OFD_POINT endPoint);

/**
 * @brief 计算文本选择对象ID的个数。
 *
 * @param[in]	hTextSel	文本页句柄。
 * @return 成功返回文本选择对象ID个数，失败返回-1。
 *				
 */
int		OFD_TextSelect_CountTextID(OFD_TEXTSELECTION hTextSel);

/**
 * @brief 获取文本选择对象的ID。
 *
 * @param[in]	hTextSel	文本页句柄。
 * @param[in]	index		字符序号（由0开始编号）。
 * @return 成功返回文本对象的ID，失败返回NULL。
 *				
 */
OFD_DWORD	OFD_TextSelect_GetTextID(OFD_TEXTSELECTION hTextSel, int index);

/**
 * @brief 计算文本选择对象的个数。
 *
 * @param[in]	hTextSel	文本页句柄。
 * @return 成功返回文本对象个数，失败返回-1。
 *				
 */
int		OFD_TextSelect_CountRect(OFD_TEXTSELECTION hTextSel);

/**
 * @brief 获取文本选择对象的属性。
 *
 * @param[in]	hTextSel	文本页句柄。
 * @param[in]	index		字符序号（由0开始编号）。
 * @return 成功返回文本选择对象的属性，失败返回属性都为0。
 *				
 */
OFD_RECT	OFD_TextSelect_GetRect(OFD_TEXTSELECTION hTextSel, int index);

/**
 * @brief 获取文本选择对象的内容。
 *
 * @param[in]	hTextSel	文本页句柄。	
 * @param[in]	text		存放字符的文本。
 * @return 无。
 *				
 */
void	OFD_TextSelect_GetText(OFD_TEXTSELECTION hTextSel, OFD_WSTR* text);

/**
 * @brief 创建文本查找对象。
 *
 * @param[in]	hPage	页句柄。		
 * @return 成功返回文本查找对象句柄，失败返回NULL。
 *				
 */
OFD_TEXTFIND	OFD_TextFind_Create(OFD_PAGE hPage);

/**
 * @brief 关闭文本查找对象，并释放对应内存。
 *
 * @param[in]	hTextFind	文本页句柄。		
 * @return 无。
 *				
 */
void	OFD_TextFind_Release(OFD_TEXTFIND hTextFind);

/**
 * @brief 设置文本查找对象。
 *
 * @param[in]	hTextFind	文本页句柄。
 * @param[in]	findwhat	查找的字符。
 * @param[in]	flags		文本查找的标志。
 * @param[in]	startPos	起始位置。
 * @return 无。
 *				
 */
void	OFD_TextFind_Set(OFD_TEXTFIND hTextFind, OFD_LPCWSTR findwhat, int flags, int startPos);

/**
 * @brief 获取第一个该字符的位置。
 *
 * @param[in]	hTextFind	文本页句柄。		
 * @return 成功返回该字符的位置，失败返回-1。
 *				
 */
int	OFD_TextFind_FindNext(OFD_TEXTFIND hTextFind);

/**
 * @brief 获取前一个该字符的位置。
 *
 * @param[in]	hTextFind	文本页句柄。		
 * @return 成功返回该字符的位置，失败返回-1。
 *				
 */
int	OFD_TextFind_FindPrev(OFD_TEXTFIND hTextFind);

/**
 * @brief 计算第一个该字符所在文本框的个数。
 *
 * @param[in]	hTextFind	文本页句柄。		
 * @return 成功返回文本框的个数，失败返回-1。
 *				
 */
int	OFD_TextFind_CountRect(OFD_TEXTFIND hTextFind);

/**
 * @brief 获取第一个该字符的属性。
 *
 * @param[in]	hTextFind	文本页句柄。		
 * @return 成功返回该字符的属性，失败返回属性都为0。
 *
 */
OFD_RECT	OFD_TextFind_GetRect(OFD_TEXTFIND hTextFind, int index);

/**
 * @brief 获取单行字符串数组句柄。
 *
 * @param[in] hPage		页句柄。
 * @return 成功返回行字符串数组句柄，失败返回NULL。
 *				
 */
OFD_TEXTOFLINE OFD_Page_GetText(OFD_PAGE hPage);

/**
 * @brief 获取字符串行数。
 *
 * @param[in] textOfLine	行字符串数组句柄。
 * @return 成功返回字符串行数，失败返回-1。
 *				
 */
int OFD_TextOfLine_CountText(OFD_TEXTOFLINE textOfLine);

/**
 * @brief 获取单行字符串。
 *
 * @param[in]  textOfLine	行字符串数组句柄。
 * @param[out] wsText		字符串。
 * @param[in]  index		序号。
 * @return 无。
 *				
 */
void OFD_TextOfLine_GetText(OFD_TEXTOFLINE textOfLine,OFD_WSTR* wsText,int index);

/**
 * @brief 释放。
 *
 * @param[in] textOfLine	行字符串数组句柄。
 * @return 无。
 *				
 */
void OFD_TextOfLine_Release(OFD_TEXTOFLINE textOfLine);

/**
 * @brief 从包中查找指定的字符串。
 *
 * @param[in]	hPackage		包句柄。	
 * @param[in]	lpwszFind		查找内容。
 * @return 成功返回查找句柄，失败返回NULL。
 *				
 */
OFD_SEARCH		OFD_Package_Search(OFD_PACKAGE hPackage, OFD_LPCWSTR lpwszFind);

/**
 * @brief 获取OFD文件全文
 *
 * @param[in]	hPackage		包句柄。
 * @param[out]	text			用于存储文字对象数据。
 * @return 无
 *				
 */
void		OFD_Package_GetTextContent(OFD_PACKAGE hPackage, OFD_WSTR* text);

#ifdef __cplusplus
};
#endif

#endif //_OFD_TEXTPAGE_R_H_

