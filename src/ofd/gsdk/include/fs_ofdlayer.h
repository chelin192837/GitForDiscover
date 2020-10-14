#ifndef _FS_OFDLAYER_H_
#define _FS_OFDLAYER_H_

class CFS_OFDContentObject;
class CFS_OFDTextObject;
class CFS_OFDPathObject;
class CFS_OFDImageObject;
class CFS_OFDVideoObject;
class CFS_OFDCompositeObject;
class CFS_OFDBlockObject;
class CFS_OFDSearchResult;
class CFS_OFDSearch;
class CFS_OFDLayer;
class CFS_OFDPage;

class CFS_OFDLayer : public CFX_Object
{
public:
	CFS_OFDLayer();
	~CFS_OFDLayer();

	FX_BOOL Create(CFS_OFDPage* pPage, COFD_ContentLayer* pContentLayer = NULL);

	FX_INT32 CountPageObject();
	CFS_OFDContentObject* GetPageObject(FX_INT32 index);
	FX_INT32 GetPageObjIndexByID(FX_INT32 PageObjID);
	FX_INT32 GetPageObjIndexByName(const CFX_WideString& wsName);

	CFS_OFDTextObject* AddTextObject();
	CFS_OFDPathObject* AddPathObject();
	CFS_OFDImageObject* AddImageObject();
	CFS_OFDVideoObject* AddVideoObject();
	CFS_OFDCompositeObject* AddCompositeObject();
	void RemoveObject(FX_INT32 index);
	void RemoveAllObject();

	void SetLayerType(const CFX_WideString& wsLayerType);

	CFS_OFDPage* GetPage();
	COFD_ContentLayer* GetContentLayer();
	COFD_WriteContentLayer* GetWriteContentLayer();

	void SetModifiedFlag();

	void CopyLayer(CFS_OFDLayer* pSrcLayer);
	void AddPageObject(OFD_CONTENTTYPE eType, FS_FORMBOX_INFO* pFormInfo, CFX_MapByteStringToPtr* pContent);
protected:
	CFS_OFDContentObject* GetPageObject(COFD_ContentObject* pContentObject, FX_POSITION* posVal = NULL);
	void ClearAllPageObjects();

protected:
	CFS_OFDPage* m_pPage;
	COFD_WriteContentLayer* m_pWriteContentLayer;

	CFX_PtrList* m_pTextObjs;
	CFX_PtrList* m_pPathObjs;
	CFX_PtrList* m_pImageObjs;
	CFX_PtrList* m_pVideoObjs;
	CFX_PtrList* m_pCompositeObjs;
};

#endif //_FS_OFDLAYER_H_
