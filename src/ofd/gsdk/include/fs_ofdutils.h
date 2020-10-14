#ifndef _FS_OFDUTILS_H_
#define _FS_OFDUTILS_H_

class CFS_OFDDocument;
class CFX_DIBitmap;
class CFS_OFDTextObject;
class CFS_OFDTextTypesetting;

#define FS_UNICODE_DIANZIGONGWEN	L"\x7535\x5b50\x516c\x6587"
#define FS_UNICODE_GONGWENTI		L"\x516c\x6587\x4f53"
#define FS_UNICODE_GONGWENYUYI		L"\x516c\x6587\x8bed\x4e49"
#define FS_UNICODE_GONGWENBIAOSHI	L"\x516c\x6587\x6807\x8bc6"
#define FS_UNICODE_WENZHONG			L"\x6587\x79cd"
#define FS_UNICODE_DIANZIFAPIAO		L"\u7535\u5b50\u53d1\u7968"
#define FS_UNICODE_DIANZIPIAOJU		L"\u7535\u5b50\u7968\u636e"
#define FS_UNICODE_DIANZIZHENGZHAO	L"\u7535\u5b50\u8bc1\u7167"

#define FS_UNICODE_FENHAO			L"\u4efd\u53f7"

#define FS_UNICODE_MIJI				L"\u5bc6\u7ea7"
#define FS_UNICODE_BAOMIQIXIAN		L"\x4fdd\x5bc6\x671f\x9650"

#define FS_UNICODE_MIJIHEBAOMIQIXIAN	L"\x5bc6\x7ea7\x548c\x4fdd\x5bc6\x671f\x9650"  
#define FS_UNICODE_JINJICHENGDU		L"\u7d27\u6025\u7a0b\u5ea6"
#define FS_UNICODE_FAWENJIGUANMINGCHENG L"\u53d1\u6587\u673a\u5173\u540d\u79f0"

#define FS_UNICODE_HOUZHUIBIAOZHI	L"\x540e\x7f00\x6807\x5fd7"
#define FS_UNICODE_FUJIANSHUOMING	L"\u9644\u4ef6\u8bf4\u660e"
#define FS_UNICODE_BIAOZHI			L"\x6807\x5fd7"
#define FS_UNICODE_FAWENJIGUANBIAOZHI	L"\u53d1\u6587\u673a\u5173\u6807\u5fd7"

#define FS_UNICODE_FAWENJIGUANDAIZI	L"\u53d1\u6587\u673a\u5173\u4ee3\u5b57"
#define FS_UNICODE_NIANFEN			L"\u5e74\u4efd"
#define FS_UNICODE_FAWENSHUNXUHAO   L"\u53d1\u6587\u987a\u5e8f\u53f7" 
#define FS_UNICODE_FAWENZIHAO		L"\u53d1\u6587\u5b57\u53f7"

#define FS_UNICODE_ZHUSONGJIGUAN	L"\u4e3b\u9001\u673a\u5173"
#define FS_UNICODE_QIANFAREN		L"\u7b7e\u53d1\u4eba"
#define FS_UNICODE_BIAOTI			L"\u6807\u9898"

#define FS_UNICODE_FUJIANSHUOMING   L"\u9644\u4ef6\u8bf4\u660e"
#define FS_UNICODE_FAWENJIGUANSHUMING L"\u53d1\u6587\u673a\u5173\u7f72\u540d"
#define FS_UNICODE_QIANMINGZHANG	L"\u7b7e\u540d\u7ae0"

#define FS_UNICODE_CHENGWENRIQI		L"\u6210\u6587\u65e5\u671f"
#define FS_UNICODE_FUZHU			L"\u9644\u6ce8"
#define FS_UNICODE_CHAOSONGJIGUAN	L"\u6284\u9001\u673a\u5173"
#define FS_UNICODE_YINFAJIGUAN		L"\u5370\u53d1\u673a\u5173"
#define FS_UNICODE_YINFARIQI		L"\u5370\u53d1\u65e5\u671f"
#define FS_UNICODE_FABUCENGCI		L"\u53d1\u5e03\u5c42\u6b21"
#define FS_UNICODE_QIANFARENZHIWU   L"\u7b7e\u53d1\u4eba\u804c\u52a1"

void FS_FilterUnusedChar(CFX_WideString& wstr);

void FS_SplitString(CFX_WideString& wsStr, CFX_WideString& wsToken, CFX_WideStringArray& ret);
void FS_MegerString(CFX_WideStringArray& wsStrArray, CFX_WideString& ret);

//
void FS_BStr_SetSize(void* bstr, int size);
void FS_WStr_SetSize(void* wstr, int size);
void FS_WStr_FX2OFD(const CFX_WideString& fxwstr, void* fswstr);

//
FX_BOOL FS_IsSamePointF(CFX_PointF left, CFX_PointF right, FX_FLOAT fEpsilon = 0.001);
FX_BOOL FS_IsSpaceChar(FX_WCHAR wch);
FX_INT32 FS_GetFloat(FX_LPCWSTR pStr, FX_INT32 iLength, FX_FLOAT *pBuf, FX_INT32 &iCount);
FX_INT32 FS_OFD2SDKPageObjectType(OFD_CONTENTTYPE iType);
OFD_PAGEAREA FS_SDK2OFDBoxType(FX_INT32 iBoxType);
FX_INT32 FS_OFD2SDKBoxType(OFD_PAGEAREA iBoxType);
OFD_CHARDIRECTION FS_SDK2OFD2CharDirection(FX_INT32 iDirection);
FX_INT32 FS_OFD2SDKCharDirection(OFD_CHARDIRECTION iDirection);
OFD_READDIRECTION FS_SDK2OFD2ReadDirection(FX_INT32 iDirection);
FX_INT32 FS_OFD2SDKReadDirection(OFD_READDIRECTION iDirection);
FX_BOOL FS_IsValidMultiMedia(CFX_WideString wsType);
FX_INT32 FS_OFD2SDKMultiMedia(CFX_WideString wsType);
CFX_WideString FS_OFD2SDKMultiMedia(FX_INT32 iType);
CFX_WideString FS_GetLayerZorder(FX_INT32 iZorderType);
IFX_FileStream* FS_CreateFileStream(FX_LPCWSTR pzwsFileName);
COFD_WriteImageObject* FS_SetWriteImageObject_CreateIfNeed(CFS_OFDDocument* pDoc, const CFX_WideString& imageFileName,
	FX_LPCBYTE imageBuf, FX_INT32 imageBufLen, FX_INT32 imageFormat, FX_BOOL bTakeOver, COFD_WriteImageObject* pOld, FX_DWORD& retResID);

FX_BOOL FS_IsValidUnitime(FX_INT32 year, FX_BYTE month, FX_BYTE day, FX_BYTE hour, FX_BYTE minute, FX_BYTE second, FX_WORD millisecond);

//void FS_Unitime_FromString(fxutil::CFX_Unitime &unitime, const CFX_WideString& wsStr);
CFX_WideString FS_GetFileTrail(const CFX_WideString& file);

CFX_DIBitmap* FS_LoadImage(const CFX_WideString& wsPath);
CFX_DIBitmap* FS_LoadImage(FX_LPCBYTE pBuf, FX_INT32 nSize);
CFX_DIBitmap* FS_LoadImage(IFX_FileRead* pFileRead);

void FS_ExportDIBToOneImageFile(CFX_DIBitmap* dib, const CFX_WideString& wsImageType, FX_LPBYTE& dest_buf, FX_DWORD& dest_size);
void FS_RenderPage(CFX_DIBitmap* pDIB, IOFD_Page* pPage, int dpi = 96, CFX_RectF *renderBox = NULL);
FX_BOOL FS_GaussBluror(CFX_DIBitmap* pBitMap, CFX_Rect rect, FX_DWORD dwHorzBlur, FX_DWORD dwVertBlur);
FX_BOOL FS_Mosaic(CFX_DIBitmap* pBitMap, int v);

//
FX_BOOL FS_IsValidDate();
FX_BOOL FS_GetLocalTime(short& year, unsigned char& month, unsigned char& day, unsigned char& hour, unsigned char& minute, unsigned char& second);

//
FX_BOOL FS_CreateProcess(CFX_WideString& wsCommand, CFX_WideString& wsLicensePath, CFX_WideString& wsSrcFile, CFX_WideString& wsDstFile);

//
FX_BOOL ReadPDFToolkitRegisterInfo(CFX_WideString wsLicenseFile, CFX_WideString &wsCode, CFX_WideString &wsLicensee);
FX_BOOL CheckPDFToolkitRegisterState(const CFX_WideString &wsCommand, CFX_WideString& wsLicensePath);

//	-------- added by xhj ----------
void FS_WriteLog(CFX_ByteString logName, CFX_ByteString logValue, int nLen = 0);

//
CFX_WideString FS_GetModuleFileName();
FX_BOOL FS_GetSealModuleFileName(CFX_WideString& wsOES, CFX_WideString& wsOESName);
FX_BOOL FS_GetDigitalModuleFileName(CFX_WideString& wsODSDir, CFX_WideString& wsODSName);
FX_BOOL FS_IsFileExist(const CFX_WideString& wsFile);
FX_BOOL FS_IsValidPath(const CFX_WideString& wsPath);

void ParseAbbreviteData(COFD_Path *pPath, const CFX_WideString& wsStr);
CFX_ByteString MakeRectData(CFX_RectF rect);

void FS_WriteBMP(CFX_DIBitmap* pBitmap, FX_LPCSTR filename);

FX_BOOL FS_QRCodeEncrypt(FX_LPCBYTE pbSrcBuf, FX_DWORD dwSrcSize, FX_LPCBYTE pbKey, FX_DWORD dwKeyLen, CFX_ByteString& bsDest);
FX_BOOL FS_QRCodeDecrypt(CFX_ByteString& bsData, FX_LPCBYTE pbKey, FX_DWORD dwKeyLen, FX_LPBYTE pbDestBuf, FX_DWORD& dwDestSize);

//For EISDK
#define OFD_FORM_XML_TAG_NAME				"Name"					//�ڵ�����
#define OFD_FORM_XML_BOUNDARY				"Boundary"				//�߿�
#define OFD_FORM_XML_FONT_NAME				"Font"					//��������
#define OFD_FORM_XML_FONT_SIZE				"FontSize"				//�����С
#define OFD_FORM_XML_FONT_COLOR				"Color"					//��ɫ
#define OFD_FORM_XML_HORIZONTAL_ALIGN		"TextAlign"				//ˮƽ����
#define OFD_FORM_XML_VERTICAL_ALIGN			"VerticalAlign"			//��ֱ����
#define OFD_FORM_XML_HORIZONTAL_JUSTIFIED	"HorizontalJustified"	//ˮƽ��ɢ����
#define OFD_FORM_XML_VERTICAL_JUSTIFIED		"VerticalJustified"		//��ֱ��ɢ����
#define OFD_FORM_XML_MULTILINE				"MultiLine"				//�Զ�����
#define OFD_FORM_XML_AUTO_SIZE				"AutoSize"				//�Զ������С
#define OFD_FORM_XML_CHAR_NUMBER			"CharNumber"			//�ַ�����
#define OFD_FORM_XML_BYTE_NUMBER			"ByteNumber"			//�ֽڸ���
#define OFD_FORM_XML_LINE_BREAK				"LineBreak"				//���з�
#define OFD_FORM_XML_ROTATE					"Rotate"				//��ת�Ƕ�
#define OFD_FORM_XML_VERTICAL_TYPE			"CompositionType"		//���Ű�ģʽ
#define OFD_FORM_XML_VERTICAL_ORDER			"VerticalDirection"		//���Ű淽��
#define OFD_FORM_XML_LINE_SPACE				"LineSpace"				//�м��
#define OFD_FORM_XML_INFOCONVER_SHAPE		"Shape"					//�ڱ���״
#define OFD_FORM_XML_INFOCONVER_MODE		"Mode"					//�ڱ�ģʽ

typedef struct _fs_data_info
{
	FX_INT32 pageIndex; //ҳ��������
	CFX_MapByteStringToPtr* pTagContent; //Tag���ƺ�����
}FS_DATA_INFO;

typedef struct _fs_form_info
{
	FX_INT32 pageIndex; //ҳ��������
	CFX_PtrList* textBoxArray; //ҳ������Array
	CFX_PtrList* imageBoxArray; //ҳ��ͼƬArray
	CFX_PtrList* sealBoxArray; //ҳ��ǩ��Array
	CFX_PtrList* hiddenAreaArray; //ҳ���ڱ�Array
}FS_FORM_INFO;

typedef struct _fs_formbox_info
{
	CFX_WideString tagName; //�ڵ�����
	CFX_RectF boundary; //���ֱ߿�

	CFX_WideString fontName; //��������
	CFX_WideString subfontName; //��ѡ��������
	FX_FLOAT fontSize; //�����С
	FX_UINT32 fontColor; //������ɫ
	FX_UINT16 HAlign; //ˮƽ���뷽ʽ
	FX_UINT16 VAlign; //��ֱ���뷽ʽ
	FX_UINT16 HJustified; //ˮƽ��ɢ����
	FX_UINT16 VJustified; //��ֱ��ɢ����
	FX_BOOL bMultiline; //�Զ�����
	FX_BOOL bAutoSize; //��С�����С
	FX_UINT16 charNum; //�ַ�����
	FX_UINT16 byteNum; //�ֽڸ���
	CFX_WideString lineBreak; //���з�
	FX_INT32 Rotate; //��ת�Ƕ�
	FX_BOOL bVerticalType; //���Ű�
	FX_INT32 verticalOrder; //���Ű淽��
	FX_FLOAT lineSpace; //�м��

	FX_INT32 shape; //�ڱ���״
	FX_INT32 mode; //�ڱη�ʽ
}FS_FORMBOX_INFO;

void FS_FormXML_TextBoxParse(FS_FORMBOX_INFO* pTextInfo, CXML_Element* pBoxXML);
void FS_FormXML_ImageBoxParse(FS_FORMBOX_INFO* pImageInfo, CXML_Element* pBoxXML);
void FS_FormXML_SealBoxParse(FS_FORMBOX_INFO* pSealInfo, CXML_Element* pBoxXML);
void FS_FormXML_HiddenAreaParse(FS_FORMBOX_INFO* pHiddenInfo, CXML_Element* pBoxXML);
FX_BOOL FS_FormXMLParse(IFX_FileStream* pFormFile, CFX_PtrList* pFormInfos);
FX_BOOL FS_DataXMLParse(IFX_FileStream* pDataFile, CFX_PtrList* pDataInfos);
void FS_FormXMLRelease(CFX_PtrList* pFormInfos);
void FS_DataXMLRelease(CFX_PtrList* pDataInfos);

int GetImgWH(FX_LPCWSTR pszFile, int nFormat, int& nWidth, int& nHeight);
//����������Ϣ����ÿ���ַ��Ŀ�ȣ�������fArrayCharWidth�����ֵΪwsText���ܿ��
FX_FLOAT GetArrayCharWidth(const CFX_WideString& wsText, CFX_Font* pFont, FX_FLOAT fFontSize, CFX_FloatArray& fArrayCharWidth);

FX_FLOAT GetArrayCharWidth(const CFX_WideString& wsText, FX_FLOAT fFontSize, CFX_FloatArray& fArrayCharWidth);

//
void FS_AddUnderLine(CFS_OFDTextObject* pTextObj);
void FS_AddUnderLine2(CFS_OFDTextObject* pTextObj, CFX_WideString& wsText, CFS_OFDTextTypesetting* m_pTextTypesetting, CFX_FloatArray& fLenArr);

//linux�£�ofd2img����ά�����������Ⱦ������cairoģʽ�����õ�����Ļص�����
#if _FX_OS_ == _FX_LINUX_DESKTOP_
void g_Free(CFX_DIBitmap* pDIBitmap, FX_LPBYTE pExtBuffer);
CFX_DIBitmap* g_Decoder(IFX_FileRead* pFileRead, FX_LPBYTE* ppExtBuffer);
typedef struct _FX_ExtBuffer
{
    FX_LPBYTE pExtBuffer;
    FX_INT32 nType;
}FX_ExtBuffer;
void  ChangeRGBToRGB32(CFX_DIBitmap* pDIBitmap, FX_LPBYTE pSrc, int srcPicth);
void  ChangeToRGBEx(CFX_DIBitmap* pDIBitmap, int width, int height, int channel);
#endif	//_FX_LINUX_DESKTOP_

//
class CFS_OFDFileRead : public IOFD_FileStream, public CFX_Object
{
public:
	CFS_OFDFileRead();
	~CFS_OFDFileRead();

	virtual void				Release();
	virtual IFX_FileStream*		Retain();

	virtual FX_FILESIZE			GetSize();
	virtual FX_BOOL				IsEOF() {return FALSE;}
	virtual FX_FILESIZE			GetPosition() {return 0;}
	virtual FX_BOOL				ReadBlock(void* buffer, FX_FILESIZE offset, size_t size);
	virtual size_t				ReadBlock(void* buffer, size_t size) {return ReadBlock(buffer, 0, size) ? size : 0;}
	virtual	FX_BOOL				WriteBlock(const void* buffer, FX_FILESIZE offset, size_t size) {return FALSE;}
	virtual FX_BOOL			Flush() {return 0;}

	virtual CFX_WideString		GetCurrentFileName(FX_INT32 nType = 0) const;

	void						Init(IFX_FileRead *pFile, const CFX_WideString& wsCurrentFileName);

protected:
	FX_DWORD			m_dwRefCount;
	IFX_FileRead*		m_pZIPFileRead;
	CFX_WideString		m_wsCurrentFileName;
};

#define LINUX_TEMPFOLDERW	L"~/.temp/"
#define LINUX_TEMPFOLDER	"~/.temp/"

void WriteLog_GSDK(CFX_ByteString logName, CFX_ByteString logValue, int nLen = 0);
void WriteLog_GSDKF(CFX_ByteString logName, CFX_ByteString logValue, double nLen = 0.0);
#endif //_FS_OFDUTILS_H_
