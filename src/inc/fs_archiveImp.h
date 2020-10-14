#ifndef _FS_ARCHIVE_IMP_H_
#define _FS_ARCHIVE_IMP_H_

#include "../../include/fs_pdfpage.h"

class CFS_Archive;

class FSArchiveImp : public FSArchive, public CFX_Object
{
public:
	FSArchiveImp();
	virtual void release();
	virtual ~FSArchiveImp();
	virtual FS_BOOL		loadData(const char* data);
	virtual FS_BOOL		serializeAnnot(FSAnnot* annot);
	virtual FSAnnot*	deserializeAnnot(FSPDFPage* page);
	virtual FSString	getData() const;

	//Internal use
	FS_BOOL			initialize();

private:
	CFS_Archive*	m_pSTArchive;
};

#endif //_FS_ARCHIVE_IMP_H_