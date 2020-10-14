#ifndef _FS_OFDPAGE_H_
#define _FS_OFDPAGE_H_

class CFS_OFDLayer;
class CFS_OFDBlockObject;
class CFS_OFDSearch;
class CFS_OFDPage;
class CFS_OFDDocument;

class CFS_OFDPage : public CFX_Object
{
public:
	CFS_OFDPage();
	virtual ~CFS_OFDPage();

	FX_BOOL Create(CFS_OFDDocument* pDocument, FX_INT32 nIndex, IOFD_WritePage* pWritePage = NULL);
	void Close();
	FX_BOOL IsClosed();

	FX_INT32 CountTemplatePages();

	FX_INT32 CountLayer();
	CFS_OFDLayer* GetLayer(FX_INT32 index);

	CFS_OFDLayer* AddLayer();
	CFS_OFDBlockObject* AddBlock();
	CFS_OFDLayer* InsertLayer(FX_INT32 index);
	CFS_OFDBlockObject* InsertBlock(FX_INT32 nIndex);
	void RemoveLayer(FX_INT32 index);
	void RemoveBlock(FX_INT32 nIndex);
	void RemoveAllLayer();
	void SetPageArea(OFD_PAGEAREA eArea, const CFX_RectF &rtArea);
	void SetDefaultArea();
	void SetTemplatePage(FX_INT32 iTemplateID, FX_INT32 iZorder = 0);

	CFX_RectF GetPageArea(OFD_PAGEAREA eArea);
	const COFD_Actions* GetActions();

	CFS_OFDDocument* GetDocument();
	IOFD_WritePage* GetWritePage();
	IOFD_Page* GetPage();
	COFD_WriteContentObjects* GetWriteContentObjects();
	FX_INT32 GetID();
	virtual FX_INT32 GetPageIndex();
	FX_INT32 IsTemplatePage();

	FX_BOOL Search(const CFX_WideString& wsKey, CFS_OFDSearch* pSearch);
	void GetText(CFX_WideStringArray& lines);
	void AddSigStamp(FX_INT32 nPageIndex, CFX_RectF& rect, FX_LPBYTE pPictureData, int pictureDataLen, int imageFormat);

	void PartBlack(CFX_RectF& rect);
	void SetRotate(FX_INT32 nRotate);

	void AddCopyrightWatermark();

	void SetModifiedFlag();

	void ObjInfoCover(CFX_RectF& rect);
	void ObjInfoCover2(CFX_RectF& rect, int shape, int mode, int horzBlur = 33, int vertBlur = 32, int v = 4);

	// For EISDK
	FX_BOOL GetFormData(FX_LPBYTE lpbData, FX_INT32 *piLen);
	IFX_FileStream* GetFormData();
	FX_BOOL SetFormData(IFX_FileStream* pFileStream);

	// END EISDK

	void CopyPage(CFS_OFDPage* pSrcPage);
protected:
	COFD_ContentLayer* GetContentLayerByIndex(FX_INT32 index);
	CFS_OFDLayer* GetLayerByIndex(FX_INT32 index, FX_POSITION* posVal = NULL);
	CFS_OFDBlockObject* GetBlockByIndex(FX_INT32 index, FX_POSITION* posVal = NULL);

	FX_BOOL AddConverImage(CFX_RectF& rect, int shape, int mode, int horzBlur, int vertBlur, int v);
	void ReplaceConverPageObj(CFX_RectF& rect, int shape, FX_BOOL ignoreImageLayer = FALSE);
	void ReplaceTextObj(COFD_TextObject* pTextObj, CFX_RectF& rect);
	void ReplaceImageObj(COFD_ImageObject* pImageObj, CFX_RectF& rect, int shape);

protected:
	CFS_OFDDocument* m_pDocument;
	IOFD_WritePage* m_pWritePage;
	COFD_WriteContentObjects* m_pWriteContentObjects;
	CFX_PtrList* m_pLayers;
	CFX_PtrList* m_pBlocks;
	CFX_DIBitmap* m_pRenderBitmap;
	FX_BOOL	m_bTemplatePage;
	FX_BOOL m_bClosed;
};

#endif //_FS_OFDPAGE_H_
