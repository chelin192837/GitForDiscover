#ifndef _FS_OFDDOCUMENT_H_
#define _FS_OFDDOCUMENT_H_

class CFS_OFDFilePackage;
class CFS_OFDPage;
class CFS_OFDTemplatePage;
class CFS_OFDOutline;
class CFS_OFDSearch;
class CFS_OFDCustomTags;
class CFS_OFDBookmarks;
class CFS_OFDAttachments;
class CFS_OFDDocument;
class CFS_OFDBlockObject;
class CFS_OFDPageAnnots;
class CFS_OFDAnnot;
class CFS_OFDPermissions;
struct OFD_Pattern;
struct OFD_AxialShd;
class CFS_OFDFont;

class COFD_StandardCryptoHandler;
class COFD_StandardSecurityHandler;
class CFS_SecurityData;

#define OFD_TAG_MODE_None		0
#define OFD_TAG_MODE_OfficeTree 1
#define OFD_TAG_MODE_TagTree	2

#define OFD_DOT_CHARACTER	0x2026 //...居中间隔符
#define OFD_CATALOG_SPACE	2.0f //标题 页码和点之间的间隔

#define FS_UNICODE_CATALOG_TITLE	L"\u76ee\u5f55"	//目录
#define FS_UNICODE_ATTACHMENT_TITLE	L"\u9644\u4ef6\uff1a" //附件：

typedef struct _OFD_CATALOG_FORMATINFO{
	CFX_WideString wsFontName;
	FX_FLOAT fFontSize;
	FX_FLOAT fLowerSpace;
}OFD_CATALOG_FORMATINFO;

typedef struct _OFD_CATALOG_ATTACHMENTINFO{
	//在合并文件时设置
	CFX_WideString wsName;	//附件名称
}OFD_CATALOG_ATTACHMENTINFO;

typedef struct _OFD_CATALOG_CATALOGINFO{
	//在合并文件时设置
	CFX_WideString wsTitle;	//文件标题
	FX_INT32 pageCount; //当前文档页数
	FX_INT32 pageNumer; //该文件页码号，从目录后开始算第一页
}OFD_CATALOG_CATALOGINFO;

class CFS_OFDDocument : public CFX_Object
{
public:
	CFS_OFDDocument(CFS_OFDFilePackage* pPackage);
	~CFS_OFDDocument();

	FX_BOOL Create(IOFD_Document* pDoc = NULL);
	void Close();

	IOFD_CryptoHandler* GetCryptoHandler();
	void SetCryptoHandler(IOFD_CryptoHandler* pCryptoHandler);
	IOFD_SecurityHandler* GetSecurityHandler();
	void SetSecurityHandler(IOFD_SecurityHandler* pSecurityHandler);

	CFS_OFDPermissions* GetPermissions();
	CFS_OFDPermissions* CreatePermissions();

	COFD_Actions* GetActions();

	FX_BOOL SetPassword(int encryptType, const CFX_ByteString& userPassword, const CFX_ByteString& ownerPassword);
	FX_BOOL RemovePassword(const CFX_ByteString& ownerPassword);
	FX_BOOL	IsAdmin();
	FX_BOOL CustomEncrypt(CFS_SecurityData* pSecurityData, const CFX_ByteString& userKeyField, const CFX_ByteString& ownerKeyField);
	FX_BOOL CertEncrypt(int encryptType, const CFX_WideString& certPath, const CFX_ByteString& password, FX_INT32 certType = 0);

	FX_INT32 GetIndex();

	IOFD_Document* GetDocument();
	IOFD_WriteDocument* GetWriteDocument();

	FX_INT32 CountPages();
	FX_INT32 GetPageIDByIndex(FX_INT32 nIndex);
	FX_INT32 GetPageIndexByID(FX_INT32 pageID);
	CFS_OFDPage* GetPageByIndex(FX_INT32 nIndex, FX_POSITION* posVal = NULL);
	CFS_OFDPage* LoadPage(int nIndex);

	FX_INT32 CountTemplatePages();
	CFS_OFDTemplatePage* GetTemplatePageByID(FX_INT32 nTemplatePageID);
	CFS_OFDTemplatePage* GetTemplatePageByName(const CFX_WideString& wsName);
	CFS_OFDTemplatePage* GetTemplatePageByIndex(FX_INT32 nIndex, FX_POSITION* posVal = NULL);
	CFS_OFDTemplatePage* LoadTemplatePage(int nIndex);

	CFS_OFDPage* AddPage();
	CFS_OFDPage* InsertPage(FX_INT32 nIndex);
	CFS_OFDTemplatePage* AddTemplatePage();
	CFS_OFDTemplatePage* InsertTemplatePage(FX_INT32 nIndex);

	FX_BOOL	DeletePage(FX_INT32 nIndex);
	FX_BOOL MovePage(FX_INT32 nSrcIndex, FX_INT32 nDstIndex);
	FX_BOOL	SplitPage(FX_INT32 nIndex);
	FX_BOOL	DeleteTemplatePage(FX_INT32 nIndex);
	FX_BOOL	DeleteAllTemplatePage();

	FX_BOOL MergeDoc(CFS_OFDDocument* pSrc,FX_INT32 index, FX_INT32 insertTo);

	CFS_OFDOutline* GetOutline();
	CFS_OFDOutline* GetSubOutline(CFS_OFDOutline* pParent, int index);
	CFS_OFDOutline* CreateOutline();
	CFS_OFDOutline* CreateSubOutline(CFS_OFDOutline* pParent);
	CFS_OFDOutline* FindOutline_Storage(COFD_Outline* pOutline);

	FX_BOOL GetMetaData(CFX_WideString& wsMetadataName, CFX_WideString& wsMetadataVal);
	void SetMetaData(CFX_WideString& wsMetadataName, CFX_WideString& wsMetadataValue);
	void SetCustomData(const CFX_WideString& wsName, const CFX_WideString& wsValue);

	FX_INT32 InsertKeyWord(CFX_WideString& wsKeyword, FX_INT32 nIndex = -1);
	FX_INT32 CountKeyWords();
	void RemoveKeyWord(FX_INT32 nIndex);
	void GetKeyWord(FX_INT32 nIndex, OFD_WSTR &key);

	FX_INT32 InsertCustomData(CFX_WideString& wsName, CFX_WideString& wsValue, FX_INT32 nIndex = -1);
	FX_INT32 CountCustomData();
	void GetCustomData(FX_INT32 nIndex, OFD_WSTR &data);
	void GetCustomDataName(FX_INT32 nIndex, OFD_WSTR &name);
	void RemoveCustomData(FX_INT32 nIndex);

	CFS_OFDCustomTags* GetCustomTags();
	CFS_OFDCustomTags* CreateCustomTags(); 

	CFS_OFDBookmarks* GetBookmarks();
	CFS_OFDBookmarks* CreateBookmarks();

	CFS_OFDAttachments* GetAttachments();
	CFS_OFDAttachments* CreateAttachments(); 

	CFS_OFDPageAnnots* GetPageAnnots(FX_INT32 page_index);
	CFS_OFDPageAnnots* CreatePageAnnots(FX_INT32 page_index);
	CFS_OFDPageAnnots* FindPageAnnots_Storage(FX_INT32 page_index);

	COFD_WriteResource* CreateWriteResource(OFD_RESOURCETYPE eType);
	FX_DWORD GetNextID();
	FX_INT32 GetFontID(const CFX_WideString& wsFontName);
	COFD_Resource* GetPublicResourceByID(FX_DWORD dwResID);
	void GetMultimediaFileByID(FX_DWORD dwResID,CFX_WideString& wsFile);
	OFD_PATTERN* CreatePattern(FX_FLOAT						width,			//底纹单元绘制区域的宽度(单位毫米)
								FX_FLOAT					height,			//底纹单元绘制区域的高度(单位毫米)
								FX_FLOAT					xStep,			//X方向底纹单元间距(默认为fWidth，单位毫米)
								FX_FLOAT					yStep,			//Y方向底纹单元间距(默认为fHeight,单位毫米)
								const CFX_Matrix			matrix,			//底纹单元变换矩阵
								const CFX_WideString&		imageFileName,	//指定单元图像文件名称
								FX_LPCBYTE					imageBuf,       //指定单元图像数据
								FX_INT32					imageBufLen,	//图像数据长度
								FX_INT32					imageFormat		//图像数据类型（取值范围OFDSDK_IMAGE_FORMAT_BMP，OFDSDK_IMAGE_FORMAT_JPG，OFDSDK_IMAGE_FORMAT_PNG，OFDSDK_IMAGE_FORMAT_TIF）
								);
	void DestroyPattern(OFD_PATTERN* pPattern);
	COFD_WritePattern* GetWritePattern(OFD_PATTERN* pPattern);
	COFD_WriteAxialShading* CreateWriteAxialShading(OFD_AXIALSHD* pAxialShd);
	COFD_WriteAxialShading* GetWriteAxialShading(OFD_AXIALSHD* pAxialShd);
	COFD_WriteBlockObject* CreatePatternCellContent(const CFX_WideString& imageFileName,
									FX_LPCBYTE imageBuf, FX_INT32 imageBufLen, FX_FLOAT width, FX_FLOAT height, FX_INT32 imageFormat);
	COFD_WriteRadialShading* CreateWriteRadialShading(OFD_RADIALSHD* pRadialShd);
	COFD_WriteRadialShading* GetWriteRadialShading(OFD_RADIALSHD* pRadialShd);

	FX_BOOL Search(const CFX_WideString& wsKey, CFS_OFDSearch* pSearch);
	void GetText(CFX_WideStringArray& lines);
	FX_BOOL	RemoveSignatures(FX_BOOL bChange2Image = TRUE);
	void	ChangeSig2Image(COFD_Signature*	pSig);
	void	AddSigStamp(FX_INT32 nPageIndex, CFX_RectF& rect, FX_LPBYTE pPictureData, int pictureDataLen, int imageFormat);

	IOFD_WriteCustomDocElement* GetCurTagElement();

	void RecoverHideInfo();

	// For EISDK
	FX_BOOL GetFormData(FX_INT32 nPageIndex, FX_LPBYTE lpbData, FX_INT32 *piLen);
	IFX_FileStream* GetFormData(FX_INT32 nPageIndex);
	FX_BOOL SetFormData(FX_INT32 nPageIndex, IFX_FileStream* pFileStream);
	//根据form.xml和data.xml装配数据
	FX_BOOL ParseFormAndData(IFX_FileStream* pDataFileStream);
	CFX_PtrList* GetFormInfos();
	void AssembleFormAndData();
	void ReleaseFormAndData();
	CFS_OFDPage* AddDetailPage(FX_INT32 nIndex);
	void InitTTNode(const CFX_WideString& wsTypeName, const CFX_WideString& wsTagNameSpace, const CFX_WideString& wsTagName, const CFX_WideString& wsTagFileExt, FX_BOOL bElementUnique);
	void BeginTTNode(const CFX_WideString& wsPathName);
	void InsertMetaData(const CFX_WideString& wsData, const CFX_WideString& wsType, FX_INT32 nIndex);
	void EndTTNode(const CFX_WideString& wsPathName);
	void ClearTTNode();
	FX_DWORD GetFPBarcodeFileID();
	void SetFPBarcodeRes(COFD_WriteMultimedia* pRes);
	COFD_WriteMultimedia* GetFPBarcodeRes();
	// End EISDK

	void Convert2EmbededFont(FX_LPCWSTR fontName = NULL);

	FX_INT32 GetTagMode();

	FX_DWORD GetMaxVerID();
	FX_INT32 CountVersions();
	CFX_WideString GetVersion(int nIndex);
	CFX_WideString GetVersionName(int nIndex);
	CFX_WideString GetVersionDocRoot(int nIndex);
	FX_INT32 VersionCountFiles(int nIndex);

	CFS_OFDFont* AddFont();
	FX_DWORD AddFont(const CFX_WideString& fontFamily, const CFX_WideString& fontName);
	FX_DWORD AddImageResFromFile(OFD_LPCWSTR lpwszFilename);
	FX_DWORD AddImageResFromBuf(OFD_LPBYTE lpbDataBuf, int iLen, int imageFormat);

protected:
	void LoadFontID();

protected:
	CFS_OFDFilePackage* m_pPackage;
	IOFD_CryptoHandler* m_pCryptoHandler;
	IOFD_SecurityHandler* m_pSecurityHandler;
	IOFD_CryptoHandler* m_pOldCryptoHandler;
	IOFD_SecurityHandler* m_pOldSecurityHandler;
	CFS_OFDPermissions* m_pPermissions;
	IOFD_WriteDocument* m_pWriteDocument;
	IOFD_WriteDocInfo* m_pWriteDocInfo;
	CFX_PtrList* m_pPages;						// 注意! Page Index 指的是IOFD_Document,这个链表中的CFS_OFDPage是无序的.
	CFX_PtrList* m_pTemplatePages;				// 注意! TemplatePage Index 指的是IOFD_Document,这个链表中的CFS_OFDTemplatePage是无序的.
	CFX_PtrList* m_pOutlines;
	CFX_PtrList* m_pPageAnnots;
	CFX_PtrList* m_pFonts;
	CFX_CMapDWordToDWord m_FontMap;
	CFX_MapPtrToPtr	m_PatternMap;
	CFX_MapPtrToPtr	m_AxialShdMap;
//	FX_INT32 m_dwNextID;

	CFS_OFDCustomTags* m_pCustomTags;
	CFS_OFDBookmarks* m_pBookmarks;
	CFS_OFDAttachments* m_pAttachments;

	FX_INT32 m_tagMode;
	FX_INT32 m_tagOldMode;

	COFD_WriteMultimedia* m_pFPBarcodeRes;
	FX_DWORD m_barcodeFileID;

	FX_BOOL m_bLoadSSL;

	// For EISDK
	FX_BOOL m_bElementUnique;
	CFX_PtrList* pFormInfos; //Form.xml信息
	CFX_PtrList* pDataInfos; //Data.xml信息
	// End EISDK
};

class CFS_OFDCatalog : public CFX_Object
{
public:
	CFS_OFDCatalog();
	~CFS_OFDCatalog();

	void SetMargin(FX_FLOAT leftMargin, FX_FLOAT upperMargin);
	void SetFormat(int eSection, const CFX_WideString& fontName, FX_FLOAT fontSize, FX_FLOAT space);

	//检测文档页数与目录信息是否相符
	FX_BOOL CheckPageCount(CFS_OFDDocument* pDoc);
	FX_BOOL CheckAttachments(CFS_OFDDocument* pDoc);

	//添加目录信息：标题和页码
	FX_BOOL SetCatalogInfo(const CFX_WideString& wsTitle, FX_INT32 pageCnt);
	void AddCatalogTitle(CFS_OFDDocument* pDoc); //添加目录首页，及总标题“目录”,设置“目录”字体、字体大小
	void AddCatalogContents();

	//添加附件信息
	FX_BOOL SetAttachmentInfo(CFS_OFDAttachments* pAttachs);
	void AddAttachmentTitle(CFS_OFDDocument* pDoc);
	void AddAttachmentContents();

	//添加大纲
	void AddOutlines(CFS_OFDDocument* pDoc);

private:
	FX_BOOL AddFirstCatalogPage(CFS_OFDDocument* pDoc);
	FX_BOOL CheckAddCatalogPage(CFX_RectF& rect);
	void AddPageHyperlink(FX_INT32 pageIndex, const CFX_RectF& rect, FX_INT32 destPageID);
	void AddAttachHyperlink(FX_INT32 pageIndex, const CFX_RectF& rect, FX_INT32 attchID);

	FX_INT32 m_iCurrPageNumber; //当前页码号。从目录后开始算第一页
	CFX_PtrList* m_pCatalogInfos; //目录信息
	CFX_PtrList* m_pAttachmentInfos; //附件信息
	CFS_OFDDocument* m_pDocument;
	CFS_OFDPage* m_pCurrCatalogPage;
	CFX_RectF m_WorkspaceRect; //页面工作区域大小
	FX_FLOAT m_fTop; //当前距顶的距离
	FX_INT32 m_iCatalogPage; //目录页数
	FX_BOOL m_bConverPage; //是否存在封面页
	FX_FLOAT m_fLeftMargin; //水平页边距
	FX_FLOAT m_fUpperMargin; //垂直页边距
	OFD_CATALOG_FORMATINFO* m_pstCatalogTitleInfo;
	OFD_CATALOG_FORMATINFO* m_pstCatalogContentInfo;
	OFD_CATALOG_FORMATINFO* m_pstAttachmentTitleInfo;
	OFD_CATALOG_FORMATINFO* m_pstAttachmentContentInfo;
};

#endif //_FS_OFDDOCUMENT_H_
