#ifndef _FS_OFDTEXTOBJECT_H_
#define _FS_OFDTEXTOBJECT_H_

class CFS_OFDContentObject;
class CFS_OFDTextObject;
class CFS_OFDLayer;
class CFS_OFDCharInfoArray;
class CFS_OFDTextTypesetting;
class CFS_OFDHorizontalTextTypesetting;

typedef struct deltaInfo{
	float* deltax;
	float* deltay;
	float flineWidth;
}*pDeltaInfo;

typedef struct _fs_surrogatepair_info{
	FX_INT32 position;
	FX_WCHAR highSurrogate;
	FX_WCHAR lowSurrogate;
	FX_FLOAT x;
	FX_FLOAT y;
}surrogatePairInfo;

typedef CFX_ArrayTemplate<deltaInfo>	CFX_DeltaInfoArray;
typedef CFX_ArrayTemplate<surrogatePairInfo>	CFX_SurrogatePairInfoArray;

class CFS_OFDTextLayout : public CFX_Object
{
public:
	CFS_OFDTextLayout(CFS_OFDTextObject* pUser = NULL);
	virtual ~CFS_OFDTextLayout();

	virtual void Update();
	void SurrogatePair_Update();

	void SetText(const CFX_WideString& wsText);
	void SetTextStartOffset(FX_FLOAT x, FX_FLOAT y);
	void SetFontName(const CFX_WideString& wsFontName);
	void SetSubFontName(const CFX_WideString& wsSubFontName);
	void SetFontSize(FX_FLOAT fontSize);
	void SetWeight(FX_INT32 nWeight);
	void SetItalic(FX_BOOL bItalic);
	void AddUnderLine();
	void SetRTFFontsize(FX_FLOAT fontSize);
	void SetHAlignment(FX_INT32 iAlign);
	void SetVAlignment(FX_INT32 iAlign);
	void SetJustifiedX(FX_BOOL bSet);
	void SetJustifiedY(FX_BOOL bSet);
	void SetMultiline(FX_BOOL bMultiline);
	void SetLineCharNum(FX_INT32 iLineCharNum);
	void SetLineBitNum(FX_INT32 iLineBitNum);
	void SetCharSpace(FX_FLOAT charSpace);
	void SetLineSpace(FX_FLOAT lineSpace);
	void SetLineBreak(const CFX_WideString& wsBreak);
	void SetRotate(FX_INT32 iRotate);

	void SetAutoMultiline(FX_BOOL bAuto);
	void SetAutoFontSize(FX_BOOL bAuto);
	void SetVerticalType(FX_BOOL bType);
	void SetVerticalOrder(FX_INT32 iOrder);
	void SetDeflateUnit(FX_FLOAT fDeflateUnit);

	void GetTextEndOffset(FX_FLOAT& x, FX_FLOAT& y);
	void SetPageLinesCount(FX_INT32 iPageLineCount);
	void CalcTextLinesInfo(const CFX_RectF& rectf, FX_BOOL bBreakPage);
	CFX_Int32Array CalcPageLinesInfo();
	FX_INT32 GetTextLineCount() const;
	CFX_WideString GetOneTextPiece(FX_INT32 index) const;
	CFX_FloatArray GetOneTextPieceDeltaX(FX_INT32 index) const;

	FX_BOOL GetSurrogatePairFlag();
	CFX_SurrogatePairInfoArray GetSurrogatePairInfo() const;
	FX_BOOL CopySurrogatePairInfo(const CFX_SurrogatePairInfoArray& src);
	CFX_WideString GetFontName() const;
	CFX_WideString GetSubFontName() const;

protected:
	virtual void Layout(COFD_WriteTextObject* pWriteTextObj, CFX_Font* pFont);
	void SurrogatePair_Layout(COFD_WriteTextObject* pWriteTextObj, CFX_Font* pFont);

	void GetLayoutBoundary(COFD_WriteTextObject* pWriteTextObj);
	void CalcLinesByParameter(COFD_WriteTextObject* pWriteTextObj, CFX_Font* pFont);
	void DoLayout_Horizontal(COFD_WriteTextObject* pWriteTextObj, CFX_Font* pFont);
	void DoLayout_Vertical(COFD_WriteTextObject* pWriteTextObj, CFX_Font* pFont);
	void DoLayout_Rotate(COFD_WriteTextObject* pWriteTextObj);

	CFX_Int32Array CalcSurrogatePairCount();
	void CalcSurrogatePairTextPiece_Horizontal(FX_INT32 line_start, CFX_WideString& wsText, CFX_FloatArray& arrayDeltaXs, 
		FX_FLOAT& resx, FX_FLOAT resy, FX_INT32& sugIndex);
	void CalcSurrogatePairTextPiece_Vertical(FX_INT32 line_start, CFX_WideString& wsText, CFX_FloatArray& arrayDeltaXs, 
		FX_FLOAT resx, FX_FLOAT& resy, FX_INT32& sugIndex);

	CFX_Font* GetFont();
	FX_DWORD GetFontID();

	FX_FLOAT CalcLineWidth(const CFX_WideString& wsText, CFX_Font* pFont, FX_FLOAT* DeltaXs, FX_BOOL bUseCharSpace = TRUE);
	FX_BOOL CalcWrap(CFX_Font* pFont, IFX_FontEncoding* pFontEncoding, FX_WCHAR c, FX_FLOAT boudary_width, FX_FLOAT& cur_x, FX_FLOAT& fGlyphWidth);
	FX_FLOAT Round2(FX_FLOAT a);
	FX_BOOL CanAbbreviteDeltax(float* DeltaXs, int count, float fJustifiedOffsetX, CFX_Int32Array& abbrPair);
	void SetAbbreviteDeltax(COFD_WriteTextPiece* pWriteTextPiece, float* DeltaXs, int count, float fJustifiedOffsetX, const CFX_Int32Array& abbrPair);

public:
	void GenerateTextPiece(COFD_WriteTextObject* pWriteTextObj, float x, float y, float* Deltax, float* Deltay);
	FX_FLOAT CalcLineWidth(const CFX_WideString& wsText, const CFX_WideString& wsFontName, FX_FLOAT fontSize, FX_FLOAT* DeltaXs = NULL);
	OFD_RECT CalcLineWidth1(const CFX_WideString& wsText, const CFX_WideString& wsFontName, FX_FLOAT fontSize);
	OFD_RECT CalcMultiLineWidth1(const CFX_WideString& wsText, const CFX_WideString& wsFontName, FX_FLOAT fontSize, FX_FLOAT lineWidth);
	OFD_RECT CalcMultiLineWidth2(const CFX_WideString& wsText, const CFX_WideString& wsFontName, FX_FLOAT fontSize, FX_FLOAT lineWidth, FX_INT32* pLineCount = 0);
	FX_INT32 CalcUTF8BitLen(FX_WCHAR c);
	FX_BOOL CalcGlyphBox(CFX_WideString& wsText, const CFX_WideString& wsFontName, CFX_Matrix matrix, 
		FX_FLOAT fontSize, FX_FLOAT start_x, FX_FLOAT start_y, float* deltax, int cDeltax, CFX_RectFArray& boxArray);
	FX_BOOL ExistSurrogatePair(const CFX_WideString& wsText);
	CFX_WideString CalcTextWithSurrogatePair(const CFX_WideString& wsText);
protected:
	CFX_WideString m_text;
	CFX_WideString m_fontName;
	CFX_WideString m_subfontName;
	FX_FLOAT m_fontSize;
	FX_INT32 m_nWeight;
	FX_BOOL m_bItalic;
	FX_BOOL m_bUnderLine;
	FX_FLOAT m_RTFfontSize;
	FX_INT32 m_iHAlign;
	FX_INT32 m_iVAlign;
	FX_BOOL m_bJustifiedX;
	FX_BOOL m_bJustifiedY;
	FX_BOOL m_bMultiline;
	FX_INT32 m_lineCharNum;
	FX_INT32 m_lineBitNum;
	CFX_WideString m_lineBreak;
	FX_FLOAT m_lineSpacing;
	FX_FLOAT m_charSpacing;
	FX_INT32 m_Rotate;
	CFX_RectF m_LayoutBoundary;

	FX_BOOL m_bAutoFontSize;
	FX_BOOL m_bRound2;
	FX_BOOL m_bVerticalType;
	FX_INT32 m_iVerticalOrder;
	FX_BOOL m_OffsetFlag;
	FX_FLOAT m_startOffsetX;
	FX_FLOAT m_startOffsetY;
	FX_FLOAT m_endOffsetX;
	FX_FLOAT m_endOffsetY;
	FX_FLOAT m_fDeflateUnit;
	FX_BOOL m_bSurrogatePairFlag;
	CFX_SurrogatePairInfoArray m_arraySurrogatePairInfo;
	FX_INT32 m_iPageLineCount;

	CFS_OFDTextObject* m_pUser;

	CFS_OFDTextTypesetting* m_pTextTypesetting;

	CFX_FloatArray fLenArr;

friend class CFS_OFDTextObject;
};

class CFS_OFDTextObject : public CFS_OFDContentObject
{
public:
	CFS_OFDTextObject();
	virtual ~CFS_OFDTextObject();

	void SetFont(const CFX_WideString& wsName);
	void SetFont(FX_DWORD dwFontID);
	void SetFontSize(FX_FLOAT fFontSize);
	void SetHorizontalScale(FX_FLOAT fHScale);
	void SetReadDirection(FX_INT32 nRD);
	void SetCharDirection(FX_INT32 nCD);
	void SetWeight(FX_INT32 iWeight);
	void SetItalic(FX_BOOL bSet);
	void SetFillState(FX_BOOL bSet);
	void SetStrokeState(FX_BOOL bSet);
	void SetCharInfo(FX_WCHAR charCode, FX_FLOAT x, FX_FLOAT y);

	// 为航信电子发票开发，针对文字记述deltax，nLabel==0 如果wsText宽度超过 boundary的宽度则直接返回
	// nLabel == 1 如果wsText宽度超过 boundary的宽度，继续生成 textpiece
	FX_BOOL SetText(const CFX_WideString& wsText, FX_FLOAT fX, FX_FLOAT fY, FX_INT32 nLabel = 0);

	void SetTextPieceInfo(OFD_TEXTPIECEINFO* pTextPieceInfos);
	void SetTextPieceInfo(FX_FLOAT x, FX_FLOAT y, CFX_WideString& deltax, CFX_WideString& wsText);

	//Test
	void ReplaceTextPieceInfo(OFD_CHARINFO* pCharInfos, int count);

	void GetFont(CFX_WideString& wsName);
	FX_FLOAT GetFontSize();
	FX_BOOL IsStroke();
	FX_BOOL IsFill();
	FX_FLOAT GetHorizontalScale();
	FX_INT32 GetReadDirection();
	FX_INT32 GetCharDirection();
	FX_INT32 GetWeight();
	FX_BOOL IsItalic();
	void GetCharInfo(OFD_CHARINFO* pCharInfos, int *iCount);

	CFS_OFDTextLayout* GetLayout();

	void CopyTextObj(CFS_OFDTextObject* pSrcTextObj, FX_BOOL bCopyTextPieceFlag = TRUE);
protected:
	virtual OFD_CONTENTTYPE GetWriteContentType();
	COFD_Font* GetFont();
	void AppendCharInfo(const COFD_TextPiece* pTextPiece, CFS_OFDCharInfoArray& arr);

protected:
	FX_WCHAR m_CharCode;

	friend class CFS_OFDTextLayout;
	CFS_OFDTextLayout* m_pTextLayout;

	CFX_WideString m_wsFontName;
};

COFD_Font* FS_GetOFDFont(IOFD_Document* pDoc, COFD_TextObject* pTextObj);
CFX_WideString FS_GetOFDFontName(IOFD_Document* pDoc, COFD_TextObject* pTextObj);
CFX_Font* FS_GetGEFont(CFX_WideString& wsFontName);

#endif //_FS_OFDTEXTOBJECT_H_
