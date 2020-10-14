#ifndef _OFD_FONT_W_H_
#define _OFD_FONT_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建字体。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 字体句柄。
 *				
 */
OFD_FONT		OFD_Document_CreateFont(OFD_DOCUMENT hDocument);

/**
 * @brief 转换内嵌字体。
 *
 * @param[in]	hDocument	文档句柄。		
 * @return 无。
 *				
 */
void		OFD_Document_Convert2EmbededFont(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszName);

/**
 * @brief 设置字体名称。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] lpwszFontName		字体名称。
 * @return 无。
 *				
 */
void 		OFD_Font_SetFontName(OFD_FONT hFont, OFD_LPCWSTR lpwszFontName);

/**
 * @brief 设置字体族名称。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] lpwszFamilyName		字体族名称。
 * @return 无。
 *				
 */
void		OFD_Font_SetFamilyName(OFD_FONT hFont, OFD_LPCWSTR lpwszFamilyName);

/**
 * @brief 设置粗体。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] bSet		是否是粗体。
 * @return 无。
 *				
 */
void		OFD_Font_SetBold(OFD_FONT hFont, OFD_BOOL bSet);

/**
 * @brief 设置衬线。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] bSet		是否有衬线。
 * @return 无。
 *				
 */
void		OFD_Font_SetSerif(OFD_FONT hFont, OFD_BOOL bSet);

/**
 * @brief 设置斜体。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] bSet		是否是斜体。
 * @return 无。
 *				
 */
void		OFD_Font_SetItalic(OFD_FONT hFont, OFD_BOOL bSet);

/**
 * @brief 设置固定宽度。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] bSet		固定宽度。
 * @return 无。
 *				
 */
void		OFD_Font_SetFixedWidth(OFD_FONT hFont, OFD_BOOL bSet);

/**
 * @brief 设置字符集。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] lpwszCharset		字型适用的字符分类，可取值为 symbol 、 prc、 big5、 unicode 等。
 * @return 无。
 *				
 */
void 		OFD_Font_SetCharset(OFD_FONT hFont, OFD_LPCWSTR lpwszCharset);

/**
 * @brief 设置字体文件。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] lpwszFile		指向内嵌字型文件，嵌入字型文件应使用 OpenType 格式。
 * @return 无。
 *				
 */
void		OFD_Font_SetFontFile(OFD_FONT hFont, OFD_LPCWSTR lpwszFile);

/**
 * @brief 设置字体文件。
 *
 * @param[in] hFont		字体句柄。	
 * @param[in] pBuf		字体数据
 * @param[in] iBufLen	字体数据长度
 * @return 无。
 *				
 */
void		OFD_Font_SetFontData(OFD_FONT hFont, OFD_LPCBYTE pBuf, int iBufLen);

#ifdef __cplusplus
}
#endif

#endif //_OFD_FONT_W_H_
