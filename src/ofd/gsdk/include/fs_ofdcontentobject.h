#ifndef _FS_OFDCONTENTOBJECT_H_
#define _FS_OFDCONTENTOBJECT_H_

class CFS_OFDContentObject;
class CFS_OFDLayer;
class CFS_OFDPage;
class CFS_OFDDocument;
class CFS_OFDClipRegion;
struct OFD_AxialShd;
struct OFD_Pattern;

class CFS_OFDContentObject : public CFX_Object
{
public:
	CFS_OFDContentObject();
	virtual ~CFS_OFDContentObject();

	FX_BOOL Create(CFS_OFDLayer* pLayer, COFD_ContentObject* pContentObj = NULL);
	FX_BOOL Create(COFD_WriteContentObject* pWriteContentObj);

	OFD_CONTENTTYPE GetType();
	CFX_WideString GetName();
	FX_BOOL IsVisible();
	FX_FLOAT GetLineWidth();
	OFD_LINECAP GetLineCap();
	OFD_LINEJOIN GetLineJoin();
	FX_FLOAT GetMiterLimit();
	FX_FLOAT GetDashOffset();
	void GetDashPattern(FX_FLOAT *fpDashPatterns, FX_INT32 *iCount);
	FX_BYTE GetAlpha();
	FX_ARGB GetFillColor();
	FX_ARGB GetStrokeColor();
	void GetBoundary(CFX_RectF &rtBoundary);
	void GetCTM(CFX_Matrix& matrix);
	const COFD_Actions* GetActions();

	void SetName(const CFX_WideString& wsName);
	void SetInvisible(FX_BOOL bSet);
	void SetBoundary(const CFX_RectF& rtBoundary);
	void SetFillColor(FX_ARGB dwFillColor);
	void SetStrokeColor(FX_ARGB dwStrokeColor);
	void SetAlpha(FX_BYTE alpha);
	void SetLineWidth(FX_FLOAT fLineWidth);
	void SetLineCap(FX_INT32 nLineCap);
	void SetLineJoin(FX_INT32 nLineJoin);
	void SetMiterLimit(FX_FLOAT fMiterLimit);
	void SetDashPattern(FX_FLOAT fDashOffset, FX_FLOAT* pfDashPatters, FX_INT32 iCount);
	void SetCTM(const CFX_Matrix &matrix);
	void SetFillPattern(OFD_PATTERN* pPattern);
	void SetFillAxialShd(OFD_AXIALSHD* pAxialShd);
	void SetFillRadialShd(OFD_RADIALSHD* pRadialShd);
	CFS_OFDClipRegion* CreateClipRegion();
	COFD_WriteActions* CreateActions();

	COFD_ContentObject* GetContentObject();
	COFD_WriteContentObject* GetWriteContentObject();

	FX_DWORD GetID();

	void SetModifiedFlag();

	CFS_OFDDocument* GetDocument();
	CFS_OFDPage* GetPage();
	CFS_OFDLayer* GetLayer();
	FX_INT32 GetPageIndex();
	
	void CopyContentObj(CFS_OFDContentObject* pSrcObj);
protected:
	virtual OFD_CONTENTTYPE GetWriteContentType() = 0;
	void InitWriteDrawParam();
	COFD_DrawParam* GetDrawParam();

	COFD_Color* GetColor(FX_BOOL bStroke);
	void InitColor(FX_BOOL bStroke, OFD_COLORTYPE eColorType);

protected:
	CFS_OFDLayer* m_pLayer;
	COFD_WriteContentObject* m_pWriteContentObject;

	COFD_ContentObject* m_pContentObject;

	//
	COFD_WriteDrawParam* m_pWriteDrawParam;

	// Fill Color
	COFD_WriteColor* m_pFillColor;

	// Stroke Color
	COFD_WriteColor* m_pStrokeColor;

	CFS_OFDClipRegion* m_pClipRegion;

	FX_BOOL m_bFillState;
	FX_BOOL m_bStrokeState;
};

#endif //_FS_OFDCONTENTOBJECT_H_
