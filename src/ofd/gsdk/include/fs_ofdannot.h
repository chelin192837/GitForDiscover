#ifndef _FS_OFDANNOT_H_
#define _FS_OFDANNOT_H_

class CFS_OFDAnnot;
class CFS_OFDPageAnnots;

class CFS_OFDAnnot : public CFX_Object
{
public:
	CFS_OFDAnnot();
	~CFS_OFDAnnot();

	FX_BOOL Create(CFS_OFDPageAnnots* pPageAnnots, OFD_ANNOTTYPE eType, COFD_Annot* pAnnot = NULL);

	FX_DWORD GetID();
	FX_INT32 GetIndex();
	OFD_ANNOTTYPE GetType();
	CFX_WideString GetCreator();
	void GetLastModDate(FX_INT32& year, FX_BYTE& month, FX_BYTE& day, FX_BYTE& hour, FX_BYTE& minute, FX_BYTE& second, FX_WORD& millisecond);
	FX_BOOL GetInvisible();
	FX_BOOL GetPrintable();
	FX_BOOL GetNoZoom();
	FX_BOOL GetNoRotate();
	FX_BOOL GetReadOnly();
	CFX_WideString GetRemark();
	CFX_RectF GetBoundary();
	CFX_WideString GetSubType();
	CFS_OFDBlockObject* GetAppearance();
	const COFD_Actions* GetActions(int index);

	int CountAppearanceObj();
	CFS_OFDContentObject* GetAppearanceObj(int index);
	int Link_GetDest(int* iPage, OFD_POINT* destPoint);
	int Link_GetURI(CFX_WideString& wsURI);

	void SetCreator(const CFX_WideString& wsCreator);
	void SetLastModDate(FX_INT32 year, FX_BYTE month, FX_BYTE day, FX_BYTE hour, FX_BYTE minute, FX_BYTE second, FX_WORD millisecond);
	void SetInvisible(FX_BOOL bSet);
	void SetPrintable(FX_BOOL bSet);
	void SetNoZoom(FX_BOOL bSet);
	void SetNoRotate(FX_BOOL bSet);
	void SetReadOnly(FX_BOOL bSet);
	void SetRemark(const CFX_WideString& wsRemark);
	void SetParameters(COFD_WriteParameters* pWriteParams);
	void SetBoundary(const CFX_RectF& boundary);
	void SetSubType(const CFX_WideString& wsSubType);
	void SetVertices(const CFX_WideString& wsVertices);
	CFS_OFDBlockObject* CreateAppearance();
	void SetAppearance(CFS_OFDBlockObject* pBlock);

	CFS_OFDContentObject* AddAppearanceObj(int pageObjType);
	CFS_OFDBlockObject*	AddAppearanceBlockObj();
	COFD_Actions* CreateActions();
	void Link_SetDest(int iPage, OFD_POINT destPoint);
	void Link_SetURI(const CFX_WideString& wsUri);

	COFD_WriteAnnot* GetWriteAnnot();
	COFD_Annot* GetAnnot();
	CFS_OFDPageAnnots* GetPageAnnots();
protected:
	CFS_OFDPageAnnots* m_pPageAnnots;
	COFD_WriteAnnot* m_pWriteAnnot;
	CFS_OFDBlockObject* m_pBlockObject;
};

class CFS_OFDPageAnnots : public CFX_Object
{
public:
	CFS_OFDPageAnnots();
	~CFS_OFDPageAnnots();

	FX_BOOL Create(CFS_OFDDocument* pDoc, FX_INT32 page_index, IOFD_PageAnnots* pPageAnnots, IOFD_WritePageAnnots* pWritePageAnnots);

	FX_INT32 Count();
	CFS_OFDAnnot* Get(FX_INT32 index);
	CFS_OFDAnnot* Add(OFD_ANNOTTYPE eType);

	CFS_OFDAnnot* Move(int nIndex);

	CFS_OFDPage* GetPage();
	FX_INT32 GetPageIndex();
	IOFD_WriteDocument* GetWriteDocument();
	FX_BOOL RemoveAnnot(int index);
	int GetAnnotIndex(CFS_OFDAnnot* annot);

protected:
	CFS_OFDAnnot* FindAnnot_Storage(COFD_Annot* pAnnot);

	CFS_OFDDocument* m_pDoc;
	FX_INT32 m_pageIndex;
	IOFD_PageAnnots* m_pPageAnnots;
	IOFD_WritePageAnnots* m_pWritePageAnnots;

	CFX_PtrList* m_annots;

	friend class CFS_OFDAnnot;
};

class CFS_OFDPageAnnotsMgr : public CFX_Object
{
public:
	CFS_OFDPageAnnotsMgr(CFS_OFDDocument* pDoc);

	int Export(int start_page, int end_page, FX_LPCWSTR filepath);
	int Import(FX_LPCWSTR filepath);

protected:
	int ExportPageAnnots(IOFD_PageAnnots* pageAnnots, CXML_Element* pRootXML);
	int ExportAnnot(const COFD_Annot* pAnnot, CXML_Element* pPageAnnotsXML);
	int ExportAnnotAttribute(const COFD_Annot* pAnnot, CXML_Element* pAnnotXML);
	int ExportAnnotRemark(const COFD_Annot* pAnnot, CXML_Element* pAnnotXML);
	int ExportAnnotParameters(const COFD_Parameters* pParameters, CXML_Element* pAnnotXML);
	int ExportAnnotAppearance(const COFD_BlockObject* pBlockObject, CXML_Element* pAnnotXML);
	int ExportBlockObject(COFD_BlockObject* pBlockObj, CXML_Element* pParentXML);
	int ExportTextObject(COFD_TextObject* pTextObj, CXML_Element* pParentXML);
	int ExportPathObject(COFD_PathObject* pPathObj, CXML_Element* pParentXML);
	int ExportImageObject(COFD_ImageObject* pImageObj, CXML_Element* pParentXML);
	int ExportContentObjectAttribute(COFD_ContentObject* pContentObj, CXML_Element* pContentXML);
	int ExportActions(COFD_ContentObject* pContentObj, CXML_Element* pContentXML);
	int ExportColor(COFD_ContentObject* pContentObj, CXML_Element* pContentXML, FX_BOOL bStroke);
	int ExportAbbreviatedData(COFD_PathObject* pPathObj, CXML_Element* pPathObjXML);
	int ExportTextPieceData(COFD_TextObject* pTextObj, CXML_Element* pTextObjXML);
	int ExportImageData(COFD_ImageObject* pImageObj, CXML_Element* pImageObjXML);

protected:
	int ImportPageAnnots(CXML_Element* pPageAnnotXML, int page);
	int ImportAnnot(CXML_Element* pAnnotXML, CFS_OFDPageAnnots* pOFDPageAnnots);
	int ImportAnnotAttribute(CXML_Element* pAnnotXML, CFS_OFDAnnot* pOFDAnnot);
	int ImportAnnotRemark(CXML_Element* pRemarkXML, CFS_OFDAnnot* pOFDAnnot);
	int ImportAnnotParameters(CXML_Element* pParametersXML, CFS_OFDAnnot* pOFDAnnot);
	int ImportAnnotAppearance(CXML_Element* pAppearanceXML, CFS_OFDAnnot* pOFDAnnot);
	int ImportBlockObject(CXML_Element* pBlockObjXML, CFS_OFDBlockObject* pOFDBlockObj);
	int ImportTextObject(CXML_Element* pTextObjXML, CFS_OFDBlockObject* pOFDBlockObj);
	int ImportPathObject(CXML_Element* pPathObjXML, CFS_OFDBlockObject* pOFDBlockObj);
	int ImportImageObject(CXML_Element* pImageObjXML, CFS_OFDBlockObject* pOFDBlockObj);
	int ImportContentObjectAttribute(CXML_Element* pContentXML, CFS_OFDContentObject* pContentObj);
	int ImportActions(CXML_Element* pContentXML, CFS_OFDContentObject* pContentObj);
	int ImportColor(CXML_Element* pContentXML, CFS_OFDContentObject* pContentObj, FX_BOOL bStroke);
	int ImportAbbreviatedData(CXML_Element* pPathXML, CFS_OFDPathObject* pPathObj);
	int ImportTextPieceData(CXML_Element* pTextObjXML, CFS_OFDTextObject* pTextObj);
	int ImportImageData(CXML_Element* pImageObjXML, CFS_OFDImageObject* pImageObj);

protected:
	CFS_OFDDocument* m_pDoc;
};

#endif //_FS_OFDANNOT_H_
