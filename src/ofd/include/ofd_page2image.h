#ifndef _OFD_PAGE2IMAGE_H_
#define _OFD_PAGE2IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *基本类型定义
 */
/**布尔变量，应为TURE或FALSE*/
typedef int						OFD_BOOL;

/**字节（8位）*/
typedef unsigned char			OFD_BYTE;

/**指向OFD_BYTE类型的指针*/
typedef OFD_BYTE *				OFD_LPBYTE;

/**指向OFD_BYTE类型常量的指针*/
typedef const OFD_BYTE *		OFD_LPCBYTE;

/**无符号整型（32位）*/
typedef unsigned int            OFD_DWORD;

/**字符（8位) */
typedef char					OFD_CHAR;

/**指向8位字符的指针*/
typedef OFD_CHAR *				OFD_LPSTR;


/**矩形区域，以毫米为单位*/
typedef struct _OFD_RECT_ {
	float		x;			//起始点（左上点）x坐标
	float		y;			//起始点（左上点）y坐标
	float		width;		//宽度
	float		height;		//高度
} OFD_RECT;

/**
 *	页面区域定义
 */
#define OFD_PAGE_BOXTYPE_PHYSICAL		1
#define OFD_PAGE_BOXTYPE_APPLICATION	2
#define OFD_PAGE_BOXTYPE_CONTENT		3
#define OFD_PAGE_BOXTYPE_BLEED			4

/**
 * 句柄定义
 */
/**句柄类型宏定义*/
#define OFD_DEFINEHANDLE(name)	typedef struct _##name {void* pData;} * name;

/**包句柄，包含多个文档*/
OFD_DEFINEHANDLE(OFD_PACKAGE);

/**文档句柄，包含多个页*/
OFD_DEFINEHANDLE(OFD_DOCUMENT);

/**页句柄，包含多个层*/
OFD_DEFINEHANDLE(OFD_PAGE);

/**
 * @brief 初始化。
 *
 * @param[in]	license_id		序列号。	
 * @param[in]	unlock_code		验证码。
 * @return 成功返回0；失败返回-1。	
 *
 */
int OFD_Init(OFD_LPSTR license_id, OFD_LPSTR unlock_code);

/**
 * @brief 从内存中加载一个包。
 *
 * @param[in] lpbBuffer	文件内存流数据。		
 * @param[in] len		文件内存流数据长度。
 * @param[in] takeOver	是否有SDK接管内存
 * @return 成功返回包句柄，失败返回NULL。
 *				
 */
OFD_PACKAGE OFD_Package_LoadFromBuffer(OFD_LPCBYTE lpbBuffer, int len, OFD_BOOL takeOver);

/**
 * @brief 获取包中文档数。
 *
 * @param[in]	hPackage	包句柄。			
 * @return 成功返回文档的数量，失败返回-1。
 *				
 */
int OFD_Package_Count(OFD_PACKAGE hPackage);

/**
 * @brief 加载指定文档。
 *
 * @param[in]	hPackage	包句柄。		
 * @param[in]	index		文档序号。
 * @return 成功返回文档句柄，失败返回NULL。
 *				
 */
OFD_DOCUMENT OFD_Package_LoadDocument(OFD_PACKAGE hPackage, int index);

/**
 * @brief 获取文档页数。
 *
 * @param[in]	hDocument		文档句柄。	
 * @return 成功返回页个数，失败返回-1。
 *				
 */
int OFD_Document_CountPage(OFD_DOCUMENT hDocument);

/**
 * @brief 加载文档一页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			页序号（由0开始编号）。
 * @return 成功返回页句柄，失败返回NULL。
 *				
 */
OFD_PAGE OFD_Document_LoadPage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 关闭页。
 *
 * @param[in] hPage	页句柄。		
 * @return 无。
 *				
 */
void OFD_Page_Close(OFD_PAGE hPage);

/**
 * @brief 获取页面区域。
 *
 * @param[in] hPage		页句柄。	
 * @param[in] iBoxType	页面区域类型（值应为OFD_PAGE_BOXTYPE_PHYSICAL或OFD_PAGE_BOXTYPE_APPLICATION或OFD_PAGE_BOXTYPE_CONTENT或OFD_PAGE_BOXTYPE_BLEED）。
 * @return 成功返回页面区域，失败返回无效区域。
 *				
 */
OFD_RECT OFD_Page_GetPageArea(OFD_PAGE hPage, int iBoxType);

/**
 * @brief 渲染页面内容并编码成图像格式。
 *
 * @param[in]	hPage			页句柄。	
 * @param[in]	clip			裁剪区。	
 * @param[out]	dest_buf		输出的图像数据，注意返回地址由OFD_Image_ReleaseBuf释放。
 * @param[out]	dest_len		输出的图像数据字节数。
 * @return 成功返回页个数，失败返回-1。
 *				
 */
int OFD_Image_RenderPage(OFD_PAGE hPage, OFD_RECT* clip, OFD_LPBYTE* dest_buf, OFD_DWORD* dest_size);
int OFD_Image_RenderPageEx(OFD_PAGE hPage, OFD_RECT* clip, OFD_LPBYTE* dest_buf, OFD_DWORD* dest_size, int imgType, int dpi);
/**
 * @brief 释放图像数据内存。
 *
 * @param[in]	dest_buf		输出的图像数据。	
 * @return 无。
 *				
 */
void OFD_Image_ReleaseBuf(OFD_LPBYTE dest_buf);

/**
 * @brief 销毁包，并释放内存。
 *
 * @param[in]	hPackage	包句柄。		
 * @return 无。
 *				
 */
void OFD_Package_Destroy(OFD_PACKAGE hPackage);

/**
 * @brief 释放。
 *
 * @return 无。	
 *
 */
void OFD_Destroy();

#ifdef __cplusplus
}
#endif

#endif //_OFD_PAGE2IMAGE_H_
