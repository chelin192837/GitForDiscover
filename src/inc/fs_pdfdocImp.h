#ifndef _FS_PDFDOC_IMP_H_
#define _FS_PDFDOC_IMP_H_

#include "../../include/common.h"
#include "../../include/fs_pdfdoc.h"

class FSFontImp;

class CPDF_Document;
class IFX_FileStream;
class CPDF_Parser;
class CFX_MapPtrToPtr;
class CFSCRT_Lock;
class CFX_WideStringC;
class FSForm;
class FSFormImp;
class IJS_Runtime;

class FSBookmarkImp;
class CFSCRT_STPDFActionHandler;

#define FSPDF_METADATA_rdf			"rdf"
#define FSPDF_METADATA_RDF			"RDF"
#define FSPDF_METADATA_DESCRIPTION	"description"
#define FSPDF_METADATA_PDFX			"pdfx"
#define FSPDF_METADATA_PDFXTAG		"xmlns:pdfx"
#define FSPDF_METADATA_BEGINXML		"<?xpacket begin=\"�? id=\"W5M0MpCehiHzreSzNTczkc9d\"?>\n"
#define FSPDF_METADATA_ENDXML		"\n<?xpacket end=\"w\"?>"

class FSPDFDocImp : public FSPDFDoc, public CFX_Object
{
public:
	static FSPDFDocImp*	create();
	static FSPDFDocImp*	createFromFilePath(const char* path);
	static FSPDFDocImp*	createFromMemory(const void* buffer, FS_INT32 bufLen, FS_BOOL bReleaseBuffer);
	static FSPDFDocImp*	createFromHandler(FSFileRead* pFileRead);
	virtual void release();
	virtual ~FSPDFDocImp();
	virtual FS_ERRORCODE	load(const char password[], FS_INT32 passwordLen);
	virtual FS_BOOL			reload();
	virtual FS_BOOL			isEncrypted() const;
	virtual FS_BOOL			isModified() const;
	virtual FS_PASSWORDTYPE	getPasswordType() const;
	virtual FS_ENCRYPTTYPE	getEncryptionType() const;
	virtual FS_PASSWORDTYPE	checkPassword(const char password[], FS_INT32 passwordLen);
	virtual FS_BOOL			saveAs(const char* filePath, FS_DWORD saveFlags);
	virtual string          saveAsStream(FS_DWORD saveFlags);	// DSS 20200901
	/** @brief	Get bookmark root node*/
	virtual FSBookmark*		getFirstBookmark();
	virtual FSBookmark*		createFirstBookmark();
	virtual FS_BOOL			removeBookmark(FSBookmark* bookmark);
	// PDF page
	virtual FS_INT32		getPageCount();
	virtual FSPDFPage*		getPage(FS_INT32 pageIndex);
	virtual FS_BOOL			closePage(FS_INT32 pageIndex);
	virtual FS_BOOL			closePage(FSPDFPage* pPage);
	virtual FS_DISPLAYMODE	getDisplayMode() const;
	// PDF object and dictionary methods
	virtual FSPDFDictionary*	getCatalog() const;
	virtual FSPDFDictionary*	getTrailer() const;
	virtual FSPDFDictionary*	getInfo() const;
	virtual FSPDFDictionary*	getEncryptDict() const;
	virtual FSPDFObject*		getIndirectObject(FS_DWORD objnum);
	virtual FS_DWORD		addIndirectObject(FSPDFObject* pObj);
	virtual void			deleteIndirectObject(FS_DWORD objnum);
	//Permission
	virtual FS_DWORD		getUserPermissions() const;
	// Wrapper
	/* @brief	Whether or not wrapper file*/
	virtual FS_BOOL			isWrapper() const;
	virtual FSWrapperData	getWrapperData() const;
	virtual FS_INT32		getWrapperOffset() const;
	// Metadata
	virtual FS_BOOL			hasMetadataKey(const char* key) const;
	virtual FSDateTime		getCreationDateTime() const;
	virtual FSDateTime		getModifiedDateTime() const;
	/** @brief	Get metadata value.
	 *	keys:
	 *	<li>"Title"</li> 
	 *	<li>"Author"</li>
	 *	<li>"Subject"</li>
	 *	<li>"Keywords"</li>
	 *	<li>"Creator"</li>
	 *	<li>"Producer"</li>
	 *	<li>"Trapped"</li>
	 *	<li>"pdfaid"</li>	
	 */
	virtual FSString		getMetadataValue(const char* key) const;
	// Page label
	virtual FS_INT32		getPageLabelRangeCount() const;
	virtual FSPageLabel		getPageLabelInfo(FS_INT32 index) const;
	// form
	virtual FS_BOOL			hasForm() const;
	virtual FSForm*			getForm();
	// reading bookmark
	virtual FS_INT32			getReadingBookmarkCount();
	virtual FSReadingBookmark*	getReadingBookmark(FS_INT32 index);
	virtual FSReadingBookmark*	insertReadingBookmark(FS_INT32 readingBookmarkIndex, const char* title, FS_INT32 destPageIndex);
	virtual FS_BOOL				removeReadingBookmark(FSReadingBookmark* readingBookmark);
	// signature
	virtual FS_INT32	getSignatureCount();
	virtual FSSignature*	getSignature(FS_INT32 index);

	//Internal use
	FSFormImp*				GetForm();
	void					setForm(FSFormImp* pForm) {m_pForm = pForm;}
	IJS_Runtime*			GetJsRuntime();
	FSAnnot*				GetFocusAnnot();

	FSPDFDocImp(FS_INT32 loadType);
	static FSPDFDocImp*		creatImp(FS_INT32 loadType, void* pInitParam = NULL, FS_INT32 bufLen = 0, FS_BOOL bReleaseBuffer = FALSE);
	CPDF_Document*			getPDFDocument() { return m_pDoc; }
	FSPDFPage*				getPageImp(FS_INT32 pageIndex, FS_BOOL bRetain);
	FS_DWORD				transformSaveFlag2Fxcore(FS_DWORD rdkSaveFlags);
	void					setModified();
	void					SetCurrentSystemTimeToModifyDate();
	FS_BOOL					addToFontMaps(FSFontImp* pFSFont, CPDF_Dictionary* pFontDict);
	FSFontImp*				getFSFont(CPDF_Font* pPDFFont);
	FS_BOOL					removeMapRecord(/*FSFontImp* pFSFont*/CPDF_Dictionary* pFontDict);

	void					GetPageCount(FX_INT32* nPageCount);
	void					GetPage(FX_INT32 nPageIndex, FSPDFPage** page);
	CFX_MapPtrToPtr*		getPageMap() {return m_pMapPage;}
	FS_BOOL					updateMetadataStream();

	CPDF_Parser*			getParser() { return m_pParser; }

	CPDF_Signature*			getSignatureAtPos(CPDF_Page* pPage, FSPointF point, FS_FLOAT fTolerance);
	FS_BOOL					addSignature(CPDF_Signature* pNewSig, CPDF_Page* pPage, FSRectF newRect);
	FS_BOOL					removeSignatureField(CPDF_Signature* pSig);
	FS_BOOL					clearSignedData(CPDF_Signature* pSig);
	CPDF_SignatureEdit*		getSigantureEdit();
	FSPDFPage*				getPageByDict(CPDF_Dictionary* pPageDict);

private:
	FS_ERRORCODE			loadImp(const char password[], FS_INT32 passwordLen, FS_BOOL bReload);
	
private:
	FS_BOOL			init();
	FSDateTime		getMetadataTime(CFX_WideStringC bstrTimeKey) const;
	FS_BOOL			loadSignatures();
private:
	FSBookmark*		loadFirstBookmark(FS_BOOL bCreate = FALSE);
	CXML_Element*	loadXMLReadingBookmark(FS_BOOL bCreate = FALSE);
	FS_BOOL			createNewMetadataRDF();
	FS_BOOL			initReadingBookmark(FS_BOOL bCreate = FALSE);
public:
	FSBookmarkImp*	m_pBmRoot;
	FSCRT_DEFINELOCK(m_lockPages);

	CFSCRT_STPDFActionHandler* GetActionHandler();
private:
	CPDF_Document*	m_pDoc;
	IFX_FileStream*	m_pFileStream;
	CPDF_Parser*	m_pParser;
	void*			m_pInitParam;

	FS_UINT16		m_uLoadDocType;
	FS_INT32		m_bufflen;
	FS_BOOL			m_bReleaseBuffer;
	
	CFX_MapPtrToPtr*	m_pMapPage;

	FSCRT_DEFINELOCK(m_lockModified);
	FS_BOOL			m_isModified;

	FSCRT_DEFINELOCK(m_lockFontMaps);
	////<FSFontImp*, font dictionary>
	//CFX_MapPtrToPtr*	m_pMapFSFont2FontDict;
	//<font dictionary, FSFontImp*>
	CFX_MapPtrToPtr*	m_pMapFontDict2FSFont;
	FSFormImp*			m_pForm;
	IJS_Runtime*	m_pJSRuntime;

	CPDF_Metadata* m_pMetadata;
	CXML_Element*  m_pXMLReadingBookmark;

	FSCRT_DEFINELOCK(m_lockReadingBookmarks);
	CFX_ArrayTemplate<void*>*	m_pReadingBookmarksArray; //store FSReadingBookmark.

	CPDF_SignatureEdit*	m_pSigEdit;
	CFSCRT_STPDFActionHandler* m_pSTActionHandler;
}; 

#endif //_FS_PDFDOC_IMP_H_
