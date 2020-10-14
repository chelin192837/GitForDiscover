#ifndef _FS_OFDWATERMARK_H_
#define _FS_OFDWATERMARK_H_

class CFS_OFDWaterMarkHekper;
class CFS_OFDBlockObject;
class CFS_OFDPage;
class CFS_OFDContentObject;
class CFS_OFDTextLayout;

#define OFD_WATERMARK_TYPE_TEXT		0
#define OFD_WATERMARK_TYPE_IMAGE	1

class CFS_OFDWaterMarkHekper : public CFX_Object
{
public:
	CFS_OFDWaterMarkHekper();
	~CFS_OFDWaterMarkHekper();
	
	void Init(CFS_OFDPage* hPage, const CFX_WideString& wsText, const CFX_WideString& wsFont, 
		 FX_FLOAT fFontSize, FX_FLOAT fCharSpace, FX_FLOAT fColor, FX_FLOAT fWidth, FX_FLOAT fHeight, FX_FLOAT fRotate, FX_BYTE alpha);

	void Init(CFS_OFDPage* hPage, FX_DWORD dwResID, FX_LPCWSTR lpwszImageFile,  FX_FLOAT fImageWidth, 
		FX_FLOAT fImageHeight, FX_LPBYTE pImageBuf, FX_BOOL iImageBufLen, FX_BOOL iImageFormat, 
		FX_FLOAT fWidth, FX_FLOAT fHeight, FX_FLOAT fRotate);
 	void Generate(FX_FLOAT x, FX_FLOAT y, FX_BOOL bTiled, FX_FLOAT fXStep, FX_FLOAT fYStep);
	void Generate_Pattern(FX_FLOAT x, FX_FLOAT y, FX_BOOL bTiled, FX_FLOAT fXStep, FX_FLOAT fYStep);
	void Generate2(int hAlign, int vAlign);
	void Generate3(int position, FX_FLOAT x, FX_FLOAT y);

	void UpdateTextWidth(float width);
	void UpdateTextHeight(float height);
	void UpdateImageWidth(float width);
	void UpdateImageHeight(float height);

	float GetTextWidth();
	float GetTextHeight();
	float GetImageWidth(int dpi);
	float GetImageHeight(int dpi);
protected:
	void AddText(CFS_OFDBlockObject* pOFDBlock,FX_FLOAT x, FX_FLOAT y,
		FX_BOOL bTiled, FX_FLOAT fXStep, FX_FLOAT fYStep);
	void AddImage(CFS_OFDBlockObject* pOFDBlock,FX_FLOAT x, FX_FLOAT y,
		FX_BOOL bTiled, FX_FLOAT fXStep, FX_FLOAT fYStep);
	void AddImage_Pattern(CFS_OFDBlockObject* pOFDBlock,FX_FLOAT x, FX_FLOAT y,
		FX_BOOL bTiled, FX_FLOAT fXStep, FX_FLOAT fYStep);

	void AddText2(CFS_OFDBlockObject* pOFDBlock, FX_INT32 hAlign, FX_INT32 vAlign);
	void AddImage2(CFS_OFDBlockObject* pOFDBlock, FX_INT32 hAlign, FX_INT32 vAlign);
	
	void AddText3(CFS_OFDBlockObject* pOFDBlock, int position, FX_FLOAT x, FX_FLOAT y);

	CFX_RectF SetAlign(FX_INT32 hAlign, FX_INT32 vAlign);
	void ContentObjSetInfo(CFS_OFDContentObject* contentObj, CFS_OFDTextLayout textLayout, CFX_Matrix matrix, CFX_RectF rect);

protected:
	CFS_OFDPage* m_pPage;
	CFX_WideString m_text;
	CFX_WideString m_fontName;
	FX_FLOAT m_fontSize;
	FX_FLOAT m_charSpace;
	FX_FLOAT m_rotate;
	FX_INT32 m_alpha;
	FX_ARGB m_Color;

	FX_DWORD m_imageResID;
	CFX_WideString m_imagefile;
	FX_FLOAT m_imageWidth;
	FX_FLOAT m_imageHeight;
	FX_LPBYTE m_imageBuf;
	FX_BOOL m_imageBufLen;
	FX_BOOL m_imageFormat;
	FX_FLOAT m_width;
	FX_FLOAT m_height;

	FX_INT32 m_type;
};

#endif //_FS_OFDWATERMARKHELPER_H_
