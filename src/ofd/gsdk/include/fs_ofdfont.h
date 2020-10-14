#ifndef _FS_OFDFONT_H_
#define _FS_OFDFONT_H_

//
class CFS_OFDDocument;

//
class CFS_OFDFont;


class CFS_OFDFont : public CFX_Object
{
public:
	CFS_OFDFont();
	~CFS_OFDFont();

	FX_BOOL Create(CFS_OFDDocument* pOFDDoc, COFD_WriteFont* pWriteFont);

	void 		SetFontName(FX_WSTR wsFontName);
	void		SetFamilyName(FX_WSTR wsFamilyName);
	void		SetBold(FX_BOOL bSet);
	void		SetSerif(FX_BOOL bSet);
	void		SetItalic(FX_BOOL bSet);
	void		SetFixedWidth(FX_BOOL bSet);
	void 		SetCharset(FX_WSTR wsCharset);
	FX_BOOL		SetFontFile(IOFD_FileStream* pFontFile);

	FX_DWORD	GetID();

protected:
	CFS_OFDDocument* m_pOFDDoc;
	COFD_WriteFont* m_pWriteFont;
};

#endif //_FS_OFDFONT_H_
