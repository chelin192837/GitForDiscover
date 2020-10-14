#ifndef _FS_OFDFONTMAPPER_H_
#define _FS_OFDFONTMAPPER_H_

class CFS_OFDFontMapper;

typedef struct OFDFontMapperData : public CFX_Object
{
	CFX_WideString fontName;
	CFX_WideString fontFile;
	void* face;
} *LPOFDFontMapperData;

class CFS_OFDFontMapper : public IFX_FontMapper
{
public:
	CFS_OFDFontMapper();
	virtual ~CFS_OFDFontMapper();

	virtual FXFT_Face	FindSubstFont(const CFX_ByteString& face_name, FX_BOOL bTrueType, FX_DWORD flags,
		int weight, int italic_angle, int CharsetCP, CFX_SubstFont* pSubstFont);

	void AddFontFile(const CFX_WideString& fontName, const CFX_WideString& fontFile);

protected:
	LPOFDFontMapperData MatchFontMapperData_ByFamilyName(const CFX_ByteString& familyName);
	LPOFDFontMapperData MatchFontMapperData_ByFamilyName(const CFX_WideString& familyName);

	CFX_PtrArray	m_name2faceMap;
};

#endif //_FS_OFDFONTMAPPER_H_
