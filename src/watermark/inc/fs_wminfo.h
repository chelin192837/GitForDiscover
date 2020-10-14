#ifndef _FS_WATERMARK_INFO_H_
#define _FS_WATERMARK_INFO_H_

#include "../../fxcore.h"
#include "../../../include/common.h"
#include "fs_watermark.h"

//Flags used for GetMatrix
#define FX_WM_ROTATION		0x01
#define FX_WM_SCALE			0x02
#define FX_WM_OFFSET		0x04
#define FX_WM_MASKALL		(FX_WM_ROTATION|FX_WM_SCALE|FX_WM_OFFSET)
#define FX_WM_MASKNOOFFSET	(FX_WM_ROTATION|FX_WM_SCALE)

class CFSPDF_WatermarkInfo : public CFX_Object
{
public:
	CFSPDF_WatermarkInfo();
	~CFSPDF_WatermarkInfo();

	FS_BOOL			CreateWatermark(CPDF_Document *pPDFDoc, CFX_WideString wText, /*IFX_FontEx* pFontEx, */CPDF_Font* pPDFFont, FX_FLOAT fFontSize, FX_ARGB color, 
									FS_BOOL bUnderline, FX_FLOAT linespace, FX_DWORD blockAlignment, FX_BOOL bSDKWatermark = FALSE);
	//FS_BOOL			CreateWatermark(CPDF_Document *pPDFDoc, IFX_Image *pImage, FX_HIMAGE hImage);
	//FS_BOOL			CreateWatermark(CPDF_Document *pPDFDoc, CFX_DIBitmap *pBitmap);
	//FS_BOOL			CreateWatermark(CPDF_Document *pPDFDoc, const CPDF_Page *pSrcPage, CPDF_ObjectArray* ObjectArray);

	FS_BOOL			InsertIntoPage(const FSPDF_WATERMARK_SETTINGS* settings, CPDF_Page *pPage, CPDF_Dictionary* pAnnotDict);

	void			UpdateSettingsXML(CFX_ByteString& xmlData);

	FS_BOOL			GetWaterMarkSize(FS_FLOAT* width, FS_FLOAT* hight);

	CFSPDF_Watermark* GetPDFWatermark(); 
protected:
	void				AddOCProperties(CPDF_Document *pPDFDoc, CPDF_Dictionary *pOCGDict, const CFX_ByteString& name, CPDF_Array* pArrayCategory);
	//	Create water mark
	void				CreateBlankFormObj(CPDF_Document *pPDFDoc, FX_BOOL bSDKWaterMark  = FALSE,FX_BOOL bTransparency = FALSE);
	CPDF_Dictionary*	CreateOCMD(CPDF_Document *pPDFDoc, const FSPDF_WATERMARK_SETTINGS* pSettings);	

	// insert into page
	void				CreateContentData(const CPDF_Page *pPage, const FSPDF_WATERMARK_SETTINGS* pSettings, CPDF_FormObject *pFormObj);
	CFX_AffineMatrix	GetMatrix(const CPDF_Page *pPage, CPDF_Rect rtWatermark, const FSPDF_WATERMARK_SETTINGS* pSettings, FX_DWORD dwFlag = FX_WM_MASKALL);
	FX_FLOAT			FitPageBox(const CFX_AffineMatrix &mt, CPDF_Rect rtPageBox, CPDF_Rect rtWM);
	void				DoRotation(const CPDF_Page *pPage, CFX_AffineMatrix &mt, const CPDF_Rect &rtPageBox, CPDF_Rect rtWM, FX_FLOAT fRotation);
	void				DoAlign(const CPDF_Page *pPage, CFX_AffineMatrix &mt, CPDF_Rect rtPageBox, CPDF_Rect rtWM, const FSPDF_WATERMARK_SETTINGS* pSettings);
	CPDF_Font*			GetPDFFont(IFX_Font *pFont);

	//	CPDF_Annot*			CreateAnnot(CPDF_Document *pPDCDoc, const FSPDF_WATERMARK_SETTINGS* pSettings);
	CPDF_Dictionary*	CreateAP(CPDF_Document *pPDFDoc);
	void				CreateContentData(const CPDF_Page *pPage, const FSPDF_WATERMARK_SETTINGS* pSettings, CPDF_FormObject *pFormObj, CPDF_Dictionary *pAnnotDict);

protected:
	CPDF_FormObject*	m_pFormObj;
	CFSPDF_Watermark*	m_pParent;
};

#endif //_FS_WATERMARK_INFO_H_