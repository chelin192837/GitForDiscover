#ifndef _FR_OFDDOCIMP_H_
#define _FR_OFDDOCIMP_H_

#include "../../include/common.h"
#include "../../include/fs_pdfdoc.h"
#include "fr_ofdutil.h"

class FR_OFDOutlineImp;

class FR_DocProvider : public IOFD_DocProvider
{
public:
	virtual CFX_DIBitmap*		GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile);

	virtual IOFD_FileStream*	GetSealFile(IOFD_Document* pDoc, IOFD_FileStream* pSealFile);
	virtual OFD_SEALTYPE		GetSealType(IOFD_FileStream* pSealFile) const;
	virtual void				SetSignVersion(OFD_SIGNVERSION eVersion) {};
private:
	CFX_DIBitmap*				CreateSealBmp(FX_LPBYTE pdata, FX_INT32 len);

	//==============================================================================================
public:
	virtual CFX_DIBitmap * GetSealBitmap(IOFD_Document *pDoc, IOFD_FileStream *pSealFile, COFD_StampAnnot *pStampAnnot);
	virtual CFX_DIBitmap * GetSealBitmap(IOFD_Document *pDoc, IOFD_FileStream *pSealFile, OFD_SEALBITMAPTYPE type, COFD_StampAnnot *pStampAnnot);
//	virtual IOFD_FileStream *GetSealFile(IOFD_Document *pDoc, IOFD_FileStream *pSealFile) = 0;
//	virtual OFD_SEALTYPE GetSealType(IOFD_FileStream *pSealFile) const { return OFD_SEAL_BITMAP; }
//	virtual void SetSignVersion(OFD_SIGNVERSION eVersion) = 0;
	virtual OFD_OESVERSION GetOesVersion(COFD_Signature *pSignature);
	virtual OFD_SEALBITMAPTYPE GetSealBitmapType();
	virtual FX_BOOL IsOesLibExist();
	CFX_DIBitmap* GetSignedValueBitmap(const unsigned char *pSignBuf, int nLen);
	CFX_DIBitmap*   RenderVetorBitmap(FX_LPBYTE pdata, FX_INT32 len,int width,int height);
	CFX_DIBitmap* FS_CreateSealBitmap(FX_LPBYTE pdata, FX_INT32 len);\
	CFX_DIBitmap* FS_LoadImage(FX_LPCBYTE pBuf, FX_INT32 nSize);
	CFX_DIBitmap* FS_LoadImage(IFX_FileRead* pFileRead);

	//==============================================================================================
	bool
	ParseSignedvalueImageMrsdk(const unsigned char *pBuf, int nBufSize,
							   unsigned char *&pImageDataBuf,
							   int &nImageSize2Res, int *pImgSubType);
};

//class FR_OFDDocImp
class FR_OFDDocImp : public FSPDFDoc, public CFX_Object
{
public:
	/*************virtual function begin*************/
	virtual void				release();
	virtual FS_ERRORCODE		load(const char password[], FS_INT32 passwordLen);
	virtual FS_BOOL				isEncrypted() const;
	virtual FS_BOOL				isModified() const;
	virtual FS_PASSWORDTYPE		getPasswordType() const;
	virtual FS_ENCRYPTTYPE		getEncryptionType() const;
	virtual FS_PASSWORDTYPE		checkPassword(const char password[], FS_INT32 passwordLen);
	virtual FS_BOOL				saveAs(const char* filePath, FS_DWORD saveFlags);
    virtual string              saveAsStream(FS_DWORD saveFlags);   // DSS 20200901
	/** @brief	Get bookmark root node*/
	virtual FSBookmark*			getFirstBookmark();
	virtual FSBookmark*			createFirstBookmark();
	virtual FS_BOOL				removeBookmark(FSBookmark* bookmark);
	// OFD page
	virtual FS_INT32			getPageCount();
	virtual FSPDFPage*			getPage(FS_INT32 pageIndex);
	virtual FS_BOOL				closePage(FS_INT32 pageIndex);
	virtual FS_BOOL				closePage(FSPDFPage* pPage);
	virtual FS_DISPLAYMODE		getDisplayMode() const;
	// OFD object and dictionary methods
	virtual FSPDFDictionary*	getCatalog() const;
	virtual FSPDFDictionary*	getTrailer() const;
	virtual FSPDFDictionary*	getInfo() const;
	virtual FSPDFDictionary*	getEncryptDict() const;
	virtual FSPDFObject*		getIndirectObject(FS_DWORD objnum);
	virtual FS_DWORD			addIndirectObject(FSPDFObject* pObj);
	virtual void				deleteIndirectObject(FS_DWORD objnum);
	//Permission
	virtual FS_DWORD			getUserPermissions() const;
	// Wrapper
	/* @brief	Whether or not wrapper file*/
	virtual FS_BOOL				isWrapper() const;
	virtual FSWrapperData		getWrapperData() const;
	virtual FS_INT32			getWrapperOffset() const;
	// Metadata
	virtual FS_BOOL				hasMetadataKey(const char* key) const;
	virtual FSDateTime			getCreationDateTime() const;
	virtual FSDateTime			getModifiedDateTime() const;
	/** @brief	Get metadata value.	---keys:	 
	 *	<li>"Title"</li> 
	 *	<li>"Author"</li>
	 *	<li>"Subject"</li>
	 *	<li>"Keywords"</li>
	 *	<li>"Creator"</li>
	 *	<li>"Producer"</li>
	 *	<li>"Trapped"</li>
	 *	<li>"pdfaid"</li>*/
	virtual FSString			getMetadataValue(const char* key) const;
	// Page label
	virtual FS_INT32			getPageLabelRangeCount() const;
	virtual FSPageLabel			getPageLabelInfo(FS_INT32 index) const;
	// form
	virtual FS_BOOL				hasForm() const;
	virtual FSForm*				getForm();
	// reading bookmark
	virtual FS_INT32			getReadingBookmarkCount();
	virtual FSReadingBookmark*	getReadingBookmark(FS_INT32 index);
	virtual FSReadingBookmark*	insertReadingBookmark(FS_INT32 readingBookmarkIndex, const char* title, FS_INT32 destPageIndex);
	virtual FS_BOOL				removeReadingBookmark(FSReadingBookmark* readingBookmark);
	// signature
	virtual FS_INT32			getSignatureCount();
	virtual FSSignature*		getSignature(FS_INT32 index);
	/*************virtual function end*************/


public:
	/*************class FR_OFDDocImp*************/
	FR_OFDDocImp(FS_INT32 loadType);
	~FR_OFDDocImp();

	static FR_OFDDocImp*		creatImp(FS_INT32 loadType, void* pInitParam = NULL, FS_INT32 bufLen = 0, FS_BOOL bReleaseBuffer = FALSE);
	static FR_OFDDocImp*		createFromFilePath(const char* path);
	static FR_OFDDocImp*		createFromMemory(const void* buffer, FS_INT32 bufLen, FS_BOOL bReleaseBuffer, FS_BOOL bOfd=true);	// DSS 20200901
	IOFD_Document*				getOFDDocument() { return m_pOFDDocument; }
	IOFD_WriteDocument*			getOFDWriteDocument() {
		if (m_pOFDWriteDocument == NULL) {
			return m_pOFDWriteDocument = OFD_WriteDoc_Create(m_pOFDDocument);
		}
		return m_pOFDWriteDocument;
	}
	IOFD_Parser*				getOFDParser(){return m_pOFDParser;}

	void					setModified();

private:
	/*************virtual function Imp begin*************/
	FSDateTime					getMetadataTime(CFX_WideStringC wstrTimeKey) const;
	FSPDFPage*					getPageImp(FS_INT32 pageIndex, FS_BOOL bRetain);
	FS_ERRORCODE				loadImp(const char password[], FS_INT32 passwordLen, FS_BOOL bReload);
	/*************virtual function Imp end*************/
public:
	FSCRT_DEFINELOCK(m_lockPages);

private:
	//ofd api
	IFX_FileRead*					m_pOFDFileRead;
	IOFD_FilePackage*				m_pOFDFilePackage;
	IOFD_Parser*					m_pOFDParser;
	IOFD_Document*					m_pOFDDocument;
	IOFD_WriteDocument*				m_pOFDWriteDocument;
	COFD_StandardSecurityHandler*	m_pOFDStandardSecurityHandler;
	COFD_StandardCryptoHandler*		m_pOFDStandardCryptoHandler;

	IOFD_RenderContext*				m_pOFDRenderContext;
	IOFD_ProgressiveRenderer*		m_pOFDProgressiveRenderer;

	//class api
	void*							m_pInitParam;
	FS_UINT16						m_uLoadDocType;
	FS_BOOL							m_isModified;
	unsigned long                   m_bufflen;

	CFX_MapPtrToPtr*				m_pMapPage;
	CFX_MapPtrToPtr*				m_pMapFontDict2FSFont;

	FSCRT_DEFINELOCK(m_lockModified);

	COFD_Signatures*                m_Signs;

protected:
	AutoPtr<FR_DocProvider>			m_DocProvider;

	FR_OFDOutlineImp*	m_pOutlineRoot;
};

#endif
