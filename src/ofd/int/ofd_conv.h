#ifndef _OFD_CONV_H_
#define _OFD_CONV_H_

#include "../include/ofd_base_r.h"

// Error Code
#define OFD_CONVERTOR_SUCCESS					0
#define OFD_CONVERTOR_LICENSEFILE				1000	//license file error
#define OFD_CONVERTOR_PARAMETER					1001	//parameter error
#define OFD_CONVERTOR_COMMON					1002	//common error

#define OFD_CONVERTOR_OPENFILE					1020	//file open error
#define OFD_CONVERTOR_CREATEFILE				1021	//file create error
#define OFD_CONVERTOR_FILEEXIST					1022	//file not exist error
#define OFD_CONVERTOR_INVALIDPATH				1023	//path not exit error
#define OFD_CONVERTOR_FILEFORMAT				1024	//file format error

#define OFD_CONVERTOR_OFDPACKAGE				1040	//ofd file package error
#define OFD_CONVERTOR_OFDPARSER					1041	//parser ofd error
#define OFD_CONVERTOR_GETOFDDOC					1042	//get ofd document error

#define OFD_CONVERTOR_PDFPARSER					1060	//parser pdf error
#define OFD_CONVERTOR_GETPDFDOC					1061	//get pdf document error
#define OFD_CONVERTOR_PDFCONVERT				1062	//convert pdf error

#define OFD_CONVERTOR_EXCEPTION					1080	//exception error,try catch

#define OFD_CONVERTOR_TIFMODULE					1090	//tif moudle decoder
#define OFD_CONVERTOR_LOADFRAME					1091	//tif load frame error
#define OFD_CONVERTOR_TIFDECODER				1092	//tif file decoder error
#define OFD_CONVERTOR_IMGDECODER				1093	//img file decoder error

#define OFD_CONVERTOR_WPSCOMMAND				1110	//wps command execute error
#define OFD_CONVERTOR_ETCOMMAND					1111	//et command execute error
#define OFD_CONVERTOR_PPTCOMMAND					1112	//et command execute error
#define OFD_CONVERTOR_PDFCOMMAND				1113    //pdf toolkit command execute error

#define OFD_CONVERTOR_PS2PDF_HANDLER_CREATE		1120    // ps2pdf handler create error
#define OFD_CONVERTOR_PS2PDF_ADD_JOB			1121	// ps2pdf add convert job error

// Internal interface

typedef struct _OFDHiddenWatermark_
{
	unsigned short wID;
}OFDHiddenWatermark;

class ConvertorParam
{
public:
	int _embededFont;
	int _flatSignature;
	char _password[32];

	int _curDoc;
	int _curPage;
	int _dpi;
	int _width;
	int _height;
	int _imgType;

	OFD_WCHAR *_fontName; //字体名称
	unsigned int _fontColor; //字体颜色
	int _fontSize; //字体大小
	int _breakPage; //识别分页符

	int _compressState; //是否高压缩
	OFD_OPTIMIZERPARAM _optimizerParam; //优化参数


	int _pageType;		//页面类型， 0 普通页， 1 模板页.
	OFD_WCHAR _templatePageName[128];	//模板页名称
	OFDHiddenWatermark _hiddenWatermark;
	void* _filePackage;
	void* pInput;	//OFD转图片，OFD文件内存指针
};

int FS_CountOFDDoc(ConvertorParam* pParam);
int FS_CountOFDPage(ConvertorParam* pParam, int doc);


//
class IFX_FileRead;
class IFX_FileWrite;
class IOFD_Document;

// Convertor
int FS_OFD2PDF(IFX_FileRead* pOFDFile, IFX_FileWrite* pPDFFile, ConvertorParam* pParam = NULL);
int FS_OFD2PDF(OFD_LPCWSTR pwOfdName, OFD_LPCWSTR pwPdfName, ConvertorParam* pParam = NULL);
int FS_OFD2PDF2(IOFD_Document *pOfdDoc, IFX_FileWrite* pPDFFile, ConvertorParam* pParam = NULL);
int FS_PDF2OFD(OFD_LPCWSTR pwPdfName, OFD_LPCWSTR pwOfdName, ConvertorParam* pParam = NULL, void* cebStampHandler = NULL);
int FS_PDF2OFD2(IFX_MemoryStream* pPdfFile, IFX_MemoryStream* pOfdFile, ConvertorParam* pParam = NULL, void* cebStampHandler = NULL);
int FS_PDF2OFD_HiddenWatermark(OFD_LPCWSTR pwLibraryPath, OFD_LPCWSTR pwPdfName, OFD_LPCWSTR pwOfdName, ConvertorParam* pParam); // HiddenWatermark
int FS_OFD_EmbedHiddenWatermark(OFD_LPCWSTR pwLibraryPath, OFD_LPCWSTR pwOfdName, OFD_LPCWSTR pwNewOfdName, ConvertorParam* pParam);
int FS_OFD_ExtractHiddenWatermark(OFD_LPCWSTR pwLibraryPath, OFD_LPCWSTR pwOfdName, OFD_WSTR* EmbedInfo);
int FS_Image2OFD(OFD_LPCWSTR pwzFile, OFD_LPCWSTR pDest, ConvertorParam* pParam = NULL);
int FS_Txt2OFD(OFD_LPCWSTR pwzFile, OFD_LPCWSTR pDest, ConvertorParam* pParam = NULL);
int FS_OFD2Txt(OFD_LPCWSTR pwOFDFile, OFD_LPCWSTR pwTxtFile, ConvertorParam* pParam = NULL);

int FS_SEP2OFD(OFD_LPCWSTR pwSepName, OFD_LPCWSTR pwOfdName);
int FS_SEP2PDF(OFD_LPCWSTR pwSepName, OFD_LPCWSTR pwPdfName);

int FS_WPS2OFD(OFD_LPCWSTR pwWpsName, OFD_LPCWSTR pwOfdName);
int FS_ConvertTagTree(OFD_LPCWSTR pwWpsName, OFD_LPCWSTR pwOfdName);
int FS_ET2OFD(OFD_LPCWSTR pwETName, OFD_LPCWSTR pwOfdName);			// VPring in windows
int FS_PPT2OFD(OFD_LPCWSTR pwPPTName, OFD_LPCWSTR pwOfdName);			// VPring in windows

int FS_PDF2TXT(OFD_LPCWSTR pwPdfName, OFD_LPCWSTR pwTxtName);

void* FS_CreateCEBStampHandler();
void FS_ReleaseCEBStampHandler(void*);
int FS_CEB2PDF(OFD_LPCWSTR pwCebName, OFD_LPCWSTR pwPdfName, void* hPause = NULL, void* stampHandler = NULL);

int FS_OFDImage(OFD_LPCWSTR pwzFile, OFD_LPCWSTR pDest);
int FS_OFDImage(OFD_LPCWSTR pwzFile, int doc, int page, int dpi, OFD_LPCWSTR pDest);
int FS_OFDImage_ContentBox(OFD_LPCWSTR pwzFile, int doc, int page, OFD_LPCWSTR pDestType, OFD_LPBYTE* dest_buf, OFD_DWORD* dest_size);
void FS_OFDImage_ReleaseBuf(OFD_LPBYTE dest_buf);

void* FS_OFD2Image_Start(OFD_LPCWSTR pSrc, ConvertorParam* pParam = NULL);
void* FS_OFD2Image_Start2(IFX_FileRead* pSrcFileRead, ConvertorParam* pParam = NULL);
void FS_OFD2Image_Convertor(void* handle, OFD_LPCWSTR pDest,ConvertorParam* pParam = NULL);
void FS_OFD2Image_Convertor2(void* handle, IFX_FileWrite* pDstFileWrite,ConvertorParam* pParam = NULL);
void FS_OFD2Image_End(void* handle, ConvertorParam* pParam = NULL);
/**
 * @brief OFD文件页转换成图片
 *
 * @param[in] hPackage		包句柄。
 * @param[in] pOutputImage	输出图片路径+名称。
 * @param[in] pParam		ConvertorParam转换参数。
 * @return 0成功，非0失败。
 *				
 */
int FS_Package_OFD2Image(OFD_PACKAGE hPackage, OFD_LPCWSTR pOutputImage, void* pParam);

void* FS_Image2OFD_Start(OFD_LPCWSTR pDest);
int FS_Image2OFD_Add(void* handle, OFD_LPCWSTR pwzFile, ConvertorParam* pParam = NULL);
int FS_Image2OFD_End(void* handle);
/**
 * @brief 图片转换成OFD文件页
 *
 * @param[in] hPackage		包句柄。
 * @param[in] pInputImage	输入图片路径+名称。
 * @param[in] pParam		ConvertorParam转换参数。
 * @return 0成功，非0失败。
 *				
 */
int FS_Package_Image2OFD(OFD_PACKAGE hPackage, OFD_LPCWSTR pInputImage, void* pParam);

void* FS_PDFPortfolio_Start(OFD_LPCWSTR pDest);
void FS_PDFPortfolio_Add(void* handle, OFD_LPCWSTR pwzFile);
void FS_PDFPortfolio_End(void* handle);

int FS_HTML2OFD(OFD_LPCWSTR pwHtmlName, OFD_LPCWSTR pwOfdName);
int FS_OFD2WORD(OFD_LPCWSTR pwOfdName, OFD_LPCWSTR pwWordName);
int FS_WORD2ODT(OFD_LPCWSTR pwWordName, OFD_LPCWSTR pwOdtName);


/**
 * @brief PS文件转换成PDF文件
 *
 * @param[in] pwPsName		需要转换的PS文件的路径+名称。对于windows，路径格式示例：“D:\\test\\...\\ps\\test.ps”.
 * @param[in] pInputImage	目标PDF文件的路径+名称。对于windows，路径格式示例：“D:\\test\\...\\pdf\\test.pdf”.
 * @param[in] pParam		转换时用来产生临时文件夹的路径。对于windows，路径格式示例：“D:\\test\\...\\”.
 * @return 0成功，非0失败。
 *				
 */
int FS_PS2PDF(OFD_LPCWSTR pwPsName, OFD_LPCWSTR pwPdfName, OFD_LPCWSTR psResourceFolder);

#endif //_OFD_CONV_H_
