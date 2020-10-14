#ifndef _FS_WATERMARK_H_
#define _FS_WATERMARK_H_

#include "../../fxcore.h"
#include "../../../include/common.h"

class FSPDFDoc;
class FSPDFPage;
class FSAnnot;

class IFX_Font;
class IFX_FontEx;
class CFSPDF_WatermarkInfo;

#ifndef _FSPDF_DEF_MACRO_WATERMARKPOS_
#define _FSPDF_DEF_MACRO_WATERMARKPOS_
/**
 * @name	Macro Definitions for Watermark Positions.
 */
/**@{*/

/** @brief	Watermark position: top left.*/
#define FSPDF_WATERMARKPOS_TOPLEFT			0
/** @brief	Watermark position: top center.*/
#define FSPDF_WATERMARKPOS_TOPCENTER		1
/** @brief	Watermark position: top right.*/
#define FSPDF_WATERMARKPOS_TOPRIGHT			2
/** @brief	Watermark position: center left.*/
#define FSPDF_WATERMARKPOS_CENTERLEFT		3
/** @brief	Watermark position: center.*/
#define FSPDF_WATERMARKPOS_CENTER			4
/** @brief	Watermark position: center right.*/
#define FSPDF_WATERMARKPOS_CENTERRIGHT		5
/** @brief	Watermark position: bottom left.*/
#define FSPDF_WATERMARKPOS_BOTTOMLEFT		6
/** @brief	Watermark position: bottom center.*/
#define FSPDF_WATERMARKPOS_BOTTOMCENTER		7
/** @brief	Watermark position: bottom right.*/
#define FSPDF_WATERMARKPOS_BOTTOMRIGHT		8

/**@}*/
#endif /* _FSPDF_DEF_MACRO_WATERMARKPOS_ */

#ifndef _FSPDF_DEF_MACRO_WATERMARKFLAG_
#define _FSPDF_DEF_MACRO_WATERMARKFLAG_
/**
 * @name	Macro Definitions for Watermark Setting Flags.
 */
/**@{*/

/** @brief	If set and insert a watermark as page content (default).*/
#define	FSPDF_WATERMARKFLAG_ASPAGECONTENTS	0
/** @brief	If set and insert a watermark as an annotation.*/
#define FSPDF_WATERMARKFLAG_ASANNOT			1
/** @brief	If set, show watermark above other page content.*/
#define FSPDF_WATERMARKFLAG_ONTOP			2
/** @brief	If set but do not print a watermark.*/
#define FSPDF_WATERMARKFLAG_NOPRINT			4
/** @brief	If set but do not display a watermark.*/
#define FSPDF_WATERMARKFLAG_INVISIBLE		8

/**@}*/
#endif /* _FSPDF_DEF_MACRO_WATERMARKFLAG_ */

#ifndef _FSPDF_DEF_MACRO_WATERMARK_FONTSTYLE_
#define _FSPDF_DEF_MACRO_WATERMARK_FONTSTYLE_
/**
 * @name	Macro Definitions for Watermark Text Font Style.
 */
/**@{*/

/** @brief	Watermark font style: normal.*/
#define FSPDF_WATERMARK_FONTSTYLE_NORMAL		0
/** @brief	Watermark font style: with underline.*/
#define FSPDF_WATERMARK_FONTSTYLE_UNDERLINE		1

/**@}*/
#endif /* _FSPDF_DEF_MACRO_WATERMARK_FONTSTYLE_ */

#ifndef _FSPDF_DEF_MACRO_WATERMARK_TEXTALIGNMENT_
#define _FSPDF_DEF_MACRO_WATERMARK_TEXTALIGNMENT_
/**
 * @name	Macro Definitions for a watermark alignment Mode.
 */
/**@{*/

/** @brief	Text watermark alignment: left.*/	
#define FSPDF_WATERMARK_TEXTALIGNMENT_LEFT		0
/** @brief	Text watermark alignment: center.*/
#define FSPDF_WATERMARK_TEXTALIGNMENT_CENTER	1
/** @brief	Text watermark alignment: right.*/	
#define FSPDF_WATERMARK_TEXTALIGNMENT_RIGHT		2

/**@}*/
#endif /* _FSPDF_DEF_MACRO_WATERMARK_TEXTALIGNMENT_ */

#ifndef _FSPDF_DEF_STRUCTURE_WATERMARK_SETTINGS_
#define _FSPDF_DEF_STRUCTURE_WATERMARK_SETTINGS_
typedef struct _FSPDF_WATERMARK_SETTINGS
{
	/** 
	 * @brief	Position mode. 
	 *
	 * @details	Please refer to macro definitions {@link FSPDF_WATERMARKPOS_TOPLEFT FSPDF_WATERMARKPOS_XXX} and this should be one of these macros.
	 */
	FS_DWORD 	position;
	/** @brief	Horizontal offset, in points. */
	FS_FLOAT 	offsetX;
	/** @brief	Vertical offset, in points. */
	FS_FLOAT 	offsetY;
	/** 
	 * @brief	Watermark flags. 
	 *
	 * @details	Please refer to macro definitions {@link FSPDF_WATERMARKFLAG_ASPAGECONTENTS FSPDF_WATERMARKFLAG_XXX} and this can be one or a combination of these macros.
	 */
	FS_DWORD 	flags;
	/** @brief	Horizontal scale coefficient. */	
	FS_FLOAT 	scaleX;
	/** @brief	Vertical scale coefficient. It should be greater than 0.001f. */	
	FS_FLOAT 	scaleY;
	/** @brief	Rotation angle in degrees. It should be greater than 0.001f. */	
	FS_FLOAT 	rotation;
	/** @brief	Opacity in percents. 0 for transparent and 100 for opaque. */
	FS_INT32 	opacity;
} FSPDF_WATERMARK_SETTINGS;
#endif /* _FSPDF_DEF_STRUCTURE_WATERMARK_SETTINGS_ */

#ifndef _FSPDF_DEF_STRUCTURE_WATERMARK_TEXTPROPERTIES_
#define _FSPDF_DEF_STRUCTURE_WATERMARK_TEXTPROPERTIES_
typedef struct _FSPDF_WATERMARK_TEXTPROPERTIES
{
	/** @brief	Font object used for text. Not support using the function ::FSCRT_Font_CreateFromFile to create font object currently.*/
	CPDF_Font*	pPDFFont;
	/** @brief	Font size in points. */
	FS_FLOAT 	fontSize;
	/** @brief	Text color. It is constructed by 0xAARRGGBB. Alpha component is ignored.*/
	FS_ARGB		color;
	/** 
	 * @brief	Font style, specifies if a text is underlined.
	 * 
	 * @details	It should be one of the following macro definitions: <br>
				<ul>
				<li>::FSPDF_WATERMARK_FONTSTYLE_NORMAL</li>
				<li>::FSPDF_WATERMARK_FONTSTYLE_UNDERLINE</li>
				</ul>
	*/
	FS_DWORD 	fontStyle;
	/** @brief	Space between vertical center of lines, in line heights.*/
	FS_FLOAT 	lineSpace;
	/** 
	 * @brief	Text horizontal alignment.
	 *
	 * @details	It should be one of the following macro definitions: <br>
				<ul>
				<li>::FSPDF_WATERMARK_TEXTALIGNMENT_LEFT</li>
				<li>::FSPDF_WATERMARK_TEXTALIGNMENT_CENTER</li>
				<li>::FSPDF_WATERMARK_TEXTALIGNMENT_RIGHT</li>
				</ul>
	*/
	FS_INT32 	alignment;
} FSPDF_WATERMARK_TEXTPROPERTIES;
#endif /* _FSPDF_DEF_STRUCTURE_WATERMARK_TEXTPROPERTIES_ */

typedef struct _FSPDF_WATERMARK_TEXTPIECE_FONTINFO
{
	FS_DWORD		nStartPos;
	FS_DWORD		nEndPos;
	IFX_FontEx*		pFontEx;
	FS_INT32		iEmbType;
} FSPDF_WATERMARK_TEXTPIECE_FONTINFO;

typedef CFX_ArrayTemplate<FSPDF_WATERMARK_TEXTPIECE_FONTINFO> FSPDF_WATERMARK_TEXTPIECE_FONTARRAY;

struct FSPDF_WATERMARK_PARAM{};
struct FSPDF_WATERMARK_TEXT_PARAM	:	public FSPDF_WATERMARK_PARAM {CFX_ByteString text; CFX_WideString wText; FSPDF_WATERMARK_TEXTPROPERTIES properties; /*FSPDF_WATERMARK_TEXTPIECE_FONTARRAY* fontarray;*/};
//struct FSPDF_WATERMARK_IMAGE_PARAM	:	public FSPDF_WATERMARK_PARAM {FSCRT_IMAGE image;};
//struct FSPDF_WATERMARK_BITMAP_PARAM	:	public FSPDF_WATERMARK_PARAM {FSCRT_BITMAP bitmap;};
//struct FSPDF_WATERMARK_PAGE_PARAM	:	public FSPDF_WATERMARK_PARAM {FSCRT_PAGE page;};

#define FSPDF_WATERMARK_TYPE_INVALID	0
#define FSPDF_WATERMARK_TYPE_TEXT		1
//#define FSPDF_WATERMARK_TYPE_IMAGE   2
//#define FSPDF_WATERMARK_TYPE_BITMAP  3
//#define FSPDF_WATERMARK_TYPE_PAGE    4

class CFSPDF_Watermark: public CFX_Object
{
public:
	explicit CFSPDF_Watermark(FSPDFDoc* pDoc);
	virtual ~CFSPDF_Watermark();

	FX_INT32  GetType(){return m_iWatermarkType;}
	//FSPDFPage*	GetPDFPage();

	FS_RESULT CreateFromText(CFX_ByteString text, const FSPDF_WATERMARK_TEXTPROPERTIES* properties, FS_BOOL bSDKWatermark = FALSE);
	//FS_RESULT CreateFromImage(FSCRT_IMAGE image);
	//FS_RESULT CreateFromBitmap(FSCRT_BITMAP bitmap);
	//FS_RESULT CreateFromPage(FSCRT_PAGE page);

	FS_RESULT InsertToPage(FSPDFPage* page, FS_BOOL bSDKWatermark = FALSE);
	FS_RESULT GetSize(FS_FLOAT* width, FS_FLOAT* height);
	CFSPDF_WatermarkInfo* GetWatermarkInfo();
	FS_RESULT UpdateSettings(const FSPDF_WATERMARK_SETTINGS* settings);

	FS_RESULT RegenerateSettingsXML();
	//FS_RESULT ST_RegenerateSettingsXML();
	//FS_RESULT ST_NOJMP_RegenerateSettingsXML(CFX_ByteString& xmlContent);

	FS_RESULT Release();

public:
	//static FS_RESULT CountPageObjWMFromPage(FSPDFPage* pPage, FS_INT32* count);
	//static FS_RESULT CountAnnotWMFromPage(FSPDFPage* pPage, FS_INT32* count);
	//static FS_RESULT RemoveAnnotWMFromPage(FSPDFPage* pPage);
	//static FS_RESULT RemovePageObjWMFromPage(FSPDFPage* pPage);

	//static FS_RESULT ST_CountPageObjWMFromPage(FSPDFPage* pPage, FS_INT32* count);
	//static FS_RESULT ST_RemovePageObjWMFromPage(FSPDFPage* pPage);

protected:
	FS_RESULT ST_CreateFromText(CFX_WideString wsText, const FSPDF_WATERMARK_TEXTPROPERTIES* properties, FS_BOOL bSDKWatermark = FALSE);
	//FS_RESULT ST_CreateFromImage(FSCRT_IMAGE image);
	//FS_RESULT ST_CreateFromBitmap(FSCRT_BITMAP bitmap);
	//FS_RESULT ST_CreateFromPage(FSCRT_PAGE page);
	//FS_RESULT ST_NOJMP_CharEmbedType(FS_WORD wch1, FS_WORD wch2);

protected:
	CFSPDF_Watermark();	//forbid accidental construction

protected:
	FSPDFDoc*							m_pFSPDFDoc;
	CFSPDF_WatermarkInfo*				m_pWatermarkInfo;

	FSPDF_WATERMARK_SETTINGS			m_settings;
	FX_INT32							m_iWatermarkType;

	struct FSPDF_WATERMARK_PARAM*		m_pWatermarkParam;
};




#endif //_FS_WATERMARK_H_