#include "fr_include.h"
#include "../../fxlib/fxcore/include/fxge/fx_font.h"
#include "../../fxlib/fxcore/include/thirdparties/freetype/ft2build.h"
#include "../../fxlib/fxcore/include/thirdparties/freetype/foxitnames.h"
#include "../../fxlib/fxcore/include/thirdparties/freetype/freetype/freetype.h"
#include "fr_ofdutil.h"
#include "fr_ofdfontmapper.h"

#include <stdio.h>
//#include <io.h>

#if defined(WIN32)
#define LogOut				printf
#elif defined(__ANDROID__)
//#define LogOut(...)			((void*)0)
#define LOG_TAG "fontmapper"
#include <jni.h>
#include <android/log.h>
#define	LogOut(...)		__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif
//*
#undef LogOut
#define LogOut(...)			((void*)0)
// */

static const char* g_StandardFonts[] = {
	"Times-Roman",
	"Times-Bold",
	"Times-Italic",
	"Times-BoldItalic",
	"Helvetica",
	"Helvetica-Bold",
	"Helvetica-Oblique",
	"Helvetica-BoldOblique",
	"Courier",
	"Courier-Bold",
	"Courier-Oblique",
	"Courier-BoldOblique",
	"Symbol",
	"ZapfDingbats",
};

static bool IsStandardFont(const char* font)
{
	int size = sizeof(g_StandardFonts)/sizeof(g_StandardFonts[0]);
	for (int i = size - 1; i >= 0; i --) {
		if (strcmp(font, g_StandardFonts[i]) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

typedef struct _FONT_TABLE
{
	char* key;	// cn name
	char* value;	// en name
	char* family;	// family name
} FONT_TABLE;

static FONT_TABLE s_cnFontTable[] = {
	{"宋体", "SimSun", "SimSun"},
	{"新宋体", "NSimSun", "SimSun"},
	{"宋体_GB2312", "SimSun_GB2312", "SimSun"},
	{"宋体_GB2312", "SimSun", "SimSun"},
	{"宋体-18030","SimSun-18030", "SimSun"},
	{"黑体", "SimHei", "SimHei"},
	{"隶书", "LiSu", "LiSu"},
	{"仿宋_GB2312","FangSong_GB2312", "FangSong"},
	{"仿宋_GB2312","FangSong", "FangSong"},
	{"仿宋", "FangSong", "FangSong"},
	{"仿宋", "FangSong_GB2312", "FangSong"},
	{"楷体_GB2312","KaiTi_GB2312","KaiTi"},
	{"楷体_GB2312","KaiTi","KaiTi"},
	{"楷体","KaiTi","KaiTi"},
	{"楷体","KaiTi_GB2312","KaiTi"},
	{"方正小标宋简体","FZXiaoBiaoSong-B05S", "FZXiaoBiaoSong"},
	{"方正小标宋简体","FZXiaoBiaoSong-B05","FZXiaoBiaoSong"},
	{"方正小标宋简体","FZXiaoBiaoSong","FZXiaoBiaoSong"},
	{"方正小标宋_GBK","FZXiaoBiaoSong","FZXiaoBiaoSong"},
	{"方正小标宋_GBK","FZXiaoBiaoSong-B05","FZXiaoBiaoSong"},
	{"方正小标宋_GBK","FZXiaoBiaoSong-B05S","FZXiaoBiaoSong"},
	{"方正仿宋_GBK","FZFangSong-Z02","FangSong"},
	{"方正仿宋_GBK","FZFangSong","FangSong"},
	{"方正仿宋_GBK","FangSong","FangSong"},
	{"方正仿宋_GBK","FangSong_GB2312","FangSong"},
	{"方正黑体_GBK","FZHei-B01","SimHei"},
	{"方正黑体_GBK","SimHei","SimHei"},
	{"方正楷体_GBK","FZKai-Z03", "KaiTi"},
	{"方正楷体_GBK","KaiTi", "KaiTi"},
	{"方正隶书_GBK","FZLiShu-S01", "LiShu"},
	{"方正隶书_GBK","LiSu", "LiShu"},
	{"方正报宋_GBK","FZBaoSong-Z04", "BaoSong"},
	{"微软雅黑","Microsoft YaHei", "SimHei"},
	{"幼园","YouYuan","YouYuan"},
	{"华文彩云","STCaiyun", "Caiuyun"},
	{"华文仿宋","STFangsong", "FangSong"},
	{"华文琥珀","STHupo", "Hupo"},
	{"华文楷体","STKaiti", "KaiTi"},
	{"华文隶书","STLiti", "LiShu"},
	{"华文宋体","STSong", "SimSun"},
	{"华文细黑","STXihei", "Sim"},
	{"华文行楷", "STXingkai", "Xingkai"},
	{"华文中宋","STZhongsong", "Zhongsong"},
	{"方正舒体","FZShuTi", "ShuTi"},
	{"方正姚体","FZYaoTi", "YaoTi"},
	{"方正超粗黑_GBK","FZChaoCuHei-M10", "SimHei"},
	{"方正粗倩_GBK","FZCuQian-M17", "CuQian"},
	{"方正粗宋_GBK","FZCuSong-B09", "CuSong"},
	{"方正彩云_GBK","FZCaiYun-M09", "CaiYun"},
	{"方正大标宋_GBK","FZDaBiaoSong-B06", "DaBiaoSong"},
	{"方正大黑_GBK","FZDaHei-B02", "DaHei"},
	{"方正黄草_GBK","FZHuangCao-S09", "HuangCao"},
	{"方正华隶_GBK","FZHuaLi-M14", "HuaLi"},
	{"方正琥珀_GBK","FZHuPo-M04", "HuPo"},
	{"方正康体_GBK","FZKangTi-S07", "KangTi"},
	{"方正少儿_GBK","FZShaoEr-M11", "ShaoEr"},
	{"方正隶二_GBK","FZLiShu II-S06", "LiSHu"},
	{"方正隶变_GBK","FZLiBian-S02", "LiBian"},
	{"方正美黑_GBK","FZMeiHei-M07", "MeiHei"},
	{"方正新报宋_GBK","FZNew BaoSong-Z12", "BaoSong"},
	{"方正新舒体_GBK","FZNew ShuTi-S08", "ShuTi"},
	{"方正平和_GBK","FZPingHe-S11", "PingHe"},
	{"方正胖娃_GBK","FZPangWa-M18", "PangWa"},
	{"方正宋三_GBK","FZSong III-Z05", "Song"},
	{"方正宋黑_GBK","FZSongHei-B07", "SongHei"},
	{"方正瘦金书_GBK","FZShouJinShu-S10", "ShouJinShu"},
	{"方正书宋_GBK","FZShuSong-Z01", "ShuSong"},
	{"方正舒体_GBK","FZShuTi-S05", "ShuTi"},
	{"方正宋一_GBK","FZSongYi-Z13", "SongYi"},
	{"方正水柱_GBK","FZShuiZhu-M08", "ShuiZhu"},
	{"方正魏碑_GBK","FZWeiBei-S03", "WeiBei"},
	{"方正细等线_GBK","FZXiDengXian-Z06", "XiDengXian"},
	{"方正细黑一_GBK","FZXiHei I-Z08", "XiHei"},
	{"方正行楷_GBK","FZXingKai-S04", "XingKai"},
	{"方正细倩_GBK","FZXiQian-M15", "XiQian"},
	{"方正细珊瑚_GBK","FZXiShanHu-M13", "XiShanHu"},
	{"方正细圆_GBK","FZXiYuan-M01", "XiYuan"},
	{"方正准圆_GBK","FZZhunYuan-M02", "ZhunYuan"},
	{"方正粗圆_GBK","FZCuYuan-M03", "CuYuan"},
	{"方正姚体_GBK","FZYaoTi-M06", "YaoTi"},
	{"方正幼线_GBK","FZYouXian-Z09", "YouXian"},
	{"方正中等线_GBK","FZZhongDengXian-Z07", "ZhongDengXian"},
	{"方正稚艺_GBK","FZZhiYi-M12", "ZhiYi"},
	{"方正中倩_GBK","FZZhongQian-M16", "ZhongQian"},
	{"方正综艺_GBK","FZZongYi-M05", "ZongYi"},
	{"方正卡通_GBK","FZKaTong-M19", "KaTong"},
	{"方正艺黑_GBK","FZYiHei-M20", "YiHei"},
	{"方正报宋简体","FZBaoSong-Z04S", "BaoSong"},
	{"方正粗圆简体","FZCuYuan-M03S", "CuYuan"},
	{"方正大标宋简体","FZDaBiaoSong-B06S", "DaBiaoSong"},
	{"方正大黑简体","FZDaHei-B02S", "DaHei"},
	{"方正仿宋简体","FZFangSong-Z02S", "FangSong"},
	{"方正黑体简体","FZHei-B01S", "SimHei"},
	{"方正琥珀简体","FZHuPo-M04S", "HuPo"},
	{"方正楷体简体","FZKai-Z03S", "KaiTi"},
	{"方正隶变简体","FZLiBian-S02S", "LiBian"},
	{"方正隶书简体","FZLiShu-S01S", "LiShu"},
	{"方正美黑简体","FZMeiHei-M07S", "MeiHei"},
	{"方正书宋简体","FZShuSong-Z01S", "ShuSong"},
	{"方正舒体简体","FZShuTi-S05S", "ShuTi"},
	{"方正水柱简体","FZShuiZhu-M08S", "ShuZhu"},
	{"方正宋黑简体","FZSongHei-B07S", "SongHei"},
	{"方正宋三简体","FZSong III-Z05S", "Song"},
	{"方正魏碑简体","FZWeiBei-S03S", "WeiBei"},
	{"方正细等线简体","FZXiDengXian-Z06S", "XiDengXian"},
	{"方正细黑一简体","FZXiHei I-Z08S", "XiHei"},
	{"方正细圆简体","FZXiYuan-M01S", "XiYuan"},
	{"方正行楷简体","FZXingKai-S04S", "XingKai"},
	{"方正姚体简体","FZYaoTi-M06S", "YaoTi"},
	{"方正中等线简体","FZZhongDengXian-Z07S", "ZhongDengXian"},
	{"方正准圆简体","FZZhunYuan-M02S", "ZhunYuan"},
	{"方正综艺简体","FZZongYi-M05S", "ZongYi"},
	{"方正彩云简体","FZCaiYun-M09S", "CaiYun"},
	{"方正隶二简体","FZLiShu II-S06S", "LiShu"},
	{"方正康体简体","FZKangTi-S07S", "KangTi"},
	{"方正超粗黑简体","FZChaoCuHei-M10S", "ChaoCuHei"},
	{"方正新报宋简体","FZNew BaoSong-Z12S", "New BaoSong"},
	{"方正新舒体简体","FZNew ShuTi-S08S", "New ShuTi"},
	{"方正黄草简体","FZHuangCao-S09S", "HuangCao"},
	{"方正少儿简体","FZShaoEr-M11S", "ShaoEr"},
	{"方正稚艺简体","FZZhiYi-M12S", "ZhiYi"},
	{"方正细珊瑚简体","FZXiShanHu-M13S","XiShanHu"},
	{"方正粗宋简体","FZCuSong-B09S", "CuSong"},
	{"方正平和简体","FZPingHe-S11S", "PingHe"},
	{"方正华隶简体","FZHuaLi-M14S", "HuaLi"},
	{"方正瘦金书简体","FZShouJinShu-S10S", "ShouJinShu"},
	{"方正细倩简体","FZXiQian-M15S", "XiQian"},
	{"方正中倩简体","FZZhongQian-M16S", "ZhongQian"},
	{"方正粗倩简体","FZCuQian-M17S", "CuQian"},
	{"方正胖娃简体","FZPangWa-M18S", "PangWa"},
	{"方正宋一简体","FZSongYi-Z13S", "SongYi"},
	{"CESI_小标宋(GB2312-1980)","CESI_XBSJ", "XiaoBiaoSong"},
	{"CESI_小标宋(GB2312-1980)","CESI_XBS", "XiaoBiaoSong"},
	{"CESI_小标宋(GB18030-2005)","CESI_XBS", "XiaoBiaoSong"},
	{"CESI_小标宋(GB18030-2005)","CESI_XBSJ", "XiaoBiaoSong"},
	{"方正小标宋_GBK","CESI_XBS", "XiaoBiaoSong"},
	{"方正小标宋_GBK","CESI_XBSJ", "XiaoBiaoSong"},
	{"方正小标宋简体","CESI_XBS", "XiaoBiaoSong"},
	{"方正小标宋简体","CESI_XBSJ", "XiaoBiaoSong"},
	{"仿宋", "CESI_FS_GB18030", "FangSong"},
	{"仿宋", "CESI_FS_GB13000", "FangSong"},
	{"仿宋", "CESI_FS_GB2312", "FangSong"},
	{"仿宋_GB2312", "CESI_FS_GB2312", "FangSong"},
	{"仿宋_GB2312", "CESI_FS_GB18030", "FangSong"},
	{"仿宋_GB2312", "CESI_FS_GB13000", "FangSong"},
	{"CESI仿宋-GB2312", "CESI_FS_GB2312", "FangSong"},
	{"CESI仿宋-GB2312", "CESI_FS_GB18030", "FangSong"},
	{"CESI仿宋-GB2312", "CESI_FS_GB13000", "FangSong"},
	{"CESI仿宋-GB2312", "FangSong", "FangSong"},
	{"CESI仿宋-GB2312", "FangSong_GB2312", "FangSong"},
	{"CESI仿宋-GB13000", "CESI_FS_GB13000", "FangSong"},
	{"CESI仿宋-GB13000", "CESI_FS_GB18030", "FangSong"},
	{"CESI仿宋-GB13000", "CESI_FS_GB2312", "FangSong"},
	{"CESI仿宋-GB13000", "FangSong", "FangSong"},
	{"CESI仿宋-GB13000", "FangSong_GB2312", "FangSong"},
	{"CESI仿宋-GB18030", "CESI_FS_GB18030", "FangSong"},
	{"CESI仿宋-GB18030", "CESI_FS_GB13000", "FangSong"},
	{"CESI仿宋-GB18030", "CESI_FS_GB2312", "FangSong"},
	{"CESI仿宋-GB18030", "FangSong", "FangSong"},
	{"CESI仿宋-GB18030", "FangSong_GB2312", "FangSong"},
	{"timesnewromanps-italicmt", "Times New Roman Italic"},
	{"timesnewromanps-italicmt", "Times New Roman"},
	{"TimesNewRomanPS-ItalicMT", "Times New Roman Italic"},
	{"TimesNewRomanPS-ItalicMT", "Times New Roman"},
	{"timesnewromanps-boldmt", "Times New Roman Bold"},
	{"timesnewromanps-boldmt", "Times New Roman"},
	{"TimesNewRomanPS-BoldMT", "Times New Roman Bold"},
	{"TimesNewRomanPS-BoldMT", "Times New Roman"},
	{"timesnewromanps-bolditalicmt", "Times New Roman Bold Italic"},
	{"timesnewromanps-bolditalicmt", "Times New Roman"},
	{"TimesNewRomanPS-BoldItalicMT", "Times New Roman Bold Italic"},
	{"TimesNewRomanPS-BoldItalicMT", "Times New Roman"},
	{"思源黑体 CN", "Source Han Sans CN Normal"},
	{"方正宋体S-超大字符集", "FZSongS-Extended(SIP)"},
	{"",""},
};

void	FetchCnFontTable(const CFX_ByteString& bsName, CFX_ByteStringArray& table)
{
	LogOut("CORP_FontMapperEx::FetchCnFontMap, begin bsName=%s", (FX_LPCSTR)bsName);
	int i = 0;
	CFX_ByteString key;
	do {
		key = s_cnFontTable[i].key;
		if (key == bsName) {
			CFX_ByteString value = s_cnFontTable[i].value;
			if (value.IsEmpty()) {
				break;
			}
			table.Add(value);
			LogOut("CORP_FontMapperEx::FetchCnFontMap, index=%d, enName=%s", table.GetSize(), s_cnFontTable[i].value);
		}
		i++;
	} while(key !="");
	LogOut("CORP_FontMapperEx::FetchCnFontMap, end size=%d", table.GetSize());
	return;
}

// class FR_OFDFontMapper : public IFX_FontMapper
FR_OFDFontMapper::FR_OFDFontMapper()
	: m_DefaultFace(NULL)
{

}

void FR_OFDFontMapper::LoadExtFontList(vector<string>& list)
{
	m_ExtFontList.RemoveAll();
	for (int i=0; i<list.size(); ++i) {
		LogOut("LoadExtFontList, path=%s", list[i].c_str());
		m_ExtFontList.Add(CFX_ByteString(list[i].c_str()));
	}
}

void		FR_OFDFontMapper::AddExtFont(const char* filepath)
{
	LogOut("CORP_FontMapperEx::AddExtFont, filepath=%s", filepath);
	m_ExtFontList.Add(CFX_ByteString(filepath));
}

void		FR_OFDFontMapper::RemoveAllExtFont()
{
	m_ExtFontList.RemoveAll();
}

FXFT_Face	FR_OFDFontMapper::FindSubstFont(const CFX_ByteString& face_name, FX_BOOL bTrueType, FX_DWORD flags,
		int weight, int italic_angle, int CharsetCP, CFX_SubstFont* pSubstFont)
{
	// Check is standard font
	LogOut("CORP_FontMapperEx::FindSubstFont begin，face_name=%s, bTrueType=%d, weight=%d, italic_angle=%d, charsetCP=%d", (FX_LPCSTR)face_name, bTrueType, weight, italic_angle, CharsetCP);
	if (0==CharsetCP && IsStandardFont((FX_LPCSTR)face_name)) {
		LogOut("CORP_FontMapperEx::FindSubstFont found standard font, name=%s, ", (FX_LPCSTR)face_name);
		return NULL;
	}

	// Check if already cached
	CFX_ByteString fontPath;
	string temp = m_FileMap[(FX_LPCSTR)face_name];
	if (!temp.empty()) {
		fontPath = temp.c_str();
	}

	FXFT_Face face;
	if (!fontPath.IsEmpty() && m_FaceMap.Lookup(fontPath, (void*&)face)) {
		LogOut("FOV_FontMapper::FindSubstFont found exist FA_Face, name=%s, path=%d", (FX_LPCSTR)face_name, (FX_LPCSTR)fontPath);
		return face;
	}

	// Get file path and face index
	int index = 0;
	fontPath = FindExtFontFromName2(face_name, bTrueType, flags, weight, italic_angle, CharsetCP, FALSE, index);
	if (fontPath.IsEmpty()) {
		CFX_ByteString faceName = face_name;
		faceName.Remove(' ');
		fontPath = FindExtFontFromName2(faceName, bTrueType, flags, weight, italic_angle, CharsetCP, FALSE, index);
	}
	if (fontPath.IsEmpty()) {
		CFX_ByteStringArray table;
		FetchCnFontTable(face_name, table);
		for (int i=0; i<table.GetSize(); ++i) {
			index = 0;
            if (0==CharsetCP && IsStandardFont((FX_LPCSTR)table[i])) {
                LogOut("CORP_FontMapperEx::FindSubstFont found standard font, name=%s, ", (FX_LPCSTR)table[i]);
                return NULL;
            }
			fontPath = FindExtFontFromName2(table[i], bTrueType, flags, weight, italic_angle, CharsetCP, FALSE, index);
			if (!fontPath.IsEmpty()) {
				break;
			}
		}
	}

	if (fontPath.IsEmpty()) {
		fontPath = GetDefaultFontPath();//"/System/Library/Fonts/Core/STHeiti-Light.ttc";
		LogOut("FOV_FontMapper::FindSubstFont use default font, name=%s, path=%s", (FX_LPCSTR)face_name, (FX_LPCSTR)fontPath);
	} else {
		LogOut("FOV_FontMapper::FindSubstFont find match font, name=%s, path=%s", (FX_LPCSTR)face_name, (FX_LPCSTR)fontPath);
	}

	if (!fontPath.IsEmpty()
#if (_FX_OS_==_FX_ANDROID_) || (_FX_OS_==_FX_IOS_)
		&& access(fontPath, F_OK) != -1
#endif
		) {
		if (m_FaceMap.Lookup(fontPath, (void*&)face))  {
			// found face, cache the name - path 
			m_FileMap[(FX_LPCSTR)face_name] = (FX_LPCSTR)fontPath;
			LogOut("CORP_FontMapperEx::FindSubstFont found=2, name=%s, path=%s", (FX_LPCSTR)face_name, (FX_LPCSTR)fontPath);
			return face;
		} else {
			// Now load the face
			face = m_pFontMgr->GetFileFace((FX_LPCSTR)fontPath, index);
			if (face != NULL) {
				// Cache it
				m_FileMap[(FX_LPCSTR)face_name] = (FX_LPCSTR)fontPath;
				//m_FaceMap.SetAt(fontPath, face);
				if (pSubstFont) {
					pSubstFont->m_Family = face->family_name;
					pSubstFont->m_Weight = weight;
					pSubstFont->m_ItalicAngle = 0;
				}
				LogOut("CORP_FontMapperEx::FindSubstFont found=1, name=%s, path=%s", (FX_LPCSTR)face_name, (FX_LPCSTR)fontPath);
				return face;
			}
		}
	}
	LogOut("CORP_FontMapperEx::FindSubstFont found=0, name=%s, not found", (FX_LPCSTR)face_name);
	return NULL;
/*
	FXFT_Face face;
	if (m_DefaultFace) {
		return m_DefaultFace;
	}
#ifndef _WIN32
	CFX_ByteString fontPath = "/system/fonts/DroidSansFallback.ttf";
	if (!fontPath.IsEmpty() && access(fontPath, F_OK) != -1) {
		// Now load the face
		face = CFX_GEModule::Get()->GetFontMgr()->GetFileFace((FX_LPCSTR)fontPath, 0);
		if (face != NULL) {
			// Cache it
			m_DefaultFace = face;
			if (pSubstFont) {
				pSubstFont->m_Family = face->family_name;
				pSubstFont->m_Weight = weight;
				pSubstFont->m_ItalicAngle = 0;
			}
			return face;
		}
	}
#endif
	return NULL;
// */
}

CFX_ByteString FR_OFDFontMapper::FindExtFontFromName2(const CFX_ByteString& face_name, FX_BOOL bTrueType,
			FX_DWORD flags, int weight, int italic_angle, int CharsetCP, int onlyUseName, int& index)
{
	LogOut("CORP_FontMapperEx::FindExtFontFromName2, begin, name=%s, weight=%d, italic=%d, charsetCP=%d",
					(FX_LPCSTR)face_name, weight, italic_angle, CharsetCP);
	CFX_FontMgr* pFontMgr = CFX_GEModule::Get()->GetFontMgr();
	for (int i=0; i<m_ExtFontList.GetSize(); ++i) {
		CFX_ByteString path = m_ExtFontList[i];
		IFX_FileRead* fileread = FX_CreateFileRead((FX_LPCSTR)path);
		if (!fileread) continue;
		AutoReleasePtr<IFX_FileRead> autoFileRead(fileread);
		CFX_Font* pFXFont = new CFX_Font;
		FX_BOOL ret = pFXFont->LoadFile(fileread);
		if (!ret) {
			continue;
		}
		CFX_ByteString bsFamilyName = pFXFont->GetFamilyName();
		CFX_ByteString bsFaceName = pFXFont->GetFaceName();
		CFX_ByteString bsPSName = pFXFont->GetPsName().UTF8Encode();
		LogOut("CORP_FontMapperEx::FindExtFontFromName2, need=%s, familyname=%s, facename=%s,psname=%s", (FX_LPCSTR)face_name, (FX_LPCSTR)bsFamilyName, (FX_LPCSTR)bsFaceName, (FX_LPCSTR)bsPSName);
		int count = pFXFont->GetFace()->num_faces;
		const CFX_SubstFont* pSubstFont = pFXFont->GetSubstFont();
		if (NULL != pSubstFont) {
			LogOut("CORP_FontMapperEx::FindExtFontFromName2, substfont={%s, %d, %x, %d, %d, %d, %d, %d}",
					(FX_LPCSTR)pSubstFont->m_Family,
					pSubstFont->m_Charset,
					pSubstFont->m_SubstFlags,
					pSubstFont->m_Weight,
					pSubstFont->m_ItalicAngle,
					pSubstFont->m_bSubstOfCJK,
					pSubstFont->m_WeightCJK,
					pSubstFont->m_bItlicCJK
					);
		}
		pFXFont->DeleteFace();
		delete pFXFont;
		//LogOut("CORP_FontMapperEx::FindExtFontFromName2, path=%s, familyName=%s, faceName=%s\r\n", (FX_LPCSTR)path, (FX_LPCSTR)bsFamilyName, (FX_LPCSTR)bsFaceName);
		if (bsFamilyName==face_name || bsPSName==face_name || bsFaceName == face_name) {
			LogOut("CORP_FontMapperEx::FindExtFontFromName2, found=2, name=%s, path=%s, index=%d", (FX_LPCSTR)face_name, (FX_LPCSTR)path, index);
			return path;
		}
		bsFamilyName.Remove(' ');
		bsFaceName.Remove(' ');
		bsPSName.Remove(' ');
		if (bsFamilyName==face_name || bsPSName==face_name || bsFaceName == face_name) {
			LogOut("CORP_FontMapperEx::FindExtFontFromName2, found=3, name=%s, path=%s, index=%d", (FX_LPCSTR)face_name, (FX_LPCSTR)path, index);
			return path;
		}


#if 0
		CFX_FontMgr* pFontMgr = CFX_GEModule::Get()->GetFontMgr();
		if (pFontMgr) {
			for (int j=0; j<count; ++j) {
				//LogOut("CORP_FontMapperEx::FindExtFontFromName2, before GetFileFace");
				FXFT_Face face = pFontMgr->GetFileFace(path, j);
				//LogOut("CORP_FontMapperEx::FindExtFontFromName2, after  GetFileFace");
				if (face == NULL) {
					continue;
				}

				//LogOut("CORP_FontMapperEx::FindExtFontFromName2, index=%d, family_name=%s, style_name=%s\r\n", j,  (FX_LPCSTR)face->family_name, (FX_LPCSTR)face->style_name);
				CFX_ByteString family_name = face->family_name;
				CFX_ByteString style_name = face->style_name;
				CFX_ByteString full_face_name = family_name;
				if (!style_name.EqualNoCase("Reguler")) {
					full_face_name += " ";
					full_face_name += style_name;
				}
				if (face_name == face->family_name) {
					index = j;
					pFontMgr->ReleaseFace(face);
					LogOut("CORP_FontMapperEx::FindExtFontFromName2, found=1, name=%s, path=%s, index=%d", (FX_LPCSTR)face_name, (FX_LPCSTR)path, index);
					return path;
				}/* else if (face_name == full_face_name){
					index = j;
					LogOut("CORP_FontMapperEx::FindExtFontFromName2, found=1, full_name=%s, path=%s, index=%d", (FX_LPCSTR)face_name, (FX_LPCSTR)path, index);
					return path;
				} // */

				pFontMgr->ReleaseFace(face);
			}
		}
#endif
	}
	LogOut("CORP_FontMapperEx::FindExtFontFromName2, found=0, name=%s", (FX_LPCSTR)face_name);
	return "";
}

CFX_ByteString		FR_OFDFontMapper::GetDefaultFontPath()
{
#if (_FX_OS_ == _FX_ANDROID_)
	char* path = "/system/fonts/DroidSansFallback.ttf";
	char* path_v6 = "/system/fonts/NotoSansSC-Regular.otf";
	if (access(path_v6, F_OK) != -1) {
		return path_v6;
	} else if (access(path, F_OK) != -1) {
		return path;
	}
#elif (_FX_OS_==_FX_IOS_)
	return "/System/Library/Fonts/Core/STHeiti-Light.ttc";
#elif (_FX_OS_==_FX_WIN32_DESKTOP_) || (_FX_OS_==_FX_WIN64_)
	return "C:\\Windows\\Fonts\\Simsun.ttc";
#else
	return "";
#endif
}