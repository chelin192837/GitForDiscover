#ifndef _RAO_FONTMAP_H_
#define _RAO_FONTMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CPDFSDK_Annot;
class IFX_SystemHandler;
class CPDF_Document;
class CPDF_Dictionary;
//class CFSCRT_LTPDFAnnot;

class CBA_FontMap : public CPWL_FontMap
{
public:
	CBA_FontMap(FSAnnot* pAnnot, IFX_SystemHandler* pSystemHandler, CPDF_Document* pDocument);
//	CBA_FontMap(CPDF_Document* pDocument, CPDF_Dictionary* pAnnotDict, IFX_SystemHandler* pSystemHandler);

	virtual ~CBA_FontMap();

	virtual void				Initial(FX_LPCSTR fontname = NULL);

public:
	void						SetDefaultFont(CPDF_Font * pFont, const CFX_ByteString & sFontName);

	void						Reset();
	void						SetAPType(const CFX_ByteString& sAPType);
	CPDF_Font*					GetDefaultFont();

protected:
	virtual CPDF_Font*			FindFontSameCharset(CFX_ByteString& sFontAlias, FX_INT32 nCharset);
	virtual void				AddedFont(CPDF_Font* pFont, CFX_ByteString& sFontAlias);
	virtual CPDF_Document*		GetDocument();
private:
	CPDF_Font*					FindResFontSameCharset(CPDF_Dictionary* pResDict, CFX_ByteString& sFontAlias, CFX_ByteString sDAFontAlias,
									FX_INT32 nCharset);
	CPDF_Font*					GetAnnotDefaultFont(CFX_ByteString &csNameTag);
	void						AddFontToAnnotDict(CPDF_Font* pFont, CFX_ByteString& sAlias);
	CPDF_Dictionary*			GetAnnotAPFontList();
	void						GetAnnotOtherFontAndAddFontData();
	CPDF_Dictionary*			GetAnnotDRDict();

private:
	CPDF_Document*				m_pDocument;
	CPDF_Dictionary*			m_pAnnotDict;
	CPDF_Font*					m_pDefaultFont;
	CFX_ByteString				m_sDefaultFontName;
	
	CFX_ByteString				m_sAPType;
};

#endif // _RAO_FONTMAP_H_
