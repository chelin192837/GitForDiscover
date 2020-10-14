#ifndef _FPDFRICHTEXT_H_
#define _FPDFRICHTEXT_H_

#define PARAGRAPH_START	1
#define PARAGRAPH_END	2

class CPDF_RichTextXML
{
public:
	struct STYLE
	{
		CFX_WideString		wsTextAlign;				//left, right, and center
		FX_FLOAT			fVerticalAlign;				//<decimal number>pt
		FX_FLOAT			fFontSize;					//<decimal number>pt
		CFX_WideString		wsFontStyle;				//normal, italic
		CFX_WideString		wsFontWeight;				//normal, bold, 100, 200, 300, 400, 500, 600, 700, 800, 900.
		CFX_WideStringArray	wsArrayFontFamily;			//A font name or list of font names to be used to display the enclosed text. 
		FX_COLORREF			color;						//#rrggbb,  rgb(rrr,ggg,bbb) 
		CFX_WideString		wsTextDecoration;			//underline, line-through
		CFX_WideString		wsFontStretch;
		FX_FLOAT			fLineHeight;				//line_height, <decimal number>pt
		FX_FLOAT			fLetterSpacing;				//letter-spacing
		FX_FLOAT			fXFAFontHorizontalScale;	//xfa-font-horizontal-scale
		FX_INT32			nScriptType;
		FX_INT32			nPropertyUsed;				//which property is use : XXX_XXX_FLAG
		FX_BOOL				bRTL;

		enum PROPERTY_USED
		{
			TEXT_ALIGN_FLAG			= 0x01,
			VERT_ALIGN_FLAG			= 0x01 << 1,
			FONT_SIZE_FLAG			= 0x01 << 2,
			FONT_STYLE_FLAG			= 0x01 << 3,
			FONT_WEIGHT_FLAG		= 0x01 << 4,
			FONT_FAMILY_FLAG		= 0x01 << 5,
			TEXT_COLOR_FLAG			= 0x01 << 6,
			TEXT_DECORATION_FLAG	= 0x01 << 7,
			FONT_STRETCH_FLAG		= 0x01 << 8,
			LINE_HEIGHT_FLAG		= 0x01 << 10,
			TEXT_LETTERSPACE_FLAG	= 0x01 << 11,
			TEXT_HORIZSCALE_FLAG	= 0x01 << 12,
			SCRIPT_TYPE_FLAG		= 0x01 << 13
		};

		STYLE():
		wsTextAlign(L""),
		fVerticalAlign(0.0f),
		fFontSize(0.0f),
		wsFontStyle(L""),
		wsFontWeight(L""),
		color(0),
		wsTextDecoration(L""),
		wsFontStretch(L""),
		fLineHeight(0.0f),
		fLetterSpacing(0.0f),
		fXFAFontHorizontalScale(100.0f),
		nScriptType(0),
		nPropertyUsed(0),
		bRTL(FALSE)
		{
		}

		STYLE& operator = (STYLE& style)
		{
			wsTextAlign	= style.wsTextAlign;
			fVerticalAlign = style.fVerticalAlign;
			fFontSize = style.fFontSize;
			wsFontStyle = style.wsFontStyle;
			wsFontWeight = style.wsFontWeight;

			wsArrayFontFamily.Copy(style.wsArrayFontFamily);

			color = style.color;
			wsTextDecoration = style.wsTextDecoration;
			wsFontStretch = style.wsFontStretch;
			fLineHeight	= style.fLineHeight;
			fLetterSpacing = style.fLetterSpacing;
			fXFAFontHorizontalScale = style.fXFAFontHorizontalScale;
			nScriptType	= style.nScriptType;
			nPropertyUsed = style.nPropertyUsed;
			bRTL = style.bRTL;

			return *this;
		}
	};

	struct TEXT_BLOCK: CFX_Object
	{
		CFX_WideString	text;
		STYLE			style;
		FX_INT32		p;		//0: normal, 1: paragraph start, 2: paragraph end
		TEXT_BLOCK() :	p(0) 
		{
		}
	};

public:
	CPDF_RichTextXML();
	~CPDF_RichTextXML();

public:
	//TRUE means has parsed information from RC or DS.
	//FALSE means not information can be parsed from RC or DS, for example, they're both empty;
	// or any error occurs.
	FS_BOOL			SetXML(CFX_WideString wsXMLString, CFX_WideString wsDS = L"");
	//void			XMLToFXEdit(IFX_Edit* pEdit, IFX_Edit_FontMap* pFontProv, FX_BOOL bAutoSetAgin = TRUE);
	FS_BOOL			getFontName(FS_INT32 index, CFX_WideString& fontName);
	FS_BOOL			getFontSize(FS_FLOAT& fFontSize);
	FS_BOOL			getColor(FS_ARGB& color);
protected:
	void			ParseXML2Text();
	void			ParseXML2Text(CXML_Element* pXML, STYLE* pDefStyle,STYLE* pHanderStyle);
	FS_BOOL			FillStyle(CXML_Element* pXML, STYLE &style);
	void			FillStyle(CFX_WideString wsStyle, STYLE &style);

	void			ClearTextBlocks();
	FX_INT32		GetFontNameAndFontSize(const CFX_WideString &wsCSS,CFX_WideString & wsFontName,FX_FLOAT &fFontSize);


	FX_LPBYTE		m_pXMLBuf;
	int				m_bufSize;

	CFX_WideString	m_wsXFASpecVer; // A string that identifies the software used to generate the rich text string. software_name:software_version
	CFX_WideString	m_wsXFAAPIVer;  //The version of the XML Forms Architecture (XFA) specification to which the rich text string complies.

	STYLE			m_defaultStyle; //DS: default style

	FX_BOOL			m_bCurrentRTL;
	int				m_Beginalign;

	CFX_ArrayTemplate<TEXT_BLOCK*>	m_TextBlocks;
private:
	// return value is from PROPERTY_USED
	FS_DWORD	GetFontSize(const CFX_WideString &wsCSS,FX_FLOAT & fFontSize,int &nSta,int &nEnd);
	void		GetFontName(const CFX_WideString &sCSS,CFX_WideString & wsFontName);
};

#endif