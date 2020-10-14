#ifndef _FS_ACTION_IMP_H_
#define _FS_ACTION_IMP_H_

#include "../../include/fs_action.h"
#include "../fxcore.h"
#include "fs_pdfdocImp.h"

class FSDestinationImp : public FSDestination, public CFX_Object
{
public:
	virtual void		release();
	virtual FS_INT32	getPageIndex() const;
	virtual FS_ZOOMMODE	getZoomMode() const;
	virtual FS_FLOAT	getLeft() const;
	virtual FS_FLOAT	getTop() const;
	virtual FS_FLOAT	getRight() const;
	virtual FS_FLOAT	getBottom() const;
	virtual FS_FLOAT	getZoomFactor() const;

	//For internal use
	static FSDestinationImp* create(FS_ZOOMMODE zoomMode, FSPDFPage* page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom, FS_FLOAT zoomFactor);
	static FSDestination* createFromPDFDest(FSPDFDoc* pFSPDFDoc, CPDF_Dest pdfDest);

	CPDF_Array*			getDestArray() { return m_pDestArray; }
	FSDestinationImp*	retain();
protected:
	FSDestinationImp();
    virtual ~FSDestinationImp();
private:
	CPDF_Array*		m_pDestArray;
	FS_INT32		m_pageIndex;

	FSCRT_DEFINELOCK(m_lock);
	FS_INT32		m_nCountRef;
};

class FSActionImp: public FSAction, public CFX_Object
{
public:

	virtual	FS_ACTIONTYPE	getType();
	virtual	FS_INT32		getSubActionCount();
	virtual FSAction*		getSubAction(FS_INT32 index);
	virtual void			release();
	virtual void			setSubAction(FS_INT32 index, FSAction* subAction);
	virtual FS_BOOL			insertSubAction(FS_INT32 index, FSAction* subAction);
	virtual FS_BOOL			removeSubAction(FS_INT32 index);
	virtual FS_BOOL			removeAllSubActions();

	//Internal use
	static FSAction*		create(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict, FS_BOOL bCreateUnsupport);
	static FSAction*		retain(FSAction* pOrgAct);

	FSActionImp();
	void				init(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict);
	CPDF_Dictionary*	getActDict()	{ return m_pActiontDict; }
	FSPDFDoc*			getFSPDFDocument()	{ return m_pFSPDFDoc; }
	void				setModified() { ((FSPDFDocImp*)m_pFSPDFDoc)->setModified();}
	//FSAction*			retain();
	FS_BOOL				initSubActionArray();

public:
	FSCRT_DEFINELOCK(m_lock);
	FS_INT32		m_nCountRef;

protected:
    	virtual ~FSActionImp();
private:
	CPDF_Dictionary*		m_pActiontDict;
	FSPDFDoc*				m_pFSPDFDoc;

	FSCRT_DEFINELOCK(m_lockSubActArray);
	CFX_ArrayTemplate<void*>*	m_pSubActionArray;
    
    void                    destroy();

};

template<typename IAction> class tFSActionImp : public IAction, public FSActionImp 
{
public:
	virtual ~tFSActionImp(){}
	virtual	FS_ACTIONTYPE	getType(){ return FSActionImp::getType(); }
	virtual	FS_INT32		getSubActionCount() { return FSActionImp::getSubActionCount(); }
	virtual FSAction*		getSubAction(FS_INT32 index) { return FSActionImp::getSubAction(index); }
	virtual void			release() { FSActionImp::release(); }
	virtual void			setSubAction(FS_INT32 index, FSAction* subAction) { FSActionImp::setSubAction(index, subAction); }
	virtual FS_BOOL			insertSubAction(FS_INT32 index, FSAction* subAction) { return FSActionImp::insertSubAction(index, subAction); }
	virtual FS_BOOL			removeSubAction(FS_INT32 index) { return FSActionImp::removeSubAction(index); }
	virtual FS_BOOL			removeAllSubActions() { return FSActionImp::removeAllSubActions(); }
};

class FSGotoActionImp: public tFSActionImp<FSGotoAction>
{
public:
	virtual	FS_ACTIONTYPE	getType();
//	virtual void			release();

	virtual	FSDestination*	getDestination();
	virtual void			setDestination(FSDestination* dest);

	// For internal use
	static FSGotoAction*	createFromDestination(FSPDFDoc* pFSPDFDoc, CPDF_Object* pDestObj);
	FSGotoActionImp();
	void				init(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict);
	FS_BOOL				init(FSPDFDoc* pFSPDFDoc, CPDF_Dest pdfDest);
	FSActionImp*		getActionImp()	{ return this; }
	FSGotoAction*		retain();
    
protected:
    virtual ~FSGotoActionImp();
private:
	FSDestination*		m_pDestination; // Temporarily, this is only used with init(FSPDFDoc* pFSPDFDoc, CPDF_Dest pdfDest).
};

class FSURIActionImp: public tFSActionImp<FSURIAction>
{
public:
	virtual	FS_ACTIONTYPE	getType();
//	virtual void			release();

	virtual	FSString	getURI();
	virtual	void		setURI(const char* uri);
	virtual	FS_BOOL		isTrackPosition();
	virtual	void		setTrackPositionFlag(FS_BOOL isTrackPosition);

	FSURIActionImp();
	void				init(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict);
	FSActionImp*		getActionImp()	{ return this; }
	FSURIAction*		retain();
protected:
    virtual ~FSURIActionImp();
};

#endif //_FS_ACTION_IMP_H_
