#ifndef _FS_OFDBLOCKOBJECT_H_
#define _FS_OFDBLOCKOBJECT_H_

class CFS_OFDLayer;

class CFS_OFDBlockObject : public CFS_OFDLayer
{
public:
	CFS_OFDBlockObject();
	~CFS_OFDBlockObject();

	FX_BOOL Create(CFS_OFDPage* pPage, COFD_BlockObject* pBlock = NULL);
	FX_BOOL Create(CFS_OFDPage* pPage, COFD_WriteBlockObject* pWriteBlock);
	FX_BOOL Create(COFD_WriteBlockObject* pWriteBlock);

	COFD_WriteBlockObject* GetWriteBlockObject();

	CFS_OFDBlockObject* AddBlockObject();

protected:
	CFX_PtrList* m_pBlockObjs;
};

#endif //_FS_OFDBLOCKOBJECT_H_
