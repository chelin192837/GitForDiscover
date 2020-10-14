#ifndef _FS_OFDTEXTTYPESETTING_H_
#define _FS_OFDTEXTTYPESETTING_H_

#define FS_DEFLATE_UNIT 0.35f

class CFS_OFDTextTypesetting : public CFX_Object
{
public:
	CFS_OFDTextTypesetting();
	virtual ~CFS_OFDTextTypesetting();

	void SetStartOffset(FX_FLOAT x, FX_FLOAT y, FX_FLOAT lineSpacing, FX_FLOAT rtfFontSize, FX_INT32 iVerticalOrder);
	void CalcLinesByCharNum(const CFX_WideString& wsText, CFX_Font* pFont, FX_INT32 lineCharNum);
	void CalcLinesByBitNum(const CFX_WideString& wsText, CFX_Font* pFont, FX_INT32 lineBitNum);
	void CalcLinesByLineBreak(const CFX_WideString& wsText, CFX_Font* pFont, const CFX_WideString& lineBreak);
	void CalcLinesBySingleLine(const CFX_WideString& wsText, CFX_Font* pFont);
	virtual void SetBreakPage(FX_BOOL bBreakPage);
	virtual CFX_Int32Array GetFeedFromLineInfo();
	virtual void CalcLinesByAutoWrapandLineBreak(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing, const CFX_WideString& lineBreak) = 0;
	virtual void CalcLinesByAutoWrapandAutoFontSize(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT& fFontSize, FX_FLOAT charSpacing, FX_FLOAT lineSpacing, FX_FLOAT DeflateUnit = 0) = 0;
	virtual void CalcLinesByAutoWrap(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing) = 0;
	virtual void CalcLinesByAutoFontSize(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT& fFontSize, FX_FLOAT charSpacing) = 0;
	virtual void SetParameter(FX_INT32 iHAlign, FX_INT32 iVAlign, FX_BOOL bJustifiedX, FX_BOOL bJustifiedY, FX_INT32 iVerticalOrder = 0) = 0;
	virtual void CalcTextPiece(FX_FLOAT originx, FX_FLOAT originy, CFX_RectF layoutBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing, FX_FLOAT lineSpacing) = 0;

protected:
	virtual void CalcHAlign(CFX_RectF rBoudary, FX_FLOAT fLineWidth, FX_FLOAT& xy) = 0;
	virtual void CalcVAlign(CFX_RectF rBoudary, FX_FLOAT fTextHeight, FX_FLOAT& xy) = 0;
	virtual void CalcJustifiedX(CFX_RectF rBoudary, FX_FLOAT fLineWidth, FX_INT32 iTextLength, FX_FLOAT& fJustifiedOffsetX, FX_FLOAT& xy) = 0;
	//virtual void CalcJustifiedY(CFX_RectF rBoudary, FX_FLOAT fTextHeight, FX_FLOAT& xy) = 0;

public:
	void GetArrayGlyphWidth(const CFX_WideString& wsText, CFX_Font* pFont);
	FX_INT32 CalcUTF8ByteLen(FX_WCHAR c);
	FX_INT32 CalcANSIByteLen(FX_WCHAR c);
		
public:
	CFX_WideStringArray m_wsTextPerLine;
	FX_INT32 m_iLineCount;
	CFX_FloatArray m_fArrayX;
	CFX_FloatArray m_fArrayY;
	CFX_FloatArray m_fArrayJustifiedOffsetX;
	FX_FLOAT m_fJustifiedOffsetY;
	CFX_FloatArray m_fArrayGlyphWidth;

protected:
	FX_BOOL m_bGlyphWidthFlag;
	FX_BOOL m_bOffsetFlag;
	FX_FLOAT m_fStartOffsetX;
	FX_FLOAT m_fStartOffsetY;
	FX_FLOAT m_fLineSpacing;
	FX_FLOAT m_fRTFFontSize;
	FX_INT32 m_iVerticalOrder;
	FX_BOOL m_bWrapFlag;
	FX_BOOL m_bDeflateFlag;
	CFX_Int32Array	m_iArrayFeedLineInfo;
	FX_BOOL m_bBreakPage;
};

class CFS_OFDHorizontalTextTypesetting : public CFS_OFDTextTypesetting
{
public:
	CFS_OFDHorizontalTextTypesetting();
	virtual ~CFS_OFDHorizontalTextTypesetting();

	void CalcLinesByAutoWrapandLineBreak(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing, const CFX_WideString& lineBreak);
	void CalcLinesByAutoWrapandAutoFontSize(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT& fFontSize, FX_FLOAT charSpacing, FX_FLOAT lineSpacing, FX_FLOAT DeflateUnit = 0);
	void CalcLinesByAutoWrap(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing);
	void CalcLinesByAutoFontSize(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT& fFontSize, FX_FLOAT charSpacing);
	void SetParameter(FX_INT32 iHAlign, FX_INT32 iVAlign, FX_BOOL bJustifiedX, FX_BOOL bJustifiedY, FX_INT32 iVerticalOrder);
	void CalcTextPiece(FX_FLOAT originx, FX_FLOAT originy, CFX_RectF layoutBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing, FX_FLOAT lineSpacing);

protected:
	void CalcHAlign(CFX_RectF rBoudary, FX_FLOAT fLineWidth, FX_FLOAT& x);
	void CalcVAlign(CFX_RectF rBoudary, FX_FLOAT fTextHeight, FX_FLOAT& y);
	void CalcJustifiedX(CFX_RectF rBoudary, FX_FLOAT fLineWidth, FX_INT32 iTextLength, FX_FLOAT& fJustifiedOffsetX, FX_FLOAT& x);
	void CalcJustifiedY(CFX_RectF rBoudary, FX_FLOAT fTextHeight, FX_FLOAT& y);

protected:
	FX_INT32 m_iHAlign;
	FX_INT32 m_iVAlign;
	FX_BOOL m_bJustifiedX;
	FX_BOOL m_bJustifiedY;
};

class CFS_OFDVerticalTextTypesetting : public CFS_OFDTextTypesetting
{
public:
	CFS_OFDVerticalTextTypesetting();
	virtual ~CFS_OFDVerticalTextTypesetting();

	void CalcLinesByAutoWrapandLineBreak(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing, const CFX_WideString& lineBreak);
	void CalcLinesByAutoWrapandAutoFontSize(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT& fFontSize, FX_FLOAT charSpacing, FX_FLOAT lineSpacing, FX_FLOAT DeflateUnit = 0);
	void CalcLinesByAutoWrap(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing);
	void CalcLinesByAutoFontSize(const CFX_WideString& wsText, CFX_Font* pFont, CFX_RectF fBoundary, FX_FLOAT& fFontSize, FX_FLOAT charSpacing);
	void SetParameter(FX_INT32 iHAlign, FX_INT32 iVAlign, FX_BOOL bJustifiedX, FX_BOOL bJustifiedY, FX_INT32 iVerticalOrder);
	void CalcTextPiece(FX_FLOAT originx, FX_FLOAT originy, CFX_RectF layoutBoundary, FX_FLOAT fFontSize, FX_FLOAT charSpacing, FX_FLOAT lineSpacing);

protected:
	void CalcHAlign(CFX_RectF rBoudary, FX_FLOAT fLineWidth, FX_FLOAT& y);
	void CalcVAlign(CFX_RectF rBoudary, FX_FLOAT fTextHeight, FX_FLOAT& x);
	void CalcJustifiedX(CFX_RectF rBoudary, FX_FLOAT fLineWidth, FX_INT32 iTextLength, FX_FLOAT& fJustifiedOffsetX, FX_FLOAT& y);
	void CalcJustifiedY(CFX_RectF rBoudary, FX_FLOAT fTextHeight, FX_FLOAT fFontSize, FX_FLOAT& x);

protected:
	FX_INT32 m_iHAlign;
	FX_INT32 m_iVAlign;
	FX_BOOL m_bJustifiedX;
	FX_BOOL m_bJustifiedY;
//	FX_INT32 m_iVerticalOrder;
};

#endif //_FS_OFDTEXTTYPESETTING_H_
