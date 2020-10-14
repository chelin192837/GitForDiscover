#ifndef _FS_BOOKMARK_IMP_H_
#define _FS_BOOKMARK_IMP_H_

class FSPDFDoc;
class FSDestination;
class CPDF_Dictionary;

class FSBookmarkImp : public FSBookmark, public CFX_Object
{
public:
	FSBookmarkImp();
	virtual ~FSBookmarkImp();

	virtual FSBookmarkImp*		getParent();
	virtual FSBookmarkImp*		getFirstChild();
	virtual FSBookmarkImp*		getNextSibling();
//	virtual FSAction*			getAction();
	virtual FSDestination*		getDestination();
	virtual void				setDestination(const FSDestination* dest);
	virtual FSString			getTitle() const;
	virtual void				setTitle(const char* title);
	virtual FS_ARGB				getColor() const ;
	virtual void				setColor(FS_ARGB color);
	virtual FS_DWORD			getStyle() const;
	virtual void				setStyle(FS_DWORD style);
	virtual FSBookmark*			insert(const char* title, FS_BOOKMARKPOSITION pos);
	virtual FS_BOOL				moveTo(FSBookmark* destBookmark, FS_BOOKMARKPOSITION pos);	

	void						init(FSPDFDoc* pDoc, CPDF_Dictionary* pBookmarkDict);

	//Internal use
	FS_BOOL						isExisted(CPDF_Dictionary* pBookmarkDict);
	FS_BOOL						isDescendant(FSBookmark* bookmark);
	FS_BOOL						getFinalGOTOAction(CPDF_Action pdfAct, CPDF_Action& gotoAction);
	CPDF_Dictionary*			getBookmarkDict();
	FS_BOOL						removeFromBMTree(FS_BOOL bRelease = TRUE);
public:
	FSBookmarkImp*		m_pBmParent;
	FSBookmarkImp*		m_pBmNextSibling;
	FSBookmarkImp*		m_pBmFirstChild;

private:
	FSPDFDoc*			m_pFSPDFDoc;
	CPDF_Dictionary*	m_pBookmarkDict;

	FSDestination*		m_pDestination;
//	FSAction*			m_pAction;

private:
	void insertPDFBookmark(FS_BOOKMARKPOSITION pos, CPDF_Dictionary* pInsertBookmarkDict);
	void insertFSBookmark(FS_BOOKMARKPOSITION pos, FSBookmarkImp* pBookmark);
	void insertBookmark(FS_BOOKMARKPOSITION pos, FSBookmarkImp* insertBookmark);

	void insertBookmarkToLastChild(FSBookmarkImp* insertBookmark);
	void insertBookmarkToPrevSibling(FSBookmarkImp* insertBookmark);

	FSBookmarkImp* getPrevSibling();
	FSBookmarkImp* getLastChild();

	FS_BOOL removeFSBookmark(FSBookmarkImp* bookmark);
};
#endif //_FS_BOOKMARK_IMP_H_