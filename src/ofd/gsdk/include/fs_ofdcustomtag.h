#ifndef _FS_OFDCUSTOMTAG_H_
#define _FS_OFDCUSTOMTAG_H_

//
class CFS_OFDDocument;

//
class CFS_OFDContentObject;
class CFS_OFDCustomTag;
class CFS_OFDCustomTags;
class CFS_OFDTagNode;
class CFS_OFDTagTree;
class CFS_OFDOfficeNode;
class CFS_OFDOfficeTree;


//
class CFS_OFDCustomTag : public CFX_Object
{
public:
	CFS_OFDCustomTag();
	~CFS_OFDCustomTag();

	FX_BOOL Create(CFS_OFDCustomTags* pCustomTags, IOFD_CustomTag* pCustomTag = NULL);

	CFX_WideString GetType();

	FX_BOOL Export2File(FX_LPCWSTR lpszFile);
	FX_BOOL	Export2File(char* pBuf, FX_INT32& nLen);

	void SetType(FX_WSTR wsType);

	FX_BOOL SetFile(FX_LPCWSTR lpszFile);
	FX_BOOL SetFile(FX_LPBYTE pBuf, FX_INT32 nLen);

	IOFD_WriteCustomTag* GetWriteCustomTag();
	IOFD_CustomTag* GetCustomTag();

protected:
	CFS_OFDCustomTags* m_pCustomTags;
	IOFD_WriteCustomTag* m_pWriteCustomTag;
};

//
class CFS_OFDCustomTags : public CFX_Object
{
public:
	CFS_OFDCustomTags();
	~CFS_OFDCustomTags();

	FX_BOOL Create(CFS_OFDDocument* pDocument);

	void SetCustomTagInfo(const CFX_WideString& wsType, const CFX_WideString& wsTagNameSpace = CFX_WideString::FromUTF8(""), const CFX_WideString& wsTagName = CFX_WideString::FromUTF8(""), const CFX_WideString& wsFileExtName = CFX_WideString::FromUTF8(""));

	FX_INT32 CountCustomTags();
	CFS_OFDCustomTag* GetCustomTag(FX_INT32 nIndex);
	CFS_OFDCustomTag* InsertCustomTag(FX_INT32 nIndex = -1);

	CFS_OFDTagTree* GetTagTree();
	CFS_OFDTagTree* CreateTagTree();

	CFS_OFDOfficeTree* GetOfficeTree();
	CFS_OFDOfficeTree* CreateOfficeTree();

	IOFD_WriteDocument* GetWriteDocument();
	IOFD_WriteCustomTags* GetWriteCustomTags();

	IOFD_CustomTags* GetCustomTags();

	CFS_OFDDocument* GetDoc();

protected:
	CFS_OFDCustomTag* GetCustomTagByIndex(FX_INT32 nIndex);

protected:
	CFS_OFDDocument* m_pDocument;
	IOFD_CustomTags* m_pCustomTags;
	IOFD_WriteCustomTags* m_pWriteCustomTags;
	CFX_PtrList* m_pCustomTagList;
	CFS_OFDTagTree* m_pTagTree;
	CFS_OFDOfficeTree* m_pOfficeTree;
	CFX_WideString m_wsTagType; //设置CustomTag的TypeID
	CFX_WideString m_wsTagNameSpace; //设置语义树的NameSpace
	CFX_WideString m_wsTagName; //设置语义树根节点名称
	CFX_WideString m_wsTagFileExtName; //设置语义树文件扩展名称
};

//
class CFS_OFDTagNode : public CFX_Object
{
public:
	CFS_OFDTagNode(CFS_OFDTagTree* pOfficeTree, IOFD_WriteCustomContents* pWriteCustomContents);
	CFS_OFDTagNode(CFS_OFDTagTree* pOfficeTree, IOFD_CustomContents* pCustomContents);
	~CFS_OFDTagNode();

	FX_INT32 Count();
	CFS_OFDTagNode* Get(FX_INT32 index);
	CFX_WideString GetTagName();
	CFX_WideString GetText();

	FX_BOOL IsGroup();
	FX_INT32 CountObject();
	FX_BOOL	GetObjID(FX_INT32 nIndex, FX_INT32& objID, FX_INT32& pageID);
	CFS_OFDContentObject* GetPageObj(FX_INT32 nIndex);

protected:
	CFS_OFDCustomTags* GetOFDCustomTags();
	CFX_WideString GetOFDContentObjText(const COFD_ContentObject* pOFDContentObject);

protected:
	IOFD_CustomContents* m_pCustomContents;
	IOFD_WriteCustomContents* m_pWriteCustomContents;
	CFS_OFDTagTree* m_pTagTree;
};

//
class CFS_OFDTagTree : public CFX_Object
{
public:
	CFS_OFDTagTree();
	~CFS_OFDTagTree();

	FX_BOOL Create(CFS_OFDCustomTags* pOFDCustomTags, IOFD_CustomTag* pCustomTag, IOFD_CustomDocGroup* pRoot);

	FX_BOOL BeginTag(const CFX_WideString& wsTagPath, FX_BOOL bElementUnique);
	FX_BOOL InsertMetaData(const CFX_WideString& wsData, const CFX_WideString& wsType, FX_INT32 nIndex);
	FX_BOOL EndTag(const CFX_WideString& wsTagPath);

	FX_BOOL ClearTagNode();
	
	IOFD_WriteCustomDocElement* GetCurTagElement();

	CFS_OFDTagNode* GetRootNode();
	CFS_OFDCustomTags* GetCustomTags();

	IOFD_WriteCustomDocGroup* GetTTNodeGroupByName(const CFX_WideString& wsPathName);
	IOFD_WriteCustomDocElement* GetTTNodeElementByName(const CFX_WideString& wsPathName);
	IOFD_WriteCustomDocElement* GetCurTTNodeElement();
	IOFD_WriteCustomDocElement*	CreateTTNodeElementByName(const CFX_WideString& wsPathName, FX_BOOL bElementUnique);

protected:
	CFS_OFDCustomTags* m_pOFDCustomTags;

	CFX_MapPtrTemplate<FX_INT32, IOFD_WriteCustomDocGroup*> m_TTNodePathNameHatch2Group;
	CFX_MapPtrTemplate<FX_INT32, IOFD_WriteCustomDocElement*> m_TTNodePathNameHatch2Element;
	IOFD_WriteCustomDocElement*	m_pCurTTNodeElement;
	IOFD_WriteCustomDocGroup* m_pRootWriteTTNodeGroup;
	IOFD_CustomDocGroup* m_pRootTTNodeGroup;

public:
	// 保存标引树节点封装类对象。
	IOFD_WriteCustomContents* GetWriteContents(IOFD_CustomContents* pCustomContents);
	CFS_OFDTagNode* GetTagNode(IOFD_WriteCustomContents* pWriteCustomContents);
	CFS_OFDTagNode* GetTagNode(IOFD_CustomContents* pCustomContents);
	void SetTagNode(IOFD_CustomContents* pWriteCustomContents, CFS_OFDTagNode* pNode);

	void Traversal_Storage(IOFD_CustomDocGroup* parent, CFX_PtrList& contents);
//	void Remove_Storage(CFX_PtrList& contents);

protected:
	CFX_MapPtrTemplate<IOFD_CustomContents*, CFS_OFDTagNode*> m_TagNodes;
};

//
class CFS_OFDOfficeNode : public CFX_Object
{
public:
	CFS_OFDOfficeNode(CFS_OFDOfficeTree* pOfficeTree, IOFD_WriteCustomContents* pWriteCustomContents);
	~CFS_OFDOfficeNode();

	FX_INT32 Count();
	CFS_OFDOfficeNode* Get(FX_INT32 index);
	CFX_WideString GetTagName();
	CFX_WideString GetText();
	CFX_WideString GetAttrValue(FX_LPCWSTR lpszName);
	FX_BOOL IsGroup();
	FX_BOOL GetObjID(FX_INT32 nIndex, FX_INT32& objID, FX_INT32& pageID);
	FX_INT32 CountObj();
	CFS_OFDContentObject* GetPageObj(FX_INT32 nIndex);


	CFS_OFDOfficeNode* Add(const CFX_WideString& name, FX_BOOL bGroup);
	void Delete(FX_INT32 index);
	void AddObject(FX_DWORD pageID, FX_DWORD objID);
	void DeleteObject(FX_DWORD pageID, FX_DWORD objID);

	void SetTagName(const CFX_WideString& name);
	void SetAttrValue(FX_LPCWSTR lpszName, const CFX_WideString& value);

protected:
	OFD_OFFICE_TYPE GetType();
	CFS_OFDCustomTags* GetOFDCustomTags();
	CFX_WideString GetOFDContentObjText(const COFD_ContentObject* pOFDContentObject);

protected:
	IOFD_WriteCustomContents* m_pWriteCustomContents;
	CFS_OFDOfficeTree* m_pOfficeTree;
};

//
class CFS_OFDOfficeTree : public CFX_Object
{
public:
	CFS_OFDOfficeTree();
	~CFS_OFDOfficeTree();

	FX_BOOL Create(CFS_OFDCustomTags* pCustomTags, IOFD_CustomDocGroup* pRoot);

	FX_BOOL BeginTag(const CFX_WideString& wsTagName);
	FX_BOOL EndTag(const CFX_WideString& wsTagName);
	void SetRootTagName(const CFX_WideString& wsName);

	IOFD_WriteCustomDocElement* GetCurTagElement();
	IOFD_WriteCustomDocElement*	CreateTagElementByName(const CFX_WideString& wsTagName);
	IOFD_WriteCustomDocGroup* GetTagGroupByName(const CFX_WideString& wsTagName);

	CFS_OFDOfficeNode* GetRootNode();
	CFS_OFDCustomTags* GetCustomTags();
	
protected:
	CFS_OFDCustomTags* m_pCustomTags;

	// 为实现BeginTag/EndTag标记文本内容，定义如下数据。
	CFX_WideString m_wsRootTagName;
	IOFD_WriteCustomDocGroup* m_pRootGroup;
	IOFD_WriteCustomDocElement*	m_pCurElement;
	CFX_MapPtrTemplate<FX_INT32, IOFD_WriteCustomDocGroup*> m_tagNameHatch2Group;

public:
	// 保存语义树节点封装类对象。
	IOFD_WriteCustomContents* GetWriteContents(IOFD_CustomContents* pCustomContents);
	CFS_OFDOfficeNode* GetOfficeNode(IOFD_WriteCustomContents* pWriteCustomContents);
	CFS_OFDOfficeNode* GetOfficeNode(IOFD_CustomContents* pCustomContents);
	void SetOfficeNode(IOFD_CustomContents* pWriteCustomContents, CFS_OFDOfficeNode* pNode);

	void Traversal_Storage(IOFD_CustomDocGroup* parent, CFX_PtrList& contents);
	void Remove_Storage(CFX_PtrList& contents);

protected:
	CFX_MapPtrTemplate<IOFD_CustomContents*, CFS_OFDOfficeNode*> m_OfficeNodes;
};

#endif //_FS_OFDCUSTOMTAG_H_
