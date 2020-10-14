#ifndef _FS_OFDCOMPOSITEOBJECT_H_
#define _FS_OFDCOMPOSITEOBJECT_H_

class CFS_OFDContentObject;
class CFS_OFDCompositeObject;
class CFS_OFDLayer;
class CFS_OFDBlockObject;

class CFS_OFDCompositeUnit : public CFX_Object
{
public:
	CFS_OFDCompositeUnit(CFS_OFDCompositeObject* pUser);
	~CFS_OFDCompositeUnit();

	CFS_OFDBlockObject* AddBlock();
	COFD_WriteCompositeUnit *GetWriteCompositeUnit();
protected:
	CFS_OFDCompositeObject* m_pUser;
	CFS_OFDBlockObject* m_pBlockObj;
	COFD_WriteCompositeUnit* m_pWriteCompositeUnit;

	friend class CFS_OFDCompositeObject;
};

class CFS_OFDCompositeObject : public CFS_OFDContentObject
{
public:
	CFS_OFDCompositeObject();
	~CFS_OFDCompositeObject();

	CFS_OFDCompositeUnit* AddUnit();

	IOFD_WriteDocument* GetWriteDoc();

protected:
	virtual OFD_CONTENTTYPE GetWriteContentType();

	CFS_OFDCompositeUnit* m_pUnit;
};

#endif //_FS_OFDCOMPOSITEOBJECT_H_
