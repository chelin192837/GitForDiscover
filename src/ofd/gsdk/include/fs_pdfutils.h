#ifndef _FS_PDFUTILS_H_
#define _FS_PDFUTILS_H_

/** @brief Save the document incrementally. */
#define FPDF_SAVEAS_INCREMENTAL		1
/** @brief Save the document without the original one. */
#define FPDF_SAVEAS_NOORIGINAL		2
/** @brief Save the document progressively. */
#define FPDF_SAVEAS_PROGRESSIVE		4

/** 
* @name Macro definitions for signature show flag
*/
/**@{*/
/** @brief Show text on signature appearance. */
#define FPDF_SIGNATURE_AP_SHOW_TEXT				0x100
/** @brief Show image on signature appearance. */
#define FPDF_SIGNATURE_AP_SHOW_IMAGE			0x080
/** @brief Show signer name on description. */
#define FPDF_SIGNATURE_AP_SHOW_NAME				0x040
/** @brief Show location on description. */
#define FPDF_SIGNATURE_AP_SHOW_LOCATION			0x020
/** @brief Show dn on description. */
#define FPDF_SIGNATURE_AP_SHOW_DN				0x010
/** @brief Show date on description. */
#define FPDF_SIGNATURE_AP_SHOW_DATE				0x008
/** @brief Show reason on description. */
#define FPDF_SIGNATURE_AP_SHOW_REASON			0x004
/** @brief Show label on description. */
#define FPDF_SIGNATURE_AP_SHOW_LABEL			0x002
/** @brief Show Foxit flag on signature appearance. */
#define FPDF_SIGNATURE_AP_SHOW_FOXITFLAG		0x001
/**@}*/

/**
 * @name Macro definitions for image types
 */
/**@{*/
/** @brief The image type is BMP. */
#define FPDF_IMAGETYPE_BMP			1
/** @brief The image type is JPG. */
#define FPDF_IMAGETYPE_JPG			2
/** @brief The image type is PNG. */
#define FPDF_IMAGETYPE_PNG			3
/**@}*/

#ifdef USE_STANDARD_SIGNATURE
	#define FPDF_SIGCONTENTS_LENGTH					8196
#else
	#define FPDF_SIGCONTENTS_LENGTH					8196*5
#endif

#define FPDF_SIG_BYTERANGE_LEN					34
#define FPDF_SIGAP_WITHOUTIMAGE					"SigAPWithoutIMG"

typedef enum
{
	AP_UK,
	AP_US,
	AP_V,
	AP_IV,
	AP_N
} SIGAPT;

class CFS_PDFSDK_Uilts
{
public:
	static CPDF_Rect			GetRotatedRect(CPDF_FormControl* pFormControl);
	static CPDF_Matrix			GetMatrix(CPDF_FormControl* pFormControl);
	static CPDF_Font*			GetDefaultFont(CPDF_Page* pPDFPage, CPDF_FormControl* pFormControl);
	static CPDF_Font*			GetPageFont(CPDF_Page* pPage, CFX_ByteString csNameTag);
	static CPDF_Font*			AddStandardFont(CPDF_Document* pDocument, CFX_ByteString csFontName);
	static void					SetDefaultFont(CPDF_FormControl* pFormControl, CFX_ByteString csTag, const CPDF_Font* pFont);
	static CFX_ByteString		GetTextMatrixString(CPDF_FormControl* pFormControl);
	static FX_BOOL				HasTextMatrix(CPDF_FormControl* pFormControl);
	static CFX_ByteString		GetColorString(CPDF_FormControl* pFormControl, FX_BOOL bStrokingOperation);
	static FX_BOOL				HasColor(CPDF_FormControl* pFormControl, FX_BOOL bStrokingOperation);
	static CPDF_Dictionary*		GetPageResourceDict(CPDF_Page* pPage);
	static CFX_ByteString		GenerateNewResourceName(const CPDF_Dictionary* pResDict, FX_LPCSTR csType, int iMinLen, FX_LPCSTR csPrefix);
	static FX_BOOL				SetFontFlag(CPDF_Font* pFont);
	static CPDF_Dictionary*		GetResourceDict(CPDF_FormControl* pFormControl, CFX_ByteString as = "");
	static CPDF_Stream*			GetAP(CPDF_FormControl* pFormControl);
	static CFX_ByteString		GetSigAppearanceStream(CPDF_Document* pDocument, CPDF_FormField* pSigField, CPDF_Stream*& pImage, CFX_ByteString sImageName, CFX_DIBitmap* sealBmp, SIGAPT apType);
	static int					LoadImageFromBuffer(CPDF_Document* pDocument, FX_INT32 nImageType, FX_DWORD dwImageSize, FX_LPBYTE lpImageBuffer, CPDF_Stream*& pStream, CFX_ByteString sImageName);
	static int					LoadImageFromBMP(CPDF_Document* pDocument, CFX_DIBitmap* pBitmap, CPDF_Stream*& pStream, CFX_ByteString sImageName);
	static int					AddImageResource(CPDF_Document* pDocument, CPDF_Image* pImage, CPDF_Stream*& pStream, CFX_ByteString sImageName);
	static void					WriteSigAP(CPDF_Document* pDoc, CPDF_Page* pPage, CPDF_FormField* pSigField, CPDF_Stream*& pStream, CFX_ByteString sImageName, CFX_DIBitmap* sealBmp, SIGAPT apType = AP_N);
	static CPDF_Stream*			FindExistingAPStream(CPDF_Document* pDocument, const CFX_ByteString &bsAPName);
	static CPDF_Stream*			CreateSigAPWithoutImage(CPDF_Document* pDocument, CPDF_Page* pPage, CPDF_FormField* pSigField, CFX_ByteString sAPStream);
	static void					HexCryptBuffer(const FX_BYTE* pOriginBuf, FX_DWORD nOriginLen, FX_BYTE* pCryptedBuf);
	static FX_DWORD				HexDeCryptBuffer(const FX_BYTE* pOriginBuf, FX_DWORD nOriginLen, FX_BYTE* pDeCryptedBuf);
	static FX_BOOL				RemoveTempAP(CPDF_Document* pDoc);
	static FX_BOOL				ValidateFieldName(CPDF_InterForm* pPDFInterForm, const CFX_WideString& wsFieldName);
};

#endif //_FS_PDFUTILS_H_
