#ifndef _FR_OFDFONTMAPPER_H_
#define _FR_OFDFONTMAPPER_H_

#include <vector>
#include <string>
#include <map>

using namespace std;

class FR_OFDFontMapper : public IFX_FontMapper
{
public:
	FR_OFDFontMapper();

	void		LoadExtFontList(vector<string>& list);
	void		AddExtFont(const char* filepath);
	void		RemoveAllExtFont();

	virtual FXFT_Face	FindSubstFont(const CFX_ByteString& face_name, FX_BOOL bTrueType, FX_DWORD flags,
		int weight, int italic_angle, int codepage, CFX_SubstFont* pSubstFont);


protected:
	CFX_ByteString		FindExtFontFromName2(const CFX_ByteString& face_name, FX_BOOL bTrueType,
				FX_DWORD flags, int weight, int italic_angle, int CharsetCP, int onlyUseName, int& index);


	CFX_ByteString		GetDefaultFontPath();

protected:
	FXFT_Face		m_DefaultFace;

	map<string, string>		m_FileMap;
	CFX_CMapByteStringToPtr		m_FaceMap;

	CFX_ByteStringArray			m_ExtFontList;
};


#endif