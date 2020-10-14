#ifndef _OFD_BASE_R_H_
#define _OFD_BASE_R_H_

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

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

/**无符号整型（16位）*/
typedef unsigned short          OFD_WORD;

/**无符号整型（32位）*/
typedef unsigned int            OFD_DWORD;

/**字符（8位) */
typedef char					OFD_CHAR;

/**指向8位字符的指针*/
typedef OFD_CHAR *				OFD_LPSTR;

/**指向8位常字符的指针*/
typedef const OFD_CHAR *		OFD_LPCSTR;

/**编译器相关宽字符 Unicode character (16-bit for Microsoft Compiler, 32-bit for gcc).*/
typedef wchar_t					OFD_WCHAR;

/**指向宽字符的指针*/
typedef OFD_WCHAR *				OFD_LPWSTR;

/**指向常宽字符的指针*/
typedef const OFD_WCHAR *		OFD_LPCWSTR;

/** 字符串结构体 */
typedef struct _OFD_BSTR_ {
	/**字符串长度*/
	int	len;
	/**字符串指针*/
	OFD_LPSTR	str;
} OFD_BSTR;

/** 宽字符串结构体 */
typedef struct _OFD_WSTR_ {
	/**字符串长度*/
	int	len;
	/**字符串指针，Unicode字符(16-bit for Microsoft Compiler, 32-bit for gcc)*/
	OFD_LPWSTR	str;
	/**@}*/
} OFD_WSTR;

/**点坐标定义*/
typedef struct _OFD_POINT_ {
	float x;	//X坐标
	float y;	//Y坐标
} OFD_POINT;

/**矩形区域，以毫米为单位*/
typedef struct _OFD_RECT_ {
    float		x;			//起始点（左上点）x坐标
    float		y;			//起始点（左上点）y坐标
	float		width;		//宽度
	float		height;		//高度
} OFD_RECT;

/**CTM定义，用于矩阵变换*/
typedef struct _OFD_MATRIX_ {
	float a, b, c, d, e, f;
} OFD_MATRIX;

/**日期类型定义*/
typedef struct _OFD_DATETIME_ {
	short			year;		//four-digits year
	unsigned char	month;		//1 ~ 12
	unsigned char	day;		//1 ~ 31
	unsigned char	hour;		//0 ~ 23
	unsigned char	minute;		//0 ~ 59
	unsigned char	second;		//0 ~ 59
} OFD_DATETIME;

//跳转动作Action，Dest数据
typedef struct _OFD_DESTDATA_ {
	float left;		
	float right;
	float top;
	float bottom;
	float zoom;
} OFD_DESTDATA;


#define		OFD_DESTTP_Unknown	0
#define 	OFD_DESTTP_XYZ		1//left, top, zoom
#define 	OFD_DESTTP_Fit		2//fit page
#define 	OFD_DESTTP_FitH		3//fit width, top
#define 	OFD_DESTTP_FitV		4//fit height, left
#define 	OFD_DESTTP_FitR		5//fit rectangle, left, top, right, bottom


/**颜色空间定义,ARGB格式颜色*/
typedef OFD_DWORD	OFD_ARGB;
#define OFD_ARGB_ENCODE(a, r, g, b)	((b) | ((g)<<8) | ((r)<<16) | ((a)<<24))
#define OFD_ARGB_GETA(argb)			(((argb)>>24) & 0xff)
#define OFD_ARGB_GETR(argb)			(((argb)>>16) & 0xff)
#define OFD_ARGB_GETG(argb)			(((argb)>>8) & 0xff)
#define OFD_ARGB_GETB(argb)			((argb) & 0xff)

/**
 *	页面区域定义
 */
#define OFD_PAGE_BOXTYPE_BLEED			0		//出血框，超出物理设备限制的额外出血区域，默认为物理区域
#define OFD_PAGE_BOXTYPE_PHYSICAL		1		//物理区域
#define OFD_PAGE_BOXTYPE_APPLICATION	2		//显示区域，页面内容实际显示或打印输出区域
#define OFD_PAGE_BOXTYPE_CONTENT		3		//版心区域即文件正文区域
#define OFD_PAGE_BOXTYPE_CROP			4		//裁剪区域

/**
 *	图元对象类型定义
 */
#define OFD_PAGEOBJECT_TYPE_IMAGE           1		//图像对象
#define OFD_PAGEOBJECT_TYPE_PATH            2		//路径对象
#define OFD_PAGEOBJECT_TYPE_TEXT            3		//文字对象
#define	OFD_PAGEOBJECT_TYPE_VIDEO		    4		//视频对象
#define OFD_PAGEOBJECT_TYPE_COMPOSITE       5		//组合对象
#define OFD_PAGEOBJECT_TYPE_SVG			    6		//扩展图元对象
#define OFD_PAGEOBJECT_TYPE_PAGEBLOCK       7		//页面块对象

/**
 *	层类型定义
 */
#define OFD_LAYER_ZORDER_BACKGROUND		0		//背景层（默认）
#define OFD_LAYER_ZORDER_BODY			1		//正文层
#define OFD_LAYER_FOREGROUND			2		//前景层

/**
 *	线端点样式定义
 */
#define OFD_LINE_CAP_BUTT				0
#define OFD_LINE_CAP_ROUND				1
#define OFD_LINE_CAP_SQUARE				2

/**
 *	两个端点结合事样式定义
 */
#define OFD_LINE_JOIN_MITER				0
#define OFD_LINE_JOIN_ROUND				1
#define OFD_LINE_JOIN_BEVEL				2

/**
 *	图像类型定义
 */
#define OFD_IMAGE_FORMAT_BMP			1		//bmp格式
#define OFD_IMAGE_FORMAT_JPG			2		//jpg格式
#define OFD_IMAGE_FORMAT_PNG			3		//png格式
#define OFD_IMAGE_FORMAT_TIF			4		//tif格式
#define OFD_IMAGE_FORMAT_GIF			5		//gif格式
#define OFD_IMAGE_FORMAT_JP2			6		//jp2格式
#define OFD_IMAGE_FORMAT_TIFF			7		//tif格式
#define OFD_IMAGE_FORMAT_JB2			8		//jb2格式

/**
 *	路径填充模式定义
 */
#define OFD_PATHFILL_NONZERO			0		//非零填充
#define OFD_PATHFILL_EVENODD			1		//奇偶填充

/**
 *	元数据
 */
#define OFD_DOCUMENT_METADATA_AUTHOR			L"Author"
#define OFD_DOCUMENT_METADATA_CREATOR			L"Creator"
#define OFD_DOCUMENT_METADATA_CREATORVERSION	L"CreatorVersion"
#define OFD_DOCUMENT_METADATA_DOCUSAGE			L"DocUsage"
#define OFD_DOCUMENT_METADATA_DOCID				L"DocID"
#define OFD_DOCUMENT_METADATA_DOCUMENTID		L"DocumentID"
#define OFD_DOCUMENT_METADATA_TITLE				L"Title"
#define OFD_DOCUMENT_METADATA_SUBJECT			L"Subject"
#define OFD_DOCUMENT_METADATA_ABSTRACT			L"Abstract"
#define OFD_DOCUMENT_METADATA_CREATEDATE		L"CreationDate"
#define OFD_DOCUMENT_METADATA_MODDATE			L"ModDate"
#define OFD_DOCUMENT_METADATA_COVER				L"Cover"
#define OFD_DOCUMENT_METADATA_GONGWENYUYI		L"Gongwenyuyi"
#define OFD_DOCUMENT_METADATA_GONGWENBIAOSHI	L"Gongwenbiaoshi"
#define OFD_DOCUMENT_METADATA_WENZHONG			L"Wenzhong"

/**
 *	文字信息
 */
typedef struct _OFD_CHARINFO_ {
    OFD_DWORD	charcode;				//文字的UNICODE编码值
    float		origin_x;               //文字位置（左下点）：相对于图元对象boundary起始点的x坐标，单位mm
    float		origin_y;               //文字位置（左下点）：相对于图元对象boundary起始点的y坐标，单位mm
} OFD_CHARINFO;

/**
 *	字形变换，当存在字形变换时， TextCode 对象中使用字形变换节点（ CGTransform） 描述字符编
 *	码和字形索引之间的关系
 */
typedef struct _OFD_CGTRANSFORM_ {
	OFD_DWORD	start_pos;				//TextCode 中字符编码的起始位置，从 0 开始
	int			code_count;             //变换关系中字符的数量，该数值应大于或等于 1
	OFD_DWORD*	glyphs;                 //变换后的字形索引数组
	int			glyph_count;            //变换关系中字形索引的个数，该数值应大于或等于 1
} OFD_CGTRANSFORM;

/**
 *	文字信息(字符块)
 */
typedef struct _OFD_TEXTPIECEINFO_ {
	OFD_DWORD*	charcode;				//文字的UNICODE编码值数组
	int			len;					//数组长度
	float		origin_x;               //文字位置（左下点）：相对于图元对象boundary起始点的x坐标，单位mm
	float		origin_y;               //文字位置（左下点）：相对于图元对象boundary起始点的y坐标，单位mm
	float*		deltaX;					//代表后一个文字与前一个文字之间在X方向的偏移值，单位mm
	float*		deltaY;					//代表后一个文字与前一个文字之间在Y方向的偏移值，单位mm
	OFD_CGTRANSFORM* cg_transform;		//字形变换数组，如果不需要指定字形变换设置为NULL
	int			cg_num;					//字形变换数组长度
} OFD_TEXTPIECEINFO;

/**
 *	OFD_AxialShd轴向渐变填充绘制方式
 */
#define OFD_SHADINGMAP_DIRECT			0			//直接填充
#define	OFD_SHADINGMAP_REPEAT			1			//重复填充
#define	OFD_SHADINGMAP_REFLECT			2			//映射填充

/**底纹填充单元*/
typedef  struct _OFD_PATTERN_ {
	float					width;			//底纹单元绘制区域的宽度(单位毫米)
	float					height;			//底纹单元绘制区域的高度(单位毫米)
	float					xStep;			//X方向底纹单元间距(默认为fWidth，单位毫米)
	float					yStep;			//Y方向底纹单元间距(默认为fHeight,单位毫米)
	OFD_MATRIX				matrix;			//底纹单元变换矩阵
	OFD_LPCWSTR				imageFileName;	//指定单元图像文件名称
	OFD_LPBYTE				imageBuf;       //指定单元图像数据
	int						imageBufLen;	//图像数据长度
	int						imageFormat;	//图像数据类型（取值范围OFDSDK_IMAGE_FORMAT_BMP，OFDSDK_IMAGE_FORMAT_JPG，OFDSDK_IMAGE_FORMAT_PNG，OFDSDK_IMAGE_FORMAT_TIF）
} OFD_PATTERN;

/**颜色段*/
typedef struct _OFD_SEGMENT_ {
	float					position;		//各颜色位置值
	OFD_ARGB				color;			//颜色
} OFD_SEGMENT;

/**轴向渐变填充*/
typedef struct _OFD_AXIALSHD_ {
	OFD_POINT				startPoint;		//起始点坐标(单位毫米)
	OFD_POINT				endPoint;		//结束点坐标(单位毫米)
	OFD_ARGB				startColor;		//起始颜色
	OFD_ARGB				endColor;		//结束颜色
	OFD_SEGMENT*			segs;			//颜色段
	int						segCount;		//颜色段数
	int						mapType;		//渐变绘制的方式（取值范围OFDSDK_SHADINGMAP_DIRECT，OFDSDK_SHADINGMAP_REPEAT，OFDSDK_SHADINGMAP_REFLECT）
	int						mapUnit;		//当mapType值不为Direct时，表示轴线上一个渐变区间的长度(单位毫米)
	int						extend;			//轴延伸方向是否继续绘制渐变。0：不向两侧继续绘制渐变；1：在结束点至起始点延长线方向绘制渐变；2：在起始点至结束点延长线方向绘制渐变；
											//3：向两侧延长线方向绘制渐变。	默认值为0
} OFD_AXIALSHD;

/**径向渐变填充*/
typedef struct _OFD_RADIALSHD_ {
	OFD_POINT				startPoint;		//起始点坐标(单位毫米)
	OFD_POINT				endPoint;		//结束点坐标(单位毫米)
	float					startRadius;
	float					endRadius;
	float					eccentricity;
	float					angle;
	OFD_ARGB				startColor;		//起始颜色
	OFD_ARGB				endColor;		//结束颜色
	OFD_SEGMENT*			segs;			//颜色段
	int						segCount;		//颜色段数
	int						mapType;		//渐变绘制的方式（取值范围OFDSDK_SHADINGMAP_DIRECT，OFDSDK_SHADINGMAP_REPEAT，OFDSDK_SHADINGMAP_REFLECT）
	int						mapUnit;		//当mapType值不为Direct时，表示轴线上一个渐变区间的长度(单位毫米)
	int						extend;			//轴延伸方向是否继续绘制渐变。0：不向两侧继续绘制渐变；1：在结束点至起始点延长线方向绘制渐变；2：在起始点至结束点延长线方向绘制渐变；
											//3：向两侧延长线方向绘制渐变。	默认值为0
} OFD_RADIALSHD;

typedef enum _OFD_PATHPTTYPE
{
	OFD_PATHPTTYPE_Normal = 0,
	OFD_PATHPTTYPE_Start,
	OFD_PATHPTTYPE_MoveTo,
	OFD_PATHPTTYPE_LineTo,
	OFD_PATHPTTYPE_QuadraticBezierTo,
	OFD_PATHPTTYPE_CubicBezierTo,
	OFD_PATHPTTYPE_ArcTo,
	OFD_PATHPTTYPE_Close,
}OFD_PATHPTTYPE;

typedef struct _OFD_PATHPT
{
	float			x;
	float			y;
	OFD_PATHPTTYPE	t;
}OFD_PATHPT;

/**
 *	OFD优化参数定义
 */
typedef struct _OFD_OPTIMIZERPARAM
{
	float			DownSample;				//图片降采样
	float			BootstrapDownSample;    //缩略图图片降采样
	int				MRCCompressLevel;		//图片MRC压缩等级，取值0~9。0：不进行压缩
	int				MRCCompressCodecType;	//图片MRC压缩格式。0：JPEG2000，1：JPEG
	int				FloatBit;				//浮点数有效位数，取值0~5。0：不进行有效位数优化
	OFD_BOOL		LZMACompressFlag;		//LZMA压缩标志位
	OFD_BOOL		RESOptimizerFlag;		//资源优化

	_OFD_OPTIMIZERPARAM()
	{
		DownSample = 1.0f;
		BootstrapDownSample = 1.0f;
		MRCCompressLevel = 0;
		MRCCompressCodecType = 0;
		FloatBit = 0;
		LZMACompressFlag = 0;
		RESOptimizerFlag = 0;
	}
}OFD_OPTIMIZERPARAM;

/**
 *	动作类型定义
 */
#define OFD_ACTIONTYPE_UNKNOWN			0
#define OFD_ACTIONTYPE_GOTO				1
#define OFD_ACTIONTYPE_Uri				2
#define OFD_ACTIONTYPE_SOUND			3
#define OFD_ACTIONTYPE_MOVIE			4
#define OFD_ACTIONTYPE_GOTOA			5

/**
 *	视频操作类型
 */
#define	OFD_MOVIE_OPERATOR_Play		0
#define	OFD_MOVIE_OPERATOR_Stop		1
#define	OFD_MOVIE_OPERATOR_Pause	2
#define	OFD_MOVIE_OPERATOR_Resume	3

/**
 *	渲染状态定义
 */
#define OFD_RENDERSTATE_Reset		0	//ready to start rendering
#define OFD_RENDERSTATE_Begin		1   //begin render
#define OFD_RENDERSTATE_Paused		2	//paused
#define OFD_RENDERSTATE_Done		3	//done
#define OFD_RENDERSTATE_Failed		4	//error

/**
 *	注释类型定义
 */
#define	OFD_ANNOTTYPE_UNKNOWN		0
#define	OFD_ANNOTTYPE_LINK			1
#define	OFD_ANNOTTYPE_PATH			2
#define	OFD_ANNOTTYPE_HIGHLIGHT		3
#define	OFD_ANNOTTYPE_STAMP			4
#define	OFD_ANNOTTYPE_WATERMARK		5

/**
 *	加密方式定义
 */
#define OFD_ENCRYPTMETHOD_NONE		0
#define OFD_ENCRYPTMETHOD_PWD		1
#define OFD_ENCRYPTMETHOD_CERT		2

/**
 *	加密算法类型定义
 */
#define OFD_ENCRYPTTYPE_NONE		0
#define OFD_ENCRYPTTYPE_RC4			1
#define OFD_ENCRYPTTYPE_AES128		2
#define OFD_ENCRYPTTYPE_AES256		4
#define OFD_ENCRYPTTYPE_SM4			5


/**
 *	授权值定义
 */
#define OFD_LICENSE_OK				0
#define OFD_LICENSE_DIR				1		//授权文件夹不存在
#define OFD_LICENSE_WRITE			2		//没有授权文件夹写权限
#define OFD_LICENSE_SERIAL			3		//序列号不正确
#define OFD_LICENSE_NET				4		//服务器通讯错误
#define OFD_LICENSE_SERIALEXPIRED	5		//序列号过期
#define OFD_LICENSE_EXCEED			6		//超过授权次数
#define OFD_LICENSE_EXPIRED			7		//授权文件过期
#define OFD_LICENSE_FILE			8		//授权文件篡改
#define OFD_LICENSE_HARDWARE		9		//授权设备变更
#define OFD_LICENSE_VERSION			10		//授权版本过低
#define OFD_LICENSE_SERIAL_PRODUCT	31		//序列号错误，产品名称不正确
#define OFD_LICENSE_SERIAL_MAJOR	32		//序列号错误，主版本号不正确
#define OFD_LICENSE_SERIAL_CPU		33		//序列号错误，CPU不正确
#define OFD_LICENSE_SERIAL_SERVER	34		//序列号错误，授权服务返回错误

/**
 *	目录页面区域定义
 */
#define OFD_CATALOG_TITLE_SECTION		0		//目录标题
#define OFD_CATALOG_CONTENT_SECTION		1		//目录内容
#define OFD_ATTACHMENT_TITLE_SECTION	2		//附件标题
#define OFD_ATTACHMENT_CONTENT_SECTION	3		//附件内容

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

/**层句柄，包含多个图元对象*/
OFD_DEFINEHANDLE(OFD_LAYER);

/**图元对象句柄，可以为Text，Path，Image等*/
OFD_DEFINEHANDLE(OFD_PAGEOBJECT);

/**大纲节点句柄*/
OFD_DEFINEHANDLE(OFD_OUTLINE);

/**书签数组句柄，包含多个书签*/
OFD_DEFINEHANDLE(OFD_BOOKMARKS);

/**书签句柄*/
OFD_DEFINEHANDLE(OFD_BOOKMARK);

/**自定义标签数组句柄，包含多个标签*/
OFD_DEFINEHANDLE(OFD_CUSTOMTAGS);

/**公文语义节点句柄*/
OFD_DEFINEHANDLE(OFD_OFFICENODE);

/**自定义标签句柄*/
OFD_DEFINEHANDLE(OFD_CUSTOMTAG);

/**附件数组句柄，包含多个附件*/
OFD_DEFINEHANDLE(OFD_ATTACHMENTS);

/**附件句柄*/
OFD_DEFINEHANDLE(OFD_ATTACHMENT);

/**字符串查找句柄*/
OFD_DEFINEHANDLE(OFD_SEARCH);

/**字体句柄*/
OFD_DEFINEHANDLE(OFD_FONT);

/**裁剪区句柄*/
OFD_DEFINEHANDLE(OFD_CLIPREGION);

/**裁剪句柄*/
OFD_DEFINEHANDLE(OFD_CLIP);

/**裁剪区域句柄*/
OFD_DEFINEHANDLE(OFD_CLIPAREA);

/**渲染器句柄*/
OFD_DEFINEHANDLE(OFD_RENDERER);

/**渲染上下文句柄*/
OFD_DEFINEHANDLE(OFD_RENDERCONTEXT);

/**步进式渲染句柄*/
OFD_DEFINEHANDLE(OFD_PROGRESS);

/**页面注释句柄*/
OFD_DEFINEHANDLE(OFD_PAGEANNOTS);

/**注释句柄*/
OFD_DEFINEHANDLE(OFD_ANNOT);

/**块对象句柄*/
OFD_DEFINEHANDLE(OFD_BLOCKOBJECT);

/**位图对象句柄*/
OFD_DEFINEHANDLE(OFD_BITMAP);

/**水印辅助对象句柄*/
OFD_DEFINEHANDLE(OFD_WATERMARKHELPER);

/**文档权限对象句柄*/
OFD_DEFINEHANDLE(OFD_PERMISSIONS);

/**图形数据对象句柄*/
OFD_DEFINEHANDLE(OFD_PATH);

/**动作数组句柄，包含多个动作*/
OFD_DEFINEHANDLE(OFD_ACTIONS);

/**动作对象句柄*/
OFD_DEFINEHANDLE(OFD_ACTION);

/**安全对象句柄*/
OFD_DEFINEHANDLE(OFD_SECURITYHANDLE);

/**OFD规范性检测错误句柄*/
OFD_DEFINEHANDLE(OFD_ERRORINFO);

/**OFD规范性xml和xsd对应信息句柄*/
OFD_DEFINEHANDLE(OFD_XMLXSDINFO);

/**OFD行字符串数组句柄*/
OFD_DEFINEHANDLE(OFD_TEXTOFLINE)

/**OFD优化句柄*/
OFD_DEFINEHANDLE(OFD_OPTIMIZER);

/**OFD复合对象句柄*/
OFD_DEFINEHANDLE(OFD_COMPOSITE);

/**OFD复合对象单元句柄*/
OFD_DEFINEHANDLE(OFD_COMPOSITEUNIT);

/**OFD生成的目录句柄*/
OFD_DEFINEHANDLE(OFD_CATALOG);

/**OFD命名空间数组句柄*/
OFD_DEFINEHANDLE(OFD_NAMESPACE)

/**
 *字符串函数定义
 */
/**
 * @brief 初始化字符串结构体。
 *
 * @param[in]	bstr		字符串结构体指针。
 * @return 无。
 *
 */
void		OFD_BStr_Init(OFD_BSTR* bstr);

/**
 * @brief 释放字符串。
 *
 * @param[in]	bstr		字符串结构体指针。	
 * @return 无。
 *
 */
void		OFD_BStr_Release(OFD_BSTR* bstr);

/**
 * @brief 初始化宽字符串结构体。
 *
 * @param[in]	wstr		宽字符串结构体指针。
 * @return 无。
 *
 */
void		OFD_WStr_Init(OFD_WSTR* wstr);

/**
 * @brief 释放宽字符串。
 *
 * @param[in]	wstr		宽字符串结构体指针。	
 * @return 无。
 *
 */
void		OFD_WStr_Release(OFD_WSTR* wstr);

/**
 * @brief UTF8编码。
 *
 * @param[in]	wstr		宽字符串结构体指针。	
 * @param[out]	bstr		字符串结构体指针。	
 * @return 无。
 *
 */
void		OFD_WStr_UTF8Encode(OFD_WSTR* wstr, OFD_BSTR* bstr);

/**
 * @brief UTF8解码。
 *
 * @param[in]	bstr		字符串结构体指针。	
 * @param[out]	wstr		宽字符串结构体指针。	
 * @return 无。
 *
 */
void		OFD_BStr_UTF8Decode(OFD_BSTR* bstr, OFD_WSTR* wstr);

/**
 * @brief 使用矩阵变化点坐标。
 *
 * @param[in]	matrix		变换矩阵。	
 * @return 点坐标。	
 *
 */
OFD_POINT		OFD_Matrix_TransformPoint(OFD_MATRIX matrix, OFD_POINT pt);

/**
 * @brief 计算矩阵逆矩阵。
 *
 * @param[in]	matrix		原始矩阵。	
 * @return 逆矩阵。	
 *
 */
OFD_MATRIX		OFD_Matrix_Reverse(OFD_MATRIX matrix);

/**
 * @brief 解析时间日期字符串。
 *
 * @param[in]	lpwszDateTime		日期字符串。	
 * @return 时间日期结构体。
 *
 */
OFD_DATETIME OFD_FromParseDateTimeString(OFD_LPCWSTR lpwszDateTime);

/**
 * @brief 初始化。
 *
 * @param[in]	license_id		序列号。	
 * @param[in]	license_path	授权文件路径。
 * @return 成功返回0；失败返回-1。见OFD_LICENSE_XXX	
 *
 */
int		OFD_Init(OFD_LPSTR license_id, OFD_LPSTR license_path);

/**
 * @brief 释放。
 *
 * @return 无。	
 *
 */
void		OFD_Destroy();

int			OFD_SetLibraryPath(OFD_LPSTR work_path);

/**
 * @brief 返回Library地址。
 *
 * @param[in]	data			返回Library地址。	
 * @return 无。
 *				
 */
void		OFD_GetLibraryPath(OFD_WSTR* data);

#ifdef __cplusplus
}
#endif

#endif  //#define _OFD_BASE_R_H_
